//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_PHP_H
#define J5C_INSTALL_FROM_SOURCE_001_PHP_H

sstr create_php_configuration(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    sstr positionCommand = std::string(commandPosition, ' ');
    sstr  compileForDebug   = settings[itemValues.programName + "->Compile_For_Debug"];
    bool bCompileForDebug   = getBoolFromString(compileForDebug);
    sstr sslVersion     = settings["openssl->Version"];
    sstr openSSLVersion = "openssl";
    openSSLVersion.append("-");
    openSSLVersion.append(sslVersion);
    //
    // Note: Don't end the command with \" to close the command here.
    //   We are going to append more to the command in the function
    //     and end the command with \" there.

    sstr tmpPath = "usr/apache/bin";
    sstr apePath = joinPathParts(itemValues.rtnPath, tmpPath);

    sstr tmpFile = "apachectl";
    sstr apacheController  = joinPathWithFile(apePath, tmpFile);

    tmpFile = "mysqld";
    sstr mariadbPath = itemValues.rtnPath + "usr/mariadb/bin";
    sstr mariadbController = joinPathWithFile(mariadbPath, tmpFile);

    std::vector<sstr> commands;
    //sstr configureStr = "";
    sstr configureStr = "eval \"set PKG_CONFIG_PATH /usr/lib64/pkgconfig;\"";
    configureStr.append("\n");

    sstr temp = "";
    temp.append(positionCommand);
    temp.append("cd '");
    temp.append(itemValues.srcPathPNV);
    temp.append("';\n");
    configureStr.append(temp);

    temp.clear();
    temp.append(positionCommand);
    temp.append("./configure");
    temp.append("  --prefix='");
    temp.append(itemValues.usrPath);
    temp.append("'");
    commands.emplace_back(temp);

    temp.clear();
    temp.append(positionCommand);
    temp.append("  --exec-prefix='");
    temp.append(itemValues.usrPath);
    temp.append("'");
    commands.emplace_back(temp);

    temp.clear();
    temp.append(positionCommand);
    temp.append("  --srcdir='");
    temp.append(itemValues.srcPathPNV);
    temp.append("'");
    commands.emplace_back(temp);

    temp.clear();
    temp.append(positionCommand);
    temp.append("  --with-openssl='" + itemValues.rtnPath + "usr/openssl/'");
    commands.emplace_back(temp);

    temp.clear();
    tmpPath = "usr/apache/bin/";
    sstr apxPathFile = joinPathParts(itemValues.rtnPath, tmpPath);
    tmpFile = "apxs";
    apxPathFile = joinPathWithFile(apePath, tmpFile);
    temp.append(positionCommand);
    temp.append("  --with-apxs2='");
    temp.append(apxPathFile);
    temp.append("'");
    commands.emplace_back(temp);

    temp.clear();
    temp.append(positionCommand);
    temp.append("  --enable-mysqlnd ");
    commands.emplace_back(temp);

    temp.clear();
    temp.append(positionCommand);
    temp.append("  --with-config-file-path='");
    tmpPath = "lib";
    sstr libPath = joinPathParts(itemValues.usrPath, tmpPath);
    temp.append(libPath);
    temp.append("'");
    commands.emplace_back(temp);

    temp.clear();
    temp.append(positionCommand);
    temp.append("  --with-config-file-scan-dir='");
    temp.append(itemValues.etcPath);
    sstr crlPath = "/usr/bin";
    temp.append("'");
    commands.emplace_back(temp);

    if (itemValues.thisOSType != OS_type::Fedora)
    {
        //I tried everything I could think of
        // and I couldn't get --with-curl to work on Fedora
        // even though the files were there the ./config command
        // couldn't find them.  I also tried tweaking the permissions
        // on the files, and still nothing.

        //I will probably have to work on this later...

        //The problem is with defaults, where some code is looking in lib and other code is looking in lib64
        //I still have to defer this till later.

        temp.clear();
        temp.append(positionCommand);
        temp.append("  --with-curl='");
        temp.append(crlPath);
        temp.append("'");
        commands.emplace_back(temp);
    }

    temp.clear();
    temp.append(positionCommand);
    temp.append("  --with-mysql-sock='");
    tmpPath = "usr/mariadb/run/";
    sstr sckPathFile = joinPathParts(itemValues.rtnPath, tmpPath);
    tmpFile = "mariadb.socket";
    sckPathFile = joinPathWithFile(sckPathFile, tmpFile);
    temp.append(sckPathFile);
    temp.append("'");
    commands.emplace_back(temp);

    commands.emplace_back(positionCommand + "  --with-pdo-mysql=mysqlnd");
    commands.emplace_back(positionCommand + "  --libdir='/usr/lib64/'");
    commands.emplace_back(positionCommand + "  --disable-cgi");
    commands.emplace_back(positionCommand + "  --disable-short-tags");
    commands.emplace_back(positionCommand + "  --enable-bcmath");
    commands.emplace_back(positionCommand + "  --with-pcre-jit");
    commands.emplace_back(positionCommand + "  --enable-sigchild");
    commands.emplace_back(positionCommand + "  --enable-libgcc");
    commands.emplace_back(positionCommand + "  --enable-calendar");
    commands.emplace_back(positionCommand + "  --enable-dba=shared");
    commands.emplace_back(positionCommand + "  --enable-ftp");
    commands.emplace_back(positionCommand + "  --enable-intl");
    commands.emplace_back(positionCommand + "  --enable-zend-test");

    //commands.emplace_back(positionCommand + "  --enable-mbstring");
    //commands.emplace_back(positionCommand + "  --enable-mbregex");
    //These two commands above require Package 'oniguruma', required by 'virtual:world', not found
    //  Consider adjusting the PKG_CONFIG_PATH environment variable if you
    //  installed software in a non-standard prefix.
    //  This is a total pain...just saying...


    if ( bCompileForDebug ){
        commands.emplace_back(positionCommand + "  --enable-debug");
    }

    configureStr.append(multilineCommand(commands, false));
    return  configureStr;
}


int test_php(an_itemValues& itemValues)
{
    std::vector<sstr> vec1;
    if (itemValues.ProperName == "Php") {
        // do nothing..
        // until I can get "expect" to read the input and "send" some data
        // I don't want the tests to hold up the script.
        vec1.emplace_back("# ");
        vec1.emplace_back("# Make test");
        vec1.emplace_back("# The tests must be run manually.");
        vec1.emplace_back("#   So you can answer the questions at the end of the tests.");
        do_command(itemValues.fileName_Build, vec1, itemValues.bScriptOnly);
    }
    return 0;
}

int install_php(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result    = -1;

    sstr positionCommand = std::string(commandPosition, ' ');

    sstr compileForDebug    = settings[itemValues.programName + "->Compile_For_Debug"];
    sstr xdebug_install     = settings[itemValues.programName + "->Xdebug_Install"];
    sstr xdebug_name        = settings[itemValues.programName + "->Xdebug_Name"];
    sstr xdebug_version     = settings[itemValues.programName + "->Xdebug_Version"];
    sstr xdebug_compression = settings[itemValues.programName + "->Xdebug_Compression"];
    sstr xdebug_wget        = settings[itemValues.programName + "->Xdebug_WGET"];
    sstr xdebug_tar_options = settings[itemValues.programName + "->Xdebug_Tar_Options"];
    sstr zts_version        = settings[itemValues.programName + "->zts_version"];
    bool bCompileForDebug   = getBoolFromString(compileForDebug);
    bool bInstall_Xdebug    = getBoolFromString(xdebug_install);

    sstr command;
    std::vector<sstr> vec;
    appendNewLogSection(itemValues.fileName_Build);

    sstr xDebugProgVersion        =  xdebug_name + "-" + xdebug_version;
    sstr xDebugCompressedFileName =  xDebugProgVersion + xdebug_compression;

    sstr tmpPath = "usr/apache/bin";
    sstr apePath = joinPathParts(itemValues.rtnPath, tmpPath);

    sstr tmpFile = "apachectl";
    sstr apacheController  = joinPathWithFile(apePath, tmpFile);

    tmpFile = "mysqld";
    sstr mariadbPath = itemValues.rtnPath + "usr/mariadb/bin";
    sstr mariadbController = joinPathWithFile(mariadbPath, tmpFile);

    if ((isFileSizeGtZero(itemValues,   apacheController))
        && isFileSizeGtZero(itemValues, mariadbController)) {

        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# Attempt to Start/Restart Apache if possible.");
        vec.emplace_back("#   If you see an error: could not bind to address [::]:80");
        vec.emplace_back("#   It most likely means Apache is already online.");
        // ./apachectl -f /wd3/j5c/p002/etc/apache/apache.conf -k start
        vec.emplace_back("eval \"cd " + apePath + "; ./apachectl -k restart \"");
        int temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        vec.emplace_back("# ");
        if (temp == 0)
        {
            vec.emplace_back("# Restarting Apache Web Server was successful.");
        }
        else
        {
            // The first attempt failed because the server is already running,
            //   But it is probably running under this config file name, so lets try again.
            //   Even though we know it is running somewhere.
            vec.clear();
            vec.emplace_back("# ");
            vec.emplace_back("# The server appears to be online, lets see if we can restart.");
            vec.emplace_back("#     using the most likely configuration file...");
            vec.emplace_back("eval \"cd '" + apePath + "'; ./apachectl -f "
                             + itemValues.etcPath.substr(0,itemValues.etcPath.length()-4)
                             + "apache/apache.conf -k restart \"");
            temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
            vec.emplace_back("# ");
            if (temp == 0) {
                vec.emplace_back("# Restarting Apache Web Server was successful.");
            } else {
                vec.emplace_back("# Skipping restart of Apache Web Server due to failure of commands.");
            }
        }

        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# MariaDB Database Server files appear to be installed.");
        vec.emplace_back("#   Not attempting to start mariadb. ");
        vec.emplace_back("#   MariaDB requires more setup. Follow the instructions");
        vec.emplace_back("#   in '" + itemValues.rtnPath + "Installation_Notes_" + itemValues.pathVersion + ".txt' " );
        vec.emplace_back("#   To start MariaDB." );
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

        set_bInstall(itemValues);
        if (itemValues.bInstall)
        {
            appendNewLogSection(itemValues.fileName_Build);
            ensureStageDirectoryExists(itemValues);
            bool staged = stageSourceCodeIfNeeded(itemValues);

            if (!staged) {
                // If the source code was just downloaded the file name is mirror
                // instead of anything useful, so we need to rename the rename the file
                vec.clear();
                vec.emplace_back("# ");
                vec.emplace_back("# Change downloaded file name if needed.");
                vec.emplace_back(
                        "eval \"cd " + itemValues.stgPath + "; mv -f mirror '" + itemValues.fileName_Compressed + "' \"");
                do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
            }
            itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);

            bool securityCheck = check_Sha256sum(itemValues);
            if (securityCheck)
            {
                result = setupInstallDirectories(itemValues);

                sstr configureStr = create_php_configuration(settings, itemValues);
                result += basicInstall(itemValues, configureStr);
                result += do_post_install(settings, itemValues, result);

                // end of code block
                createProtectionWhenRequired(result, itemValues, false);
            } else {
                result = badSha256sum(itemValues);
            }
        }
    }
    else {
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# Apache Web Server and MariaDB are required ");
        vec.emplace_back("# to be installed before PHP can be installed.");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        result = 1;
    }
    return  result;
}


int postInstall_PHP(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = 0;
    int temp   = 0;
    sstr positionCommand = std::string(commandPosition, ' ');

    sstr compileForDebug    = settings[itemValues.programName + "->Compile_For_Debug"];
    sstr xdebug_install     = settings[itemValues.programName + "->Xdebug_Install"];
    sstr xdebug_name        = settings[itemValues.programName + "->Xdebug_Name"];
    sstr xdebug_version     = settings[itemValues.programName + "->Xdebug_Version"];
    sstr xdebug_compression = settings[itemValues.programName + "->Xdebug_Compression"];
    sstr xdebug_wget        = settings[itemValues.programName + "->Xdebug_WGET"];
    sstr xdebug_tar_options = settings[itemValues.programName + "->Xdebug_Tar_Options"];
    sstr zts_version        = settings[itemValues.programName + "->zts_version"];
    bool bCompileForDebug   = getBoolFromString(compileForDebug);
    bool bInstall_Xdebug    = getBoolFromString(xdebug_install);

    sstr xDebugProgVersion        =  xdebug_name + "-" + xdebug_version;
    sstr xDebugCompressedFileName =  xDebugProgVersion + xdebug_compression;

    sstr tmpPath = "usr/apache/bin";
    sstr apePath = joinPathParts(itemValues.rtnPath, tmpPath);

    sstr tmpFile = "apachectl";
    sstr apacheController  = joinPathWithFile(apePath, tmpFile);

    tmpFile = "mysqld";
    sstr mariadbPath = itemValues.rtnPath + "usr/mariadb/bin";
    sstr mariadbController = joinPathWithFile(mariadbPath, tmpFile);

    std::vector<sstr> vec;
    vec.clear();
    vec.emplace_back("eval \"cd '" + itemValues.usrPath + "'; mkdir -p libs \"");
    result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    vec.clear();
    vec.emplace_back("# ");
    vec.emplace_back("# Copy Php.ini files to '" + itemValues.etcPath + "'");
    vec.emplace_back("eval \"cd '" + itemValues.srcPathPNV + "'; cp *.ini* '" + itemValues.etcPath  + ".' \"");
    vec.emplace_back("# ");
    vec.emplace_back("# libtool --finish");
    vec.emplace_back("eval \"cd '" + itemValues.srcPathPNV + "'; cp libs/* '" + itemValues.usrPath + "libs/.' \"");
    vec.emplace_back("eval \"cd '" + itemValues.srcPathPNV + "'; ./libtool --finish '" + itemValues.usrPath + "libs' \"");
    vec.emplace_back("# ");
    vec.emplace_back("# Copy library to apache web server");

    vec.emplace_back("eval \"cp '" + itemValues.usrPath + "libs/libphp.so' '" + itemValues.rtnPath + "usr/apache/modules/libphp.so'  \"");
    vec.emplace_back("eval \"cp '" + itemValues.usrPath + "libs/libphp.so' '" + itemValues.rtnPath + "usr/apache/modules/mod_php.so' \"");
    temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    vec.clear();
    if (temp == 0) {
        vec.emplace_back("# Copy library file operations were successful.");
    }
    else  {
        vec.emplace_back("# Copy library file operations were NOT successful.");
    }
    result += temp;
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    vec.clear();
    vec.emplace_back("# ");
    vec.emplace_back("# Ensure Apache user / group exists");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    vec.clear();
    vec.emplace_back("# ");
    vec.emplace_back("# Set apache ownership");
    vec.emplace_back("eval \"chown root:" + APACHE_GROUP + " '" + itemValues.rtnPath + "usr/apache/modules/libphp.so' \"");
    vec.emplace_back("eval \"chown root:" + APACHE_GROUP + " '" + itemValues.rtnPath + "usr/apache/modules/mod_php.so' \"");
    vec.emplace_back("# ");
    vec.emplace_back("# Set apache permissions");
    vec.emplace_back("eval \"chmod 755 '" + itemValues.rtnPath + "usr/apache/modules/libphp.so' \"");
    vec.emplace_back("eval \"chmod 755 '" + itemValues.rtnPath + "usr/apache/modules/mod_php.so' \"");
    temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    vec.clear();
    if (temp == 0) {
        vec.emplace_back("# Change ownership and permission file operations were successful.");
    } else {
        vec.emplace_back("# Change ownership and permission file operations NOT were successful.");
    }
    result += temp;
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    sstr xdebugProgVersionCompression = xdebug_version + xdebug_compression;

    // When analyzing code, view this whole block together...
    //    if we install Xdebug, are we installing for PHP (debug mode) or PHP (non debug mode)?
    //    depending on the mode of PHP we need to change some text below...
    if (bInstall_Xdebug) {
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# wget xdebug");
        vec.emplace_back("eval \"cd '" + itemValues.usrPath + "';\nwget '" + xdebug_wget + xDebugCompressedFileName + "' \"");
        vec.emplace_back(
                "eval \"cd '" + itemValues.usrPath + "'; tar '" + xdebug_tar_options + "' '" + xDebugCompressedFileName +
                "' \"");

        vec.emplace_back("# ");
        vec.emplace_back("# phpize");
        vec.emplace_back("eval \"cd '" + itemValues.usrPath + xDebugProgVersion + "';\n ../bin/phpize > '" + itemValues.bldPath + "phpize.txt' \"");
        vec.emplace_back("# ");
        vec.emplace_back("# config");
        vec.emplace_back("eval \"cd '" + itemValues.usrPath + xDebugProgVersion + "'; ./configure --with-php-config='"
                         + itemValues.usrPath + "bin/php-config' > '" + itemValues.bldPath + "xdebug-configure.txt' \"");

        temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        vec.clear();
        if (temp == 0) {
            vec.emplace_back("# Wget, phpize, and configure commands were successful.");
        } else {
            vec.emplace_back("# Wget, phpize, and configure commands were NOT successful.");
        }
        result += temp;
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# make");
        vec.emplace_back("eval \"cd '" + itemValues.usrPath + xDebugProgVersion + "'; make > '"
                         + itemValues.bldPath + "xdebug-make.txt' \"");
        temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        vec.clear();
        if (temp == 0) {
            vec.emplace_back("# Make command was successful.");
        } else {
            vec.emplace_back("# Make command was NOT successful.");
        }
        result += temp;
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# cp modules/xdebug.so");

        // So apparently php change directory structure slightly so this is the smallest adjustment I could make
        //   and still get the results I want.
        //   -- (and a few other changes a few lines down as well changing lib to libs)
        vec.emplace_back("eval \"mkdir -p '" + itemValues.usrPath + "libs/php/extensions'\"");
        // end of these notes

        // checking for the mode of PHP and adjusting accordingly
        if (bCompileForDebug) {
            vec.emplace_back("eval \"cd '" + itemValues.usrPath + xDebugProgVersion + "'; cp modules/xdebug.so '"
                             + itemValues.usrPath + "libs/php/extensions/debug-zts-" + zts_version + "' \"");

        } else {
            vec.emplace_back("eval \"cd '" + itemValues.usrPath + xDebugProgVersion + "'; cp modules/xdebug.so '"
                             + itemValues.usrPath + "libs/php/extensions/no-debug-zts-" + zts_version + "' \"");

        }
        result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

        // This small section is the same for PHP
        //   regardless of the debug / non-debug mode.
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("# Create: '" + itemValues.etcPath + "lib'");
        vec.emplace_back("eval \"mkdir -p '" + itemValues.etcPath + "lib' \"");
        // end of small section

        // checking for the mode of PHP and adjusting accordingly
        if (bCompileForDebug) {
            vec.emplace_back("# ");
            vec.emplace_back("# zend_extension = '" + itemValues.usrPath + "libs/php/extensions/debug-zts-" + zts_version +
                             "/xdebug.so'");
            vec.emplace_back("eval \"cd '" + itemValues.etcPath + "libs/';\necho zend_extension = '"
                             + itemValues.usrPath + "libs/php/extensions/debug-zts-" + zts_version +
                             "/xdebug.so' > php_ext.ini \"");
        } else {
            vec.emplace_back("# ");
            vec.emplace_back("# zend_extension = '" + itemValues.usrPath + "libs/php/extensions/debug-zts-" + zts_version +
                             "/xdebug.so'");
            vec.emplace_back("eval \"cd '" + itemValues.etcPath + "lib/';\necho zend_extension = '"
                             + itemValues.usrPath + "libs/php/extensions/no-debug-zts-" + zts_version +
                             "/xdebug.so' > php_ext.ini \"");
        }
    } else {
        vec.emplace_back("# Xdebug not installed.");
    }
    temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    vec.clear();
    if (temp == 0) {
        vec.emplace_back("# Copy libraries and modules was successful.");
    } else {
        vec.emplace_back("# Copy libraries and modules was not successful.");
    }
    result += temp;
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_PHP_H
