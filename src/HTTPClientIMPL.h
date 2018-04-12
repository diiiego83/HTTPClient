/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#ifndef HTTP_CLIENT_IMPL_H
#define HTTP_CLIENT_IMPL_H

#include "HTTPClient.h"
#include "HTTPClientCURL.h"
#include <memory>

namespace httpclient {

class HTTPClient::HTTPClientIMPL{

public:

    HTTPClientIMPL();   
    void send();

private:

    void reset_internal_status();

    void error_client(const std::string& error);
    void error_http();

    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

private:

    std::shared_ptr<LibCurl>    _libCurl;
    std::shared_ptr<CURL>       _curl;

    std::string _userAgent = "httpclient/1.0"; // vscode bug if using init. list

    std::string _url;

    enum class BUFFER_TYPE { DEFAULT };
    BUFFER_TYPE _res_buffer_type{ BUFFER_TYPE::DEFAULT };
    std::vector<char> _res_buffer_default;

    HTTP_STATUS _status{HTTP_STATUS::OK};
    uint64_t _statusCode;
    std::string _statusMex;

    friend class HTTPClient;

};

} // namespace httpclient

#endif