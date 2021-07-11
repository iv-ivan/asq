//
//  task_processor.hpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#ifndef task_processor_hpp
#define task_processor_hpp

#include "task.hpp"
#include <memory>

class IContext;

class ITaskProcessor {
public:
    virtual void Reschedule(const ITask* task, std::shared_ptr<ISchedulePolicy> schedulePolicy) = 0;
    virtual void Schedule(std::shared_ptr<ITask> task, std::shared_ptr<ISchedulePolicy> schedulePolicy) = 0;
    virtual std::string GetNewTaskId() const;
    std::shared_ptr<IContext> GetContext();
protected:
    std::shared_ptr<IContext> context;
};

#endif /* task_processor_hpp */
