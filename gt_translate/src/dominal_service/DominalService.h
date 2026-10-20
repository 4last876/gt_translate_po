#pragma once
#include "../../include/include.h"
#include "../dominal_model/DominalModel.h"
#include "../infrastructure/Helper/DTO.h"

class IRepository
{
    public:
    virtual void add_translate_unit(DTOUnits&) = 0;
    virtual DTOUnits get_translate_units() = 0;
    virtual void clear_translate_units() = 0;
    virtual ~IRepository() = default;

};


class СoncreteRepository : public  IRepository
{
std::shared_ptr<TranslateUnitCatalog> DominalModel;

    public:

    СoncreteRepository(std::shared_ptr<TranslateUnitCatalog> DominalModel_) : DominalModel{DominalModel_}{
    }

    void add_translate_unit(DTOUnits& units) override{
        for(const auto& unit : units){
            DominalModel->add_unit(unit.msgid,unit.msgstr);
        }
    }

    DTOUnits get_translate_units() override{
        DTOUnits dtos;
        std::transform(std::begin(*DominalModel),std::end(*DominalModel),std::insert_iterator<DTOUnits>(dtos,std::begin(dtos)),[](const TranslateUnit& tu) {
        return DTOUnit(tu.msgid,tu.msgstr);
    });
        return dtos;
    }

    void clear_translate_units() override{
        DominalModel->clear();
    }

};
