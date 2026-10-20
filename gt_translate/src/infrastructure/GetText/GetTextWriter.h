#pragma once
#include "../../../include/include.h"
#include "../Helper/FileManager.h"
#include "../Helper/JsonView.h"
#include "../Helper/DTO.h"

class IGetTextWriter{

    public:

    virtual void set_name_input_file(std::string) = 0;
    virtual void set_name_output_file(std::string) = 0;
    virtual void write(const std::string&) = 0;
    virtual void validate() = 0;
    virtual ~IGetTextWriter() = default;
};

class ConcreteGetTextWriter : public IGetTextWriter{

    std::string filename_in;
    std::string filename_out;
    po_file_t file;
    static constexpr char* EMPTY_FIEL = ""; 

     static constexpr void xerror (int severity, po_message_t message, const char *filename, size_t lineno, size_t column, int multiline_p, const char *message_text){
        if(message_text != nullptr){
            std::cerr <<"WHAT? "  <<  message_text << std::endl;
        }

        if(filename != nullptr){
            std::cerr << "WHERE? " << ' '<< filename << ' ' << 
            lineno << ' ' << column << std::endl;
        }

        if(severity == PO_SEVERITY_FATAL_ERROR){
            std::cerr << "FATAL_ERROR" << std::endl;
            throw(std::runtime_error("FATAL_ERROR see console"));
        }

        if(message_text != nullptr|| filename != nullptr){
             throw(std::runtime_error("ERROR it’s possible the wrong file is open"));
        }

     }

     static constexpr void xerror2 (int severity, po_message_t message1, const char *filename1, size_t lineno1, size_t column1, int multiline_p1, const char *message_text1, po_message_t message2, const char *filename2, size_t lineno2, size_t column2, int multiline_p2, const char *message_text2){
     }

     struct po_xerror_handler handler =
    {
         .xerror = xerror,
         .xerror2 = xerror2
    };

    public:

    void set_name_input_file(std::string filename_in_) override{
        filename_in = filename_in_;
        file = po_file_read (filename_in.c_str(), &handler);
    }
    
    void set_name_output_file(std::string filename_out_) override{
        filename_out = filename_out_;
    }

void write(const std::string& json_string) override
{
    auto dtos = JsonView::serialization_json_to_dtos(json_string);

    std::unordered_map<std::string, std::string> translations;
    translations.reserve(dtos.size());

    for (const auto& dto : dtos)
    {
        if (dto.msgid.empty() || dto.msgid == EMPTY_FIEL)
            continue;

        translations[dto.msgid] = dto.msgstr;
    }

    const char* const* domains = po_file_domains(file);

    if (domains != nullptr){

    for (const char* const* domainp = domains;
         *domainp != nullptr;
         ++domainp)
    {
        po_message_iterator_t iterator =
            po_message_iterator(file, *domainp);

        if (iterator == nullptr)
            continue;

        po_message_t message;

        while ((message = po_next_message(iterator)) != nullptr)
        {
            const char* raw_msgid = po_message_msgid(message);

            if (raw_msgid == nullptr)
                continue;

            std::string msgid(raw_msgid);

            auto it = translations.find(msgid);
        if(!FileManager::is_exist(filename_in)) throw(std::runtime_error("ERROR the file does not exist"));
            if (it != translations.end())
            {
                po_message_set_msgstr(
                    message,
                    it->second.c_str()
                );
            }
        }

        po_message_iterator_free(iterator);
    }

}

    po_file_write(file, filename_out.c_str(), &handler);

    po_file_free(file);
}


    void validate() override{
    po_file_check_all(file,&handler);
    }

};