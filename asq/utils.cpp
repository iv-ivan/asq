//
//  utils.cpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#include "utils.hpp"

TEnsureError::TEnsureError(const std::string file_, const size_t lineNumber_) : file(file_), lineNumber(lineNumber_) {}

void MyEnsure(const bool clause, const char* fileName, const size_t lineNumber) {
    if (!clause) {
        throw TEnsureError(std::string(fileName), lineNumber);
    }
}
