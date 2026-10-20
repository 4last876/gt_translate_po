#include "../include/include.h"
#include "infrastructure/GetText/GetTextManager.h"
#include "infrastructure/ConfigParser.h"
#include "infrastructure/NetworkTransfer.h"
#include "infrastructure/NetworkResponseParser.h"
#include "dominal_model/DominalModel.h"
#include "dominal_service/DominalService.h"
#include "service/Service.h"

void print_config(const Config& config){
std::cout << "=================== CONFIGURATION ===================\n";
std::cout << std::left;
std::cout << std::setw(18) << "API Key:"         << config.api_key << "\n";
std::cout << std::setw(18) << "Model Name:"      << config.model_name << "\n";
std::cout << std::setw(18) << "Temperature:"     << config.temperature << "\n";
std::cout << std::setw(18) << "Input File:"      << config.filename_input << "\n";
std::cout << std::setw(18) << "JSON File:"       << config.filename_json << "\n";
std::cout << std::setw(18) << "Output File:"     << config.filename_output << "\n";
std::cout << std::setw(18) << "Prompt:"          << config.promt << "\n";
std::cout << std::setw(18) << "Delim text:"      << config.text_delim << "\n";
std::cout << "=====================================================\n";
}

int main(){
    std::shared_ptr<IConfigParser> cp = std::make_shared<JsonConfigParser>("config.json");
    cp->read();
    cp->validate();
    Config config = cp->load();
    print_config(config);

    std::shared_ptr<TranslateUnitCatalog> ct = std::make_shared<TranslateUnitCatalog>();
    std::shared_ptr<IRepository> cr = std::make_shared<СoncreteRepository>(ct);
    std::shared_ptr<Service> sv = std::make_shared<Service>(cr,config);

    std::shared_ptr<IGetTextManager> gm = std::make_shared<ConcreteGetTextManager>();
    std::shared_ptr<INetworkTransfer> nt = std::make_shared<CurlTransfer>();

    gm->set_name_input_file(config.filename_input);
    gm->set_name_output_file(config.filename_output);

    {
    auto dtos = gm->ExportChanges();
    // convenient viewing
    FileManager::write(config.filename_json,JsonView::serialization_dtos_to_json(dtos));
    sv->load(dtos);
    }

    {
    NetRequest net_request;
    FinishedRequest finished_request = JsonGeminiNetworkParser::create_request(sv->create_promt(),config);
    net_request.body = finished_request.text;
    net_request.headers.push_back("x-goog-api-key: " + config.api_key);
    net_request.headers.push_back("Content-Type: application/json");
    net_request.url = "https://generativelanguage.googleapis.com/v1beta/interactions";
    
    auto raw_response = nt->post(net_request);

    if(!raw_response.has_value()) throw(std::runtime_error("ERROR No response was received."));
    auto response = JsonGeminiNetworkParser::parse_response(raw_response.value());
    std::cout << "this is an finish answer : " << response.text << std::endl;
    sv->validate_and_safe(response.text);
    sv->show();
    }

    {
    auto dtos = cr->get_translate_units();
    gm->UploadChanges(dtos);
    // convenient viewing
    FileManager::write(config.filename_json,JsonView::serialization_dtos_to_json(dtos));
    }
}