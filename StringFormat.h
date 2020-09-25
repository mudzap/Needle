#ifndef _STRING_FORMAT_
#define _STRING_FORMAT_

#include <string>

struct  StringFormat
{

    static std::string zeroPad(const int value, const int total_length)
    {
        std::string str = std::to_string(value);
        if (str.length() < total_length)
            str.insert(str.front() == '-' ? 1 : 0, total_length - str.length(), '0');
        return str;
    }

};

#endif