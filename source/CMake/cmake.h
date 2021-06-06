//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_CMAKE_H
#define J5C_INSTALL_FROM_SOURCE_001_CMAKE_H

int install_cmake(std::map<sstr, sstr>& settings, an_itemValues& itemValues) {
    int result = -1;
    sstr positionCommand = std::string(commandPosition, ' ');
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall) {
        appendNewLogSection(itemValues.fileName_Build);
        ensureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

        bool securityCheck = check_Sha256sum(itemValues);
        if (securityCheck) {

            result = setupInstallDirectories(itemValues);
            // Unlike all the other install_xxx functions this time we have to call configure
            //   two times, so the first time we call it individually with the bootstrap command.
            //   The second time we pass gmake to the basicInstall function
            //   But that happens at a sub function level.

            sstr configureStr = "eval \"cd '" + itemValues.srcPathPNV + "'\"\n";
            std::vector<sstr> commands;
            commands.emplace_back(positionCommand + " ./bootstrap --prefix='" + itemValues.usrPath + "' ");
            configureStr.append(multilineCommand(commands, false));

            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_CMAKE_H
