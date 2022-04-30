# j5c_install_from_source_001

This project "j5c_install_from_source_001" is the source code
for a program that will download and install Apache Web Server, 
and various programming languages and libraries.
  
The current list of programs:

--Dependencies-- from package manager

From source code:
============================================================================
cmake->Version     : 3.23.1   (used for building software)

libzip->Version    : 1.8.0   (used for compression)

perl5->Version       : 5.34.1   (used for web programming and more)

openssl->Version     : 1.1.1n (used for encryption)

judy->Version        : 1.0.5  (used in MariaDB)

mariadb->Version     : 10.7.X (database -- removed from this program)
just use dnf install mariadb-server instead you still get 10.3 or higher

apr->Version         : 1.7.0 (needed for Apache Web Server)

apr-util->Version    : 1.6.1 (needed for Apache Web Server)

apr-iconv->Version   : 1.2.2 (needed for Apache Web Server)

pcre2->Version       : 10.40 (needed for Apache Web Server, and PHP)

apache->Version      : 2.4.53 (Apache Web Server)

perl6->Version       : 2022.04 (programming language with virtual machine)

php->Version         : 8.1.5 (web programming scripting language)

php->Xdebug->Version : 3.1.4 (web programming debugging for php)

poco->Version        : 1.11.2 (c++ library)

postfix->Version     : 3.7.2 (mail server -- downloads, not installed)

python->Version      : 3.10.4 (a general purpose programming language)

ruby->Version        : 3.1.2 (a general purpose and web programming language)

tcl->Version         : 8.7a5 (a scripting tool)

tk->Version          : 8.7a5 (a scripting tool that does multi-platform GUIs)

wxWidgets->Version   : 3.1.6 (a cross platform GUIs using C++)

