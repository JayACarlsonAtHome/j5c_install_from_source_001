# j5c_install_from_source_001

This project "j5c_install_from_source_001" is the source code for a program
that will download and install Apache Web Server, and various programming languages and libraries.
  
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

There is a settings file "Install_Settings.cfg" that must be in the same
directory as the created executable.  The executable will load the details
from the settings file to load the programs.  So by changing the settings
you can get a newer version of the programming language or server.

Also there are some path variables in the "Install_Settings.cfg" file as well.
Lets suppose you run the program once and create the files like

cmp = your company abbreviation / or your initials...
 
/usr/local/cmp/p001/usr/installed_programs...

You take the time to customize the apache web server, and get your web site working
locally on PHP version 8.1.5.  Some time later PHP comes out with version 8.3.2 and 
you want to see if your web site will run with it or not.

You customize the Install_Settings.cfg to load the new version of PHP, and change
the number from 001 to 002.  You now have the same initial setup with the new
PHP version.  You can copy your apache config files over to p002, and your web
code, and test out your new version without disrupting your currently
running version.
 
/usr/local/cmp/p002/usr/installed_programs...

And that is why I created this utility tool in the first place to be able to 
check my website against newer versions of PHP, Apache, and MariaDB without a 
great deal of trouble.

Obviously to fully setup a website from scratch requires a lot of knowledge of 
permissions, and security.  So probably very few people will be able to use this
to create full production environments, but it lays the ground work, that the rest
could be done through scripts built by running commands by hand first and adding to 
a script file, then saved.  So each later time it gets easier.

On a side note, if you wanted to learn some C++, the code is dependable.
The code may not be optimized to the level it could be, but examine this code for 
a second...

//to allow the code sample to work, 
//  you need the following line...
using sstr = std::string;

sstr lowerCaseString(sstr& some_value)
{
    // unicode lower case conversions require
    // very specialized code, and this is not it
    // but it will handle the english words that
    // we need for this program.

    sstr result;
    int oneChar;
    for (auto idx = 0ul; idx < some_value.length(); ++idx)
    {
        if (std::isupper(some_value.at(idx)))
        {
            oneChar =  std::tolower(some_value.at(idx));
        }
        else
        {
            oneChar = some_value.at(idx);
        }
        result.append({static_cast<char>(oneChar)});
    }
    return result;
}

//the faster version of lowercase

sstr lowerCaseString(sstr& some_value)
{

    // unicode lower case conversions require
    // very specialized code, and this is not it
    // but it will handle the english words that
    // we need for this program.

    auto len = some_value.length();
    char strChar[len+1];
    strcpy(strChar, some_value.c_str());
    for (auto idx = 0ul; idx < len; ++idx)
    {
        strChar[idx] =  std::tolower(strChar[idx]);
    }
    sstr result {strChar};
    return result;
}

Lets just say that this code takes 30 milli seconds to run.
But compiling MariaDB takes 40 minutes. Optimizing this code to take 
27 milli seconds isn't going to matter a hill a beans in the grand scheme of things.

So all the code works.  Some of it could be better.  But I could keep working to 
improve it forever, and not release it, and it would help no-one.  So I am releasing
it now in the hopes that some one can use it and appreciate it.

Compiling Notes....
This was made to be compiled and run on the following operating systems:
  1. CentOS 7.3  and higher
  2. Fedora 27   and higher
  3. Linux Mint 18.3 and higher
  4. Red Hat 7.3 and higher

I will leave it up to a mac enthusiast to convert to mac.

The project was built with JetBrains CLion.
So that would be your easiest way to build the executable on your system.
But if you don't have that...

/usr/bin/c++ ./source/j5c_date.h ./source/j5c_date.cpp main.cpp -std=c++17 -o Install_From_Source           
should build the executable as "Install_From_Source" on your system.

If you use JetBrains CLion to compile the project, it will 
create the output file  "Install.sh" in the "cmake-build-debug" directory

I used different files names so you could compile them both ways and not conflict.

So to finish installation and start running...

su - root 

# -- These are sample commands to get to the right path
#cd /home/yournamehere/pathto/j5c_install_from_source_001
#cd /home/jay/git/j5c_install_from_source_001

# -- Create the executable
/usr/bin/c++             \
  ./source/j5c_date.h    \
  ./source/j5c_date.cpp  \
  main.cpp               \
  -std=c++17             \
  -o Install_From_Source           

# You would have to replace J5C with your company abbreviation
mkdir /home/J5C_Install
cp Install_From_Source    /home/J5C_Install
cd settings
cp Install_Settings.cfg   /home/J5C_Install
vim /home/J5C_Install/Install_Settings.cfg
# in Install_Settings.cfg replace XXX to your company abbreviation
cd /home/J5C_Install
#
# When you have made all the changes above
#  and executed the commands, you are 
#  ready to run the code below to install all the programs.
#
./Install_From_Source

# Using the code above the following directories will be made
#  except J5C would be your company initials.

/home/J5C/stg   -- stage all the dowloads here, 
                --  if you re-run the program, 
                --  it won't download again 
                --  unless you change versions of a program
/home/J5C/p001  -- This is your first version, 
                --  you can increment the next time to compare different programs
/home/J5C/p001\bld -- all the building results will be here
/home/J5C/p001\etc -- all the configuration files will be here
/home/J5C/p001\src -- all the source files will be here
/home/J5C/p001\usr -- all the compiled output files / run files will be here.
/home/J5C/p001\tls -- for certificates and other encryption stuff
/home/J5C/p001\www -- for web files and folders to be used by apache web server

Reading and implementing many of the notes that are logged will
help with further setup and implementations.

/r
Jay Carlson (Cheers!)

