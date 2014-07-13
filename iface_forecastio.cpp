#include "jsonapi/jsonapi.h"
#include "iface_forecastio.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void process_file()
{
    std::ifstream forecastjson(FORECASTIOFILE);
    std::stringstream buffer;
    JsonParse *parser = new JsonParse();

    // read the file in...
    buffer << forecastjson.rdbuf();

    // set the parser on it
    std::string s = buffer.str();
    parser->SetInput(s);

    // run through it!
    if (parser->Parse() == true) {
        JSONValue *val = JSONAPI::GetValue(parser);
        JsonType type = val->GetType();

        switch (type) {
            case JsonType_Object:
                std::cout << "woo woo\n";
                break;
        }
    }
}

int main()
{
    process_file();
}
