#include "Parser.h"

#include <stdlib.h>

JsonBox::Object Parser::loadAndCheck(const std::string& file)
{
    JsonBox::Value v;
    v.loadFromFile(file);

    return v.getObject();
}

void Parser::checkObject(const JsonBox::Value& val, const std::string& cls)
{
    if (!val.isObject()) {
        std::cerr << "Parse error: expected object for \"" << cls << "\".\n";
        exit(1);
    }
}

void Parser::checkArray(const JsonBox::Value& val, const std::string& cls)
{
    if (!val.isArray()) {
        std::cerr << "Parse error: expected array for \"" << cls << "\".\n";
        exit(1);
    }
}

void Parser::checkParam(JsonBox::Object& obj, const std::string& cls, 
    const std::string& param, const int type)
{
    if (obj.find(param) == obj.end()) { // not found
        std::cerr << "Parse error: missing parameter \"" << param << "\" in " << cls << ".\n";
        exit(1);
    }

    switch (type) {
    case INTEGER:
        if (!obj[param].isInteger()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", integer expected.\n";
            exit(1);
        }
    case NUMBER:
        if (!obj[param].isDouble()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", number expected.\n";
            exit(1);
        }
    case STRING:
        if (!obj[param].isString()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", string expected.\n";
            exit(1);
        }
    case VEC3:
        if (!obj[param].isArray() || !obj[param].getArray()[0].isDouble() ||
            !obj[param].getArray()[1].isDouble() || !obj[param].getArray()[2].isDouble()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", vec3 expected.\n";
            exit(1);
        }
    case COLOR:
        if (!obj[param].isArray() || !obj[param].getArray()[0].isDouble() ||
            !obj[param].getArray()[1].isDouble() || !obj[param].getArray()[2].isDouble()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", rgb expected.\n";
            exit(1);
        }
    default:
        fprintf(stderr, "Parse error: invalid type code %d.\n", type);
        exit(1);
    }
}

void Parser::checkOption(JsonBox::Object& obj, const std::string& cls, 
    const std::string& param, const int type, const std::string& value)
{
    if (obj.find(param) == obj.end()) { // not found
        std::cout << "Parse warning: use default parameter \"" << value 
            << "\" for " << cls << ":" << param << ".\n";
        return;
    }

    switch (type) {
    case INTEGER:
        if (!obj[param].isInteger()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", integer expected.\n";
            exit(1);
        }
    case NUMBER:
        if (!obj[param].isDouble()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", number expected.\n";
            exit(1);
        }
    case STRING:
        if (!obj[param].isString()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", string expected.\n";
            exit(1);
        }
    case VEC3:
        if (!obj[param].isArray() || !obj[param].getArray()[0].isDouble() ||
            !obj[param].getArray()[1].isDouble() || !obj[param].getArray()[2].isDouble()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", vec3 expected.\n";
            exit(1);
        }
    case COLOR:
        if (!obj[param].isArray() || !obj[param].getArray()[0].isDouble() ||
            !obj[param].getArray()[1].isDouble() || !obj[param].getArray()[2].isDouble()) {
            std::cerr << "Parse error: invalid parameter \"" << obj[param].getString() 
                << "\" for " << cls << ":" << param << ", rgb expected.\n";
            exit(1);
        }
    default:
        fprintf(stderr, "Parse error: invalid type code %d.\n", type);
        exit(1);
    }
}

int Parser::asInteger(const JsonBox::Value& val)
{
    return val.getInteger();
}
    
double Parser::asNumber(const JsonBox::Value& val)
{
    return val.getDouble();
}

std::string Parser::asString(const JsonBox::Value& val)
{
    return val.getString();
}

Vector Parser::asVector(const JsonBox::Value& val)
{
    return Vector(
        val.getArray()[0].getDouble(),
        val.getArray()[1].getDouble(),
        val.getArray()[2].getDouble()
    );
}

RGB Parser::asRGB(const JsonBox::Value& val)
{
    return RGB(
        val.getArray()[0].getDouble(),
        val.getArray()[1].getDouble(),
        val.getArray()[2].getDouble()
    );
}

std::string Parser::getMaterial(const JsonBox::Value& val, const std::string& cls)
{
    JsonBox::Object obj = val.getObject();
    if (obj.find("material") == obj.end()) { // not found
        std::cerr << "Parse error: missing material for \"" << cls << "\".\n";
        exit(1);
    }

    return obj["material"].getString();
}
