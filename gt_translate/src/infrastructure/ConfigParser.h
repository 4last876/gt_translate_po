#pragma once
#include "../../include/include.h"
#include "Helper/FileManager.h"
#include "Helper/json.hpp"

struct Config{
    std::string filename_json;
    std::string filename_input;
    std::string filename_output;
    std::string promt;
    std::string api_key;
    std::string model_name;
    std::string text_delim;

    int temperature = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config,filename_json,filename_input,filename_output,promt,api_key,model_name,temperature,text_delim); 

class IConfigParser{

    public:
    virtual void validate() = 0;
    virtual void read() = 0;
    virtual Config load() = 0;
    virtual ~IConfigParser() = default;

};

class JsonConfigParser : public IConfigParser {

    std::string config_text;
    std::string name_file;

       void create_structure(){
        Config config{"","","","","","","",0};
        nlohmann::json config_structure = config;
        FileManager::write(name_file,config_structure.dump(2));
       }

        public:

        JsonConfigParser(std::string name_file_) : name_file{name_file_}{
        }

    void validate() override{
       try{
        nlohmann::json json = nlohmann::json::parse(config_text);
        json.at("api_key");
        json.at("filename_input");
        json.at("filename_json");
        json.at("filename_output");
        json.at("model_name");
        json.at("promt");
        json.at("text_delim");
        json.at("temperature");
       }catch(std::exception &e){
        std::cerr <<e.what() << std::endl;
        throw(std::runtime_error("fill in the JSON configuration correctly"));
       }

    }
    void read() override{
        if(!FileManager::is_exist(name_file)){ 
            create_structure();
            throw std::runtime_error("ERROR configuration file is not exist");
        }

        config_text = FileManager::read(name_file);
    }
    Config load() override{
        Config config;
        if(!config_text.empty()){
            nlohmann::json json = nlohmann::json::parse(config_text);
            config = json.get<Config>();
        }
        return config;
    }
};
