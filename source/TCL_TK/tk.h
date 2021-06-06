//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_TK_H
#define J5C_INSTALL_FROM_SOURCE_001_TK_H

int install_tk(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    sstr positionCommand = std::string(commandPosition, ' ');
    sstr tclProgramName = "tcl";
    sstr tclVersion     = settings["tcl->Version"];
    sstr tclConfigurePath = itemValues.rtnPath;
    sstr tclSrcPath = "src";
    tclConfigurePath = joinPathParts(tclConfigurePath, tclSrcPath);
    tclConfigurePath = joinPathParts(tclConfigurePath, tclProgramName);
    sstr tclProgramNameVersion = tclProgramName.append(tclVersion);
    tclConfigurePath = joinPathParts(tclConfigurePath, tclProgramNameVersion);
    sstr installOS = get_InstallOS(itemValues.thisOSType);
    tclConfigurePath = joinPathParts(tclConfigurePath, installOS);

    int result = -1;
    sstr command;
    std::vector<sstr> vec;

    set_bInstall(itemValues);
    if (itemValues.bInstall)
    {
        appendNewLogSection(itemValues.fileName_Build);
        ensureStageDirectoryExists(itemValues);
        stageSourceCodeIfNeeded(itemValues);
        itemValues.srcPathPNV       = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);
        itemValues.srcPathInstallOS = joinPathParts(itemValues.srcPathPNV, installOS);

        bool securityCheck = check_Sha256sum(itemValues);
        if (securityCheck)
        {
            result = setupInstallDirectories(itemValues);
            sstr configureStr = "eval \"cd '" + itemValues.srcPathInstallOS + "' \";\n";
            std::vector<sstr> commands;
            commands.emplace_back(positionCommand + "./configure --prefix='" + itemValues.usrPath + "'");
            commands.emplace_back(positionCommand + "  --with-tcl='" + tclConfigurePath + "'");
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


#endif //J5C_INSTALL_FROM_SOURCE_001_TK_H
