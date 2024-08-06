//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_RUBY_H
#define J5C_INSTALL_FROM_SOURCE_001_RUBY_H

int install_ruby(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = -1;
    sstr positionCommand = std::string(commandPosition, ' ');
    sstr temp;

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
            temp.clear();
            temp.append(positionCommand);
            temp.append("./configure");
            temp.append("  --prefix='");
            temp.append(itemValues.usrPath);
            temp.append("'");
            commands.emplace_back(temp);
            configureStr.append(multilineCommand(commands, false));
            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}

#endif //J5C_INSTALL_FROM_SOURCE_001_RUBY_H
