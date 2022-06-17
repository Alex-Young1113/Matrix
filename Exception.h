#ifndef __EXCEPTIONS_H__

#define    __EXCEPTIONS_H__

#include <exception>
#include <string>
#include <utility>

class Exception : public std::exception {

public:

    explicit Exception(std::string message) : exception(), message(std::move(message)) {}


    virtual ~Exception(void) throw() {}


    inline std::string getMessage(void) const {
        return this->message;
    }


protected:

    std::string message;

};


class InvalidDimensionsException : public Exception {

public:

    explicit InvalidDimensionsException(const std::string &message) : Exception(message) {}

};


class InvalidCoordinatesException : public Exception {

public:

    explicit InvalidCoordinatesException(const std::string &message) : Exception(message) {}

};

class OperatorAddition_MatrixNotMatched : public Exception {

public:

    OperatorAddition_MatrixNotMatched(const std::string &message) : Exception(message) {}

};

#endif