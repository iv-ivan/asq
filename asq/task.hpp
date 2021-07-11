//
//  task.hpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#ifndef task_hpp
#define task_hpp
#include <memory>
#include <string>
#include "schedule_policy.hpp"


class TAsq;
class ITaskProcessor;
class IContext;
class IRunStatistics;

class ITask {
public:
    virtual void DoJob(std::shared_ptr<IRunStatistics> runStats) const = 0;
    virtual std::string TaskName() const = 0;
    
    const std::string& TaskId() const;
protected:
    void Reschedule(std::shared_ptr<ISchedulePolicy> schedulePolicy) const;
    std::shared_ptr<IContext> context;
private:
    void AttachToProcessor(std::shared_ptr<ITaskProcessor> taskProcessorPtr);
private:
    std::shared_ptr<ITaskProcessor> taskProcessorPtr;
    std::string taskId;
    friend class TAsq;
};


struct ITaskParams {
    virtual std::string Serialize() const = 0;
    virtual void Deserialize(const std::string& data) = 0;
};

class IParametrizedTask : public ITask {
public:
    IParametrizedTask(std::shared_ptr<ITaskParams> params);
    std::shared_ptr<ITaskParams> GetParams() const;
protected:
    std::shared_ptr<ITaskParams> params;
};

template<typename TTaskParams>
class TParametrizedTask : public IParametrizedTask {
public:
    TParametrizedTask(std::shared_ptr<ITaskParams> params);
    std::shared_ptr<TTaskParams> GetCurrentParams() const;
};

template<typename TTaskParams>
TParametrizedTask<TTaskParams>::TParametrizedTask(std::shared_ptr<ITaskParams> params) : IParametrizedTask(params) {}


template<typename TTaskParams>
std::shared_ptr<TTaskParams> TParametrizedTask<TTaskParams>::GetCurrentParams() const {
    return std::dynamic_pointer_cast<TTaskParams>(params);
}

#endif /* task_hpp */
