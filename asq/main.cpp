//
//  main.cpp
//  asq
//
//  Created by Ivan Ivashkovskii on 11.07.2021.
//

#include "asq.hpp"
#include "local_asq/local_task_processor.hpp"
#include <chrono>
#include <thread>
#include <iostream>

using namespace std::literals;

struct TCoutParams : ITaskParams {
    TCoutParams(int number_) : number(number_) {};
    std::string Serialize() const override {return "";}; //TODO
    void Deserialize(const std::string& data) override {};
    int number;
};

class TCoutTask : public TParametrizedTask<TCoutParams> {
public:
    TCoutTask(std::shared_ptr<ITaskParams> params) : TParametrizedTask<TCoutParams>(params) {}

    virtual void DoJob(std::shared_ptr<IRunStatistics> runStats) const override {
        std::cout << GetCurrentParams()->number << std::endl;
        Reschedule(std::make_shared<TBasicSchedulePolicy>(std::chrono::system_clock::now() + 1s));
    }
    std::string TaskName() const override {return "cout_task";};
};

int main(int argc, const char * argv[]) {
    auto proc = std::make_shared<TLocalTaskProcessor>(2);
    TAsq asq(proc);
    
    asq.ScheduleTask(std::make_shared<TCoutTask>(std::make_shared<TCoutParams>(234)), std::make_shared<TBasicSchedulePolicy>(std::chrono::system_clock::now() + 5s));
    
    asq.ScheduleTask(std::make_shared<TCoutTask>(std::make_shared<TCoutParams>(0)), std::make_shared<TBasicSchedulePolicy>(std::chrono::system_clock::now() + 2s));
    std::this_thread::sleep_until(std::chrono::system_clock::now() + 10s);
    return 0;
}
