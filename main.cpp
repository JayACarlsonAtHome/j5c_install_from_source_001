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

const sstr ABBR_COMPANYNAME = "Section_General_Abbr_CompanyName";
const sstr DEF__COMPANYNAME = "j5c";
const sstr CREATESCRIPTONLY = "Section_General_CreateScriptOnly";
const sstr DEFLT_SCRIPTONLY = "false";
const sstr ARE_WE_TORUN_TST = "Section_General_RunTests";
const sstr DEFLT_RUNTESTOFF = "false";

const sstr THE_PATH_VERSION = "Section_General_The_Path_Version";
const sstr DEF_PATH_VERSION = "001";
const sstr OPERATIONGSYSTEM = "Section_General_The_Operating_System";
const sstr DEF_OPER__SYSTEM = "Red Hat";

enum class OS_type { Selection_Not_Available = -1, No_Selection_Made = 0, Linux_Mint = 1, CentOS = 2, RedHat = 3, MaxOSX = 4};
enum class Mac_PM  { Selection_Not_Available = -1, No_Selection_Made = 0, Home_Brew = 0, MacPorts = 1 };

sstr key_part_replacement(sstr& key, sstr& replacement)
{
    sstr result = key.replace(0,5, replacement);
    std::cout << "key = *" << key << "* : replacement = *" << replacement << "* : result = *" << result << "*" << std::endl;
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

    key = "XXXXX->Step";
    newKey  = key_part_replacement(key, progName);
    result.insert(std::pair<sstr, sstr>(newKey, novalue));

    return result;
};

class installItem
{
private:
    sstr m_returnPath;
    sstr m_buildFileName;
    sstr m_prgName;
    sstr m_getPath;
    sstr m_stgPath;
    sstr m_srcPath;
    sstr m_usrPath;
    sstr m_tstPath;
    sstr m_version;
    sstr m_comprsn;
    bool m_ScriptOnly;
    bool m_doTests;
    bool m_debugOnly;
    OS_type m_thisOS;

public:
    installItem(){};
    installItem(sstr returnPath,
                sstr buildFileName,
                sstr prgName,
                sstr getPath,
                sstr stgPath,
                sstr srcPath,
                sstr usrPath,
                sstr tstPath,
                sstr version,
                sstr compression,
                bool ScriptOnly,
                bool doTests,
                bool debugOnly,
                OS_type thisOS
    )
            : m_returnPath(returnPath),
              m_buildFileName(buildFileName),
              m_prgName(prgName),
              m_getPath(getPath),
              m_stgPath(stgPath),
              m_srcPath(srcPath),
              m_usrPath(usrPath),
              m_tstPath(tstPath),
              m_version(version),
              m_comprsn(compression),
              m_ScriptOnly(ScriptOnly),
              m_doTests(doTests),
              m_debugOnly(debugOnly),
              m_thisOS(thisOS)
    {};
    void set_returnPath(sstr returnPath) { m_returnPath = returnPath; }
    void set_buildFileName(sstr buildFN) { m_buildFileName = buildFN; }
    void set_prgName(sstr name) { m_prgName = name; }
    void set_getPath(sstr path) { m_getPath = path; }
    void set_stgPath(sstr path) { m_stgPath = path; }
    void set_srcPath(sstr path) { m_srcPath = path; }
    void set_usrPath(sstr path) { m_usrPath = path; }
    void set_tstPath(sstr path) { m_tstPath = path; }
    void set_version(sstr ver ) { m_version = ver;  }
    void set_compression(sstr comp) { m_comprsn = comp; }

    void set_ScriptOnly(bool ScriptOnly ) { m_ScriptOnly = ScriptOnly; }
    void set_doTests(   bool doTests  )   { m_doTests    = doTests;   }
    void set_debugOnly( bool debugOnly)   { m_debugOnly  = debugOnly; }
    void set_ThisOS(OS_type thisOS)       { m_thisOS     = thisOS;    }

    sstr get_returnPath()   { return m_returnPath; };
    sstr get_buildFileName(){ return m_buildFileName;  };
    sstr get_prgName()      { return m_prgName;    }
    sstr get_getPath()      { return m_getPath;    }
    sstr get_stgPath()      { return m_stgPath;    }
    sstr get_srcPath()      { return m_srcPath;    }
    sstr get_usrPath()      { return m_usrPath;    }
    sstr get_tstPath()      { return m_tstPath;    }
    sstr get_version()      { return m_version;    }
    sstr get_compression()  { return m_comprsn;    }
    bool get_ScriptOnly()   { return m_ScriptOnly; }
    bool get_doTests()      { return m_doTests;    }
    bool get_debugOnly()    { return m_debugOnly;  }
    OS_type get_ThisOS()    { return m_thisOS;     }
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
    auto max = std::numeric_limits<unsigned long>::max();
    auto maxLineCount = max;
    int  theCount = 0;
    std::map<sstr, sstr> result;
    sstr it_data   = "";
    sstr delimiter = "";
    sstr key       = "";
    sstr value     = "";
    sstr ws        = " \t\n\r";

    //load general default values into map

    result.emplace(std::pair<sstr , sstr >(ABBR_COMPANYNAME, DEF__COMPANYNAME));
    result.emplace(std::pair<sstr , sstr >(THE_PATH_VERSION, DEF_PATH_VERSION));
    result.emplace(std::pair<sstr , sstr >(OPERATIONGSYSTEM, DEF_OPER__SYSTEM));
    result.emplace(std::pair<sstr , sstr >(CREATESCRIPTONLY, DEFLT_SCRIPTONLY));
    result.emplace(std::pair<sstr , sstr >(ARE_WE_TORUN_TST, DEFLT_RUNTESTOFF));

    //load perl default values into map
    sstr K_PROGRM_NAME = "Perl";
    sstr V_PROGRM_NAME = "perl";
    sstr K_WGET_PATH__ = "Perl WGET";
    sstr V_WGET_PATH__ = "http://www.cpan.org/src/5.0/";
    sstr K_PROGRAM_VER = "Perl Version";
    sstr V_PROGRAM_VER = "5.26.1";
    sstr K_COMPRESSION = "Perl Compression";
    sstr V_COMPRESSION = ".tar.gz";

    result.emplace(std::pair<sstr , sstr >(K_PROGRM_NAME, V_PROGRM_NAME));
    result.emplace(std::pair<sstr , sstr >(K_WGET_PATH__, V_WGET_PATH__));
    result.emplace(std::pair<sstr , sstr >(K_PROGRAM_VER, V_PROGRAM_VER));
    result.emplace(std::pair<sstr , sstr >(K_COMPRESSION, V_COMPRESSION));

    //load ruby default values into map
    K_PROGRM_NAME = "Ruby";
    V_PROGRM_NAME = "ruby";
    K_WGET_PATH__ = "Ruby WGET";
    V_WGET_PATH__ = "https://cache.ruby-lang.org/pub/ruby/2.5/";
    K_PROGRAM_VER = "Ruby Version";
    V_PROGRAM_VER = "2.5.0";
    K_COMPRESSION = "Ruby Compression";
    V_COMPRESSION = ".tar.gz";

    result.emplace(std::pair<sstr , sstr >(K_PROGRM_NAME, V_PROGRM_NAME));
    result.emplace(std::pair<sstr , sstr >(K_WGET_PATH__, V_WGET_PATH__));
    result.emplace(std::pair<sstr , sstr >(K_PROGRAM_VER, V_PROGRAM_VER));
    result.emplace(std::pair<sstr , sstr >(K_COMPRESSION, V_COMPRESSION));

/*





    result.emplace(std::pair<sstr , sstr >(THE_PERL_VERSION, DEFAULT_PERL_VER));
    result.emplace(std::pair<sstr , sstr >(THE_RUBY_VERSION, DEFAULT_RUBY_VER));
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
*/
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

    /*

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

                    //std::cout << "it_data = ***" << it_data << "***" << std::endl;
                    //std::cout << "Key     = ***" << key     << "***" << std::endl;
                    //std::cout << "Value   = ***" << value   << "***" << std::endl;

                    if (value.length() > 0) {
                        auto pair = result.find(key);
                        if (pair != result.cend())
                        {
                            result[key] = value;
                        }
                    }
                }
            }
        }
    }
    */
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

int install_perl(std::map<sstr, sstr>& perlMap)
{

    //unpack the map to make the code easier to read
    sstr buildFileName = perlMap["perl->Build_Name"];
    sstr getPath       = perlMap["perl->WGET"];
    sstr stgPath       = perlMap["perl->STG_Path"];
    sstr srcPath       = perlMap["perl->SRC_Path"];
    sstr usrPath       = perlMap["perl->USR_Path"];
    sstr tstPath       = perlMap["perl->TST_Path"];
    sstr rtnPath       = perlMap["perl->RTN_Path"];
    sstr version       = perlMap["perl->Version"];
    sstr compression   = perlMap["perl->Compression"];
    sstr scriptOnly    = perlMap["perl->Script_Only"];
    sstr doTests       = perlMap["perl->Do_Tests"];
    sstr debugOnly     = perlMap["perl->Debug_Only"];
    sstr thisOS        = perlMap["perl->This_OS"];
    sstr step          = perlMap["perl->Step"];

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
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | " + tstPath +"/Perl_TestResults.txt & \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }

    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
    return result;
}


int install_ruby(std::map<sstr, sstr>& rubyMap)
{
    //unpack the map to make the code easier to read
    sstr buildFileName = rubyMap["ruby->Build_Name"];
    sstr getPath       = rubyMap["ruby->WGET"];
    sstr stgPath       = rubyMap["ruby->STG_Path"];
    sstr srcPath       = rubyMap["ruby->SRC_Path"];
    sstr usrPath       = rubyMap["ruby->USR_Path"];
    sstr tstPath       = rubyMap["ruby->TST_Path"];
    sstr rtnPath       = rubyMap["ruby->RTN_Path"];
    sstr version       = rubyMap["ruby->Version"];
    sstr compression   = rubyMap["ruby->Compression"];
    sstr scriptOnly    = rubyMap["ruby->Script_Only"];
    sstr doTests       = rubyMap["ruby->Do_Tests"];
    sstr debugOnly     = rubyMap["ruby->Debug_Only"];
    sstr thisOS        = rubyMap["ruby->This_OS"];
    sstr step          = rubyMap["ruby->Step"];

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
            vec.push_back("eval \"cd " + workingPath + "; make test 2>&1 | " + tstPath +"/Ruby_TestResults.txt & \"");
        }
        vec.push_back("eval \"cd "     + workingPath + "; make install \"");
    }

    vec.push_back("eval \"cd " + rtnPath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, bScriptOnly);
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
    vec.push_back("eval \"mkdir"
                          " -p " + srcPath + "\"");
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

int main()
{
    OS_type thisOS;
    Mac_PM  mpm;

    // get settings from file
    std::map<sstr, sstr> settings;
    sstr fileSettings   = "/j5c/Install_Settings.cfg";
    settings            = getProgramSettings(fileSettings);

    // put settings into program variables
    std::cout << "Puttins settings into program variables..." << std::endl;
    sstr pricomy    = "j5c";
    sstr company    = "zzz";
    sstr pVersion   = settings[THE_PATH_VERSION];

    sstr scriptOnly = settings[CREATESCRIPTONLY];
    sstr theOStext  = settings[OPERATIONGSYSTEM];

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

    auto max = std::numeric_limits<unsigned long>::max();

    bool ScriptOnly   = false;
    if ((scriptOnly.find_first_of("T") != max ) || (scriptOnly.find_first_of("t") != max ))
    {
        ScriptOnly = true;
    }


    //basic setup
    bool sectionLoaded      = false;
    bool doTests            = true;
    bool debugOnly          = false;

    sstr version = "";
    sstr compression = "";
    sstr basePath = "/" + company + "/p" + pVersion;
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
        if ((thisOS == OS_type::RedHat) || (thisOS == OS_type::CentOS)) {
            install_yum_required_dependencies(fileName_Build, programName, ScriptOnly);
            print_blank_lines(2);
        }

        if (thisOS == OS_type::Linux_Mint) {
            install_apt_required_dependencies(fileName_Build, programName, ScriptOnly);
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
    std::map<sstr, sstr> perl_map = get_program_base_keys(programName);
    // load defaults;
    // we could use a generic map here, but the simplicity ensures that
    //    we are not munging settings across installs.
    version = "5.26.1";
    perl_map["perl->Build_Name"]  = fileName_Build;
    perl_map["perl->WGET"]        = "http://www.cpan.org/src/5.0/";
    perl_map["perl->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    perl_map["perl->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    perl_map["perl->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    perl_map["perl->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    perl_map["perl->RTN_Path"]    = basePath;
    perl_map["perl->Version"]     = version;
    perl_map["perl->Compression"] = ".tar.gz";
    perl_map["perl->Script_Only"] = "false";
    perl_map["perl->Do_Tests"]    = "true";
    perl_map["perl->Debug_Only"]  = "false";
    perl_map["perl->This_OS"]     = "Red Hat";
    perl_map["perl->Step"]        = "-1";

    int width = 32;
    std::cout << std::endl << std::endl;
    std::cout << "#    Listing Settings : Values (Defaults)" << std::endl;
    std::cout << "#=========================================================" << std::endl;
    for (auto element = perl_map.cbegin(); element !=  perl_map.cend(); ++ element)
    {
        std::cout << ": " << std::setw(width) << element->first << " : " << element->second << std::endl;
    }

    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        result = install_perl(perl_map);
        reportResults(fileName_Build, fileNameResult, programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    //ruby setup
    programName = "ruby";
    std::map<sstr, sstr> ruby_map = get_program_base_keys(programName);
    // load defaults;
    // we could use a generic map here, but the simplicity ensures that
    //    we are not munging settings across installs.
    version = "2.5.0";
    ruby_map["ruby->Build_Name"]  = fileName_Build;
    ruby_map["ruby->WGET"]        = "https://cache.ruby-lang.org/pub/ruby/2.5/";
    ruby_map["ruby->STG_Path"]    = setPath(pricomy, prod_Stg_Offset, programName);
    ruby_map["ruby->SRC_Path"]    = setPath(company, prod_Src_Offset, programName);
    ruby_map["ruby->USR_Path"]    = setPath(company, prod_Usr_Offset, programName);
    ruby_map["ruby->TST_Path"]    = setPath(company, prod_Tst_Offset, programName);
    ruby_map["ruby->RTN_Path"]    = basePath;
    ruby_map["ruby->Version"]     = version;
    ruby_map["ruby->Compression"] = ".tar.gz";
    ruby_map["ruby->Script_Only"] = "false";
    ruby_map["ruby->Do_Tests"]    = "true";
    ruby_map["ruby->Debug_Only"]  = "false";
    ruby_map["ruby->This_OS"]     = "Red Hat";
    ruby_map["ruby->Step"]        = "-1";

    width = 32;
    std::cout << std::endl << std::endl;
    std::cout << "#    Listing Settings : Values (Defaults)" << std::endl;
    std::cout << "#=========================================================" << std::endl;
    for (auto element = ruby_map.cbegin(); element != ruby_map.cend(); ++element)
    {
        std::cout << ": " << std::setw(width) << element->first << " : " << element->second << std::endl;
    }

    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        result = install_ruby(ruby_map);
        reportResults(fileName_Build, fileNameResult, programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    /*
    //tcl setup
    programName = "tcl";
    version = settings[THE_TCL__VERSION];
    if (version.length() < 1)
    {
        version = DEFAULT_TCL_VERS;
    }
    step = -1;
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
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
    stgPath = setPath(pricomy, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    tstPath = setPath(company, prod_Tst_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        result = install_postfix(basePath, fileName_Build, stgPath, srcPath, usrPath, tstPath, version, createScriptOnly, doTests, debugOnly );
        reportResults(fileName_Build, fileNameResult,  programName, version, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    */
    sstr end = "End of Program";
    file_append_line(fileName_Build, end);
    file_append_line(fileNameResult, end);

    return 0;
}