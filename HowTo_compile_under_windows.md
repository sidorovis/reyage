# Introduction #

Please install mingw (last one).
  * http://www.mingw.org/
Please get QT under windows. Read this to get source code
  * http://qtnode.net/wiki?title=Qt4_with_Visual_Studio#Download_and_extract_the_patch_allowing_Qt_to_be_compiled_with_Visual_Studio_compilers
Get source code of reyage from:
http://reyage.googlecode.com/svn/trunk/sources/

> Install mingw.
> Compile QT.
> Place source code into the folder named 'reyage'.

# Using #

  * Run cmd. (cntr + r)
  * Run next command: 'set path=%path%;c:\MinGW\bin;c:\MinGW\lib;c:\Mingw\include;c:\Qt\4.3.1\bin;c:\Qt\4.3.1\lib;c:\Qt\4.3.1\include'
  * Go to path with source code.
  * Put 'qmake -project'
  * Put 'qmake reyage.pro'
  * Put 'make release'

That's all.