//
//  task.cpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#include "task.hpp"
#include "utils.hpp"
#include "task_processor.hpp"

void ITask::AttachToProcessor(std::shared_ptr<ITaskProcessor> taskProcessorPtr_) {
    MY_ENSURE(taskProcessorPtr_ != nullptr);
    taskProcessorPtr = taskProcessorPtr_;
    context = taskProcessorPtr->GetContext();
    taskId = taskProcessorPtr->GetNewTaskId();
}

void ITask::Reschedule(std::shared_ptr<ISchedulePolicy> schedulePolicy) const {
    MY_ENSURE(schedulePolicy != nullptr);
    taskProcessorPtr->Reschedule(this, schedulePolicy);
}

const std::string& ITask::TaskId() const {
    return taskId;
}

IParametrizedTask::IParametrizedTask(std::shared_ptr<ITaskParams> params_) : params(params_) {}

std::shared_ptr<ITaskParams> IParametrizedTask::GetParams() const {
    return params;
}
