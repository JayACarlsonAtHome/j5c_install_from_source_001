//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_TCL_H
#define J5C_INSTALL_FROM_SOURCE_001_TCL_H

int install_tcl(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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
        sstr installOS = get_InstallOS(itemValues.thisOSType);
        itemValues.srcPathInstallOS = joinPathParts(itemValues.srcPathPNV, installOS);

        bool securityCheck = check_Sha256sum(itemValues);
        if (securityCheck)
        {
            result = setupInstallDirectories(itemValues);
            sstr configureStr = "eval \"cd '" + itemValues.srcPathInstallOS + "' \"\n";

            std::vector<sstr> commands;
            commands.emplace_back(positionCommand + "./configure --prefix='" + itemValues.usrPath + "'");
            commands.emplace_back(positionCommand + "  --enable-threads");
            commands.emplace_back(positionCommand + "  --enable-shared");
            commands.emplace_back(positionCommand + "  --enable-symbols");
            commands.emplace_back(positionCommand + "  --enable-64bit");
            configureStr.append(multilineCommand(commands,false));
            result += basicInstall_tcl(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_TCL_H
