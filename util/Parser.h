#ifndef UTIL_PARSER_H_
#define UTIL_PARSER_H_

#include "Vector.h"
#include "RGB.h"
#include "common.h"
#include "../third-party/jsonbox/JsonBox.h"

#include <string>

class Parser
{
public:
    // loader
    static JsonBox::Object loadAndCheck(const std::string& file);

    // validators
    static void checkObject(const JsonBox::Value& val, const std::string& cls);
    static void checkArray(const JsonBox::Value& val, const std::string& cls);
    
    static void checkParam(JsonBox::Object& obj, const std::string& cls, 
        const std::string& param, const int type);
    static void checkOption(JsonBox::Object& obj, const std::string& cls, 
        const std::string& param, const int type, const std::string& value);
    
    // data translators
    static int asInteger(const JsonBox::Value& val);
    static double asNumber(const JsonBox::Value& val);
    static std::string asString(const JsonBox::Value& val);
    static Vector asVector(const JsonBox::Value& val);
    static RGB asRGB(const JsonBox::Value& val);

    static std::string getMaterial(const JsonBox::Value& val, const std::string& cls);

    // types
    const static int INTEGER = 0;
    const static int NUMBER = 1;
    const static int STRING = 2;
    const static int VEC3 = 3;
    const static int COLOR = 4;
};

#endif // UTIL_PARSER_H_