//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_WXWIDGETS_H
#define J5C_INSTALL_FROM_SOURCE_001_WXWIDGETS_H

int install_wxwidgets(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr positionCommand = std::string(commandPosition, ' ');
    sstr command;
    sstr subDirectory = "buildud_ns_nm";
    std::vector<sstr> vec;

    set_bInstall(itemValues);

    if (itemValues.bInstall)
    {
        sstr fixedSourcePath = itemValues.programName;
        fixedSourcePath = "wxWidgets-" ;
        fixedSourcePath.append(itemValues.version);

        appendNewLogSection(itemValues.fileName_Build);
        ensureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, fixedSourcePath);
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPathPNV, subDirectory);
        sstr temp = itemValues.srcPathPNV.substr(1, itemValues.srcPathPNV.length()-2);

        bool securityCheck = check_Sha256sum(itemValues);
        if (securityCheck)
        {
            result = setupInstallDirectories(itemValues);
            ensure_Directory_exists1(itemValues.srcPathPNV);
            //It is recommended to have subdirectories for each type of configuration
            //  you create so we are going to use buildud_ns_nm as our directory
            //  which stands for build unicode, debug, not shared, not monolithic

            std::vector<sstr> commands;
            sstr configureStr = "eval \"cd '" + itemValues.srcPathPNV +  "'\" \n";
            commands.emplace_back(positionCommand + "../configure");
            commands.emplace_back(positionCommand + "  --enable-unicode");
            commands.emplace_back(positionCommand + "  --enable-debug");
            commands.emplace_back(positionCommand + "  --disable-shared");
            commands.emplace_back(positionCommand + "  --disable-monolithic");
            configureStr.append(multilineCommand(commands, false));
            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);

        } else {
            result = badSha256sum(itemValues);
        }

    }
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_WXWIDGETS_H
