#include "Material.h"

Material Material::parse(const JsonBox::Value& val,
    const std::string& CLS)
{
    Material m;

    Parser::checkObject(val, CLS);
    
    JsonBox::Object obj = val.getObject();
    Parser::checkOption(obj, CLS, "index_of_refraction", Parser::NUMBER_OR_INF, "1");
    Parser::checkOption(obj, CLS, "absorvance", Parser::NUMBER, "0");
    Parser::checkOption(obj, CLS, "roughness", Parser::NUMBER, "0");
    Parser::checkOption(obj, CLS, "emittance", Parser::NUMBER, "0");
    Parser::checkOption(obj, CLS, "color", Parser::COLOR, "[1, 1, 1]");

    if (obj.find("index_of_refraction") != obj.end()) {
        m.index_of_refraction = Parser::asNumberOrInf(obj["index_of_refraction"]);
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
    if (obj.find("texture") != obj.end()) {
        m.texture = Parser::asString(obj["texture"]);
    }

    return m;
}