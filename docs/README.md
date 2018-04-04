# j5c_install_from_source_001

This project "j5c_install_from_source_001" is the source code for a program
that will download and install programming languages and servers.  
The current list of programs is:

--Dependencies-- from package manager

From source code:
===========================

Perl-5.26.1

Openssl 1.1.0g

Mariadb-10.3.5

Apr-1.6.3

Apr-util-1.6.1

Apr-iconv-1.2.2

Pcre-8.41

Pcre2-10.30

Apache-2.4.29

Perl6-2018.01

Php-7.2.3 (with optional xdebug)

Poco-1.9.0

Postfix-3.2.4

Python-3.6.4

Ruby-2.5.0

Tcl-8.7a1

Tk-8.7a1


There is a settings file "Install_Settings.cfg" that must be in the same
directory as the created executable.  The executable will load the details
from the settings file to load the programs.  So by changing the settings
you can get a newer version of the programming language or server.

Also there are some path variables in the "Install_Settings.cfg" file as well.
Lets suppose you run the program once and create the files like

cmp = your company abbreviation / or your initials...
 
/usr/local/cmp/p001/usr/installed_programs...

You take the time to customize the apache web server, and get your web site working
locally on PHP version 7.2.2.  Some time later PHP comes out with version 7.4.6 and 
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

I was going to do MacOSX but there are enough differences
and the fact that MacBook Pros really don't have enough resources for 
it to really matter, and I don't have a Mac Desktop. So I haven't finished
and maybe will never finish that one.

The project was built with JetBrains CLion.
So that would be your easiest way to build the executable on your system.
But if you don't have that...

/usr/bin/c++ ./source/j5c_date.h   \
             ./source/j5c_date.cpp \ 
              main.cpp             \
             -std=c++11            \
             -o Install_From_Source
             
should build the executable as "Install_From_Source" on your system.

Using JetBrains CLion compile the project.
then copy the "Install.sh" file from the "cmake-build-debug" 
directory to the directory you want to run the program from.
cd ../settings
copy the Install_Settings.cfg file to the same directory as above.
cd to the executable path...
vim Install_Settings.cfg ... change as desired.
run the executable ./Install.sh
and two to six hours later depending on the speed of your hardware
you will have your system baseline setup.





