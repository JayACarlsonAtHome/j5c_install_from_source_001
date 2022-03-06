//  j5c_install_from_source_001 -- build a LAMP+ system from source
//  Copyright (C) 2018  Jay A Carlson of J5C Marketing LLC.
//
// This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, version 3.
//
// This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
// This program can automatically install a LAMP system + other stuff
// LAMP        := Linux, Apache Web Server, MariaDB, Php
// Other stuff := Perl, Ruby, Tcl/Tk, POCO C++ Libraries

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <limits>

#include "source/j5c_date.h"
#include "source/const_expressions.h"
#include "source/constants.h"
#include "source/enums.h"
#include "source/type_abbreviations.h"
#include "source/structs.h"

#include "source/function_declarations.h"
#include "source/Time_Functions/time_functions.h"
#include "source/String_Functions/string_functions.h"
#include "source/Permissions/permissions.h"
#include "source/Dependencies/dependencies.h"
#include "source/Commands/commands.h"

#include "source/Apache/apache_install.h"
#include "source/CMake/cmake.h"
#include "source/Judy/judy.h"
#include "source/Libzip/libzip.h"
#include "source/Logging/logging.h"
#include "source/MariaDB/mariadb.h"
#include "source/Perl/perl5.h"
#include "source/Perl/perl6.h"
#include "source/PHP/php.h"
#include "source/Poco/poco_Install.h"
#include "source/PostFix/postfix.h"
#include "source/Python/python.h"
#include "source/Ruby/ruby.h"
#include "source/TCL_TK/tcl.h"
#include "source/TCL_TK/tk.h"
#include "source/TLS_SSL/tls_ssl.h"
#include "source/wxWidgets/wxwidgets.h"

using namespace J5C_DSL_Code;


/***   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ***   !!!         Main Starts Here                  !!!
 ***   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ***/

int main() {


    struct programs
    {
        an_itemValues itemValues;
        int  step;
        int (*funptr)(std::map<sstr, sstr> &settings, an_itemValues& itemValues);
    } program;

    OS_type thisOSType;
    Mac_PM mpm;
    sstr prefix;
    sstr protectModeText;
    time_t programStart;
    time_t programStop;
    programStart = get_Time();

    // get settings from file
    sstr fileSettings = "/home/J5C_Install/Install_Settings.cfg";
    std::map<sstr, sstr> settings;
    system("pwd");
    settings = getProgramSettings(fileSettings);
    std::vector<sstr> generated_settings;

    // put settings into program variables
    std::cout << "Putting settings into program variables..." << std::endl;
    sstr pricomy = "/usr/local/j5c";
    sstr company    = settings[KEY_COMPANY_NAME];

    sstr usePrefix  = settings[KEY_TOUSE_PREFIX];
    bool bUsePrefix = getBoolFromString(usePrefix);
    if (bUsePrefix) {
        prefix = settings[KEY_DEFLT_PREFIX];
        program.itemValues.company = joinPathParts(prefix, company);
    } else {
        sstr beginPath = "/";
        program.itemValues.company = joinPathParts(beginPath, company);
    }

    sstr pVersion = settings[KEY_PATH_VERSION];
    pVersion = getValid_pVersion(pVersion);
    program.itemValues.pathVersion = "p" + pVersion;

    // assign a default; and change if not correct...
    thisOSType = OS_type::RedHat;
    mpm = Mac_PM::No_Selection_Made;
    sstr theOStext = settings[KEY_AN_OS_SYSTEM];
    theOStext = lowerCaseString(theOStext);
    if (theOStext == "cent os")    { thisOSType = OS_type::CentOS;      }
    if (theOStext == "cent_os")    { thisOSType = OS_type::CentOS;      }
    if (theOStext == "centos")     { thisOSType = OS_type::CentOS;      }
    if (theOStext == "fedora")     { thisOSType = OS_type::Fedora;      }
    if (theOStext == "linux mint") { thisOSType = OS_type::Linux_Mint;  }
    if (theOStext == "linux_mint") { thisOSType = OS_type::Linux_Mint;  }
    if (theOStext == "linuxmint")  { thisOSType = OS_type::Linux_Mint;  }
    if (theOStext == "red hat")    { thisOSType = OS_type::RedHat;      }
    if (theOStext == "red_hat")    { thisOSType = OS_type::RedHat;      }
    if (theOStext == "redhat")     { thisOSType = OS_type::RedHat;      }
    if (theOStext == "OSX") {
        thisOSType = OS_type::MaxOSX;
        // TODO add a setting for Homebrew or Mac Ports
        // there currently is no setting for this yet so we will pick the default for now.
        mpm = Mac_PM::MacPorts;
    }

    program.itemValues.thisOSType = thisOSType;

    protectModeText = settings[KEY_PROTECT_MODE];
    program.itemValues.bProtectMode = getBoolFromString(protectModeText);

    sstr runDepds = settings[KEY_RUN_DEPENDCS];
    bool runDependencies = getBoolFromString(runDepds);

    bool sectionLoaded = false;

    sstr temp = "p" + pVersion;
    program.itemValues.rtnPath = joinPathParts(program.itemValues.company, temp);
    temp = STG_NAME;
    program.itemValues.cpyStgPath = joinPathParts(program.itemValues.company, temp);
    temp = "xxx";
    sstr xxxPath  = joinPathParts(program.itemValues.rtnPath, temp);
    sstr programName = "Dependencies";
    program.itemValues.programName = programName;
    program.itemValues.ProperName  = getProperNameFromString(program.itemValues.programName);
    program.itemValues.tlsPath = get_xxx_Path(xxxPath, "tls");
    program.itemValues.wwwPath = get_xxx_Path(xxxPath, "www");

    ensure_Directory_exists1(program.itemValues.rtnPath);
    ensure_Directory_exists1(program.itemValues.cpyStgPath);
    ensure_Directory_exists1(program.itemValues.tlsPath);
    ensure_Directory_exists1(program.itemValues.wwwPath);

    sstr getPath = "xxx";
    sstr buildVersion;

    temp = "Installation_Builds_p" + pVersion + ".txt";
    program.itemValues.fileName_Build = joinPathWithFile(program.itemValues.rtnPath, temp);

    temp = "Installation_Notes_p" + pVersion + ".txt";
    program.itemValues.fileName_Notes = joinPathWithFile(program.itemValues.rtnPath, temp);

    temp = "Installation_Results_p" + pVersion + ".txt";
    program.itemValues.fileName_Results = joinPathWithFile(program.itemValues.rtnPath, temp);

    std::vector<sstr> vec;
    program.itemValues.step             = -1;
    program.itemValues.itemStartTime    = get_Time();


    create_file(program.itemValues.fileName_Build);
    do_command(program.itemValues.fileName_Build, vec, false);

    if (!(isFileSizeGtZero(program.itemValues, program.itemValues.fileName_Notes))) {
        create_file(program.itemValues.fileName_Notes);
    } else {
        ensure_file(program.itemValues.fileName_Notes);
    }

    if (!(isFileSizeGtZero(program.itemValues, program.itemValues.fileName_Results))) {
        create_file(program.itemValues.fileName_Results);
    } else {
        ensure_file(program.itemValues.fileName_Results);
    }

    int result = 0;
    sectionLoaded = prior_Results(program.itemValues.fileName_Results, programName);
    if (!sectionLoaded) {
        bool bScriptOnly = !runDependencies;
        if ((thisOSType == OS_type::RedHat) || (thisOSType == OS_type::CentOS) || (thisOSType == OS_type::Fedora)) {
            install_dnf_required_dependencies(program.itemValues.fileName_Build, program.itemValues.programName, bScriptOnly);
            print_blank_lines(2);
        }

        if (thisOSType == OS_type::Linux_Mint) {
            install_apt_required_dependencies(program.itemValues.fileName_Build, programName, bScriptOnly);
        }

        if (thisOSType == OS_type::MaxOSX) {
            install_mac_required_dependencies(mpm);
        }
        reportResults(program.itemValues, result);
    } else {
        sstr version = "";
        section_already_loaded(programName, version);
    }

    std::vector<programs> progVector;

    program.itemValues.programName = "cmake";
    program.itemValues.step        = -1;
    program.funptr = &install_cmake;
    progVector.emplace_back(program);

    program.itemValues.programName = "libzip";
    program.itemValues.step        = -1;
    program.funptr = &install_libzip;
    progVector.emplace_back(program);

    program.itemValues.programName = "perl";
    program.itemValues.step        = -1;
    program.funptr = &install_perl5;
    progVector.emplace_back(program);

    program.itemValues.programName = "openssl";
    program.itemValues.step        = -1;
    program.funptr = &install_openssl;
    progVector.emplace_back(program);

    program.itemValues.programName = "Judy";
    program.itemValues.step        = -1;
    program.funptr = &install_judy;
    progVector.emplace_back(program);

    program.itemValues.programName = "mariadb";
    program.itemValues.step        = -1;
    program.funptr = &install_mariadb;
    progVector.emplace_back(program);

    program.itemValues.programName = "apr";
    program.itemValues.step        = 1;
    program.funptr = &install_apache_step_01;
    progVector.emplace_back(program);

    program.itemValues.programName = "apr-util";
    program.itemValues.step        = 2;
    program.funptr = &install_apache_step_02;
    progVector.emplace_back(program);

    program.itemValues.programName = "apr-iconv";
    program.itemValues.step        = 3;
    program.funptr = &install_apache_step_03;
    progVector.emplace_back(program);

    program.itemValues.programName = "pcre2";
    program.itemValues.step        = 5;
    program.funptr = &install_apache_step_05;
    progVector.emplace_back(program);

    program.itemValues.programName = "apache";
    program.itemValues.step        = -1;
    program.funptr = &install_apache;
    progVector.emplace_back(program);

    program.itemValues.programName = "perl6";
    program.itemValues.step        = -1;
    program.funptr = &install_perl6;
    progVector.emplace_back(program);

    program.itemValues.programName = "php";
    program.itemValues.step        = -1;
    program.funptr = &install_php;
    progVector.emplace_back(program);

    program.itemValues.programName = "poco";
    program.itemValues.step        = -1;
    program.funptr = &install_poco;
    progVector.emplace_back(program);

    program.itemValues.programName = "postfix";
    program.itemValues.step        = -1;
    program.funptr = &install_postfix;
    progVector.emplace_back(program);

    program.itemValues.programName = "python";
    program.itemValues.step        = -1;
    program.funptr = &install_python;
    progVector.emplace_back(program);

    program.itemValues.programName = "ruby";
    program.itemValues.step        = -1;
    program.funptr = &install_ruby;
    progVector.emplace_back(program);

    program.itemValues.programName = "tcl";
    program.itemValues.step        =  1;
    program.funptr = &install_tcl;
    progVector.emplace_back(program);

    program.itemValues.programName = "tk";
    program.itemValues.step        =  2;
    program.funptr = &install_tk;
    progVector.emplace_back(program);

    program.itemValues.programName = "wxWidgets";
    program.itemValues.step        =  -1;
    program.funptr = &install_wxwidgets;
    progVector.emplace_back(program);

    program.itemValues.thisOSType   = thisOSType;

    //function pointer declaration
    int (*funptr)(std::map<sstr, sstr> &settings, an_itemValues& itemValues);

    bool anyInstalled = false;

    ensureMariaDB_UserAndGroupExist(program.itemValues);
    ensureApacheUserAndGroupExists(program.itemValues);

    //
    //  This is where the main code starts to do something...
    //  Unless your an expert in C++ you will never figure out what this does
    //    so I will explain it a little.
    //  We have a small structure -> struct programs that contains another structure called itemValues
    //    Structure programs contains 3 items.
    //         1. The Programs "itemValues" (a struct the contains things like name, install path, build path, run path, etc)
    //         2. The Program step (in the case of Apache Web Server there are 5 steps, where there is only 1 step we put -1 )
    //         3. And a function pointer that holds the address of the function to run to load the program.
    //  We load all the programs into a vector, so we can loop through it, (iterate through it)

    for( auto& it: progVector )
    {
        result = -1;
        // get the start time
        it.itemValues.itemStartTime = get_Time();
        // at the start of the program we loaded all the settings from a file
        // the next step will load the settings that apply to the program we are installing
        // The settings may say to skip the install, so the skip value is the return value
        bool skip = set_settings(settings, it.itemValues);

        //if we are not skipping then we are loading the program
        if (!skip) {
            // we set the function pointer to point to the function that loads this particle program.
            // we set this by hand at like this... program.funptr = &install_cmake;   (for the cmake program)
            // the other programs each have their hand coded function.
            funptr = (it.funptr);
            //we process the installation depending on the name and the functions
            //although there is some commonality in the "install" functions, they are almost all unique
            //many of the settings, and programs must work together, so the set of programs and versions
            //that have been provided in the settings file are designed to work together as a set or a stack.
            result = process_section(settings, funptr, it.itemValues);
            // If we succeed with any install we retain that knowledge
            if (result > -1) { anyInstalled = true; }
        }
    }

    // If we installed anything then show that we ended.
    if (anyInstalled) {
        programStop = get_Time();
        sstr end = "End of Program";
        //this is typical logging, in that I have a file name and path
        // and pass some values, the functions will open the files, and append the data.
        file_append_line(program.itemValues.fileName_Build,   end, programStop, programStart);
        file_append_line(program.itemValues.fileName_Results, end, programStop, programStart);
    }
    return 0;
}

//A few other notes...
//This was written before "filesystem" header was created
//   So there is a lot of extra code that probably could be eliminated at some time.
//Installing some of these things the first time from source if you have never
//  done it before can be hard.  The logging from this program pretty much gives
//  you the commands as they would be typed out by hand.  You can copy and paste
//  the commands, (you will have to strip off the left side time stamps etc)
//Originally this was written as one file, except "J5C_Date" code
//  but it was getting too big to work on effectively, so I broke it up into smaller files.
