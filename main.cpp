#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

enum class OS_type { Selection_Not_Available = -1, No_Selection_Made = 0, Linux_Mint = 1, CentOS = 2, RedHat = 3, MaxOSX = 4};
enum class Mac_PM  { Selection_Not_Available = -1, No_Selection_Made = 0, Home_Brew = 0, MacPorts = 1 };

using sstr = std::string;

int do_command(std::vector<sstr>& vec)
{
    sstr command = "";
    int result = 0;
    for (auto it = vec.cbegin(); it != vec.cend(); ++it )
    {
        result = 0;
        command =  *it;
        std::cout << "Command: " << command << std::endl;
        if (command.substr(0,1) != "#")
        {
            result = system(command.c_str());
        }

        if ((result != 0) && (!((command.substr(0,3) == "apt") || (command.substr(0,3) == "yum"))) )
        {
            std::cout << "!!!Error -- Process terminated for safety...";
            break;
        }
    }
    return result;
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
    }
    if (mpm == Mac_PM ::MacPorts) {
        //install_macPort_required_dependencies();
    }
    return 1;
}

int install_yum_required_dependencies()
{
    sstr fedora_release = "7";
    sstr command = "";
    std::vector<sstr> vec;
    vec.push_back("yum -y install wget");
    vec.push_back("wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-" + fedora_release + ".noarch.rpm");
    vec.push_back("yum -y install epel-release-latest-" + fedora_release +".noarch.rpm");
    vec.push_back("rm -f epel-release-latest-" + fedora_release +".noarch.rpm");
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
    vec.push_back("yum -y install libX11-devel");
    vec.push_back("yum -y install openssl-devel");
    vec.push_back("yum -y install re2c");
    vec.push_back("yum -y install ruby");
    vec.push_back("yum -y install sqlite-devel");
    vec.push_back("yum -y install sqlite-tcl");
    vec.push_back("yum -y install tcltls-devel");
    vec.push_back("yum -y install xml2");
    vec.push_back("yum -y xorg-x11-fonts*");
    vec.push_back("yum -y xorg-x11-server-Xnest");
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
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(vec);
    return result;
}

int install_tcl(sstr& path, sstr& usrPath, OS_type thisOS, sstr& version)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    if (thisOS == OS_type::RedHat)      installOS = "unix";
    if (thisOS == OS_type::Linux_Mint)  installOS = "unix";
    if (thisOS == OS_type::MaxOSX)      installOS = "macosx";

    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget https://prdownloads.sourceforge.net/tcl/tcl" + version + "-src.tar.gz\"");
    vec.push_back("eval \"cp ./tcl" + version + "-src.tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./tcl" + version + "-src.tar.gz \"");
    vec.push_back("eval \"cd " + path + "; tar xvf tcl" + version + "-src.tar.gz\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/tcl" +version + "/" + installOS
                  + "; ./configure --prefix="  + usrPath + " --enable-threads --enable-shared --enable-symbols;  \"");
    vec.push_back("eval \"cd " + path + "/tcl" +version + "/" + installOS + "; make \"");
    vec.push_back("eval \"cd " + path + "/tcl" +version + "/" + installOS + "; make test \"");
    vec.push_back("eval \"cd " + path + "/tcl" +version + "/" + installOS + "; make install \"");
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(vec);
    return result;
}


int install_tk(sstr& path, sstr& usrPath, OS_type thisOS, sstr& oldPath, sstr& version)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    if (thisOS == OS_type::RedHat)      installOS = "unix";
    if (thisOS == OS_type::Linux_Mint)  installOS = "unix";
    if (thisOS == OS_type::MaxOSX)      installOS = "macosx";

    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget https://prdownloads.sourceforge.net/tcl/tk" + version +"-src.tar.gz\"");
    vec.push_back("eval \"cp ./tk" + version + "-src.tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./tk"+ version +"-src.tar.gz \"");
    vec.push_back("eval \"cd " + path + "; tar xvf tk" + version + "-src.tar.gz\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/tk" + version + "/" + installOS
                  + "; ./configure --prefix="  + usrPath + " --with-tcl=" + oldPath + "/tcl" + version +"/" + installOS
                  + " --enable-threads --enable-shared --enable-symbols;  \"");
    vec.push_back("eval \"cd " + path + "/tk" + version + "/" + installOS + "; make \"");
    vec.push_back("eval \"cd " + path + "/tk" + version + "/" + installOS + "; make test \"");
    vec.push_back("eval \"cd " + path + "/tk" + version + "/" + installOS + "; make install \"");

    vec.push_back("eval \"cd /j5c\"");
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
    OS_type thisOS = OS_type::RedHat;
    Mac_PM  mpm    = Mac_PM ::Selection_Not_Available;


    if (thisOS == OS_type::RedHat)
    {
        install_yum_required_dependencies();
    }

    if (thisOS == OS_type::Linux_Mint)
    {
        install_apt_required_dependencies();
    }

    if (thisOS == OS_type::MaxOSX)
    {
        install_mac_required_dependencies(mpm);
    }

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
    std::cout << "Install Perl result = " << result << "." << std::endl;

    //tcl setup
    programName = "tcl";
    version     = "8.6.8";
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    result = install_tcl(path, usrPath, thisOS, version);
    std::cout << "Install Tcl result = " << result << "." << std::endl;

    //tk setup
    programName = "tk";
    version     = "8.6.8";
    sstr oldPath = path;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    result = install_tk(path, usrPath, thisOS, oldPath, version);
    std::cout << "Install Tk result = " << result << "." << std::endl;

    return 0;
}