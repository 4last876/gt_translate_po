#pragma once
#include "../Helper/json.hpp"
#include "DTO.h"

using DTOUnits = std::vector<DTOUnit>;

class JsonView{

public:

    static DTOUnits serialization_json_to_dtos(const std::string& json_string){

    DTOUnits dto_container;
    if(json_string.empty()) throw(std::runtime_error("ERROR string is json_string"));

    try{

    nlohmann::json json = nlohmann::json::parse(json_string);

    for (const auto& elem : json)
    {
        
        nlohmann::json obj = nlohmann::json::parse(elem.dump());

        std::string msgid = obj.at("msgid").get<std::string>();
        std::string msgstr = obj.at("msgstr").get<std::string>();
        dto_container.emplace_back(msgid,msgstr);

    }

    }catch(std::exception &e){
    std::cerr <<e.what() << std::endl;
    throw(std::runtime_error("fill in the JSON configuration correctly"));
    }

    return dto_container;
    }



    static std::string serialization_dtos_to_json(DTOUnits& dtos){
    nlohmann::json json_array;
    try{

     json_array = nlohmann::json::array();
     for(const DTOUnit& dto : dtos){
        nlohmann::json json;
        json["msgid"] = dto.msgid;
        json["msgstr"] = dto.msgstr;
        json_array.push_back(json);
     }

    }catch(std::exception &e){
    std::cerr <<e.what() << std::endl;
    throw(std::runtime_error("fill in the JSON configuration correctly"));
    }
        return json_array.dump(2);
   }
};