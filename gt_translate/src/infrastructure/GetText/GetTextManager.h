#pragma once
#include "../../../include/include.h"
#include "../Helper/DTO.h"
#include "../Helper/JsonView.h"
#include "../Helper/FileManager.h"
#include "GetTextWriter.h"
#include "GetTextParser.h"

class IGetTextManager{

    public:
    virtual void set_name_input_file(std::string) = 0;
    virtual void set_name_output_file(std::string) = 0;
    virtual void UploadChanges(DTOUnits&) = 0;
    virtual DTOUnits ExportChanges() = 0;
    virtual ~IGetTextManager() = default;

};

class ConcreteGetTextManager : public IGetTextManager{

    static constexpr std::string json_file = "data.json";
    std::string filename_in;
    std::string filename_out;

    std::shared_ptr<IGetTextWriter> itextwriter = std::make_shared<ConcreteGetTextWriter>();
    std::shared_ptr<IGetTextParser> itextparser = std::make_shared<ConcreteGetTextParser>();

    public:

    void set_name_input_file(std::string filename_in_) override{
        filename_in = filename_in_;
    }
    
    void set_name_output_file(std::string filename_out_) override{
        filename_out = filename_out_;
    }

    void UploadChanges(DTOUnits& dtos) override {
        try{
        if(filename_in.empty() && filename_out.empty()) throw std::runtime_error("ERROR file name not specified");
        itextwriter->set_name_input_file(filename_in);
        itextwriter->set_name_output_file(filename_out);
        itextwriter->validate();

        auto json_string = JsonView::serialization_dtos_to_json(dtos);
        
        FileManager::write(json_string,json_file);
        itextwriter->write(json_string);

        }catch(std::exception &e){
            std::cerr << e.what() << std::endl;
        }
    }

    DTOUnits ExportChanges() override {
        DTOUnits dtos;
        try{
        if(filename_in.empty() && filename_out.empty()) throw std::runtime_error("ERROR file name not specified");

        itextparser->set_name_input_file(filename_in);
        itextparser->validate();
        dtos = itextparser->parse();

        }catch(std::exception &e){
            std::cerr << e.what() << std::endl;
        }

        return dtos;
    }


};