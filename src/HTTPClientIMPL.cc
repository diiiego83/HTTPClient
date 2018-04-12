/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#include "HTTPClientIMPL.h"
#include "HTTPClientErrors.h"

#include <iostream> // [TODO]

namespace httpclient {

HTTPClient::HTTPClientIMPL::HTTPClientIMPL(): _libCurl(LibCurl::Load()) { }        

void HTTPClient::HTTPClientIMPL::send() {

    CURLcode ccode;
    auto CCOK = CURLcode::CURLE_OK;

    // reset the internal client status before perform the request
    reset_internal_status();

    // initialize a curl handler or reset the previous one
    // for consecutive call
    if(_curl.use_count() == 0) {
        _curl = _libCurl->init();
        if(!_curl) { throw std::runtime_error(CERROR_INIT); }
    } else {
        _libCurl->reset(_curl);
    }

    // set the user agent
    if(_libCurl->setopt(_curl, CURLOPT_USERAGENT, _userAgent.c_str()) !=  CCOK) {
        error_client(CERROR_USERAGENT); return;
    }    

    // set the url
    if(_url.empty()) { error_client(ERROR_URL); return; }
    if(_libCurl->setopt(_curl, CURLOPT_URL, _url.c_str()) !=  CCOK) {
        error_client(CERROR_URL); return;
    }

    // set the callback write function
    if(_libCurl->setopt(_curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(this)) != CCOK) {
        error_client(CERROR_WRITEDATA); return;
    }
    if(_libCurl->setopt(_curl, CURLOPT_WRITEFUNCTION, &HTTPClientIMPL::write_callback) != CCOK) {
        error_client(CERROR_WRITEDATA); return;
    }    

    // perform the http request
    if(_libCurl->perform(_curl) !=  CCOK) {
        error_client(CERROR_PERFORM); return;
    }

    // get the return http code
    if(_libCurl->getinfo(_curl, CURLINFO_RESPONSE_CODE, &_statusCode) != CCOK) {
        error_client(CERROR_INFOCODE); return;
    }

    // set the http status
    if(_statusCode > 299 ) {
        error_http(); return;
    }

}

void HTTPClient::HTTPClientIMPL::reset_internal_status() {

    _status = HTTP_STATUS::OK;
    _statusCode = 0;
    _statusMex = "";

    if(_res_buffer_type == BUFFER_TYPE::DEFAULT) {
        if(_res_buffer_default.capacity() > 0) {
            _res_buffer_default.clear();
        } else {
            _res_buffer_default.reserve(CURL_MAX_WRITE_SIZE);
        }
    } else {
        _res_buffer_default.shrink_to_fit();
    }
}

size_t HTTPClient::HTTPClientIMPL::write_callback(  char *ptr, 
                                                    size_t size, 
                                                    size_t nmemb, 
                                                    void *userdata) {

    size_t realsize = size * nmemb;
    auto client = reinterpret_cast<HTTPClient::HTTPClientIMPL*>(userdata);

    auto ccptr = reinterpret_cast<const char*>(ptr);
    if(client->_res_buffer_type == BUFFER_TYPE::DEFAULT) {
        client->_res_buffer_default.insert(client->_res_buffer_default.end(), ccptr, ccptr + realsize);
    } else {
        exit(1); // TODO
    }

  return realsize;

}

void HTTPClient::HTTPClientIMPL::error_client(const std::string& errormex) {
    _status = HTTP_STATUS::CLIENT_ERROR;
    _statusMex = errormex;
}

void HTTPClient::HTTPClientIMPL::error_http() {
    _status = HTTP_STATUS::HTTP_ERROR;
    _statusMex = "HTTPClient::" + std::to_string(_statusCode) + ", ";
    if(_res_buffer_type == BUFFER_TYPE::DEFAULT ) {
        _statusMex += (std::string(_res_buffer_default.begin(), _res_buffer_default.end()) + ".");
    } else {
        exit(1); // [TODO]
    }
}

} // namespace httpclient