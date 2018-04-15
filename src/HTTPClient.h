/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <memory>
#include <vector>
#include <unordered_map>

namespace httpclient {

enum class HTTP_STATUS { OK, CLIENT_ERROR, HTTP_ERROR };

struct HTTPOptions { 
    std::string url;
    std::unordered_map<std::string, std::string> headers;
};

class HTTPClient {

public:

    HTTPClient();
    HTTPClient(const HTTPClient & rhs) = delete;
    HTTPClient& operator = (const HTTPClient& rhs) = delete;
    ~HTTPClient();

    void set_response_buffer(std::vector<char>* response_buffer);
    std::string get_resonse_message() const;
    uint64_t get_response_code() const;

    HTTP_STATUS status() const;
    std::string status_message() const;
    bool status_ok() const;

    void raise_for_status() const;
    void raise_for_http_status() const;
    void raise_for_client_status() const;

    void get(const HTTPOptions& opt);
    void get(const std::string& url, 
             const std::unordered_map<std::string, std::string> headers = 
                   std::unordered_map<std::string, std::string>());

private:

    class HTTPClientIMPL;
    std::unique_ptr<HTTPClientIMPL> _impl;

};

} // namespace httpclient

#endif