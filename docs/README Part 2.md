When running install from source there is a settings file
"Install_Settings.cfg" that must be in the same directory 
as the running executable. Changing the settings you can 
get a newer version of the programming language or server.

Also there are some path variables in the "Install_Settings.cfg" 

cmp = your company abbreviation / or your initials...
/usr/local/cmp/p001/usr/installed_programs...

Why was the project made?
You take the time to customize the apache web server, and get 
your web site working locally on PHP version 8.1.5.  Some time 
later PHP comes out with version 8.3.2 and you want to see if 
your web site will run with it or not.

You customize the Install_Settings.cfg to load the new version 
of PHP, and change the stack number from 001 to 002. You now 
have the same initial setup with the new PHP version. You can 
copy your apache config files over to p002, and your web code, 
and test out your new version without disrupting your currently 
running version.

Your new version would be here...
/usr/local/cmp/p002/usr/installed_programs...

And that is why I created this utility tool in the first place, to be able to 
check my website against newer versions of PHP, Apache, and MariaDB without a 
great deal of trouble.

Obviously to fully setup a website from scratch requires a lot of knowledge of 
permissions, and security.  So probably very few people will be able to use this
to create full production environments, but it lays the ground work, that the rest
could be done through scripts built by running commands by hand first and adding to 
a script file, then saved.  So each later time it gets easier.

/r
Jay Carlson (Cheers!)

