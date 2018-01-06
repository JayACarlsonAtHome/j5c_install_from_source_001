// Copyright J5C Marketing LLC
// Jay A Carlson
// jay.a.carlson@gmail.com
// 360-649-6218

// This program can automatically install a LAMP system
//   and a backup system

#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/locid.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <limits>
#include "source/j5c_date.h"

using sstr = std::string;
using namespace J5C_DSL_Code;

const sstr ABBR_COMPANYNAME = "Section_a1_Abbr_CompanyName";
const sstr DEF__COMPANYNAME = "j5c";
const sstr CREATESCRIPTONLY = "Section_a2_CreateScriptOnly";
const sstr DEFLT_SCRIPTONLY = "false";
const sstr THE_PATH_VERSION = "Section_a3_The_Path_Version";
const sstr DEF_PATH_VERSION = "001";
const sstr OPERATIONGSYSTEM = "Section_a4_The_Operating_System";
const sstr DEF_OPER__SYSTEM = "Red Hat";

const sstr THE_PERL_VERSION = "Section_b1_The_Perl_Version";
const sstr DEFAULT_PERL_VER = "5.26.1";
const sstr THE_TCL__VERSION = "Section_b2_The_Tcl_Version";
const sstr DEFAULT_TCL_VERS = "8.6.8";
const sstr THE_TK___VERSION = "Section_b3_The_Tk_Version";
const sstr DEFAULT_TK_VERSI = "8.6.8";

const sstr THE_APR__VERSION = "Section_c1_The_Apr_Version";
const sstr DEFAULT_APR_VERS = "1.6.3";
const sstr THE_APR_UTIL_VER = "Section_c2_The_Apr-Util_Version";
const sstr DEF_APR_UTIL_VER = "1.6.1";
const sstr THE_APR_ICONVVER = "Section_c3_The_Apr-Iconv_Version";
const sstr DEF_APR_ICONVVER = "1.2.2";
const sstr THE_PCRE_VERSION = "Section_c4_The_Pcre_Version";
const sstr DEF_PCRE_VERSION = "8.41";
const sstr THE_PCRE2_VERS_N = "Section_c5_The_Pcre2_Version";
const sstr DEF_PCRE2_VERS_N = "10.30";
const sstr THE_APACHE_VERSN = "Section_c6_The_Apache_Version";
const sstr DEF_APACHE_VERSN = "2.4.29";

const sstr THE_MARIADB_VERS = "Section_d1_The_MariaDB_Version";
const sstr DEF_MARIADB_VERS = "10.3.3";
const sstr THE_PHP__VERSION = "Section_d2_The_PHP_Version";
const sstr DEF_PHP__VERSION = "7.2.0";
const sstr THE_POSTFIX_VERS = "Section_d3_The_PostFix_Version";
const sstr DEF_POSTFIX_VERS = "3.2.4";


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
        file << " ========================================================================================" << std::endl;
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

        sstr strResults = "      " + std::to_string(installResult);
        strResults = strResults.substr(strResults.length()-5,5);
        line += " : Result = " + strResults;

        if (installResult == 0)
        {
            line += "  :  ( Good... ) ";
        }
        else
        {
            line += "  :  ( What the heck?...)";
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

std::vector<sstr> readFile(sstr &fileName, int maxCount)
{
    int theCount = 0;
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
    int count = 10000;
    bool result = false;
    bool temp_result = false;
    bool cont = true;
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
            sstr temp2;
            if (found2 < max)
            {
                temp1 = it_data.substr(found2+7,it_data.length());
            }
            auto end = temp1.find_first_of(":");
            temp2 = temp1.substr(0, end-1);
            auto temp3 = std::stol(temp2, nullptr, 10);
            if (temp3 == 0)
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
    int  maxCount = 100;
    int  theCount = 0;
    std::map<sstr, sstr> result;

    //load default values into map
    result.emplace(std::pair<sstr , sstr >(ABBR_COMPANYNAME, DEF__COMPANYNAME));
    result.emplace(std::pair<sstr , sstr >(THE_PATH_VERSION, DEF_PATH_VERSION));
    result.emplace(std::pair<sstr , sstr >(OPERATIONGSYSTEM, DEF_OPER__SYSTEM));
    result.emplace(std::pair<sstr , sstr >(CREATESCRIPTONLY, DEFLT_SCRIPTONLY));

    result.emplace(std::pair<sstr , sstr >(THE_PERL_VERSION, DEFAULT_PERL_VER));
    result.emplace(std::pair<sstr , sstr >(THE_TCL__VERSION, DEFAULT_TCL_VERS));
    result.emplace(std::pair<sstr , sstr >(THE_TK___VERSION, DEFAULT_TK_VERSI));

    // Apache Items
    result.emplace(std::pair<sstr , sstr >(THE_APR__VERSION, DEFAULT_APR_VERS));
    result.emplace(std::pair<sstr , sstr >(THE_APR_UTIL_VER, DEF_APR_UTIL_VER));
    result.emplace(std::pair<sstr , sstr >(THE_APR_ICONVVER, DEF_APR_ICONVVER));
    result.emplace(std::pair<sstr , sstr >(THE_PCRE_VERSION, DEF_PCRE_VERSION));
    result.emplace(std::pair<sstr , sstr >(THE_PCRE2_VERS_N, DEF_PCRE2_VERS_N));
    result.emplace(std::pair<sstr , sstr >(THE_APACHE_VERSN, DEF_APACHE_VERSN));

    result.emplace(std::pair<sstr , sstr >(THE_MARIADB_VERS, DEF_MARIADB_VERS));
    result.emplace(std::pair<sstr , sstr >(THE_PHP__VERSION, DEF_PHP__VERSION));
    result.emplace(std::pair<sstr , sstr >(THE_POSTFIX_VERS, DEF_POSTFIX_VERS));

    sstr it_data = "";
    auto max = std::numeric_limits<unsigned long>::max();

    // file format specifications
    // # this is a comment line
    // # # is not allowed to be a delimiter

    // # [delimiter] : variable : value
    // # : variable  : value
    // # * variable  * value
    // # semicolons are optional and are ignored
    // # only one variable : value pair per line
    // # no spaces allowed in variable name or in value

    // : Company    : j5c
    // : PathVersion: 001
    // : CreateScriptOnly : false;
    // : CreateScriptOnly : true;

    int width = 32;


    std::cout << "#    Listing Settings : Values (Defaults)" << std::endl;
    std::cout << "#=========================================================" << std::endl;
    for (auto element  =  result.cbegin();
              element !=  result.cend();
           ++ element)
    {
        std::cout << ": " << std::setw(width) << element->first << " : " << element->second << std::endl;
    }



    std::vector<sstr> data = readFile(fileSettings, theCount);
    for (auto it = data.cbegin(); it != data.cend(); ++it )
    {
        it_data = *it;
    }

    std::cout << std::endl << std::endl;

    std::cout << "#    Listing Settings : Values (Loading Setting from File)" << std::endl;
    std::cout << "#=========================================================" << std::endl;
    for (auto element  =  result.cbegin();
         element !=  result.cend();
         ++ element)
    {
        std::cout << ": " << std::setw(width) << element->first << " : " << element->second << std::endl;
    }


    return result;
}



enum class OS_type { Selection_Not_Available = -1, No_Selection_Made = 0, Linux_Mint = 1, CentOS = 2, RedHat = 3, MaxOSX = 4};
enum class Mac_PM  { Selection_Not_Available = -1, No_Selection_Made = 0, Home_Brew = 0, MacPorts = 1 };

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

int do_command(sstr& fileName, std::vector<sstr>& vec, bool createScriptOnly)
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
            std::cout << "!!!Error -- Process terminated for safety...";
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

int install_perl(sstr& homePath,
                 sstr& buildFileName,
                 sstr& stgPath,
                 sstr& srcPath,
                 sstr& usrPath,
                 sstr& tstPath,
                 sstr& version,
                 bool createScriptOnly,
                 bool doTests = true,
                 bool debugOnly = false)
{
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
        vec.push_back("eval \"cd " + stgPath + "; wget http://www.cpan.org/src/5.0/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Perl install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Perl.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./Configure -Dprefix=" + usrPath +" -d -e\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (doTests)
        {
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | tee " + tstPath +"/Perl_TestResults.txt & \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }

    vec.push_back("eval \"cd "     + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}

int install_tcl(sstr& homePath,
                 sstr& buildFileName,
                 sstr& stgPath,
                 sstr& srcPath,
                 sstr& usrPath,
                 sstr& tstPath,
                 sstr& version,
                 bool createScriptOnly,
                 bool doTests = true,
                 OS_type thisOS = OS_type::Linux_Mint,
                 bool debugOnly = false)
{

    sstr command = "";
    sstr installOS = "unix";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    if (thisOS == OS_type::RedHat)      installOS = "unix";
    if (thisOS == OS_type::Linux_Mint)  installOS = "unix";
    if (thisOS == OS_type::MaxOSX)      installOS = "macosx";

    sstr fileName           =  "tcl" + version;
    sstr compressedFileName =  fileName + "-src.tar.gz";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName + "/" + installOS;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget https://prdownloads.sourceforge.net/tcl/" + compressedFileName + "\"");
    }

    vec.push_back("# ");
    vec.push_back("# Remove unfinished Tcl install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Tcl.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix="  + usrPath
                                       + " --enable-threads --enable-shared --enable-symbols --enable-64bit;\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (doTests)
        {
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | tee " + tstPath +"/Tcl_TestResults.txt & \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}



int install_tk(sstr& homePath,
                sstr& buildFileName,
                sstr& stgPath,
                sstr& srcPath,
                sstr& usrPath,
                sstr& tstPath,
                sstr& oldPath,
                sstr& version,
                bool createScriptOnly,
                bool doTests = true,
                OS_type thisOS = OS_type::Linux_Mint,
                bool debugOnly = false)
{

    sstr command = "";
    sstr installOS = "unix";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    if (thisOS == OS_type::RedHat)      installOS = "unix";
    if (thisOS == OS_type::Linux_Mint)  installOS = "unix";
    if (thisOS == OS_type::MaxOSX)      installOS = "macosx";

    sstr fileName           =  "tk" + version;
    sstr compressedFileName =  fileName + "-src.tar.gz";
    sstr stagedFileName     =  stgPath + "/" + compressedFileName;
    sstr workingPath        =  srcPath + "/" + fileName + "/" + installOS;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget https://prdownloads.sourceforge.net/tcl/" + compressedFileName + "\"");
    }

    vec.push_back("# ");
    vec.push_back("# Remove unfinished Tk install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Tk.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix="  + usrPath
                                       + " --with-tcl=" + oldPath + "/tcl" + version +"/" + installOS
                                       + " --enable-threads --enable-shared --enable-symbols --enable-64bit ;  \"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (doTests)
        {
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | tee " + tstPath +"/Tk_TestResults.txt & \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
        vec.push_back("eval \"cd " + homePath + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_01(sstr& homePath,
                 sstr& buildFileName,
                 sstr& stgPath,
                 sstr& srcPath,
                 sstr& usrPath,
                 sstr& tstPath,
                 sstr& version,
                 bool createScriptOnly,
                 bool doTests = true,
                 bool debugOnly = false)
{

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName = "apr-" + version;
    sstr compressedFileName = fileName + ".tar.bz2";
    sstr stagedFileName = stgPath + "/" + compressedFileName;
    sstr workingPath = srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        vec.push_back("eval \"cd " + stgPath + "; wget http://www.apache.org/dist/apr/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished apr install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 01 : Get apr source.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (doTests) {
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | tee " + tstPath +"/Apr_TestResults.txt & \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_02(sstr& homePath,
                           sstr& buildFileName,
                           sstr& stgPath,
                           sstr& srcPath,
                           sstr& usrPath,
                           sstr& tstPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = true,
                           bool debugOnly = false)
{

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName = "apr-util-" + version;
    sstr compressedFileName = fileName + ".tar.bz2";
    sstr stagedFileName = stgPath + "/" + compressedFileName;
    sstr workingPath = srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        vec.push_back("eval \"cd " + stgPath + "; wget http://www.apache.org/dist/apr/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished apr-util install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 02 : Get apr-util source.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "  --with-apr=" +
                      usrPath.substr(0, 17) + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (doTests) {
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | tee " + tstPath +"/Apr-Util_TestResults.txt & \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd "         + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}


int install_apache_step_03(sstr& homePath,
                           sstr& buildFileName,
                           sstr& stgPath,
                           sstr& srcPath,
                           sstr& usrPath,
                           sstr& tstPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = true,
                           bool debugOnly = false)
 {

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName = "apr-iconv-" + version;
    sstr compressedFileName = fileName + ".tar.bz2";
    sstr stagedFileName = stgPath + "/" + compressedFileName;
    sstr workingPath = srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        vec.push_back("eval \"cd " + stgPath + "; wget http://www.apache.org/dist/apr/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished apr-iconv install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 03 : Get apr-iconv source.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath
                     + "  --with-apr=" + usrPath.substr(0, 17) + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");

        // there are no tests at this time
        if (doTests && false)
        {
            vec.push_back("eval \"cd " + workingPath + "; make test \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}


int install_apache_step_04(sstr& homePath,
                           sstr& buildFileName,
                           sstr& stgPath,
                           sstr& srcPath,
                           sstr& usrPath,
                           sstr& tstPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = true,
                           bool debugOnly = false)
{

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName = "pcre-" + version;
    sstr compressedFileName = fileName + ".tar.bz2";
    sstr stagedFileName = stgPath + "/" + compressedFileName;
    sstr workingPath = srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        vec.push_back("eval \"cd " + stgPath + "; wget https://ftp.pcre.org/pub/pcre/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished pcre install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 04 : Get pcre source.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (doTests) {
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | tee " + tstPath +"/Pcre_TestResults.txt & \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_05(sstr& homePath,
                           sstr& buildFileName,
                           sstr& stgPath,
                           sstr& srcPath,
                           sstr& usrPath,
                           sstr& tstPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = true,
                           bool debugOnly = false)
{

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName = "pcre2-" + version;
    sstr compressedFileName = fileName + ".tar.bz2";
    sstr stagedFileName = stgPath + "/" + compressedFileName;
    sstr workingPath = srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        vec.push_back("eval \"cd " + stgPath + "; wget https://ftp.pcre.org/pub/pcre/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished pcre2 install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Apache Step 05 : Get pcre2 source.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");

        // There are no tests at this time
        if (doTests && false)
        {
            vec.push_back("eval \"cd " + workingPath + "; make test \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}

int install_apache(sstr& homePath,
                           sstr& buildFileName,
                           sstr& stgPath,
                           sstr& srcPath,
                           sstr& usrPath,
                           sstr& tstPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = true,
                           bool debugOnly = false)
{

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName = "httpd-" + version;
    sstr compressedFileName = fileName + ".tar.bz2";
    sstr stagedFileName = stgPath + "/" + compressedFileName;
    sstr workingPath = srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        vec.push_back("eval \"cd " + stgPath + "; wget http://www.apache.org/dist/httpd/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished apache install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install apache.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "  "
                + "--with-apr="        + usrPath.substr(0, 13) + "/apr/bin  "
                + "--with-apr-util="   + usrPath.substr(0, 13) + "/apr-util   "
                + "--with-apr-iconv="  + usrPath.substr(0, 13) + "/apr-iconv  "
                + "--with-pcre="       + usrPath.substr(0, 13) + "/pcre " + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        if (doTests) {
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | tee " + tstPath +"/Apache_TestResults.txt & \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}


int install_mariadb(sstr& homePath,
                   sstr& buildFileName,
                   sstr& stgPath,
                   sstr& srcPath,
                   sstr& usrPath,
                   sstr& tstPath,
                   sstr& version,
                   bool createScriptOnly,
                   bool doTests = true,
                   bool debugOnly = false)
{

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName = "mariadb-" + version;
    sstr compressedFileName = fileName + ".tar.gz";
    sstr stagedFileName = stgPath + "/" + compressedFileName;
    sstr workingPath = srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        vec.push_back("eval \"cd " + stgPath + "; wget https://downloads.mariadb.org/interstitial/" + fileName + "/source/" + compressedFileName + "\"");
    }

    vec.push_back("# ");
    vec.push_back("# Remove unfinished mariadb install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);

    vec.push_back("# ");
    vec.push_back("# Install mariadb.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly) {

        vec.push_back("eval \"cd "     + workingPath + "; ./BUILD/autorun.sh\"");
        vec.push_back("eval \"cd "     + workingPath + "; "
                      + "./configure --prefix=" + usrPath + " " + "\\\n"
                      + "--enable-assembler                 " + "\\\n"
                      + "--with-extra-charsets=complex      " + "\\\n"
                      + "--enable-thread-safe-client        " + "\\\n"
                      + "--with-big-tables                  " + "\\\n"
                      + "--with-plugin-maria                " + "\\\n"
                      + "--with-aria-tmp-tables             " + "\\\n"
                      + "--without-plugin-innodb_plugin     " + "\\\n"
                      + "--with-mysqld-ldflags=-static      " + "\\\n"
                      + "--with-client-ldflags=-static      " + "\\\n"
                      + "--with-readline                    " + "\\\n"
                      + "--with-ssl                         " + "\\\n"
                      + "--with-embedded-server             " + "\\\n"
                      + "--with-libevent                    " + "\\\n"
                      + "--with-mysqld-ldflags=-all-static  " + "\\\n"
                      + "--with-client-ldflags=-all-static  " + "\\\n"
                      + "--with-zlib-dir=bundled            " + "\\\n"
                      + "--enable-local-infile\"");

        vec.push_back("eval \"cd "     + workingPath + "; make \"");
    }
    vec.push_back("eval \"cd "     + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    if (doTests) {
        // These tests will fail the build if we do it before
        vec.clear();
        vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | tee " + tstPath +"/Mariadb_TestResults_1.txt\"");
        vec.push_back("eval \"cd " + workingPath + "/mysql-test; ./mysql-test-run.pl 2>&1 | tee " + tstPath + "/Mariadb_TestResults_2.txt\"");
        do_command(buildFileName, vec, createScriptOnly);
    }
    return result;
}


int install_php(sstr& homePath,
                   sstr& buildFileName,
                   sstr& stgPath,
                   sstr& srcPath,
                   sstr& usrPath,
                   sstr& tstPath,
                   sstr& version,
                   bool createScriptOnly,
                   bool doTests = true,
                   bool debugOnly = false)
{

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName = "php-" + version;
    sstr compressedFileName = fileName + ".tar.bz2";
    sstr stagedFileName = stgPath + "/" + compressedFileName;
    sstr workingPath = srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        // this will download the file with the fileName of mirror
        vec.push_back("eval \"cd " + stgPath + "; wget http://php.net/get/" + compressedFileName + "/from/this/mirror\"");
        // copy to the compressedFileName
        vec.push_back("eval \"cd " + stgPath + "; cp ./"    + "mirror " + compressedFileName + "\"");
        // remove the mirror file
        vec.push_back("eval \"cd " + stgPath + "; rm -f "   + "mirror\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished PHP install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, tstPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install PHP.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        vec.push_back("eval \"cd "     + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        vec.push_back("eval \"cd "     + workingPath + "; make \"");
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");

        if (   ((version.substr(0, 1) == "7") && (version.substr(1, 1) == "."))
            && ((version.substr(2, 1) == "0") || (version.substr(2, 1) == "1")))
        {
            vec.push_back("eval \"cd " + usrPath + "/bin; ./pecl install xdebug\"");
        } else
        {
            vec.push_back("# Xdebug not installed. Not compatible with this version of PHP");
        }
    }

    vec.push_back("eval \"cd " + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    if (doTests) {
        // These tests will fail the build if we do it before
        vec.clear();
        vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | tee " + tstPath +"/PHP_TestResults.txt & \"");
        do_command(buildFileName, vec, createScriptOnly);
    }
    return result;
}

int install_postfix(sstr& homePath,
                sstr& buildFileName,
                sstr& stgPath,
                sstr& srcPath,
                sstr& usrPath,
                sstr& tstPath,
                sstr& version,
                bool createScriptOnly,
                bool doTests = true,
                bool debugOnly = true)
 {

    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(buildFileName);

    sstr fileName = "postfix-" + version;
    sstr compressedFileName = fileName + ".tar.gz";
    sstr stagedFileName = stgPath + "/" + compressedFileName;
    sstr workingPath = srcPath + "/" + fileName;

    vec.push_back("# Ensure stg directory exists.");
    vec.push_back("eval \"mkdir -p " + stgPath + "\"");
    if (!(isFileSizeGtZero(stagedFileName))) {
        vec.push_back(
                "eval \"cd " + stgPath + "; wget https://archive.mgm51.com/mirrors/postfix-source/official/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished postfix install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install postfix.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + tstPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    if (!debugOnly)
    {
        //Currently I do not support installing postfix... but hope to soon...

        //vec.push_back("eval \"cd " + workingPath + "; ./configure --prefix=" + usrPath + "\"");
        //vec.push_back("eval \"cd " + workingPath + "; make \"");
        //if (doTests) {
        //    vec.push_back("eval \"cd " + workingPath + "; make test \"");
        //}
        //vec.push_back("eval \"cd " + workingPath + "; make install \"");
    }
    vec.push_back("eval \"cd " + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}

sstr setPath(sstr& company, sstr& PathOffset, sstr& programName)
{
    return "/" + company + PathOffset + "/" + programName;
};

int reportResults(sstr& fileNameBuilds, sstr& fileNameResult, sstr& programName, sstr& version, int step, int installResult)
{
    int result = 0;
    if (step < 0)
    {
        std::cout << "Install " << programName << " result = " << installResult << "." << std::endl;
    }
    else
    {
        std::cout << "Install " << programName << " step " << std::setw(2) << std::setfill('0') << step <<  " : Result = " << installResult << std::endl;
    }
    result += file_append_results(fileNameBuilds, programName, version, step, installResult);
    result += file_append_results(fileNameResult, programName, version, step, installResult);
    print_blank_lines(2);
    return result;
};

int main()
{
    OS_type thisOS;
    Mac_PM  mpm;

    // get settings from file
    std::map<sstr, sstr> settings;
    sstr fileSettings = "/j5c/Install_Settings.cfg";
    settings = getProgramSettings(fileSettings);

    // put settings into program variables
    sstr company    = settings[ABBR_COMPANYNAME];
    sstr pVersion   = settings[THE_PATH_VERSION];

    sstr scriptOnly = settings[CREATESCRIPTONLY];
    sstr theOStext  = settings[OPERATIONGSYSTEM];

    if (theOStext == "CentOS")     thisOS = OS_type ::CentOS;
    if (theOStext == "Linux Mint") thisOS = OS_type ::Linux_Mint;
    if (theOStext == "OSX")        thisOS = OS_type ::MaxOSX;
    if (theOStext == "Red Hat")    thisOS = OS_type ::RedHat;

    auto max = std::numeric_limits<unsigned long>::max();

    bool createScriptOnly   = false;
    if ((scriptOnly.find_first_of("T") != max ) || (scriptOnly.find_first_of("t") != max ))
    {
        createScriptOnly = true;
    }

    //basic setup
    bool sectionLoaded      = false;
    bool doTests            = false;
    bool debugOnly          = false;

    sstr version = "";
    sstr basePath = "/" + company + "/p" + pVersion;
    sstr buildPathOffset = "/build_" + pVersion;
    sstr prod_Stg_Offset = "/stg";
    sstr prod_Src_Offset = "/p"      + pVersion + "/src";
    sstr prod_Usr_Offset = "/p"      + pVersion + "/usr";
    sstr prod_Tst_Offset = "/p"      + pVersion + "/tst";
    sstr programName = "Dependencies";
    sstr stgPathName = "stg";
    sstr stgPath = "xxx";
    sstr srcPath = "xxx";
    sstr usrPath = "xxx";
    sstr tstPath = "xxx";
    sstr buildVersion = "";

    
    int step = -1;
    int result = 0;

    ensure_Directory_exists1(basePath);
    sstr fileName_Build = "/" + company + "/p" + pVersion + "/Installation_Builds_p" + pVersion + ".txt";
    sstr fileNameIssues = "/" + company + "/p" + pVersion + "/Installation_Issues_p" + pVersion + ".txt";
    sstr fileName_Notes = "/" + company + "/p" + pVersion + "/Installation_Notes__p" + pVersion + ".txt";
    sstr fileNameResult = "/" + company + "/p" + pVersion + "/Installation_Result_p" + pVersion + ".txt";

    create_file(fileName_Build);
    ensure_file(fileNameIssues);
    ensure_file(fileName_Notes);
    ensure_file(fileNameResult);

    appendNewLogSection(fileNameResult);

    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        if (thisOS == OS_type::RedHat) {
            install_yum_required_dependencies(fileName_Build, programName, createScriptOnly);
            print_blank_lines(2);
        }

        if (thisOS == OS_type::Linux_Mint) {
            install_apt_required_dependencies(fileName_Build, programName, createScriptOnly);
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

    //perl setup
    programName = "perl";
    version = settings[THE_PERL_VERSION];
    if (version.length() < 1)
    {
        version = DEFAULT_PERL_VER;
    }
    step = -1;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_perl(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult, programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    //tcl setup
    programName = "tcl";
    version = settings[THE_TCL__VERSION];
    if (version.length() < 1)
    {
        version = DEFAULT_TCL_VERS;
    }
    step = -1;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_tcl(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, thisOS, debugOnly );
        reportResults(fileName_Build, fileNameResult, programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    //tk setup
    programName = "tk";
    version = settings[THE_TK___VERSION];
    if (version.length() < 1)
    {
        version = DEFAULT_TK_VERSI;
    }
    step = -1;
    sstr oldPath = srcPath;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_tk(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, oldPath, version, createScriptOnly, doTests, thisOS, debugOnly );
        reportResults(fileName_Build, fileNameResult, programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    //Get and Build Apache Dependencies
    programName = "apr";
    version = settings[THE_APR__VERSION];
    if (version.length() < 1)
    {
        version = DEFAULT_APR_VERS;
    }
    step = 1;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_apache_step_01(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "apr-util";
    version = settings[THE_APR_UTIL_VER];
    if (version.length() < 1)
    {
        version = DEF_APR_UTIL_VER;
    }
    step = 2;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_apache_step_02(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "apr-iconv";
    version = settings[THE_APR_ICONVVER];
    if (version.length() < 1)
    {
        version = DEF_APR_ICONVVER;
    }
    step = 3;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_apache_step_03(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "pcre";
    version = settings[THE_PCRE_VERSION];
    if (version.length() < 1)
    {
        version = DEF_PCRE_VERSION;
    }
    step = 4;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_apache_step_04(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "pcre2";
    version = settings[THE_PCRE2_VERS_N];
    if (version.length() < 1)
    {
        version = DEF_PCRE2_VERS_N;
    }
    step = 5;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_apache_step_05(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "apache";
    version = settings[THE_APACHE_VERSN];
    if (version.length() < 1)
    {
        version = DEF_APACHE_VERSN;
    }
    step = -1;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_apache(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "mariadb";
    version = settings[THE_MARIADB_VERS];
    if (version.length() < 1)
    {
        version = DEF_MARIADB_VERS;
    }
    step = -1;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_mariadb(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "php";
    version = settings[THE_PHP__VERSION];
    if (version.length() < 1)
    {
        version = DEF_PHP__VERSION;
    }
    step = -1;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_php(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "postfix";
    version = settings[THE_POSTFIX_VERS];
    if (version.length() < 1)
    {
        version = DEF_POSTFIX_VERS;
    }
    step = -1;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_postfix(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    sstr end = "End of Program";
    file_append_line(fileName_Build, end);
    file_append_line(fileNameResult, end);

    return 0;
}