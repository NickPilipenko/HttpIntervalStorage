#include "HttpException.hpp"

#include <boost/assign.hpp>

#include <map>

using namespace Http;

const char* INTERNAL_ERROR = "INTERNAL ERROR";

typedef std::map <int, const char*> HttpErrorMap;

const HttpErrorMap ERROR = boost::assign::map_list_of
                    (HE_BAD_REQUEST, "Bad Request")
                    (HE_NOT_ALLOWED, "Method Not Allowed")
                    (HE_NOT_IMPLEMENTED, "Not Implemented")
                    (HE_INTERNAL_ERROR, "Internal Server Error");

HttpException::HttpException(unsigned code = HE_INTERNAL_ERROR)
    : _code(code)
{
    HttpErrorMap::const_iterator a = ERROR.find(_code);
    if (a == ERROR.end())
        _code = HE_INTERNAL_ERROR;
}


HttpException::~HttpException() throw()
{}


unsigned HttpException::GetCode() const throw()
{
    return _code;
}


const char* HttpException::what() const throw()
{
    HttpErrorMap::const_iterator a = ERROR.find(_code);
    return a->second;
}
