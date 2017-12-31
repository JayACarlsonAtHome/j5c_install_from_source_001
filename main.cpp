#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdio.h>
#include <limits>

using sstr = std::string;

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



int create_file(sstr &fileName)
{
    std::ofstream file;
    int result = 1;  // assume failure
    file.open(fileName, std::ios::out | std::ios::trunc );
    if ( (file.is_open()) && (file.good()) )
    {
        file << "# Copyright J5C Marketing LLC" << std::endl;
        file << "# Jay A Carlson" << std::endl;
        file << "# jay.a.carlson@gmail.com" << std::endl;
        file << "# 360-649-6218" << std::endl;
        file << std::endl;
        file << std::endl;
        result = 0; // success
    }
    else
    {
        std::cout << "!!!Error -- unable to create file -- " << std::endl;
    }
    file.close();
    return result;
}

int file_write_vetor_to_file(sstr &fileName, std::vector <sstr> &vec_lines)
{
    std::ofstream file;
    int result = 1; // assume failure
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        for (auto it = vec_lines.cbegin(); it != vec_lines.cend(); ++it )
        {
            if (*it == "\n")
            {
                file << std::endl;
            }
            else
            {
                file << *it << std::endl;
            }

        }
        result = 0; // success
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
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        file << line << std::endl;
        result = 0;
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
    std::ofstream file;
    int result = 1;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        if (step < 0)
        {
            file << "Install " << programName << " : Result = " << installResult << std::endl;
        }
        else
        {
            file << "Install " << programName << " step " << std::setw(2) << std::setfill('0') << step <<  " : Result = " << installResult << std::endl;
        }

        result = 0;
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
    int result = 1;
    std::vector<sstr> vec;
    while (count > 0)
    {
        count -= 1;
        vec.push_back("\n");
    }
    result = file_write_vetor_to_file(fileName, vec);
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


bool prior_Results(sstr& fileNameResult, sstr& programName, int step)
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

            bool cond1 = (found2 == max);
            bool cond2 = (found3 == max);

            if ((!cond1) || (!cond2))
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

int removeDirectory(sstr& fileName, sstr& path, bool createScriptOnly)
{
    std::vector<sstr> vec;
    if (path.length() > 2)
    {
        vec.push_back("eval \"rm -rf " + path + "\"");
    }
    do_command(fileName, vec, createScriptOnly);
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

int install_perl(sstr& fileName, sstr& path, sstr& usrPath, sstr& verDir, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    std::vector<sstr> vec;
    vec.push_back("# Install Perl.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget http://www.cpan.org/src/" + verDir +"/perl-" + version + ".tar.gz\"");
    vec.push_back("eval \"cp ./perl-" + version + ".tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./perl-" + version + ".tar.gz \"");
    vec.push_back("eval \"cd " + path + "; tar xvf perl-" + version +".tar.gz\"");
    vec.push_back("eval \"mkdir -p " + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/perl-" + version + "; ./Configure -Dprefix=/j5c/p001/usr/perl -d -e\"");
    vec.push_back("eval \"cd " + path + "/perl-" + version + "; make \"");
    vec.push_back("eval \"cd " + path + "/perl-" + version + "; make test \"");
    vec.push_back("eval \"cd " + path + "/perl-" + version + "; make install \"");
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_tcl(sstr& fileName, sstr& path, sstr& usrPath, OS_type thisOS, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    if (thisOS == OS_type::RedHat)      installOS = "unix";
    if (thisOS == OS_type::Linux_Mint)  installOS = "unix";
    if (thisOS == OS_type::MaxOSX)      installOS = "macosx";

    vec.push_back("# Install Tcl.");
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
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}


int install_tk(sstr& fileName, sstr& path, sstr& usrPath, OS_type thisOS, sstr& oldPath, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    if (thisOS == OS_type::RedHat)      installOS = "unix";
    if (thisOS == OS_type::Linux_Mint)  installOS = "unix";
    if (thisOS == OS_type::MaxOSX)      installOS = "macosx";

    vec.push_back("# Install Tk.");
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
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}


int install_apache_step_01(sstr& fileName, sstr& path, sstr& usrPath, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    vec.push_back("# Install Apache Step 01 : Get apr source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget http://www.apache.org/dist/apr/apr-" + version +".tar.bz2\"");
    vec.push_back("eval \"cp ./apr-" + version + ".tar.bz2 " + path + "\"");
    vec.push_back("eval \"rm -f ./apr-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "; tar xvf apr-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "/apr-" + version + "; ./configure --prefix=" + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/apr-" + version + "; make\"");
    vec.push_back("eval \"cd " + path + "/apr-" + version + "; make test\"");
    vec.push_back("eval \"cd " + path + "/apr-" + version + "; make install\"");
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_02(sstr& fileName, sstr& path, sstr& usrPath, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    vec.push_back("# Install Apache Step 02: Get apr-util source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget http://www.apache.org/dist/apr/apr-util-" + version +".tar.gz\"");
    vec.push_back("eval \"cp ./apr-util-" + version + ".tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./apr-util-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "; tar xvf apr-util-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "/apr-util-" + version + "; ./configure --prefix=" + usrPath +"  --with-apr=" + usrPath.substr(0,17) + "\"");
    vec.push_back("eval \"cd " + path + "/apr-util-" + version + "; make\"");
    vec.push_back("eval \"cd " + path + "/apr-util-" + version + "; make test\"");
    vec.push_back("eval \"cd " + path + "/apr-util-" + version + "; make install\"");

    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_03(sstr& fileName, sstr& path, sstr& usrPath, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    vec.push_back("# Install Apache Step 03: Get apr-iconv source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget http://www.apache.org/dist/apr/apr-iconv-" + version +".tar.bz2\"");
    vec.push_back("eval \"cp ./apr-iconv-" + version + ".tar.bz2 " + path + "\"");
    vec.push_back("eval \"rm -f ./apr-iconv-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "; tar xvf apr-iconv-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "/apr-iconv-" + version + "; ./configure --prefix=" + usrPath +"  --with-apr=" + usrPath.substr(0,17) + "\"");
    vec.push_back("eval \"cd " + path + "/apr-iconv-" + version + "; make\"");
    // no tests -- will stop program if you try
    vec.push_back("eval \"cd " + path + "/apr-iconv-" + version + "; make install\"");

    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}


int install_apache_step_04(sstr& fileName, sstr& path, sstr& usrPath, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    vec.push_back("# Install Apache Step 04: Get pcre source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget https://ftp.pcre.org/pub/pcre/pcre-" + version +".tar.gz\"");
    vec.push_back("eval \"cp ./pcre-" + version + ".tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./pcre-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "; tar xvf pcre-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "/pcre-" + version + "; ./configure --prefix=" + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/pcre-" + version + "; make\"");
    vec.push_back("eval \"cd " + path + "/pcre-" + version + "; make test\"");
    vec.push_back("eval \"cd " + path + "/pcre-" + version + "; make install\"");

    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_apache_step_05(sstr& fileName, sstr& path, sstr& usrPath, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    vec.push_back("# Install Apache Step 05: Get pcre2 source.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget https://ftp.pcre.org/pub/pcre/pcre2-" + version +".tar.gz\"");
    vec.push_back("eval \"cp ./pcre2-" + version + ".tar.gz " + path + "\"");
    vec.push_back("eval \"rm -f ./pcre2-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "; tar xvf pcre2-" + version + ".tar.gz\"");
    vec.push_back("eval \"cd " + path + "/pcre2-" + version + "; ./configure --prefix=" + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/pcre2-" + version + "; make\"");
    // no tests -- will stop program if you try
    vec.push_back("eval \"cd " + path + "/pcre2-" + version + "; make install\"");

    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}


int install_apache_step_06(sstr& fileName, sstr& path, sstr& usrPath, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    sstr installOS = "";
    std::vector<sstr> vec;

    vec.push_back("# Install Apache Step 06: Get Apache source and install with dependencies.");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget http://www.apache.org/dist/httpd/httpd-" + version +".tar.bz2\"");
    vec.push_back("eval \"cp ./httpd-" + version + ".tar.bz2 " + path + "\"");
    vec.push_back("eval \"rm -f ./httpd-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "; tar xvf httpd-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "/httpd-" + version + "; ./configure --prefix=" + usrPath + "  "
                  + "--with-apr=" + usrPath.substr(0,13) + "/apr/bin  "
                  + "--with-apr-util="  + usrPath.substr(0,13) + "/apr-util   "
                  + "--with-apr-iconv=" + usrPath.substr(0,13) + "/apr-iconv  "
                  + "--with-pcre2=" + usrPath.substr(0,13) + "/pcre2 " + "\"");
    vec.push_back("eval \"cd " + path + + "/httpd-" + version + "; make\"");
    // no tests available
    vec.push_back("eval \"cd " + path + + "/httpd-" + version + "; make install\"");

    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}


int install_mariadb(sstr& fileName, sstr& path, sstr& usrPath, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    std::vector<sstr> vec;

    vec.push_back("# Install Mariadb");
    vec.push_back("eval \"mkdir -p " + path + "\"");
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
    vec.push_back("eval \"cd " + path + "/mariadb-" + version +"; make test\"");
    vec.push_back("eval \"cd " + path + "/mariadb-" + version +"; make install\"");
    vec.push_back("eval \"cd " + usrPath + "/mysql-test; ./mysql-test-run.pl --force \"");
    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
    return result;
}

int install_php(sstr& fileName, sstr& path, sstr& usrPath, sstr& version, bool createScriptOnly)
{
    sstr command = "";
    std::vector<sstr> vec;
    vec.push_back("# Install PHP");
    vec.push_back("eval \"mkdir -p " + path + "\"");
    vec.push_back("eval \"wget http://php.net/get/php-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cp ./php-" + version + ".tar.bz2 " + path + "\"");
    vec.push_back("eval \"rm -f ./php-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "; tar xvf php-" + version + ".tar.bz2\"");
    vec.push_back("eval \"cd " + path + "/php-" + version + "; ./configure --prefix=" + usrPath + "\"");
    vec.push_back("eval \"cd " + path + "/php-" + version + "; make\"");
    vec.push_back("eval \"cd " + path + "/php-" + version + "; make test\"");
    vec.push_back("eval \"cd " + path + "/php-" + version + "; make install\"");

    vec.push_back("eval \"cd /j5c\"");
    int result = do_command(fileName, vec, createScriptOnly);
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
    result += file_append_blank_lines(fileNameBuilds, 2);
    return result;
};

int main()
{
    OS_type thisOS = OS_type::RedHat;
    Mac_PM  mpm    = Mac_PM ::Selection_Not_Available;

    //basic setup
    bool sectionLoaded = false;
    bool createScriptOnly = false;

    sstr company = "/j5c";
    sstr verDir  = "";
    sstr version = "";
    sstr pVersion = "002";
    sstr buildPathOffset = "/build_" + pVersion;
    sstr prod_PathOffset = "/p"      + pVersion;
    sstr prod_Usr_Offset = "/p"      + pVersion + "/usr";
    sstr programName = "Dependencies";
    sstr path = "none";
    sstr usrPath = "none";
    int step = -1;
    int result = 0;



    ensure_Directory_exists1(company);
    sstr fileName_Build = "Installation_Script_Builds_p" + pVersion + ".txt";
    sstr fileNameResult = "Installation_Script_Result_p" + pVersion + ".txt";
    create_file(fileName_Build);

    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        if (thisOS == OS_type::RedHat) {
            install_yum_required_dependencies(fileName_Build, programName, createScriptOnly);
            print_blank_lines(2);
            file_append_blank_lines(fileName_Build, 2);
        }

        if (thisOS == OS_type::Linux_Mint) {
            install_apt_required_dependencies();
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
    verDir = "5.0";
    version = "5.26.1";
    step = -1;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_perl(fileName_Build, path, usrPath, verDir, version, createScriptOnly);
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
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_tcl(fileName_Build, path, usrPath, thisOS, version, createScriptOnly);
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
    sstr oldPath = path;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_tk(fileName_Build, path, usrPath, thisOS, oldPath, version, createScriptOnly);
        reportResults(fileName_Build, fileNameResult, programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    //Get and Build Apache Dependencies
    programName = "apr";
    version     = "1.6.3";
    step = 1;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_01(fileName_Build, path, usrPath, version, createScriptOnly);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "apr-util";
    version     = "1.6.1";
    step = 2;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_02(fileName_Build, path, usrPath, version, createScriptOnly);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "apr-iconv";
    version     = "1.2.2";
    step = 3;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_03(fileName_Build, path, usrPath, version, createScriptOnly);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "pcre";
    version     = "8.41";
    step = 4;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_04(fileName_Build, path, usrPath, version, createScriptOnly);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }


    programName = "pcre2";
    version     = "10.30";
    step = 5;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_05(fileName_Build, path, usrPath, version, createScriptOnly);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "apache";
    version     = "2.4.29";
    step = 6;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_apache_step_06(fileName_Build, path, usrPath, version, createScriptOnly);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "mariadb";
    version     = "10.2.11";
    step = 0;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_mariadb(fileName_Build, path, usrPath, version, createScriptOnly);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    programName = "php";
    version     = "7.2.0";
    step = -1;
    path = setPath(company, prod_PathOffset, programName);
    usrPath = setUsrPath(company, prod_Usr_Offset, programName);
    sectionLoaded = prior_Results(fileNameResult, programName, step);
    if (!sectionLoaded)
    {
        removeDirectory(fileName_Build, path, createScriptOnly);
        removeDirectory(fileName_Build, usrPath, createScriptOnly);
        result = install_php(fileName_Build, path, usrPath, version, createScriptOnly);
        reportResults(fileName_Build, fileNameResult,  programName, step, result);
    }
    else
    {
        section_already_loaded(programName, version);
    }

    return 0;
}