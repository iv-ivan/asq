//
//  asq.hpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#ifndef asq_hpp
#define asq_hpp
#include <memory>
#include "schedule_policy.hpp"
#include "task.hpp"
#include "task_processor.hpp"

class TAsq final {
public:
    void ScheduleTask(std::shared_ptr<ITask> task, std::shared_ptr<ISchedulePolicy> schedulePolicy) const;
    TAsq(std::shared_ptr<ITaskProcessor> taskProcessorPtr);
    ~TAsq() = default;
private:
    std::shared_ptr<ITaskProcessor> taskProcessorPtr;
};

#endif /* asq_hpp */
