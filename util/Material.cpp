#include "Material.h"

const std::string Material::CLS = "materials";

Material Material::parse(const JsonBox::Value& val)
{
    Material m;

    Parser::checkObject(val, CLS);
    
    JsonBox::Object obj = val.getObject();
    Parser::checkOption(obj, CLS, "index_of_refraction", Parser::NUMBER, "DEFAULT_IND");
    Parser::checkOption(obj, CLS, "absorvance", Parser::NUMBER, "DEFAULT_ABS");
    Parser::checkOption(obj, CLS, "roughness", Parser::NUMBER, "DEFAULT_ROU");
    Parser::checkOption(obj, CLS, "emittance", Parser::NUMBER, "DEFAULT_EMI");
    Parser::checkOption(obj, CLS, "color", Parser::COLOR, "[0, 0, 0]");

    if (obj.find("index_of_refraction") != obj.end()) {
        m.index_of_refraction = Parser::asNumber(obj["index_of_refraction"]);
    }
    if (obj.find("absorvance") != obj.end()) {
        m.absorvance = Parser::asNumber(obj["absorvance"]);
    }
    if (obj.find("roughness") != obj.end()) {
        m.roughness = Parser::asNumber(obj["roughness"]);
    }
    if (obj.find("emittance") != obj.end()) {
        m.emittance = Parser::asNumber(obj["emittance"]);
    }
    if (obj.find("color") != obj.end()) {
        m.color = Parser::asRGB(obj["color"]);
    }

    return m;
}