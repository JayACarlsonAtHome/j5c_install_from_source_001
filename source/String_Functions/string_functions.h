//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_STRING_FUNCTIONS_H
#define J5C_INSTALL_FROM_SOURCE_001_STRING_FUNCTIONS_H

bool getBoolFromString(sstr& some_value)
{
    bool result = false;
    sstr temp1;
    int temp2;
    for (auto idx = 0ul; idx < some_value.length(); ++idx)
    {
        if (std::isupper(some_value.at(idx)))
        {
            temp2 =  std::tolower(some_value.at(idx));
        }
        else
        {
            temp2 = some_value.at(idx);
        }
        temp1.append({static_cast<char>(temp2)});
    }
    if (temp1 == "t")    result = true;
    if (temp1 == "true") result = true;
    if (temp1 == "on")   result = true;
    return result;
}

sstr lowerCaseString(sstr& some_value)
{
    // unicode lower case conversions require
    // very specialized code, and this is not it
    // but it will handle the english words that
    // we need for this program.

    // I tested 6 versions of lowercase
    //   (this was code test number 4 and the fastest version)
    //   (turns out c++ string operations are much slower than c string operations)
    // Note you have to be in the millions of operations
    //   for it to even matter, and we only do a less than
    //   a thousand here.

    auto len = some_value.length();
    char strChar[len+1];
    strcpy(strChar, some_value.c_str());
    for (auto idx = 0ul; idx < len; ++idx)
    {
        strChar[idx] =  std::tolower(strChar[idx]);
    }
    sstr result {strChar};
    return result;
}

sstr upperCaseString(sstr& some_value)
{
    // unicode upper case conversions require
    // very specialized code, and this is not it
    // but it will handle the english words that
    // we need for this program.

    auto len = some_value.length();
    char strChar[len+1];
    strcpy(strChar, some_value.c_str());
    for (auto idx = 0ul; idx < len; ++idx)
    {
        strChar[idx] =  std::toupper(strChar[idx]);
    }
    sstr result {strChar};
    return result;

}

sstr trimLeftAndRight(sstr& inString, sstr& ws)
{
    sstr result;
    if (ws.length() == 0)
    {
        ws = " \n\r\t";
    }
    auto max = std::numeric_limits<unsigned long>::max();
    auto start = inString.find_first_not_of(ws);
    if (start != max)
    {
        result = inString.substr(start, inString.find_last_not_of(ws) - start + 1);
    }
    return result;
}

sstr getDigitsInStringAsString(sstr& some_value)
{
    sstr result;
    int oneChar;
    if (some_value.length() > 0) {
        for (auto idx = 0ul; idx < some_value.length(); ++idx) {
            if (std::isdigit(some_value.at(idx))) {
                oneChar = some_value.at(idx);
                result.append({static_cast<char>(oneChar)});
            }
        }
    }
    else
    {
        result = "";
    }
    return result;
}

sstr getValid_pVersion(sstr& some_value)
{
    // explanation of what this does...
    // input ""         output "001"
    // input "rr4gre4"  output "00044"
    // input "2"        output "002"
    // input "45"       output "045"
    // input "652"      output "652"
    // input "843xc"    output "00843"

    // in essence pad leading zeros to max length of string
    // only accept digits in string, except all letters.
    // prefer a length of 3 digits, but enable to grow to a max of 5 digits

    sstr result;
    int oneChar;
    auto maxWidth = some_value.length();
    if (maxWidth > 5) { maxWidth  = 5; }
    if (maxWidth < 3) { maxWidth  = 3; }

    for (auto idx = 0ul; idx < some_value.length(); ++idx)
    {
        if (std::isdigit(some_value.at(idx)))
        {
            oneChar =  some_value.at(idx);
            result.append({static_cast<char>(oneChar)});
        }
    }
    if ((result.length() < maxWidth) && (result.length() > 0))
    {
        auto pad_length = maxWidth - result.length();
        sstr pad_string = sstr(pad_length, '0');
        result = pad_string.append(result);
    }
    if (result.length() < 3)
    {
        // We want to make this safe so no-one deletes any real data
        //    by overwriting a good directory
        result = "x001";
    }
    if (result.length() > maxWidth)
    {
        result = result.substr(0, maxWidth);
    }
    return result;
}

sstr padLeftZeros(int max_width, int number)
{
    auto max = pow(10,max_width);
    bool preConditionsMet = false;
    sstr result;
    //pre setup
    sstr strNumber = std::to_string(number);
    unsigned long pad_length = static_cast<unsigned long>(max_width);

    //check preConditions
    if ((number > -1) && (number < max)) {
        preConditionsMet = true;
        pad_length = max_width - strNumber.length();
    }

    if (preConditionsMet) {
        if (pad_length > 0) {
            sstr pad_string = sstr(pad_length, '0');
            result = pad_string;
            result.append(strNumber);
        } else
        {
            result = strNumber;
        }
    }
    else
    {
        result = "Error in padLeftZeros(int max_width, int number);";
    }
    return result;
};

sstr stripCharFromString(sstr& inString, const char c)
{
    sstr result;
    auto max = inString.length();
    auto current = max - max;
    while (current < max)
    {
        if (inString.at(current) != c)
        {
            result.append(inString.substr(current, 1));
        }
        current += 1;
    }
    return result;
}

sstr removeCharFromStartOfString(sstr& str, char c )
{
    sstr result;
    if (((str.length() == 1) && (str.at(0) = c)) || (str.length() == 0))
    {
        result = "";
    }
    else {
        auto first = str.find_first_not_of(c);
        result = str.substr(first, str.length());
    }
    return result;
}

sstr removeCharFromEnd__OfString(sstr &str, char c)
{
    sstr result;
    if (((str.length() == 1) && (str.at(0) = c))  || (str.length() == 0))
    {
        result = "";
    }
    else {
        auto last = str.find_last_not_of(c);
        result = str.substr(0, last + 1);
    }
    return result;
}

sstr getProperNameFromString(sstr& some_value)
{
    // make the first letter capital
    // make all other letters lowercase

    // in the effort of don't repeat yourself
    //   I would like to call lowerCaseString()
    //   and then uppercase the first charactor
    //   but the string to char arrays back to strings to char arrays
    //   make it sub-optimal

    auto len = some_value.length();
    char strChar[len+1];
    strcpy(strChar, some_value.c_str());
    for (auto idx = 0ul; idx < len; ++idx)
    {
        strChar[idx] =  std::tolower(strChar[idx]);
    }
    strChar[0] =  std::toupper(strChar[0]);
    sstr result {strChar};
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_STRING_FUNCTIONS_H
