#include "../../../include/include.h"
#include "../Helper/FileManager.h"
#include "../Helper/JsonView.h"
#include "../Helper/DTO.h"

struct IGetTextParser
{
    public:

    virtual void set_name_input_file(std::string) = 0;
    virtual DTOUnits parse() = 0;
    virtual void validate() = 0;
    virtual ~IGetTextParser() = default;
};


class ConcreteGetTextParser : public IGetTextParser
{

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

    static constexpr char* EMPTY_FIEL = ""; 
    std::string filename_in;
    po_file_t file;

    public:

    void set_name_input_file(std::string filename_in_) override {
        filename_in = filename_in_;
        file = po_file_read (filename_in.c_str(), &handler);
    }
    DTOUnits parse() override{
        
    DTOUnits dtos;    

    const char * const *domains = po_file_domains (file);
    const char * const *domainp;

    if (domains != nullptr){

  for (domainp = domains; *domainp; domainp++)
    {

      po_message_t message = po_message_create();
      const char *domain = *domainp;
      po_message_iterator_t iterator = po_message_iterator (file, domain);

      if (iterator == nullptr)
        continue;

      while ((message = po_next_message (iterator)) != NULL)
        {
         const char* raw_msgid = po_message_msgid (message);
         const char* raw_msgstr = po_message_msgstr (message);

         if(raw_msgid == nullptr)
          continue;

         std::string msgid = raw_msgid;
         std::string msgstr = raw_msgstr;

          if(msgid == EMPTY_FIEL)
           continue;

          dtos.emplace_back(msgid,msgstr);
        } 
 
      po_message_iterator_free (iterator);
       }
    }
    po_file_free(file);

    return dtos;
    }
        
    void validate()override{
        if(!FileManager::is_exist(filename_in)) throw(std::runtime_error("ERROR the file does not exist"));
        po_file_check_all(file,&handler);
    }
};
