// Copyright J5C Marketing LLC
// Jay A Carlson
// jay.a.carlson@gmail.com
// 360-649-6218

// This program can automatically install a LAMP system
//   and a backup system

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <limits>
#include "source/j5c_date.h"

using sstr = std::string;
using namespace J5C_DSL_Code;

bool isFileNotEmptyOrNull(sstr& fileName)
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
        result += startNewLogSection(file);
    }
    else
    {
        std::cout << "!!!Error -- unable to create file -- " << std::endl;
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

int file_append_results(sstr& fileName, sstr& programName, int step, int installResult)
{
    int width1 = 20;
    int width2 = 30;
    sstr fill1(width1,'.');
    sstr fill2(width2,'.');
    std::ofstream file;
    sstr line;
    int result = 1;
    bool found = false;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        if (step < 0)
        {
            line = "Install " + programName + fill2;
            found = true;
        }
        if ((!found) && (step < 10))
        {
            line = "Install " + programName + fill1;
            line = line.substr(0,width1) +  " step 0"  + std::to_string(step) + fill2;
            found = true;
        }
        if ((!found) && (step > 9))
        {
            line = "Install " + programName + fill1;
            line = line.substr(0,width1) +  " step "  + std::to_string(step) + fill2;
            found = true;
        }
        line = line.substr(0,width2);
        line += " : Result = " + std::to_string(installResult);

        if (installResult == 0)
        {
            line += "  :  ( Good... ) ";
        }
        else
        {
            line += "  :  ( Something didn't go right...)";
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

std::vector<sstr> file_read_file(sstr& fileName, int count)
{
    std::ifstream file;
    std::vector<sstr> result;
    file.open(fileName, std::ios::in );
    sstr lineData = "";
    if ( (file.is_open()) && (file.good()) )
    {
        while (getline(file, lineData))
        {
            result.push_back(lineData + "\n");
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
    sstr it_data = "";
    auto max = std::numeric_limits<unsigned long>::max();

    std::vector<sstr> data = file_read_file(fileNameResult, count);
    for (auto it = data.cbegin(); it != data.cend(); ++it )
    {
        it_data = *it;
        auto found1 = it_data.find(programName);
        auto found2 = found1;
        auto found3 = found1;
        if ( found1 != max )
        {
            found2 = it_data.find("Result = 0");
            found3 = it_data.find("result = 0");

            if ((found2 != max) || (found3 != max))
            {
                result = true;
                break;
            }
        }
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
                 sstr& version,
                 bool createScriptOnly,
                 bool doTests = true)
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
    if (!(isFileNotEmptyOrNull(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget http://www.cpan.org/src/5.0/" + compressedFileName + "\"");
    }
    vec.push_back("# ");
    vec.push_back("# Remove unfinished Perl install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Perl.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");
    vec.push_back("eval \"cd " + workingPath + "; ./Configure -Dprefix=" + usrPath +" -d -e\"");
    vec.push_back("eval \"cd " + workingPath + "; make \"");
    if (doTests)
    {
        vec.push_back("eval \"cd " + workingPath + "; make test \"");
    }
    vec.push_back("eval \"cd " + workingPath + "; make install \"");
    vec.push_back("eval \"cd " + homePath + "\"");
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
                 sstr& version,
                 bool createScriptOnly,
                 bool doTests = true,
                 OS_type thisOS = OS_type::Linux_Mint)
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
    if (!(isFileNotEmptyOrNull(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget https://prdownloads.sourceforge.net/tcl/" + compressedFileName + "\"");
    }

    vec.push_back("# ");
    vec.push_back("# Remove unfinished Tcl install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Tcl.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");
    vec.push_back("eval \"cd " + workingPath + "; ./configure --prefix="  + usrPath + " --enable-threads --enable-shared --enable-symbols;  \"");
    vec.push_back("eval \"cd " + workingPath + "; make \"");
    if (doTests)
    {
        vec.push_back("eval \"cd " + workingPath + "; make test \"");
    }
    vec.push_back("eval \"cd " + workingPath + "; make install \"");
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
                sstr& oldPath,
                sstr& version,
                bool createScriptOnly,
                bool doTests = true,
                OS_type thisOS = OS_type::Linux_Mint)
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
    if (!(isFileNotEmptyOrNull(stagedFileName)))
    {
        vec.push_back("eval \"cd " + stgPath + "; wget https://prdownloads.sourceforge.net/tcl/" + compressedFileName + "\"");
    }

    vec.push_back("# ");
    vec.push_back("# Remove unfinished Tk install (if any).");
    removeDirectory(buildFileName, srcPath, createScriptOnly, vec);
    removeDirectory(buildFileName, usrPath, createScriptOnly, vec);
    vec.push_back("# ");
    vec.push_back("# Install Tk.");
    vec.push_back("eval \"mkdir -p " + srcPath + "\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");

    vec.push_back("eval \"cd " + stgPath + "; cp ./"    + compressedFileName + " " + srcPath + "\"");
    vec.push_back("eval \"cd " + srcPath + "; tar xvf " + compressedFileName + "\"");
    vec.push_back("eval \"cd " + srcPath + "; rm -f "   + compressedFileName + "\"");

    vec.push_back("eval \"cd " + workingPath + "; ./configure --prefix="  + usrPath +
                      + " --with-tcl=" + oldPath + "/tcl" + version +"/" + installOS
                      + " --enable-threads --enable-shared --enable-symbols;  \"");
    vec.push_back("eval \"cd " + workingPath + "; make \"");
    if (doTests)
    {
        vec.push_back("eval \"cd " + workingPath + "; make test \"");
    }
    vec.push_back("eval \"cd " + workingPath + "; make install \"");
    vec.push_back("eval \"cd " + homePath + "\"");
    vec.push_back("# ");
    vec.push_back("# ");
    int result = do_command(buildFileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_01(sstr& fileName,
                           sstr& path,
                           sstr& usrPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = true)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;
    appendNewLogSection(fileName);

    vec.push_back("# Install Apache Step 01 : Get apr source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"mkdir -p " + path + "staged\"");
    vec.push_back("eval \"cd " + path + "staged\"");
    vec.push_back("eval \"wget http://www.apache.org/dist/apr/apr-" + version +".tar.bz2\"");
    vec.push_back("eval \"cp ./apr-" + version + ".tar.bz2 " + path + "\"");
    vec.push_back("eval \"rm -f ./apr-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "; tar xvf apr-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "/apr-" + version + "; ./configure --prefix=" + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/apr-" + version + "; make\"");
    if (doTests)
    {
        vec.push_back("eval \"cd " + path + "/apr-" + version + "; make test\"");
    }
    vec.push_back("eval \"cd " + path + "/apr-" + version + "; make install\"");
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_02(sstr& fileName,
                           sstr& path,
                           sstr& usrPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = true)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;
    appendNewLogSection(fileName);

    vec.push_back("# Install Apache Step 02: Get apr-util source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"mkdir -p " + path + "staged\"");
    vec.push_back("eval \"cd " + path + "staged\"");
    vec.push_back("eval \"wget http://www.apache.org/dist/apr/apr-util-" + version +".tar.gz\"");
    vec.push_back("eval \"cp ./apr-util-" + version + ".tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./apr-util-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "; tar xvf apr-util-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "/apr-util-" + version + "; ./configure --prefix=" + usrPath +"  --with-apr=" + usrPath.substr(0,17) + "\"");
    vec.push_back("eval \"cd " + path + "/apr-util-" + version + "; make\"");
    if (doTests)
    {
        vec.push_back("eval \"cd " + path + "/apr-util-" + version + "; make test\"");
    }
    vec.push_back("eval \"cd " + path + "/apr-util-" + version + "; make install\"");

    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_03(sstr& fileName,
                           sstr& path,
                           sstr& usrPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = false)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;
    appendNewLogSection(fileName);

    vec.push_back("# Install Apache Step 03: Get apr-iconv source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"mkdir -p " + path + "staged\"");
    vec.push_back("eval \"cd " + path + "staged\"");
    vec.push_back("eval \"wget http://www.apache.org/dist/apr/apr-iconv-" + version +".tar.bz2\"");
    vec.push_back("eval \"cp ./apr-iconv-" + version + ".tar.bz2 " + path + "\"");
    vec.push_back("eval \"rm -f ./apr-iconv-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "; tar xvf apr-iconv-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "/apr-iconv-" + version + "; ./configure --prefix=" + usrPath +"  --with-apr=" + usrPath.substr(0,17) + "\"");
    vec.push_back("eval \"cd " + path + "/apr-iconv-" + version + "; make\"");
    // no tests -- will stop program if you try
    vec.push_back("eval \"cd " + path + "/apr-iconv-" + version + "; make install\"");
    if (doTests)
    {
        vec.push_back("# There are no tests to run.");
    }
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_04(sstr& fileName,
                           sstr& path,
                           sstr& usrPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = true )
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;
    appendNewLogSection(fileName);

    vec.push_back("# Install Apache Step 04: Get pcre source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"mkdir -p " + path + "staged\"");
    vec.push_back("eval \"cd " + path + "staged\"");
    vec.push_back("eval \"wget https://ftp.pcre.org/pub/pcre/pcre-" + version +".tar.gz\"");
    vec.push_back("eval \"cp ./pcre-" + version + ".tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./pcre-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "; tar xvf pcre-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "/pcre-" + version + "; ./configure --prefix=" + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/pcre-" + version + "; make\"");
    if (doTests)
    {
        vec.push_back("eval \"cd " + path + "/pcre-" + version + "; make test\"");
    }
    vec.push_back("eval \"cd " + path + "/pcre-" + version + "; make install\"");
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_05(sstr& fileName,
                           sstr& path,
                           sstr& usrPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = false )
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;
    appendNewLogSection(fileName);

    vec.push_back("# Install Apache Step 05: Get pcre2 source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"mkdir -p " + path + "staged\"");
    vec.push_back("eval \"cd " + path + "staged\"");
    vec.push_back("eval \"wget https://ftp.pcre.org/pub/pcre/pcre2-" + version +".tar.gz\"");
    vec.push_back("eval \"cp ./pcre2-" + version + ".tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./pcre2-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "; tar xvf pcre2-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "/pcre2-" + version + "; ./configure --prefix=" + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/pcre2-" + version + "; make\"");
    if (doTests)
    {
        vec.push_back("# There are no tests to run.");
    }
    vec.push_back("eval \"cd " + path + "/pcre2-" + version + "; make install\"");
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_apache(sstr& fileName,
                           sstr& path,
                           sstr& usrPath,
                           sstr& version,
                           bool createScriptOnly,
                           bool doTests = false)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;
    appendNewLogSection(fileName);

    vec.push_back("# Install Apache: Get Apache source and install with dependencies.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"mkdir -p " + path + "staged\"");
    vec.push_back("eval \"cd " + path + "staged\"");
    vec.push_back("eval \"wget http://www.apache.org/dist/httpd/httpd-" + version +".tar.bz2\"");
    vec.push_back("eval \"cp ./httpd-" + version + ".tar.bz2 " + path + "\"");
    vec.push_back("eval \"rm -f ./httpd-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "; tar xvf httpd-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "/httpd-" + version + "; ./configure --prefix=" + usrPath + "  "
                  + "--with-apr=" + usrPath.substr(0,13) + "/apr/bin  "
                  + "--with-apr-util="  + usrPath.substr(0,13) + "/apr-util   "
                  + "--with-apr-iconv=" + usrPath.substr(0,13) + "/apr-iconv  "
                  + "--with-pcre=" + usrPath.substr(0,13) + "/pcre " + "\"");
    vec.push_back("eval \"cd " + path + + "/httpd-" + version + "; make\"");
    if (doTests)
    {
        vec.push_back("# There are no tests to run.");
    }
    vec.push_back("eval \"cd " + path + + "/httpd-" + version + "; make install\"");

    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_mariadb(sstr& fileName,
                    sstr& path,
                    sstr& usrPath,
                    sstr& version,
                    bool createScriptOnly,
                    bool doTests = true )
{
    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(fileName);

    int result = 0;
    vec.push_back("# Install Mariadb");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"mkdir -p " + path + "staged\"");
    vec.push_back("eval \"cd " + path + "staged\"");
    vec.push_back("eval \"wget https://downloads.mariadb.org/interstitial/mariadb-" + version +"/source/mariadb-" + version +".tar.gz\"");
    vec.push_back("eval \"cp ./mariadb-" + version + ".tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./mariadb-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "; tar xvf mariadb-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "/mariadb-" + version +"; ./BUILD/autorun.sh\"");
    vec.push_back("eval \"cd " + path + "/mariadb-" + version +"; "
                  + "./configure --prefix=" + usrPath + " "  + "\\\n"
                  +  "--enable-assembler                 "   + "\\\n"
                  +  "--with-extra-charsets=complex      "   + "\\\n"
                  +  "--enable-thread-safe-client        "   + "\\\n"
                  +  "--with-big-tables                  "   + "\\\n"
                  +  "--with-plugin-maria                "   + "\\\n"
                  +  "--with-aria-tmp-tables             "   + "\\\n"
                  +  "--without-plugin-innodb_plugin     "   + "\\\n"
                  +  "--with-mysqld-ldflags=-static      "   + "\\\n"
                  +  "--with-client-ldflags=-static      "   + "\\\n"
                  +  "--with-readline                    "   + "\\\n"
                  +  "--with-ssl                         "   + "\\\n"
                  +  "--with-embedded-server             "   + "\\\n"
                  +  "--with-libevent                    "   + "\\\n"
                  +  "--with-mysqld-ldflags=-all-static  "   + "\\\n"
                  +  "--with-client-ldflags=-all-static  "   + "\\\n"
                  +  "--with-zlib-dir=bundled            "   + "\\\n"
                  +  "--enable-local-infile\"");
    vec.push_back("eval \"cd " + path + "/mariadb-" + version +"; make\"");
    vec.push_back("eval \"cd " + path + "/mariadb-" + version +"; make install\"");
    result = do_command(fileName, vec, createScriptOnly);
    if( (result == 0) && (doTests))
    {
        //these tests are failing, and return a failing error code
        //so we must do these differently if we want to load the
        //whole LAMP system.

        vec.clear();
        vec.push_back("eval \"#Force run tests, on failure continue --force \"");
        vec.push_back("eval \"(cd " + usrPath + "/mysql-test; ./mysql-test-run.pl --force)\"");
        do_command(fileName, vec, createScriptOnly);
    }
    vec.push_back("eval \"cd /j5c\"");
    return result;
}

int install_php(sstr& fileName,
                sstr& path,
                sstr& usrPath,
                sstr& version,
                bool createScriptOnly,
                bool doTests = true )
{
    sstr command = "";
    std::vector<sstr> vec;
    appendNewLogSection(fileName);

    vec.push_back("# Install PHP");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"mkdir -p " + path + "staged\"");
    vec.push_back("eval \"cd " + path + "staged\"");
    vec.push_back("eval \"wget http://php.net/get/php-" + version + ".tar.bz2/from/this/mirror\"");
    vec.push_back("eval \"cp ./mirror " + path + "\"");
    vec.push_back("eval \"rm -f ./mirror\"");
    vec.push_back("eval \"cd " + path + "; mv mirror php-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "; tar xvjf php-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "/php-" + version + "; ./configure --prefix=" + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/php-" + version + "; make\"");
    if (doTests)
    {
        vec.push_back("eval \"cd " + path + "/php-" + version + "; make test\"");
    }
    vec.push_back("eval \"cd " + path + "/php-" + version + "; make install\"");
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

sstr setPath(sstr& company, sstr& PathOffset, sstr& programName)
{
    return company + PathOffset + "/" + programName;
};

int reportResults(sstr& fileNameBuilds, sstr& fileNameResult, sstr& programName, int step, int installResult)
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
    result += file_append_results(fileNameBuilds, programName, step, installResult);
    result += file_append_results(fileNameResult, programName, step, installResult);
    print_blank_lines(2);
    return result;
};

int main()
{
    OS_type thisOS = OS_type::Linux_Mint;
    Mac_PM  mpm    = Mac_PM ::Selection_Not_Available;

    //basic setup
    bool sectionLoaded      = false;
    bool createScriptOnly   = false;
    bool doTests            = false;
    bool alphaBuild         = true;

    sstr company = "/j5c";
    sstr version = "";
    sstr pVersion = "001";
    sstr buildPathOffset = "/build_" + pVersion;
    sstr prod_Stg_Offset = "/p"      + pVersion + "/stg";
    sstr prod_Src_Offset = "/p"      + pVersion + "/src";
    sstr prod_Usr_Offset = "/p"      + pVersion + "/usr";
    sstr programName = "Dependencies";
    sstr stgPathName = "stg";
    sstr stgPath = "xxx";
    sstr srcPath = "xxx";
    sstr usrPath = "xxx";
    sstr buildVersion = "";

    
    int step = -1;
    int result = 0;

    ensure_Directory_exists1(company);

    sstr fileName_Build = "Installation_Script_Builds_p" + pVersion + ".txt";
    sstr fileNameResult = "Installation_Script_Result_p" + pVersion + ".txt";
    create_file(fileName_Build);
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
        reportResults(fileName_Build, fileNameResult, programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    //perl setup
    programName = "perl";
    version = "5.26.1";
    step = -1;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_perl(company, fileName_Build, stgPath, srcPath, usrPath, version, createScriptOnly, doTests );
        reportResults(fileName_Build, fileNameResult, programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    //tcl setup
    programName = "tcl";
    version     = "8.6.8";
    step = -1;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_tcl(company, fileName_Build, stgPath, srcPath, usrPath, version, createScriptOnly, doTests, thisOS);
        reportResults(fileName_Build, fileNameResult, programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    //tk setup
    programName = "tk";
    version     = "8.6.8";
    step = -1;
    sstr oldPath = srcPath;
    stgPath = setPath(company, prod_Stg_Offset, programName);
    srcPath = setPath(company, prod_Src_Offset, programName);
    usrPath = setPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        result = install_tk(company, fileName_Build, stgPath, srcPath, usrPath, oldPath, version, createScriptOnly, doTests, thisOS);
        reportResults(fileName_Build, fileNameResult, programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    /*
    //Get and Build Apache Dependencies
    programName = "apr";
    version     = "1.6.3";
    step = 1;
    srcPath = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        removeDirectory(fileName_Build, srcPath, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_01(fileName_Build, srcPath, usrPath, version, createScriptOnly, doTests);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "apr-util";
    version     = "1.6.1";
    step = 2;
    srcPath = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        removeDirectory(fileName_Build, srcPath, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_02(fileName_Build, srcPath, usrPath, version, createScriptOnly, doTests);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "apr-iconv";
    version     = "1.2.2";
    step = 3;
    srcPath = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        removeDirectory(fileName_Build, srcPath, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_03(fileName_Build, srcPath, usrPath, version, createScriptOnly, doTests);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "pcre";
    version     = "8.41";
    step = 4;
    srcPath = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        removeDirectory(fileName_Build, srcPath, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_04(fileName_Build, srcPath, usrPath, version, createScriptOnly, doTests);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }


    programName = "pcre2";
    version     = "10.30";
    step = 5;
    srcPath = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        removeDirectory(fileName_Build, srcPath, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_05(fileName_Build, srcPath, usrPath, version, createScriptOnly, doTests);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "apache";
    version     = "2.4.29";
    step = -1;
    srcPath = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        removeDirectory(fileName_Build, srcPath, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache(fileName_Build, srcPath, usrPath, version, createScriptOnly, doTests);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "mariadb";
    version     = "10.3.3";
    step = -1;
    srcPath = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        removeDirectory(fileName_Build, srcPath, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_mariadb(fileName_Build, srcPath, usrPath, version, createScriptOnly, doTests);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "php";
    version     = "7.2.0";
    step = -1;
    srcPath = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        appendNewLogSection(fileName_Build);
        removeDirectory(fileName_Build, srcPath, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_php(fileName_Build, srcPath, usrPath, version, createScriptOnly, doTests);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
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