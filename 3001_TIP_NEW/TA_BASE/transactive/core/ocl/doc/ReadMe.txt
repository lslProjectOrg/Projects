Oracle Class Library for C++ 3.01.0.6
Copyright 1999-2005, Core Lab. All Rights Reserved
--------------------------------------------------

Oracle Class Library (OCL) provides native connectivity to the Oracle
database server. OCL uses Oracle Call Interface (OCI) directly. OCI is
low-level native application programming interface to access to Oracle.
Using OCI in Oracle Class Library allows to create lightweight and fast
applications working with Oracle. Oracle Class Library encapsulates OCI
calls in high-level classes that allows to hide the complexity of using
OCI directly and keep performance and all abilities of native routines.
With OCL you can use the power and flexibility of SQL in your application
programs without any restrictions.

OCL contains classes to control connection, execute SQL statements, store
and process result rows of queries and some common classes useful for
developing database applications. All classes have intuitive, easy to use
interface. 

OCL is written with ANSI C++ and uses Standard C++ Library only that
allows you to port your application easily to another platform. 
Oracle Class Library provides easiness in using from Pro*C/C++ and power
of Oracle Call Interface. 

OCL allows you to 

 - design and develop cross-platform highly customized database applications  
 - improve performance of data processing in your applications  
 - compile the same application for different platforms  
 - automatically convert between Oracle internal datatypes and high-level
   language datatypes  
 - fetch rows quickly  
 - cache result rows in memory to process them later  
 - use arrays as input and output program variables  
 - call stored procedures easily  
 - handle errors  
 - use LONG fields in the database  

To sum it up, OCL is a full-featured tool that supports a professional
approach to develop database applications. 


Compatibility
-------------

OCL supports Oracle 10g, 9i, 8i, 8.0 and 7.3, including Personal and Lite.
OCL 3.00 includes library for Microsoft Visual C++ 7.1, 7.0 and 6.0,
Borland C++ Builder 6 and 5, Kylix 3 for C++ and GNU compiler.
OCL was compiled with GNU C++ compiler version 2.91 and later.


Using
-----

To support OCL in your project you need

- Add OCL header and OCI header folders
- Add OCL library and OCI library
- Insert OCL header #include "ocl.h"

OCL headers: ocl\include
OCL library: MSVC: ocl\lib\msvc\ocl.lib
             BCB: ocl\lib\bcb\ocl.lib
             gcc: libocl.a or libocl.so in 
                 gcc3.4: ocl\lib\gcc34\ 	
                 gcc3.X: ocl\lib\gcc3\ 
                 gcc2.X: ocl\lib\gcc2\

OCI headers: ocl\oci\include or 
             MSWIN: <ORACLE_HOME>\oci\include
             Linux: $ORACLE_HOME/rdbms/demo
OCI library: MSVC: ociw32.lib or oci.lib in ocl\oci\lib\msvc\ or
                   <ORACLE_HOME>\oci\lib\msvc\
             BCB: For Oracle servers 8.17 and earlier add libraries ociw32.lib 
                  or oci.lib from ocl\oci\lib\bcb\ or <ORACLE_HOME>\oci\lib\bc\
                  folders.

                  For later Oracle servers use implib.exe Borland utility to
                  create .lib file from existing .dll. This goal can be achived
                  using the following command line:
                  <BCB>\bin\implib.exe <ORACLE_HOME>\oci\lib\bc\oci.lib <ORACLE_HOME>\bin\oci.dll

             gcc: $ORACLE_HOME/lib/libclntsh.so



Trial Version
-------------
If you are using OCL trial version you can fetch only first five columns.
In all other cases OCL trial does not differ from OCL full version.

OCL trial version for Visual C++ was compiled with multithreaded static
C runtime (/MT compiler option).


--------------------
http://www.crlab.com
ocl@crlab.com
