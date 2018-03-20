//  j5c_install_from_source_001 -- build a LAMP+ system from source
//  Copyright (C) 2018  Jay A Carlson of J5C Marketing LLC.
//
// This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, version 3.
//
// This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
// This program can automatically install a LAMP system + other stuff
// LAMP        := Linux, Apache Web Server, MariaDB, Php
// Other stuff := Perl, Ruby, Tcl/Tk, POCO C++ Libraries

#include <math.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <limits>
#include "source/j5c_date.h"

using namespace J5C_DSL_Code;
using sstr = std::string;

//constant expressions
constexpr auto daysIn400Years = 146096l;
constexpr auto seconds_in_minute = 60;
constexpr auto seconds_in_hour   = 60 * seconds_in_minute;
constexpr auto seconds_in_day    = 24 * seconds_in_hour;
// the actual value is not constant, but we can average over 400 years to make it closer
constexpr auto average_seconds_in_year = (daysIn400Years * seconds_in_day)/400.0;


//constants
const sstr skip_value = "Skip_This_Install";

const sstr STG_NAME         = "stg";
const sstr KEY_COMPANY_NAME = "Section_a1_Abbr_CompanyName";
const sstr VAL_COMPANY_NAME = "zzz";
const sstr KEY_DEFLT_PREFIX = "Section_a2_The_Default_Prefix";
const sstr VAL_DEFLT_PREFIX = "/usr/local";
const sstr KEY_TOUSE_PREFIX = "Section_a3_Use_Prefix";
const sstr VAL_TOUSE_PREFIX = "false";
const sstr KEY_PATH_VERSION = "Section_a4_The_Path_Version";
const sstr VAL_PATH_VERSION = "001";
const sstr KEY_AN_OS_SYSTEM = "Section_a5_The_Operating_System";
const sstr VAL_AN_OS_SYSTEM = "Red Hat";
const sstr KEY_RUN_DEPENDCS = "Section_a6_Run_Dependencies";
const sstr VAL_RUN_DEPENDCS = "false";
const sstr KEY_PROTECT_MODE = "Section_a7_Enable_Protect_Mode";
const sstr VAL_PROTECT_MODE = "true";

enum class OS_type { Selection_Not_Available = -1, No_Selection_Made = 0, CentOS = 1, Fedora = 2, Linux_Mint = 3, RedHat = 4, MaxOSX = 5};
enum class Mac_PM  { Selection_Not_Available = -1, No_Selection_Made = 0, Home_Brew = 0, MacPorts = 1 };

sstr lowerCaseString(sstr& some_value)
{
    sstr result;
    int oneChar;
    for (auto idx = 0ul; idx < some_value.length(); ++idx)
    {
        if (std::isupper((*some_value.substr(idx,1).c_str())))
        {
            oneChar =  tolower(*some_value.substr(idx,1).c_str());
        }
        else
        {
            oneChar = (*some_value.substr(idx,1).c_str());
        }
        result.append({static_cast<char>(oneChar)});
    }
    return result;
}

sstr upperCaseString(sstr& some_value)
{
    sstr result;
    int oneChar;
    for (auto idx = 0ul; idx < some_value.length(); ++idx)
    {
        if (std::islower((*some_value.substr(idx,1).c_str())))
        {
            oneChar =  toupper(*some_value.substr(idx,1).c_str());
        }
        else
        {
            oneChar = (*some_value.substr(idx,1).c_str());
        }
        result.append({static_cast<char>(oneChar)});
    }
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
        if (std::isdigit((*some_value.substr(idx,1).c_str())))
        {
            oneChar =  *some_value.substr(idx,1).c_str();
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
        result = "001";
    }
    if (result.length() > maxWidth)
    {
        result = result.substr(0, maxWidth);
    }
    return result;
}


sstr padLeftZeros(int max_width, int number)
{
    bool validResult = false;
    auto max = pow(10,max_width);
    bool preConditionsMet = false;

    //pre setup
    sstr strNumber = std::to_string(number);
    auto pad_length = max_width;

    //check preConditions
    if ((number > -1) && (number < max)) {
        preConditionsMet = true;
        pad_length = max_width - strNumber.length();
    }

    if (preConditionsMet) {
        if (pad_length > 0) {
            sstr pad_string = sstr(pad_length, '0');
            sstr result = pad_string;
            result.append(strNumber);
            return result;
        } else
        {
            return strNumber;
        }
    }
    else
    {
        return "Error in padLeftZeros(int max_width, int number);";
    }

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

sstr joinPathParts(sstr& partA, sstr& partB)
{
    sstr fixed1, fixed2, path;
    fixed1 = removeCharFromStartOfString(partA,  '/');
    fixed1 = removeCharFromEnd__OfString(fixed1, '/');
    fixed2 = removeCharFromStartOfString(partB,  '/');
    fixed2 = removeCharFromEnd__OfString(fixed2, '/');
    return path = "/" + fixed1 + "/" + fixed2 +  "/";
}

sstr joinPathWithFile(sstr& partA, sstr& fileName)
{
    sstr fixed1, fixed2, path;
    fixed1 = removeCharFromStartOfString(partA,    '/');
    fixed1 = removeCharFromEnd__OfString(fixed1,   '/');
    fixed2 = removeCharFromStartOfString(fileName, '/');
    fixed2 = removeCharFromEnd__OfString(fixed2,   '/');
    return path = "/" + fixed1 + "/" + fixed2;
}


bool isFileSizeGtZero(sstr &fileName, bool bShow = false)
{
    bool result = false;
    std::ifstream file(fileName, std::ios::in | std::ios::binary);
    file.seekg (0, file.end);
    auto worthless = file.tellg();
    auto real_size = static_cast<long long>(worthless);
    file.close();
    if (real_size > 0) { result = true; }
    if (bShow) {
        if (real_size > 0) {
            std::cout << "Found file: " << fileName << " and size = " << real_size << std::endl;
            result = true;
        } else {
            std::cout << "File: " << fileName << " not found or size is 0" << std::endl;
        }
    }
    return result;
}

int ensure_Directory_main(sstr& fullCommand, int result)
{
    std::cout << "  " << fullCommand << std::endl;
    result = system(fullCommand.c_str());
    if (result == 0) {
        std::cout << "  ->success on running command:" << std::endl;
    } else {
        std::cout << "  ->failure on running command:" << std::endl;
    }
    return result;
}

int ensure_Directory_exists1(sstr& path)
{
    int result = 1;
    sstr command  = "mkdir -p ";
    sstr fullCommand = command + path;
    ensure_Directory_main(fullCommand, result);
    return result;
}

int ensure_Directory_exists2(sstr& basePath, sstr& path)
{
    int result = 1;
    sstr fullpath = basePath + path;
    if (basePath.substr(basePath.length()-1, 1) != "/")
    {
        sstr fullPath = basePath + "/" + path;
    }
    sstr command  = "mkdir -p ";
    sstr fullCommand = command + fullpath;
    ensure_Directory_main(fullCommand, result);
    return result;
}

bool directoryExists(const sstr& path)
{

    bool result = false;
    sstr command = "cd " + path;
    command.append(" 2> /dev/null ");
    int commandResult = system(command.c_str());
    if (commandResult == 0) result = true;
    return result;
}

time_t get_Time()
{
    time_t rawtime;
    time (&rawtime);
    return rawtime;
}

template <typename T>
sstr get_Time_Part(T timePart)
{
    sstr strTimePart = std::to_string(timePart);
    if (timePart < 10) {
        strTimePart = "0" + strTimePart;
    }
    return strTimePart;
}

sstr get_GmtOffset()
{
    time_t theTime;
    struct tm * timeinfo;
    time (&theTime);
    timeinfo = localtime (&theTime);

    long gmt    = timeinfo->tm_gmtoff;
    long gmtHoursOffset = gmt / 3600;

    sstr strGmtOff  = std::to_string(gmtHoursOffset);
    sstr offset;
    if (gmtHoursOffset > -1)
    {
        offset = get_Time_Part<long>(gmtHoursOffset);
        if (gmtHoursOffset == 0)
        {
            strGmtOff = "( " + offset + ")";
        } else
        {
            strGmtOff = "(+" + offset + ")";
        }
    }
    else
    {
        gmtHoursOffset *= -1;
        offset = get_Time_Part<long>(gmtHoursOffset);
        strGmtOff = "(-" + offset + ")";
    }
    return strGmtOff;
}



sstr get_Time_as_String(time_t theTime)
{
    struct tm * timeinfo;
    if (theTime == 0)
    {
        time (&theTime);
    }
    timeinfo = localtime (&theTime);

    int hours   = timeinfo->tm_hour;
    int minutes = timeinfo->tm_min;
    int seconds = timeinfo->tm_sec;
    long gmt    = timeinfo->tm_gmtoff;
    long gmtHoursOffset = gmt / 3600;

    sstr strHours   = get_Time_Part<int>(hours);
    sstr strMinutes = get_Time_Part<int>(minutes);
    sstr strSeconds = get_Time_Part<int>(seconds);

    sstr time = strHours + ":" + strMinutes + ":" + strSeconds + " ";

    return time;
}

sstr make_fileName_dateTime(time_t theTime)
{
    sstr thefileTime = get_Time_as_String(theTime);
    j5c_Date d1{};
    sstr theDate = d1.strDate();
    theDate = theDate.replace(4,1,"_");
    theDate = theDate.replace(7,1,"_");
    theDate.append("_at_");
    theDate.append(thefileTime);
    theDate = theDate.replace(16,1,"_");
    theDate = theDate.replace(19,1,"_");
    if (theDate.length() > 22)
    {
        theDate = theDate.substr(0,22);
    }
    return theDate;
}


sstr getDuration(time_t stop, time_t start)
{
    // we are going to give a positive duration
    //   even if the parameters get switched

    long long secondsTotal = 0;
    if (start > stop)
    {
        secondsTotal = start - stop;
    }
    else
    {
        secondsTotal = stop - start;
    }

    int years = secondsTotal / average_seconds_in_year;
    long long remainingSeconds = secondsTotal - (years * average_seconds_in_year);
    sstr strYears = padLeftZeros(3, years);

    int days = remainingSeconds / seconds_in_day;
    remainingSeconds = remainingSeconds - (days * seconds_in_day);
    sstr strDays = padLeftZeros(3, days);

    int hours = remainingSeconds / seconds_in_hour;
    remainingSeconds = remainingSeconds - (hours * seconds_in_hour);
    sstr strHours = padLeftZeros(2, hours);

    int minutes = remainingSeconds / seconds_in_minute;
    remainingSeconds = remainingSeconds - (minutes * seconds_in_minute);
    sstr strMinutes = padLeftZeros(2, minutes);

    int seconds = remainingSeconds;
    sstr strSeconds = padLeftZeros(2, seconds);

    sstr result = strYears;
    result.append(":");
    result.append(strDays);
    result.append(":");
    result.append(strHours);
    result.append(":");
    result.append(strMinutes);
    result.append(":");
    result.append(strSeconds);
    result.append(" ");
    return result;
}


int startNewLogSection(std::ofstream& file, sstr utc = "-7")
{
    int result = 1;  // assume failure
    if ( (file.is_open()) && (file.good()) )
    {
        j5c_Date thisDate{};
        file << std::endl << std::endl;
        file << " Status started on " << thisDate.strDate() << "  UTC = " << utc <<  std::endl;
        file << " " << std::endl;
        file << " Start    : Stop     : Duration         : Command / Comments" << std::endl;
        file << " HH:MM:SS : HH:MM:SS : YYY:DDD:HH:MM:SS :" << std::endl;
        file << " =======================================================================================================================" << std::endl;
        result = 0; // success
    }
    else
    {
        std::cout << "!!!Error -- unable to create new log section -- " << std::endl;
    }
    return result;
}

int appendNewLogSection(sstr &fileName)
{
    j5c_Date thisDate{};
    std::ofstream file;
    int result = 0;
    file.open(fileName, std::ios::out | std::ios::app);
    if ((file.is_open()) && (file.good())) {
        sstr gmtOffset = get_GmtOffset();
        startNewLogSection(file, gmtOffset);
    }
    file.close();
    return result;
}

int create_file(const sstr& fileName)
{
    j5c_Date thisDate{};
    std::ofstream file;
    int result = 0;
    file.open(fileName, std::ios::out | std::ios::trunc );
    if ( (file.is_open()) && (file.good()) )
    {
        file << "# File Created on " << thisDate.strDate() << std::endl;
        file << "# " << std::endl;
        file << "# Copyright J5C Marketing LLC" << std::endl;
        file << "# Jay A Carlson" << std::endl;
        file << "# jay.a.carlson@gmail.com" << std::endl;
        file << "# 360-649-6218" << std::endl;
        file << "# " << std::endl;
        file << "# " << std::endl;
        file.close();
    }
    else
    {
        std::cout << "!!!Error -- unable to create file -- " << std::endl;
        result = 1;
    }

    sstr c1 = "eval \"uname -a >> ";
    sstr c2 = c1 + fileName + "\"";

    system(c2.c_str() );

    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        file << "# " << std::endl;
        file << "# " << std::endl;
        result += startNewLogSection(file);
    }
    else
    {
        std::cout << "!!!Error -- unable to append to file -- " << std::endl;
        result = 1;
    }

    file.close();
    return result;
}

int ensure_file(sstr &fileName)
{
    std::ofstream file;
    int result = 1;  // assume failure
    file.open(fileName, std::ios::out | std::ios::app);
    if (!(file.is_open()))
    {
        create_file(fileName);
        result = 0;
    }
    return result;
}

int write_file_entry(std::ofstream& file, const sstr& entry, time_t stop, time_t start, bool includeTime = false)
{
    int result = 0;
    if (includeTime)
    {
        sstr strStart = get_Time_as_String(start);
        sstr strStop  = get_Time_as_String(stop);
        sstr strDuration = getDuration(stop, start);
        file << " " << strStart << ": " << strStop << ": " << strDuration << ": " << entry << std::endl;
    }
    else
    {
        file << entry << std::endl;
    }
    return result;
}


int file_write_vector_to_file(sstr &fileName, std::vector <sstr> &vec_lines, bool includeTime = true)
{
    std::ofstream file;
    int result = 0;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        for (const auto& it: vec_lines)
        {
            time_t start = get_Time();
            time_t stop  = get_Time();

            if (it == "\n")
            {
                result += write_file_entry(file, "\n", stop, start, includeTime);
            }
            else
            {
                result += write_file_entry(file, it, stop, start, includeTime);
            }
        }
    }
    else
    {
        std::cout << "!!!Error -- unable to write vector contents to file -- " << std::endl;
    }
    file.close();
    return result;
}

int file_append_line(sstr &fileName, sstr &line, time_t stop, time_t start)
{
    std::ofstream file;
    int result = 1;
    bool withTime = true;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        result = write_file_entry(file, line, stop, start, withTime);
    }
    else
    {
        std::cout << "!!!Error -- unable to append line to file -- " << std::endl;
    }
    file.close();
    return result;
}

sstr getProperNameFromString(sstr& some_value)
{
    // make the first letter capital
    // make all other letters lowercase

    sstr result;
    sstr temp1 = some_value.substr(1,some_value.length());
    temp1 = lowerCaseString(temp1);
    result = static_cast<char>(toupper(*some_value.substr(0,1).c_str()));
    result.append({temp1});
    return result;
}

int file_append_results(sstr& fileName, sstr& programName, sstr& version, int step, int installResult, time_t stop, time_t start)
{
    unsigned long width1 = 31;
    unsigned long width2 = 40;
    sstr fill1(width1,'.');
    sstr fill2(width2,'.');
    sstr ProperName = getProperNameFromString(programName);
    std::ofstream file;
    sstr line;
    int result = 1;
    bool found = false;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        if (programName.substr(0,12) == "Dependencies")
        {
            line = "Install " + programName + fill1;
        }
        else
        {
            line = "Install " + programName + "-" + version + fill1;
        }


        if (step < 0)
        {
            found = true;
        }

        if ((!found) && (step < 10))
        {
            line = line.substr(0,width1) +  "step 0"  + std::to_string(step) + fill2;
            found = true;
        }
        if ((!found) && (step > 9))
        {
            line = line.substr(0,width1) +  "step "  + std::to_string(step) + fill2;
            found = true;
        }
        line = line.substr(0,width2);

        sstr strResults = "......" + std::to_string(installResult);
        strResults = strResults.substr(strResults.length()-5,5);
        line += " : Result = " + strResults;
        if (installResult == -1)
        {
            line += " : Install Blocked..";
        }
        if (installResult == 0)
        {
            line += " : Good.............";
        }
        if (installResult > 0)
        {
            line += " : What the heck?...";
        }
        if (found) {
            result = write_file_entry(file, line, stop, start, true);
        }
    }
    else
    {
        std::cout << "!!!Error -- unable to append results to file -- " << std::endl;
    }
    file.close();
    return result;
}


int file_append_blank_lines(sstr& fileName, int count)
{
    std::ofstream file;
    int result = 1;
    std::vector<sstr> vec;
    file.open(fileName, std::ios::out | std::ios::app );

    if ( (file.is_open()) && (file.good()) )
    {
        while (count > 0)
        {
            count -= 1;
            file << std::endl;
        }
    }
    return result;
}

std::vector<sstr> readFile(sstr &fileName, unsigned long maxCount)
{
    unsigned long theCount = 0;
    std::ifstream file;
    std::vector<sstr> result;
    file.open(fileName, std::ios::in );
    sstr lineData;
    if ( (file.is_open()) && (file.good()) )
    {
        while (getline(file, lineData))
        {
            result.push_back(lineData + "\n");
            theCount += 1;
            if (theCount >= maxCount)
            {
                break;
            }
        }
    }
    else
    {
        std::cout << "!!!Error -- unable to read file -- " << std::endl;
    }
    file.close();
    return result;
}

bool prior_Results(sstr& fileNameResult, sstr& searchStr)
{
    auto count = 50000ul;
    bool result = false;
    sstr it_data;
    searchStr = getProperNameFromString(searchStr);
    auto max = std::numeric_limits<unsigned long>::max();

    std::vector<sstr> data = readFile(fileNameResult, count);
    for (const auto& it : data )
    {
        it_data =  it;

        auto found1 = it_data.find(searchStr);
        if ( found1 != max )
        {
            auto found2 = it_data.find("Result =");
            sstr temp1;
            if (found2 < max)
            {
                temp1 = it_data.substr(found2+8,it_data.length());
            }
            auto end = temp1.find_first_of(':');
            temp1 = temp1.substr(0, end - 1);
            unsigned long idx = 0;
            while (idx < temp1.length())
            {
                if (isdigit(temp1[idx])) break;
                if (temp1[idx] == '.') temp1[idx] = ' ';
                ++idx;
            }
            auto temp2 = std::stol(temp1, nullptr, 10);
            if (temp2 == 0)
            {
                result = true;
                break;
            }
        }
    }
    return result;
}

std::map<sstr, sstr> getProgramSettings(sstr& fileSettings)
{
    static sstr temp;
    static auto max = temp.max_size();
    auto maxLineCount = 500ul;
    std::map<sstr, sstr> result;
    sstr it_data;
    sstr delimiter;
    sstr key;
    sstr value;
    sstr ws = " \t\n\r";

    //load general default values into map

    result.emplace(std::pair<sstr , sstr >(KEY_COMPANY_NAME, VAL_COMPANY_NAME));
    result.emplace(std::pair<sstr , sstr >(KEY_DEFLT_PREFIX, VAL_DEFLT_PREFIX));
    result.emplace(std::pair<sstr , sstr >(KEY_TOUSE_PREFIX, VAL_TOUSE_PREFIX));
    result.emplace(std::pair<sstr , sstr >(KEY_PATH_VERSION, VAL_PATH_VERSION));
    result.emplace(std::pair<sstr , sstr >(KEY_AN_OS_SYSTEM, VAL_AN_OS_SYSTEM));
    result.emplace(std::pair<sstr , sstr >(KEY_RUN_DEPENDCS, VAL_RUN_DEPENDCS));
    result.emplace(std::pair<sstr , sstr >(KEY_PROTECT_MODE, VAL_PROTECT_MODE));

    int width = 32;
    std::cout << std::endl << std::endl;
    std::cout << "#    Listing Settings : Values (Defaults)" << std::endl;
    std::cout << "#=========================================================" << std::endl;
    for (auto element : result)
    {
        std::cout << ": " << std::setw(width) << element.first << " : " << element.second << std::endl;
    }

    std::cout << std::endl;
    std::vector<sstr> data = readFile(fileSettings, maxLineCount);
    for (const auto& it : data )
    {
        it_data = it;
        if (it_data.length() > 1)
        {
            delimiter = it_data.substr(0,1);
            if (delimiter != "#") {
                it_data = it_data.substr(1, it_data.length());
                auto split = it_data.find_first_of(delimiter);
                if (split < max) {
                    key = it_data.substr(0, split - 1);
                    key = trimLeftAndRight(key, ws);
                    value = it_data.substr(split+2, it_data.length());
                    value = trimLeftAndRight(value, ws);

                    std::cout << "Key     = ***" << key     << "***" << std::endl;
                    std::cout << "Value   = ***" << value   << "***" << std::endl;
                    std::cout << std::endl;

                    if (value.length() > 0) {
                        result[key] = value;
                    }
                }
            }
        }
    }

    std::cout << std::endl << std::endl;
    std::cout << "#    Listing Settings : Values (Loading Setting from File)" << std::endl;
    std::cout << "#=========================================================" << std::endl;
    for (auto element : result)
    {
        std::cout << ": " << std::setw(width) << element.first << " : " << element.second << std::endl;
    }
    std::cout << std::endl << std::endl;

    return result;
}

void section_already_loaded(sstr& programName, sstr& version)
{
    if (version.length() > 0) {
        std::cout << "This section: " << programName + "-" + version << " already loaded." << std::endl;
    }
    else
    {
        std::cout << "This section: " << programName << " already loaded." << std::endl;
    }
}

void print_blank_lines(int count)
{
    while (count > 0)
    {
        count-= 1;
        std::cout << std::endl;
    }
}

int do_command(sstr& fileName, std::vector<sstr>& vec, bool createScriptOnly = false)
{
    sstr command;
    int result = 0;
    bool outputToFile = false;
    if (fileName.length()> 0)
    {
        outputToFile = true;
    }
    else
    {
        std::cout << "Doing commands will not be saved to file." << std::endl;
        std::cout << "Because no file name was given."           << std::endl;
    }
    for (const auto& it : vec )
    {
        time_t start = get_Time();
        result = 0;
        command = it;

        std::cout << "Command: " << command << std::endl;

        if ((command.substr(0,1) != "#") && (command.substr(0,1) != ":")) {
            if (!createScriptOnly) {
                result += system(command.c_str());
            }
        }

        if ((result != 0) && (!((command.substr(0,3) == "apt") || (command.substr(0,3) == "yum"))) )
        {
            std::cout << "!!!Error -- Process terminated for safety..." << std::endl;
            break;
        }

        time_t stop = get_Time();

        if (outputToFile) {
            file_append_line(fileName, command, stop, start);
        }

    }
    return result;
}

int removeDirectory(sstr& fileName, sstr& path, std::vector<sstr>& vec)
{
    if (path.length() > 2)
    {
        vec.emplace_back("eval \"rm -rf " + path + "\"");
    }
    return 0;
}

int install_apt_required_dependencies()
{
    std::cout << "Not Implemented Yet" << std::endl;
    return 1;
}

int install_mac_required_dependencies(Mac_PM mpm)
{
    if (mpm == Mac_PM ::Home_Brew) {
        //install_home_brew_required_dependencies();
        std::cout << "Not Implemented Yet" << std::endl;
        return 1;
    }
    if (mpm == Mac_PM ::MacPorts) {
        //install_macPort_required_dependencies();
        std::cout << "Not Implemented Yet" << std::endl;
        return 1;
    }
    return 1;
}

int install_yum_required_dependencies(sstr& fileName, sstr& programName, bool createScriptOnly)
{
    sstr fedora_release = "7";
    sstr command;
    std::vector<sstr> vec;
    vec.emplace_back("# Install " + programName + ".");
    vec.emplace_back("yum -y install wget");
    vec.emplace_back("wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-" + fedora_release + ".noarch.rpm");
    vec.emplace_back("yum -y install epel-release-latest-" + fedora_release +".noarch.rpm");
    vec.emplace_back("rm -f epel-release-latest-" + fedora_release +".noarch.rpm");
    vec.emplace_back("yum -y install autoconf");
    vec.emplace_back("yum -y install automake");
    vec.emplace_back("yum -y install bison");
    vec.emplace_back("yum -y install boost-devel");
    vec.emplace_back("yum -y install bzip2-devel");
    vec.emplace_back("yum -y install cmake");
    vec.emplace_back("yum -y install cmake-gui");
    vec.emplace_back("yum -y install expat-devel");
    vec.emplace_back("yum -y install ftp");
    vec.emplace_back("yum -y install gawk");
    vec.emplace_back("yum -y install google-chrome-stable");
    vec.emplace_back("yum -y install git");
    vec.emplace_back("yum -y install gitk");
    vec.emplace_back("yum -y install gcc");
    vec.emplace_back("yum -y install gcc-c++");
    vec.emplace_back("yum -y install gcc-c++-x86_64-linux-gnu");
    vec.emplace_back("yum -y install gnutls-c++");
    vec.emplace_back("yum -y install gnutls-devel");
    vec.emplace_back("yum -y install innotop");
    vec.emplace_back("yum -y install iotop");
    vec.emplace_back("yum -y install jemalloc-devel");
    vec.emplace_back("yum -y install java-1.8.0-openjdk");
    vec.emplace_back("yum -y install Judy");
    vec.emplace_back("yum -y install libcurl-devel");
    vec.emplace_back("yum -y install libedit-devel");
    vec.emplace_back("yum -y install libicu-devel");
    vec.emplace_back("yum -y install libjpeg-turbo-utils");
    vec.emplace_back("yum -y install libjpeg-turbo-devel");
    vec.emplace_back("yum -y install libpng-devel");
    vec.emplace_back("yum -y install libstdc++");
    vec.emplace_back("yum -y install libstdc++-devel");
    vec.emplace_back("yum -y install libstdc++-docs");
    vec.emplace_back("yum -y install libstdc++-static");
    vec.emplace_back("yum -y install libwebp-devel");
    vec.emplace_back("yum -y install libxml2-devel");
    vec.emplace_back("yum -y install libxslt-devel");
    vec.emplace_back("yum -y install libX11-devel");
    vec.emplace_back("yum -y install moreutils");
    vec.emplace_back("yum -y install mysqlreport");
    vec.emplace_back("yum -y install mysqltuner");
    vec.emplace_back("yum -y install mytop");
    vec.emplace_back("yum -y install ncurses-devel");
    vec.emplace_back("yum -y install openssl-devel");
    vec.emplace_back("yum -y install perl-CPAN");
    vec.emplace_back("yum -y install java-1.8.0-openjdk");
    vec.emplace_back("yum -y install re2c");
    vec.emplace_back("yum -y install sqlite-devel");
    vec.emplace_back("yum -y install sqlite-tcl");
    vec.emplace_back("yum -y install tcltls-devel");
    vec.emplace_back("yum -y install xml2");
    vec.emplace_back("yum -y xorg-x11-fonts*");
    vec.emplace_back("yum -y xorg-x11-server-Xnest");
    vec.emplace_back("yum -y install vsqlite++-devel");
    vec.emplace_back("yum -y install yum-utils");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}


int install_apt_required_dependencies(sstr& fileName, sstr& programName, bool createScriptOnly)
{
    sstr command;
    std::vector<sstr> vec;
    vec.emplace_back("# Install " + programName + ".");
    vec.emplace_back("apt update");
    vec.emplace_back("apt upgrade -y");
    vec.emplace_back("apt install openjdk-9-jdk -y");
    vec.emplace_back("apt autoremove -y");
    vec.emplace_back("apt install autoconf -y");
    vec.emplace_back("apt install build-essential -y");
    vec.emplace_back("apt install bison -y");
    vec.emplace_back("apt install bzip2 -y");
    vec.emplace_back("apt install cmake -y");
    vec.emplace_back("apt install cmake-gui -y");
    vec.emplace_back("apt install ftp -y");
    vec.emplace_back("apt install google-chrome-stable -y");
    vec.emplace_back("apt install git -y");
    vec.emplace_back("apt install gitk -y");
    vec.emplace_back("apt install git-cola -y");
    vec.emplace_back("apt install gcc -y");
    vec.emplace_back("apt install gnutls-dev -y");
    vec.emplace_back("apt install g++ -y");
    vec.emplace_back("apt install libboost-dev -y");
    vec.emplace_back("apt install libcurl4-gnutls-dev -y");
    vec.emplace_back("apt install libncurses5-dev -y");
    vec.emplace_back("apt install libedit-dev -y");
    vec.emplace_back("apt install libexpat-dev -y");
    vec.emplace_back("apt install libicu-dev -y");
    vec.emplace_back("apt install libjemalloc-dev -y");
    vec.emplace_back("apt install libjpeg-dev -y");
    vec.emplace_back("apt install libjudy-dev -y");
    vec.emplace_back("apt install libncurses5-dev -y");
    vec.emplace_back("apt install libpng-dev -y");
    vec.emplace_back("apt install libsqlite3-tcl -y");
    vec.emplace_back("apt install libssl-dev -y");
    vec.emplace_back("apt install libstdc++-5-dev -y");
    vec.emplace_back("apt install libstdc++-5-doc -y");
    vec.emplace_back("apt install libwebp-dev -y");
    vec.emplace_back("apt install libxml2-dev -y");
    vec.emplace_back("apt install libxslt-dev -y");
    vec.emplace_back("apt install libx11-dev -y");
    vec.emplace_back("apt install openssl-dev -y");
    vec.emplace_back("apt install x11-common -y");
    vec.emplace_back("apt install x11-xserver-utils  -y");
    vec.emplace_back("apt install x11-utils -y");
    vec.emplace_back("apt install re2c -y");
    vec.emplace_back("apt install sqlite3 -y");
    vec.emplace_back("apt install xml2 -y");
    vec.emplace_back("apt install wget -y");
    vec.emplace_back("apt install xxdiff -y");
    vec.emplace_back("apt update");
    vec.emplace_back("apt upgrade -y");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

bool getBoolFromString(sstr& some_value)
{
    bool result = false;
    sstr temp1;
    int temp2;
    for (auto idx = 0ul; idx < some_value.length(); ++idx)
    {
        if (std::isupper((*some_value.substr(idx,1).c_str())))
        {
            temp2 =  tolower(*some_value.substr(idx,1).c_str());
        }
        else
        {
            temp2 = (*some_value.substr(idx,1).c_str());
        }
        temp1.append({static_cast<char>(temp2)});
    }
    if (temp1 == "t")    result = true;
    if (temp1 == "true") result = true;
    if (temp1 == "on")   result = true;
    return result;
}

void protectProgram_IfRequired(sstr &buildFileName,
                               sstr &protectedFileName,
                               sstr &srcPath,
                               bool bProtectMode,
                               bool bScriptOnly)
{
    std::vector<sstr> vec;
    if (!(isFileSizeGtZero(protectedFileName, false))) {
        if (bProtectMode) {
            ensure_Directory_exists1(srcPath);
            vec.emplace_back("eval \"cd " + srcPath + "; echo 'protection = true' > " + protectedFileName + "\"");
            do_command(buildFileName, vec, bScriptOnly);
        }
    }
}

void howToRemoveFileProtection(sstr& buildFileName,
                           sstr& ProperName,
                           sstr& srcPath,
                           sstr& protectedFileName,
                           bool  bScriptOnly)
{
    std::vector<sstr> vec;
    if (!(isFileSizeGtZero(protectedFileName, false))) {

        vec.emplace_back("# ");
        vec.emplace_back("# This " + ProperName + " install is currently protected:");
        vec.emplace_back("# ");
        vec.emplace_back("# To remove this protection, run this command:");
        vec.emplace_back("# eval \"cd " + srcPath + "; rm -f " + protectedFileName + "\"");
        vec.emplace_back("# ");
        do_command(buildFileName, vec, bScriptOnly);
    }
}

void stageSourceCodeIfNeeded(sstr& buildFileName,
                             sstr& stagedFileName,
                             sstr& stgPath,
                             sstr& getPath,
                             sstr& compressedFileName,
                             bool bScriptOnly)
{
    std::vector<sstr> vec;
    vec.emplace_back("#");
    vec.emplace_back("# Stage source file if needed.");
    do_command(buildFileName, vec, bScriptOnly);
    vec.clear();
    if (!(isFileSizeGtZero(stagedFileName, true)))
    {
        vec.emplace_back("# Attempting to download file...");
        vec.emplace_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    else
    {
        vec.emplace_back("# Source code appears to be staged.");
    }
    do_command(buildFileName, vec, bScriptOnly);

}

sstr get_xxx_Path(const sstr& xxxPath, const sstr& replacement)
{

    if (replacement.length() == 3) {
        sstr newPath = xxxPath;
        auto start = newPath.rfind("xxx");
        if (start != -1) {
            newPath.replace(start, 3, replacement, 0, 3);
        }
        else
        {
            // this will most likely cause the program
            //   to not do what is desired, but the
            //   rest of the program will handle the
            //   failure instead of crashing the program.
            //   And it won't cause deleting the entire partition.
            newPath = "Err in get_xxx_Path()...xxx was not found.";
        }
        return newPath;
    }
    return "Err in get_xxx_Path()...Replacement path length is != 3.";
}


int removeWorkingDirectories(sstr& buildFileName, sstr& ProperName, sstr& bldPath, sstr& etcPath, sstr& srcPath, sstr& usrPath, bool bScriptOnly)
{
    std::vector<sstr> vec;
    removeDirectory(buildFileName, bldPath, vec);
    removeDirectory(buildFileName, etcPath, vec);
    removeDirectory(buildFileName, srcPath, vec);
    if (ProperName != "Tk") {
        removeDirectory(buildFileName, usrPath, vec);
    }
    int result = do_command(buildFileName, vec, bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Directories were deleted successfully.");
        do_command(buildFileName, vec, bScriptOnly);
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Directories were not deleted successfully.");
        vec.emplace_back("# Check for valid path and permissions.");
        do_command(buildFileName, vec, bScriptOnly);
    }
    return result;
}

int ensureWrkDirExist(sstr& buildFileName, sstr& ProperName,
                                  sstr& bldPath, sstr& etcPath, sstr& srcPath, sstr& usrPath,
                                  bool bScriptOnly)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Ensure " + ProperName + " working "
                                                        ""
                                                        "directories exist.");
    vec.emplace_back("eval \"mkdir -p " + bldPath + "\"");
    vec.emplace_back("eval \"mkdir -p " + etcPath + "\"");
    vec.emplace_back("eval \"mkdir -p " + srcPath + "\"");
    vec.emplace_back("eval \"mkdir -p " + usrPath + "\"");
    int result = do_command(buildFileName, vec, bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Directories exist or were created.");
        do_command(buildFileName, vec, bScriptOnly);
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Directories were not created successfully.");
        vec.emplace_back("# Check for valid path and permissions.");
        do_command(buildFileName, vec, bScriptOnly);
    }
    return result;
}

int createTargetFromStage(sstr& buildFileName, sstr& ProperName, sstr& stgPath, sstr& srcPath, sstr& compressedFileName, bool bScriptOnly)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Copy, decompress, and remove compressed file.");
    vec.emplace_back("eval \"cd " + stgPath + "; cp ./"   + compressedFileName + " " + srcPath + "\"");
    vec.emplace_back("eval \"cd " + srcPath + "; tar xf " + compressedFileName + "\"");
    vec.emplace_back("eval \"cd " + srcPath + "; rm -f "  + compressedFileName + "\"");
    int result = do_command(buildFileName, vec, bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Target created.");
        do_command(buildFileName, vec, bScriptOnly);
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Target was not created successfully.");
        vec.emplace_back("# Check for valid path and permissions.");
        do_command(buildFileName, vec, bScriptOnly);
    }
    return result;
}

int EnsureStageDirectoryExists(sstr& buildFileName, sstr& ProperName, sstr& stgPath, bool bScriptOnly)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Ensure stage directory exists.");
    vec.emplace_back("eval \"mkdir -p " + stgPath + "\"");
    int result = do_command(buildFileName, vec, bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Stage directory exists.");
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Stage directory does NOT exists.");
        vec.emplace_back("# Check for valid path, and permissions.");
    }
    do_command(buildFileName, vec, bScriptOnly);
    return result;
}



int setupInstallDirectories(sstr& buildFileName, sstr& ProperName, sstr& compressedFileName,
                            sstr& rtnPath, sstr& stgPath, const sstr& xxxPath, bool bScriptOnly)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Remove unfinished " + ProperName + " install (if any).");
    int result = do_command(buildFileName, vec, bScriptOnly);

    sstr bldPath = get_xxx_Path( xxxPath, "bld");
    sstr etcPath = get_xxx_Path( xxxPath, "etc");
    sstr srcPath = get_xxx_Path( xxxPath, "src");
    sstr usrPath = get_xxx_Path( xxxPath, "usr");

    if ((ProperName == "Tcl") || (ProperName == "Tk"))
    {
        sstr tmpPath = "usr/Tcl_Tk";
        usrPath = joinPathParts(rtnPath, tmpPath);
    }
    result += removeWorkingDirectories(buildFileName, ProperName, bldPath, etcPath, srcPath, usrPath, bScriptOnly);
    result += ensureWrkDirExist       (buildFileName, ProperName, bldPath, etcPath, srcPath, usrPath, bScriptOnly);
    result += createTargetFromStage   (buildFileName, ProperName, stgPath, srcPath, compressedFileName, bScriptOnly);
    return result;
}

int create_my_cnf_File(sstr& buildFileName, sstr& notes_file, sstr& etcPath, sstr& usrPath, bool bScriptOnly)
{
    std::vector<sstr> vec;
    vec.clear();
    vec.emplace_back("#Save the original my.cnf file if any, to have as a guide.");

    //lets add the date_time to the my.cnf.old in case we run this multiple times
    //  in a day we will still have the original file somewhere.

    sstr theDate = make_fileName_dateTime(0);
    vec.emplace_back("eval \"cd /etc; cp my.cnf" + etcPath + "my.cnf.old_" + theDate + "\"");
    do_command(buildFileName, vec, bScriptOnly);

    //Now we have to create the new my.cnf file
    vec.clear();
    vec.emplace_back("#Create new file " + etcPath + "my.cnf ");
    vec.emplace_back("eval \"cd /etc; echo ' ' >" + etcPath + "my.cnf \"");
    do_command(buildFileName, vec, bScriptOnly);

    sstr my_cnf_File = etcPath + "my.cnf";
    //Now we add the content to the /etc/my.cnf file
    vec.clear();
    vec.emplace_back("[mysqld]");
    vec.emplace_back("user=mysql");
    vec.emplace_back("socket='" + usrPath + "run/mariadb.socket'");
    vec.emplace_back("bind-address=127.0.1.1");
    vec.emplace_back("port=3306");
    vec.emplace_back(" ");
    vec.emplace_back("#Directories");
    vec.emplace_back("  basedir='" + usrPath + "'");
    vec.emplace_back("  datadir='" + usrPath + "data/'");
    vec.emplace_back(" ");
    vec.emplace_back("#Log File");
    vec.emplace_back("  log-basename='MariaDB_Logs'");
    vec.emplace_back(" ");
    vec.emplace_back("#Other Stuff");
    vec.emplace_back("#skip-networking");
    vec.emplace_back("  skip-ssl");
    vec.emplace_back("  key_buffer_size=32M");
    vec.emplace_back(" ");
    vec.emplace_back("# Disabling symbolic-links is recommended to prevent assorted security risks symbolic-links=0");
    vec.emplace_back("# Settings user and group are ignored when systemd is used.");
    vec.emplace_back("# If you need to run mysqld under a different user or group,");
    vec.emplace_back("# customize your systemd unit file for mariadb according to the");
    vec.emplace_back("# instructions in http://fedoraproject.org/wiki/Systemd");
    vec.emplace_back(" ");
    vec.emplace_back("[mysqld_safe]");
    vec.emplace_back("log-error='" + usrPath + "var/log/mariadb'");
    vec.emplace_back("pid-file='"  + usrPath + "run/mariadb_pid'");
    vec.emplace_back(" ");
    vec.emplace_back("#");
    vec.emplace_back("# include all files from the config directory");
    vec.emplace_back("#");
    vec.emplace_back("# Not using anything here....");
    vec.emplace_back("#   in fact no directory or files here");
    vec.emplace_back("#   !includedir " + etcPath + "my.cnf.d");
    vec.emplace_back(" ");
    vec.emplace_back("## end of file");
    vec.emplace_back(" ");
    file_write_vector_to_file(my_cnf_File, vec, false);
    return 0;
};

int configure(sstr& buildFileName, sstr& configureStr, sstr& xxxPath,  bool bDebug, bool bScriptOnly)
{
    sstr bldPath = get_xxx_Path(xxxPath, "bld");
    if (!bDebug) {
        std::vector<sstr> vec1;
        std::vector<sstr> vec2;
        vec1.emplace_back("# ");
        vec1.emplace_back("# Configure...");
        vec1.emplace_back("# Piping results to " + bldPath + ".");
        // We are ending the command we started here with \"
        //   This was started in the configureStr.
        configureStr.append(" > " + bldPath + "configure_results.txt 2>&1 \"");
        vec1.emplace_back(configureStr);
        int result = do_command(buildFileName, vec1, bScriptOnly);
        if (result == 0) {
            vec2.clear();
            vec2.emplace_back("# Configure completed successfully.");
        } else {
            vec2.clear();
            vec2.emplace_back("# Configure had some problems.");
            vec2.emplace_back("# Look through '" + bldPath + "configure_results.txt' to find the issue. ");
        }
        do_command(buildFileName, vec2, bScriptOnly);
        return result;
    }
}

int make(sstr& buildFileName, sstr& ProperName, sstr& progVersion, sstr& xxxPath,  bool bDebug, bool bScriptOnly)
{
    sstr bldPath = get_xxx_Path(xxxPath, "bld");
    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath, progVersion);

    std::vector<sstr> vec1;
    std::vector<sstr> vec2;
    if ((ProperName != "Perl") && (ProperName != "Perl6")) {
        vec1.emplace_back("# ");
        vec1.emplace_back("# make clean");
        vec1.emplace_back("eval \"cd " + srcPath + "; make clean > " + bldPath + "make_clean_results.txt 2>&1 \"");
    }
    vec1.emplace_back("# ");
    vec1.emplace_back("# make ");
    vec1.emplace_back("eval \"cd " + srcPath + "; make > " + bldPath + "make_results.txt 2>&1 \"");
    int result = do_command(buildFileName, vec1, bScriptOnly);
    if (result == 0) {
        vec2.clear();
        vec2.emplace_back("# Make completed successfully.");
    } else {
        vec2.clear();
        vec2.emplace_back("# Make had some problems.");
        vec2.emplace_back("# Look through '" + bldPath + "make_results.txt' to find the issue. ");
    }
    do_command(buildFileName, vec2, bScriptOnly);
    return result;

}

int test_php(sstr& buildFileName, sstr& ProperName, bool bScriptOnly)
{
    std::vector<sstr> vec1;
    if (ProperName == "Php") {
        // do nothing..
        // until I can get "expect" to read the input and "send" some data
        // I don't want the tests to hold up the script.
        vec1.emplace_back("# ");
        vec1.emplace_back("# make test");
        vec1.emplace_back("# The tests must be run manually.");
        vec1.emplace_back("#   So you can answer the questions at the end of the tests.");
        do_command(buildFileName, vec1, bScriptOnly);
    }
    return 0;
}

int test_perl6(sstr& buildFileName, sstr& ProperName, sstr& xxxPath, bool bScriptOnly)
{
    sstr suffix1 = "make_test_results.txt";
    sstr suffix2 = "rakudo_test_results.txt";
    sstr suffix3 = "rakudo_specTest_results.txt";
    sstr suffix4 = "modules_test_results.txt";

    sstr bldPath = get_xxx_Path(xxxPath, "bld");
    sstr srcPath = get_xxx_Path(xxxPath, "src");

    sstr testPathAndFileName1 = joinPathWithFile(bldPath, suffix1);
    sstr testPathAndFileName2 = joinPathWithFile(bldPath, suffix2);
    sstr testPathAndFileName3 = joinPathWithFile(bldPath, suffix3);
    sstr testPathAndFileName4 = joinPathWithFile(bldPath, suffix4);

    std::vector<sstr> vec1;
    vec1.clear();
    vec1.emplace_back("# ");
    vec1.emplace_back("# make test...");
    vec1.emplace_back("# !!! Warning this may take a while...");
    vec1.emplace_back("eval \"cd " + srcPath + "; make test > "            + testPathAndFileName1 + " 2>&1 \"");
    do_command(buildFileName, vec1, bScriptOnly);
    vec1.clear();
    vec1.emplace_back("eval \"cd " + srcPath + "; make rakudo-test > "     + testPathAndFileName2 + " 2>&1 \"");
    do_command(buildFileName, vec1, bScriptOnly);
    vec1.clear();
    vec1.emplace_back("eval \"cd " + srcPath + "; make rakudo-spectest > " + testPathAndFileName3 + " 2>&1 \"");
    do_command(buildFileName, vec1, bScriptOnly);
    vec1.clear();
    vec1.emplace_back("eval \"cd " + srcPath + "; make modules-test > "    + testPathAndFileName4 + " 2>&1 \"");
    do_command(buildFileName, vec1, bScriptOnly);
}


int mariadb_notes(sstr& buildFileName, sstr& notes_File, sstr& ProperName, sstr& progVersion, sstr& rtnPath, sstr& xxxPath, bool bScriptOnly)
{
    std::vector<sstr> vec;
    if (ProperName == "Mariadb") {

        sstr bldPath = get_xxx_Path(xxxPath, "bld");
        sstr usrPath = get_xxx_Path(xxxPath, "usr");
        sstr etcPath = get_xxx_Path(xxxPath, "etc");

        // one of these things is not like the others...on purpose...
        sstr tmpPath = get_xxx_Path(xxxPath, "src");
        sstr srcPath = joinPathParts(tmpPath, progVersion);


        sstr testPathAndFileName = bldPath;
        sstr suffix = "test_results_02.txt";

        auto len = usrPath.find_first_of("usr");
        sstr perlPath;
        perlPath.append(usrPath.substr(0, len - 1));
        perlPath.append("/usr/perl/bin/perl");

        vec.clear();
        testPathAndFileName = joinPathWithFile(testPathAndFileName, suffix);
        vec.emplace_back("# ");
        vec.emplace_back("# optional testing...(after installation and starting)...");
        vec.emplace_back(
                "# eval \"cd " + srcPath + "mysql-test; " + perlPath + " mysql-test-run.pl > " +
                testPathAndFileName + " 2>&1 \"");
        do_command(buildFileName, vec, bScriptOnly);


        vec.clear();
        vec.emplace_back("#################################################################################");
        vec.emplace_back("# ");
        vec.emplace_back("# MariaDB Note Section");
        vec.emplace_back("# ");
        vec.emplace_back("#################################################################################");
        vec.emplace_back("# ");
        vec.emplace_back("#--> Run these commands to secure and start mariadb.");
        vec.emplace_back("groupadd   mysql ");
        vec.emplace_back("useradd -g mysql mysql ");
        //Create required directories if needed
        vec.emplace_back("# ");
        vec.emplace_back("mkdir -p " + usrPath + "data/temp");
        vec.emplace_back("mkdir -p " + usrPath + "run");
        vec.emplace_back("mkdir -p " + usrPath + "var/log");
        // Add required run files
        vec.emplace_back("# ");
        vec.emplace_back("cd "       + usrPath + "run");
        vec.emplace_back("touch mariadb.socket ");
        vec.emplace_back("touch pid ");
        //set permissions for mariadb directory recursively
        vec.emplace_back("# ");
        vec.emplace_back("cd " + usrPath + "../ ");
        vec.emplace_back("chown -R root:mysql mariadb ");
        vec.emplace_back("chmod -R 770        mariadb ");
        //Over ride permissions as required
        vec.emplace_back("# ");
        vec.emplace_back("cd " + usrPath);
        vec.emplace_back("chown -R mysql:mysql data ");
        vec.emplace_back("chmod -R 770         data ");
        vec.emplace_back("chown -R mysql:mysql run  ");
        vec.emplace_back("chmod -R 770         run  ");
        vec.emplace_back("chown -R mysql:mysql var  ");
        vec.emplace_back("chmod -R 770         var  ");
        file_write_vector_to_file(notes_File, vec, false);

        create_my_cnf_File(buildFileName, notes_File, etcPath, usrPath, bScriptOnly);

        vec.clear();
        vec.emplace_back("cd " + usrPath);
        vec.emplace_back("#");
        vec.emplace_back("# script the initial database");
        vec.emplace_back("#   Note 1: If you see a lot of permission errors and the script fails...");
        vec.emplace_back("#           It probably means you need to run chmod o+x on all the directories");
        vec.emplace_back("#            up to the usr/mariadb directory. Once permissions are set up to ");
        vec.emplace_back("#            the mariadb directory, the rest of the permissions should be ok. ");
        vec.emplace_back("#            Don't use chmod -R o+x because that would set all the files as well");
        vec.emplace_back("#              and we only need the directories.");
        vec.emplace_back("#   Note 2:  Note 1 may not be secure enough for you, In that case you must use");
        vec.emplace_back("#               Access Control Lists, and there are too many different user options ");
        vec.emplace_back("#               to create a detailed list here.");
        sstr command = "./scripts/mysql_install_db --user=mysql                   \\ \n";
        command.append("   --defaults-file='");
        command.append(etcPath);
        command.append("my.cnf'     \\ \n");
        command.append("         --basedir='");
        command.append(usrPath.substr(0,usrPath.length()-1));
        command.append("'            \\  \n");
        command.append("         --datadir='");
        command.append(usrPath);
        command.append("data' ");
        command.append("      \\  \n");
        command.append("          --tmpdir='");
        command.append(usrPath);
        command.append("data/temp' ");
        command.append(" \\  \n");
        command.append("          --socket='");
        command.append(usrPath);
        command.append("run/mariadb.socket' ");
        vec.emplace_back(command);
        vec.emplace_back("#");
        vec.emplace_back("# start the database ");
        vec.emplace_back("cd " + usrPath);
        command.clear();
        command.append("./bin/mysqld_safe --hostname=mariadb & ");
        vec.emplace_back(command);
        vec.emplace_back("#");
        vec.emplace_back("#Secure the installation by running:");
        vec.emplace_back("cd " + usrPath);
        command.clear();
        command = "./bin/mysql_secure_installation  --hostname=mariadb & ";
        vec.emplace_back(command);
        vec.emplace_back("#");
        vec.emplace_back("#After securing mariadb start the client console:");
        command.clear();
        command = "./bin/mysql -h mariadb -u root -p ";
        vec.emplace_back(command);
        vec.emplace_back("# ");
        vec.emplace_back("# ");
        vec.emplace_back("# When you want to shutdown run this:");
        vec.emplace_back("cd " + usrPath);

        command.clear();
        command ="./bin/mysqladmin -u root -p shutdown ";
        command.append(" --socket='");
        command.append(usrPath);
        command.append("run/mariadb.socket' & ");
        vec.emplace_back(command);
        file_write_vector_to_file(notes_File, vec, false);

        vec.clear();
        vec.emplace_back("#");
        vec.emplace_back("# See the Installation_Notes on how to setup and start mariadb.");
        vec.emplace_back("eval \"cd " + rtnPath + "\"");
        int result = do_command(buildFileName, vec, bScriptOnly);
        return result;
    }

}

int make_install(sstr& buildFileName, sstr& progVersion, sstr& xxxPath, bool bScriptOnly )
{
    std::vector<sstr> vec;
    sstr bldPath = get_xxx_Path(xxxPath, "bld");
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath, progVersion);

    vec.clear();
    sstr makePathAndFileName = bldPath;
    sstr suffix = "make_install_results.txt";
    makePathAndFileName = joinPathWithFile(makePathAndFileName, suffix);

    vec.emplace_back("# ");
    vec.emplace_back("# make install...");
    vec.emplace_back("eval \"cd " + srcPath + "; make install > " + makePathAndFileName + " 2>&1 \"");
    vec.emplace_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    if (result == 0) {
        vec.clear();
        vec.emplace_back("# Make Install completed successfully.");
    } else {
        vec.clear();
        vec.emplace_back("# Make Install had some problems.");
        vec.emplace_back("# Look through '" + bldPath + "make_install_results.txt' to find the issue. ");
    }
    return result;
}


int basicInstall(sstr& buildFileName, sstr& notes_file,  sstr& ProperName, sstr& configureStr,
                 sstr& xxxPath,       sstr& progVersion, sstr& rtnPath,
                 bool bDebug,         bool bDoTests,     bool bScriptOnly)
{
    sstr bldPath = get_xxx_Path(xxxPath, "bld");
    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath, progVersion);

    int result = 0;
    std::vector<sstr> vec1;
    std::vector<sstr> vec2;
    if (!bDebug) {
        result = configure(buildFileName, configureStr, xxxPath,  bDebug, bScriptOnly);
        if (result == 0) {
            result += make(buildFileName, ProperName, progVersion, xxxPath, bDebug, bScriptOnly);
        }

        if (bDoTests) {
            if (ProperName == "Php")   {
                result += test_php(buildFileName, ProperName, bScriptOnly);
            }
            if (ProperName == "Perl6") {
                test_perl6(buildFileName, ProperName, xxxPath, bScriptOnly);
            }
            sstr testPathAndFileName = bldPath;
            sstr suffix = "test_results.txt";
            testPathAndFileName = joinPathWithFile(testPathAndFileName, suffix);

            vec2.emplace_back("# ");
            vec2.emplace_back("# make test...");
            vec2.emplace_back("# !!! Warning this may take a while...");
            vec2.emplace_back("eval \"cd " + srcPath + "; make test > " + testPathAndFileName + " 2>&1 \"");

            //Most tests have some failures, so we don't want to fail the install because of a test failure.
            //So we don't record the result here.
            do_command(buildFileName, vec2, bScriptOnly);
        }

        // make install starts here...
        if (result == 0)
        {
            result = make_install(buildFileName, progVersion, xxxPath, bScriptOnly);
        }
        if ((result == 0) && (ProperName == "Mariadb")) {
            result += mariadb_notes(buildFileName, notes_file, ProperName, progVersion, rtnPath, xxxPath, bScriptOnly);
        }
    }
    if (result == 0 )
    {
        vec1.clear();
        vec1.emplace_back("# Install was successful.");
    }
    else
    {
        vec1.clear();
        vec1.emplace_back("# Install had some issues.");
        vec1.emplace_back("# Look through the build logs in the '" + bldPath + "' directory.");
    }
    do_command(buildFileName, vec1, bScriptOnly);

    return result;
}



int basicInstall_tcl(sstr& buildFileName, sstr& notes_file,  sstr& ProperName, sstr& configureStr,
                 sstr& srcPath, sstr& xxxPath, sstr& progVersion, sstr& rtnPath,
                 bool bDebug, bool bDoTests, bool bScriptOnly)
{
    int result = -1;
    bool skipTests = false;

    sstr bldPath = get_xxx_Path(xxxPath, "bld");
    sstr tmpPath = "usr/Tcl_Tk";
    sstr usrPath = joinPathParts(rtnPath, tmpPath);

    std::vector<sstr> vec1;
    std::vector<sstr> vec2;

    if (!bDebug) {

        vec1.clear();
        vec1.emplace_back("# ");
        vec1.emplace_back("# Configure...");
        vec1.emplace_back("# Piping results to the \"" + bldPath + "\" directory.");

        // We are ending the command we started with \"
        //   This was started in the configureStr in the calling function.
        configureStr.append(" > " + bldPath + "configure_results.txt 2>&1 \"");
        vec1.emplace_back(configureStr);
        vec1.emplace_back("# ");
        vec1.emplace_back("# make");

        sstr makePathAndFileName = bldPath;
        sstr suffix = "make_results.txt";
        makePathAndFileName = joinPathWithFile(makePathAndFileName, suffix);
        vec1.emplace_back("eval \"cd " + srcPath + "; make -j  > " + makePathAndFileName + " 2>&1 \"");
        result = do_command(buildFileName, vec1, bScriptOnly);

        if (bDoTests) {

            if (!skipTests) {
                sstr testPathAndFileName = bldPath;
                sstr suffix = "test_results.txt";
                testPathAndFileName = joinPathWithFile(testPathAndFileName, suffix);

                vec2.emplace_back("# ");
                vec2.emplace_back("# make test");
                vec2.emplace_back("# !!! Warning this may take a while...");
                vec2.emplace_back("eval \"cd " + srcPath + "; make test > " + testPathAndFileName + " 2>&1 \"");
                do_command(buildFileName, vec2, bScriptOnly);
            }
        }
        vec1.clear();
        vec1.emplace_back("# ");
        vec1.emplace_back("# make install");
        vec1.emplace_back("eval \"cd " + srcPath + "; make install > " + bldPath + "install_results.txt 2>&1 \"");
        vec1.emplace_back("# ");
        result += do_command(buildFileName, vec1, bScriptOnly);

        vec2.clear();
        vec2.emplace_back("# ");
        vec2.emplace_back("# ");
        vec2.emplace_back("#################################################################################");
        vec2.emplace_back("# ");
        vec2.emplace_back("# Tcl/Tk Note Section");
        vec2.emplace_back("# ");
        vec2.emplace_back("#################################################################################");
        vec2.emplace_back("# ");
        vec2.emplace_back("# ");
        vec2.emplace_back("# To use tcl cd to this directory: ");

        sstr command = "cd ";
        command.append(usrPath);
        command.append("bin/");
        vec2.emplace_back(command);
        vec2.emplace_back("ls -al");
        command.clear();
        command = "# Then run tcl with ./tcl [tab complete] then [enter]";
        vec2.emplace_back(command);
        vec2.emplace_back("# At this point you should see a prompt like % ");
        vec2.emplace_back("# type after prompt--> % info tclversion ");
        vec2.emplace_back("#   and you should see the version you installed. ");
        vec2.emplace_back("#   add the path to your PATH variable to run this version from any directory.");
        vec2.emplace_back("# type after prompt--> % wish ");
        vec2.emplace_back("#   and you should see a small new window open up. ");
        vec2.emplace_back("#   depending on how you installed this you may have change permissions.");
        vec2.emplace_back("#   chmod o+rx in the directory tree as appropriate.");
        vec2.emplace_back("#  Some reference material ");
        vec2.emplace_back("#    1. The Tcl and TK programming for the Absolute Beginner by Kurt Wall");
        vec2.emplace_back("#    2. The TCL Programming Language by Ashok P. Nadkarni");
        vec2.emplace_back("#    3. The TCL and the Tk Toolkit by John K. Outsterhout and Ken Jones");
        vec2.emplace_back("#    4. The TCL/Tk A Developers Guide by Clif Flynt");
        vec2.emplace_back("# ");
        file_write_vector_to_file(notes_file, vec2, false);
    }
    if (result == 0 )
    {
        vec1.clear();
        vec1.emplace_back("# Install was successful.");
    }
    else
    {
        vec1.clear();
        vec1.emplace_back("# Install had some issues.");
        vec1.emplace_back("# Look through the build logs in the '" + bldPath + "' directory.");
    }
    do_command(buildFileName, vec1, bScriptOnly);
    return result;
}


bool programNotProtected(std::map<sstr, sstr> settings,
                        sstr& buildFileName, sstr& ProperName, sstr& protectedFileName,
                        sstr& srcPath,   sstr& xxxPath,
                        bool bScriptOnly)
{
    std::vector<sstr> vec;

    sstr usrPath         = get_xxx_Path(xxxPath, "usr");
    sstr protectModeText = settings[KEY_PROTECT_MODE];

    bool bProtectMode = getBoolFromString(protectModeText);
    bool bInstall = true;

    sstr protectionFileWithPath = srcPath + "/" + protectedFileName;

    if (isFileSizeGtZero(protectionFileWithPath, false)) {
        bProtectMode = true;
        bInstall = false;
        howToRemoveFileProtection(buildFileName, ProperName, srcPath, protectedFileName, bScriptOnly);
    }
    if ((bProtectMode) && (directoryExists(usrPath)))
    {
        bInstall = false;
        vec.emplace_back("# The " + ProperName + " usr directory (" + usrPath + ") exists already.");
        vec.emplace_back("# This will prevent the installation out of caution. ");
        vec.emplace_back("# Remove this directory to install this program.");
        vec.emplace_back("# you can use the command below to do it: (without the '#')");
        vec.emplace_back("# rm -rf " + usrPath);
        do_command(buildFileName, vec, bScriptOnly);
    }
    return bInstall;
}

void createProtectionWhenRequired(int result, sstr& buildFileName, sstr& protectedFileName, sstr& srcPath,
                                 sstr& ProperName, bool bProtectMode, bool bScriptOnly )
{
    std::vector<sstr> vec;
    if ((bProtectMode) && (result == 0))
    {
        vec.clear();
        vec.emplace_back("#");
        protectProgram_IfRequired(buildFileName, protectedFileName, srcPath, bProtectMode, bScriptOnly);
        howToRemoveFileProtection(buildFileName, ProperName, srcPath, protectedFileName, bScriptOnly);
        do_command(buildFileName, vec, bScriptOnly);
    }
}

sstr get_InstallOS(sstr& thisOS )
{
    // from above...
    // enum class OS_type
    // { Selection_Not_Available = -1,
    //   No_Selection_Made = 0,
    //   CentOS = 1,
    //   Fedora = 2,
    //   Linux_Mint = 3,
    //   RedHat = 4,
    //   MaxOSX = 5};
    sstr installOS = "unknown";
    if (thisOS == "1")  { installOS = "unix";  }
    if (thisOS == "2")  { installOS = "unix";  }
    if (thisOS == "3")  { installOS = "unix";  }
    if (thisOS == "4")  { installOS = "unix";  }
    if (thisOS == "5")  { installOS = "macosx";}
    return  installOS;
}



int install_perl5(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "perl";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read

    //This part needs explanation....
    //  Perl now has two versions.
    //  Perl5 and Perl6, and Perl6 is not backwards compatible
    //  So in the settings I use Perl5 and Perl6
    //  As for the "program name" Perl5 is just Perl

    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "5->WGET"];        // We are adding 5 to match perl5 in the settings
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "5->Version"];      // We are adding 5 to match perl5 in the settings
    sstr compression   = settings[programName + "5->Compression"];  // We are adding 5 to match perl5 in the settings
    sstr scriptOnly    = settings[programName + "5->Script_Only"];  // We are adding 5 to match perl5 in the settings
    sstr doTests       = settings[programName + "5->Do_Tests"];     // We are adding 5 to match perl5 in the settings
    sstr debugOnly     = settings[programName + "5->Debug_Only"];   // We are adding 5 to match perl5 in the settings
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  srcPath,   xxxPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                        rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        if (result == 0) {
            sstr configureStr = "eval \"cd " + srcPath + "; ./Configure -Dprefix=" + usrPath + " -d -e ";
            result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                                   xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);
        }

        if (result == 0) {
            createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                         bProtectMode, bScriptOnly);
        }
    }
    return result;
}

int install_mariadb(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "mariadb";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + srcPath + "; ./BUILD/autorun.sh; "
                            + " cd " + srcPath + "; " + "./configure --prefix=" + usrPath + " " + "\\\n"
                            + "--enable-assembler                  "  + "\\\n"
                            + "--jemalloc_static_library=/usr/lib64"  + "\\\n"
                            + "--with-extra-charsets=complex       "  + "\\\n"
                            + "--enable-thread-safe-client         "  + "\\\n"
                            + "--with-big-tables                   "  + "\\\n"
                            + "--with-plugin-maria                 "  + "\\\n"
                            + "--with-aria-tmp-tables              "  + "\\\n"
                            + "--without-plugin-innodb_plugin      "  + "\\\n"
                            + "--with-mysqld-ldflags=-static       "  + "\\\n"
                            + "--with-client-ldflags=-static       "  + "\\\n"
                            + "--with-readline                     "  + "\\\n"
                            + "--with-ssl                          "  + "\\\n"
                            + "--with-embedded-server              "  + "\\\n"
                            + "--with-libevent                     "  + "\\\n"
                            + "--with-mysqld-ldflags=-all-static   "  + "\\\n"
                            + "--with-client-ldflags=-all-static   "  + "\\\n"
                            + "--with-zlib-dir=bundled             "  + "\\\n"
                            + "--enable-local-infile ";

        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);


        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );
    }
    return result;
}

int install_perl6(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "perl6";
    //sstr programName       = "rakudo-star";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read

    //This part needs explanation....
    //  Perl now has two versions.
    //  Perl5 and Perl6, and Perl6 is not backwards compatible
    //  So in the settings I use Perl5 and Perl6
    //  As for the "program name" Perl5 is just Perl
    //  Program Name for "Perl6 is Perl6.
    //  But since the setting prefix Perl6 = the Program name Perl6 we don't have to do anything funky here.

    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr fileName      = settings[programName + "->FileName"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    // we need this path for running perl5 as part of the configure statement
    sstr usrPathPerl5Executable  = settings[programName + "->Perl5_Executable"];


    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  fileName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + srcPath + "; " + usrPathPerl5Executable + "perl Configure.pl "
                              + " --backend=moar --gen-moar --prefix=" + usrPath + "";
        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);

        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );
    }
    return result;
}

int install_ruby(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "ruby";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr bldPath       = settings[programName + "->BLD_Path"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + srcPath + "; ./configure --prefix=" + usrPath + " ";

        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);


        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );

    }
    return result;
}

int install_apache_step_01(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "apr";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr bldPath       = settings[programName + "->BLD_Path"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);
    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  srcPath,   xxxPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd "     + srcPath + "; ./configure --prefix=" + usrPath + " ";
        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);

        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );
    }
    return result;
}

int install_apache_step_02(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "apr-util";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr bldPath       = settings[programName + "->BLD_Path"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + srcPath + "; ./configure --prefix="
                            + usrPath + "  --with-apr="
                            + usrPath.substr(0, (usrPath.length()-10)) + "/apr/bin ";
        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);

        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );
    }
    return result;
}

int install_apache_step_03(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "apr-iconv";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr bldPath       = settings[programName + "->BLD_Path"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  srcPath,   xxxPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + srcPath + "; ./configure --prefix="
                            + usrPath    + "  --with-apr="
                            + usrPath.substr(0,(usrPath.length()-11)) + "/apr/bin ";

        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);

        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );
    }
    return result;
}

int install_apache_step_04(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "pcre";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr bldPath       = settings[programName + "->BLD_Path"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + srcPath + "; ./configure --prefix=" + usrPath + " ";
        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);

        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );
    }
    return result;
}

int install_apache_step_05(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "pcre2";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr bldPath       = settings[programName + "->BLD_Path"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + srcPath + "; ./configure --prefix=" + usrPath + " ";
        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);

        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );

    }
    return result;
}

int install_apache(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "apache";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr bldPath       = settings[programName + "->BLD_Path"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  "httpd-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + srcPath + "; "  +
                              " ./configure --prefix=" + usrPath + " "
                            + " --with-apr="       + usrPath.substr(0, (usrPath.length()-8)) + "/apr        "
                            + " --with-apr-util="  + usrPath.substr(0, (usrPath.length()-8)) + "/apr-util   "
                            + " --with-apr-iconv=" + usrPath.substr(0, (usrPath.length()-8)) + "/apr-iconv  "
                            + " --with-pcre="      + usrPath.substr(0, (usrPath.length()-8)) + "/pcre " + " ";
                            + " --enable-so ";

        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);


        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );

    }
    return result;
}

int install_php(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "php";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName      = settings[programName + "->Build_Name"];
    sstr notes_file         = settings[programName + "->Notes_File"];
    sstr getPath            = settings[programName + "->WGET"];
    sstr stgPath            = settings[programName + "->STG_Path"];
    sstr rtnPath            = settings[programName + "->RTN_Path"];
    sstr xxxPath            = settings[programName + "->XXX_Path"];
    sstr version            = settings[programName + "->Version"];
    sstr compression        = settings[programName + "->Compression"];
    sstr scriptOnly         = settings[programName + "->Script_Only"];
    sstr doTests            = settings[programName + "->Do_Tests"];
    sstr debugOnly          = settings[programName + "->Debug_Only"];
    sstr compile_for_debug  = settings[programName + "->Compile_For_Debug"];
    sstr thisOS             = settings[programName + "->This_OS"];

    sstr xdebug_install     = settings[programName + "->Xdebug_Install"];
    sstr xdebug_name        = settings[programName + "->Xdebug_Name"];
    sstr xdebug_version     = settings[programName + "->Xdebug_Version"];
    sstr xdebug_compression = settings[programName + "->Xdebug_Compression"];
    sstr xdebug_wget        = settings[programName + "->Xdebug_WGET"];
    sstr xdebug_tar_options = settings[programName + "->Xdebug_Tar_Options"];

    sstr zts_version        = settings[programName + "->zts_version"];

    bool bScriptOnly        = getBoolFromString(scriptOnly);
    bool bDoTests           = getBoolFromString(doTests);
    bool bDebug             = getBoolFromString(debugOnly);
    bool bCompile_For_Debug = getBoolFromString(compile_for_debug);
    bool bInstall_Xdebug    = getBoolFromString(xdebug_install);
    bool bInstall           = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr xDebugProgVersion        =  xdebug_name + "-" + xdebug_version;
    sstr xDebugCompressedFileName =  xDebugProgVersion + xdebug_compression;

    sstr etcPath = get_xxx_Path(xxxPath, "etc");
    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");
    sstr bldPath = get_xxx_Path(xxxPath, "bld");

         tmpPath = "usr/apache/bin";
    sstr apePath = joinPathParts(rtnPath, tmpPath);

    sstr tmpFile = "apachectl";
    sstr apacheController  = joinPathWithFile(apePath, tmpFile);

         tmpFile = "mysqld";
    sstr mariadbPath = rtnPath + "usr/mariadb/bin";
    sstr mariadbController = joinPathWithFile(mariadbPath, tmpFile);

    if ((isFileSizeGtZero(apacheController)) && isFileSizeGtZero(mariadbController)) {

        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# Attempt to Start/Restart Apache if possible.");
        vec.emplace_back("#   If you see an error: could not bind to address [::]:80");
        vec.emplace_back("#   It most likely means Apache is already online.");
        vec.emplace_back("eval \"cd " + apePath + "; ./apachectl -k restart \"");
        do_command(buildFileName, vec, bScriptOnly);
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# MariaDB Database Server files appear to be installed.");
        vec.emplace_back("#   Not attempting to start mariadb. ");
        vec.emplace_back("#   MariaDB requires more setup. Follow the instructions");
        vec.emplace_back("#   in '" + bldPath + "Installation_Notes/p???.txt' " );
        vec.emplace_back("#   To start MariaDB." );
        do_command(buildFileName, vec, bScriptOnly);

        // staging
        result = EnsureStageDirectoryExists(buildFileName, ProperName, stgPath, bScriptOnly);

        if (result == 0)
        {
            bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName, srcPath,   xxxPath,    bScriptOnly);
        }

        if (bInstall) {

            // PHP install is special in so many ways...
                if (!(isFileSizeGtZero(stagedFileName))) {

                    vec.clear();
                    // this will download the file with the fileName of mirror
                    vec.emplace_back(
                            "eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "/from/this/mirror\"");
                    // copy to the compressedFileName
                    vec.emplace_back("eval \"cd " + stgPath + "; cp " + "mirror " + compressedFileName + "\"");
                    // remove the mirror file
                    vec.emplace_back("eval \"cd " + stgPath + "; rm -f " + "mirror\"");
                    // We need to start apache for this to work...
                    do_command(buildFileName, vec, bScriptOnly);
                }

                result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                                 rtnPath, stgPath, xxxPath, bScriptOnly);

                //
                // Note: Don't end the command with \" to close the command here.
                //   We are going to append more to the command in the function
                //     and end the command with \" there.
                sstr configureStr = "eval \"set PKG_CONFIG_PATH /usr/lib64/pkgconfig; ";
                configureStr.append("cd ");
                configureStr.append(srcPath);
                configureStr.append("; ./configure");
                configureStr.append("  --prefix=");
                configureStr.append(usrPath);
                configureStr.append("  --exec-prefix=");
                configureStr.append(usrPath);
                configureStr.append("  --srcdir=");
                configureStr.append(srcPath);
                tmpPath = "usr/apache/bin/";
                sstr apxPathFile = joinPathParts(rtnPath, tmpPath);
                sstr tmpFile = "apxs";
                apxPathFile = joinPathWithFile(apePath, tmpFile);
                configureStr.append("  --with-apxs2=");
                configureStr.append(apxPathFile);
                configureStr.append("  --enable-mysqlnd ");
                tmpPath = "usr/mariadb/";
                sstr mdbPath = joinPathParts(rtnPath, tmpPath);
                configureStr.append("  --with-pdo-mysql=");
                configureStr.append(mdbPath);
                sstr pcePath = "/usr/pcre";
                pcePath = joinPathParts(rtnPath, pcePath);
                configureStr.append("  --with-pcre-regex=");
                configureStr.append(pcePath);
                configureStr.append("  --with-config-file-path=");
                tmpPath = "lib";
                sstr libPath = joinPathParts(usrPath, tmpPath);
                configureStr.append(libPath);
                configureStr.append("  --with-config-file-scan-dir=");
                configureStr.append(etcPath);
                sstr crlPath = "/usr/bin";
                configureStr.append("  --with-curl=");
                configureStr.append(crlPath);
                configureStr.append("  --with-mysql-sock=");
                tmpPath = "usr/mariadb/run/";
                sstr sckPathFile = joinPathParts(rtnPath, tmpPath);
                tmpFile = "mariadb.socket";
                sckPathFile = joinPathWithFile(sckPathFile, tmpFile);
                configureStr.append("  --enable-embedded-mysqli");
                configureStr.append("  --disable-cgi ");
                configureStr.append("  --disable-short-tags ");
                configureStr.append("  --enable-bcmath ");
                configureStr.append("  --with-pcre-jit ");
                configureStr.append("  --enable-sigchild ");
                configureStr.append("  --enable-libgcc ");
                configureStr.append("  --enable-calendar ");
                configureStr.append("  --enable-dba=shared");
                configureStr.append("  --enable-ftp");
                configureStr.append("  --enable-intl");
                configureStr.append("  --enable-mbstring");
                configureStr.append("  --enable-zip");
                configureStr.append("  --enable-zend-test");
                if (bCompile_For_Debug) {
                    configureStr.append("  --enable-debug");
                }

                result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                                       xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);


                // for php only
                vec.clear();
                vec.emplace_back("eval \"cd " + usrPath + "; mkdir -p libs \"");
                result += do_command(buildFileName, vec, bScriptOnly);

                vec.clear();
                vec.emplace_back("# ");
                vec.emplace_back("# Copy Php.ini files to '" + etcPath + "'");
                vec.emplace_back("eval \"cd " + srcPath + "; cp *.ini* " + etcPath  + " \"");
                vec.emplace_back("# ");
                vec.emplace_back("# libtool --finish");
                vec.emplace_back("eval \"cd " + srcPath + "; cp libs/* " + usrPath + "libs/. \"");
                vec.emplace_back("eval \"cd " + srcPath + "; ./libtool --finish " + usrPath + "libs \"");
                vec.emplace_back("# ");
                vec.emplace_back("# Change apache permissions");
                vec.emplace_back("eval \"chmod 755 " + rtnPath + "usr/apache/modules/libphp7.so \"");



                result += do_command(buildFileName, vec, bScriptOnly);


                sstr xdebugProgVersionCompression = xdebug_version + xdebug_compression;

                // When analyzing code, view this whole block together...
                //    if we install Xdebug, are we installing for PHP (debug mode) or PHP (non debug mode)?
                //    depending on the mode of PHP we need to change some text below...
                if (bInstall_Xdebug) {
                    vec.clear();
                    vec.emplace_back("# ");
                    vec.emplace_back("# wget xdebug");
                    vec.emplace_back("eval \"cd " + usrPath + "; wget " + xdebug_wget + xDebugCompressedFileName + " \"");
                    vec.emplace_back(
                            "eval \"cd " + usrPath + "; tar " + xdebug_tar_options + " " + xDebugCompressedFileName +
                            " \"");
                    vec.emplace_back("# ");
                    vec.emplace_back("# phpize");
                    vec.emplace_back("eval \"cd " + usrPath + "; cd " + xDebugProgVersion + "; ../bin/phpize > "
                                     + bldPath + "phpize.txt \"");
                    vec.emplace_back("# ");
                    vec.emplace_back("# config");
                    vec.emplace_back("eval \"cd " + usrPath + xDebugProgVersion + "; ./configure --with-php-config="
                                     + usrPath + "bin/php-config > " + bldPath + "xdebug-configure.txt \"");
                    vec.emplace_back("# ");
                    vec.emplace_back("# make");
                    vec.emplace_back("eval \"cd " + usrPath + xDebugProgVersion + "; make > "
                                     + bldPath + "xdebug-make.txt \"");
                    result += do_command(buildFileName, vec, bScriptOnly);

                    vec.clear();
                    vec.emplace_back("# ");
                    vec.emplace_back("# cp modules/xdebug.so");

                    // checking for the mode of PHP and adjusting accordingly
                    if (bCompile_For_Debug) {
                        vec.emplace_back("eval \"cd " + usrPath + xDebugProgVersion + "; cp modules/xdebug.so "
                                         + usrPath + "lib/php/extensions/debug-zts-" + zts_version + " \"");

                    } else {
                        vec.emplace_back("eval \"cd " + usrPath + xDebugProgVersion + "; cp modules/xdebug.so "
                                         + usrPath + "lib/php/extensions/no-debug-zts-" + zts_version + " \"");

                    }
                    result += do_command(buildFileName, vec, bScriptOnly);

                    // This small section is the same for PHP
                    //   regardless of the debug / non-debug mode.
                    vec.clear();
                    vec.emplace_back("# ");
                    vec.emplace_back("# Create: " + etcPath + "lib");
                    vec.emplace_back("eval \"mkdir -p " + etcPath + "lib \"");
                    // end of small section

                    // checking for the mode of PHP and adjusting accordingly
                    if (bCompile_For_Debug) {
                        vec.emplace_back("# ");
                        vec.emplace_back("# zend_extension = " + usrPath + "lib/php/extensions/debug-zts-" + zts_version +
                                         "/xdebug.so");
                        vec.emplace_back("eval \"cd " + etcPath + "lib/; echo zend_extension = "
                                         + usrPath + "lib/php/extensions/debug-zts-" + zts_version +
                                         "/xdebug.so > php_ext.ini \"");
                    } else {
                        vec.emplace_back("# ");
                        vec.emplace_back("# zend_extension = " + usrPath + "lib/php/extensions/debug-zts-" + zts_version +
                                         "/xdebug.so");
                        vec.emplace_back("eval \"cd " + etcPath + "lib/; echo zend_extension = "
                                         + usrPath + "lib/php/extensions/no-debug-zts-" + zts_version +
                                         "/xdebug.so > php_ext.ini \"");
                    }
                } else {
                    vec.emplace_back("# Xdebug not installed.");
                }
                result += do_command(buildFileName, vec, bScriptOnly);
                // end of code block


                createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                             bProtectMode, bScriptOnly);

        }
    }
    else {
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# Apache Web Server and MariaDB is required ");
        vec.emplace_back("# to be installed before PHP can be installed.");
        result += do_command(buildFileName, vec, bScriptOnly);
    }

    if (bInstall)
    {
        return  result;
    }
    else
    {
        return -1;
    }

}

int install_poco(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "poco";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr bldPath       = settings[programName + "->BLD_Path"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version + "-all";
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);


    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.

        sstr configureStr = "eval \"cd " + srcPath + "; ./configure --prefix=" + usrPath + " ";

        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);


        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );
    }
    return result;
}

int install_python(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "python";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  ProperName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.

        sstr configureStr = "eval \"cd " + srcPath + "; ./configure --prefix=" + usrPath + " ";

        result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);


        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );

    }
    return result;
}

int install_postfix(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "postfix";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    //bool bDoTests    = getBoolFromString(doTests);
    //bool bDebug      = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + "-" + version;
    sstr compressedFileName =  progVersion + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,progVersion);
    sstr usrPath = get_xxx_Path(xxxPath, "usr");

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        /*
         * Currently Installation of this is not supported...
         *   Maybe later when I know more about it.
         *
         *  // Note: Don't end the command with \" to close the command here.
         *  //   We are going to append more to the command in the function
         *  //     and end the command with \" there.
         *
         * sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + " ";
         *
         * result += basicInstall(buildFileName, notes_file, ProperName, configureStr,
         *                      xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);
         *
         * createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
         *                            bProtectMode,  bScriptOnly );
         */
    }
    return result;
}

 int install_tcl(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "tcl";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr installOS = get_InstallOS(thisOS);

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr progVersion        =  programName + version + "-src";
    sstr compressedFileName =  progVersion + compression;
         progVersion        =  programName + version;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,  progVersion);
         srcPath = joinPathParts(srcPath, installOS);
         tmpPath = "usr/Tcl_Tk";
    sstr usrPath = joinPathParts(rtnPath, tmpPath);

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);


        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd ";
        configureStr.append(srcPath);
        configureStr.append("; ");
        configureStr.append(" ./configure --prefix=");
        configureStr.append(usrPath);
        configureStr.append(" --enable-threads");
        configureStr.append(" --enable-shared ");
        configureStr.append(" --enable-symbols");
        configureStr.append(" --enable-64bit ");
        result += basicInstall_tcl(buildFileName, notes_file, ProperName, configureStr, srcPath,
                               xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);

        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );

    }
    return result;
}


int install_tk(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "tk";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read

    //Need tcl install values for tk install
    sstr tclxxxPath    = settings["tcl->XXX_Path"];
    sstr tclVersion    = settings["tcl->Version"];

    //Need for tk install
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr notes_file    = settings[programName + "->Notes_File"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr xxxPath       = settings[programName + "->XXX_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);
    bool bInstall      = false;

    sstr installOS = get_InstallOS(thisOS);
    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr tclPath            = get_xxx_Path(tclxxxPath, "src");
    sstr tclProgVersion     = "tcl" + tclVersion;
    sstr tclConfigPath      = joinPathWithFile(tclPath,  tclProgVersion );
         tclConfigPath      = joinPathWithFile(tclConfigPath, installOS );

    sstr progVersion        = programName + version + "-src";
    sstr compressedFileName = progVersion + compression;
    progVersion             = programName + version;
    sstr stagedFileName     = joinPathWithFile(stgPath, compressedFileName);

    sstr tmpPath = get_xxx_Path(xxxPath, "src");
    sstr srcPath = joinPathParts(tmpPath,  progVersion);
    srcPath = joinPathParts(srcPath, installOS);
    tmpPath = "usr/Tcl_Tk";
    sstr usrPath = joinPathParts(rtnPath, tmpPath);

    // staging
    EnsureStageDirectoryExists(buildFileName, ProperName,     stgPath, bScriptOnly);
    stageSourceCodeIfNeeded(   buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                   srcPath,   xxxPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         rtnPath, stgPath, xxxPath, bScriptOnly);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd ";
        configureStr.append(srcPath);
        configureStr.append("; ");
        configureStr.append(" ./configure --prefix=");
        configureStr.append(usrPath);
        configureStr.append(" --with-tcl=");
        configureStr.append(tclConfigPath);
        configureStr.append(" --enable-threads");
        configureStr.append(" --enable-shared ");
        configureStr.append(" --enable-symbols");
        configureStr.append(" --enable-64bit ");
        result += basicInstall_tcl(buildFileName, notes_file, ProperName, configureStr, srcPath,
                                   xxxPath, progVersion, rtnPath, bDebug, bDoTests, bScriptOnly);

        createProtectionWhenRequired(result, buildFileName, protectedFileName, srcPath, ProperName,
                                     bProtectMode,  bScriptOnly );
    }
    return result;
}

int reportResults(time_t startTime, sstr& fileNameBuilds, sstr& fileNameResult, sstr& programName, sstr& version, int step, int installResult)
{
    int result = 0;
    programName = getProperNameFromString(programName);
    if (step < 0)
    {
        std::cout << "Install " << programName << "-" + version << " result = " << installResult << "." << std::endl;
    }
    else
    {
        std::cout << "Install " << programName << "-" + version << " step " << std::setw(2) << std::setfill('0') << step <<  " : Result = " << installResult << std::endl;
    }

    time_t stop = get_Time();

    result += file_append_results(fileNameBuilds, programName, version, step, installResult, stop, startTime);
    result += file_append_results(fileNameResult, programName, version, step, installResult, stop, startTime);
    print_blank_lines(2);
    return result;
};

int logFinalSettings(sstr& fileNameBuilds, std::map<sstr, sstr>& settings, sstr& programName )
{
    int max_set_width = 32;
    sstr tempProgramName = programName + "->";
    sstr pad_string;
    sstr str_buffer;
    std::vector<sstr> generated_settings;
    std::cout << std::endl << std::endl;
    generated_settings.emplace_back("#                 Listing Settings : Values ");
    generated_settings.emplace_back("#============================================================================");
    for (auto element : settings)
    {

        if (element.first.substr(0, tempProgramName.length()) == tempProgramName) {
            auto pad_length = max_set_width - element.first.length();
            pad_string = sstr(pad_length, ' ');
            str_buffer = ": "  + pad_string + element.first + " : " + element.second;
            generated_settings.push_back(str_buffer);
        }
    }
    file_write_vector_to_file(fileNameBuilds, generated_settings);
    return 0;
}

int process_section(time_t startTime,
                    sstr& fileNameResult,
                    sstr& fileName_Build,
                    sstr& fileName_Notes,
                    std::map<sstr, sstr>& settings,
                    sstr& programName,
                    sstr& version,
                    int (*pfunc)(std::map<sstr, sstr>& settings, bool bProtectMode),
                    int step,
                    bool protectMode)
{
    int result = -1;
    sstr searchStr;

    programName = lowerCaseString(programName);

    if (programName != "dependencies") {
        searchStr = getProperNameFromString(programName);
        searchStr = searchStr + "-" + version;
    } else {
        searchStr = "Dependencies";
    }

    bool sectionLoaded = prior_Results(fileNameResult, searchStr);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        logFinalSettings(fileName_Build, settings, programName);
        result = pfunc(settings, protectMode);
        reportResults( startTime, fileName_Build, fileNameResult, programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }
    return result;

}

sstr set_settings(std::map<sstr,sstr>& settings, sstr& programName, sstr& fileName_Build, sstr& fileName_Notes,
                  sstr& company, sstr& basePath, sstr& stgPath,     sstr& xxxPath)
{
    sstr skip  = settings[programName + "->Skip"];
    if (programName == "perl")
    {
        skip  = settings[programName + "5->Skip"];
    }
    bool bSkip = getBoolFromString(skip);
    if (bSkip)
    {
        return skip_value;
    }

    sstr stgPathforProgram = joinPathParts(stgPath, programName);

    settings[programName + "->Build_Name"]  = fileName_Build;
    settings[programName + "->Notes_File"]  = fileName_Notes;
    settings[programName + "->STG_Path"]    = stgPathforProgram;
    settings[programName + "->RTN_Path"]    = basePath;
    settings[programName + "->XXX_Path"]    = joinPathParts(xxxPath,  programName);
    sstr version = settings[programName + "->Version"];
    settings.emplace(std::pair<sstr,sstr>(programName + "->Perl5_Executable",""));
    if (programName == "perl")
    {
        version = settings[programName + "5->Version"];
    }
    if (programName == "perl6")
    {
        sstr perl5path = "usr/perl/bin";
        sstr path      = joinPathParts(basePath,  perl5path);
        settings[programName + "->Perl5_Executable"]    = path;
    }
    return version;
}

/*
 * Main Starts Here
 *
 */

int main() {

    OS_type thisOS;
    Mac_PM mpm;
    sstr prefix;
    sstr protectModeText;
    bool bProtectMode;
    time_t programStart;
    time_t programStop;
    time_t itemStartTime;
    time_t itemStop_Time;

    programStart = get_Time();

    //sstr theDateTime = make_fileName_dateTime(0);

    // get settings from file
    std::map<sstr, sstr> settings;
    sstr fileSettings = "./Install_Settings.cfg";
    settings = getProgramSettings(fileSettings);
    std::vector<sstr> generated_settings;

    // put settings into program variables
    std::cout << "Putting settings into program variables..." << std::endl;
    sstr pricomy = "/usr/local/j5c";
    sstr company = settings[KEY_COMPANY_NAME];
    sstr usePrefix = settings[KEY_TOUSE_PREFIX];
    bool bUsePrefix = getBoolFromString(usePrefix);

    if (bUsePrefix) {
        prefix = settings[KEY_DEFLT_PREFIX];
        company = joinPathParts(prefix, company);
    } else {
        sstr beginPath = "/";
        company = joinPathParts(beginPath, company);
    }

    sstr pVersion = settings[KEY_PATH_VERSION];
    pVersion = getValid_pVersion(pVersion);

    // assign a default; and change if not correct...
    thisOS = OS_type::RedHat;
    mpm = Mac_PM::No_Selection_Made;
    sstr theOStext = settings[KEY_AN_OS_SYSTEM];
    theOStext = lowerCaseString(theOStext);
    if (theOStext == "cent os")    { thisOS = OS_type::CentOS;      }
    if (theOStext == "cent_os")    { thisOS = OS_type::CentOS;      }
    if (theOStext == "centos")     { thisOS = OS_type::CentOS;      }
    if (theOStext == "fedora")     { thisOS = OS_type::Fedora;      }
    if (theOStext == "linux mint") { thisOS = OS_type::Linux_Mint;  }
    if (theOStext == "linux_mint") { thisOS = OS_type::Linux_Mint;  }
    if (theOStext == "linuxmint")  { thisOS = OS_type::Linux_Mint;  }
    if (theOStext == "red hat")    { thisOS = OS_type::RedHat;      }
    if (theOStext == "red_hat")    { thisOS = OS_type::RedHat;      }
    if (theOStext == "redhat")     { thisOS = OS_type::RedHat;      }
    if (theOStext == "OSX") {
        thisOS = OS_type::MaxOSX;
        // TODO add a setting for Homebrew or Mac Ports
        // there currently is no setting for this yet so we will pick the default for now.
        mpm = Mac_PM::MacPorts;
    }

    protectModeText = settings[KEY_PROTECT_MODE];
    bProtectMode = getBoolFromString(protectModeText);

    sstr runDepds = settings[KEY_RUN_DEPENDCS];
    bool runDependencies = getBoolFromString(runDepds);

    bool sectionLoaded;

    sstr version;
    sstr compression;
    sstr temp = "p" + pVersion;
    sstr basePath = joinPathParts(company, temp);
    temp = STG_NAME;
    sstr stgPath  = joinPathParts(company, temp);
    temp = "xxx";
    sstr xxxPath  = joinPathParts(basePath, temp);
    sstr programName = "Dependencies";
    sstr tlsPath = get_xxx_Path(xxxPath, "tls");
    sstr wwwPath = get_xxx_Path(xxxPath, "www");

    ensure_Directory_exists1(basePath);
    ensure_Directory_exists1(stgPath);
    ensure_Directory_exists1(tlsPath);
    ensure_Directory_exists1(wwwPath);

    sstr getPath = "xxx";
    sstr buildVersion;

    int step = -1;
    int result = 0;
    bool anyInstalled = false;

    temp = "Installation_Builds_p" + pVersion + ".txt";
    sstr fileName_Build = joinPathWithFile(basePath, temp);

    temp = "Installation_Notes_p" + pVersion + ".txt";
    sstr fileName_Notes = joinPathWithFile(basePath, temp);

    temp = "Installation_Results_p" + pVersion + ".txt";
    sstr fileNameResult = joinPathWithFile(basePath, temp);

    std::vector<sstr> vec;

    create_file(fileName_Build);
    do_command(fileName_Build, vec, false);

    if (!(isFileSizeGtZero(fileName_Notes))) {
        create_file(fileName_Notes);
    } else {
        ensure_file(fileName_Notes);
    }

    if (!(isFileSizeGtZero(fileNameResult))) {
        create_file(fileNameResult);
    } else {
        ensure_file(fileNameResult);
    }

    itemStartTime = get_Time();
    sectionLoaded = prior_Results(fileNameResult, programName);
    if (!sectionLoaded) {
        bool bScriptOnly = !runDependencies;
        if ((thisOS == OS_type::RedHat) || (thisOS == OS_type::CentOS)) {
            install_yum_required_dependencies(fileName_Build, programName, bScriptOnly);
            print_blank_lines(2);
        }

        if (thisOS == OS_type::Linux_Mint) {
            install_apt_required_dependencies(fileName_Build, programName, bScriptOnly);
        }

        if (thisOS == OS_type::MaxOSX) {
            install_mac_required_dependencies(mpm);
        }
        reportResults(itemStartTime, fileName_Build, fileNameResult, programName, version, step, result);
    } else {
        section_already_loaded(programName, version);
    }

    //function pointer declaration
    int (*funptr)(std::map<sstr, sstr> &settings, bool bProtectMode);

    struct programs
    {
        sstr progName;
        int  step;
        int (*funptr)(std::map<sstr, sstr> &settings, bool bProtectMode);
    } program;

    std::vector<programs> progVector;

    program.progName = "perl";       program.step     = -1;    program.funptr = &install_perl5;
    progVector.emplace_back(program);

    program.progName = "mariadb";    program.step     = -1;    program.funptr = &install_mariadb;
    progVector.emplace_back(program);

    program.progName = "apr";        program.step     =  1;    program.funptr = &install_apache_step_01;
    progVector.emplace_back(program);

    program.progName = "apr-util";   program.step     =  2;    program.funptr = &install_apache_step_02;
    progVector.emplace_back(program);

    program.progName = "apr-iconv";  program.step     =  3;    program.funptr = &install_apache_step_03;
    progVector.emplace_back(program);

    program.progName = "pcre";       program.step     =  4;    program.funptr = &install_apache_step_04;
    progVector.emplace_back(program);

    program.progName = "pcre2";      program.step     =  5;    program.funptr = &install_apache_step_05;
    progVector.emplace_back(program);

    program.progName = "apache";     program.step     =  -1;   program.funptr = &install_apache;
    progVector.emplace_back(program);

    program.progName = "perl6";      program.step     = -1;    program.funptr = &install_perl6;
    progVector.emplace_back(program);

    program.progName = "php";        program.step     = -1;    program.funptr = &install_php;
    progVector.emplace_back(program);

    program.progName = "poco";      program.step     =  -1;    program.funptr = &install_poco;
    progVector.emplace_back(program);

    program.progName = "postfix";   program.step     =  -1;    program.funptr = &install_postfix;
    progVector.emplace_back(program);

    program.progName = "python";    program.step     =  -1;    program.funptr = &install_python;
    progVector.emplace_back(program);

    program.progName = "ruby";      program.step     = -1;     program.funptr = &install_ruby;
    progVector.emplace_back(program);

    program.progName = "tcl";       program.step     =  1;     program.funptr = &install_tcl;
    progVector.emplace_back(program);

    program.progName = "tk";        program.step     =  2;     program.funptr = &install_tk;
    progVector.emplace_back(program);


    for( const auto& it: progVector )
    {
        programName = it.progName;
        itemStartTime = get_Time();

        version = set_settings(settings, programName, fileName_Build, fileName_Notes, company, basePath, stgPath, xxxPath);

        if (version != skip_value) {
            step = it.step;
            funptr = (it.funptr);
            settings[programName + "->This_OS"] = std::to_string(static_cast<int>(thisOS));
            result = process_section(itemStartTime, fileNameResult, fileName_Build, fileName_Notes,
                                     settings, programName, version, funptr, step, bProtectMode);
            if (result > -1) { anyInstalled = true; }
        }
    }
    if (anyInstalled) {
        programStop = get_Time();
        sstr end = "End of Program";
        file_append_line(fileName_Build, end, programStop, programStart);
        file_append_line(fileNameResult, end, programStop, programStart);
    }
    return 0;
}