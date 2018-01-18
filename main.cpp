// Copyright J5C Marketing LLC
// Jay A Carlson
// jay.a.carlson@gmail.com
// 360-649-6218

// This program can automatically install a LAMP system + other stuff
// LAMP        := Linux, Apache Web Server, MaraiDB, Php
// Other stuff := Perl, Ruby, Tcl/Tk

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <limits>
#include <ctype.h>
#include <stdio.h>
#include "source/j5c_date.h"

using sstr = std::string;
using namespace J5C_DSL_Code;

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

enum class OS_type { Selection_Not_Available = -1, No_Selection_Made = 0, Linux_Mint = 1, CentOS = 2, RedHat = 3, MaxOSX = 4};
enum class Mac_PM  { Selection_Not_Available = -1, No_Selection_Made = 0, Home_Brew = 0, MacPorts = 1 };

sstr lowerCaseString(sstr& some_value)
{
    sstr result;
    char oneChar;
    auto idx = some_value.length();
    for (idx = 0; idx < some_value.length(); ++idx)
    {
        if (std::isupper((*some_value.substr(idx,1).c_str())))
        {
            oneChar =  tolower(*some_value.substr(idx,1).c_str());
        }
        else
        {
            oneChar = (*some_value.substr(idx,1).c_str());
        }
        result.append({oneChar});
    }
    return result;
}

sstr upperCaseString(sstr& some_value)
{
    sstr result;
    char oneChar;
    auto idx = some_value.length();
    for (idx = 0; idx < some_value.length(); ++idx)
    {
        if (std::islower((*some_value.substr(idx,1).c_str())))
        {
            oneChar =  toupper(*some_value.substr(idx,1).c_str());
        }
        else
        {
            oneChar = (*some_value.substr(idx,1).c_str());
        }
        result.append({oneChar});
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
    if ((str.length() == 1) && (str.at(0) = c))
    {
        result = "";
    }
    else {
        auto first = str.find_first_not_of(c);
        result = str.substr(first, str.length());
    }
    return result;
}

sstr removeCharFromEndOfString(sstr& str, char c )
{
    sstr result;
    if ((str.length() == 1) && (str.at(0) = c))
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
    fixed1 = removeCharFromStartOfString(partA, '/');
    fixed1 = removeCharFromEndOfString(fixed1, '/');
    fixed2 = removeCharFromStartOfString(partB, '/');
    fixed2 = removeCharFromEndOfString(fixed2, '/');
    return path = "/" + fixed1 + "/" + fixed2 + "/";
}

sstr joinPathWithFile(sstr& partA, sstr& fileName)
{
    sstr fixed1, fixed2, path;
    fixed1 = removeCharFromStartOfString(partA, '/');
    fixed1 = removeCharFromEndOfString(fixed1, '/');
    fixed2 = removeCharFromStartOfString(fileName, '/');
    fixed2 = removeCharFromEndOfString(fixed2, '/');
    return path = "/" + fixed1 + "/" + fixed2;
}


bool isFileSizeGtZero(sstr &fileName)
{
    bool result = false;
    std::streampos fsize = 0;
    std::ifstream file (fileName, std::ios::in | std::ios::binary);

    fsize = file.tellg();
    file.seekg(0, std::ios::end);
    fsize = file.tellg() - fsize;
    file.close();

    if (fsize > 0) result = true;
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

bool directoryExists(sstr path)
{
    bool result = false;
    sstr command = "cd " + path;
    int commandResult = system(command.c_str());
    if (commandResult == 0) result = true;
    return result;
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

sstr get_Time_as_String()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    int hours   = timeinfo->tm_hour;
    int minutes = timeinfo->tm_min;
    int seconds = timeinfo->tm_sec;
    long gmt    = timeinfo->tm_gmtoff;
    long gmtHoursOffset = gmt / 3600;

    sstr strGmtOff  = std::to_string(gmtHoursOffset);
    sstr strHours   = get_Time_Part<int>(hours);
    sstr strMinutes = get_Time_Part<int>(minutes);
    sstr strSeconds = get_Time_Part<int>(seconds);

    sstr offset = "";
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
    sstr time = strHours + ":" + strMinutes + ":" + strSeconds + " " + strGmtOff;

    return time;
}

int startNewLogSection(std::ofstream& file)
{
    int result = 1;  // assume failure
    if ( (file.is_open()) && (file.good()) )
    {
        j5c_Date thisDate{};
        file << std::endl << std::endl;
        file << " Status started on " << thisDate.strDate() << std::endl;
        file << " 24-Hour  (UTC) : Command(s) / Comment(s)" << std::endl;
        file << " ==============================================================================================" << std::endl;
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
        startNewLogSection(file);
    }
    file.close();
    return result;
}

int create_file(sstr &fileName)
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
    sstr f1 = fileName;
    sstr c2 = c1 + f1 + "\"";

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

int write_file_entry(std::ofstream& file, sstr entry, bool includeTime = false)
{
    int result = 0;
    if (includeTime)
    {
        sstr time = get_Time_as_String();
        file << " " << time << " : " << entry << std::endl;
    }
    else
    {
        file << entry << std::endl;
    }
    return result;
}

int file_write_vector_to_file(sstr &fileName, std::vector <sstr> &vec_lines)
{
    std::ofstream file;
    int result = 0;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        for (auto it = vec_lines.cbegin(); it != vec_lines.cend(); ++it )
        {
            if (*it == "\n")
            {
                result += write_file_entry(file, "\n", true);
            }
            else
            {
                result += write_file_entry(file, *it, true);
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

int file_append_line(sstr &fileName, sstr &line)
{
    std::ofstream file;
    int result = 1;
    bool withTime = true;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        result = write_file_entry(file, line, withTime);
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

    sstr result = "";
    sstr temp1 = some_value.substr(1,some_value.length());
    temp1 = lowerCaseString(temp1);
    if (std::islower((*some_value.substr(0,1).c_str())))
    {
        result =  toupper(*some_value.substr(0,1).c_str());
    }
    else
    {
        result = (*some_value.substr(0,1).c_str());
    }
    result.append({temp1});

    return result;
}

int file_append_results(sstr& fileName, sstr& programName, sstr& version, int step, int installResult)
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
            result = write_file_entry(file, line, true);
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
    sstr lineData = "";
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

bool prior_Results(sstr& fileNameResult, sstr& searchStr, const int step)
{
    int count = 50000;
    bool result = false;
    sstr it_data = "";
    searchStr = lowerCaseString(searchStr);
    auto max = std::numeric_limits<unsigned long>::max();

    std::vector<sstr> data = readFile(fileNameResult, count);
    for (auto it = data.cbegin(); it != data.cend(); ++it )
    {
        it_data =  *it;
        it_data = lowerCaseString(it_data);

        auto found1 = it_data.find(searchStr);
        if ( found1 != max )
        {
            auto found2 = it_data.find("result =");
            sstr temp1;
            if (found2 < max)
            {
                temp1 = it_data.substr(found2+8,it_data.length());
            }
            auto end = temp1.find_first_of(":");
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
    auto maxLineCount = 500;
    int  theCount = 0;
    std::map<sstr, sstr> result;
    sstr it_data   = "";
    sstr delimiter = "";
    sstr key       = "";
    sstr value     = "";
    sstr ws        = " \t\n\r";

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
    for (auto element  =  result.cbegin();
              element !=  result.cend();
           ++ element)
    {
        std::cout << ": " << std::setw(width) << element->first << " : " << element->second << std::endl;
    }

    std::cout << std::endl;
    std::vector<sstr> data = readFile(fileSettings, maxLineCount);
    for (auto it = data.cbegin(); it != data.cend(); ++it )
    {
        it_data = *it;
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
    for (auto element  =  result.cbegin();  element !=  result.cend(); ++element)
    {
        std::cout << ": " << std::setw(width) << element->first << " : " << element->second << std::endl;
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
    sstr command = "";
    int result = 0;
    for (auto it = vec.cbegin(); it != vec.cend(); ++it )
    {
        result = 0;
        command =  *it;

        file_append_line(fileName, command);
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
    }
    return result;
}

int removeDirectory(sstr& fileName, sstr& path, bool createScriptOnly, std::vector<sstr>& vec)
{
    if (path.length() > 2)
    {
        vec.push_back("eval \"rm -rf " + path + "\"");
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
    sstr command = "";
    std::vector<sstr> vec;
    vec.push_back("# Install " + programName + ".");
    vec.push_back("yum -y install wget");
    vec.push_back("wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-" + fedora_release + ".noarch.rpm");
    vec.push_back("yum -y install epel-release-latest-" + fedora_release +".noarch.rpm");
    vec.push_back("rm -f epel-release-latest-" + fedora_release +".noarch.rpm");
    vec.push_back("yum -y install autoconf");
    vec.push_back("yum -y install automake");
    vec.push_back("yum -y install bison");
    vec.push_back("yum -y install boost-devel");
    vec.push_back("yum -y install bzip2-devel");
    vec.push_back("yum -y install cmake");
    vec.push_back("yum -y install cmake-gui");
    vec.push_back("yum -y install expat-devel");
    vec.push_back("yum -y install ftp");
    vec.push_back("yum -y install gawk");
    vec.push_back("yum -y install google-chrome-stable");
    vec.push_back("yum -y install gitk");
    vec.push_back("yum -y install gcc");
    vec.push_back("yum -y install gcc-c++");
    vec.push_back("yum -y install gcc-c++-x86_64-linux-gnu");
    vec.push_back("yum -y install gnutls-c++");
    vec.push_back("yum -y install gnutls-devel");
    vec.push_back("yum -y install jemalloc-devel");
    vec.push_back("yum -y install java-1.8.0-openjdk");
    vec.push_back("yum -y install Judy");
    vec.push_back("yum -y install libcurl-devel");
    vec.push_back("yum -y install libedit-devel");
    vec.push_back("yum -y install libicu-devel");
    vec.push_back("yum -y install libjpeg-turbo-utils");
    vec.push_back("yum -y install libjpeg-turbo-devel");
    vec.push_back("yum -y install libpng-devel");
    vec.push_back("yum -y install libstdc++");
    vec.push_back("yum -y install libstdc++-devel");
    vec.push_back("yum -y install libstdc++-docs");
    vec.push_back("yum -y install libstdc++-static");
    vec.push_back("yum -y install libwebp-devel");
    vec.push_back("yum -y install libxml2-devel");
    vec.push_back("yum -y install libxslt-devel");
    vec.push_back("yum -y install libX11-devel");
    vec.push_back("yum -y install openssl-devel");
    vec.push_back("yum -y install java-1.8.0-openjdk");
    vec.push_back("yum -y install re2c");
    vec.push_back("yum -y install ruby");
    vec.push_back("yum -y install sqlite-devel");
    vec.push_back("yum -y install sqlite-tcl");
    vec.push_back("yum -y install tcltls-devel");
    vec.push_back("yum -y install xml2");
    vec.push_back("yum -y xorg-x11-fonts*");
    vec.push_back("yum -y xorg-x11-server-Xnest");
    vec.push_back("yum -y install vsqlite++-devel");
    vec.push_back("yum -y install yum-utils");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}


int install_apt_required_dependencies(sstr& fileName, sstr& programName, bool createScriptOnly)
{
    sstr command = "";
    std::vector<sstr> vec;
    vec.push_back("# Install " + programName + ".");
    vec.push_back("apt update");
    vec.push_back("apt upgrade -y");
    vec.push_back("apt install openjdk-9-jdk -y");
    vec.push_back("apt autoremove -y");
    vec.push_back("apt install autoconf -y");
    vec.push_back("apt install bison -y");
    vec.push_back("apt install bzip2 -y");
    vec.push_back("apt install cmake -y");
    vec.push_back("apt install cmake-gui -y");
    vec.push_back("apt install ftp -y");
    vec.push_back("apt install google-chrome-stable -y");
    vec.push_back("apt install gitk -y");
    vec.push_back("apt install git-cola -y");
    vec.push_back("apt install gcc -y");
    vec.push_back("apt install gnutls-dev -y");
    vec.push_back("apt install g++ -y");
    vec.push_back("apt install libboost-dev -y");
    vec.push_back("apt install libcurl4-gnutls-dev -y");
    vec.push_back("apt install libedit-dev -y");
    vec.push_back("apt install libexpat-dev -y");
    vec.push_back("apt install libicu-dev -y");
    vec.push_back("apt install libjemalloc-dev -y");
    vec.push_back("apt install libjpeg-dev -y");
    vec.push_back("apt install libjudy-dev -y");
    vec.push_back("apt install libncurses5-dev -y");
    vec.push_back("apt install libpng-dev -y");
    vec.push_back("apt install libsqlite3-tcl -y");
    vec.push_back("apt install libstdc++-5-dev -y");
    vec.push_back("apt install libstdc++-5-doc -y");
    vec.push_back("apt install libwebp-dev -y");
    vec.push_back("apt install libxml2-dev -y");
    vec.push_back("apt install libxslt-dev -y");
    vec.push_back("apt install libx11-dev -y");
    vec.push_back("apt install openssl-dev -y");
    vec.push_back("apt install x11-common -y");
    vec.push_back("apt install x11-xserver-utils  -y");
    vec.push_back("apt install x11-utils -y");
    vec.push_back("apt install re2c -y");
    vec.push_back("apt install ruby -y");
    vec.push_back("apt install sqlite3 -y");
    vec.push_back("apt install xml2 -y");
    vec.push_back("apt install wget -y");
    vec.push_back("apt install xxdiff -y");
    vec.push_back("apt update");
    vec.push_back("apt upgrade -y");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

bool getBoolFromString(sstr& some_value)
{
    bool result = false;
    sstr temp1;
    char temp2;
    auto idx = some_value.length();
    for (idx = 0; idx < some_value.length(); ++idx)
    {
        if (std::isupper((*some_value.substr(idx,1).c_str())))
        {
            temp2 =  tolower(*some_value.substr(idx,1).c_str());
        }
        else
        {
            temp2 = (*some_value.substr(idx,1).c_str());
        }
        temp1.append({temp2});
    }
    if (temp1 == "t")    result = true;
    if (temp1 == "true") result = true;
    if (temp1 == "on")   result = true;
    return result;
}

void protectProgramIfRequired(sstr &buildFileName,
                              sstr &protectedFileName,
                              sstr &workingPath,
                              bool bProtectMode,
                              bool bScriptOnly)
{
    std::vector<sstr> vec;
    if (!(isFileSizeGtZero(protectedFileName))) {
        if (bProtectMode) {
            ensure_Directory_exists1(workingPath);
            vec.push_back("eval \"cd " + workingPath + "; echo 'protection = true' > " + protectedFileName + "\"");
            do_command(buildFileName, vec, bScriptOnly);
        }
    }
}

void howToRemoveFileProtection(sstr& buildFileName,
                           sstr& ProperName,
                           sstr& workingPath,
                           sstr& protectedFileName,
                           bool  bScriptOnly)
{
    std::vector<sstr> vec;
    if (!(isFileSizeGtZero(protectedFileName))) {
        vec.push_back("# This " + ProperName + " install is currently protected:");
        vec.push_back("# To remove this protection, run this command:");
        vec.push_back("# eval \"cd " + workingPath + "; rm -f " + protectedFileName + "\"");
        vec.push_back("# ");
        do_command(buildFileName, vec, bScriptOnly);
    }
}


void showProgramProtectedByA7(sstr& buildFileName,
                    sstr& fileName,
                    sstr& workingPath,
                    sstr& protectedFileName,
                    bool  bScriptOnly)
{
    std::vector<sstr> vec;
    sstr ProperName = getProperNameFromString(fileName);
    vec.push_back("# ");
    vec.push_back("# This " + ProperName + " installation is protected.");
    vec.push_back("# The protection mechanism is from the configuration file.");
    vec.push_back(":   Section_a7_Enable_Protect_Mode : true");
    do_command(buildFileName, vec, bScriptOnly);

}

void stageSourceCodeIfNeeded(sstr& buildFileName,
                             sstr& stagedFileName,
                             sstr& stgPath,
                             sstr& getPath,
                             sstr& compressedFileName,
                             bool bScriptOnly)
{
    std::vector<sstr> vec;
    vec.push_back("# ");
    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    do_command(buildFileName, vec, bScriptOnly);

}


int setupInstallDirectories(sstr& buildFileName, sstr& ProperName, sstr& compressedFileName,
              sstr& stgPath, sstr& srcPath, sstr& tstPath, sstr& usrPath, bool bScriptOnly)
{
    std::vector<sstr> vec;
    vec.push_back("# ");
    vec.push_back("# Remove unfinished " + ProperName + " install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install " + ProperName + ".");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int basicInstall(sstr& buildFileName, sstr& ProperName, sstr& configureStr,
                 sstr& workingPath,   sstr& tstPath,    sstr& usrPath,       sstr& rtnPath,
        bool bDebug, bool bDoTests, bool bScriptOnly)
{
    int result = 0;
    if (!bDebug) {
        std::vector<sstr> vec;
        vec.push_back(configureStr);
        vec.push_back("eval \"cd " + workingPath + "; make \"");
        if (bDoTests) {
            if (ProperName == "Php")
            {
                // do nothing..
                // until I can get "expect" to read the input and "send" n
                // I don't want the tests to hold up the script.

                vec.push_back("# The tests must be run manually.");
                vec.push_back("#   So you can answer the questions at the end of the tests.");

            }
            else
            {
                vec.push_back("# make test 2>&1> " + tstPath + "/" + ProperName + "_TestResults.txt");
                vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
                vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath + "/" + ProperName +
                              "_TestResults.txt\"");
            }

        }
        vec.push_back("eval \"cd " + workingPath + "; make install \"");
        vec.push_back("eval \"cd " + rtnPath + "\"");
        vec.push_back("# ");
        int result = do_command(buildFileName, vec, bScriptOnly);
    }
    return result;
}

bool programNotProtected(std::map<sstr, sstr> settings,
                        sstr& buildFileName, sstr& ProperName, sstr& protectedFileName,
                        sstr& workingPath,   sstr& usrPath,
                        bool bScriptOnly)
{
    std::vector<sstr> vec;
    sstr protectModeText = settings[KEY_PROTECT_MODE];
    bool bProtectMode = getBoolFromString(protectModeText);
    bool bInstall = true;

    sstr protectionFileWithPath = workingPath + "/" + protectedFileName;

    if (isFileSizeGtZero(protectionFileWithPath)) {
        bProtectMode = true;
        bInstall = false;
        howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
    }
    if ((bProtectMode) && (directoryExists(usrPath)))
    {
        bInstall = false;
        vec.push_back("# The " + ProperName + " usr directory (" + usrPath + ") exists already.");
        vec.push_back("# This will prevent the installation out of caution. ");
        vec.push_back("# Remove this directory to install this program.");
        vec.push_back("# you can use the command below to do it: (without the '#')");
        vec.push_back("# rm -rf " + usrPath);
        do_command(buildFileName, vec, bScriptOnly);
    }
    return bInstall;
}


int install_perl(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr programName       = "perl";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                        stgPath, srcPath, tstPath, usrPath, bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./Configure -Dprefix=" + usrPath + " -d -e\"";
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);


    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + "\"";
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
    }
    return result;
}

int install_tcl(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr installOS         = "";
    sstr programName       = "tcl";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    if (thisOS == "CentOS")       installOS = "unix";
    if (thisOS == "Linux Mint")   installOS = "unix";
    if (thisOS == "Red Hat")      installOS = "unix";
    if (thisOS == "Mac OSX")      installOS = "macosx";

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "tcl" + version;
    sstr compressedFileName =  fileName + "-src" + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);
    workingPath             =  joinPathParts(workingPath, installOS);


    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath
        + " --enable-threads --enable-shared --enable-symbols --enable-64bit;\"";

        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
    }
    return result;
}


int install_tk(std::map<sstr, sstr>& settings, bool bProtectMode = true)
{
    int result = -1;
    sstr installOS         = "";
    sstr programName       = "tk";
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");

    sstr ProperName = getProperNameFromString(programName);

    //unpack the map to make the code easier to read
    sstr buildFileName = settings[programName + "->Build_Name"];
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr oldPath       = settings[programName + "->OLD_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    if (thisOS == "CentOS")       installOS = "unix";
    if (thisOS == "Linux Mint")   installOS = "unix";
    if (thisOS == "Red Hat")      installOS = "unix";
    if (thisOS == "Mac OSX")      installOS = "macosx";

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "tk" + version;
    sstr compressedFileName =  fileName + "-src" + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);
    workingPath             =  joinPathParts(workingPath, installOS);


    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath
        + " --with-tcl=" + oldPath + "/tcl" + version +"/" + installOS
        + " --enable-threads --enable-shared --enable-symbols --enable-64bit\"";

        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);



    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);

    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"";
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix="
                            + usrPath + "  --with-apr="
                            + usrPath.substr(0, 17) + "\"";
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix="
                            + usrPath    + "  --with-apr="
                            + usrPath.substr(0, 17) + "\"";

        bDoTests = false; // there are no tests for this item
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + "\"";
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + "\"";
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "httpd-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix="
                            + usrPath + "  " + "--with-apr=" + usrPath.substr(0, 13) + "/apr/bin  "
                            + "--with-apr-util="  + usrPath.substr(0, 13) + "/apr-util   "
                            + "--with-apr-iconv=" + usrPath.substr(0, 13) + "/apr-iconv  "
                            + "--with-pcre="      + usrPath.substr(0, 13) + "/pcre " + "\"";

        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./BUILD/autorun.sh; "
         + " cd " + workingPath + "; " + "./configure --prefix=" + usrPath + " " + "\\\n"
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
                  + "--enable-local-infile\"";

        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr buildFileName  = settings[programName + "->Build_Name"];
    sstr getPath        = settings[programName + "->WGET"];
    sstr stgPath        = settings[programName + "->STG_Path"];
    sstr srcPath        = settings[programName + "->SRC_Path"];
    sstr usrPath        = settings[programName + "->USR_Path"];
    sstr tstPath        = settings[programName + "->TST_Path"];
    sstr rtnPath        = settings[programName + "->RTN_Path"];
    sstr version        = settings[programName + "->Version"];
    sstr compression    = settings[programName + "->Compression"];
    sstr scriptOnly     = settings[programName + "->Script_Only"];
    sstr doTests        = settings[programName + "->Do_Tests"];
    sstr debugOnly      = settings[programName + "->Debug_Only"];
    sstr thisOS         = settings[programName + "->This_OS"];
    sstr Install_Xdebug = settings[programName + "->Install_Xdebug"]= "true";

    bool bScriptOnly     = getBoolFromString(scriptOnly);
    bool bDoTests        = getBoolFromString(doTests);
    bool bDebug          = getBoolFromString(debugOnly);
    bool bInstall_Xdebug = getBoolFromString(Install_Xdebug);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    // This section is special to the PHP install.
    // Do not remove and replace with the like section of
    //   other programs. It will not work.
    if (!(isFileSizeGtZero(stagedFileName))) {
        // this will download the file with the fileName of mirror
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "/from/this/mirror\"");
        // copy to the compressedFileName
        vec.push_back("eval \"cd " + stgPath + "; cp "    + "mirror " + compressedFileName + "\"");
        // remove the mirror file
        vec.push_back("eval \"cd " + stgPath + "; rm -f " + "mirror\"");
        do_command(buildFileName, vec, bScriptOnly);
    }

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + "\"";
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);
        vec.clear();
        if (bInstall_Xdebug)
        {
            vec.push_back("eval \"cd " + usrPath + "/bin; ./pecl install xdebug\"");
        } else
        {
            vec.push_back("# Xdebug not installed.");
        }
        result += do_command(buildFileName, vec, bScriptOnly);

        if (result == 0)
        {
            vec.clear();
            vec.push_back("#");
            do_command(buildFileName, vec, bScriptOnly);
            protectProgramIfRequired( buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
    }
    return result;
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName1           =  programName + "-"  + version;
    sstr fileName2           =  fileName1 + "-all";
    sstr compressedFileName =  fileName2 + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName2);


    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + "\"";
        bDoTests = false;
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  ProperName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + "\"";
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
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
    sstr getPath       = settings[programName + "->WGET"];
    sstr stgPath       = settings[programName + "->STG_Path"];
    sstr srcPath       = settings[programName + "->SRC_Path"];
    sstr usrPath       = settings[programName + "->USR_Path"];
    sstr tstPath       = settings[programName + "->TST_Path"];
    sstr rtnPath       = settings[programName + "->RTN_Path"];
    sstr version       = settings[programName + "->Version"];
    sstr compression   = settings[programName + "->Compression"];
    sstr scriptOnly    = settings[programName + "->Script_Only"];
    sstr doTests       = settings[programName + "->Do_Tests"];
    sstr debugOnly     = settings[programName + "->Debug_Only"];
    sstr thisOS        = settings[programName + "->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  programName + "-" + version;
    sstr compressedFileName =  fileName + compression;
    sstr stagedFileName     =  joinPathWithFile(stgPath, compressedFileName);
    sstr workingPath        =  joinPathParts(srcPath, fileName);

    stageSourceCodeIfNeeded(buildFileName, stagedFileName, stgPath, getPath, compressedFileName, bScriptOnly);

    bool bInstall = programNotProtected(settings, buildFileName, ProperName, protectedFileName,
                                                  workingPath,   usrPath,    bScriptOnly);
    if (bInstall)
    {
        result = setupInstallDirectories(buildFileName, ProperName, compressedFileName,
                                         stgPath, srcPath, tstPath, usrPath,
                                         bScriptOnly);

        /*
         * Currently Installation of this is not supported...
         *   Maybe later when I know more about it.
         *
        sstr configureStr = "eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + "\"";
        result += basicInstall(buildFileName, ProperName, configureStr,
                               workingPath, tstPath, usrPath, rtnPath,
                               bDebug, bDoTests, bScriptOnly);

        */
        if (result == 0)
        {
            protectProgramIfRequired(buildFileName, protectedFileName, workingPath, bProtectMode, bScriptOnly);
            howToRemoveFileProtection(buildFileName, ProperName, workingPath, protectedFileName, bScriptOnly);
        }
    }
    return result;
}

int reportResults(sstr& fileNameBuilds, sstr& fileNameResult, sstr& programName, sstr& version, int step, int installResult)
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
    result += file_append_results(fileNameBuilds, programName, version, step, installResult);
    result += file_append_results(fileNameResult, programName, version, step, installResult);
    print_blank_lines(2);
    return result;
};

int logFinalSettings(sstr& fileNameBuilds, std::map<sstr, sstr>& settings, sstr& programName )
{
    int max_set_width = 32;
    sstr pad_string;
    sstr str_buffer;
    std::vector<sstr> generated_settings;
    std::cout << std::endl << std::endl;
    generated_settings.push_back("#                 Listing Settings : Values ");
    generated_settings.push_back("#============================================================================");
    for (auto element = settings.cbegin(); element !=  settings.cend(); ++ element)
    {
        if (element->first.substr(0, programName.length()) == programName) {
            int pad_length = max_set_width - element->first.length();
            pad_string = sstr(pad_length, ' ');
            str_buffer = ": "  + pad_string + element->first + " : " + element->second;
            generated_settings.push_back(str_buffer);
        }
    }
    file_write_vector_to_file(fileNameBuilds, generated_settings);
    return 0;
}

int process_section(sstr& fileNameResult,
                    sstr& fileName_Build,
                    std::map<sstr, sstr>& settings,
                    sstr& programName,
                    sstr& version,
                    int (*pfunc)(std::map<sstr, sstr>& settings, bool protectMode),
                    int step,
                    bool protectMode)
{
    int result = -1;
    sstr searchStr;
    programName = lowerCaseString(programName);
    if (programName != "dependencies") {
        searchStr = programName + "-" + version;
    } else {
        searchStr = "dependencies";
    }
    bool sectionLoaded = prior_Results(fileNameResult, searchStr, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        logFinalSettings(fileName_Build, settings, programName);
        result = pfunc(settings, protectMode);
        reportResults(fileName_Build, fileNameResult, programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }
    return result;

}

sstr set_settings(std::map<sstr,sstr>& settings, sstr& programName, sstr& fileName_Build, sstr& company,
               sstr& basePath,  sstr& srcPath, sstr& usrPath, sstr& tstPath)
{
    sstr stg_name = STG_NAME;
    settings[programName + "->Build_Name"]  = fileName_Build;
    settings[programName + "->STG_Path"]    = joinPathParts(basePath, stg_name);
    settings[programName + "->SRC_Path"]    = joinPathParts(srcPath,  programName);
    settings[programName + "->USR_Path"]    = joinPathParts(usrPath,  programName);
    settings[programName + "->TST_Path"]    = joinPathParts(tstPath,  programName);
    settings[programName + "->RTN_Path"]    = basePath;
    sstr version = settings[programName + "->Version"];
    return version;
}


/*
 * Main Starts Here
 *
 *
 */

int main()
{
    OS_type thisOS;
    Mac_PM  mpm;
    sstr prefix = "";
    sstr protectModeText = "";
    bool bProtectMode = true;

    // get settings from file
    std::map<sstr, sstr> settings;
    sstr fileSettings   = "/usr/local/j5c/Install_Settings.cfg";
    settings            = getProgramSettings(fileSettings);
    std::vector<sstr>  generated_settings;

    // put settings into program variables
    std::cout << "Putting settings into program variables..." << std::endl;
    sstr pricomy    = "/usr/local/j5c";
    sstr company    = settings[KEY_COMPANY_NAME];
    sstr usePrefix  = settings[KEY_TOUSE_PREFIX];
    bool bUsePrefix = getBoolFromString(usePrefix);

    if (bUsePrefix)  {
        prefix = settings[KEY_DEFLT_PREFIX];
        company =  joinPathParts(prefix, company) ;    }
    else  {
        sstr beginPath = "/";
        company = joinPathParts(beginPath , company);
    }

    sstr pVersion   = settings[KEY_PATH_VERSION];

    sstr theOStext  = settings[KEY_AN_OS_SYSTEM];
    if (theOStext == "Red Hat")    thisOS = OS_type ::RedHat;
    if (theOStext == "CentOS")     thisOS = OS_type ::CentOS;
    if (theOStext == "Linux Mint") thisOS = OS_type ::Linux_Mint;
    if (theOStext == "OSX")
    {
        thisOS = OS_type ::MaxOSX;
        // TODO add a setting for Homebrew or Mac Ports
        // there currently is no setting for this yet so we will pick the default for now.
        mpm = Mac_PM::MacPorts;
    }

    protectModeText = settings[KEY_PROTECT_MODE];
    bProtectMode = getBoolFromString(protectModeText);

    sstr runDepds        = settings[KEY_RUN_DEPENDCS];
    bool runDependencies = getBoolFromString(runDepds);

    bool sectionLoaded        = false;

    sstr version = "";
    sstr compression = "";
    sstr temp = "p" + pVersion;
    sstr basePath = joinPathParts(company, temp);
    temp = STG_NAME;
    sstr stgPath = joinPathParts(pricomy, temp);
    temp = "src";
    sstr srcPath = joinPathParts(basePath, temp);
    temp = "usr";
    sstr usrPath = joinPathParts(basePath, temp);
    temp = "tst";
    sstr tstPath = joinPathParts(basePath, temp);
    sstr programName = "Dependencies";

    sstr getPath = "xxx";
    sstr buildVersion = "";
    
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
    vec.push_back("chown -R root:wheel " + company);
    vec.push_back("chmod -R 770 " + company);
    vec.push_back("# Sleep for 10 seconds.");
    vec.push_back("sleep 10");
    do_command(fileName_Build, vec, false);

    ensure_Directory_exists1(basePath);
    create_file(fileName_Build);

    if (!(isFileSizeGtZero(fileName_Notes))) {
        create_file(fileName_Notes);  }
    else  {
        ensure_file(fileName_Notes);  }

    if (!(isFileSizeGtZero(fileNameResult))) {
        create_file(fileNameResult);  }
    else  {
        ensure_file(fileNameResult);  }

    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
        reportResults(fileName_Build, fileNameResult, programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    //function pointer declaration
    int (*funptr) (std::map<sstr,sstr>& settings, bool bProtectMode);

    //perl setup
    programName = "perl";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = -1;
    funptr = &install_perl;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    //ruby setup
    programName = "ruby";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = -1;
    funptr = &install_ruby;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    //tcl setup
    programName = "tcl";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    settings[programName + "->This_OS"]     = theOStext;
    step = -1;
    funptr = &install_tcl;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    //tk setup
    programName = "tk";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    settings[programName + "->This_OS"]     = theOStext;
    settings[programName + "->OLD_Path"]    = settings["tcl->SRC_Path"];
    step = -1;
    funptr = &install_tk;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    //Get and Build Apache Dependencies
    // apr setup
    programName = "apr";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = 1;
    funptr = &install_apache_step_01;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // apr-util setup
    programName = "apr-util";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = 2;
    funptr = &install_apache_step_02;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // apr-iconv
    programName = "apr-iconv";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = 3;
    funptr = &install_apache_step_03;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // pcre setup
    programName = "pcre";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = 4;
    funptr = &install_apache_step_04;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // pcre2 setup
    programName = "pcre2";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = 5;
    funptr = &install_apache_step_05;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // apache setup
    programName = "apache";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = -1;
    funptr = &install_apache;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // mariadb setup
    programName = "mariadb";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = -1;
    funptr = &install_mariadb;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // php setup
    programName = "php";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = -1;
    funptr = &install_php;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // poco setup
    programName = "poco";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = -1;
    funptr = &install_poco;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // python setup
    programName = "python";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    step = -1;
    funptr = &install_python;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    // postfix setup
    programName = "postfix";
    version = set_settings(settings, programName, fileName_Build, company, basePath, srcPath, usrPath, tstPath);
    funptr = &install_postfix;
    result = process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step, bProtectMode);
    if (result > -1) {  anyInstalled = true;  }

    if (anyInstalled == true) {
        sstr end = "End of Program";
        file_append_line(fileName_Build, end);
        file_append_line(fileNameResult, end);
    }

    return 0;
}