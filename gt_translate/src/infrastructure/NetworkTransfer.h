#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include <curl/curl.h>

struct NetRequest
{
std::string body;
std::vector<std::string> headers;
std::string url;
};

struct NetResponse
{
long code = 0;
std::string body;
};



class INetworkTransfer{

    public:

    virtual std::optional<NetResponse> post(const NetRequest&) = 0;
    virtual ~INetworkTransfer() = default;

};

class CurlTransfer : public INetworkTransfer{

   static constexpr size_t data_reader(char *ptr, size_t size, size_t nmemb, void *userdata) 
    {
        int exact_size = size * nmemb;
        std::string *buffer = static_cast<std::string *>(userdata);
        buffer->append(ptr,exact_size);
        return exact_size;
    }

    public:


    std::optional<NetResponse> post(const NetRequest& request) override{

    NetResponse response;

    CURL *easy_handle = curl_easy_init();
    //curl_easy_setopt(easy_handle,CURLOPT_VERBOSE,1L);

    curl_easy_setopt(easy_handle, CURLOPT_URL, request.url.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, request.body.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, data_reader);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, static_cast<void*>(&response.body));

    struct curl_slist *list = nullptr;

    for(const auto& head : request.headers ){
        list = curl_slist_append(list, head.c_str());
    }
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, list);
    CURLcode code = curl_easy_perform(easy_handle);
    curl_slist_free_all(list); 

    if(code == CURLE_OK){
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response.code);
    curl_easy_cleanup(easy_handle);
       return response;
    }
    curl_easy_cleanup(easy_handle);
    return std::nullopt;
}

};
