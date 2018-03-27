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
constexpr auto daysIn400Years = 146097L;
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


struct an_itemValues
{
    bool bDebug;
    bool bDoTests;
    bool bInstall;
    bool bProtectMode;
    bool bScriptOnly;
    bool bSkip;

    int     result;
    int     step;
    OS_type thisOSType;

    time_t itemStartTime;
    time_t itemStop_Time;

    sstr compression;

    //Paths
    sstr bldPath;
    sstr etcPath;
    sstr getPath;
    sstr rtnPath;
    sstr srcPath;
    sstr srcPathPNV;
    sstr srcPathInstallOS;
    sstr cpyStgPath;
    sstr stgPath;
    sstr tlsPath;
    sstr tmpPath;
    sstr usrPath;
    sstr wwwPath;

    //fileNames
    sstr fileName_Compressed;
    sstr fileName_Staged;
    sstr fileName_Build;
    sstr fileName_Notes;
    sstr fileName_Results;
    sstr fileName_Protection;

    sstr company;
    sstr programName;
    sstr programNameVersion;
    sstr ProperName;
    sstr version;

    sstr cmakeRunPath;
    sstr perl5RunPath;
};

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
    auto max = pow(10,max_width);
    bool preConditionsMet = false;

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

    // We are only showing 3 digits for the year,
    //    and if that is not enough we have other problems...
    int years = static_cast<int>(secondsTotal / average_seconds_in_year);
    long long remainingSeconds = static_cast<long long>(secondsTotal - (years * average_seconds_in_year));

    // We are only showing 3 digits for the year,
    //    and if that is not enough we have other problems...
    if (years > 999) years = 999;
    sstr strYears = padLeftZeros(3, years);

    // There can only be 3 digits here so we are safe with an int
    int days = static_cast<int>(remainingSeconds / seconds_in_day);
    remainingSeconds = remainingSeconds - (days * seconds_in_day);
    sstr strDays = padLeftZeros(3, days);

    // There can only be 2 digits here so we are safe with an int
    int hours = static_cast<int>(remainingSeconds / seconds_in_hour);
    remainingSeconds = remainingSeconds - (hours * seconds_in_hour);
    sstr strHours = padLeftZeros(2, hours);

    // There can only be 2 digits here so we are safe with an int
    int minutes = static_cast<int>(remainingSeconds / seconds_in_minute);
    remainingSeconds = remainingSeconds - (minutes * seconds_in_minute);
    sstr strMinutes = padLeftZeros(2, minutes);

    // There can only be 2 digits here so we are safe with an int
    int seconds = static_cast<int>(remainingSeconds);
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
        time_t start  = get_Time();
        sstr strStart = get_Time_as_String(start);
        result = 0;
        command = it;

        std::cout << " " << strStart << "Command: " << command << std::endl;

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

sstr get_ProtectionFileWithPath(an_itemValues& itemValues)
{
    sstr protectionFileWithPath = itemValues.srcPath;
    protectionFileWithPath = joinPathParts(protectionFileWithPath, itemValues.programNameVersion);
    protectionFileWithPath.append("protection-");
    protectionFileWithPath.append(itemValues.programName);
    protectionFileWithPath.append(".txt");
    return protectionFileWithPath;
}


void protectProgram_IfRequired(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    sstr protectionFileWithPath = get_ProtectionFileWithPath(itemValues);
    if (!(isFileSizeGtZero(itemValues.fileName_Protection, false))) {
        if (itemValues.bProtectMode) {
            ensure_Directory_exists1(itemValues.srcPath);
            vec.emplace_back("eval \"echo 'protection = true' > " + protectionFileWithPath + "\"");
            do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        }
    }
}

void howToRemoveFileProtection(an_itemValues& itemValues)
{
    sstr protectionFileWithPath = get_ProtectionFileWithPath(itemValues);
    std::cout << "itemValues.fileName_Protection = " << itemValues.fileName_Protection << std::endl;
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# This " + itemValues.ProperName + " install is currently protected:");
    vec.emplace_back("# ");
    vec.emplace_back("# To remove this protection, run this command:");
    vec.emplace_back("# eval \" rm -f " + protectionFileWithPath + "\"");
    vec.emplace_back("# ");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
}

void stageSourceCodeIfNeeded(an_itemValues& itemValues)
{

    std::vector<sstr> vec;
    itemValues.getPath.append(itemValues.fileName_Compressed);

    vec.emplace_back("#");
    vec.emplace_back("# Stage source file if needed.");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    vec.clear();
    if (!(isFileSizeGtZero(itemValues.fileName_Staged, true)))
    {
        vec.emplace_back("# Attempting to download file...");
        if (itemValues.programName == "php")
        {
            vec.emplace_back("eval \"cd " + itemValues.stgPath + "; wget " + itemValues.getPath + "/from/this/mirror \"");
        }
        else
        {
            vec.emplace_back("eval \"cd " + itemValues.stgPath + "; wget " + itemValues.getPath + "\"");
        }

    }
    else
    {
        vec.emplace_back("# Source code appears to be staged.");
    }
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

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


int removeWorkingDirectories(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    removeDirectory(itemValues.fileName_Build, itemValues.bldPath, vec);
    removeDirectory(itemValues.fileName_Build, itemValues.etcPath, vec);
    removeDirectory(itemValues.fileName_Build, itemValues.srcPath, vec);

    if (itemValues.ProperName != "Tk") {
        removeDirectory(itemValues.fileName_Build, itemValues.usrPath, vec);
    }
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Directories were deleted successfully.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Directories were not deleted successfully.");
        vec.emplace_back("# Check for valid path and permissions.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    return result;
}

int ensureWrkDirExist(an_itemValues& itemValues)
{
    int result = -1;
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Ensure " + itemValues.ProperName + " working directories exist. ");
    vec.emplace_back("eval \"mkdir -p " + itemValues.bldPath + "\"");
    vec.emplace_back("eval \"mkdir -p " + itemValues.etcPath + "\"");
    vec.emplace_back("eval \"mkdir -p " + itemValues.srcPath + "\"");
    if (itemValues.programName == "tk") {
        vec.emplace_back("#already exists " + itemValues.usrPath + "\"");
    }
    else
    {
        vec.emplace_back("eval \"mkdir -p " + itemValues.usrPath + "\"");
    }
    result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Directories exist or were created.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Directories were not created successfully.");
        vec.emplace_back("# Check for valid path and permissions.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    return result;
}

int createTargetFromStage(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Copy, decompress, and remove compressed file.");
    vec.emplace_back("eval \"cd " + itemValues.stgPath + ";        cp ./"   + itemValues.fileName_Compressed + " " + itemValues.srcPath + "\"");
    vec.emplace_back("eval \"cd " + itemValues.srcPath + "; tar xf " + itemValues.fileName_Compressed + "\"");
    vec.emplace_back("eval \"cd " + itemValues.srcPath + "; rm  -f " + itemValues.fileName_Compressed + "\"");
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Target created.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Target was not created successfully.");
        vec.emplace_back("# Check for valid path and permissions.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    return result;
}

int EnsureStageDirectoryExists(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Ensure stage directory exists.");
    vec.emplace_back("eval \"mkdir -p " + itemValues.stgPath + "\"");
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
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
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

int setupInstallDirectories(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Remove unfinished " + itemValues.ProperName + " install (if any).");
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    if ((itemValues.ProperName == "Tcl") || (itemValues.ProperName == "Tk"))
    {
        sstr tmpPath = "usr/Tcl_Tk";
        itemValues.usrPath = joinPathParts(itemValues.rtnPath, tmpPath);
    }
    result += removeWorkingDirectories(itemValues);
    result += ensureWrkDirExist       (itemValues);
    result += createTargetFromStage   (itemValues);
    return result;
}

int create_my_cnf_File(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    vec.clear();
    vec.emplace_back("#Save the original my.cnf file if any, to have as a guide.");

    //lets add the date_time to the my.cnf.old in case we run this multiple times
    //  in a day we will still have the original file somewhere.

    sstr theDate = make_fileName_dateTime(0);
    vec.emplace_back("eval \"cd /etc; cp my.cnf" + itemValues.etcPath + "my.cnf.old_" + theDate + "\"");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    //Now we have to create the new my.cnf file
    vec.clear();
    vec.emplace_back("#Create new file " + itemValues.etcPath + "my.cnf ");
    vec.emplace_back("eval \"cd /etc; echo ' ' >" + itemValues.etcPath + "my.cnf \"");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    sstr my_cnf_File = itemValues.etcPath + "my.cnf";
    //Now we add the content to the /etc/my.cnf file
    vec.clear();
    vec.emplace_back("[mysqld]");
    vec.emplace_back("user=mysql");
    vec.emplace_back("socket='" + itemValues.usrPath + "run/mariadb.socket'");
    vec.emplace_back("#bind-address=127.0.1.1");
    vec.emplace_back("#port=3306");
    vec.emplace_back(" ");
    vec.emplace_back("#Directories");
    vec.emplace_back("  basedir='" + itemValues.usrPath + "'");
    vec.emplace_back("  datadir='" + itemValues.usrPath + "data/'");
    vec.emplace_back(" ");
    vec.emplace_back("#Log File");
    vec.emplace_back("  log-basename='MariaDB_Logs'");
    vec.emplace_back(" ");
    vec.emplace_back("#Other Stuff");
    vec.emplace_back("skip-networking");
    vec.emplace_back("skip-ssl");
    vec.emplace_back("key_buffer_size=32M");
    vec.emplace_back(" ");
    vec.emplace_back("# Disabling symbolic-links is recommended to prevent assorted security risks symbolic-links=0");
    vec.emplace_back("# Settings user and group are ignored when systemd is used.");
    vec.emplace_back("# If you need to run mysqld under a different user or group,");
    vec.emplace_back("# customize your systemd unit file for mariadb according to the");
    vec.emplace_back("# instructions in http://fedoraproject.org/wiki/Systemd");
    vec.emplace_back(" ");
    vec.emplace_back("[mysqld_safe]");
    vec.emplace_back("log-error='" + itemValues.usrPath + "var/log/mariadb'");
    vec.emplace_back(" pid-file='" + itemValues.usrPath + "run/mariadb_pid'");
    vec.emplace_back(" ");
    vec.emplace_back("#");
    vec.emplace_back("# include all files from the config directory");
    vec.emplace_back("#");
    vec.emplace_back("# Not using anything here....");
    vec.emplace_back("#   in fact no directory or files here");
    vec.emplace_back("#   !includedir " + itemValues.etcPath + "my.cnf.d");
    vec.emplace_back(" ");
    vec.emplace_back("## end of file");
    vec.emplace_back(" ");
    file_write_vector_to_file(my_cnf_File, vec, false);
    return 0;
};

int configure(an_itemValues& itemValues,  sstr& configureStr)
{
    sstr outFileName1 = "pre_make_results.txt";
    sstr outFileName2 = "gmake_results.txt";

    sstr outFileName = outFileName1;
    if (configureStr.find("gmake") != -1)
    {
        outFileName = outFileName2;
    }

    if (!itemValues.bDebug) {
        std::vector<sstr> vec1;
        std::vector<sstr> vec2;
        vec1.emplace_back("# ");
        vec1.emplace_back("# Pre make commands -- usually configure, but not always...");
        vec1.emplace_back("# Piping results to " + itemValues.bldPath + ".");
        // We are ending the command we started here with \"
        //   This was started in the configureStr.
        configureStr.append(" > '" + itemValues.bldPath + outFileName + "' 2>&1 \"");
        vec1.emplace_back(configureStr);
        int result = do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
        if (result == 0) {
            vec2.clear();
            if (configureStr.find("gmake") != -1)
            {
                vec2.emplace_back("# gmake commands completed successfully.");
            }
            else
            {
                vec2.emplace_back("# Pre make commands completed successfully.");
            }

        } else {
            vec2.clear();
            if (configureStr.find("gmake") != -1)
            {
                vec2.emplace_back("# gmake commands had some problems.");
            }
            else
            {
                vec2.emplace_back("# Pre make commands had some problems.");
            }
            vec2.emplace_back("# Look through '" + itemValues.bldPath + outFileName + "' to find the issue. ");
        }
        do_command(itemValues.fileName_Build, vec2, itemValues.bScriptOnly);
        return result;
    }
}

int make(an_itemValues& itemValues)
{
    sstr clnFileName = "make_clean_results.txt";
    sstr mkeFileName = "make_results.txt";

    std::vector<sstr> vec1;
    std::vector<sstr> vec2;
    if ((itemValues.ProperName != "Perl")
        && (itemValues.ProperName != "Perl6")
        && (itemValues.ProperName != "Libzip")
        && (itemValues.ProperName != "Cmake"))
    {
        vec1.emplace_back("# ");
        vec1.emplace_back("# make clean");
        vec1.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; make clean > '" + itemValues.bldPath + clnFileName + "' 2>&1 \"");
    }
    vec1.emplace_back("# ");
    sstr command = "make";
    int result = 0;
    if (itemValues.ProperName == "Cmake")
    {
        // do nothing
    }
    else
    {
        vec1.emplace_back("# " + command);
        vec1.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; " + command + " > '" + itemValues.bldPath + mkeFileName + "' 2>&1 \"");

        result = do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
        if (result == 0) {
            vec2.clear();
            vec2.emplace_back("# Make completed successfully.");
        } else {
            vec2.clear();
            vec2.emplace_back("# Make had some problems.");
            vec2.emplace_back("# Look through '" + itemValues.bldPath + mkeFileName + "' to find the issue. ");
        }
        do_command(itemValues.fileName_Build, vec2, itemValues.bScriptOnly);
    }
    return result;
}

int test_php(an_itemValues& itemValues)
{
    std::vector<sstr> vec1;
    if (itemValues.ProperName == "Php") {
        // do nothing..
        // until I can get "expect" to read the input and "send" some data
        // I don't want the tests to hold up the script.
        vec1.emplace_back("# ");
        vec1.emplace_back("# make test");
        vec1.emplace_back("# The tests must be run manually.");
        vec1.emplace_back("#   So you can answer the questions at the end of the tests.");
        do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
    }
    return 0;
}

int test_perl6(an_itemValues& itemValues)
{
    sstr suffix1 = "make_test_results.txt";
    sstr suffix2 = "rakudo_test_results.txt";
    sstr suffix3 = "rakudo_specTest_results.txt";
    sstr suffix4 = "modules_test_results.txt";

    sstr testPathAndFileName1 = joinPathWithFile(itemValues.bldPath, suffix1);
    sstr testPathAndFileName2 = joinPathWithFile(itemValues.bldPath, suffix2);
    sstr testPathAndFileName3 = joinPathWithFile(itemValues.bldPath, suffix3);
    sstr testPathAndFileName4 = joinPathWithFile(itemValues.bldPath, suffix4);

    std::vector<sstr> vec1;
    vec1.clear();
    vec1.emplace_back("# ");
    vec1.emplace_back("# make test...");
    vec1.emplace_back("# !!! Warning this may take a while...");
    vec1.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; make test > "            + testPathAndFileName1 + " 2>&1 \"");
    do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
    vec1.clear();
    vec1.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; make rakudo-test > "     + testPathAndFileName2 + " 2>&1 \"");
    do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
    vec1.clear();
    vec1.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; make rakudo-spectest > " + testPathAndFileName3 + " 2>&1 \"");
    do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
    vec1.clear();
    vec1.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; make modules-test > "    + testPathAndFileName4 + " 2>&1 \"");
    do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
}


int mariadb_notes(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    if (itemValues.ProperName == "Mariadb") {

        sstr testPathAndFileName = itemValues.bldPath;
        sstr suffix = "test_results_02.txt";

        auto len = itemValues.usrPath.find_first_of("usr");
        sstr perlPath;
        perlPath.append(itemValues.usrPath.substr(0, len - 1));
        perlPath.append("/usr/perl/bin/perl");

        vec.clear();
        testPathAndFileName = joinPathWithFile(testPathAndFileName, suffix);
        vec.emplace_back("# ");
        vec.emplace_back("# optional testing...(after installation and starting)...");
        vec.emplace_back(
                "# eval \"cd " + itemValues.srcPathPNV + "mysql-test; " + perlPath + " mysql-test-run.pl > " +
                testPathAndFileName + " 2>&1 \"");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

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
        vec.emplace_back("mkdir -p " + itemValues.usrPath + "data/temp");
        vec.emplace_back("mkdir -p " + itemValues.usrPath + "run");
        vec.emplace_back("mkdir -p " + itemValues.usrPath + "var/log");
        // Add required run files
        vec.emplace_back("# ");
        vec.emplace_back("cd "       + itemValues.usrPath + "run");
        vec.emplace_back("touch mariadb.socket ");
        vec.emplace_back("touch mariadb_pid ");
        //set permissions for mariadb directory recursively
        vec.emplace_back("# ");
        vec.emplace_back("cd " + itemValues.usrPath + "../ ");
        vec.emplace_back("chown -R root:mysql mariadb ");
        vec.emplace_back("chmod -R 770        mariadb ");
        //Over ride permissions as required
        vec.emplace_back("# ");
        vec.emplace_back("cd " + itemValues.usrPath);
        vec.emplace_back("chown -R mysql:mysql data ");
        vec.emplace_back("chmod -R 770         data ");
        vec.emplace_back("chown -R mysql:mysql run  ");
        vec.emplace_back("chmod -R 770         run  ");
        vec.emplace_back("chown -R mysql:mysql var  ");
        vec.emplace_back("chmod -R 770         var  ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        create_my_cnf_File(itemValues);

        vec.clear();
        vec.emplace_back("cd " + itemValues.usrPath);
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
        sstr command = "./scripts/mysql_install_db ";
        command.append(" --defaults-file='");
        command.append(itemValues.etcPath);
        command.append("my.cnf' ");
        command.append(" --basedir='");
        command.append(itemValues.usrPath.substr(0,itemValues.usrPath.length()-1));
        command.append("' ");
        command.append(" --datadir='");
        command.append(itemValues.usrPath);
        command.append("data' ");
        command.append(" --tmpdir='");
        command.append(itemValues.usrPath);
        command.append("data/temp' ");
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket' ");
        command.append(" --user=mysql");
        vec.emplace_back(command);
        vec.emplace_back("#");
        vec.emplace_back("# start the database ");
        vec.emplace_back("cd " + itemValues.usrPath);
        command.clear();
        command.append("./bin/mysqld_safe ");
        command.append(" --defaults-file='");
        command.append(itemValues.etcPath);
        command.append("my.cnf' ");
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket' ");
        command.append(" --user=mysql &");
        vec.emplace_back(command);
        vec.emplace_back("#");
        vec.emplace_back("#Secure the installation after starting server by running command:");
        vec.emplace_back("cd " + itemValues.usrPath);
        command.clear();
        command = "./bin/mysql_secure_installation ";
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket' ");
        vec.emplace_back(command);
        vec.emplace_back("#");
        vec.emplace_back("#After securing mariadb start the client console:");
        command.clear();
        command = "./bin/mysql ";
        command.append(" --defaults-file='");
        command.append(itemValues.etcPath);
        command.append("my.cnf' ");
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket' ");
        command.append(" -u root -p ");
        vec.emplace_back(command);
        vec.emplace_back("# ");
        vec.emplace_back("# ");
        vec.emplace_back("# When you want to shutdown run this:");
        vec.emplace_back("cd " + itemValues.usrPath);
        command.clear();
        command ="./bin/mysqladmin ";
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket'  ");
        command.append(" -u root -p shutdown ");
        vec.emplace_back(command);
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        vec.clear();
        vec.emplace_back("#");
        vec.emplace_back("# See the Installation_Notes on how to setup and start mariadb.");
        vec.emplace_back("eval \"cd " + itemValues.rtnPath + "\"");
        int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        return result;
    }

}


int make_install(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    vec.clear();
    sstr makePathAndFileName = itemValues.bldPath;
    sstr suffix = "make_install_results.txt";
    makePathAndFileName = joinPathWithFile(makePathAndFileName, suffix);
    vec.emplace_back("# ");
    vec.emplace_back("# make install...");
    vec.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; make install > " + makePathAndFileName + " 2>&1 \"");
    vec.emplace_back("# ");
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (result == 0) {
        vec.clear();
        vec.emplace_back("# Make Install completed successfully.");
    } else {
        vec.clear();
        vec.emplace_back("# Make Install had some problems.");
        vec.emplace_back("# Look through '" + itemValues.bldPath + "make_install_results.txt' to find the issue. ");
    }
    return result;
}


int basicInstall(an_itemValues& itemValues, sstr& configureStr)
{
    int result = 0;
    std::vector<sstr> vec1;
    std::vector<sstr> vec2;
    if (!itemValues.bDebug) {
        if (configureStr.length() > 0) {
            result = configure(itemValues, configureStr);
        }
        if (result == 0) {

            result += make(itemValues);
        }

        if (itemValues.bDoTests) {
            if (itemValues.ProperName == "Php")   {
                result += test_php(itemValues);
            }
            if (itemValues.ProperName == "Perl6") {
                test_perl6(itemValues);
            }
            sstr testPathAndFileName = itemValues.bldPath;
            sstr suffix = "test_results.txt";
            testPathAndFileName = joinPathWithFile(testPathAndFileName, suffix);

            vec2.emplace_back("# ");
            vec2.emplace_back("# make test...");
            vec2.emplace_back("# !!! Warning this may take a while...");
            vec2.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; make test > " + testPathAndFileName + " 2>&1 \"");

            //Most tests have some failures, so we don't want to fail the install because of a test failure.
            //So we don't record the result here.
            do_command(itemValues.fileName_Build, vec2, itemValues.bScriptOnly);
        }

        // make install starts here...
        if (result == 0)
        {
            result = make_install(itemValues);
        }
        if ((result == 0) && (itemValues.ProperName == "Mariadb")) {
            result += mariadb_notes(itemValues);
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
        vec1.emplace_back("# Look through the build logs in the '" + itemValues.bldPath + "' directory.");
    }
    do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);

    return result;
}



int basicInstall_tcl(an_itemValues& itemValues, sstr& configureStr)
{
    int result = -1;
    bool skipTests = false;

    sstr tmpPath = "usr/Tcl_Tk";
    sstr usrPath = joinPathParts(itemValues.rtnPath, tmpPath);

    std::vector<sstr> vec1;
    std::vector<sstr> vec2;

    if (!itemValues.bDebug) {

        vec1.clear();
        vec1.emplace_back("# ");
        vec1.emplace_back("# Configure...");
        vec1.emplace_back("# Piping results to the \"" + itemValues.bldPath + "\" directory.");

        // We are ending the command we started with \"
        //   This was started in the configureStr in the calling function.
        configureStr.append(" > " + itemValues.bldPath + "configure_results.txt 2>&1 \"");
        vec1.emplace_back(configureStr);
        vec1.emplace_back("# ");
        vec1.emplace_back("# make");

        sstr makePathAndFileName = itemValues.bldPath;
        sstr suffix = "make_results.txt";
        makePathAndFileName = joinPathWithFile(makePathAndFileName, suffix);
        vec1.emplace_back("eval \"cd " + itemValues.srcPathInstallOS + "; make -j  > " + makePathAndFileName + " 2>&1 \"");
        result = do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);

        if (itemValues.bDoTests) {

            if (!skipTests) {
                sstr testPathAndFileName = itemValues.bldPath;
                suffix = "test_results.txt";
                testPathAndFileName = joinPathWithFile(testPathAndFileName, suffix);

                vec2.emplace_back("# ");
                vec2.emplace_back("# make test");
                vec2.emplace_back("# !!! Warning this may take a while...");
                vec2.emplace_back("eval \"cd " + itemValues.srcPathInstallOS + "; make test > " + testPathAndFileName + " 2>&1 \"");
                do_command(itemValues.fileName_Build, vec2, itemValues.bScriptOnly);
            }
        }
        vec1.clear();
        vec1.emplace_back("# ");
        vec1.emplace_back("# make install");
        vec1.emplace_back("eval \"cd " + itemValues.srcPathInstallOS + "; make install > " + itemValues.bldPath + "install_results.txt 2>&1 \"");
        vec1.emplace_back("# ");
        result += do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);

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
        file_write_vector_to_file(itemValues.fileName_Notes, vec2, false);
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
        vec1.emplace_back("# Look through the build logs in the '" + itemValues.bldPath + "' directory.");
    }
    do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
    return result;
}

void set_bInstall(an_itemValues &itemValues)
{
    std::vector<sstr> vec;
    sstr protectionFileWithPath = get_ProtectionFileWithPath(itemValues);

    bool bProtected = false;

    if (isFileSizeGtZero(protectionFileWithPath, true)) {
        bProtected = true;
        howToRemoveFileProtection(itemValues);
    }
    if ((bProtected) && (directoryExists(itemValues.usrPath)))
    {
        bProtected = true;
        vec.emplace_back("# The " + itemValues.ProperName + " usr directory (" + itemValues.usrPath + ") exists already.");
        vec.emplace_back("# This will prevent the installation out of caution. ");
        vec.emplace_back("# Remove this directory to install this program.");
        vec.emplace_back("# you can use the command below to do it: (without the '#')");
        vec.emplace_back("# rm -rf " + itemValues.usrPath);
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    if (bProtected)
    {
        itemValues.bProtectMode = true;
        itemValues.bInstall = false;
    }
}

void createProtectionWhenRequired(int result, an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    if ((itemValues.bProtectMode) && (result == 0))
    {
        vec.clear();
        vec.emplace_back("#");
        protectProgram_IfRequired(itemValues);
        howToRemoveFileProtection(itemValues);
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
}

sstr get_InstallOS(OS_type thisOS_Type)
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
    if (thisOS_Type == OS_type::CentOS)     { installOS = "unix";  }
    if (thisOS_Type == OS_type::Fedora)     { installOS = "unix";  }
    if (thisOS_Type == OS_type::RedHat)     { installOS = "unix";  }
    if (thisOS_Type == OS_type::Linux_Mint) { installOS = "unix";  }
    if (thisOS_Type == OS_type::MaxOSX)     { installOS = "macosx";  }

    return  installOS;
}

sstr getProtectedFileName(sstr& programName)
{
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");
    return protectedFileName;
}

int install_cmake(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Unlike all the other install_xxx functions this time we have to call configure
        //   two times, so the first time we call it individually with the bootstrap command.
        //   The second time we pass gmake to the basicInstall function.

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./bootstrap --prefix=" + itemValues.usrPath + " ";
        result += configure(itemValues, configureStr);

        configureStr = "eval \"cd " + itemValues.srcPathPNV + "; gmake ";
        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_libzip(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);
        sstr tmpPath = "build";
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPathPNV, tmpPath);

        vec.clear();
        vec.emplace_back("#");
        vec.emplace_back("# Special Instructions for libzip");
        vec.emplace_back("eval \"mkdir -p " + itemValues.srcPathPNV + "\"");
        vec.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; " + itemValues.rtnPath + "usr/cmake/bin/cmake .. -DCMAKE_INSTALL_PREFIX=" + itemValues.usrPath +"\"");
        result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        if (result == 0) {
            sstr configureStr = "# No configuration command required.";
            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues);
        }
        else
        {
            vec.clear();
            vec.emplace_back("#");
            vec.emplace_back("# non-system cmake failed");
            do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        }
    }
    return result;
}

int install_perl5(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        if (result == 0) {
            sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./Configure -Dprefix=" + itemValues.usrPath + " -d -e ";
            result += basicInstall(itemValues, configureStr);
        }
        if (result == 0) {
            createProtectionWhenRequired(result, itemValues);
        }
    }
    return result;
}

int install_openssl(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./config --prefix="
                            + itemValues.usrPath + " ";
        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_mariadb(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./BUILD/autorun.sh; "
                            + " cd " + itemValues.srcPathPNV + "; " + "./configure --prefix=" + itemValues.usrPath + " " + "\\\n"
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

        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_perl6(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    // we need this path for running perl5 as part of the configure statement
    itemValues.perl5RunPath = settings[itemValues.programName + "->Perl5_Executable"];

    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; " + itemValues.perl5RunPath + "perl Configure.pl "
                              + " --backend=moar --gen-moar --prefix=" + itemValues.usrPath + "";
        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_ruby(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./configure --prefix=" + itemValues.usrPath + " ";
        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_apache_step_01(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd "     + itemValues.srcPathPNV + "; ./configure --prefix=" + itemValues.usrPath + " ";
        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_apache_step_02(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);

    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./configure --prefix="
                            + itemValues.usrPath + "  --with-apr="
                            + itemValues.usrPath.substr(0, (itemValues.usrPath.length()-10)) + "/apr/bin ";
        result += basicInstall(itemValues, configureStr);

        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_apache_step_03(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./configure --prefix="
                            + itemValues.usrPath    + "  --with-apr="
                            + itemValues.usrPath.substr(0,(itemValues.usrPath.length()-11)) + "/apr/bin ";

        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_apache_step_04(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./configure --prefix=" + itemValues.usrPath + " ";
        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_apache_step_05(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./configure --prefix=" + itemValues.usrPath + " ";
        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_apache(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; "
            + " ./configure --prefix=" + itemValues.usrPath + " "
            + " --with-apr="       + itemValues.usrPath.substr(0, (itemValues.usrPath.length()-8)) + "/apr        "
            + " --with-apr-util="  + itemValues.usrPath.substr(0, (itemValues.usrPath.length()-8)) + "/apr-util   "
            + " --with-apr-iconv=" + itemValues.usrPath.substr(0, (itemValues.usrPath.length()-8)) + "/apr-iconv  "
            + " --with-pcre="      + itemValues.usrPath.substr(0, (itemValues.usrPath.length()-8)) + "/pcre " + " "
            + " --enable-so ";

        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_php(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;

    sstr compileForDebug    = settings[itemValues.programName + "->Compile_For_Debug"];
    sstr xdebug_install     = settings[itemValues.programName + "->Xdebug_Install"];
    sstr xdebug_name        = settings[itemValues.programName + "->Xdebug_Name"];
    sstr xdebug_version     = settings[itemValues.programName + "->Xdebug_Version"];
    sstr xdebug_compression = settings[itemValues.programName + "->Xdebug_Compression"];
    sstr xdebug_wget        = settings[itemValues.programName + "->Xdebug_WGET"];
    sstr xdebug_tar_options = settings[itemValues.programName + "->Xdebug_Tar_Options"];
    sstr zts_version        = settings[itemValues.programName + "->zts_version"];
    bool bCompileForDebug   = getBoolFromString(compileForDebug);
    bool bInstall_Xdebug    = getBoolFromString(xdebug_install);

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(itemValues.fileName_Build);

    sstr xDebugProgVersion        =  xdebug_name + "-" + xdebug_version;
    sstr xDebugCompressedFileName =  xDebugProgVersion + xdebug_compression;

    sstr tmpPath = "usr/apache/bin";
    sstr apePath = joinPathParts(itemValues.rtnPath, tmpPath);

    sstr tmpFile = "apachectl";
    sstr apacheController  = joinPathWithFile(apePath, tmpFile);

         tmpFile = "mysqld";
    sstr mariadbPath = itemValues.rtnPath + "usr/mariadb/bin";
    sstr mariadbController = joinPathWithFile(mariadbPath, tmpFile);

    if ((isFileSizeGtZero(apacheController)) && isFileSizeGtZero(mariadbController)) {

        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# Attempt to Start/Restart Apache if possible.");
        vec.emplace_back("#   If you see an error: could not bind to address [::]:80");
        vec.emplace_back("#   It most likely means Apache is already online.");
        // ./apachectl -f /wd3/j5c/p002/etc/apache/apache.conf -k start
        vec.emplace_back("eval \"cd " + apePath + "; ./apachectl -k restart \"");
        int temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        if (temp == 0)
        {
            vec.emplace_back("# ");
            vec.emplace_back("# Restarting Apache Web Server was successful.");
        }
        else
        {
            // The first attempt failed because the server is already running,
            //   But it is probably running under this config file name, so lets try again.
            //   Even though we know it is running somewhere.
            vec.clear();
            vec.emplace_back("# ");
            vec.emplace_back("# The server appears to be online, lets see if we can restart.");
            vec.emplace_back("#     using the most likely configuration file...");
            vec.emplace_back("eval \"cd " + apePath + "; ./apachectl -f "
                             + itemValues.etcPath.substr(0,itemValues.etcPath.length()-4)
                             + "apache/apache.conf -k restart \"");
            temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
            if (temp == 0)
            {
                vec.emplace_back("# ");
                vec.emplace_back("# Restarting Apache Web Server was successful.");
            }
        }

        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# MariaDB Database Server files appear to be installed.");
        vec.emplace_back("#   Not attempting to start mariadb. ");
        vec.emplace_back("#   MariaDB requires more setup. Follow the instructions");
        vec.emplace_back("#   in '" + itemValues.bldPath + "Installation_Notes/p???.txt' " );
        vec.emplace_back("#   To start MariaDB." );
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

        // If the source code was just downloaded the file name is mirror
        // instead of anything useful, so we need to rename the rename the file
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# Change downloaded file name if needed.");
        vec.emplace_back("eval \"cd " + itemValues.stgPath + "; mv -f mirror " + itemValues.fileName_Compressed + "\"");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

        set_bInstall(itemValues);
        if (itemValues.bInstall)
        {
            appendNewLogSection(itemValues.fileName_Build);
            EnsureStageDirectoryExists(itemValues);
            stageSourceCodeIfNeeded(itemValues);
            result = setupInstallDirectories(itemValues);
            itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

            //
            // Note: Don't end the command with \" to close the command here.
            //   We are going to append more to the command in the function
            //     and end the command with \" there.
            sstr configureStr = "eval \"set PKG_CONFIG_PATH /usr/lib64/pkgconfig; ";
            configureStr.append("cd ");
            configureStr.append(itemValues.srcPathPNV);
            configureStr.append("; ./configure");
            configureStr.append("  --prefix=");
            configureStr.append(itemValues.usrPath);
            configureStr.append("  --exec-prefix=");
            configureStr.append(itemValues.usrPath);
            configureStr.append("  --srcdir=");
            configureStr.append(itemValues.srcPathPNV);
            configureStr.append("  --with-openssl=" + itemValues.usrPath.substr(0,itemValues.usrPath.length()-4) + "openssl ");
            tmpPath = "usr/apache/bin/";
            sstr apxPathFile = joinPathParts(itemValues.rtnPath, tmpPath);
                 tmpFile = "apxs";
            apxPathFile = joinPathWithFile(apePath, tmpFile);
            configureStr.append("  --with-apxs2=");
            configureStr.append(apxPathFile);
            configureStr.append("  --enable-mysqlnd ");
            tmpPath = "usr/mariadb/";
            sstr mdbPath = joinPathParts(itemValues.rtnPath, tmpPath);
            configureStr.append("  --with-pdo-mysql=");
            configureStr.append(mdbPath);
            sstr pcePath = "/usr/pcre";
            pcePath = joinPathParts(itemValues.rtnPath, pcePath);
            configureStr.append("  --with-pcre-regex=");
            configureStr.append(pcePath);
            configureStr.append("  --with-config-file-path=");
            tmpPath = "lib";
            sstr libPath = joinPathParts(itemValues.usrPath, tmpPath);
            configureStr.append(libPath);
            configureStr.append("  --with-config-file-scan-dir=");
            configureStr.append(itemValues.etcPath);
            sstr crlPath = "/usr/bin";
            configureStr.append("  --with-curl=");
            configureStr.append(crlPath);
            configureStr.append("  --with-mysql-sock=");
            tmpPath = "usr/mariadb/run/";
            sstr sckPathFile = joinPathParts(itemValues.rtnPath, tmpPath);
            tmpFile = "mariadb.socket";
            sckPathFile = joinPathWithFile(sckPathFile, tmpFile);
            configureStr.append(sckPathFile);
            configureStr.append("  --with-libzip='");
                 tmpPath = "/usr/libzip";
            sstr libZipPath =  joinPathParts(itemValues.rtnPath, tmpPath);
            configureStr.append(libZipPath);
            configureStr.append("' ");

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
            configureStr.append("  --enable-zend-test");
            if (bCompileForDebug) {
                configureStr.append("  --enable-debug");
            }

            result += basicInstall(itemValues, configureStr);


            // for php only
            vec.clear();
            vec.emplace_back("eval \"cd " + itemValues.usrPath + "; mkdir -p libs \"");
            result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

            vec.clear();
            vec.emplace_back("# ");
            vec.emplace_back("# Copy Php.ini files to '" + itemValues.etcPath + "'");
            vec.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; cp *.ini* " + itemValues.etcPath  + ". \"");
            vec.emplace_back("# ");
            vec.emplace_back("# libtool --finish");
            vec.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; cp libs/* " + itemValues.usrPath + "libs/. \"");
            vec.emplace_back("eval \"cd " + itemValues.srcPathPNV + "; ./libtool --finish " + itemValues.usrPath + "libs \"");
            vec.emplace_back("# ");
            vec.emplace_back("# Change apache permissions");
            vec.emplace_back("eval \"chmod 755 " + itemValues.rtnPath + "usr/apache/modules/libphp7.so \"");



            result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);


            sstr xdebugProgVersionCompression = xdebug_version + xdebug_compression;

            // When analyzing code, view this whole block together...
            //    if we install Xdebug, are we installing for PHP (debug mode) or PHP (non debug mode)?
            //    depending on the mode of PHP we need to change some text below...
            if (bInstall_Xdebug) {
                vec.clear();
                vec.emplace_back("# ");
                vec.emplace_back("# wget xdebug");
                vec.emplace_back("eval \"cd " + itemValues.usrPath + "; wget " + xdebug_wget + xDebugCompressedFileName + " \"");
                vec.emplace_back(
                        "eval \"cd " + itemValues.usrPath + "; tar " + xdebug_tar_options + " " + xDebugCompressedFileName +
                        " \"");

                vec.emplace_back("# ");
                vec.emplace_back("# phpize");
                vec.emplace_back("eval \"cd " + itemValues.usrPath + xDebugProgVersion + "; ../bin/phpize > " + itemValues.bldPath + "phpize.txt \"");
                vec.emplace_back("# ");
                vec.emplace_back("# config");
                vec.emplace_back("eval \"cd " + itemValues.usrPath + xDebugProgVersion + "; ./configure --with-php-config="
                                 + itemValues.usrPath + "bin/php-config > " + itemValues.bldPath + "xdebug-configure.txt \"");
                vec.emplace_back("# ");
                vec.emplace_back("# make");
                vec.emplace_back("eval \"cd " + itemValues.usrPath + xDebugProgVersion + "; make > "
                                 + itemValues.bldPath + "xdebug-make.txt \"");
                result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

                vec.clear();
                vec.emplace_back("# ");
                vec.emplace_back("# cp modules/xdebug.so");

                // checking for the mode of PHP and adjusting accordingly
                if (bCompileForDebug) {
                    vec.emplace_back("eval \"cd " + itemValues.usrPath + xDebugProgVersion + "; cp modules/xdebug.so "
                                     + itemValues.usrPath + "lib/php/extensions/debug-zts-" + zts_version + " \"");

                } else {
                    vec.emplace_back("eval \"cd " + itemValues.usrPath + xDebugProgVersion + "; cp modules/xdebug.so "
                                     + itemValues.usrPath + "lib/php/extensions/no-debug-zts-" + zts_version + " \"");

                }
                result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

                // This small section is the same for PHP
                //   regardless of the debug / non-debug mode.
                vec.clear();
                vec.emplace_back("# ");
                vec.emplace_back("# Create: " + itemValues.etcPath + "lib");
                vec.emplace_back("eval \"mkdir -p " + itemValues.etcPath + "lib \"");
                // end of small section

                // checking for the mode of PHP and adjusting accordingly
                if (bCompileForDebug) {
                    vec.emplace_back("# ");
                    vec.emplace_back("# zend_extension = " + itemValues.usrPath + "lib/php/extensions/debug-zts-" + zts_version +
                                     "/xdebug.so");
                    vec.emplace_back("eval \"cd " + itemValues.etcPath + "lib/; echo zend_extension = "
                                     + itemValues.usrPath + "lib/php/extensions/debug-zts-" + zts_version +
                                     "/xdebug.so > php_ext.ini \"");
                } else {
                    vec.emplace_back("# ");
                    vec.emplace_back("# zend_extension = " + itemValues.usrPath + "lib/php/extensions/debug-zts-" + zts_version +
                                     "/xdebug.so");
                    vec.emplace_back("eval \"cd " + itemValues.etcPath + "lib/; echo zend_extension = "
                                     + itemValues.usrPath + "lib/php/extensions/no-debug-zts-" + zts_version +
                                     "/xdebug.so > php_ext.ini \"");
                }
            } else {
                vec.emplace_back("# Xdebug not installed.");
            }
            result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
            // end of code block

            createProtectionWhenRequired(result, itemValues);
        }
    }
    else {
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# Apache Web Server and MariaDB is required ");
        vec.emplace_back("# to be installed before PHP can be installed.");
        result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }

    if (itemValues.bInstall)
    {
        return  result;
    }
    else
    {
        // Why 5, just a random number that is greater than zero...
        // to indicate an error.
        return 5;
    }
}

int install_poco(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./configure --prefix=" + itemValues.usrPath + " ";
        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_python(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "; ./configure --prefix=" + itemValues.usrPath + " ";
        result += basicInstall(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int install_postfix(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

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

 int install_tcl(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);
        sstr installOS = get_InstallOS(itemValues.thisOSType);
        itemValues.srcPathInstallOS = joinPathParts(itemValues.srcPathPNV, installOS);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd ";
        configureStr.append(itemValues.srcPathInstallOS);
        configureStr.append("; ");
        configureStr.append(" ./configure --prefix=");
        configureStr.append(itemValues.usrPath);
        configureStr.append(" --enable-threads");
        configureStr.append(" --enable-shared ");
        configureStr.append(" --enable-symbols");
        configureStr.append(" --enable-64bit ");
        result += basicInstall_tcl(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}


int install_tk(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    sstr tclProgramName = "tcl";
    sstr tclVersion     = settings["tcl->Version"];
    sstr tclConfigurePath = itemValues.rtnPath;
    sstr tclSrcPath = "src";
    tclConfigurePath = joinPathParts(tclConfigurePath, tclSrcPath);
    tclConfigurePath = joinPathParts(tclConfigurePath, tclProgramName);
    sstr tclProgramNameVersion = tclProgramName.append(tclVersion);
    tclConfigurePath = joinPathParts(tclConfigurePath, tclProgramNameVersion);
    sstr installOS = get_InstallOS(itemValues.thisOSType);
    tclConfigurePath = joinPathParts(tclConfigurePath, installOS);

    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        result = setupInstallDirectories(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);
        itemValues.srcPathInstallOS = joinPathParts(itemValues.srcPathPNV, installOS);

        // staging
        EnsureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        set_bInstall(itemValues);

        // Note: Don't end the command with \" to close the command here.
        //   We are going to append more to the command in the function
        //     and end the command with \" there.
        sstr configureStr = "eval \"cd ";
        configureStr.append(itemValues.srcPathInstallOS);
        configureStr.append("; ");
        configureStr.append(" ./configure --prefix=");
        configureStr.append(itemValues.usrPath);
        configureStr.append(" --with-tcl=");
        configureStr.append(tclConfigurePath);
        configureStr.append(" --enable-threads");
        configureStr.append(" --enable-shared ");
        configureStr.append(" --enable-symbols");
        configureStr.append(" --enable-64bit ");
        result += basicInstall_tcl(itemValues, configureStr);
        createProtectionWhenRequired(result, itemValues);
    }
    return result;
}

int reportResults(an_itemValues& itemValues, int installResult)
{
    int result = 0;
    sstr programName = itemValues.ProperName;
    if (itemValues.step < 0)
    {
        std::cout << "Install " << programName << "-"
                        << itemValues.version
                        << " : Result = " << installResult << "." << std::endl;
    }
    else
    {
        std::cout << "Install " << programName << "-"
                        << itemValues.version
                        << " step " << std::setw(2) << std::setfill('0') << itemValues.step
                        << " : Result = " << installResult << std::endl;
    }

    time_t stop = get_Time();

    result += file_append_results(itemValues.fileName_Build,   programName, itemValues.version,
                                  itemValues.step, installResult, stop, itemValues.itemStartTime);
    result += file_append_results(itemValues.fileName_Results, programName, itemValues.version,
                                  itemValues.step, installResult, stop, itemValues.itemStartTime);
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

int process_section(std::map<sstr, sstr>& settings,
                    int (*pfunc)(std::map<sstr, sstr>& settings, an_itemValues& itemValues),
                    an_itemValues& itemValues )
{
    int result = -1;
    sstr searchStr = lowerCaseString(itemValues.programName);
    itemValues.programName = searchStr;

    if (itemValues.programName != "dependencies") {
        searchStr = itemValues.programNameVersion;
    } else {
        searchStr = "Dependencies";
    }

    bool sectionLoaded = prior_Results(itemValues.fileName_Results, searchStr);
    if (!sectionLoaded)
    {
        appendNewLogSection(itemValues.fileName_Build);
        logFinalSettings(itemValues.fileName_Build, settings, itemValues.programName);
        result = pfunc(settings, itemValues);
        reportResults( itemValues, result);
    }
    else
    {
        section_already_loaded(itemValues.programName, itemValues.version);
    }
    return result;
}

bool set_settings(std::map<sstr,sstr>& settings, an_itemValues& itemValues )
{
    itemValues.fileName_Protection = getProtectedFileName(itemValues.programName);
    itemValues.ProperName          = getProperNameFromString(itemValues.programName);

    sstr skip  = settings[itemValues.programName + "->Skip"];
    if (itemValues.programName == "perl")
    {
        skip  = settings[itemValues.programName + "5->Skip"];
    }
    bool bSkip = getBoolFromString(skip);
    if (!bSkip) {
        itemValues.bSkip = bSkip;

        sstr scriptOnly = "";
        sstr doTests    = "";
        sstr debugOnly  = "";
        sstr thisOS     = "";
        sstr version    = "";

        scriptOnly    = settings[itemValues.programName + "->Script_Only"];
        doTests       = settings[itemValues.programName + "->Do_Tests"];
        debugOnly     = settings[itemValues.programName + "->Debug_Only"];
        sstr stgPath = joinPathParts(itemValues.cpyStgPath, itemValues.programName);
        itemValues.compression = settings[itemValues.programName + "->Compression"];
        itemValues.version     = settings[itemValues.programName + "->Version"];
        itemValues.getPath     = settings[itemValues.programName + "->WGET"];


        if (itemValues.programName == "perl")
        {
            // We have to override these values because perl  and perl5 are the same thing
            //   But we don't want to confuse it with   perl6 because that is a totally different by similar language
            //   We are adding 5 or 6 to match the perl version
            skip                   = settings[itemValues.programName + "5->Skip"];
            itemValues.compression = settings[itemValues.programName + "5->Compression"];
            debugOnly              = settings[itemValues.programName + "5->Debug_Only"];
            doTests                = settings[itemValues.programName + "5->Do_Tests"];
            scriptOnly             = settings[itemValues.programName + "5->Script_Only"];
            itemValues.version     = settings[itemValues.programName + "5->Version"];
            itemValues.getPath     = settings[itemValues.programName + "5->WGET"];
        }

        itemValues.bSkip         = getBoolFromString(skip);
        itemValues.bScriptOnly   = getBoolFromString(scriptOnly);
        itemValues.bDoTests      = getBoolFromString(doTests);
        itemValues.bDebug        = getBoolFromString(debugOnly);
        itemValues.bInstall      = true;

        itemValues.programNameVersion = itemValues.programName;
        itemValues.programNameVersion.append("-");
        itemValues.programNameVersion.append(itemValues.version);

        if (itemValues.programName == "apache")
        {
            itemValues.programNameVersion = "httpd-";
            itemValues.programNameVersion.append(itemValues.version);
        }

        if (itemValues.programName == "perl6")
        {
            itemValues.programNameVersion = "rakudo-star-";
            itemValues.programNameVersion.append(itemValues.version);
        }

        if (itemValues.programName == "python")
        {
            itemValues.programNameVersion = "Python-";
            itemValues.programNameVersion.append(itemValues.version);
        }

        if ((itemValues.programName == "tcl") || (itemValues.programName == "tk"))
        {
            itemValues.programNameVersion = itemValues.programName;
            itemValues.programNameVersion.append(itemValues.version);
        }

        itemValues.stgPath             = joinPathParts(itemValues.cpyStgPath,itemValues.programName);
        itemValues.fileName_Compressed =  itemValues.programNameVersion + itemValues.compression;

        if ((itemValues.programName == "tcl") || (itemValues.programName == "tk"))
        {
            itemValues.fileName_Compressed =  itemValues.programNameVersion;
            itemValues.fileName_Compressed.append("-src");
            itemValues.fileName_Compressed.append(itemValues.compression);
        }

        itemValues.fileName_Staged     =  joinPathWithFile(itemValues.stgPath, itemValues.fileName_Compressed);

        sstr tmpPath1 = "src";
        sstr tmpPath2 = joinPathParts(itemValues.rtnPath, tmpPath1);
             tmpPath1 = itemValues.programName;
        itemValues.srcPath = joinPathParts(tmpPath2, tmpPath1);

             tmpPath1 = "xxx";
        sstr xxxPath = joinPathParts(itemValues.rtnPath, tmpPath1);
        itemValues.bldPath = get_xxx_Path(xxxPath, "bld");
        itemValues.bldPath.append(itemValues.programName);
        itemValues.bldPath.append("/");
        itemValues.etcPath = get_xxx_Path(xxxPath, "etc");
        itemValues.etcPath.append(itemValues.programName);
        itemValues.etcPath.append("/");

        itemValues.usrPath = get_xxx_Path(xxxPath, "usr");
        itemValues.usrPath.append(itemValues.programName);
        itemValues.usrPath.append("/");
    }
    return bSkip;
}

/*
 * Main Starts Here
 *
 */

int main() {

    struct programs
    {
        an_itemValues itemValues;
        int  step;
        int (*funptr)(std::map<sstr, sstr> &settings, an_itemValues& itemValues);
    } program;

    OS_type thisOSType;
    Mac_PM mpm;
    sstr prefix;
    sstr protectModeText;
    time_t programStart;
    time_t programStop;
    programStart = get_Time();
    
    // get settings from file
    std::map<sstr, sstr> settings;
    sstr fileSettings = "./Install_Settings.cfg";
    settings = getProgramSettings(fileSettings);
    std::vector<sstr> generated_settings;

    // put settings into program variables
    std::cout << "Putting settings into program variables..." << std::endl;
    sstr pricomy = "/usr/local/j5c";
    sstr company    = settings[KEY_COMPANY_NAME];

    sstr usePrefix  = settings[KEY_TOUSE_PREFIX];
    bool bUsePrefix = getBoolFromString(usePrefix);
    if (bUsePrefix) {
        prefix = settings[KEY_DEFLT_PREFIX];
        program.itemValues.company = joinPathParts(prefix, company);
    } else {
        sstr beginPath = "/";
        program.itemValues.company = joinPathParts(beginPath, company);
    }

    sstr pVersion = settings[KEY_PATH_VERSION];
    pVersion = getValid_pVersion(pVersion);

    // assign a default; and change if not correct...
    thisOSType = OS_type::RedHat;
    mpm = Mac_PM::No_Selection_Made;
    sstr theOStext = settings[KEY_AN_OS_SYSTEM];
    theOStext = lowerCaseString(theOStext);
    if (theOStext == "cent os")    { thisOSType = OS_type::CentOS;      }
    if (theOStext == "cent_os")    { thisOSType = OS_type::CentOS;      }
    if (theOStext == "centos")     { thisOSType = OS_type::CentOS;      }
    if (theOStext == "fedora")     { thisOSType = OS_type::Fedora;      }
    if (theOStext == "linux mint") { thisOSType = OS_type::Linux_Mint;  }
    if (theOStext == "linux_mint") { thisOSType = OS_type::Linux_Mint;  }
    if (theOStext == "linuxmint")  { thisOSType = OS_type::Linux_Mint;  }
    if (theOStext == "red hat")    { thisOSType = OS_type::RedHat;      }
    if (theOStext == "red_hat")    { thisOSType = OS_type::RedHat;      }
    if (theOStext == "redhat")     { thisOSType = OS_type::RedHat;      }
    if (theOStext == "OSX") {
        thisOSType = OS_type::MaxOSX;
        // TODO add a setting for Homebrew or Mac Ports
        // there currently is no setting for this yet so we will pick the default for now.
        mpm = Mac_PM::MacPorts;
    }

    program.itemValues.thisOSType = thisOSType;

    protectModeText = settings[KEY_PROTECT_MODE];
    program.itemValues.bProtectMode = getBoolFromString(protectModeText);

    sstr runDepds = settings[KEY_RUN_DEPENDCS];
    bool runDependencies = getBoolFromString(runDepds);

    bool sectionLoaded;

    sstr temp = "p" + pVersion;
    program.itemValues.rtnPath = joinPathParts(program.itemValues.company, temp);
    temp = STG_NAME;
    program.itemValues.cpyStgPath = joinPathParts(program.itemValues.company, temp);
    temp = "xxx";
    sstr xxxPath  = joinPathParts(program.itemValues.rtnPath, temp);
    sstr programName = "dependencies";
    program.itemValues.programName = "dependencies";
    program.itemValues.ProperName  = getProperNameFromString(program.itemValues.programName);
    program.itemValues.tlsPath = get_xxx_Path(xxxPath, "tls");
    program.itemValues.wwwPath = get_xxx_Path(xxxPath, "www");

    ensure_Directory_exists1(program.itemValues.rtnPath);
    ensure_Directory_exists1(program.itemValues.cpyStgPath);
    ensure_Directory_exists1(program.itemValues.tlsPath);
    ensure_Directory_exists1(program.itemValues.wwwPath);

    sstr getPath = "xxx";
    sstr buildVersion;

    int result = 0;
    bool anyInstalled = false;

    temp = "Installation_Builds_p" + pVersion + ".txt";
    program.itemValues.fileName_Build = joinPathWithFile(program.itemValues.rtnPath, temp);

    temp = "Installation_Notes_p" + pVersion + ".txt";
    program.itemValues.fileName_Notes = joinPathWithFile(program.itemValues.rtnPath, temp);

    temp = "Installation_Results_p" + pVersion + ".txt";
    program.itemValues.fileName_Results = joinPathWithFile(program.itemValues.rtnPath, temp);

    std::vector<sstr> vec;
    program.itemValues.step             = -1;
    program.itemValues.itemStartTime    = get_Time();


    create_file(program.itemValues.fileName_Build);
    do_command(program.itemValues.fileName_Build, vec, false);

    if (!(isFileSizeGtZero(program.itemValues.fileName_Notes))) {
        create_file(program.itemValues.fileName_Notes);
    } else {
        ensure_file(program.itemValues.fileName_Notes);
    }

    if (!(isFileSizeGtZero(program.itemValues.fileName_Results))) {
        create_file(program.itemValues.fileName_Results);
    } else {
        ensure_file(program.itemValues.fileName_Results);
    }

    sectionLoaded = prior_Results(program.itemValues.fileName_Results, programName);
    if (!sectionLoaded) {
        bool bScriptOnly = !runDependencies;
        if ((thisOSType == OS_type::RedHat) || (thisOSType == OS_type::CentOS) || (thisOSType == OS_type::Fedora)) {
            install_yum_required_dependencies(program.itemValues.fileName_Build, program.itemValues.programName, bScriptOnly);
            print_blank_lines(2);
        }

        if (thisOSType == OS_type::Linux_Mint) {
            install_apt_required_dependencies(program.itemValues.fileName_Build, programName, bScriptOnly);
        }

        if (thisOSType == OS_type::MaxOSX) {
            install_mac_required_dependencies(mpm);
        }
        reportResults(program.itemValues, result);
    } else {
        sstr version = "";
        section_already_loaded(programName, version);
    }

    std::vector<programs> progVector;

    program.itemValues.programName = "cmake";
    program.itemValues.step        = -1;
    program.funptr = &install_cmake;
    progVector.emplace_back(program);

    program.itemValues.programName = "libzip";
    program.itemValues.step        = -1;
    program.funptr = &install_libzip;
    progVector.emplace_back(program);

    program.itemValues.programName = "perl";
    program.itemValues.step        = -1;
    program.funptr = &install_perl5;
    progVector.emplace_back(program);

    program.itemValues.programName = "openssl";
    program.itemValues.step        = -1;
    program.funptr = &install_openssl;
    progVector.emplace_back(program);

    program.itemValues.programName = "mariadb";
    program.itemValues.step        = -1;
    program.funptr = &install_mariadb;
    progVector.emplace_back(program);

    program.itemValues.programName = "apr";
    program.itemValues.step        = 1;
    program.funptr = &install_apache_step_01;
    progVector.emplace_back(program);

    program.itemValues.programName = "apr-util";
    program.itemValues.step        = 2;
    program.funptr = &install_apache_step_02;
    progVector.emplace_back(program);

    program.itemValues.programName = "apr-iconv";
    program.itemValues.step        = 3;
    program.funptr = &install_apache_step_03;
    progVector.emplace_back(program);

    program.itemValues.programName = "pcre";
    program.itemValues.step        = 4;
    program.funptr = &install_apache_step_04;
    progVector.emplace_back(program);

    program.itemValues.programName = "pcre2";
    program.itemValues.step        = 5;
    program.funptr = &install_apache_step_05;
    progVector.emplace_back(program);

    program.itemValues.programName = "apache";
    program.itemValues.step        = -1;
    program.funptr = &install_apache;
    progVector.emplace_back(program);

    program.itemValues.programName = "perl6";
    program.itemValues.step        = -1;
    program.funptr = &install_perl6;
    progVector.emplace_back(program);

    program.itemValues.programName = "php";
    program.itemValues.step        = -1;
    program.funptr = &install_php;
    progVector.emplace_back(program);

    program.itemValues.programName = "poco";
    program.itemValues.step        = -1;
    program.funptr = &install_poco;
    progVector.emplace_back(program);

    program.itemValues.programName = "postfix";
    program.itemValues.step        = -1;
    program.funptr = &install_postfix;
    progVector.emplace_back(program);

    program.itemValues.programName = "python";
    program.itemValues.step        = -1;
    program.funptr = &install_python;
    progVector.emplace_back(program);

    program.itemValues.programName = "ruby";
    program.itemValues.step        = -1;
    program.funptr = &install_ruby;
    progVector.emplace_back(program);

    program.itemValues.programName = "tcl";
    program.itemValues.step        =  1;
    program.funptr = &install_tcl;
    progVector.emplace_back(program);

    program.itemValues.programName = "tk";
    program.itemValues.step        =  2;
    program.funptr = &install_tk;
    progVector.emplace_back(program);

    program.itemValues.thisOSType   = thisOSType;

    //function pointer declaration
    int (*funptr)(std::map<sstr, sstr> &settings, an_itemValues& itemValues);

    for( auto& it: progVector )
    {
        result = -1;
        it.itemValues.itemStartTime = get_Time();
        bool skip = set_settings(settings, it.itemValues);

        if (!skip) {
            funptr = (it.funptr);
            result = process_section(settings, funptr, it.itemValues);
            if (result > -1) { anyInstalled = true; }
        }

    }
    if (anyInstalled) {
        programStop = get_Time();
        sstr end = "End of Program";
        file_append_line(program.itemValues.fileName_Build, end, programStop, programStart);
        file_append_line(program.itemValues.fileName_Build, end, programStop, programStart);
    }
    return 0;
}
