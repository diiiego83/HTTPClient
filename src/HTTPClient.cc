/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#include "HTTPClientIMPL.h"

#include <iostream> // TODO

namespace httpclient {

HTTPClient::HTTPClient(): _impl(new HTTPClientIMPL()) { }

HTTPClient::~HTTPClient() { }

// void HTTPClient::set_url(const std::string& url) { 
//     _impl->_url = url; 
// }

// void HTTPClient::add_header(const std::string& key, const std::string& value) {
//     _impl->_headers.emplace(key, value);
// }

void HTTPClient::set_response_buffer(std::vector<char>* response_buffer) {
    _impl->_res_buffer_external = response_buffer;
}

std::vector<char> HTTPClient::get_resonse_buffer() const {
    return _impl->_res_buffer_default;
}

void HTTPClient::get(const HTTPOptions& opt) {
    _impl->send(opt);
}

void HTTPClient::get(const std::string& url, 
         const std::unordered_map<std::string, std::string> headers) {
    _impl->send(url, headers);
}

bool HTTPClient::status_ok() const {
     return _impl->_status == HTTP_STATUS::OK; 
}

HTTP_STATUS HTTPClient::status() const {
     return _impl->_status; 
}

uint64_t HTTPClient::get_response_code() const {
    return _impl->_res_code;
}

std::string HTTPClient::status_message() const {
     return _impl->_statusMex; 
}

void HTTPClient::raise_for_status() const {
    if(!status_ok()) { throw std::runtime_error(_impl->_statusMex); }
}

void HTTPClient::raise_for_http_status() const {
    if(status()==HTTP_STATUS::HTTP_ERROR) {
         throw std::runtime_error(_impl->_statusMex);
    }
}

void HTTPClient::raise_for_client_status() const {
    if(status()==HTTP_STATUS::CLIENT_ERROR) { 
        throw std::runtime_error(_impl->_statusMex); 
    }
}

} // namespace httpclient
