//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_COMMANDS_H
#define J5C_INSTALL_FROM_SOURCE_001_COMMANDS_H

sstr multilineCommand(std::vector<sstr>& commands, bool remove_last_continuation_char) {

    sstr result = "";


    if (commands.size() > 0) {
        // this is to just get the type
        auto maxLen = commands[0].length();
        auto curLen = maxLen;
        auto padLen = maxLen;
        auto constPad = maxLen;
        constPad = 2;
        sstr padding = "";

        for (auto element : commands) {
            curLen = element.length();
            if (curLen > maxLen) {
                maxLen = curLen;
            }
        }

        bool first = true;
        for (auto element : commands) {
            curLen = element.length();
            padLen = maxLen - curLen + constPad;
            result.append(element);
            padding.clear();

            //This (padLen > 0) should always be true as long as constPad > 0
            // but if constPad is changed by a programmer in the future this will protect
            // future code changes from breaking code.

            if (padLen > 0) {
                padding = std::string(padLen, ' ');
                result.append(padding);
            }
            result.append("\\\n");
        }
    }
    if (remove_last_continuation_char) {
        result = result.substr(0, result.length() - 2);
    }
    return result;
}

bool prior_Results(sstr& fileNameResult, sstr& searchStr)
{
    auto count = 50000ul;
    bool result = false;
    sstr it_data;
    searchStr = getProperNameFromString(searchStr);
    auto max = std::numeric_limits<unsigned long>::max();

    std::vector<sstr> data = readFile(fileNameResult, count);
    for (const auto& it : data )
    {
        it_data =  it;

        auto found1 = it_data.find(searchStr);
        if ( found1 != max )
        {
            auto found2 = it_data.find("Result =");
            sstr temp1;
            if (found2 < max)
            {
                temp1 = it_data.substr(found2+8,it_data.length());
            }
            auto end = temp1.find_first_of(':');
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
    auto maxLineCount = 500ul;
    std::map<sstr, sstr> result;
    sstr it_data;
    sstr delimiter;
    sstr key;
    sstr value;
    sstr ws = " \t\n\r";

    //load general default values into map

    result.emplace(std::pair<sstr , sstr >(KEY_COMPANY_NAME, VAL_COMPANY_NAME));
    result.emplace(std::pair<sstr , sstr >(KEY_DEFLT_PREFIX, VAL_DEFLT_PREFIX));
    result.emplace(std::pair<sstr , sstr >(KEY_TOUSE_PREFIX, VAL_TOUSE_PREFIX));
    result.emplace(std::pair<sstr , sstr >(KEY_PATH_VERSION, VAL_PATH_VERSION));
    result.emplace(std::pair<sstr , sstr >(KEY_AN_OS_SYSTEM, VAL_AN_OS_SYSTEM));
    result.emplace(std::pair<sstr , sstr >(KEY_RUN_DEPENDCS, VAL_RUN_DEPENDCS));
    result.emplace(std::pair<sstr , sstr >(KEY_PROTECT_MODE, VAL_PROTECT_MODE));

    int width = 32;
    std::cout << std::endl << std::endl;
    std::cout << "#    Listing Settings : Values (Defaults)" << std::endl;
    std::cout << "#=========================================================" << std::endl;
    for (auto element : result)
    {
        std::cout << ": " << std::setw(width) << element.first << " : " << element.second << std::endl;
    }

    std::cout << std::endl;
    std::vector<sstr> data = readFile(fileSettings, maxLineCount);
    for (const auto& it : data )
    {
        it_data = it;
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
    for (auto element : result)
    {
        std::cout << ": " << std::setw(width) << element.first << " : " << element.second << std::endl;
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

int do_command(sstr& fileName, std::vector<sstr>& vec, bool createScriptOnly )
{
    time_t start;
    time_t stop;
    sstr command;
    int result = 0;
    bool outputToFile = false;
    if (fileName.length()> 0)
    {
        outputToFile = true;
    }
    else
    {
        std::cout << "Doing commands will not be saved to file." << std::endl;
        std::cout << "Because no file name was given."           << std::endl;
    }
    for (const auto& it : vec )
    {
        start  = get_Time();
        sstr strStart = get_Time_as_String(start);
        result = 0;
        command = it;

        std::cout << " " << strStart << "Command: " << command << std::endl;

        if ((command.substr(0,1) != "#") && (command.substr(0,1) != ":")) {
            if (!createScriptOnly) {
                result += system(command.c_str());
            }
        }

        if ((result != 0) && (!((command.substr(0,3) == "apt") || (command.substr(0,3) == "yum") || (command.substr(0,3) == "dnf"))) )
        {
            std::cout << "!!!Error -- Process terminated for safety..." << std::endl;
            break;
        }

        stop = get_Time();

        if (outputToFile) {
            //
            //  We need to position the output so that it
            //    is not under the start, stop and duration times.
            bool cont = true;
            auto startPos = command.find('\n');
            sstr temp = std::string(commandPosition + 4, ' ');
            sstr replacement = "\n";
            replacement.append(temp);
            while (cont)
            {
                if (startPos != -1) {
                    command.replace(startPos, 1, replacement, 0, replacement.length());
                }
                else
                {
                    cont = false;
                }
                startPos = command.find('\n', startPos + replacement.length() + 1);
            }
            file_append_line(fileName, command, stop, start);

            if ((result != 0) && (!((command.substr(0,3) == "apt") || (command.substr(0,3) == "yum"))) )
            {
                command = "!!!Error -- Process terminated for safety...";
                file_append_line(fileName, command, stop, start);
                break;
            }
        }
    }
    return result;
}

sstr get_sha256sum(an_itemValues& itemValues)
{
    sstr result  = "Sha256sum not found.";
    sstr positionCommand = std::string(commandPosition, ' ');
    auto length  = itemValues.sha256sum_Config.length();
    sstr sha256File = "sha256sum.txt";
    std::vector<sstr> vec;
    sstr it_data;
    sstr outPathFileName = joinPathWithFile(itemValues.stgPath, sha256File);
    sstr command = "rm -f " + itemValues.stgPath + "sha256sum.txt;";
    command.append("\n");
    command.append(positionCommand);
    command.append("sha256sum ");
    command.append(itemValues.stgPath);
    command.append( "* > ");
    command.append(outPathFileName);
    vec.emplace_back(command);

    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    ensure_file(outPathFileName);
    vec.clear();
    // this is assuming there won't be more than 1000 different versions
    //  in the same directory -- potentially a bad decision, but unlikely
    vec = readFile(outPathFileName, 1000);
    for (const auto& it : vec)
    {
        it_data = it;
        auto find = it_data.find(itemValues.fileName_Compressed);
        if (find < it_data.length())
        {
            result = it_data.substr(0,length);
            break;
        }
    }
    return result;
}

bool check_Sha256sum(an_itemValues& itemValues)
{
    bool result = false;
    itemValues.sha256sum_Real = get_sha256sum(itemValues);
    sstr positionCommand = std::string(commandPosition, ' ');
    std::vector<sstr> vec;
    vec.clear();
    sstr command {"# Expected sha256sum_Config = "};
    command.append(itemValues.sha256sum_Config);
    vec.emplace_back(command);
    command.clear();
    command.append("# Actual   sha256sum_File   = ");
    command.append(itemValues.sha256sum_Real);
    vec.emplace_back(command);
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if ((itemValues.sha256sum_Real == itemValues.sha256sum_Config) && (itemValues.sha256sum_Real.length() > 1 ))
    {
        result = true;
    }
    return result;
}

int badSha256sum(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    vec.clear();
    vec.emplace_back("# ");
    vec.emplace_back("# Shaw256sums are not acceptable: ");
    vec.emplace_back("#!!!--Warning--!!! Skipping installation due to security concerns !!! ");
    vec.emplace_back("#");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    // just an arbitrary number greater than 0;
    return 2;
}

int configure(an_itemValues& itemValues,  sstr& configureStr)
{
    sstr positionCommand = std::string(commandPosition,' ');
    sstr outFileName = "pre_make_results.txt";
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Pre make commands -- usually configure, but not always...");
    vec.emplace_back("# Piping results to '" + itemValues.bldPath + "'.");

    configureStr.append(positionCommand + "> '" + itemValues.bldPath + outFileName + "' 2>&1 ");
    vec.emplace_back(configureStr);
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    vec.clear();
    if (result == 0) {
        vec.emplace_back("# Pre make commands completed successfully.");
    } else {
        vec.emplace_back("# Pre make commands had some problems.");
        vec.emplace_back("# Look through '" + itemValues.bldPath + outFileName + "' to find the issue. ");
    }
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

int make_clean(an_itemValues& itemValues)
{
    int result = 0;
    sstr positionCommand = std::string(commandPosition,' ');
    sstr clnFileName = "make_clean_results.txt";
    sstr mkeFileName = "make_results.txt";

    std::vector<sstr> vec;
    if ((itemValues.ProperName    != "Perl")
        && (itemValues.ProperName != "Perl6")
        && (itemValues.ProperName != "Libzip")
        && (itemValues.ProperName != "Cmake"))
    {
        vec.emplace_back("# ");
        vec.emplace_back("# Make clean");
        vec.emplace_back(          "eval \"    cd '" + itemValues.srcPathPNV + "';\n"
                                   + positionCommand + "make clean > '" + itemValues.bldPath + clnFileName + "' 2>&1 \"");
    }
    result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    vec.clear();
    if (result == 0) {
        vec.emplace_back("# Make clean commands completed successfully.");
    } else {
        vec.emplace_back("# Make clean commands NOT completed successfully.");
    }
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

int make(an_itemValues& itemValues)
{
    sstr command;
    sstr positionCommand = std::string(commandPosition,' ');
    sstr clnFileName = "make_clean_results.txt";
    sstr mkeFileName = "make_results.txt";

    std::vector<sstr> vec;
    vec.emplace_back("# ");

    int result = 0;
    if (
            (itemValues.ProperName == "Cmake")
            && (
                    (itemValues.thisOSType == OS_type::RedHat)
                    || (itemValues.thisOSType == OS_type::Fedora)
                    || (itemValues.thisOSType == OS_type::CentOS)
            )
            )
    {
        command = "gmake";
    }

    command = "make";
    sstr printCommand = getProperNameFromString(command);
    vec.emplace_back("# " + printCommand);
    vec.emplace_back("eval \"cd '" + itemValues.srcPathPNV + "';\n"
                     + positionCommand + command + "  > '" + itemValues.bldPath + mkeFileName + "' 2>&1 \"");
    result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    vec.clear();
    if (result == 0) {
        vec.emplace_back("# " + printCommand + " completed successfully.");
    } else {
        vec.emplace_back("# " + printCommand + " NOT completed successfully.");
        vec.emplace_back("# Look through '" + itemValues.bldPath + mkeFileName + "' to find the issue. ");
    }
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    if (itemValues.ProperName == "Judy")
    {
        command = "make check";
        sstr printCommand = getProperNameFromString(command);
        vec.emplace_back("# " + printCommand);
        vec.emplace_back("eval \"cd '" + itemValues.srcPathPNV + "';\n"
                         + positionCommand + command + "  > '" + itemValues.bldPath + mkeFileName + "' 2>&1 \"");
        result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        vec.clear();
        if (result == 0) {
            vec.emplace_back("# " + printCommand + " completed successfully.");
        } else {
            vec.emplace_back("# " + printCommand + " NOT completed successfully.");
            vec.emplace_back("# Look through '" + itemValues.bldPath + mkeFileName + "' to find the issue. ");
        }
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }

    if (itemValues.ProperName == "Judy")
    {
        command = "make install";
        sstr printCommand = getProperNameFromString(command);
        vec.emplace_back("# " + printCommand);
        vec.emplace_back("eval \"cd '" + itemValues.srcPathPNV + "';\n"
                         + positionCommand + command + "  > '" + itemValues.bldPath + mkeFileName + "' 2>&1 \"");
        result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        vec.clear();
        if (result == 0) {
            vec.emplace_back("# " + printCommand + " completed successfully.");
        } else {
            vec.emplace_back("# " + printCommand + " NOT completed successfully.");
            vec.emplace_back("# Look through '" + itemValues.bldPath + mkeFileName + "' to find the issue. ");
        }
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }

    return result;
}

int make_tests(an_itemValues& itemValues)
{
    int result = 0;
    bool cont = true;
    sstr positionCommand = std::string(commandPosition,' ');
    std::vector<sstr> vec;
    if (itemValues.bDoTests) {
        if (itemValues.ProperName == "Php")   {
            result += test_php(itemValues);
            cont = false;
        }
        if (itemValues.ProperName == "Perl6") {
            test_perl6(itemValues);
            cont = false;
        }
        if (itemValues.ProperName == "Python") {
            test_python(itemValues);
            cont = false;
        }

        if (cont) {
            sstr testPathAndFileName = itemValues.bldPath;
            sstr suffix = "test_results.txt";
            testPathAndFileName = joinPathWithFile(testPathAndFileName, suffix);

            vec.emplace_back("# ");
            vec.emplace_back("# Make test(s)...");
            vec.emplace_back("# !!! Warning this may take a while...");
            vec.emplace_back(
                    "eval \"cd    '" + itemValues.srcPathPNV + "';\n"
                    + positionCommand + "make test > '" + testPathAndFileName + "' 2>&1 \"");

            //Most tests have some failures,
            //  so we don't want to fail the install because of a test failure.
            //  so we don't record the result here.
            do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        }

        // so technically we never fail the tests
        vec.clear();
        vec.emplace_back("# Make test(s) have completed");
        vec.emplace_back("# See results in " + itemValues.bldPath +".");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    return 0;
}

int make_install(an_itemValues& itemValues)
{
    sstr positionCommand = std::string(commandPosition,' ');
    std::vector<sstr> vec;
    sstr makePathAndFileName = itemValues.bldPath;
    sstr suffix = "make_install_results.txt";
    makePathAndFileName = joinPathWithFile(makePathAndFileName, suffix);
    vec.emplace_back("# ");
    vec.emplace_back("# ProperName....." + itemValues.ProperName);
    vec.emplace_back("# Version........" + itemValues.version);
    vec.emplace_back("# Make install...");
    vec.emplace_back("eval \"      cd '" + itemValues.srcPathPNV + "';\n"
                     + positionCommand + "make install > '" + makePathAndFileName + "' 2>&1 \"");

    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (result == 0) {
        vec.clear();
        vec.emplace_back("# Make install completed successfully.");
    } else {
        vec.clear();
        vec.emplace_back("# Make install had some problems.");
        vec.emplace_back("# Look through '" + itemValues.bldPath + "make_install_results.txt' to find the issue. ");
    }
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

int decrementResultIfTesting(an_itemValues& itemValues, int in_result)
{
    int result = in_result;
    if (itemValues.bDebug)
    {
        result-=1;
    }
    return result;
}

int do_configure(an_itemValues& itemValues, sstr& configureStr)
{
    int result = 0;
    if (configureStr.length() > 0) {
        if (    (!itemValues.bDebug) ||
                ((itemValues.bDebug) && (itemValues.debugLevel > 0)))
        {
            result = configure(itemValues, configureStr);
            if (itemValues.debugLevel == 1) {
                result = decrementResultIfTesting(itemValues, result);
            }
        }
    }
    return result;
}

int do_make_clean(an_itemValues& itemValues)
{
    int result = 0;
    if (    (!itemValues.bDebug) ||
            ((itemValues.bDebug) && (itemValues.debugLevel > 1)))
    {
        result = make_clean(itemValues);
        if (itemValues.debugLevel == 2) {
            result = decrementResultIfTesting(itemValues, result);
        }
    }
    return result;
}

int do_make(an_itemValues& itemValues)
{
    int result = 0;
    if (    (!itemValues.bDebug) ||
            ((itemValues.bDebug) && (itemValues.debugLevel > 2)))
    {
        result = make(itemValues);
        if (itemValues.debugLevel == 3) {
            result = decrementResultIfTesting(itemValues, result);
        }
    }
    return result;
}

int do_make_tests(an_itemValues& itemValues)
{
    int result = 0;
    if (    (!itemValues.bDebug) ||
            ((itemValues.bDebug) && (itemValues.debugLevel > 3)))
    {
        if (itemValues.ProperName != "Mariadb")
        {
            result = make_tests(itemValues);
            if (itemValues.debugLevel == 4) {
                result = decrementResultIfTesting(itemValues, result);
            }
        }
    }
    result = 0;
    return result;
}

int do_make_install(an_itemValues& itemValues, int results)
{
    if (    (!itemValues.bDebug) ||
            ((itemValues.bDebug) && (itemValues.debugLevel > 4))) {
        if (results == 0) {
            results = make_install(itemValues);
        }
        if ((results == 0) && (itemValues.ProperName == "Apache")) {
            results += apache_notes(itemValues);
        }
        if ((results == 0) && (itemValues.ProperName == "Mariadb")) {
            results += mariadb_notes(itemValues);
        }
        if (itemValues.debugLevel == 5) {
            results = decrementResultIfTesting(itemValues, results);
        }
    }
    return results;
}

int do_post_install(std::map<sstr, sstr>& settings, an_itemValues& itemValues, int results)
{
    int newResults = 0;
    if (    (!itemValues.bDebug) ||
            ((itemValues.bDebug) && (itemValues.debugLevel > 5))) {
        if (results == 0)
        {
            if (itemValues.ProperName == "Php")
            {
                newResults = postInstall_PHP(settings, itemValues);
            }
            if (itemValues.ProperName == "Apache")
            {
                newResults = postInstall_Apache(itemValues);
            }
            if (itemValues.ProperName == "MariaDB")
            {
                newResults = postInstall_MariaDB(settings, itemValues);
            }
        }
        if (itemValues.debugLevel == 6) {
            newResults = decrementResultIfTesting(itemValues, results);
        }
    }
    return newResults;
}

int basicInstall(an_itemValues& itemValues, sstr& configureStr)
{
    int result = 0;
    std::vector<sstr> vec;

    if (    (!itemValues.bDebug) ||
            ((itemValues.bDebug) && (itemValues.debugLevel > -1))) {
        if (itemValues.debugLevel == 0){
            result = -1;
        }
        if (result == 0) {
            result = do_configure(itemValues, configureStr);
        }
        if (result == 0) {
            if (itemValues.ProperName != "Judy")
            {
                result += do_make_clean(itemValues);
            }
        }
        if (result == 0) {
            result += do_make(itemValues);
        }
        if (result == 0) {
            result += do_make_tests(itemValues);
        }
        if (result == 0) {
            result += do_make_install(itemValues, result);
        }
    }
    if (result == 0 )
    {
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# Summary Status");
        vec.emplace_back("# " + itemValues.ProperName + " install was successful.");
    }
    else
    {
        vec.clear();
        if (itemValues.bDebug)
        {
            sstr temp1 = "# Install for " + itemValues.ProperName + " blocked because Debug_Only  = True";
            sstr temp2 = "# Install for " + itemValues.ProperName + " blocked because Debug_Level = " + std::to_string(itemValues.debugLevel);
            vec.emplace_back(temp1);
            vec.emplace_back(temp2);
        }
        else
        {
            vec.emplace_back("# " + itemValues.ProperName + " Install has some issues.");
            vec.emplace_back("# Look through the build logs in the '" + itemValues.bldPath + "' directory.");
        }
    }
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

int basicInstall_tcl(an_itemValues& itemValues, sstr& configureStr)
{
    int result = -1;
    bool skipTests = false;

    sstr tmpPath = "usr/Tcl_Tk";
    sstr usrPath = joinPathParts(itemValues.rtnPath, tmpPath);

    std::vector<sstr> vec1;
    std::vector<sstr> vec2;

    if (!itemValues.bDebug) {

        vec1.clear();
        vec1.emplace_back("# ");
        vec1.emplace_back("# Configure...");
        vec1.emplace_back("# Piping results to the \"'" + itemValues.bldPath + "'\" directory.");

        configureStr.append(" > '" + itemValues.bldPath + "configure_results.txt' 2>&1 ");
        vec1.emplace_back(configureStr);
        vec1.emplace_back("# ");
        vec1.emplace_back("# make");

        sstr makePathAndFileName = itemValues.bldPath;
        sstr suffix = "make_results.txt";
        makePathAndFileName = joinPathWithFile(makePathAndFileName, suffix);
        vec1.emplace_back("eval \"cd '" + itemValues.srcPathInstallOS + "'; make -j  > '" + makePathAndFileName + "' 2>&1 \"");
        result = do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);

        if (itemValues.bDoTests) {

            if (!skipTests) {
                sstr testPathAndFileName = itemValues.bldPath;
                suffix = "test_results.txt";
                testPathAndFileName = joinPathWithFile(testPathAndFileName, suffix);

                vec2.emplace_back("# ");
                vec2.emplace_back("# make test");
                vec2.emplace_back("# !!! Warning this may take a while...");
                vec2.emplace_back("eval \"cd '" + itemValues.srcPathInstallOS + "'; make test > '" + testPathAndFileName + "' 2>&1 \"");
                do_command(itemValues.fileName_Build, vec2, itemValues.bScriptOnly);
            }
        }
        vec1.clear();
        vec1.emplace_back("# ");
        vec1.emplace_back("# make install");
        vec1.emplace_back("eval \"cd '" + itemValues.srcPathInstallOS + "'; make install > '" + itemValues.bldPath + "install_results.txt' 2>&1 \"");
        vec1.emplace_back("# ");
        result += do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);

        if (itemValues.programName == "tcl") {
            vec2.clear();
            vec2.emplace_back("# ");
            vec2.emplace_back("# ");
            vec2.emplace_back("#################################################################################");
            vec2.emplace_back("# ");
            vec2.emplace_back("# Tcl/Tk Note Section");
            vec2.emplace_back("# ");
            vec2.emplace_back("#################################################################################");
            vec2.emplace_back("# ");
            vec2.emplace_back("# ");
            vec2.emplace_back("# To use tcl cd to this directory: ");

            sstr command = "cd '";
            command.append(usrPath);
            command.append("bin/'");
            vec2.emplace_back(command);
            vec2.emplace_back("ls -al");
            command.clear();
            command = "# Then run tcl with ./tcl [tab complete] then [enter]";
            vec2.emplace_back(command);
            vec2.emplace_back("# At this point you should see a prompt like % ");
            vec2.emplace_back("# type after prompt--> % info tclversion ");
            vec2.emplace_back("#   and you should see the version you installed. ");
            vec2.emplace_back("#   add the path to your PATH variable to run this version from any directory.");
            vec2.emplace_back("# type after prompt--> % wish ");
            vec2.emplace_back("#   and you should see a small new window open up. ");
            vec2.emplace_back("#   depending on how you installed this you may have change permissions.");
            vec2.emplace_back("#   chmod o+rx in the directory tree as appropriate.");
            vec2.emplace_back("#  Some reference material ");
            vec2.emplace_back("#    1. The Tcl and TK programming for the Absolute Beginner by Kurt Wall");
            vec2.emplace_back("#    2. The TCL Programming Language by Ashok P. Nadkarni");
            vec2.emplace_back("#    3. The TCL and the Tk Toolkit by John K. Outsterhout and Ken Jones");
            vec2.emplace_back("#    4. The TCL/Tk A Developers Guide by Clif Flynt");
            vec2.emplace_back("# ");
            file_write_vector_to_file(itemValues.fileName_Notes, vec2, false);
        }
    }
    if (result == 0 )
    {
        vec1.clear();
        vec1.emplace_back("# Install was successful.");
    }
    else
    {
        vec1.clear();
        vec1.emplace_back("# Install had some issues.");
        vec1.emplace_back("# Look through the build logs in the '" + itemValues.bldPath + "' directory.");
    }
    do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
    return result;
}

void set_bInstall(an_itemValues &itemValues)
{
    std::vector<sstr> vec;
    sstr protectionFileWithPath = get_ProtectionFileWithPath(itemValues);

    bool bProtected = false;

    vec.emplace_back("#");
    vec.emplace_back("# Check for " + itemValues.ProperName + " protection file.");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (isFileSizeGtZero(itemValues, protectionFileWithPath, true)) {
        bProtected = true;
        howToRemoveFileProtection(itemValues);
    }
    if ((bProtected) && (directoryExists(itemValues.usrPath)))
    {
        bProtected = true;
        vec.emplace_back("# The " + itemValues.ProperName + " usr directory (" + itemValues.usrPath + ") exists already.");
        vec.emplace_back("# This will prevent the installation out of caution. ");
        vec.emplace_back("# Remove this directory to install this program.");
        vec.emplace_back("# you can use the command below to do it: (without the '#')");
        vec.emplace_back("# rm -rf '" + itemValues.usrPath + "'");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    if (bProtected)
    {
        itemValues.bProtectMode = true;
        itemValues.bInstall = false;
    }
}

sstr get_InstallOS(OS_type thisOS_Type)
{
    // from above...
    // enum class OS_type
    // { Selection_Not_Available = -1,
    //   No_Selection_Made = 0,
    //   CentOS = 1,
    //   Fedora = 2,
    //   Linux_Mint = 3,
    //   RedHat = 4,
    //   MaxOSX = 5};
    sstr installOS = "unknown";
    if (thisOS_Type == OS_type::CentOS)     { installOS = "unix";  }
    if (thisOS_Type == OS_type::Fedora)     { installOS = "unix";  }
    if (thisOS_Type == OS_type::RedHat)     { installOS = "unix";  }
    if (thisOS_Type == OS_type::Linux_Mint) { installOS = "unix";  }
    if (thisOS_Type == OS_type::MaxOSX)     { installOS = "macosx";  }

    return  installOS;
}

int process_section(std::map<sstr, sstr>& settings,
                    int (*pfunc)(std::map<sstr, sstr>& settings, an_itemValues& itemValues),
                    an_itemValues& itemValues )
{
    int result = -1;
    sstr searchStr = lowerCaseString(itemValues.programName);
    itemValues.programName = searchStr;

    if (itemValues.programName != "dependencies") {
        searchStr = itemValues.programNameVersion;
    } else {
        searchStr = "Dependencies";
    }

    bool sectionLoaded = prior_Results(itemValues.fileName_Results, searchStr);
    if (!sectionLoaded)
    {
        appendNewLogSection(itemValues.fileName_Build);
        logFinalSettings(itemValues.fileName_Build, settings, itemValues.programName);
        result = pfunc(settings, itemValues);
        reportResults( itemValues, result);
    }
    else
    {
        section_already_loaded(itemValues.programName, itemValues.version);
    }
    return result;
}

bool set_settings(std::map<sstr,sstr>& settings, an_itemValues& itemValues )
{
    // In order for future knowledge of using if statements with these variables
    //    I am providing these guaranties
    // programName --> will be guaranteed to be: lowercase
    // ProperName  --> will be guaranteed to be: the first letter is capital, all others lowercase
    // Except for Judy
    if (itemValues.programName != "Judy")
    {
        itemValues.programName = lowerCaseString(itemValues.programName);
        itemValues.ProperName = getProperNameFromString(itemValues.programName);
    }
    else
    {
        itemValues.programName = getProperNameFromString(itemValues.programName);
        itemValues.ProperName = getProperNameFromString(itemValues.programName);
    }
    //
    //

    itemValues.fileName_Protection = getProtectedFileName(itemValues.programName);

    sstr skip  = settings[itemValues.programName + "->Skip"];
    if (itemValues.programName == "perl")
    {
        skip  = settings[itemValues.programName + "5->Skip"];
    }
    bool bSkip = getBoolFromString(skip);
    if (!bSkip) {
        itemValues.bSkip = bSkip;

        sstr scriptOnly = "";
        sstr doTests = "";
        sstr debugOnly = "";
        sstr thisOS = "";
        sstr version = "";


        scriptOnly = settings[itemValues.programName + "->Script_Only"];
        doTests = settings[itemValues.programName + "->Do_Tests"];
        debugOnly = settings[itemValues.programName + "->Debug_Only"];

        sstr stgPath = joinPathParts(itemValues.cpyStgPath, itemValues.programName);
        sstr temp = settings[itemValues.programName + "->Debug_Level"];

        // we don't want to throw any exceptions,
        //    so make it safe to convert to int
        sstr debugLevel = getDigitsInStringAsString(temp);
        if (debugLevel.length() == 0) {

            // this means run everything
            itemValues.debugLevel = 7;

        } else {

            // else accept user input
            itemValues.debugLevel = std::stoi(debugLevel);
        }
        itemValues.compression = settings[itemValues.programName + "->Compression"];
        itemValues.version = settings[itemValues.programName + "->Version"];
        itemValues.getPath = settings[itemValues.programName + "->WGET"];
        itemValues.getPath_Extension = settings[itemValues.programName + "->WGET_Extension"];
        itemValues.sha256sum_Config = settings[itemValues.programName + "->Sha256sum"];

        if (itemValues.programName == "perl")
        {
            // We have to override these values because perl  and perl5 are the same thing
            //   But we don't want to confuse it with   perl6 because that is a totally different by similar language
            //   We are adding 5 or 6 to match the perl version
            skip                    = settings[itemValues.programName + "5->Skip"];
            itemValues.compression  = settings[itemValues.programName + "5->Compression"];
            debugOnly               = settings[itemValues.programName + "5->Debug_Only"];
            doTests                 = settings[itemValues.programName + "5->Do_Tests"];
            scriptOnly              = settings[itemValues.programName + "5->Script_Only"];
            itemValues.version      = settings[itemValues.programName + "5->Version"];
            itemValues.getPath      = settings[itemValues.programName + "5->WGET"];
            itemValues.sha256sum_Config = settings[itemValues.programName + "5->Sha256sum"];
        }

        itemValues.bSkip         = getBoolFromString(skip);
        itemValues.bScriptOnly   = getBoolFromString(scriptOnly);
        itemValues.bDoTests      = getBoolFromString(doTests);
        itemValues.bDebug        = getBoolFromString(debugOnly);
        itemValues.bInstall      = true;

        itemValues.programNameVersion = itemValues.programName;
        itemValues.programNameVersion.append("-");
        itemValues.programNameVersion.append(itemValues.version);


        //This section is to re-write the program name
        //     for programNameVersion strings
        if (itemValues.programName == "apache")
        {
            itemValues.programNameVersion = "httpd-";
            itemValues.programNameVersion.append(itemValues.version);
        }

        if (itemValues.programName == "perl6")
        {
            itemValues.programNameVersion = "rakudo-";
            itemValues.programNameVersion.append(itemValues.version);
        }

        if (itemValues.programName == "wxwidgets")
        {
            itemValues.programNameVersion = "wxWidgets-";
            itemValues.programNameVersion.append(itemValues.version);
        }


        if (itemValues.programName == "python")
        {
            itemValues.programNameVersion = "Python-";
            itemValues.programNameVersion.append(itemValues.version);
        }

        if ((itemValues.programName == "tcl") || (itemValues.programName == "tk"))
        {
            itemValues.programNameVersion = itemValues.programName;
            itemValues.programNameVersion.append(itemValues.version);
        }

        itemValues.stgPath             = joinPathParts(itemValues.cpyStgPath,itemValues.programName);

        itemValues.fileName_Compressed =  itemValues.programNameVersion + itemValues.compression;

        if ((itemValues.programName == "tcl") || (itemValues.programName == "tk"))
        {
            itemValues.fileName_Compressed =  itemValues.programNameVersion;
            itemValues.fileName_Compressed.append("-src");
            itemValues.fileName_Compressed.append(itemValues.compression);
        }

        itemValues.sha256sum_FileName = joinPathWithFile(itemValues.stgPath, itemValues.fileName_Compressed);
        //end of section

        itemValues.fileName_Staged     =  joinPathWithFile(itemValues.stgPath, itemValues.fileName_Compressed);

        sstr tmpPath1 = "src";
        sstr tmpPath2 = joinPathParts(itemValues.rtnPath, tmpPath1);
        tmpPath1 = itemValues.programName;
        itemValues.srcPath = joinPathParts(tmpPath2, tmpPath1);

        tmpPath1 = "xxx";
        sstr xxxPath = joinPathParts(itemValues.rtnPath, tmpPath1);
        itemValues.bldPath = get_xxx_Path(xxxPath, "bld");
        itemValues.bldPath.append(itemValues.programName);
        itemValues.bldPath.append("/");
        itemValues.etcPath = get_xxx_Path(xxxPath, "etc");
        itemValues.etcPath.append(itemValues.programName);
        itemValues.etcPath.append("/");

        itemValues.usrPath = get_xxx_Path(xxxPath, "usr");
        itemValues.usrPath.append(itemValues.programName);
        itemValues.usrPath.append("/");
    }
    return bSkip;
}



#endif //J5C_INSTALL_FROM_SOURCE_001_COMMANDS_H
