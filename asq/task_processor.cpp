//
//  task_processor.cpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#include "task_processor.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <sstream>

std::shared_ptr<IContext> ITaskProcessor::GetContext() {
    return context;
}

std::string ITaskProcessor::GetNewTaskId() const {
    std::stringstream ss;
    ss << boost::uuids::random_generator()();
    return ss.str();
}
