#pragma once
#include "../../include/include.h"

struct TranslateUnit
{
    std::string msgid;
    std::string msgstr;
};

class TranslateUnitCatalog{

    std::vector<TranslateUnit> translate_units;

    public:
    
    void add_unit(const std::string& msgid, const std::string& msgstr){
        translate_units.emplace_back(msgid,msgstr);
    }

    void delete_unit(std::string& msgid, std::string& msgstr){

        TranslateUnit translate_unit{msgid,msgstr};
        
        auto it = std::find_if(std::begin(translate_units),std::end(translate_units),[&](const auto& fv){

            return fv.msgid == translate_unit.msgid && fv.msgstr == translate_unit.msgstr;
        });

        if(it == std::end(translate_units)) return;

        translate_units.erase(it);
    
    }

     std::optional<TranslateUnit> get_unit(std::string& msgid, std::string& msgstr){

        TranslateUnit translate_unit{msgid,msgstr};
        
        auto it = std::find_if(std::begin(translate_units),std::end(translate_units),[&](const TranslateUnit& tu){
            return tu.msgid == translate_unit.msgid && tu.msgstr == translate_unit.msgstr;
        });

        if(it == std::end(translate_units)) return std::nullopt;

        return translate_unit;
    
    }

    void clear(){
        translate_units.clear();
    }

    int get_size(){
        return translate_units.size();
    }

    auto begin(){
        return std::begin(translate_units);
    }

    auto end(){
        return std::end(translate_units);
    }
};


using TranslateUnits = std::vector<TranslateUnit>;