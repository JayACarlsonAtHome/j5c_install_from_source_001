cmake_minimum_required(VERSION 3.12)

project(j5c_install_from_source_001)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
        source/j5c_date.h
        source/j5c_date.cpp

        source/const_expressions.h
        source/constants.h
        source/enums.h
        source/type_abbreviations.h
        source/structs.h

        source/function_declarations.h
        source/Time_Functions/time_functions.h
        source/String_Functions/string_functions.h
        source/Permissions/permissions.h
        source/Dependencies/dependencies.h
        source/File_Functions/file_functions.h
        source/Commands/commands.h

        source/Apache/apache_install.h
        source/CMake/cmake.h
        source/Libzip/libzip.h
        source/Logging/logging.h
        source/MariaDB/mariadb.h
        source/Perl/perl5.h
        source/Perl/perl6.h
        source/PHP/php.h
        source/Python/python.h
        source/Poco/poco_Install.h
        source/PostFix/postfix.h
        source/Ruby/ruby.h
        source/TCL_TK/tcl.h
        source/TCL_TK/tk.h
        source/TLS_SSL/tls_ssl.h
        source/wxWidgets/wxwidgets.h

        main.cpp
        source/Judy/judy.h)

add_executable(Install.sh ${SOURCE_FILES})

