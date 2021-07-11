//
//  local_task_processor.hpp
//  asq
//
//  Created by Ivan Ivashkovskii on 10.07.2021.
//

#ifndef local_task_processor_hpp
#define local_task_processor_hpp

#include <unordered_map>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include "task_processor.hpp"
#include <thread>

class TLocalTaskProcessor : public ITaskProcessor {
public:
    TLocalTaskProcessor(const size_t threadPoolSize);
    virtual ~TLocalTaskProcessor();
    void Reschedule(const ITask* task, std::shared_ptr<ISchedulePolicy> schedulePolicy) override;
    void Schedule(std::shared_ptr<ITask> task, std::shared_ptr<ISchedulePolicy> schedulePolicy) override;
    
private:
    void RunJobs();
    void RunJob(const std::string& taskId);
    std::unordered_map<std::string, std::shared_ptr<ITask>> taskRegistry;
    std::unordered_map<std::string, int> taskScheduleCount;
    std::unordered_map<std::string, std::shared_ptr<IRunStatistics>> tasksRunStatistics;
    std::multimap<std::chrono::time_point<std::chrono::system_clock>, std::string> runQueue;
    
    boost::asio::thread_pool pool;
    std::thread jobRunner;
    
    bool stopped = false;
    std::mutex runJobsMutex; //can be better - several mutexes
    std::mutex taskScheduleCountMutex;
    std::mutex taskRegistryMutex;
    std::condition_variable runJobsCV;
    // TODO - file with dump on disk
    // can recover from this disk file
    // can print current state - all tasks with next schedule and params
    // write logs in file
};

#endif /* local_task_processor_hpp */
