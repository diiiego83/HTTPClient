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

void get() {

    std::string url("https://httpstat.us/200");
    
    print_line();
    std::cout<<" * Performes an http get call to "+url+"\n";

    HTTPClient http;
    http.set_url(url);
    http.send();
    http.raise_for_status();

}

void get_repeat() {

    std::string url("https://httpstat.us/200");
    
    print_line();
    std::cout<<" * Performes 5 http get call to "+url+"\n";
    std::cout<<" * Reuse the same client handler\n";

    HTTPClient http;
    http.set_url(url);
    for(auto ii=0; ii<5; ii++) {
        http.send();
        http.raise_for_status();
    }         

}

void get_response() {

    std::string url("https://httpstat.us/200");
    
    print_line();
    std::cout<<" * Performes an http get call to "+url+"\n";
    std::cout<<" * Get the response from the default response buffer\n";

    HTTPClient http;
    http.set_url(url);
    http.send();
    http.raise_for_status();
    auto res = http.get_resonse_buffer();

    std::cout<<" - "<<res.data()<<'\n';    

}

void get_response_external_buffer() {

    std::string url("https://httpstat.us/200");
    
    print_line();
    std::cout<<" * Performes an http get call to "+url+"\n";
    std::cout<<" * Get the response by setting an external buffer\n";

    HTTPClient http;
    http.set_url(url);
    std::vector<char> response(1024);
    http.set_response_buffer(&response);
    http.send();
    http.raise_for_status();
    
    std::cout<<" - "<<response.data()<<'\n';    

}

int main(int argc, char* argv[]) {

    try {

        get();

        get_repeat();

        get_response();

        get_response_external_buffer();

    } catch(std::runtime_error err) {
        std::cout<<"[ERROR] "<<err.what()<<'\n';
        print_line();
        return 1;
    }

    print_line();

    return 0;
}