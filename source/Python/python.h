//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_PYTHON_H
#define J5C_INSTALL_FROM_SOURCE_001_PYTHON_H

int install_python(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr positionCommand = std::string(commandPosition, ' ');
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        ensureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        bool securityCheck = check_Sha256sum(itemValues);
        if (securityCheck)
        {
            result = setupInstallDirectories(itemValues);
            sstr configureStr = "eval \"cd '" + itemValues.srcPathPNV + "' \";\n"
                                + positionCommand + " ./configure --prefix='" + itemValues.usrPath + "' \\\n";
            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}


int test_python(an_itemValues& itemValues)
{
    std::vector<sstr> vec1;
    if (itemValues.ProperName == "Python") {
        // do nothing..
        // until I can get "expect" to read the input and "send" some data
        // I don't want the tests to hold up the script.
        vec1.emplace_back("# ");
        vec1.emplace_back("# I am tired of python tests never finishing,");
        vec1.emplace_back("#  or failing in such a way that the scripts will not complete.");
        vec1.emplace_back("# You can run the tests manually at your own peril.");
        vec1.emplace_back("# And that's all I have to say about that...");
        do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
    }
    return 0;
}

#endif //J5C_INSTALL_FROM_SOURCE_001_PYTHON_H
