//
//  utils.hpp
//  asq
//
//  Created by Ivan Ivashkovskii on 09.07.2021.
//

#ifndef utils_hpp
#define utils_hpp

#include <exception>
#include <string>

class TEnsureError : public std::exception {
public:
    TEnsureError(const std::string file, const size_t lineNumber);
private:
    const std::string file;
    const size_t lineNumber;
};

void MyEnsure(const bool clause, const char* fileName, const size_t lineNumber);

#define MY_ENSURE(clause) MyEnsure(clause, __FILE__, __LINE__)

#endif /* utils_hpp */
