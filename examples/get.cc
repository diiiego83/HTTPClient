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
    http.get(url, {{"Accept", "application/json"}});
    http.raise_for_status();

    std::cout<<" - Response code: "<<http.get_response_code()<<'\n';
    std::cout<<" - Response body: "<<http.get_resonse_message().data()<<'\n';

}


void get_opt() {

    std::string url("https://httpstat.us/200");
    
    print_line();
    std::cout<<" * Performes an http get call to "+url+"\n";
    std::cout<<" * Call parameters set as ext options\n";    

    HTTPOptions opt;
    opt.url = url;
    opt.headers["Accept"] = "application/json";

    HTTPClient http;
    http.get(opt);
    http.raise_for_status();
    std::cout<<" - Response code: "<<http.get_response_code()<<'\n';
    std::cout<<" - Response body: "<<http.get_resonse_message().data()<<'\n';

}

void get_repeat() {

    std::string url("https://httpstat.us/20");
    
    print_line();
    std::cout<<" * Performes 5 http get call to "+url+"x\n";
    std::cout<<" * Reuse the same client handler.\n";

    HTTPOptions opt;
    opt.url = url + "0";
    opt.headers["Accept"] = "application/json";

    HTTPClient http;    
    for(auto ii=0; ii<4; ii++) {     
        http.get(opt);
        http.raise_for_status();
        std::cout<<" - Response code: "<<http.get_response_code()<<'\n';
    }

    opt.url = url + "4";
    http.get(opt);      
    http.raise_for_status();
    std::cout<<" - Response code: "<<http.get_response_code()<<'\n';       

}

void get_response_external_buffer() {

    std::string url("https://httpstat.us/200");
    
    print_line();
    std::cout<<" * Performes an http get call to "+url+"\n";
    std::cout<<" * Get the response by setting an external buffer\n";

    std::vector<char> mybuffer(1024);

    HTTPOptions opt;
    opt.url = url;
    opt.headers["Accept"] = "application/json";

    HTTPClient http;
    http.set_response_buffer(&mybuffer);
    http.get(opt);
    http.raise_for_status();    

    std::cout<<" - Response code: "<<http.get_response_code()<<'\n';
    std::cout<<" - Rsponse body: "<<mybuffer.data()<<'\n';    
}

int main(int argc, char* argv[]) {

    try {

        get();

        get_opt();

        get_repeat();

        get_response_external_buffer();

    } catch(std::runtime_error err) {
        std::cout<<"[ERROR] "<<err.what()<<'\n';
        print_line();
        return 1;
    }

    print_line();

    return 0;
}