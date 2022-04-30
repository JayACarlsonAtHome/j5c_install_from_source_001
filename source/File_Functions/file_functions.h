//
// Created by jay on 6/2/21.
//

#include <filesystem>
#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_FILE_FUNCTIONS_H
#define J5C_INSTALL_FROM_SOURCE_001_FILE_FUNCTIONS_H

sstr safeFilePath(sstr& path)
{
    sstr resultPath  = path;
    auto startPos1 = resultPath.find(' ');
    auto startPos2 = resultPath.find('\\');
    bool cont = true;
    int failSafe = 0;
    if (startPos1 == -1) { cont = false; }
    while (cont)
    {
        if (startPos1 != -1)
        {
            if (startPos2 + 1 == startPos1)
            {
                cont = false;
            }
            else
            {
                resultPath.replace(startPos1, 1, "\\ ");
                startPos1+=2;
                startPos1 = resultPath.find(' ',  startPos1);
                startPos2 = resultPath.find('\\', startPos1);
            }
        }
        failSafe+=1;

        // this should be adequate for most situations
        // it would probably be safe to reduce this
        // number to 100
        if (failSafe > 999) break;
    }
    return resultPath;
}

sstr unDelimitPath(sstr& path)
{
    sstr newPath = removeCharFromStartOfString(path, '\'');
    newPath = removeCharFromEnd__OfString(newPath, '\'');
    return newPath;
}

sstr delimitPath(sstr& path)
{
    sstr temp;
    temp = removeCharFromStartOfString(path, '\'');
    temp = removeCharFromEnd__OfString(temp, '\'');
    sstr newPath  = "'";
    newPath.append(temp);
    newPath.append("'");
    return newPath;
}

sstr joinPathParts(sstr& partA, sstr& partB)
{
    sstr fixed1, fixed2, path;
    fixed1 = removeCharFromStartOfString(partA,  '/');
    fixed1 = removeCharFromEnd__OfString(fixed1, '/');
    fixed2 = removeCharFromStartOfString(partB,  '/');
    fixed2 = removeCharFromEnd__OfString(fixed2, '/');
    return path = "/" + fixed1 + "/" + fixed2 +  "/";
}

sstr joinPathWithFile(sstr& partA, sstr& fileName)
{
    sstr fixed1, fixed2, path;
    fixed1 = removeCharFromStartOfString(partA,    '/');
    fixed1 = removeCharFromEnd__OfString(fixed1,   '/');
    fixed2 = removeCharFromStartOfString(fileName, '/');
    fixed2 = removeCharFromEnd__OfString(fixed2,   '/');
    return path = "/" + fixed1 + "/" + fixed2;
}

int ensure_Directory_main(sstr& fullCommand, int result, bool show)
{
    std::cout << "  " << fullCommand << std::endl;
    result = system(fullCommand.c_str());
    if (show) {
        if (result == 0) {
            std::cout << "  ->success on running command:" << std::endl;
        } else {
            std::cout << "  ->failure on running command:" << std::endl;
        }
    }
    return result;
}

int ensure_Directory_exists1(sstr& path)
{
    int result = 1;
    sstr command  = "mkdir -p ";
    sstr fullCommand = command + path;
    result = ensure_Directory_main(fullCommand, result, true);
    return result;
}

int ensure_Directory_exists2(sstr& basePath, sstr& path)
{
    int result = 1;
    sstr full_path = basePath + path;
    if (basePath.substr(basePath.length()-1, 1) != "/")
    {
        sstr fullPath = basePath + "/" + path;
    }
    sstr command  = "mkdir -p ";
    sstr fullCommand = command + full_path;
    ensure_Directory_main(fullCommand, result);
    return result;
}

bool directoryExists(const sstr& path)
{
    bool result = false;
    sstr command = "cd " + path;
    command.append(" 2> /dev/null ");
    int commandResult = system(command.c_str());
    if (commandResult == 0) result = true;
    return result;
}

int startNewLogSection(std::ofstream& file, sstr utc )
{
    int result = 1;  // assume failure
    if ( (file.is_open()) && (file.good()) )
    {
        J5C_DSL_Code::j5c_Date thisDate{};
        file << std::endl << std::endl;
        file << " Status started on " << thisDate.strDate() << "  UTC = " << utc <<  std::endl;
        file << " " << std::endl;
        file << " Start    : Stop     : Duration         : Command / Comments" << std::endl;
        file << " HH:MM:SS : HH:MM:SS : YYY:DDD:HH:MM:SS :" << std::endl;
        file << " ===============================================================================================================================" << std::endl;
        result = 0; // success
    }
    else
    {
        std::cout << "!!!Error -- unable to create new log section -- " << std::endl;
    }
    return result;
}

int appendNewLogSection(sstr &fileName)
{
    J5C_DSL_Code::j5c_Date thisDate{};
    std::ofstream file;
    int result = 0;
    file.open(fileName, std::ios::out | std::ios::app);
    if ((file.is_open()) && (file.good())) {
        sstr gmtOffset = get_GmtOffset();
        startNewLogSection(file, gmtOffset);
    }
    file.close();
    return result;
}

int create_file(const sstr& fileName)
{
    J5C_DSL_Code::j5c_Date thisDate{};
    std::ofstream file;
    int result = 0;
    file.open(fileName, std::ios::out | std::ios::trunc );
    if ( (file.is_open()) && (file.good()) )
    {
        file << "# File Created on " << thisDate.strDate() << std::endl;
        file << "# " << std::endl;
        file << "# Copyright J5C Marketing LLC" << std::endl;
        file << "# Jay A Carlson" << std::endl;
        file << "# jay.a.carlson@gmail.com" << std::endl;
        file << "# 360-649-6218 -- Unless you email me first, I probably will just think your a spam caller." << std::endl;
        file << "# " << std::endl;
        file.close();
    }
    else
    {
        std::cout << "!!!Error -- unable to create file -- " << std::endl;
        result = 1;
    }

    sstr c1 = "eval \"uname -a >> ";
    sstr c2 = c1 + fileName + "\"";

    system(c2.c_str() );

    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        file << "# " << std::endl;
        file << "# " << std::endl;
        result += startNewLogSection(file);
    }
    else
    {
        std::cout << "!!!Error -- unable to append to file -- " << std::endl;
        result = 1;
    }

    file.close();
    return result;
}

int ensure_file(sstr &fileName)
{
    std::ofstream file;
    int result = 1;  // assume failure
    file.open(fileName, std::ios::out | std::ios::app);
    if (!(file.is_open()))
    {
        create_file(fileName);
        result = 0;
    }
    return result;
}

int write_file_entry(std::ofstream& file, const sstr& entry, time_t stop, time_t start, bool includeTime)
{
    int result = 0;
    if (includeTime)
    {
        sstr strStart = get_Time_as_String(start);
        sstr strStop  = get_Time_as_String(stop);
        sstr strDuration = getDuration(stop, start);
        file << " " << strStart << ": " << strStop << ": " << strDuration << ": " << entry << std::endl;
    }
    else
    {
        file << entry << std::endl;
    }
    return result;
}

int file_write_vector_to_file(sstr &fileName, std::vector <sstr> &vec_lines, bool includeTime)
{
    std::ofstream file;
    int result = 0;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        for (const auto& it: vec_lines)
        {
            time_t start = get_Time();
            time_t stop  = get_Time();

            if (it == "\n")
            {
                result += write_file_entry(file, "\n", stop, start, includeTime);
            }
            else
            {
                result += write_file_entry(file, it, stop, start, includeTime);
            }
        }
    }
    else
    {
        std::cout << "!!!Error -- unable to write vector contents to file -- " << std::endl;
    }
    file.close();
    return result;
}

int file_append_line(sstr &fileName, sstr &line, time_t stop, time_t start)
{
    std::ofstream file;
    int result = 1;
    bool withTime = true;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        result = write_file_entry(file, line, stop, start, withTime);
    }
    else
    {
        std::cout << "!!!Error -- unable to append line to file -- " << std::endl;
    }
    file.close();
    return result;
}

int file_append_results(sstr& fileName, an_itemValues& itemValues, int installResult, time_t stop)
{
    unsigned long width1 = 31;
    unsigned long width2 = 40;
    sstr fill1(width1,'.');
    sstr fill2(width2,'.');
    sstr ProperName = getProperNameFromString(itemValues.programName);
    std::ofstream file;
    sstr line;
    int result = 1;
    bool found = false;
    file.open(fileName, std::ios::out | std::ios::app );
    if ( (file.is_open()) && (file.good()) )
    {
        if (itemValues.programName.substr(0,12) == "Dependencies")
        {
            line = "Install " + itemValues.ProperName + fill1;
        }
        else
        {
            line = "Install " + itemValues.ProperName + "-" + itemValues.version + fill1;
        }


        if (itemValues.step < 0)
        {
            found = true;
        }

        if ((!found) && (itemValues.step < 10))
        {
            line = line.substr(0,width1) +  "step 0"  + std::to_string(itemValues.step) + fill2;
            found = true;
        }
        if ((!found) && (itemValues.step > 9))
        {
            line = line.substr(0,width1) +  "step "  + std::to_string(itemValues.step) + fill2;
            found = true;
        }
        line = line.substr(0,width2);

        sstr strResults = "......" + std::to_string(installResult);
        strResults = strResults.substr(strResults.length()-5,5);
        line += " : Result = " + strResults;
        if (installResult == -1)
        {
            if (itemValues.bDebug)
            {
                if (itemValues.debugLevel >= 5) {
                    line += " : Results Blocked by Debug.";
                } else {
                    line += " : Install Blocked by Debug.";
                }
            }
            else
            {
                line += " : Install Blocked..........";
            }
        }
        if (installResult == 0)
        {
            line += " : Good.....................";
        }
        if (installResult > 0)
        {
            line += " : What the heck?...........";
        }
        if (found) {
            result = write_file_entry(file, line, stop, itemValues.itemStartTime, true);
        }
    }
    else
    {
        std::cout << "!!!Error -- unable to append results to file -- " << std::endl;
    }
    file.close();
    return result;
}

int file_append_blank_lines(sstr& fileName, int count)
{
    std::ofstream file;
    int result = 1;
    std::vector<sstr> vec;
    file.open(fileName, std::ios::out | std::ios::app );

    if ( (file.is_open()) && (file.good()) )
    {
        while (count > 0)
        {
            count -= 1;
            file << std::endl;
        }
    }
    return result;
}

std::vector<sstr> readFile(sstr &fileName, unsigned long maxCount)
{
    unsigned long theCount = 0;
    std::ifstream file;
    std::vector<sstr> result;
    file.open(fileName, std::ios::in );
    sstr lineData;
    if ( (file.is_open()) && (file.good()) )
    {
        while (getline(file, lineData))
        {
            result.push_back(lineData + "\n");
            theCount += 1;
            if (theCount >= maxCount)
            {
                break;
            }
        }
    }
    else
    {
        std::cout << "!!!Error -- unable to read file -- " << std::endl;
    }
    file.close();
    return result;
}

bool isFileSizeGtZero(an_itemValues itemValues, sstr &fileName, bool bShow)
{
    bool result = false;
    std::vector<sstr> vec;
    sstr positionCommand = std::string(commandPosition, ' ');
    sstr safeFileName = safeFilePath(fileName);
    sstr printCommand = "";
    auto startPos = fileName.find(" ");
    if (startPos == -1) {
        std::ifstream file(safeFileName, std::ios::in | std::ios::binary);
        file.seekg(0, file.end);
        auto worthless = file.tellg();
        auto real_size = static_cast<long long>(worthless);
        file.close();
        if (real_size > 0) { result = true; }
        if (real_size < 0) { real_size = 0; }
        if (bShow) {
            sstr size = std::to_string(real_size);
            if (real_size > 0) {
                vec.clear();
                printCommand = "# Found File: '";
                printCommand.append(fileName);
                printCommand.append("' \n");
                printCommand.append(positionCommand);
                printCommand.append("#   and size= ");
                printCommand.append(size);
                printCommand.append(".");
                vec.emplace_back(printCommand);
                result = true;
            } else {
                vec.clear();
                printCommand = "# Search File: '";
                printCommand.append(fileName);
                printCommand.append("' \n");
                printCommand.append(positionCommand);
                printCommand.append("#      Status= ");
                printCommand.append(" File not found or size = ");
                printCommand.append(size);
                printCommand.append(".");
                vec.emplace_back(printCommand);
            }
            if (itemValues.fileName_Build.length() > 0) {
                do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
            }
        }
    }
    else {
        vec.clear();
        printCommand = "# Search File: '";
        printCommand.append(fileName);
        printCommand.append("' \n");
        printCommand.append(positionCommand);
        printCommand.append("#      Status= ");
        printCommand.append(" is not a valid file name: not found.");
        vec.emplace_back(printCommand);
        std::cout << "File: " << fileName << " is not a valid file name: not found" << std::endl;
    }
    return result;
}

int removeDirectory(sstr& fileName, sstr& path, std::vector<sstr>& vec)
{
    if (path.length() > 2)
    {
        vec.emplace_back("eval \"rm -rf '" + path + "'\"");
    }
    return 0;
}

sstr get_ProtectionFileWithPath(an_itemValues& itemValues)
{
    sstr protectionFileWithPath = itemValues.srcPath;
    protectionFileWithPath = joinPathParts(protectionFileWithPath, itemValues.programNameVersion);
    protectionFileWithPath.append("protection-");
    protectionFileWithPath.append(itemValues.programName);
    protectionFileWithPath.append(".txt");
    return protectionFileWithPath;
}

void protectProgram_IfRequired(an_itemValues& itemValues, bool show)
{
    std::vector<sstr> vec;
    sstr positionCommand = std::string(commandPosition, ' ');
    sstr command = "";
    sstr protectionFileWithPath = get_ProtectionFileWithPath(itemValues);
    if (!(isFileSizeGtZero(itemValues, itemValues.fileName_Protection, show))) {
        if (itemValues.bProtectMode) {
            vec.emplace_back("#");
            command = "# Enable protection for ";
            command.append(itemValues.ProperName);
            vec.emplace_back(command);
            command.clear();
            command = "eval \"echo 'protection = true' \\\n";
            command.append(positionCommand);
            command.append(" > '");
            command.append(protectionFileWithPath);
            command.append("'\"");
            vec.emplace_back(command);
            do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        }
    }
    return;
}

void howToRemoveFileProtection(an_itemValues& itemValues)
{
    sstr positionCommand = std::string(commandPosition, ' ');
    sstr protectionFileWithPath = get_ProtectionFileWithPath(itemValues);
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# This " + itemValues.ProperName + " install is currently protected:");
    vec.emplace_back("# To remove this protection, run this command:");
    vec.emplace_back("# eval \" rm -f '" + protectionFileWithPath + "' \"");
    vec.emplace_back("# ");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return;
}

bool stageSourceCodeIfNeeded(an_itemValues& itemValues)
{
    bool result = false;
    bool special = false;
    sstr fileName = "";
    sstr fileNameTemp = "";
    std::vector<sstr> vec;

    vec.emplace_back("#");
    vec.emplace_back("# Stage source file if needed.");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    vec.clear();
    fileName = itemValues.fileName_Staged;
    if (!(isFileSizeGtZero(itemValues, fileName, true)))
    {
        vec.emplace_back("# Attempting to download file...");
        vec.emplace_back("eval \"cd '" + itemValues.stgPath + "'; wget " + itemValues.getPath +  itemValues.fileName_Compressed + "\"");
    }
    else
    {
        vec.emplace_back("# Source code appears to be staged.");
        result = true;
    }
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

sstr get_xxx_Path(const sstr& xxxPath, const sstr& replacement)
{
    sstr result = "";
    if (replacement.length() == 3) {
        sstr newPath = xxxPath;
        auto start = newPath.rfind("xxx");
        if (start != -1) {
            newPath.replace(start, 3, replacement, 0, 3);
        }
        else
        {
            // this will most likely cause the program
            //   to not do what is desired, but the
            //   rest of the program will handle the
            //   failure instead of crashing the program.
            //   And it won't cause deleting the entire partition.
            newPath = "Err in get_xxx_Path()...xxx was not found.";
        }
        result = newPath;
    }
    else {
        result = "Err in get_xxx_Path()...Replacement path length is != 3.";
    }
    return result;
}

int removeWorkingDirectories(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    removeDirectory(itemValues.fileName_Build, itemValues.bldPath, vec);
    removeDirectory(itemValues.fileName_Build, itemValues.etcPath, vec);
    removeDirectory(itemValues.fileName_Build, itemValues.srcPath, vec);

    if (itemValues.ProperName != "Tk") {
        removeDirectory(itemValues.fileName_Build, itemValues.usrPath, vec);
    }
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Directories were deleted successfully.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Directories were not deleted successfully.");
        vec.emplace_back("# Check for valid path and permissions.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    return result;
}

int ensureWrkDirExist(an_itemValues& itemValues)
{
    int result = -1;
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Ensure " + itemValues.ProperName + " working directories exist. ");
    vec.emplace_back("eval \"mkdir -p '" + itemValues.bldPath + "'\"");
    vec.emplace_back("eval \"mkdir -p '" + itemValues.etcPath + "'\"");
    vec.emplace_back("eval \"mkdir -p '" + itemValues.srcPath + "'\"");
    if (itemValues.programName == "tk") {
        vec.emplace_back("#already exists '" + itemValues.usrPath + "'\"");
    }
    else
    {
        vec.emplace_back("eval \"mkdir -p '" + itemValues.usrPath + "'\"");
    }
    result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Directories exist or were created.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Directories were not created successfully.");
        vec.emplace_back("# Check for valid path and permissions.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    return result;
}

int createTargetFromStage(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Copy, decompress, and remove compressed file.");

    vec.emplace_back(
            "eval \"cd '" + itemValues.stgPath + "';        cp './" + itemValues.fileName_Compressed + "' '" +
            itemValues.srcPath + "'\"");
    vec.emplace_back("eval \"cd '" + itemValues.srcPath + "'; tar xf '" + itemValues.fileName_Compressed + "'\"");
    vec.emplace_back("eval \"cd '" + itemValues.srcPath + "'; rm  -f '" + itemValues.fileName_Compressed + "'\"");

    // Run all the commands we built up
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Source code positioned for compilation.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Source code NOT positioned for compilation.");
        vec.emplace_back("# Check for valid path and permissions.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    }
    return result;
}

int ensureStageDirectoryExists(an_itemValues &itemValues)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Ensure stage directory exists.");
    vec.emplace_back("eval \"mkdir -p '" + itemValues.stgPath + "'\"");
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    if (result == 0)
    {
        vec.clear();
        vec.emplace_back("# Stage directory exists.");
    }
    else
    {
        vec.clear();
        vec.emplace_back("# Stage directory does NOT exists.");
        vec.emplace_back("# Check for valid path, and permissions.");
    }
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

int setupInstallDirectories(an_itemValues& itemValues)
{
    std::vector<sstr> vec;
    vec.emplace_back("# ");
    vec.emplace_back("# Remove unfinished " + itemValues.ProperName + " install (if any).");
    int result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    if ((itemValues.ProperName == "Tcl") || (itemValues.ProperName == "Tk"))
    {
        sstr tmpPath = "usr/Tcl_Tk";
        itemValues.usrPath = joinPathParts(itemValues.rtnPath, tmpPath);
    }
    result += removeWorkingDirectories(itemValues);
    result += ensureWrkDirExist       (itemValues);
    result += createTargetFromStage   (itemValues);
    return result;
}

void createProtectionWhenRequired(int result, an_itemValues& itemValues, bool show)
{
    std::vector<sstr> vec;
    if ((itemValues.bProtectMode) && (result == 0))
    {
        protectProgram_IfRequired(itemValues, show);
        howToRemoveFileProtection(itemValues);
    }
}

sstr getProtectedFileName(sstr& programName)
{
    sstr protectedFileName = "protection";
    protectedFileName.append("-");
    protectedFileName.append(programName);
    protectedFileName.append(".txt");
    return protectedFileName;
}




#endif //J5C_INSTALL_FROM_SOURCE_001_FILE_FUNCTIONS_H
