/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#include "HTTPClientIMPL.h"

namespace httpclient {

HTTPClient::HTTPClient(): _impl(new HTTPClientIMPL()) { }

HTTPClient::~HTTPClient() { }

void HTTPClient::url(const std::string& url) { 
    _impl->_url = url; 
}

void HTTPClient::user_agent(const std::string& user_agent) {
    _impl->_userAgent = user_agent;
}

void HTTPClient::send() {
    _impl->send();
}

bool HTTPClient::ok() const {
     return _impl->_status == HTTP_STATUS::OK; 
}

bool HTTPClient::is_http_error() const {
     return _impl->_status == HTTP_STATUS::HTTP_ERROR; 
}

bool HTTPClient::is_client_error() const {
     return _impl->_status == HTTP_STATUS::CLIENT_ERROR; 
}

HTTP_STATUS HTTPClient::status() const {
     return _impl->_status; 
}

uint64_t HTTPClient::status_code() const {
    return _impl->_statusCode;
}

std::string HTTPClient::status_message() const {
     return _impl->_statusMex; 
}

void HTTPClient::raise_for_status() const {
    if(!ok()) { throw std::runtime_error(_impl->_statusMex); }
}

void HTTPClient::raise_for_http_status() const {
    if(is_http_error()) { throw std::runtime_error(_impl->_statusMex); }
}

void HTTPClient::raise_for_client_status() const {
    if(is_client_error()) { throw std::runtime_error(_impl->_statusMex); }
}

} // namespace httpclient
