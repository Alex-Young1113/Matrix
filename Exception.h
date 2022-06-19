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

class Multiply_DimensionsNotMatched : public Exception {

public:

    explicit Multiply_DimensionsNotMatched(const std::string &message) : Exception(message) {}

};

class Determinant_NotSquareMatrix : public Exception {

public:

    explicit Determinant_NotSquareMatrix(const std::string &message) : Exception(message) {}

};

class Cominor_CoordinateExceedsBounds : public Exception {

public:

    explicit Cominor_CoordinateExceedsBounds(const std::string &message) : Exception(message) {}

};

class Trace_NotSquareMatrix : public Exception {

public:

    explicit Trace_NotSquareMatrix(const std::string &message) : Exception(message) {}

};

class Addition_DimensionNotMatched : public Exception {

public:

    explicit Addition_DimensionNotMatched(const std::string &message) : Exception(message) {}

};

class ClassTypeNotSupport : public Exception {

public:

    explicit ClassTypeNotSupport(const std::string &message) : Exception(message) {}

};

class Inverse_NotSquareMatrix : public Exception {

public:

    explicit Inverse_NotSquareMatrix(const std::string &message) : Exception(message) {}

};

class Inverse_NotInvertible : public Exception {

public:

    explicit Inverse_NotInvertible(const std::string &message) : Exception(message) {}

};


#endif