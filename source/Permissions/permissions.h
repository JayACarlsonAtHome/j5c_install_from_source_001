//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_PERMISSIONS_H
#define J5C_INSTALL_FROM_SOURCE_001_PERMISSIONS_H

int ensure_GroupExists(an_itemValues itemValues, sstr groupName)
{
    int result = 0;
    std::vector<sstr> vec;
    sstr groupFileName = "/etc/group";
    sstr groupTestFile = itemValues.srcPath + groupName + "-group-test.txt";

    // So to find out if a user exists already we are going to do a
    // little hacking... cat /etc/group | grep groupName will give us one line
    // if the groupName exists. So we pipe the results to a create/truncate
    // file pipe, then we check to see if the file size is greater than 0.

    // Group must exist before adding the user to the group
    // ...so we must do the group first
    vec.clear();
    vec.emplace_back("cat " + groupFileName + "  | grep " + groupName + " > '" + groupTestFile + "'");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    bool GroupExists = isFileSizeGtZero(itemValues, groupTestFile, false);
    vec.clear();
    if (!GroupExists) {
        vec.emplace_back("# Adding " + groupName + " group");
        vec.emplace_back("groupadd " + groupName);
    } else {
        vec.emplace_back("# " + groupName + " group already exists.");
    }
    result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    // we should remove the groupTestFile now...
    vec.clear();
    vec.emplace_back("rm -f '" + groupTestFile + "'");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

int ensure_UserExists(an_itemValues itemValues, sstr groupName, sstr userName)
{
    int result = 0;
    std::vector<sstr> vec;
    sstr userFileName = "/etc/passwd";
    sstr user_TestFile = itemValues.srcPath + "users.txt";

    // So to find out if a user exists already we are going to do a
    // little hacking... cat /etc/passwd | grep apache_ws will give us one line
    // if the user exists. So we pipe the results to a create/truncate
    // file pipe, then we check to see if the file size is greater than 0.

    vec.clear();
    vec.emplace_back("cat " + userFileName + " | grep  " + userName + "  > '" + user_TestFile + "'" );
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    bool userExists = isFileSizeGtZero(itemValues, user_TestFile, false);
    vec.clear();
    if (!userExists)
    {
        vec.emplace_back("# Adding " + userName + " user");
        vec.emplace_back("useradd  -g " + groupName + " --no-create-home --system " + userName);
    }
    else
    {
        vec.emplace_back("# " + userName + " user already exists.");
    }
    result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    // we should remove the user_TestFile now...
    vec.clear();
    vec.emplace_back("rm -f '" + user_TestFile + "'");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

int ensureApacheUserAndGroupExists(an_itemValues& itemValues)
{
    int result = 0;
    std::vector<sstr> vec;
    vec.emplace_back("#");
    vec.emplace_back("# Check for required User and Group");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    result += ensure_GroupExists(itemValues, APACHE_GROUP);
    result += ensure_UserExists( itemValues, APACHE_GROUP, APACHE_USER);
    return result;
}

int ensureMariaDB_UserAndGroupExist(an_itemValues& itemValues)
{
    int result = 0;
    std::vector<sstr> vec;
    vec.emplace_back("#");
    vec.emplace_back("# Check for required User and Group");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    result += ensure_GroupExists(itemValues, MARIADB_GROUP);
    result += ensure_UserExists( itemValues, MARIADB_GROUP, MARIADB_USER);
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_PERMISSIONS_H
