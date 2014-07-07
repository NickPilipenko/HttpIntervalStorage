#include "HttpResponseBuilder.hpp"

#include <sstream>

const char *INIT_STR = "HTTP/1.0 ";
const char *OK_STR   = "200 OK";
const char *ALLOWED  = "Allow: ADD, SEARCH, REMOVE, OPTIONS";

using namespace Http;
using namespace std;


HttpResponseBuilder::HttpResponseBuilder()
{
}


string HttpResponseBuilder::Build(const HttpException &err) const throw()
{
    ostringstream _osstr;
    _osstr << INIT_STR << err.GetCode() << " " << err.what() << endl;
    if ((err.GetCode() == HE_NOT_ALLOWED) || (err.GetCode() == HE_NOT_IMPLEMENTED))
        _osstr << ALLOWED << endl;
    return _osstr.str();
}


string HttpResponseBuilder::Build(const string &result) const throw()
{
    ostringstream _osstr;
    _osstr << INIT_STR << OK_STR << endl << endl << result;
    return _osstr.str();
}


string HttpResponseBuilder::BuildOption() const throw()
{
    ostringstream _osstr;
    _osstr << INIT_STR << OK_STR << endl
           << ALLOWED << endl;
    return _osstr.str();
}
