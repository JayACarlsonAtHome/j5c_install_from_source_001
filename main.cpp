#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

enum class Os_type{Linux_Mint = 0, CentOS = 1, RedHat = 2, MaxOSX = 3};

using sstr = std::string;

int do_command(std::vector<sstr>& vec)
{
    sstr command = "";
    int result = 0;
    for (auto it = vec.cbegin(); it != vec.cend(); ++it )
    {
        command =  *it;
        std::cout << "Command: " << command << std::endl;
        result = system(command.c_str());
        //result = 0;
        if ((result != 0) && (!(command.substr(0,3) == "yum")))
        {
            std::cout << "!!!Error -- Process terminated for safety...";
            break;
        }
    }
    return result;
}

int install_required_dependencies()
{
    sstr federa_release = "7";
    sstr command = "";
    std::vector<sstr> vec;
    vec.push_back("yum -y install wget");
    vec.push_back("wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-" + federa_release + ".noarch.rpm");
    vec.push_back("yum -y install epel-release-latest-" + federa_release +".noarch.rpm");
    vec.push_back("rm -f epel-release-latest-7.noarch.rpm");
    vec.push_back("yum -y install autoconf");
    vec.push_back("yum -y install bison");
    vec.push_back("yum -y install boost-devel");
    vec.push_back("yum -y install bzip2-devel");
    vec.push_back("yum -y install cmake");
    vec.push_back("yum -y install cmake-gui");
    vec.push_back("yum -y install expat-devel");
    vec.push_back("yum -y install ftp");
    vec.push_back("yum -y install google-chrome-stable");
    vec.push_back("yum -y install gitk");
    vec.push_back("yum -y install gcc");
    vec.push_back("yum -y install gcc-c++");
    vec.push_back("yum -y install gcc-c++-x86_64-linux-gnu");
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
    vec.push_back("yum -y install openssl-devel");
    vec.push_back("yum -y install re2c");
    vec.push_back("yum -y install ruby");
    vec.push_back("yum -y install tcltls-devel");
    vec.push_back("yum -y install xml2");
    vec.push_back("yum -y install vsqlite++-devel");

    int result = do_command(vec);

    return result;
}

int install_perl(sstr& path, sstr& usrPath)
{
    sstr command = "";
    std::vector<sstr> vec;
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget http://www.cpan.org/src/5.0/perl-5.26.1.tar.gz\"");
    vec.push_back("eval \"cp ./perl-5.26.1.tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./perl-5.26.1.tar.gz \"");
    vec.push_back("eval \"cd " + path + "; tar xvf perl-5.26.1.tar.gz\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/perl-5.26.1; ./Configure -Dprefix=/j5c/p001/usr/perl -d -e\"");
    vec.push_back("eval \"cd " + path + "/perl-5.26.1; make \"");
    vec.push_back("eval \"cd " + path + "/perl-5.26.1; make test \"");
    vec.push_back("eval \"cd " + path + "/perl-5.26.1; make install \"");
    int result = do_command(vec);
    return result;
}

int install_tcl(sstr& path, sstr& usrPath, Os_type thisOS)
{
    sstr command = "";
    std::vector<sstr> vec;
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget https://prdownloads.sourceforge.net/tcl/tcl8.6.7-src.tar.gz\"");
    vec.push_back("eval \"cp ./tcl8.6.7-src.tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./tcl8.6.7-src.tar.gz \"");
    vec.push_back("eval \"cd " + path + "; tar xvf tcl8.6.7-src.tar.gz\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");
    if ((thisOS == Os_type::Linux_Mint)
        || (thisOS == Os_type::CentOS)
        || (thisOS == Os_type::RedHat))
    {
        vec.push_back("eval \"cd " + path + "/tcl8.6.7; ./unix/configure --prefix=/j5c/p001/usr/tcl --enable-threads --enable-shared --enable-symbols;  \"");
    }

    if (thisOS == Os_type::MaxOSX)
    {
        vec.push_back("eval \"cd " + path + "/tcl8.6.7; ./macosx/configure --prefix=/j5c/p001/usr/tcl --enable-threads --enable-shared --enable-symbols;  \"");
    }
    vec.push_back("eval \"cd " + path + "/tcl8.6.7; make \"");
    vec.push_back("eval \"cd " + path + "/tcl8.6.7; make test \"");
    vec.push_back("eval \"cd " + path + "/tcl8.6.7; make install \"");
    int result = do_command(vec);
    return result;
}



bool ensure_Directory_exists(sstr& basePath, sstr& path)
{
    bool result = false;
    sstr fullpath = basePath + path;
    if (basePath.substr(basePath.length()-1, 1) != "/")   {
        sstr fullPath = basePath + "/" + path;
    }
    sstr command  = "mkdir -p ";
    sstr fullCommand = command + fullpath;
    result = system(fullCommand.c_str());
    if (result) {
        std::cout << "success on running command:" << std::endl;
    } else {
        std::cout << "failure on running command:" << std::endl;
    }
    std::cout << "  " << fullCommand << std::endl;
    return result;
}

bool File_Header(std::ofstream& file, sstr& fileName)
{
    bool result = false;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        file << "# Copyright J5C Marketing LLC" << std::endl;
        file << "# Jay A Carlson" << std::endl;
        file << "# jay.a.carlson@gmail.com" << std::endl;
        file << "# 360-649-6218" << std::endl;
        file << std::endl;
        file << std::endl;
        result = true;
    }
    else
    {
        std::cout << "!!!Error -- unable to write to file -- " << std::endl;
    }
    file.close();
    return result;
}

bool File_Contents(std::ofstream& file, sstr& fileName, std::vector<sstr>& vec_lines)
{
    bool result = false;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        for (auto it = vec_lines.cbegin(); it != vec_lines.cend(); ++it )
        {
            file << *it << std::endl;
        }
        result = true;
    }
    else
    {
        std::cout << "!!!Error -- unable to write to file -- " << std::endl;
    }
    file.close();
    return result;
}

sstr setPath(sstr& company, sstr& prod_PathOffset, sstr& programName)
{
    return company + prod_PathOffset + "/" + programName;

};

sstr setUsrPath(sstr& company, sstr& prod_Usr_Offset, sstr& programName)
{
    return company + prod_Usr_Offset + "/" + programName;
};
int main()
{
    Os_type thisOS = Os_type ::RedHat;

    //ensure dependencies are met
    install_required_dependencies();

    //basic setup
    sstr company = "/j5c";
    sstr version = "001";
    sstr buildPathOffset = "/build_" + version;
    sstr prod_PathOffset = "/p"     + version;
    sstr prod_Usr_Offset = "/p"     + version + "/usr";
    sstr programName = "none";
    sstr path = "none";
    sstr usrPath = "none";
    int result = 0;

    //perl setup
    programName = "perl";
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    result = install_perl(path, usrPath);
    std::cout << "Install Perl result = " << result << ".";

    //tcl setup
    programName = "tcl";
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    result = install_tcl(path, usrPath, thisOS);
    std::cout << "Install Tcl result = " << result << ".";

    return 0;
}