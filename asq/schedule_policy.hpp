//
//  schedule_policy.hpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#ifndef schedule_policy_hpp
#define schedule_policy_hpp

#include <chrono>

class ISchedulePolicy {
public:
    virtual std::chrono::time_point<std::chrono::system_clock> NextRunTimepoint() const = 0;
};

class TBasicSchedulePolicy : public ISchedulePolicy {
public:
    std::chrono::time_point<std::chrono::system_clock> NextRunTimepoint() const override;
    TBasicSchedulePolicy(std::chrono::time_point<std::chrono::system_clock> nextRun);
private:
    std::chrono::time_point<std::chrono::system_clock> nextRun;
};

#endif /* schedule_policy_hpp */
