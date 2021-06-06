//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_APACHE_INSTALL_H
#define J5C_INSTALL_FROM_SOURCE_001_APACHE_INSTALL_H

int install_apache_step_01(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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

            sstr configureStr = "eval \"cd '" + itemValues.srcPathPNV + "' \";\n"
                                + positionCommand + " ./configure --prefix='" + itemValues.usrPath + "' \\\n";
            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}

int install_apache_step_02(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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
            sstr aprBasePath = itemValues.rtnPath + "usr/apr/bin";
            sstr configureStr = "eval \"cd '" + itemValues.srcPathPNV + "' \";\n";

            std::vector<sstr> commands;
            commands.emplace_back(positionCommand + "./configure");
            commands.emplace_back(positionCommand + "  --prefix='" + itemValues.usrPath + "'");
            commands.emplace_back(positionCommand + "  --with-apr='" + aprBasePath + "'");
            configureStr.append(multilineCommand(commands, false));

            result += basicInstall(itemValues, configureStr);

            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }

    }
    return result;
}

int install_apache_step_03(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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
            sstr aprBasePath = itemValues.rtnPath + "usr/apr/bin";
            sstr configureStr = "eval \"cd '" + itemValues.srcPathPNV + "' \";\n";

            std::vector<sstr> commands;
            commands.emplace_back(positionCommand + "./configure");
            commands.emplace_back(positionCommand + "  --prefix='" + itemValues.usrPath + "'");
            commands.emplace_back(positionCommand + "  --with-apr='" + aprBasePath + "'");
            configureStr.append(multilineCommand(commands, false));

            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}

int install_apache_step_04(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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
            sstr configureStr = "eval \"cd '" + itemValues.srcPathPNV + "' \";\n";

            std::vector<sstr> commands;
            commands.emplace_back(positionCommand + "./configure");
            commands.emplace_back(positionCommand + "  --prefix='" + itemValues.usrPath + "'");
            configureStr.append(multilineCommand(commands, false));

            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}

int install_apache_step_05(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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
            sstr configureStr =    "eval \"    cd '" + itemValues.srcPathPNV + "' \";\n";
            std::vector<sstr> commands;
            commands.emplace_back(positionCommand + "./configure");
            commands.emplace_back(positionCommand + "  --prefix='" + itemValues.usrPath + "'");
            configureStr.append(multilineCommand(commands, false));

            result += basicInstall(itemValues, configureStr);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}

int install_apache(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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
            sstr usrBasePath = itemValues.rtnPath + "usr/";
            sstr configureStr = "eval \"cd '" + itemValues.srcPathPNV + "' \";\n";

            std::vector<sstr> commands;
            commands.emplace_back(positionCommand + "./configure");
            commands.emplace_back(positionCommand + "  --prefix='" + itemValues.usrPath + "'");
            commands.emplace_back(positionCommand + "  --with-apr='" + usrBasePath + "apr'");
            commands.emplace_back(positionCommand + "  --with-apr-util='" + usrBasePath + "apr-util'");
            commands.emplace_back(positionCommand + "  --with-apr-iconv='" + usrBasePath + "apr-iconv'");
            commands.emplace_back(positionCommand + "  --with-pcre='" + usrBasePath + "pcre'");
            //
            // Note:
            //  We are NOT enabling http2 in the apache setup
            //     because of the following error
            //
            //    checking for nghttp2... checking for user-provided nghttp2 base directory... none
            //    checking for pkg-config along ... checking for nghttp2 version >= 1.2.1... FAILED
            //    configure: WARNING: nghttp2 version is too old
            //
            //  even though
            //
            //  yum install libnghttp2
            //  Loaded plugins: fastestmirror, langpacks
            //  Loading mirror speeds from cached hostfile
            //     * base: mirror.web-ster.com
            //     * epel: mirror.prgmr.com
            //     * extras: mirror.web-ster.com
            //     * updates: mirror.web-ster.com
            //  Package libnghttp2-1.31.1-1.el7.x86_64 already installed and latest version
            //  Nothing to do
            //
            //  so we have to comment out the next command
            //
            //  commands.emplace_back(positionCommand + "  --enable-http2");
            //
            commands.emplace_back(positionCommand + "  --enable-so");
            configureStr.append(multilineCommand(commands, false));

            result += basicInstall(itemValues, configureStr);
            result += do_post_install(settings, itemValues, result);

            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}


int create_apache_conf_File(an_itemValues &itemValues)
{
    std::vector<sstr> vec;
    sstr positionCommand = std::string(commandPosition, ' ');
    vec.clear();
    vec.emplace_back("#Save the original apache.conf file if any, to have as a guide.");

    //lets add the date_time to the my.cnf.old in case we run this multiple times
    //  in a day we will still have the original file somewhere.

    sstr theDate = make_fileName_dateTime(0);
    vec.emplace_back("eval \"cd /etc/httpd/conf/; cp httpd.conf \\\n"
                     + positionCommand  + "  '" +  itemValues.etcPath + "extra/rpm_based_httpd.conf.old_" + theDate + "' \"");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    //Now we have to create the new my.cnf file
    vec.clear();
    vec.emplace_back("#Create new file " + itemValues.etcPath + "apache.conf ");
    vec.emplace_back("eval \"cd /etc; echo ' ' >" + itemValues.etcPath + "apache.conf \"");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    sstr apache_conf_File = itemValues.etcPath + "apache.conf";
    //Now we add the content to the /etc/my.cnf file
    vec.clear();
    vec.emplace_back("#");
    vec.emplace_back("# This is the main Apache HTTP server configuration file.");
    vec.emplace_back("# See <URL:http://httpd.apache.org/docs/2.4/> for detailed information.");
    vec.emplace_back("#");
    vec.emplace_back("# If you are unsure consult the online docs. ");
    vec.emplace_back("# This default configuration does not give adequate protection, You have been warned.");
    vec.emplace_back("# Additional setup is required by the system administrator");
    vec.emplace_back("");
    vec.emplace_back("ServerRoot " + itemValues.usrPath);
    vec.emplace_back("Listen 80");
    vec.emplace_back("");
    vec.emplace_back("# To check the syntax of this file run the following commands");
    vec.emplace_back("# cd " + itemValues.usrPath + "bin;");
    vec.emplace_back("# ./apachectl -f " + itemValues.etcPath + "apache.conf -t");
    vec.emplace_back("");
    vec.emplace_back("# To check the syntax of this virtual hosts file(s) run the following commands");
    vec.emplace_back("# cd " + itemValues.usrPath + "bin;");
    vec.emplace_back("# ./apachectl -f " + itemValues.etcPath + "apache.conf -S");
    vec.emplace_back("");
    vec.emplace_back("# To check the version of Apache Web server run the following commands");
    vec.emplace_back("# cd " + itemValues.usrPath + "bin;");
    vec.emplace_back("# ./apachectl -f " + itemValues.etcPath + "apache.conf -v");
    vec.emplace_back("");
    vec.emplace_back("# To start/restart Apache Web server run the following commands");
    vec.emplace_back("# cd " + itemValues.usrPath + "bin;");
    vec.emplace_back("# ./apachectl -f " + itemValues.etcPath + "apache.conf -k restart");
    vec.emplace_back("");
    vec.emplace_back("# To stop Apache Web server run the following commands");
    vec.emplace_back("# cd " + itemValues.usrPath + "bin;");
    vec.emplace_back("# ./apachectl -f " + itemValues.etcPath + "apache.conf -k graceful-stop");
    vec.emplace_back("#");
    vec.emplace_back("# Where possible the LoadModules have been put in alphabetical order.");
    vec.emplace_back("# But, order does matter in some cases, so if you change the order run:");
    vec.emplace_back("# cd " + itemValues.usrPath + "bin;");
    vec.emplace_back("# ./apachectl -f " + itemValues.etcPath + "apache.conf -k graceful-stop");
    vec.emplace_back("# ./apachectl -f " + itemValues.etcPath + "apache.conf -t");
    vec.emplace_back("# ./apachectl -f " + itemValues.etcPath + "apache.conf -k restart");
    vec.emplace_back("#  Then check the error log to see if there are any module related errors.");
    vec.emplace_back("#");
    vec.emplace_back("# Dynamic Shared Object (DSO) Support");
    vec.emplace_back("#");
    vec.emplace_back("LoadModule access_compat_module modules/mod_access_compat.so");
    vec.emplace_back("LoadModule alias_module modules/mod_alias.so");
    vec.emplace_back("LoadModule request_module modules/mod_request.so");
    vec.emplace_back("LoadModule auth_form_module modules/mod_auth_form.so");
    vec.emplace_back("LoadModule autoindex_module modules/mod_autoindex.so");
    vec.emplace_back("LoadModule authn_file_module modules/mod_authn_file.so");
    vec.emplace_back("#");
    vec.emplace_back("LoadModule authn_core_module modules/mod_authn_core.so");
    vec.emplace_back("LoadModule authz_host_module modules/mod_authz_host.so");
    vec.emplace_back("LoadModule authz_groupfile_module modules/mod_authz_groupfile.so");
    vec.emplace_back("LoadModule authz_user_module modules/mod_authz_user.so");
    vec.emplace_back("LoadModule authz_core_module modules/mod_authz_core.so");
    vec.emplace_back("LoadModule auth_basic_module modules/mod_auth_basic.so");
    vec.emplace_back("#");
    vec.emplace_back("LoadModule cache_module modules/mod_cache.so");
    vec.emplace_back("LoadModule cache_disk_module modules/mod_cache_disk.so");
    vec.emplace_back("LoadModule cache_socache_module modules/mod_cache_socache.so");
    vec.emplace_back("LoadModule dir_module modules/mod_dir.so");
    vec.emplace_back("LoadModule env_module modules/mod_env.so");
    vec.emplace_back("LoadModule file_cache_module modules/mod_file_cache.so");
    vec.emplace_back("#");
    vec.emplace_back("LoadModule filter_module modules/mod_filter.so");
    vec.emplace_back("LoadModule headers_module modules/mod_headers.so");
    vec.emplace_back("LoadModule include_module modules/mod_include.so");
    vec.emplace_back("LoadModule info_module modules/mod_info.so");
    vec.emplace_back("LoadModule lbmethod_heartbeat_module modules/mod_lbmethod_heartbeat.so");
    vec.emplace_back("LoadModule log_config_module modules/mod_log_config.so");
    vec.emplace_back("#");
    vec.emplace_back("LoadModule mime_module modules/mod_mime.so");
    vec.emplace_back("LoadModule negotiation_module modules/mod_negotiation.so");
    vec.emplace_back("LoadModule php7_module modules/mod_php7.so");
    vec.emplace_back("LoadModule reqtimeout_module modules/mod_reqtimeout.so");
    vec.emplace_back("LoadModule session_module modules/mod_session.so");
    vec.emplace_back("#");
    vec.emplace_back("LoadModule session_cookie_module modules/mod_session_cookie.so");
    vec.emplace_back("LoadModule setenvif_module modules/mod_setenvif.so");
    vec.emplace_back("LoadModule socache_shmcb_module modules/mod_socache_shmcb.so");
    vec.emplace_back("LoadModule socache_dbm_module modules/mod_socache_dbm.so");
    vec.emplace_back("LoadModule socache_memcache_module modules/mod_socache_memcache.so");
    vec.emplace_back("LoadModule slotmem_shm_module modules/mod_slotmem_shm.so");
    vec.emplace_back("#");
    vec.emplace_back("LoadModule ssl_module modules/mod_ssl.so");
    vec.emplace_back("LoadModule status_module modules/mod_status.so");
    vec.emplace_back("LoadModule substitute_module modules/mod_substitute.so");
    vec.emplace_back("LoadModule version_module modules/mod_version.so");
    vec.emplace_back("LoadModule unixd_module modules/mod_unixd.so");
    vec.emplace_back("LoadModule vhost_alias_module modules/mod_vhost_alias.so");
    vec.emplace_back("#");
    vec.emplace_back("# Unused Modules removed...get newly required from the original file...");
    vec.emplace_back("#");
    vec.emplace_back("  <IfModule unixd_module>");
    vec.emplace_back("    User  apache_ws");
    vec.emplace_back("    Group apache_ws");
    vec.emplace_back("  </IfModule>");
    vec.emplace_back("#");
    vec.emplace_back("  <Directory />");
    vec.emplace_back("    AllowOverride none");
    vec.emplace_back("    Require all denied");
    vec.emplace_back("  </Directory>");
    vec.emplace_back("#");
    vec.emplace_back("ServerAdmin yourAdminEmail@somewhere.com");
    vec.emplace_back("ServerName  127.0.1.1");
    vec.emplace_back("");
    vec.emplace_back("DocumentRoot " + itemValues.usrPath  + "htdocs");
    vec.emplace_back("<Directory   " + itemValues.usrPath  + "htdocs>");
    vec.emplace_back("");
    vec.emplace_back("  <RequireAny>");
    vec.emplace_back("    Require ip 10.0");
    vec.emplace_back("    Require ip 192.168");
    vec.emplace_back("    Require ip ::1");
    vec.emplace_back("</RequireAny>");
    vec.emplace_back("");
    vec.emplace_back("  Require all Granted");
    vec.emplace_back("");
    vec.emplace_back("  AddType application/x-httpd-php .php");
    vec.emplace_back("");
    vec.emplace_back("  <IfModule php7_module>");
    vec.emplace_back("    php_flag magic_quotes_gpc Off");
    vec.emplace_back("    php_flag short_open_tag Off");
    vec.emplace_back("    php_flag register_globals Off");
    vec.emplace_back("    php_flag register_argc_argv On");
    vec.emplace_back("    php_flag track_vars On");
    vec.emplace_back("    # this setting is necessary for some locales");
    vec.emplace_back("    php_value mbstring.func_overload 0");
    vec.emplace_back("    php_value include_path .");
    vec.emplace_back("  </IfModule>");
    vec.emplace_back("");
    vec.emplace_back("</Directory>");
    vec.emplace_back("");
    vec.emplace_back("<IfModule dir_module>");
    vec.emplace_back("  DirectoryIndex index.php index.html");
    vec.emplace_back("</IfModule>");
    vec.emplace_back("#");
    vec.emplace_back("#");
    vec.emplace_back("<Files \".ht*\">");
    vec.emplace_back("  Require all denied");
    vec.emplace_back("</Files>");
    vec.emplace_back("#");
    vec.emplace_back("ErrorLog \"logs/error_log\"");
    vec.emplace_back("LogLevel trace6");
    vec.emplace_back("#");
    vec.emplace_back("<IfModule log_config_module>");
    vec.emplace_back("  LogFormat \"%h %l %u %t \\\"%r\\\" %>s %b \\\"%{Referer}i\\\" \\\"%{User-Agent}i\\\" combined");
    vec.emplace_back("  LogFormat \"%h %l %u %t \\\"%r\\\" %>s %b\" common");
    vec.emplace_back("<IfModule logio_module>");
    vec.emplace_back("  # You need to enable mod_logio.c to use %I and %O");
    vec.emplace_back("    LogFormat \"%h %l %u %t \\\"%r\\\" %>s %b \\\"%{Referer}i\\\" \\\"%{User-Agent}i\\\" %I %O\" combinedio");
    vec.emplace_back("</IfModule>");
    vec.emplace_back("  CustomLog \"logs/access_log\" common");
    vec.emplace_back("</IfModule>");
    vec.emplace_back("#");
    vec.emplace_back("#");
    vec.emplace_back("# Customizable error responses come in three flavors:");
    vec.emplace_back("# 1) plain text 2) local redirects 3) external redirects");
    vec.emplace_back("#");
    vec.emplace_back("# Some examples:");
    vec.emplace_back("#ErrorDocument 500 \"Server Fault.\"");
    vec.emplace_back("#ErrorDocument 404 /missing.html");
    vec.emplace_back("#ErrorDocument 404 \"/cgi-bin/missing_handler.pl\"");
    vec.emplace_back("#ErrorDocument 402 http://www.example.com/subscription_info.html");
    vec.emplace_back("#");
    vec.emplace_back("#");
    vec.emplace_back("EnableMMAP off");
    vec.emplace_back("EnableSendfile on");
    vec.emplace_back("#");
    vec.emplace_back("<IfModule ssl_module>");
    vec.emplace_back("  Listen 443");
    vec.emplace_back("  SSLRandomSeed startup builtin");
    vec.emplace_back("  SSLRandomSeed connect builtin");
    vec.emplace_back("  SSLSessionCache  shmcb:" + itemValues.rtnPath +"tls/ssl_scache(512000)");
    vec.emplace_back("</IfModule>");
    vec.emplace_back("");
    vec.emplace_back("# Server-pool management (MPM specific)");
    vec.emplace_back("  Include " + itemValues.etcPath + "extra/httpd-mpm.conf");
    vec.emplace_back("");
    vec.emplace_back("# Multi-language error messages");
    vec.emplace_back("  Include " + itemValues.etcPath + "extra/httpd-multilang-errordoc.conf");
    vec.emplace_back("");
    vec.emplace_back("# Fancy directory listings");
    vec.emplace_back("  Include " + itemValues.etcPath + "extra/httpd-autoindex.conf");
    vec.emplace_back("");
    vec.emplace_back("# Language settings");
    vec.emplace_back("  Include " + itemValues.etcPath + "extra/httpd-languages.conf");
    vec.emplace_back("");
    vec.emplace_back("# Real-time info on requests and configuration");
    vec.emplace_back("  Include " + itemValues.etcPath + "extra/httpd-info.conf");
    vec.emplace_back("");
    vec.emplace_back("# Virtual hosts");
    vec.emplace_back("#  Include " + itemValues.etcPath + "sites-enabled/site-001-dev.conf");
    vec.emplace_back("#  Include " + itemValues.etcPath + "sites-enabled/site-001-tst.conf");
    vec.emplace_back("#  Include " + itemValues.etcPath + "sites-enabled/site-001-prd.conf");
    vec.emplace_back("");
    vec.emplace_back("# Local access to the Apache HTTP Server Manual");
    vec.emplace_back("  Include " + itemValues.etcPath + "extra/httpd-manual.conf");
    vec.emplace_back("");
    vec.emplace_back("## Unused Extras removed...get required from the original file...");
    vec.emplace_back("");
    vec.emplace_back("## end of configuration file");
    vec.emplace_back("");
    file_write_vector_to_file(apache_conf_File, vec, false);
    return 0;
};

int postInstall_Apache(an_itemValues& itemValues)
{
    int result = 0;
    std::vector<sstr> vec;
    sstr positionCommand = std::string(commandPosition, ' ');
    vec.clear();
    vec.emplace_back("# ");
    vec.emplace_back("# Copy apache configuration files to '" + itemValues.etcPath + "extra'");
    vec.emplace_back("mkdir -p '" + itemValues.etcPath + "extra' ");
    vec.emplace_back("cp   -rf '" + itemValues.usrPath + "conf/extra/'* '" + itemValues.etcPath  + "extra/.' ");
    int temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    std::cout << "cp results = " << temp << std::endl;
    vec.clear();
    if (temp == 0) {
        vec.emplace_back("# Copy apache configuration files was successful.");
    } else {
        vec.emplace_back("# Copy apache configuration files was NOT successful.");
    }
    result += temp;

    //set permissions for apache directory recursively
    vec.clear();
    vec.emplace_back("# ");
    vec.emplace_back("# Change Apache permissions.");
    vec.emplace_back("chown -R root:" + APACHE_GROUP + " '" + itemValues.usrPath + "'");
    vec.emplace_back("chmod -R 770  '"                      + itemValues.usrPath + "'");
    temp = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    vec.clear();
    vec.emplace_back("# ");
    vec.emplace_back("chown -R root:" + APACHE_GROUP + " '" + itemValues.etcPath + "'");
    vec.emplace_back("chmod -R 770 '" + itemValues.etcPath + "'");
    temp += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    vec.clear();
    if (temp == 0) {
        vec.emplace_back("# Change Apache file permissions were successful.");
    } else {
        vec.emplace_back("# Change Apache file permissions were NOT successful.");
    }
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    result += temp;

    return result;
}

int apache_notes(an_itemValues& itemValues)
{
    int result = 0;
    std::vector<sstr> vec;
    if (itemValues.ProperName == "Apache") {

        vec.clear();
        vec.emplace_back("#################################################################################");
        vec.emplace_back("# ");
        vec.emplace_back("# Apache Note Section");
        vec.emplace_back("# ");
        vec.emplace_back("#################################################################################");
        vec.emplace_back("# ");
        vec.emplace_back("#--> Commands to create the apache group and user.");
        vec.emplace_back("#-->   Should have been completed with a succesful install.");
        vec.emplace_back("groupadd   apache_ws ");
        vec.emplace_back("useradd -g apache_ws apache_ws ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        //Ensure firewalld starts on boot and more
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("#You can only use iptables or firewalld");
        vec.emplace_back("#We are choosing to only use firewalld");
        vec.emplace_back("#Ensure iptables will not start on boot");
        vec.emplace_back("systemctl mask   iptables");
        vec.emplace_back("#Ensure firewalld starts on boot");
        vec.emplace_back("systemctl enable firewalld");
        vec.emplace_back("#It could be a good idea to reboot your system now.");
        vec.emplace_back("#Ensure firewalld is started now");
        vec.emplace_back("systemctl status -l  firewalld");
        vec.emplace_back("#If your filewall is not running use this command.");
        vec.emplace_back("systemctl start  firewalld");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("firewall-cmd             --list-ports");
        vec.emplace_back("firewall-cmd --permanent --list-ports");
        vec.emplace_back("#");
        vec.emplace_back("#After making temp changes to reset firewall back to permanent rules");
        vec.emplace_back("systemctl reload firewalld");
        vec.emplace_back("# ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        //Remove http and https from firewall before website is ready
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("#Remove http and https from firewall before website is ready");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("firewall-cmd             --remove-service=http");
        vec.emplace_back("firewall-cmd --permanent --remove-service=http");
        vec.emplace_back("firewall-cmd             --remove-service=https");
        vec.emplace_back("firewall-cmd --permanent --remove-service=https");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("systemctl reload firewalld");
        vec.emplace_back("# ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        //Remove http port and https port from firewall when website is not ready
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("#Remove http and https from firewall before website is ready");
        vec.emplace_back("firewall-cmd             --list-ports");
        vec.emplace_back("firewall-cmd --permanent --list-ports");
        vec.emplace_back("firewall-cmd             --remove-port=80/tcp");
        vec.emplace_back("firewall-cmd --permanent --remove-port=80/tcp");
        vec.emplace_back("firewall-cmd             --remove-port=443/tcp");
        vec.emplace_back("firewall-cmd --permanent --remove-port=443/tcp");
        vec.emplace_back("firewall-cmd             --list-ports");
        vec.emplace_back("firewall-cmd --permanent --list-ports");
        vec.emplace_back("systemctl reload firewalld");
        vec.emplace_back("# ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        //Remove remote access via ssh
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("#Remove remote access if needed for security");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("firewall-cmd             --remove-service=ssh");
        vec.emplace_back("firewall-cmd --permanent --remove-service=ssh");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("systemctl reload firewalld");
        vec.emplace_back("# ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        //Remove remote access via ssh
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("#Remove remote access if needed for security");
        vec.emplace_back("firewall-cmd             --list-ports");
        vec.emplace_back("firewall-cmd --permanent --list-ports");
        vec.emplace_back("firewall-cmd             --remove-port=22/tcp");
        vec.emplace_back("firewall-cmd --permanent --remove-port=22/tcp");
        vec.emplace_back("firewall-cmd             --list-ports");
        vec.emplace_back("firewall-cmd --permanent --list-ports");
        vec.emplace_back("systemctl reload firewalld");
        vec.emplace_back("# ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        //Add remote access via ssh
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("#Add remote access if needed");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("firewall-cmd             --add-service=ssh");
        vec.emplace_back("firewall-cmd --permanent --add-service=ssh");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("systemctl reload firewalld");
        vec.emplace_back("# ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        //Add remote access via ssh
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("#Add remote access if needed for security");
        vec.emplace_back("firewall-cmd             --list-ports");
        vec.emplace_back("firewall-cmd --permanent --list-ports");
        vec.emplace_back("firewall-cmd             --add-port=22/tcp");
        vec.emplace_back("firewall-cmd --permanent --add-port=22/tcp");
        vec.emplace_back("firewall-cmd             --list-ports");
        vec.emplace_back("firewall-cmd --permanent --list-ports");
        vec.emplace_back("systemctl reload firewalld");
        vec.emplace_back("# ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        //Add http and https to firewall when website is ready
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("#Add http and https to firewall when website is ready");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("firewall-cmd             --add-service=http");
        vec.emplace_back("firewall-cmd --permanent --add-service=http");
        vec.emplace_back("firewall-cmd             --add-service=https");
        vec.emplace_back("firewall-cmd --permanent --add-service=https");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("systemctl reload firewalld");
        vec.emplace_back("# ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        //Add http port and https port to firewall when website is ready
        vec.clear();
        vec.emplace_back("#Add http and https to firewall when website is ready");
        vec.emplace_back("firewall-cmd             --list-ports");
        vec.emplace_back("firewall-cmd --permanent --list-ports");
        vec.emplace_back("firewall-cmd             --add-port=80/tcp");
        vec.emplace_back("firewall-cmd --permanent --add-port=80/tcp");
        vec.emplace_back("firewall-cmd             --add-port=443/tcp");
        vec.emplace_back("firewall-cmd --permanent --add-port=443/tcp");
        vec.emplace_back("firewall-cmd             --list-services");
        vec.emplace_back("firewall-cmd --permanent --list-services");
        vec.emplace_back("systemctl reload firewalld");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);


        //Create required directories if needed
        vec.clear();
        vec.emplace_back("# ");
        vec.emplace_back("mkdir -p " + itemValues.etcPath + "extra");
        vec.emplace_back("mkdir -p " + itemValues.etcPath + "sites-available");
        vec.emplace_back("mkdir -p " + itemValues.etcPath + "sites-enabled");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        vec.clear();
        vec.emplace_back("");
        vec.emplace_back("# To check the version of Apache Web server run the following commands");
        vec.emplace_back("cd " + itemValues.usrPath + "bin;");
        vec.emplace_back("./apachectl -f " + itemValues.etcPath + "apache.conf -v");
        vec.emplace_back("");
        vec.emplace_back("# To check the syntax of this file run the following commands");
        vec.emplace_back("cd " + itemValues.usrPath + "bin;");
        vec.emplace_back("./apachectl -f " + itemValues.etcPath + "apache.conf -t");
        vec.emplace_back("");
        vec.emplace_back("# To check the syntax of virtual hosts run the following commands");
        vec.emplace_back("cd " + itemValues.usrPath + "bin;");
        vec.emplace_back("./apachectl -f " + itemValues.etcPath + "apache.conf -S");
        vec.emplace_back("");
        vec.emplace_back("# To start/restart Apache Web server run the following commands");
        vec.emplace_back("cd " + itemValues.usrPath + "bin;");
        vec.emplace_back("./apachectl -f " + itemValues.etcPath + "apache.conf -k restart");
        vec.emplace_back("");
        vec.emplace_back("# To stop Apache Web server run the following commands");
        vec.emplace_back("cd " + itemValues.usrPath + "bin;");
        vec.emplace_back("./apachectl -f " + itemValues.etcPath + "apache.conf -k graceful-stop");
        vec.emplace_back("#");
        vec.emplace_back("# To set permissions run the following commands");
        vec.emplace_back("cd " + itemValues.etcPath + "../;");
        vec.emplace_back("chown -R :apache_ws apache");
        vec.emplace_back("chmod -R 750 apache");
        vec.emplace_back("# To set permissions run the following commands");
        vec.emplace_back("cd " + itemValues.usrPath + "../;");
        vec.emplace_back("chown -R :apache_ws apache");
        vec.emplace_back("chmod -R 750 apache");
        vec.emplace_back("#");

        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        create_apache_conf_File(itemValues);

        vec.clear();
        vec.emplace_back("#");
        vec.emplace_back("# See the Installation Notes on how to");
        vec.emplace_back("#   setup and start apache web server.");
        result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        return result;
    }
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_APACHE_INSTALL_H
