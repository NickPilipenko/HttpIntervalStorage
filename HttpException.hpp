#pragma once

#include <exception>

namespace Http {

enum eHttpError
{
    HE_BAD_REQUEST = 400,
    HE_NOT_ALLOWED = 405,
    HE_NOT_IMPLEMENTED = 501,
    HE_INTERNAL_ERROR = 500,
};

class HttpException
    : public std::exception
{
public:
    HttpException(unsigned code);
    virtual ~HttpException() throw();

    unsigned GetCode() const throw();
    virtual const char* what() const throw();
protected:
    unsigned _code;
};

} // namespace Http
