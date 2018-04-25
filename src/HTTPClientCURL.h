/*

 * Copyright (c) 2018 Diego Molteni, moltediego@gmail.com
 * SPDX-License-Identifier: MIT 

*/

#ifndef HTTP_CORE_H
#define HTTP_CORE_H

#include <curl/curl.h>
#include <memory>
#include <vector>
#include <mutex>

namespace {
    const uint16_t LIBCURL_MAX_CACHE_SIZE = 129;
}

namespace httpclient {

// libcurl wrapper class
class LibCurl {

private:

    // private constructor, the class can only be instantiate by 
    // calling the Load() static method. This private ctor 
    // initialize curl's resources
    LibCurl() { curl_global_init(CURL_GLOBAL_ALL); }

public:

    // non copyable class
    LibCurl(const LibCurl& rhs) = delete;
    LibCurl& operator = (const LibCurl &rhs) = delete;

    // the dctor, release curl's resources
    ~LibCurl() { curl_global_cleanup(); }

    static std::shared_ptr<LibCurl> Load() {

        // create a static libcurl object. this object is created once, 
        // constructor and destructor called once.
        // curl resource globally initialized in the ctor and released in the dctor.
        static std::shared_ptr<LibCurl> libcurl(new LibCurl());
        return libcurl;
    }

    std::shared_ptr<CURL> init() {

        // cache curl handlers!
        // initliaze and release a curl handlers is an expensive operation.
        
        // create the cache for save curl handlers
        static std::vector<std::shared_ptr<CURL>> _cache_handlers;
        static std::mutex _cache_mu;
        std::unique_lock<std::mutex> lock(_cache_mu);
        
        // check if there is a free curl handler in the cache.
        // if found, reset and return it (reuse curl handler)
        for(auto &handler: _cache_handlers) {
            if(handler.use_count()==1) {
                reset(handler);
                return handler;
            }
        }

        // create a new handler if no a free one can be found in the
        // cache or if the cache is empty
        std::shared_ptr<CURL> handler(::curl_easy_init(), [](CURL* curl) { 
            ::curl_easy_cleanup(curl); 
        });

        // add the new handler to the cache if not full
        if(_cache_handlers.size() < LIBCURL_MAX_CACHE_SIZE) {
            _cache_handlers.emplace_back(handler);
        }

        // return the new handler
        return handler;
    }

    void reset(std::shared_ptr<CURL>& curl) {
        ::curl_easy_reset(curl.get());
    }

    CURLcode setopt(std::shared_ptr<CURL> curl, CURLoption opt, const char* param) {
        return ::curl_easy_setopt(curl.get(), opt, param);
    }

    CURLcode setopt(std::shared_ptr<CURL> curl, CURLoption opt, void* param) {
        return ::curl_easy_setopt(curl.get(), opt, param);
    }

    CURLcode setopt(std::shared_ptr<CURL> curl, CURLoption opt, 
                    size_t (*fun)(const void *ptr, size_t size, size_t nmemb, void *userdata)) {
        return ::curl_easy_setopt(curl.get(), opt, fun);
    }    

    CURLcode setopt(std::shared_ptr<CURL> curl, CURLoption opt, curl_slist* list) {
        return ::curl_easy_setopt(curl.get(), opt, list);
    }

    CURLcode  perform(std::shared_ptr<CURL> curl) {
        return ::curl_easy_perform(curl.get());
    }

    CURLcode getinfo(std::shared_ptr<CURL> curl, CURLINFO  info, uint64_t* codep) {
        return ::curl_easy_getinfo(curl.get(), info, codep);
    }

    curl_slist *slist_append(curl_slist* list, const char* item) {
        return ::curl_slist_append(list, item);
    }

    void slist_free(curl_slist* list) {
        ::curl_slist_free_all(list);
    }

};

} // namespace httpclient

#endif