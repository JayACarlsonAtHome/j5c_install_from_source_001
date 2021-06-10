//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_PERL6_H
#define J5C_INSTALL_FROM_SOURCE_001_PERL6_H

int install_perl6(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    // we need this path for running perl5 as part of the configure statement
    itemValues.perl5RunPath = settings["perl5RunPath"];
    if (itemValues.perl5RunPath.length()< 1)
    {
        sstr temp = "usr/perl/bin";
        itemValues.perl5RunPath = joinPathParts(itemValues.rtnPath,temp);
    }

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
            sstr configureStr = "eval \"cd '" + itemValues.srcPathPNV + "'\";\n";
            std::vector<sstr> commands;
            if (itemValues.perl5RunPath.substr(itemValues.perl5RunPath.length()-1,1) == "/")
            {
                itemValues.perl5RunPath = itemValues.perl5RunPath.substr(0,itemValues.perl5RunPath.length()-1);
            }
            commands.emplace_back(positionCommand + "'" + itemValues.perl5RunPath + "/perl' Configure.pl");
            commands.emplace_back(positionCommand + "--backend=moar --gen-moar --prefix='" + itemValues.usrPath + "'");
            configureStr.append(multilineCommand(commands, false));

            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}


int test_perl6(an_itemValues& itemValues)
{
    int result = 0;
    sstr positionCommand = std::string(commandPosition,' ');
    sstr suffix1 = "make_test_results.txt";
    sstr suffix2 = "rakudo_test_results.txt";
    sstr suffix3 = "rakudo_specTest_results.txt";
    sstr suffix4 = "modules_test_results.txt";

    sstr testPathAndFileName1 = joinPathWithFile(itemValues.bldPath, suffix1);
    sstr testPathAndFileName2 = joinPathWithFile(itemValues.bldPath, suffix2);
    sstr testPathAndFileName3 = joinPathWithFile(itemValues.bldPath, suffix3);
    sstr testPathAndFileName4 = joinPathWithFile(itemValues.bldPath, suffix4);

    std::vector<sstr> vec1;
    vec1.clear();
    vec1.emplace_back("# ");
    vec1.emplace_back("# make test...");
    vec1.emplace_back("# !!! Warning this may take a while...");
    vec1.emplace_back("eval \"cd '" + itemValues.srcPathPNV + "';\n"
                      + positionCommand + "make test > '" + testPathAndFileName1 + "' 2>&1 \"");
    do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
    vec1.clear();
    // We are assuming the tests will always fail to some degree,
    //    and we are ok with that, just report conditions normal...
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_PERL6_H
