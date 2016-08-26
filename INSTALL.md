*******************************************************************************
*                                                                             *
*                         Rank Maximal Matchings                              *
*                                                                             *
*                          UNIX INSTALLATION                                  *
*                                                                             *
*******************************************************************************


In this file I use <LEP> to denote the path name where you have
extracted the LEP packet. Default is the directory <LEDA>/LEPS right
below the LEDA main directory. If you have extracted the RMM
package in that directory the directory <LEP>/rmm (here I use the name 
rmm for the directory, but it might be something like rmm-0.1 based on 
the version of the package) should contain the following files and 
subdirectories:

README                    information about the LEP
INSTALL                   this file
CHANGES  (please read !)  most recent changes
FIXES                     bug fixes since last release
Makefile                  make script
make.config               make configuration settings
configure                 configuration command
doc/                      user manual
incl/                     include directory
src/                      source files
test/ or prog/            test programs
demo/                     demo programs


1. CONFIGURATION
----------------

   a)  Go to the directory <LEP>/rmm

   b)  Type: configure

       where <cc> is the name (or command) of your C++ compiler
       and the optional second parameter defines the path where
       the needed LEDA package is installed 

       
   c)  For deviations from the default retype configure with
       additional information:
       
       Example: CXX=g++ LEDAROOT=/Somepath/LEDA/ LINKING_MODE=static configure

In case you have any problems, try first to go to the confdir directory
and call "make" and then do the previous steps.

2. COMPILING
-------------

   Now just type "make"


3. INSTALLATION 
----------------

a) Header Files
  
   All LEDA header files are contained in "<LEDA>/incl/LEDA".
   All LEP header files are contained in "<LEP>/rmm/incl"

   You can also copy the include files into one of the global
   include directories, e.g.:

   AT&T CC:  cp -r <LEP>/rmm/incl /usr/include/CC

   GNU g++:  cp -r <LEP>/rmm/incl /usr/local/lib/g++-include     

   Or you can use the -I flag of the compiler as described below:

   CC (g++) -I<LEDA>/incl -I<LEP>/rmm/incl \
            -L<LEDA> ...

   If you have access to the LEDA installation tree and you extracted
   the LEP rmm inside the directory <LEDA>/LEPS you can copy
   the header files to the default include path 

   <LEDA>/incl/LEP/rmm

   by the command

	make lepintegrate

   After this you can compile programs which use LEDA and the LEP
   by the simpler command:

   CC (g++) -I<LEDA>/incl -L<LEDA> ...



5. USER MANUAL
--------------

   You can grab the user manual and the implementation documents from
   the www server where you obtained this package.

   The manual is also contained in the doc directory.	
