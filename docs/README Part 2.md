There is a settings file "Install_Settings.cfg" that must be in the same
directory as the created executable.  The executable will load the details
from the settings file to load the programs.  So by changing the settings
you can get a newer version of the programming language or server.

Also there are some path variables in the "Install_Settings.cfg" file as well.
Lets suppose you run the program once and create the files like

cmp = your company abbreviation / or your initials...
 
/usr/local/cmp/p001/usr/installed_programs...

You take the time to customize the apache web server, and get 
your web site working locally on PHP version 8.1.5.  Some time 
later PHP comes out with version 8.3.2 and you want to see if 
your web site will run with it or not.

You customize the Install_Settings.cfg to load the new version 
of PHP, and change the number from stack number from 001 to 002.  
You now have the same initial setup with the new PHP version.  
You can copy your apache config files over to p002, and your 
web code, and test out your new version without disrupting your 
currently running version.
 
/usr/local/cmp/p002/usr/installed_programs...

And that is why I created this utility tool in the first place to be able to 
check my website against newer versions of PHP, Apache, and MariaDB without a 
great deal of trouble.

Obviously to fully setup a website from scratch requires a lot of knowledge of 
permissions, and security.  So probably very few people will be able to use this
to create full production environments, but it lays the ground work, that the rest
could be done through scripts built by running commands by hand first and adding to 
a script file, then saved.  So each later time it gets easier.

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

#These are sample commands to get to the right path

#cd /home/yournamehere/pathto/j5c_install_from_source_001

#cd /home/jay/git/j5c_install_from_source_001

#Create the executable
/usr/bin/c++             \\
  ./source/j5c_date.h    \\
  ./source/j5c_date.cpp  \\
  main.cpp               \\
  -std=c++17             \\
  -o Install_From_Source           

#You would have to replace J5C with your company abbreviation

mkdir /home/J5C_Install \
cp Install_From_Source    /home/J5C_Install \
cd settings \
cp Install_Settings.cfg   /home/J5C_Install \
vim /home/J5C_Install/Install_Settings.cfg \
#in Install_Settings.cfg replace XXX to your company abbreviation \
cd /home/J5C_Install \
#
#When you have made all the changes above
#and executed the commands, you are 
#ready to run the code below to install all the programs.
# \
./Install_From_Source \

#Using the code above the following directories will be made
#except J5C would be your company initials.

/home/J5C/stg   -- stage all the dowloads here, 
                --  if you re-run the program, 
                --  it won't download again 
                --  unless you change versions of a program
/home/J5C/p001  -- This is your first version, 
                -- you can increment the next time to compare different programs
                -- there are also notes in here that are helpfull
                -- and the Installation_Results.txt file that gives an overview
                --   of the total installtion progress and results.
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

