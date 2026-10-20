#pragma once
#include "../../include/include.h"
#include "Helper/json.hpp"


struct FinishedResponse
{
std::string text;
};

struct FinishedRequest
{
std::string text;
};

class JsonGeminiNetworkParser{

    public:

    static FinishedRequest create_request(const std::string& promt,const Config& config){
    FinishedRequest request;
    nlohmann::json req_json;
    req_json["model"] = config.model_name;
    req_json["input"] = promt;
    req_json["generation_config"]["temperature"] = config.temperature;
    request.text = req_json.dump(0);
    return request;
     }

    
    static FinishedResponse parse_response(const NetResponse& json_text ){
        FinishedResponse response;
        try {
        nlohmann::json data = nlohmann::json::parse(json_text.body);

        if(data.contains("error"))  throw std::runtime_error("ERROR The server returned an invalid response. Please check your input values ​​and VPN.");

        std::string ai_response = "";
        for (const auto& step : data["steps"]) {
            if (step.contains("type") && step["type"] == "model_output") {
                ai_response = step["content"][0]["text"];
                break;
            }
        }
        response.text = ai_response;
       }catch(std::exception &e){
        std::cerr <<e.what() << std::endl;
        throw(std::runtime_error("fill in the JSON configuration correctly"));
       }
    return response;
    }

};