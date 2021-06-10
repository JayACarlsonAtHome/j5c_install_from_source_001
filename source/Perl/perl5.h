//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_PERL5_H
#define J5C_INSTALL_FROM_SOURCE_001_PERL5_H

int install_perl5(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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
            if (result == 0) {
                sstr configureStr   = "eval \"cd   '" + itemValues.srcPathPNV + "'\";\n "
                                      + positionCommand + "./Configure  -Dprefix='" + itemValues.usrPath + "' -d -e \\\n ";
                result += basicInstall(itemValues, configureStr);
            }
            if (result == 0) {
                createProtectionWhenRequired(result, itemValues, false);
            }
        } else {
            result = badSha256sum(itemValues);
        }
    }
    settings["perl5RunPath"] = itemValues.usrPath + "bin";
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_PERL5_H
