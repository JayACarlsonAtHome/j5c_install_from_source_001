//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_MARIADB_H
#define J5C_INSTALL_FROM_SOURCE_001_MARIADB_H

int create_mariaDB_cnf_File(an_itemValues &itemValues)
{
    std::vector<sstr> vec;
    vec.clear();
    vec.emplace_back("#Save the original my.cnf file if any, to have as a guide.");

    //lets add the date_time to the my.cnf.old in case we run this multiple times
    //  in a day we will still have the original file somewhere.

    sstr theDate = make_fileName_dateTime(0);
    vec.emplace_back("eval \"cd /etc; cp my.cnf " + itemValues.etcPath + "my.cnf.old_" + theDate + "\"");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    //Now we have to create the new my.cnf file
    vec.clear();
    vec.emplace_back("#Create new file " + itemValues.etcPath + "my.cnf ");
    vec.emplace_back("eval \"cd /etc; echo ' ' >" + itemValues.etcPath + "my.cnf \"");
    do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

    sstr my_cnf_File = itemValues.etcPath + "my.cnf";
    //Now we add the content to the /etc/my.cnf file
    vec.clear();
    vec.emplace_back("[mysqld]");
    vec.emplace_back("user=mysql");
    vec.emplace_back("socket='" + itemValues.usrPath + "run/mariadb.socket'");
    vec.emplace_back("bind-address=127.0.0.10");
    vec.emplace_back("port=3306");
    vec.emplace_back("#skip-external-locking");
    vec.emplace_back(" ");
    vec.emplace_back("#Directories");
    vec.emplace_back("  basedir='" + itemValues.usrPath + "'");
    vec.emplace_back("  datadir='" + itemValues.usrPath + "data/'");
    vec.emplace_back(" ");
    vec.emplace_back("#Log File");
    vec.emplace_back("  log-basename='MariaDB_Logs'");
    vec.emplace_back(" ");
    vec.emplace_back("#Other Stuff");
    vec.emplace_back("skip-ssl");
    vec.emplace_back("key_buffer_size=32M");
    vec.emplace_back(" ");
    vec.emplace_back("# Disabling symbolic-links is recommended to prevent assorted security risks symbolic-links=0");
    vec.emplace_back("# Settings user and group are ignored when systemd is used.");
    vec.emplace_back("# If you need to run mysqld under a different user or group,");
    vec.emplace_back("# customize your systemd unit file for mariadb according to the");
    vec.emplace_back("# instructions in http://fedoraproject.org/wiki/Systemd");
    vec.emplace_back(" ");
    vec.emplace_back("[mysqld_safe]");
    vec.emplace_back("log-error='" + itemValues.usrPath + "var/log/mariadb'");
    vec.emplace_back(" pid-file='" + itemValues.usrPath + "run/mariadb_pid'");
    vec.emplace_back(" ");
    vec.emplace_back("#");
    vec.emplace_back("# include all files from the config directory");
    vec.emplace_back("#");
    vec.emplace_back("# Not using anything here....");
    vec.emplace_back("#   in fact no directory or files here");
    vec.emplace_back("#   !includedir " + itemValues.etcPath + "my.cnf.d");
    vec.emplace_back(" ");
    vec.emplace_back("## end of file");
    vec.emplace_back(" ");
    file_write_vector_to_file(my_cnf_File, vec, false);
    return 0;
};

int install_mariadb(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
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
        if (itemValues.programName == "mariadb")
        {
            sstr ProgramName = itemValues.srcPrefix;
            ProgramName.append("-");
            ProgramName.append(itemValues.programNameVersion);
            itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, ProgramName);
        }
        else
        {
            itemValues.srcPathPNV = joinPathParts(itemValues.srcPath, itemValues.programNameVersion);
        }

        bool securityCheck = check_Sha256sum(itemValues);
        if (securityCheck)
        {
            std::cout << std::endl;
            std::cout << "itemValues.srcPath   :" << itemValues.srcPath    << std::endl;
            std::cout << "itemValues.srcPathPNV:" << itemValues.srcPathPNV << std::endl;
            std::cout << "itemValues.usrPath   :" << itemValues.usrPath    << std::endl;
            std::cout << std::endl;

            //git clone git@github.com:JayACarlsonAtHome/server.git
            //make clean
            //rm CMakeCache.txt
            //git clean -dffx
            //git reset --hard HEAD
            //git submodule update --init --recursive
            //mkdir build-mariadb-server-debug
            //cd build-mariadb-server-debug
            ///opt/J5C/p001/usr/cmake/bin/cmake ../../server -DCMAKE_BUILD_TYPE=Debug
            ///opt/J5C/p001/usr/cmake/bin/cmake  --build . --parallel=4
            //mysql-test/mtr --parallel=5 --mem --force --max-test-fail=40


            result = setupInstallDirectories(itemValues);
            ensureMariaDB_UserAndGroupExist(itemValues);
            sstr configureStr = "eval \"cd " + itemValues.srcPathPNV + "\";\n "
                                + positionCommand + "./BUILD/autorun.sh;\n "
                                + positionCommand + " cd '" + itemValues.srcPathPNV + "';\n";

            std::vector<sstr> commands;
            commands.emplace_back(positionCommand +  "./configure --prefix='" +  itemValues.usrPath + "'");
            commands.emplace_back(positionCommand +  "  --enable-assembler");
            commands.emplace_back(positionCommand +  "  --jemalloc_static_library='/usr/lib64'");
            commands.emplace_back(positionCommand +  "  --with-extra-charsets=complex");
            commands.emplace_back(positionCommand +  "  --enable-thread-safe-client");
            commands.emplace_back(positionCommand +  "  --with-big-tables");
            commands.emplace_back(positionCommand +  "  --with-plugin-maria");
            commands.emplace_back(positionCommand +  "  --with-aria-tmp-tables");
            commands.emplace_back(positionCommand +  "  --without-plugin-innodb_plugin");
            commands.emplace_back(positionCommand +  "  --with-mysqld-ldflags=-static");
            commands.emplace_back(positionCommand +  "  --with-client-ldflags=-static");
            commands.emplace_back(positionCommand +  "  --with-readline");
            commands.emplace_back(positionCommand +  "  --with-ssl");
            commands.emplace_back(positionCommand +  "  --with-embedded-server");
            commands.emplace_back(positionCommand +  "  --with-libevent");
            commands.emplace_back(positionCommand +  "  --with-mysqld-ldflags=-all-static");
            commands.emplace_back(positionCommand +  "  --with-client-ldflags=-all-static");
            commands.emplace_back(positionCommand +  "  --with-zlib-dir=bundled");
            commands.emplace_back(positionCommand +  "  --enable-local-infile");
            configureStr.append(multilineCommand(commands, false));

            //Git install -- requires (and not checked for in advance in this program)
            //   a forked version of mariadb in your own github repository
            //   gitk install on the clien machine
            //   and a lot of time for the compilation and tests


            result += do_post_install(settings, itemValues, result);
            createProtectionWhenRequired(result, itemValues, false);
        } else {
            result = badSha256sum(itemValues);
        }
    }
    return result;
}


int postInstall_MariaDB(std::map<sstr, sstr>& settings, an_itemValues& itemValues)
{
    int result = 0;
    sstr positionCommand = std::string(commandPosition, ' ');
    std::vector<sstr> vec;

    //Create required directories if needed
    vec.clear();
    vec.emplace_back("# ");
    vec.emplace_back("mkdir -p '" + itemValues.usrPath + "data/temp'");
    vec.emplace_back("mkdir -p '" + itemValues.usrPath + "data/source'");
    vec.emplace_back("mkdir -p '" + itemValues.usrPath + "run'");
    vec.emplace_back("mkdir -p '" + itemValues.usrPath + "var/log'");

    // Add required run files
    vec.emplace_back("# ");
    vec.emplace_back("cd '"       + itemValues.usrPath + "run'");
    vec.emplace_back("touch mariadb.socket ");
    vec.emplace_back("touch mariadb_pid ");
    //set permissions for mariadb directory recursively
    vec.emplace_back("# ");
    vec.emplace_back("eval \"cd '" + itemValues.rtnPath + "usr' \";\n ");
    vec.emplace_back("chown -R root:" + MARIADB_GROUP + " " + itemValues.programName );
    vec.emplace_back("chmod -R 770  '" + itemValues.programName + "'");
    //Over ride permissions as required
    vec.emplace_back("# ");
    vec.emplace_back("eval \"cd '" + itemValues.usrPath + "'\";\n");
    vec.emplace_back("chown -R " + MARIADB_OWNER + ":" + MARIADB_GROUP  + " data ");
    vec.emplace_back("chmod -R 770         data ");
    vec.emplace_back("chown -R " + MARIADB_OWNER + ":" + MARIADB_GROUP  + " run ");
    vec.emplace_back("chmod -R 770         run  ");
    vec.emplace_back("chown -R " + MARIADB_OWNER + ":" + MARIADB_GROUP  + " var ");
    vec.emplace_back("chmod -R 770         var  ");
    result += do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
    return result;
}

int mariadb_notes(an_itemValues& itemValues)
{
    int result = 0;
    std::vector<sstr> vec;
    if (itemValues.ProperName == "Mariadb") {

        sstr testPathAndFileName = itemValues.bldPath;
        sstr suffix = "test_results_02.txt";

        auto len = itemValues.usrPath.find_first_of("usr");
        sstr perlPath;
        perlPath.append(itemValues.usrPath.substr(0, len - 1));
        perlPath.append("/usr/perl/bin/perl");

        vec.clear();
        testPathAndFileName = joinPathWithFile(testPathAndFileName, suffix);
        vec.emplace_back("# ");
        vec.emplace_back("# optional testing...(after installation and starting)...");
        vec.emplace_back(
                "# eval \"cd '" + itemValues.srcPathPNV + "mysql-test'; '" + perlPath + "' mysql-test-run.pl > " +
                testPathAndFileName + " 2>&1 \"");
        do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);

        vec.clear();
        vec.emplace_back("#################################################################################");
        vec.emplace_back("# ");
        vec.emplace_back("# MariaDB Note Section");
        vec.emplace_back("# ");
        vec.emplace_back("#################################################################################");
        vec.emplace_back("# ");
        vec.emplace_back("#--> Edit the /etc/hosts file to contain the following line:");
        vec.emplace_back("     127.0.1.2   mariadb");
        vec.emplace_back("#");
        vec.emplace_back("#--> Run these commands to secure and start mariadb.");
        vec.emplace_back("#--> Commands run by install (no need to run again)");
        vec.emplace_back("#-->    This information is provided only for your knowledge");
        vec.emplace_back("groupadd   mysql ");
        vec.emplace_back("useradd -g mysql mysql ");
        //Create required directories if needed
        vec.emplace_back("# ");
        vec.emplace_back("mkdir -p '/auth_pam_tool_dir/auth_pam_tool'");
        vec.emplace_back("mkdir -p '" + itemValues.usrPath + "data/temp'");
        vec.emplace_back("mkdir -p '" + itemValues.usrPath + "data/source'");
        vec.emplace_back("mkdir -p '" + itemValues.usrPath + "run'");
        vec.emplace_back("mkdir -p '" + itemValues.usrPath + "var/log'");
        // Add required run files
        vec.emplace_back("# ");
        vec.emplace_back("cd '"       + itemValues.usrPath + "run'");
        vec.emplace_back("touch mariadb.socket ");
        vec.emplace_back("touch mariadb_pid ");
        //set permissions for mariadb directory recursively
        vec.emplace_back("# ");
        vec.emplace_back("cd '" + itemValues.rtnPath + "usr' ");
        vec.emplace_back("chown -R root:mysql mariadb ");
        vec.emplace_back("chmod -R 770        mariadb ");
        //Over ride permissions as required
        vec.emplace_back("# ");
        vec.emplace_back("cd '" + itemValues.usrPath + "'");
        vec.emplace_back("chown -R mysql:mysql data ");
        vec.emplace_back("chmod -R 770         data ");
        vec.emplace_back("chown -R mysql:mysql run  ");
        vec.emplace_back("chmod -R 770         run  ");
        vec.emplace_back("chown -R mysql:mysql var  ");
        vec.emplace_back("chmod -R 770         var  ");
        vec.emplace_back("#--> End of Commands run by install (no need to run again)");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        create_mariaDB_cnf_File(itemValues);

        vec.clear();
        vec.emplace_back("#--> #");
        vec.emplace_back("#--> Starting here you must run the commands by hand.");
        vec.emplace_back("#-->     Start the server");
        vec.emplace_back("#-->     Set the initial security");
        vec.emplace_back("#-->     Start the client");
        vec.emplace_back("#-->     Run a few test commands");
        vec.emplace_back("cd '" + itemValues.usrPath + "'");
        vec.emplace_back("#");
        vec.emplace_back("# Script the initial database");
        vec.emplace_back("#   Note 1: If you see a lot of permission errors and the script fails...");
        vec.emplace_back("#             It probably means you need to run chmod o+x on all the directories");
        vec.emplace_back("#             from root down to the " + itemValues.usrPath + " directory.");
        vec.emplace_back("#             Once permissions are set up to the mariadb directory, the rest of ");
        vec.emplace_back("#             the permissions should be ok. ");
        vec.emplace_back("#   Note 2: Don't use chmod -R o+x because that would set all the files as well");
        vec.emplace_back("#              and we only need the directories.");
        vec.emplace_back("#   Note 3: Note 1 may not be secure enough for you, In that case you must use");
        vec.emplace_back("#               Access Control Lists, and there are too many different user options ");
        vec.emplace_back("#               to create a detailed list here.");
        vec.emplace_back("# ");
        sstr command = "./scripts/mysql_install_db ";
        command.append(" --defaults-file='");
        command.append(itemValues.etcPath);
        command.append("my.cnf' ");
        command.append(" --basedir='");
        command.append(itemValues.usrPath.substr(0,itemValues.usrPath.length()-1));
        command.append("' ");
        command.append(" --datadir='");
        command.append(itemValues.usrPath);
        command.append("data' ");
        command.append(" --tmpdir='");
        command.append(itemValues.usrPath);
        command.append("data/temp' ");
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket' ");
        command.append(" --user=mysql");
        vec.emplace_back(command);
        vec.emplace_back("#");
        vec.emplace_back("# start the database ");
        vec.emplace_back("cd '" + itemValues.usrPath + "'");
        command.clear();
        command.append("./bin/mysqld_safe ");
        command.append(" --defaults-file='");
        command.append(itemValues.etcPath);
        command.append("my.cnf' ");
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket' ");
        command.append(" --user=mysql &");
        vec.emplace_back(command);
        vec.emplace_back("#");
        vec.emplace_back("#Secure the installation after starting server by running command:");
        vec.emplace_back("cd " + itemValues.usrPath);
        command.clear();
        command = "./bin/mysql_secure_installation ";
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket' ");
        vec.emplace_back(command);
        vec.emplace_back("#");
        vec.emplace_back("#After securing mariadb start the client console:");
        command.clear();
        command = "./bin/mysql ";
        command.append(" --defaults-file='");
        command.append(itemValues.etcPath);
        command.append("my.cnf' ");
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket' ");
        command.append(" -u root -p ");
        vec.emplace_back(command);
        vec.emplace_back("# ");
        vec.emplace_back("# ");
        vec.emplace_back("# When you want to shutdown run this:");
        vec.emplace_back("cd '" + itemValues.usrPath + "'");
        command.clear();
        command ="./bin/mysqladmin ";
        command.append(" --socket='");
        command.append(itemValues.usrPath);
        command.append("run/mariadb.socket'  ");
        command.append(" -u root -p shutdown ");
        vec.emplace_back(command);
        vec.emplace_back("# ");
        vec.emplace_back("# ");
        vec.emplace_back("# ");
        file_write_vector_to_file(itemValues.fileName_Notes, vec, false);

        vec.clear();
        vec.emplace_back("#");
        vec.emplace_back("# See the Installation_Notes on how to setup and start mariadb.");
        vec.emplace_back("eval \"cd '" + itemValues.rtnPath + "' \"");
        result = do_command(itemValues.fileName_Build, vec, itemValues.bScriptOnly);
        return result;
    }
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_MARIADB_H
