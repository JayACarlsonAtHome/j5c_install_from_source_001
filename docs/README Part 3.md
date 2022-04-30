
# Compiling Notes....
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

