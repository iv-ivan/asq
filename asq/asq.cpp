//
//  asq.cpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#include "asq.hpp"
#include "utils.hpp"

TAsq::TAsq(std::shared_ptr<ITaskProcessor> taskProcessorPtr_) : taskProcessorPtr(taskProcessorPtr_) {}

void TAsq::ScheduleTask(std::shared_ptr<ITask> task, std::shared_ptr<ISchedulePolicy> schedulePolicy) const {
    MY_ENSURE(task != nullptr);
    MY_ENSURE(taskProcessorPtr != nullptr);

    task->AttachToProcessor(taskProcessorPtr);
    taskProcessorPtr->Schedule(task, schedulePolicy);
}
