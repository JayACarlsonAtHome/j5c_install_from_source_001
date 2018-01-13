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

enum class OS_type { Selection_Not_Available = -1, No_Selection_Made = 0, Linux_Mint = 1, CentOS = 2, RedHat = 3, MaxOSX = 4};
enum class Mac_PM  { Selection_Not_Available = -1, No_Selection_Made = 0, Home_Brew = 0, MacPorts = 1 };

sstr key_part_replacement(sstr& key, sstr& replacement)
{
    sstr result = key.replace(0,5, replacement);
    return result;
}

std::map<sstr, sstr> get_program_base_keys(sstr& progName)
{
    std::map<sstr, sstr> result;

    sstr key = "XXXXX->Build_Name";
    sstr newKey  = key_part_replacement(key, progName);
    sstr novalue = "";
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->WGET";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->STG_Path";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->SRC_Path";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->USR_Path";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->TST_Path";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->RTN_Path";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->Version";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->Compression";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->Script_Only";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->Do_Tests";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->Debug_Only";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    key = "XXXXX->This_OS";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    return result;
};

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

int file_append_results(sstr& fileName, sstr& programName, sstr& version, int step, int installResult)
{
    unsigned long width1 = 31;
    unsigned long width2 = 40;
    sstr fill1(width1,'.');
    sstr fill2(width2,'.');

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

        if (installResult == 0)
        {
            line += " : Good.............";
        }
        else
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

bool prior_Results(sstr& fileNameResult, sstr& programName, const int step)
{
    int count = 50000;
    bool result = false;
    sstr it_data = "";
    auto max = std::numeric_limits<unsigned long>::max();

    std::vector<sstr> data = readFile(fileNameResult, count);
    for (auto it = data.cbegin(); it != data.cend(); ++it )
    {
        it_data = *it;
        auto found1 = it_data.find(programName);
        if ( found1 != max )
        {
            auto found2 = it_data.find("esult =");
            sstr temp1;
            if (found2 < max)
            {
                temp1 = it_data.substr(found2+7,it_data.length());
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
        if (command.substr(0,1) != "#")
        {
            std::cout << "Command: " << command << std::endl;
            if (!createScriptOnly)
            {
                result += system(command.c_str());
            }
        }
        else
        {
            std::cout << command << std::endl;
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

int install_perl(std::map<sstr, sstr>& perl_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = perl_Map["perl->Build_Name"];
    sstr getPath       = perl_Map["perl->WGET"];
    sstr stgPath       = perl_Map["perl->STG_Path"];
    sstr srcPath       = perl_Map["perl->SRC_Path"];
    sstr usrPath       = perl_Map["perl->USR_Path"];
    sstr tstPath       = perl_Map["perl->TST_Path"];
    sstr rtnPath       = perl_Map["perl->RTN_Path"];
    sstr version       = perl_Map["perl->Version"];
    sstr compression   = perl_Map["perl->Compression"];
    sstr scriptOnly    = perl_Map["perl->Script_Only"];
    sstr doTests       = perl_Map["perl->Do_Tests"];
    sstr debugOnly     = perl_Map["perl->Debug_Only"];
    sstr thisOS        = perl_Map["perl->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "perl-" + version;
    sstr compressedFileName =  fileName + ".tar.gz";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Perl install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Perl.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./Configure -Dprefix=" + usrPath +" -d -e\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (bDoTests)
        {
            vec.push_back("# make test 2>&1> " + tstPath +"/Perl_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Perl_TestResults.txt\"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }

    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}


int install_ruby(std::map<sstr, sstr>& ruby_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = ruby_Map["ruby->Build_Name"];
    sstr getPath       = ruby_Map["ruby->WGET"];
    sstr stgPath       = ruby_Map["ruby->STG_Path"];
    sstr srcPath       = ruby_Map["ruby->SRC_Path"];
    sstr usrPath       = ruby_Map["ruby->USR_Path"];
    sstr tstPath       = ruby_Map["ruby->TST_Path"];
    sstr rtnPath       = ruby_Map["ruby->RTN_Path"];
    sstr version       = ruby_Map["ruby->Version"];
    sstr compression   = ruby_Map["ruby->Compression"];
    sstr scriptOnly    = ruby_Map["ruby->Script_Only"];
    sstr doTests       = ruby_Map["ruby->Do_Tests"];
    sstr debugOnly     = ruby_Map["ruby->Debug_Only"];
    sstr thisOS        = ruby_Map["ruby->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "ruby-" + version;
    sstr compressedFileName =  fileName + ".tar.gz";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Ruby install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Ruby.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath +"\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (bDoTests)
        {
            vec.push_back("# make test 2>&1> " + tstPath +"/Ruby_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Ruby_TestResults.txt\"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }

    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}


int install_tcl(std::map<sstr, sstr>& tcl_Map)
{
    sstr installOS = "";
    //unpack the map to make the code easier to read
    sstr buildFileName = tcl_Map["tcl->Build_Name"];
    sstr getPath       = tcl_Map["tcl->WGET"];
    sstr stgPath       = tcl_Map["tcl->STG_Path"];
    sstr srcPath       = tcl_Map["tcl->SRC_Path"];
    sstr usrPath       = tcl_Map["tcl->USR_Path"];
    sstr tstPath       = tcl_Map["tcl->TST_Path"];
    sstr rtnPath       = tcl_Map["tcl->RTN_Path"];
    sstr version       = tcl_Map["tcl->Version"];
    sstr compression   = tcl_Map["tcl->Compression"];
    sstr scriptOnly    = tcl_Map["tcl->Script_Only"];
    sstr doTests       = tcl_Map["tcl->Do_Tests"];
    sstr debugOnly     = tcl_Map["tcl->Debug_Only"];
    sstr thisOS        = tcl_Map["tcl->This_OS"];

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
    sstr compressedFileName =  fileName + "-src.tar.gz";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName + "/" + installOS;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Tcl install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Tcl.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath
                        + " --enable-threads --enable-shared --enable-symbols --enable-64bit;\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (bDoTests)
        {
            vec.push_back("# make test 2>&1> " + tstPath +"/Tcl_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Tcl_TestResults.txt\"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }

    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_tk(std::map<sstr, sstr>& tk_Map)
{
    sstr installOS = "";
    //unpack the map to make the code easier to read
    sstr buildFileName = tk_Map["tk->Build_Name"];
    sstr getPath       = tk_Map["tk->WGET"];
    sstr stgPath       = tk_Map["tk->STG_Path"];
    sstr srcPath       = tk_Map["tk->SRC_Path"];
    sstr usrPath       = tk_Map["tk->USR_Path"];
    sstr tstPath       = tk_Map["tk->TST_Path"];
    sstr rtnPath       = tk_Map["tk->RTN_Path"];
    sstr version       = tk_Map["tk->Version"];
    sstr compression   = tk_Map["tk->Compression"];
    sstr scriptOnly    = tk_Map["tk->Script_Only"];
    sstr doTests       = tk_Map["tk->Do_Tests"];
    sstr debugOnly     = tk_Map["tk->Debug_Only"];
    sstr thisOS        = tk_Map["tk->This_OS"];
    sstr oldPath       = tk_Map["tk->OLD_Path"];

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
    sstr compressedFileName =  fileName + "-src.tar.gz";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName + "/" + installOS;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Tk install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Tk.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath
                      + " --with-tcl=" + oldPath + "/tcl" + version +"/" + installOS
                      + " --enable-threads --enable-shared --enable-symbols --enable-64bit\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (bDoTests)
        {
            vec.push_back("# make test 2>&1> " + tstPath +"/Tk_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Tk_TestResults.txt\"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }

    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_apache_step_01(std::map<sstr, sstr>& apr_Map)
{

    //unpack the map to make the code easier to read
    sstr buildFileName = apr_Map["apr->Build_Name"];
    sstr getPath       = apr_Map["apr->WGET"];
    sstr stgPath       = apr_Map["apr->STG_Path"];
    sstr srcPath       = apr_Map["apr->SRC_Path"];
    sstr usrPath       = apr_Map["apr->USR_Path"];
    sstr tstPath       = apr_Map["apr->TST_Path"];
    sstr rtnPath       = apr_Map["apr->RTN_Path"];
    sstr version       = apr_Map["apr->Version"];
    sstr compression   = apr_Map["apr->Compression"];
    sstr scriptOnly    = apr_Map["apr->Script_Only"];
    sstr doTests       = apr_Map["apr->Do_Tests"];
    sstr debugOnly     = apr_Map["apr->Debug_Only"];
    sstr thisOS        = apr_Map["apr->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "apr-" + version;
    sstr compressedFileName =  fileName + ".tar.bz2";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Apr install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 01: Apr.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath +"\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (bDoTests)
        {
            vec.push_back("# make test 2>&1> " + tstPath +"/Apr_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Apr_TestResults.txt\"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }

    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_apache_step_02(std::map<sstr, sstr>& apr_util_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = apr_util_Map["apr-util->Build_Name"];
    sstr getPath       = apr_util_Map["apr-util->WGET"];
    sstr stgPath       = apr_util_Map["apr-util->STG_Path"];
    sstr srcPath       = apr_util_Map["apr-util->SRC_Path"];
    sstr usrPath       = apr_util_Map["apr-util->USR_Path"];
    sstr tstPath       = apr_util_Map["apr-util->TST_Path"];
    sstr rtnPath       = apr_util_Map["apr-util->RTN_Path"];
    sstr version       = apr_util_Map["apr-util->Version"];
    sstr compression   = apr_util_Map["apr-util->Compression"];
    sstr scriptOnly    = apr_util_Map["apr-util->Script_Only"];
    sstr doTests       = apr_util_Map["apr-util->Do_Tests"];
    sstr debugOnly     = apr_util_Map["apr-util->Debug_Only"];
    sstr thisOS        = apr_util_Map["apr-util->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "apr-util-" + version;
    sstr compressedFileName =  fileName + ".tar.bz2";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Apr-Util install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 02: Apr-Util.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "  --with-apr=" +
                      usrPath.substr(0, 17) + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (bDoTests) {
            vec.push_back("# make test 2>&1> " + tstPath +"/Apr_Util_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Apr-Util_TestResults.txt\"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd "         + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_apache_step_03(std::map<sstr, sstr>& apr_iconv_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = apr_iconv_Map["apr-iconv->Build_Name"];
    sstr getPath       = apr_iconv_Map["apr-iconv->WGET"];
    sstr stgPath       = apr_iconv_Map["apr-iconv->STG_Path"];
    sstr srcPath       = apr_iconv_Map["apr-iconv->SRC_Path"];
    sstr usrPath       = apr_iconv_Map["apr-iconv->USR_Path"];
    sstr tstPath       = apr_iconv_Map["apr-iconv->TST_Path"];
    sstr rtnPath       = apr_iconv_Map["apr-iconv->RTN_Path"];
    sstr version       = apr_iconv_Map["apr-iconv->Version"];
    sstr compression   = apr_iconv_Map["apr-iconv->Compression"];
    sstr scriptOnly    = apr_iconv_Map["apr-iconv->Script_Only"];
    sstr doTests       = apr_iconv_Map["apr-iconv->Do_Tests"];
    sstr debugOnly     = apr_iconv_Map["apr-iconv->Debug_Only"];
    sstr thisOS        = apr_iconv_Map["apr-iconv->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "apr-iconv-" + version;
    sstr compressedFileName =  fileName + ".tar.bz2";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Apr-Iconv install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 03: Apr-Iconv.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath
                      + "  --with-apr=" + usrPath.substr(0, 17) + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");

        // there are no tests at this time
        if (bDoTests && false)
        {
            vec.push_back("eval \"cd " + workingPath + "; make test \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_apache_step_04(std::map<sstr, sstr>& pcre_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = pcre_Map["pcre->Build_Name"];
    sstr getPath       = pcre_Map["pcre->WGET"];
    sstr stgPath       = pcre_Map["pcre->STG_Path"];
    sstr srcPath       = pcre_Map["pcre->SRC_Path"];
    sstr usrPath       = pcre_Map["pcre->USR_Path"];
    sstr tstPath       = pcre_Map["pcre->TST_Path"];
    sstr rtnPath       = pcre_Map["pcre->RTN_Path"];
    sstr version       = pcre_Map["pcre->Version"];
    sstr compression   = pcre_Map["pcre->Compression"];
    sstr scriptOnly    = pcre_Map["pcre->Script_Only"];
    sstr doTests       = pcre_Map["pcre->Do_Tests"];
    sstr debugOnly     = pcre_Map["pcre->Debug_Only"];
    sstr thisOS        = pcre_Map["pcre->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "pcre-" + version;
    sstr compressedFileName =  fileName + ".tar.bz2";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished PCRE install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 04: PCRE.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (bDoTests) {
            vec.push_back("# make test 2>&1> " + tstPath +"/Pcre_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Pcre_TestResults.txt\"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_apache_step_05(std::map<sstr, sstr>& pcre2_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = pcre2_Map["pcre2->Build_Name"];
    sstr getPath       = pcre2_Map["pcre2->WGET"];
    sstr stgPath       = pcre2_Map["pcre2->STG_Path"];
    sstr srcPath       = pcre2_Map["pcre2->SRC_Path"];
    sstr usrPath       = pcre2_Map["pcre2->USR_Path"];
    sstr tstPath       = pcre2_Map["pcre2->TST_Path"];
    sstr rtnPath       = pcre2_Map["pcre2->RTN_Path"];
    sstr version       = pcre2_Map["pcre2->Version"];
    sstr compression   = pcre2_Map["pcre2->Compression"];
    sstr scriptOnly    = pcre2_Map["pcre2->Script_Only"];
    sstr doTests       = pcre2_Map["pcre2->Do_Tests"];
    sstr debugOnly     = pcre2_Map["pcre2->Debug_Only"];
    sstr thisOS        = pcre2_Map["pcre2->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "pcre2-" + version;
    sstr compressedFileName =  fileName + ".tar.bz2";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished PCRE2 install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 05: PCRE2.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");

        // There are no tests at this time
        if (bDoTests && false)
        {
            vec.push_back("# make test 2>&1> " + tstPath +"/Pcre2_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_apache(std::map<sstr, sstr>& apache_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = apache_Map["apache->Build_Name"];
    sstr getPath       = apache_Map["apache->WGET"];
    sstr stgPath       = apache_Map["apache->STG_Path"];
    sstr srcPath       = apache_Map["apache->SRC_Path"];
    sstr usrPath       = apache_Map["apache->USR_Path"];
    sstr tstPath       = apache_Map["apache->TST_Path"];
    sstr rtnPath       = apache_Map["apache->RTN_Path"];
    sstr version       = apache_Map["apache->Version"];
    sstr compression   = apache_Map["apache->Compression"];
    sstr scriptOnly    = apache_Map["apache->Script_Only"];
    sstr doTests       = apache_Map["apache->Do_Tests"];
    sstr debugOnly     = apache_Map["apache->Debug_Only"];
    sstr thisOS        = apache_Map["apache->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "httpd-" + version;
    sstr compressedFileName =  fileName + ".tar.bz2";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Apache install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Web Server.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "  "
                      + "--with-apr="        + usrPath.substr(0, 13) + "/apr/bin  "
                      + "--with-apr-util="   + usrPath.substr(0, 13) + "/apr-util   "
                      + "--with-apr-iconv="  + usrPath.substr(0, 13) + "/apr-iconv  "
                      + "--with-pcre="       + usrPath.substr(0, 13) + "/pcre " + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (bDoTests) {
            vec.push_back("# make test 2>&1> " + tstPath +"/Apache_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Apache_TestResults.txt\"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_mariadb(std::map<sstr, sstr>& mariadb_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = mariadb_Map["mariadb->Build_Name"];
    sstr getPath       = mariadb_Map["mariadb->WGET"];
    sstr stgPath       = mariadb_Map["mariadb->STG_Path"];
    sstr srcPath       = mariadb_Map["mariadb->SRC_Path"];
    sstr usrPath       = mariadb_Map["mariadb->USR_Path"];
    sstr tstPath       = mariadb_Map["mariadb->TST_Path"];
    sstr rtnPath       = mariadb_Map["mariadb->RTN_Path"];
    sstr version       = mariadb_Map["mariadb->Version"];
    sstr compression   = mariadb_Map["mariadb->Compression"];
    sstr scriptOnly    = mariadb_Map["mariadb->Script_Only"];
    sstr doTests       = mariadb_Map["mariadb->Do_Tests"];
    sstr debugOnly     = mariadb_Map["mariadb->Debug_Only"];
    sstr thisOS        = mariadb_Map["mariadb->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "mariadb-" + version;
    sstr compressedFileName =  fileName + ".tar.gz";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + fileName + "/source/" + compressedFileName + "\"");
    }

    vec.push_back("# ");
    vec.push_back("# Remove unfinished MariaDB install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install MariaDB Database Server.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug) {

        vec.push_back("eval \"cd "     + workingPath + "; ./BUILD/autorun.sh\"");
        vec.push_back("eval \"cd "     + workingPath + "; "
                      + "./configure --prefix=" + usrPath + " " + "\\\n"
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
                      + "--enable-local-infile\"");

        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        vec.push_back("eval \"cd "     + workingPath + "; make install\"");
    }
    vec.push_back("eval \"cd "     + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    if (bDoTests) {
        // These tests will fail the build if we do it before
        vec.clear();
        vec.push_back("# ");
        vec.push_back("# MariaDB Tests can take several hours...");
        vec.push_back("# Your best bet is to either hit control+c to use this shell,");
        vec.push_back("#    or just check on it tomorrow...seriously...it takes that long");
        vec.push_back("# Also the tests are expected to fail, so it may not be worth it.");
        vec.push_back("# Because the tests are expected to fail, it does not cause the installation");
        vec.push_back("#    to be marked as a failure.  The installation is good or bad separate");
        vec.push_back("#    from the tests.");
        vec.push_back("eval \"cd " + workingPath + "/mysql-test; ./mysql-test-run.pl 2>&1> " + tstPath + "/Mariadb_TestResults_2.txt\"");
        do_command(buildFileName, vec, bScriptOnly);
    }
    return result;
}


int install_php(std::map<sstr, sstr>& php_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName  = php_Map["php->Build_Name"];
    sstr getPath        = php_Map["php->WGET"];
    sstr stgPath        = php_Map["php->STG_Path"];
    sstr srcPath        = php_Map["php->SRC_Path"];
    sstr usrPath        = php_Map["php->USR_Path"];
    sstr tstPath        = php_Map["php->TST_Path"];
    sstr rtnPath        = php_Map["php->RTN_Path"];
    sstr version        = php_Map["php->Version"];
    sstr compression    = php_Map["php->Compression"];
    sstr scriptOnly     = php_Map["php->Script_Only"];
    sstr doTests        = php_Map["php->Do_Tests"];
    sstr debugOnly      = php_Map["php->Debug_Only"];
    sstr thisOS         = php_Map["php->This_OS"];
    sstr Install_Xdebug = php_Map["php->Install_Xdebug"]= "true";

    bool bScriptOnly     = getBoolFromString(scriptOnly);
    bool bDoTests        = getBoolFromString(doTests);
    bool bDebug          = getBoolFromString(debugOnly);
    bool bInstall_Xdebug = getBoolFromString(Install_Xdebug);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "php-" + version;
    sstr compressedFileName =  fileName + ".tar.bz2";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        // this will download the file with the fileName of mirror
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "/from/this/mirror\"");
        // copy to the compressedFileName
        vec.push_back("eval \"cd " + stgPath + "; cp ./"    + "mirror " + compressedFileName + "\"");
        // remove the mirror file
        vec.push_back("eval \"cd " + stgPath + "; rm -f "   + "mirror\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished PHP install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install PHP.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");

        if (bInstall_Xdebug)
        {
            vec.push_back("eval \"cd " + usrPath + "/bin; ./pecl install xdebug\"");
        } else
        {
            vec.push_back("# Xdebug not installed.");
        }
    }

    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    if (bDoTests) {
        // These tests will fail the build if we do it before
        vec.clear();
        vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/PHP_TestResults.txt\"");
        do_command(buildFileName, vec, bScriptOnly);
    }
    return result;
}

int install_poco(std::map<sstr, sstr>& poco_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = poco_Map["poco->Build_Name"];
    sstr getPath       = poco_Map["poco->WGET"];
    sstr stgPath       = poco_Map["poco->STG_Path"];
    sstr srcPath       = poco_Map["poco->SRC_Path"];
    sstr usrPath       = poco_Map["poco->USR_Path"];
    sstr tstPath       = poco_Map["poco->TST_Path"];
    sstr rtnPath       = poco_Map["poco->RTN_Path"];
    sstr version       = poco_Map["poco->Version"];
    sstr compression   = poco_Map["poco->Compression"];
    sstr scriptOnly    = poco_Map["poco->Script_Only"];
    sstr doTests       = poco_Map["poco->Do_Tests"];
    sstr debugOnly     = poco_Map["poco->Debug_Only"];
    sstr thisOS        = poco_Map["poco->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName1           =  "poco-"  + version;
    sstr fileName2           =  "poco-"  + version + "-all";
    sstr compressedFileName =  fileName2 +  compression;
    sstr stagedFileName     =  stgPath   + "/" + compressedFileName;
    sstr workingPath        =  srcPath   + "/" + fileName2;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + fileName1 + "/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished POCO install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install POCO.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "/" + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");

        // There are no tests at this time
        if (bDoTests && false)
        {
            vec.push_back("# make test 2>&1> " + tstPath +"/Poco_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Poco_TestResults.txt");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_python(std::map<sstr, sstr>& python_Map)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = python_Map["python->Build_Name"];
    sstr getPath       = python_Map["python->WGET"];
    sstr stgPath       = python_Map["python->STG_Path"];
    sstr srcPath       = python_Map["python->SRC_Path"];
    sstr usrPath       = python_Map["python->USR_Path"];
    sstr tstPath       = python_Map["python->TST_Path"];
    sstr rtnPath       = python_Map["python->RTN_Path"];
    sstr version       = python_Map["python->Version"];
    sstr compression   = python_Map["python->Compression"];
    sstr scriptOnly    = python_Map["python->Script_Only"];
    sstr doTests       = python_Map["python->Do_Tests"];
    sstr debugOnly     = python_Map["python->Debug_Only"];
    sstr thisOS        = python_Map["python->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "Python-" + version;
    sstr compressedFileName =  fileName + ".tar.xz";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Python install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Python.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (bDoTests)
        {
            vec.push_back("# make test 2>&1> " + tstPath +"/Python_TestResults.txt");
            vec.push_back("# !!! Warning this may take a while...get some coffee...or something...");
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1> " + tstPath +"/Python_TestResults.txt\"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install\"");
    }

    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

int install_postfix(std::map<sstr, sstr>& postfix_Map)
{

    //unpack the map to make the code easier to read
    sstr buildFileName = postfix_Map["postfix->Build_Name"];
    sstr getPath       = postfix_Map["postfix->WGET"];
    sstr stgPath       = postfix_Map["postfix->STG_Path"];
    sstr srcPath       = postfix_Map["postfix->SRC_Path"];
    sstr usrPath       = postfix_Map["postfix->USR_Path"];
    sstr tstPath       = postfix_Map["postfix->TST_Path"];
    sstr rtnPath       = postfix_Map["postfix->RTN_Path"];
    sstr version       = postfix_Map["postfix->Version"];
    sstr compression   = postfix_Map["postfix->Compression"];
    sstr scriptOnly    = postfix_Map["postfix->Script_Only"];
    sstr doTests       = postfix_Map["postfix->Do_Tests"];
    sstr debugOnly     = postfix_Map["postfix->Debug_Only"];
    sstr thisOS        = postfix_Map["postfix->This_OS"];

    bool bScriptOnly   = getBoolFromString(scriptOnly);
    bool bDoTests      = getBoolFromString(doTests);
    bool bDebug        = getBoolFromString(debugOnly);

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName           =  "postfix-" + version;
    sstr compressedFileName =  fileName + ".tar.gz";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget " + getPath + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Postfix install (if any).");
    removeDirectory(buildFileName, srcPath, bScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, bScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, bScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Postfix.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!bDebug)
    {
        //Currently I do not support installing postfix... but hope to soon...

        //vec.push_back("eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        //vec.push_back("eval \"cd " + workingPath + "; make \"");
        //if (doTests) {
        //    vec.push_back("eval \"cd " + workingPath + "; make test \"");
        //}
        //vec.push_back("eval \"cd " + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}

sstr setPath(sstr& company, sstr& PathOffset, sstr& programName)
{
    return company + PathOffset + "/" + programName;
};

int reportResults(sstr& fileNameBuilds, sstr& fileNameResult, sstr& programName, sstr& version, int step, int installResult)
{
    int result = 0;
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
}

int process_section(sstr& fileNameResult,
                    sstr& fileName_Build,
                    std::map<sstr, sstr>& settings,
                    sstr& programName,
                    sstr& version,
                    int (*pfunc)(std::map<sstr, sstr>& settings),
                    int step)
{
    int result = -1;
    bool sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        logFinalSettings(fileName_Build, settings, programName);
        result = pfunc(settings);
        reportResults(fileName_Build, fileNameResult, programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }
    return result;

}



int main()
{
    OS_type thisOS;
    Mac_PM  mpm;
    sstr prefix = "";

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
        company = prefix + "/" + company;    }
    else  {
        company = "/" + company;
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

    sstr runDepds        = settings[KEY_RUN_DEPENDCS];
    bool runDependencies = getBoolFromString(runDepds);

    bool sectionLoaded        = false;

    sstr version = "";
    sstr compression = "";
    sstr basePath = company + "/p" + pVersion;
    sstr prod_Stg_Offset = "/stg";
    sstr prod_Src_Offset = "/p"      + pVersion + "/src";
    sstr prod_Usr_Offset = "/p"      + pVersion + "/usr";
    sstr prod_Tst_Offset = "/p"      + pVersion + "/tst";
    sstr programName = "Dependencies";
    sstr stgPathName = "stg";
    sstr getPath = "xxx";
    sstr stgPath = "xxx";
    sstr srcPath = "xxx";
    sstr usrPath = "xxx";
    sstr tstPath = "xxx";
    sstr buildVersion = "";
    
    int step = -1;
    int result = 0;

    ensure_Directory_exists1(basePath);
    sstr fileName_Build = company + "/p" + pVersion + "/Installation_Builds_p" + pVersion + ".txt";
    sstr fileName_Notes = company + "/p" + pVersion + "/Installation_Notes__p" + pVersion + ".txt";
    sstr fileNameResult = company + "/p" + pVersion + "/Installation_Result_p" + pVersion + ".txt";

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
        if ((thisOS == OS_type::RedHat) || (thisOS == OS_type::CentOS)) {
            install_yum_required_dependencies(fileName_Build, programName, runDependencies);
            print_blank_lines(2);
        }

        if (thisOS == OS_type::Linux_Mint) {
            install_apt_required_dependencies(fileName_Build, programName, runDependencies);
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
    int (*funptr) (std::map<sstr,sstr>& settings );

    //perl setup
    appendNewLogSection(fileName_Build);
    programName = "perl";
    settings["perl->Build_Name"]  = fileName_Build;
    settings["perl->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["perl->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["perl->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["perl->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["perl->RTN_Path"]    = basePath;
    version = settings["perl->Version"];
    step = -1;

    funptr = &install_perl;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    //ruby setup
    appendNewLogSection(fileName_Build);
    programName = "ruby";
    settings["ruby->Build_Name"]  = fileName_Build;
    settings["ruby->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["ruby->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["ruby->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["ruby->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["ruby->RTN_Path"]    = basePath;
    version = settings["ruby->Version"];
    step = -1;

    funptr = &install_ruby;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);


    //tcl setup
    appendNewLogSection(fileName_Build);
    programName = "tcl";
    settings["tcl->Build_Name"]  = fileName_Build;
    settings["tcl->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["tcl->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["tcl->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["tcl->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["tcl->RTN_Path"]    = basePath;
    settings["tcl->This_OS"]     = theOStext;
    version = settings["tcl->Version"];
    step = -1;

    funptr = &install_tcl;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    //tk setup
    appendNewLogSection(fileName_Build);
    programName = "tk";
    settings["tk->Build_Name"]  = fileName_Build;
    settings["tk->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["tk->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["tk->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["tk->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["tk->OLD_Path"]    = settings["tcl->SRC_Path"];
    settings["tk->RTN_Path"]    = basePath;
    settings["tk->This_OS"]     = theOStext;
    version = settings["tk->Version"];
    step = -1;

    // function pointer initialization
    funptr = &install_tk;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    //Get and Build Apache Dependencies
    programName = "apr";
    appendNewLogSection(fileName_Build);
    programName = "apr";
    settings["apr->Build_Name"]  = fileName_Build;
    settings["apr->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["apr->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["apr->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["apr->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["apr->RTN_Path"]    = basePath;
    version = settings["apr->Version"];
    step = 1;

    funptr = &install_apache_step_01;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    appendNewLogSection(fileName_Build);
    programName = "apr-util";
    settings["apr-util->Build_Name"]  = fileName_Build;
    settings["apr-util->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["apr-util->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["apr-util->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["apr-util->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["apr-util->RTN_Path"]    = basePath;
    version = settings["apr-util->Version"];
    step = 2;

    funptr = &install_apache_step_02;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    appendNewLogSection(fileName_Build);
    programName = "apr-iconv";
    settings["apr-iconv->Build_Name"]  = fileName_Build;
    settings["apr-iconv->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["apr-iconv->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["apr-iconv->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["apr-iconv->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["apr-iconv->RTN_Path"]    = basePath;
    version = settings["apr-iconv->Version"];
    step = 3;

    funptr = &install_apache_step_03;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    appendNewLogSection(fileName_Build);
    programName = "pcre";
    settings["pcre->Build_Name"]  = fileName_Build;
    settings["pcre->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["pcre->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["pcre->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["pcre->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["pcre->RTN_Path"]    = basePath;
    version = settings["pcre->Version"];
    step = 4;

    funptr = &install_apache_step_04;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    appendNewLogSection(fileName_Build);
    programName = "pcre2";
    settings["pcre2->Build_Name"]  = fileName_Build;
    settings["pcre2->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["pcre2->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["pcre2->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["pcre2->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["pcre2->RTN_Path"]    = basePath;
    version = settings["pcre2->Version"];
    step = 5;

    funptr = &install_apache_step_05;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);


    appendNewLogSection(fileName_Build);
    programName = "apache";
    settings["apache->Build_Name"]  = fileName_Build;
    settings["apache->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["apache->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["apache->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["apache->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["apache->RTN_Path"]    = basePath;
    version = settings["apache->Version"];
    step = -1;

    funptr = &install_apache;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    appendNewLogSection(fileName_Build);
    programName = "mariadb";
    settings["mariadb->Build_Name"]  = fileName_Build;
    settings["mariadb->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["mariadb->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["mariadb->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["mariadb->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["mariadb->RTN_Path"]    = basePath;
    version = settings["mariadb->Version"];
    step = -1;

    funptr = &install_mariadb;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    appendNewLogSection(fileName_Build);
    programName = "php";
    settings["php->Build_Name"]  = fileName_Build;
    settings["php->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["php->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["php->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["php->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["php->RTN_Path"]    = basePath;
    version = settings["php->Version"];
    step = -1;

    funptr = &install_php;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    appendNewLogSection(fileName_Build);
    programName = "poco";
    settings["poco->Build_Name"]  = fileName_Build;
    settings["poco->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["poco->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["poco->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["poco->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["poco->RTN_Path"]    = basePath;
    version = settings["poco->Version"];
    step = -1;

    funptr = &install_poco;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    appendNewLogSection(fileName_Build);
    programName = "python";
    settings["python->Build_Name"]  = fileName_Build;
    settings["python->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["python->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["python->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["python->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["python->RTN_Path"]    = basePath;
    version = settings["python->Version"];
    step = -1;

    funptr = &install_python;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);


    appendNewLogSection(fileName_Build);
    programName = "postfix";
    settings["postfix->Build_Name"]  = fileName_Build;
    settings["postfix->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    settings["postfix->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    settings["postfix->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    settings["postfix->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    settings["postfix->RTN_Path"]    = basePath;
    version = settings["postfix->Version"];
    step = -1;

    funptr = &install_postfix;
    process_section(fileNameResult, fileName_Build,  settings, programName, version, funptr, step);

    sstr end = "End of Program";
    file_append_line(fileName_Build, end);
    file_append_line(fileNameResult, end);

    return 0;
}