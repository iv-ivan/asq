//
//  schedule_policy.cpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#include "schedule_policy.hpp"

TBasicSchedulePolicy::TBasicSchedulePolicy(std::chrono::time_point<std::chrono::system_clock> nextRun_): nextRun(nextRun_) {}

std::chrono::time_point<std::chrono::system_clock> TBasicSchedulePolicy::NextRunTimepoint() const {
    return nextRun;
}
