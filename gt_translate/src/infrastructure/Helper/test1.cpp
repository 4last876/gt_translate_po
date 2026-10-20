#include "../../../include/include.h"
#include "FileManager.h"
#include "json.hpp"

struct Config{
    std::string filename_json;
    std::string filename_input;
    std::string filename_output;
    std::string promt;
    std::string api_key;
    std::string model_name;

    int temperature = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config,filename_json,filename_input,filename_output,promt,api_key,model_name,temperature); 

class IConfigParser{

    public:

    virtual void validate() = 0;
    virtual void read() = 0;
    virtual Config load() = 0;

};

class JsonConfigParser : public IConfigParser {

    std::string config_text;
    std::string name_file;

       void create_structure(){
        Config config{"","","","","","",0};
        nlohmann::json config_structure = config;
        FileManager::write(name_file,config_structure.dump(2));
       }

        public:

        JsonConfigParser(std::string name_file_){
            name_file = name_file_;
        }

    void validate() override{
        if(config_text.empty()) {
            throw std::runtime_error("ERROR read the file");
        }
       try{
        nlohmann::json json = nlohmann::json::parse(config_text);
        json.at("api_key");
        json.at("filename_input");
        json.at("filename_json");
        json.at("filename_output");
        json.at("model_name");
        json.at("promt");
        json.at("temperature");
       }catch(std::exception &e){
        throw(e);
       }

    }
    void read() override{
        if(!FileManager::is_exist(name_file))
         create_structure();

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
