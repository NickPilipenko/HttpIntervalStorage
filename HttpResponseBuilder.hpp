#pragma once

#include "HttpException.hpp"

#include <string>

namespace Http {

class HttpResponseBuilder
{
    public:
        HttpResponseBuilder();
        std::string Build(const HttpException &err) const throw();
        std::string Build(const std::string & result) const throw();
        std::string BuildOption() const throw();

//        void AddResponse(const std::string &);
//        void SetResponse(const std::string &);
//........viod ResetResponse()
    private:
//        std::string _result;
};

} // namespace Http
