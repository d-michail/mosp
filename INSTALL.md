*******************************************************************************
*                                                                             *
*                            MOSP library                                     *
*                                                                             *
*                          UNIX INSTALLATION                                  *
*                                                                             *
*******************************************************************************


In this file I use <LEP> to denote the path name where you have
extracted the LEP packet. Default is the directory <LEDA>/LEPS right
below the LEDA main directory. If you have extracted the RMM
package in that directory the directory <LEP>/mosp (here I use the name 
mosp for the directory, but it might be something like libmosp-0.1 based on 
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

   a)  Go to the directory <LEP>/mosp

   b)  Type: configure

       where <cc> is the name (or command) of your C++ compiler
       and the optional second parameter defines the path where
       the needed LEDA package is installed 

       
   c)  For deviations from the default retype configure with
       additional information:
       
       Example: CXX=g++ LEDAROOT=/Somepath/LEDA/ LINKING_MODE=static configure

In case you have any problems, try first to go to the confdir directory
and call "make" and then do the previous steps.

1.1 NOTE FOR LEDA VERSION 5.0 OR LATER
--------------------------------------
Starting from version 5.0, LEDA has a different include structure, which might 
prevent the LEP from beeing compiled.

The configure script on unix platforms compiles the code with the flag
LEDA_GE_V5 in case LEDA-5.0 or newer is detected.

For older code there is another alternative, to set HEADER_COMPAT to yes
when calling configure.
Execute configure as:

LEDAROOT=/path/to/LEDA/5.0 <other options> HEADER_COMPAT=yes configure

and the old style include structure will be compiled as well.

2. COMPILING
-------------

   Now just type "make"


3. INSTALLATION 
----------------

a) Header Files
  
   All LEDA header files are contained in "<LEDA>/incl/LEDA".
   All LEP header files are contained in "<LEP>/mosp/incl"

   You can also copy the include files into one of the global
   include directories, e.g.:

   AT&T CC:  cp -r <LEP>/mosp/incl /usr/include/CC

   GNU g++:  cp -r <LEP>/mosp/incl /usr/local/lib/g++-include     

   Or you can use the -I flag of the compiler as described below:

   CC (g++) -I<LEDA>/incl -I<LEP>/mosp/incl \
            -L<LEDA> ...

   If you have access to the LEDA installation tree and you extracted
   the LEP mosp inside the directory <LEDA>/LEPS you can copy
   the header files to the default include path 

   <LEDA>/incl/LEP/mosp

   by the command

	make lepintegrate

   After this you can compile programs which use LEDA and the LEP
   by the simpler command:

   CC (g++) -I<LEDA>/incl -L<LEDA> ...

4. Template Makefile
--------------------

After compiling the LEP you can find in LEP/mosp/doc/makefiles a template
makefile for compiling programs using the same flags.

Simply copy the makefile to the directory containing your source.
You must then open the makefile and change two things: a) the variable
PROGS must contain the names of the programs that you want to build,
b) the makefile has to include the file make.config located in the
root directory of the LEP.

Details are also contained inside the makefile.

5. USER MANUAL
--------------

   You can grab the user manual and the implementation documents from
   the www server where you obtained this package.

   The manual is also contained in the doc directory.	




*******************************************************************************
*                                                                             *
*                         Rank Maximal Matchings                              *
*                                                                             *
*                          WINDOWS INSTALLATION                               *
*                                                                             *
*******************************************************************************


6. WINDOWS - BORLAND C++ BCC32 (STATIC)
-----------------------------------

The LEP can be compiled for use with the LEDA and Borland bcc32
compiler. LEDA must be compiled as static.

a) install LEDA with static variables, LEDAROOT=path to leda
   The LEDAROOT directory should contains some or all of these files

	libL.lib      basic library
  	libG.lib      graph library
  	libP.lib      d2-geo library
  	libD3.lib     d3-geo library
  	libGeoW.lib   GeoWin library
  	libW.lib      window library

b) Setting the Environment Variables for Borland C++:

    * Make sure that the directory of bcc32.exe is contained in your path. bcc32.exe is 
	  located in the \bin directory of your compiler, 
	  e.g., in c:\borland\bcc55\bin
    * Make sure that there is a file bcc32.cfg in the \bin directory of your compiler 
	  which sets the compiler options for the include and lib paths to the correct 
	  directories. On the author's machine this means adding the lines
      -I"c:\borland\bcc55\include"
      -L"c:\borland\bcc55\lib"
      to bcc32.cfg.
    * Make sure that there is a file ilink32.cfg in the \bin directory of your compiler which 
	  sets the linker options for the lib path to the correct directory. On the author's 
	  machine this means adding the line
      -L"c:\borland\bcc55\lib" -L"c:\borland\bcc55\lib\PSDK"
      to ilink32.cfg.

c) Go to the <LEP>/mosp directory and execute make_bcc_static.bat
   To clean the sources run 
   
   make allclean

   or simply

   make clean

d) To compile your program use something like

   bcc32 -P -I<LEP>/mosp/incl -I%LEDAROOT%/incl \
            -L<LEP>/mosp -L%LEDAROOT% \
			yoursource.c libmosp.lib libg.lib libl.lib ...

7. WINDOWS - BORLAND C++ BCC32 (DYNAMIC)
----------------------------------------

The LEP can be compiled for use with the LEDA and Borland bcc32
compiler. LEDA must be as dynamic library (dll).

a) install LEDA and set LEDAROOT=path to leda
   The LEDAROOT directory should contain the following files

	leda.dll
	leda.lib
	libGeoW.lib

   Add to the PATH variable the LEDAROOT path, so that the leda.dll can 
   be found, or copy leda.dll to some other appropriate path.

b) Setting the Environment Variables for Borland C++:

    * Make sure that the directory of bcc32.exe is contained in your path. bcc32.exe is 
	  located in the \bin directory of your compiler, 
	  e.g., in c:\borland\bcc55\bin
    * Make sure that there is a file bcc32.cfg in the \bin directory of your compiler 
	  which sets the compiler options for the include and lib paths to the correct 
	  directories. On the author's machine this means adding the lines
      -I"c:\borland\bcc55\include"
      -L"c:\borland\bcc55\lib"
      to bcc32.cfg.
    * Make sure that there is a file ilink32.cfg in the \bin directory of your compiler which 
	  sets the linker options for the lib path to the correct directory. On the author's 
	  machine this means adding the line
      -L"c:\borland\bcc55\lib" -L"c:\borland\bcc55\lib\PSDK"
      to ilink32.cfg.

c) Go to the <LEP>/mosp directory and execute make_bcc_dll.bat
   To clean the sources run 
   
   make allclean

   or simply

   make clean

d) To compile your program use something like

   bcc32 -P -DLEDA_DLL -I<LEP>/mosp/incl -I%LEDAROOT%/incl \
         -L<LEP>/mosp -L%LEDAROOT% \
	  yoursource.c libmosp.lib leda.lib

8. Optimization Options and Configuration - IMPORTANT
-----------------------------------------------------

8.1 GCC and LEDA
----------------

If you are using GCC version 3.4 or newer in combination with LEDA 5.0.1
or older do not use the code with the following optimization flags

-fschedule-insns
-fschedule-insns2

at the author's PC, the resulting code contains a memory leak.
This is due to incompatibility between GCC and LEDA's memory manager.

Note that the flags -O2 and -O3 use these optimization flags
and therefore the flags -fno-schedule-insns and -fno-schedule-insns2
must also be used. The configure script automatically enables these
if required.

Moreover, when using GCC 4.x and LEDA do not use strict aliasing, again
in the author's machine unpredictable results occur. The configure script
automatically enables the option -fno-strict-aliasing if this scenario is
detected.

8.2 Checking Disabled
---------------------

The configure script offers a flag

--disable-checking

which compiles the LEP, with any internal check disabled
by adding the flag -DLEDA_CHECKING_OFF, the same flag
that LEDA uses to disable any preconditions' or invariants'
checking.

Note that no check is performed in such a case.

8.3 Configuration Flags
-----------------------

All configuration flags are contained in the header file config.h .


- Dimitrios Michail 
