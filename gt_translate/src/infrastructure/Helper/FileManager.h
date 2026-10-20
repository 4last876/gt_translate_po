#pragma once
#include "../../../include/include.h"

class FileManager{

    public:

    static bool is_exist(const std::string& file_input_name){
      std::ifstream ifs {file_input_name};
      if(ifs.is_open()){
        ifs.close();
        return true;
      }else{
        return false;
      }
    }

    static std::string read(const std::string& file_input_name){
      std::ifstream ifs {file_input_name};
      std::string buffer;
      if(!ifs.is_open()) throw(std::runtime_error("ERROR File not open"));

      std::copy(std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>(),std::insert_iterator<std::string>(buffer,std::begin(buffer)));

      ifs.close();
      return buffer;
    }

    static void write(const std::string& file_input_name,const std::string& input_text){
    std::ofstream ofs {file_input_name};
    ofs.write(input_text.c_str(),input_text.size());
    ofs.close();
    }

};