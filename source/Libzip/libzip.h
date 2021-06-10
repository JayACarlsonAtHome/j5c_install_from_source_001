//
// Created by jay on 6/2/21.
//

#ifndef J5C_INSTALL_FROM_SOURCE_001_LIBZIP_H
#define J5C_INSTALL_FROM_SOURCE_001_LIBZIP_H

int install_libzip(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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
        sstr tmpPath = "build";
        itemValues.srcPathPNV = joinPathParts(itemValues.srcPathPNV, tmpPath);

        bool securityCheck = check_Sha256sum(itemValues);
        if (securityCheck)
        {
            result = setupInstallDirectories(itemValues);
            vec.clear();
            vec.emplace_back("#");
            vec.emplace_back("# Special Instructions for libzip");
            vec.emplace_back("eval \"mkdir -p '" + itemValues.srcPathPNV + "' \"");
            vec.emplace_back("eval \"cd       '" + itemValues.srcPathPNV + "';\n"
                             + positionCommand + " '" + itemValues.rtnPath + "usr/cmake/bin/cmake' ..\\\n"
                             + positionCommand + " -DCMAKE_INSTALL_PREFIX='" + itemValues.usrPath + "' \\\n"
                             + positionCommand +"> '" + itemValues.bldPath + "custom_cmake.txt'\"");
            result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

            if (result == 0) {
                sstr configureStr = "# No configuration command required. \\\n";
                result += basicInstall(itemValues, configureStr);
                createProtectionWhenRequired(result, itemValues, false);
            }
            else
            {
                vec.clear();
                vec.emplace_back("#");
                vec.emplace_back("# non-system cmake failed");
                do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
            }
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_LIBZIP_H
