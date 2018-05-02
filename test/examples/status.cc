/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#include "../../src/HTTPClient.h"

#include <iostream>

using namespace httpclient;

void inline line() {
    std::cout<<"----------------------------------------------------------------------\n";
}

// performs a simple http get request
// raise an exception if a problem occured in the http request
void client_status() {

    HTTPOptions opt;
    opt.headers["Accept"] = "application/json";

    HTTPClient http;   
    try {
        http.get(opt);         
        http.raise_for_client_status();
    } catch(std::runtime_error err) {
        std::cout<<" # "<<err.what()<<'\n';
    }

}

// performs a simple http get request
// raise an exception if the request return an http code != 1xx or != 2xx
void http_status() {

    HTTPOptions opt;
    opt.url = "https://httpstat.us/500";
    opt.headers["Accept"] = "application/json";

    HTTPClient http;
    http.get(opt);
    try {
        http.raise_for_http_status();
    } catch(std::runtime_error err) {
        std::cout<<" # "<<err.what()<<'\n';
    }

}

// performs a simple http get request
// raise an exception if a problem occured in the http request
// or if the request return an http code != 1xx or != 2xx
// in the example no excepion should be thrown
void client_http_status() {

    HTTPOptions opt;
    opt.url = "https://httpstat.us/200";
    opt.headers["Accept"] = "application/json";

    HTTPClient http;
    http.get(opt);    
    try {
        http.raise_for_status();
    } catch(std::runtime_error err) {
        std::cout<<" # "<<err.what()<<'\n';
        return;
    }
    std::cout<<" # No Exception has been trown\n";
}

int main(int argc, char* argv[]) {

    line();
    client_status();

    line();
    http_status();

    line();
    client_http_status();

    line();

    return 0;
}