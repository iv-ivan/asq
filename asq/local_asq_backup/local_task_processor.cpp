//
//  local_task_processor.cpp
//  asq
//
//  Created by Ivan Ivashkovskii on 10.07.2021.
//

#include <iostream>
#include "local_task_processor.hpp"
#include "utils.hpp"

TLocalTaskProcessor::TLocalTaskProcessor(const size_t threadPoolSize) : pool(threadPoolSize) {
    jobRunner = std::thread(&TLocalTaskProcessor::RunJobs, this);
}

TLocalTaskProcessor::~TLocalTaskProcessor() {
    {
        std::lock_guard<std::mutex> lk(runJobsMutex);
        stopped = true;
    }
    runJobsCV.notify_one();
    jobRunner.join();
    pool.join();
}


void TLocalTaskProcessor::Reschedule(const ITask* task, std::shared_ptr<ISchedulePolicy> schedulePolicy) {
    MY_ENSURE(task != nullptr);
    MY_ENSURE(schedulePolicy != nullptr);
    MY_ENSURE(taskRegistry.find(task->TaskId()) != taskRegistry.end());

    {
        std::lock_guard<std::mutex> lk(taskScheduleCountMutex);
        ++taskScheduleCount[task->TaskId()];
    }
    {
        std::lock_guard<std::mutex> lk(runJobsMutex);
        runQueue.insert({schedulePolicy->NextRunTimepoint(), task->TaskId()});
    }
    runJobsCV.notify_one();
}

void TLocalTaskProcessor::Schedule(std::shared_ptr<ITask> task, std::shared_ptr<ISchedulePolicy> schedulePolicy) {
    MY_ENSURE(task != nullptr);
    MY_ENSURE(schedulePolicy != nullptr);
    //TODO - WAL on disk
    {
        std::lock_guard<std::mutex> lk(taskRegistryMutex);
        taskRegistry[task->TaskId()] = task;
    }
    {
        std::lock_guard<std::mutex> lk(taskScheduleCountMutex);
        ++taskScheduleCount[task->TaskId()];
    }
    {
        std::lock_guard<std::mutex> lk(runJobsMutex);
        runQueue.insert({schedulePolicy->NextRunTimepoint(), task->TaskId()});
    }
    runJobsCV.notify_one();
}

void TLocalTaskProcessor::RunJobs() {
    bool hasWaitUntil = false;
    std::chrono::time_point<std::chrono::system_clock> waitUntil;
    
    while (true) {
        if (!hasWaitUntil) {
            std::unique_lock<std::mutex> lk(runJobsMutex);
            runJobsCV.wait(lk, [this]{return this->stopped || !this->runQueue.empty();});
            if (stopped) {
                return;
            }
            
            hasWaitUntil = true;
            waitUntil = runQueue.begin()->first;
        }
        
        std::unique_lock<std::mutex> lk(runJobsMutex);
        runJobsCV.wait_until(lk, waitUntil, [this, waitUntil]{return this->stopped || waitUntil != runQueue.begin()->first;}); // wake up if new points added
        if (stopped) {
            return;
        }
        
        const auto firstTimePoint = runQueue.begin()->first;
        if (firstTimePoint != waitUntil) {
            waitUntil = firstTimePoint;
            continue;
        }
        
        auto range = runQueue.equal_range(firstTimePoint);
        for (auto i = range.first; i != range.second; ++i) {
            const std::string& taskId = i->second;
            boost::asio::post(pool, [this, taskId] {this->RunJob(taskId);});
            {
                std::lock_guard<std::mutex> lk(taskScheduleCountMutex);
                const int futureRunsCount = --taskScheduleCount[taskId];
                if (futureRunsCount <= 0) {
                    taskScheduleCount.erase(taskId);
                }
            }
        }
        runQueue.erase(firstTimePoint);
        hasWaitUntil = false;
    }
}

void TLocalTaskProcessor::RunJob(const std::string& taskId) {
    //TODO - tasksRunStatistics pass to RunJob
    taskRegistry[taskId]->DoJob(nullptr);
    {
        std::lock_guard<std::mutex> lk(taskScheduleCountMutex);
        const auto futureRunsIt = taskScheduleCount.find(taskId);
        if (futureRunsIt == taskScheduleCount.end()) {
            taskRegistry.erase(taskId);
            //tasksRunStatistics.erase(taskId);
        }
    }
}
