/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#include "HTTPClientIMPL.h"
#include "HTTPClientErrors.h"

#include <algorithm> 

#include <iostream> // TODO

namespace httpclient {

HTTPClient::HTTPClientIMPL::HTTPClientIMPL(): _libCurl(LibCurl::Load()) { }        

void HTTPClient::HTTPClientIMPL::send(const std::string& url, 
    const std::unordered_map<std::string, std::string> headers) {

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

    // set the url
    if(url.empty()) { error_client(ERROR_URL); return; }
    if(_libCurl->setopt(_curl, CURLOPT_URL, url.c_str()) !=  CCOK) {
        error_client(CERROR_URL); return;
    }

    // set the headers
    curl_slist* headers_slist = nullptr;
    for(auto &header: headers) {
        headers_slist = _libCurl->slist_append(
            headers_slist, 
            (header.first + ": "+ header.second).c_str());
    }
    if(headers_slist) {
        if(_libCurl->setopt(_curl, CURLOPT_HTTPHEADER, headers_slist) !=  CCOK) {
            error_client(CERROR_URL); return;
        }
    }

    // set the callback header function
    if(_libCurl->setopt(_curl, CURLOPT_HEADERDATA, reinterpret_cast<void*>(this)) != CCOK) {
        error_client(CERROR_HEADERDATA); return;
    }
    if(_libCurl->setopt(_curl, CURLOPT_HEADERFUNCTION, &HTTPClientIMPL::header_callback) != CCOK) {
        error_client(CERROR_HEADERFUN); return;
    }

    // set the callback write function
    if(_libCurl->setopt(_curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(this)) != CCOK) {
        error_client(CERROR_WRITEDATA); return;
    }
    if(_libCurl->setopt(_curl, CURLOPT_WRITEFUNCTION, &HTTPClientIMPL::write_callback) != CCOK) {
        error_client(CERROR_WRITEFUN); return;
    }    

    // perform the http request
    if(_libCurl->perform(_curl) !=  CCOK) {
        error_client(CERROR_PERFORM); return;
    }

    // get the return http code
    if(_libCurl->getinfo(_curl, CURLINFO_RESPONSE_CODE, &_res_code) != CCOK) {
        error_client(CERROR_INFOCODE); return;
    }

    // set the http status
    if(_res_code > 299 ) {
        error_http(); return;
    }

    // clear headers
    if(headers_slist) {
        _libCurl->slist_free(headers_slist);
    }
}

void HTTPClient::HTTPClientIMPL::reset_internal_status() {

    _res_code = 0;

    _status = HTTP_STATUS::OK;
    _statusMex = "";    

    if(_res_buffer_external) {
        _res_buffer_default.shrink_to_fit();
        _res_buffer_external->clear();
    } else {
        if(_res_buffer_default.capacity() == 0) {
            _res_buffer_default.reserve(CURL_MAX_WRITE_SIZE);
        }
        _res_buffer_default.clear();
    }

    if(_res_headers_on) {
        _res_headers.clear();
    }

}

size_t HTTPClient::HTTPClientIMPL::header_callback( const void *ptr, 
                                                    size_t size, 
                                                    size_t nmemb, 
                                                    void *userdata) {
    size_t realsize = size * nmemb;
    auto ccptr = reinterpret_cast<const char*>(ptr);
    auto client = reinterpret_cast<HTTPClient::HTTPClientIMPL*>(userdata);

    std::string str{ccptr, ccptr+realsize};
    std::string key;
    std::string::size_type pos = 0;

    while ( ( pos = str.find ("\r\n", pos) ) != std::string::npos ) {
        str.erase ( pos, 2 );
    }

    if(str.size()>0) {

        if( ( pos = str.find(':') ) != std::string::npos ) {

        key = str.substr(0, pos);
        str = str.substr(pos+2);
        key.erase(key.begin(), std::find_if(key.begin(), key.end(), [](int ch) {
            return !std::isspace(ch);
        }));
        key.erase(std::find_if(key.rbegin(), key.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), key.end()); 
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
            return !std::isspace(ch);
        }));
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), str.end());      

        client->_res_headers.emplace(key, str);

        }
    }

    return realsize;                                                      
}

size_t HTTPClient::HTTPClientIMPL::write_callback(  const void *ptr, 
                                                    size_t size, 
                                                    size_t nmemb, 
                                                    void *userdata) {

    size_t realsize = size * nmemb;
    auto client = reinterpret_cast<HTTPClient::HTTPClientIMPL*>(userdata);

    auto ccptr = reinterpret_cast<const char*>(ptr);
    if(client->_res_buffer_external) {
        client->_res_buffer_external->insert(client->_res_buffer_external->end(), ccptr, ccptr + realsize);
    } 
    else {
        client->_res_buffer_default.insert(client->_res_buffer_default.end(), ccptr, ccptr + realsize);
    }

    return realsize;

}

void HTTPClient::HTTPClientIMPL::error_client(const std::string& errormex) {
    _status = HTTP_STATUS::CLIENT_ERROR;
    _statusMex = errormex;
}

void HTTPClient::HTTPClientIMPL::error_http() {
    _status = HTTP_STATUS::HTTP_ERROR;
    _statusMex = "HTTPClient::" + std::to_string(_res_code) + ", ";
    if(_res_buffer_external) {
        _statusMex += (std::string(_res_buffer_external->begin(), _res_buffer_external->end()) + ".");
    } 
    else {
        _statusMex += (std::string(_res_buffer_default.begin(), _res_buffer_default.end()) + ".");
    } 
}

} // namespace httpclient