//
// Created by jay on 6/2/21.
//

#include "../type_abbreviations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_DEPENDENCIES_H
#define J5C_INSTALL_FROM_SOURCE_001_DEPENDENCIES_H

int install_apt_required_dependencies()
{
    std::cout << "Not Implemented Yet" << std::endl;
    return 1;
}

int install_mac_required_dependencies(Mac_PM mpm)
{
    int result = 0;
    if (mpm == Mac_PM ::Home_Brew) {
        //install_home_brew_required_dependencies();
        std::cout << "Not Implemented Yet" << std::endl;
        result = 1;
    }
    if (mpm == Mac_PM ::MacPorts) {
        //install_macPort_required_dependencies();
        std::cout << "Not Implemented Yet" << std::endl;
        result = 1;
    }
    return result;
}

int install_dnf_required_dependencies(sstr& fileName, sstr& programName, bool createScriptOnly)
{
    sstr fedora_release = "8";
    sstr command;
    std::vector<sstr> vec;
    vec.emplace_back("# Install " + programName + ".");
    vec.emplace_back("dnf -y install wget");
    //vec.emplace_back("wget https://dl.fedoraproject.org/pub/epel/epel-release-latest-" + fedora_release + ".noarch.rpm");
    //vec.emplace_back("dnf -y install epel-release-latest-" + fedora_release +".noarch.rpm");
    //vec.emplace_back("rm -f epel-release-latest-" + fedora_release +".noarch.rpm");
    vec.emplace_back("dnf -y group install \"Server with GUI\"");
    vec.emplace_back("dnf -y install apr-util-sqlite");
    vec.emplace_back("dnf -y install autoconf");
    vec.emplace_back("dnf -y install automake");
    vec.emplace_back("dnf -y install openssl-devel"); //this must be before cmake
    vec.emplace_back("dnf -y install bison");
    vec.emplace_back("dnf -y install boost-devel");
    vec.emplace_back("dnf -y install bzip2-devel");
    vec.emplace_back("dnf -y install clang");
    vec.emplace_back("dnf -y install cmake");
    vec.emplace_back("dnf -y install cmake-gui");
    vec.emplace_back("dnf -y install expat-devel");
    vec.emplace_back("dnf -y install flex");
    vec.emplace_back("dnf -y install ftp");
    vec.emplace_back("dnf -y install gawk");
    vec.emplace_back("dnf -y install git");
    vec.emplace_back("dnf -y install gitk");
    vec.emplace_back("dnf -y install gcc");
    vec.emplace_back("dnf -y install gcc-c++");
    vec.emplace_back("dnf -y install gcc-toolset-11*");
    vec.emplace_back("dnf -y install gnutls-c++");
    vec.emplace_back("dnf -y install gnutls-devel");
    vec.emplace_back("dnf -y install gtk2-devel");
    vec.emplace_back("dnf -y install gtk3-devel");
    vec.emplace_back("dnf -y install iotop");
    vec.emplace_back("dnf -y install java-1.8.0-openjdk");
    vec.emplace_back("dnf -y install java-17*");
    vec.emplace_back("dnf -y install Judy");
    vec.emplace_back("dnf -y install libcurl-devel");
    vec.emplace_back("dnf -y install libedit");
    vec.emplace_back("dnf -y install libffi-devel");
    vec.emplace_back("dnf -y install libicu-devel");
    vec.emplace_back("dnf -y install libcurl-devel");
    vec.emplace_back("dnf -y install libjpeg-turbo-utils");
    vec.emplace_back("dnf -y install libjpeg-turbo-devel");
    vec.emplace_back("dnf -y install libpng-devel");
    vec.emplace_back("dnf -y install libstdc++*");
    vec.emplace_back("dnf -y install libvmmalloc-devel");
    vec.emplace_back("dnf -y install libwebp-devel");
    vec.emplace_back("dnf -y install libxml2-devel");
    vec.emplace_back("dnf -y install libxslt-devel");
    vec.emplace_back("dnf -y install libX11-devel");
    vec.emplace_back("dnf -y install libzstd-devel");
    vec.emplace_back("dnf -y install ncurses-devel");
    vec.emplace_back("dnf -y install oniguruma");
    vec.emplace_back("dnf -y install pcre2-devel");
    vec.emplace_back("dnf -y install perl-CPAN");
    vec.emplace_back("dnf -y install perl-IPC*");
    vec.emplace_back("dnf -y install perl-Pod-Html");
    vec.emplace_back("dnf -y install pmempool");
    vec.emplace_back("dnf -y install podman");
    vec.emplace_back("dnf -y install postfix-sqlite");
    vec.emplace_back("dnf -y install sqlite-devel");
    vec.emplace_back("dnf -y install sqlite-libs");
    vec.emplace_back("dnf -y install xorg*");
    vec.emplace_back("dnf -y install yum-utils");
    vec.emplace_back("dnf -y install xz-devel");
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
    vec.emplace_back("apt install clang -y");
    vec.emplace_back("apt install cmake -y");
    vec.emplace_back("apt install cmake-gui -y");
    vec.emplace_back("apt install ftp -y");
    vec.emplace_back("apt install flex -y");
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
    vec.emplace_back("apt install libffi-dev -y");
    vec.emplace_back("apt install libicu-dev -y");
    vec.emplace_back("apt install libjemalloc-dev -y");
    vec.emplace_back("apt install libjpeg-dev -y");
    vec.emplace_back("apt install libjudy-dev -y");
    vec.emplace_back("apt install libncurses5-dev -y");
    vec.emplace_back("apt install libnghttp2");
    vec.emplace_back("apt install libonig-dev");
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
    vec.emplace_back("apt install qt4-qmake -y");
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


#endif //J5C_INSTALL_FROM_SOURCE_001_DEPENDENCIES_H
