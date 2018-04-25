
/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#ifndef HTTP_CLIENT_ERRORS_H
#define HTTP_CLIENT_ERRORS_H

#include <string>

namespace httpclient{

    const std::string ERROR_URL = "HTTPClient::Url, the url has not been set.";
    const std::string CERROR_INIT = "HTTPClient::LibCurl::Init, an error occurred initlizing a curl handler.";
    const std::string CERROR_USERAGENT = "HTTPClient::LibCurl::UserAgent, an error occurred setting the user agent.";    
    const std::string CERROR_URL = "HTTPClient::LibCurl::Url, an error occurred setting the url.";
    const std::string CERROR_HEADERDATA = "HTTPClient::LibCurl::HeaderData, an error occurred setting the callback header data object.";
    const std::string CERROR_HEADERFUN = "HTTPClient::LibCurl::HeaderFunction, an error occurred setting the callback function.";
    const std::string CERROR_WRITEDATA = "HTTPClient::LibCurl::WriteData, an error occurred setting the callback write data object.";
    const std::string CERROR_WRITEFUN = "HTTPClient::LibCurl::WriteFunction, an error occurred setting the callback function.";
    const std::string CERROR_PERFORM = "HTTPClient::LibCurl::Perform, an error occurred performing the http request.";
    const std::string CERROR_INFOCODE = "HTTPClient::LibCurl::Perform, ann error uccurred retriving the http return code.";

} // namespace httpclient

#endif // HTTP_CLIENT_ERRORS_H