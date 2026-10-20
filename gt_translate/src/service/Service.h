#include "../../include/include.h"
#include "../dominal_service/DominalService.h"
#include "../infrastructure/Helper/DTO.h"

//Business logic is almost entirely absent; hopefully, there will be some later on.

class Service 
{
    std::shared_ptr<IRepository> irepository;
    DTOUnits buffer_dtos;
    const Config& config;
    public:

    Service(std::shared_ptr<IRepository> irepository_,const Config& config_) : irepository{irepository_},config{config_}{

    }


    void load( DTOUnits& buffer_dtos_){
      buffer_dtos = std::move(buffer_dtos_);
    }

    void show(){
    for (const auto& dto : buffer_dtos) {
        std::cout << " msgid: " << dto.msgid << "\t msgstr: " << dto.msgstr << std::endl;
    }
    }

    std::string create_promt(){
    if(config.promt.empty()) throw(std::runtime_error("ERROR string is empty (promt)"));
    std::stringstream ss;
    ss << config.promt << ' ';
      for(const auto& dto : buffer_dtos){
        ss << dto.msgid << config.text_delim;
      }
      return ss.str();
    }

    void validate_and_safe(const std::string& text){
    if(text.empty()) throw(std::runtime_error("ERROR string is empty (response_text)"));
    std::vector<std::string> msgstrs;

    int start_pos_delim = 0;
    while(start_pos_delim < text.size()){
      int current_pos_delim = text.find_first_of(config.text_delim,start_pos_delim);
      std::string part = text.substr(start_pos_delim,current_pos_delim - start_pos_delim);
        if(current_pos_delim == std::string::npos){
      if(part.size()) msgstrs.push_back(part);
        break;
    }
      start_pos_delim = current_pos_delim;
      if(part.size()) msgstrs.push_back(part);

      ++start_pos_delim;
    }

    if(msgstrs.size() != buffer_dtos.size()) throw(std::runtime_error("ERROR the containers don't match. try changing the delimiter character"));
    auto it_msgstrs = msgstrs.begin();
    auto it_dtos = buffer_dtos.begin();
    while(it_msgstrs != msgstrs.end() && it_dtos != buffer_dtos.end()){
      (*it_dtos).msgstr = (*it_msgstrs);
    it_msgstrs++;
    it_dtos++;
    }

    irepository->add_translate_unit(buffer_dtos);
    }



};
