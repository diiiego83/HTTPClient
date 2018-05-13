/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#include "../src/HTTPClient.h"

#include <iostream>

using namespace httpclient;

void inline line() {
    std::cout<<"----------------------------------------------------------------------\n";
}

// Perform a simple http get request
void get() {

    HTTPClient http;
    http.get("https://httpstat.us/200", {{"Accept", "application/json"}});
    http.raise_for_status();

    std::cout<<"   - Response code: "<<http.get_response_code()<<'\n';
    std::cout<<"   - Response body: "<<http.get_resonse_message()<<'\n';
}

// Perform a simple http get request
// Request parameters set through an external option object   
void get_opt() {

    HTTPOptions opt;
    opt.url = "https://httpstat.us/200";
    opt.headers["Accept"] = "application/json";

    HTTPClient http;
    http.get(opt);
    http.raise_for_status();

    std::cout<<"   - Response code: "<<http.get_response_code()<<'\n';
    std::cout<<"   - Response body: "<<http.get_resonse_message()<<'\n';

}

// Perform a simple http get request multiple times
void get_repeat() {

    HTTPOptions opt;
    opt.url = "https://httpstat.us/200";
    opt.headers["Accept"] = "application/json";

    HTTPClient http;    
    for(auto ii=0; ii<4; ii++) {

        http.get(opt);
        http.raise_for_status();

        std::cout<<"   - Response code: "<<http.get_response_code()<<'\n';
    }

    // change a request parameter and repeat again
    opt.url = "https://httpstat.us/204";
    http.get(opt);      
    http.raise_for_status();

    std::cout<<"   - Response code: "<<http.get_response_code()<<'\n';       

}

// perform a simple http get request
// the response buffer is externally set
void get_response_external_buffer() {

    std::vector<char> mybuffer(1024);

    HTTPOptions opt;
    opt.url = "https://httpstat.us/200";
    opt.headers["Accept"] = "application/json";

    HTTPClient http;
    http.set_response_buffer(&mybuffer);
    http.get(opt);
    http.raise_for_status();    

    std::cout<<"   - Response code: "<<http.get_response_code()<<'\n';
    std::cout<<"   - Rsponse body: "<<mybuffer.data()<<'\n';    
}

// perform a simple http get request
// and retrieve the response headers
void get_response_headers() {

    HTTPClient http;
    http.enable_response_headers();
    http.get("https://httpstat.us/200");
    http.raise_for_status();

    auto headers = http.get_response_headers();
    for(auto &header: headers) {
        std::cout<<"   - "<<header.first<<": "<<header.second<<'\n';
    }
}

int main(int argc, char* argv[]) {

    try {

        line();
        get();

        line();
        get_opt();

        line();
        get_repeat();

        line();
        get_response_external_buffer();

        line();
        get_response_headers();

    } catch(std::runtime_error err) {
        std::cout<<"[ERROR] "<<err.what()<<'\n';
        line();
        return 1;
    }

    line();

    return 0;
}