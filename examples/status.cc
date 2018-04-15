/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#include "../src/HTTPClient.h"

#include <iostream>

using namespace httpclient;

void inline print_line() {
    std::cout<<"----------------------------------------------------------------------\n";
}

void client_status() {
    
    print_line();
    std::cout<<" * Raise for client error status, no url set\n";

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

void http_status() {

    print_line();
    std::cout<<" * Raise for http error status, 500 Internal Server Error\n";

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

void client_http_status() {

    print_line();
    std::cout<<" * Raise if client or http error status, should no raise here\n";

    HTTPOptions opt;
    opt.url = "https://httpstat.us/200";
    opt.headers["Accept"] = "application/json";

    HTTPClient http;
    http.get(opt);    
    try {
        http.raise_for_status();
    } catch(std::runtime_error err) {
        std::cout<<" # "<<err.what()<<'\n';
    }
}

int main(int argc, char* argv[]) {

    client_status();

    http_status();

    client_http_status();

    print_line();

    return 0;
}