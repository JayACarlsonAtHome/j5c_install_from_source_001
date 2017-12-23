#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using sstr = std::string;

void install_required_dependencies()
{
    sstr federa_release = "7";
    sstr command = "";
    std::vector<sstr> vec_yum;
    vec_yum.push_back("yum -y install wget");
    vec_yum.push_back("wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-" + federa_release + ".noarch.rpm");
    vec_yum.push_back("yum -y install epel-release-latest-" + federa_release +".noarch.rpm");
    vec_yum.push_back("rm -f epel-release-latest-7.noarch.rpm");
    vec_yum.push_back("yum -y install autoconf");
    vec_yum.push_back("yum -y install bison");
    vec_yum.push_back("yum -y install boost-devel");
    vec_yum.push_back("yum -y install bzip2-devel");
    vec_yum.push_back("yum -y install cmake");
    vec_yum.push_back("yum -y install cmake-gui");
    vec_yum.push_back("yum -y install expat-devel");
    vec_yum.push_back("yum -y install ftp");
    vec_yum.push_back("yum -y install google-chrome-stable");
    vec_yum.push_back("yum -y install gitk");
    vec_yum.push_back("yum -y install gcc");
    vec_yum.push_back("yum -y install gcc-c++");
    vec_yum.push_back("yum -y install gcc-c++-x86_64-linux-gnu");
    vec_yum.push_back("yum -y install jemalloc-devel");
    vec_yum.push_back("yum -y install java-1.8.0-openjdk");
    vec_yum.push_back("yum -y install Judy");
    vec_yum.push_back("yum -y install libcurl-devel");
    vec_yum.push_back("yum -y install libedit-devel");
    vec_yum.push_back("yum -y install libicu-devel");
    vec_yum.push_back("yum -y install libjpeg-turbo-utils");
    vec_yum.push_back("yum -y install libjpeg-turbo-devel");
    vec_yum.push_back("yum -y install libpng-devel");
    vec_yum.push_back("yum -y install libstdc++");
    vec_yum.push_back("yum -y install libstdc++-devel");
    vec_yum.push_back("yum -y install libstdc++-docs");
    vec_yum.push_back("yum -y install libstdc++-static");
    vec_yum.push_back("yum -y install libwebp-devel");
    vec_yum.push_back("yum -y install libxml2-devel");
    vec_yum.push_back("yum -y install libxslt-devel");
    vec_yum.push_back("yum -y install openssl-devel");
    vec_yum.push_back("yum -y install re2c");
    vec_yum.push_back("yum -y install ruby");
    vec_yum.push_back("yum -y install tcltls-devel");
    vec_yum.push_back("yum -y install xml2");
    vec_yum.push_back("yum -y install vsqlite++-devel");

    for (auto it = vec_yum.cbegin(); it != vec_yum.cend(); ++it )
    {
        command =  *it;
        system(command.c_str());
    }

}

void install_perl(sstr& path)
{
    sstr command = "";
    std::vector<sstr> vec_yum;
    vec_yum.push_back("mkdir -p " + path);
    vec_yum.push_back("cd " + path);
    vec_yum.push_back("wget http://www.cpan.org/src/5.0/perl-5.26.1.tar.gz");
    vec_yum.push_back("tar xvf perl-5.26.1.tar.gz");
    vec_yum.push_back("cd /j5c/p001/perl/perl-5.26.1");
    vec_yum.push_back("./Configure -Dprefix=/j5c/p001/usr/perl -d -e");
    vec_yum.push_back("make");
    vec_yum.push_back("make test");
    vec_yum.push_back("make install");

    bool result = false;
    for (auto it = vec_yum.cbegin(); it != vec_yum.cend(); ++it )
    {
        command =  *it;
        result = system(command.c_str());
        if (!result)
        {
            std::cout << "!!!Error -- Process terminated for safety...";
            break;
        }
    }

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


int main()
{
    //basic setup
    sstr company = "/j5c";
    sstr version = "001";
    sstr buildPathOffset = "build_" + version;
    sstr prod_PathOffset = "p_"     + version;

    //perl setup
    sstr programName     = "perl";
    sstr path = company + prod_PathOffset + programName;

    install_required_dependencies();
    install_perl(path);



    return 0;
}