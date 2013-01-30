ciniread
========

A small library for reading INI style configuration files.

Use the included Codeblocks IDE project file to build the example.

Depends on C standard libraries.  
Developed on Linux X86_64 using GCC 4.7.2


General Information
===================
To use the library please include libs/ciniread.h in your source.
Otherwise follow the appropriate steps for your environment.

# Indicates a comment.  Comments must be on their own line and the 
# character has to be the first character on the line itself.
Comments are skipped when reading the file.
	
[section] Starts a section of related keys
key=value the = sign is used to separate the key value pairs.

Keys and values are read in as strings it is the users responsibility to
convert the values to whatever internal format they need once the data
has been read in.

ciniread should handle arbitrary blank lines with out a problem.

Example ini file:
# this is a sample ini file
# sample section is used to setup some things in sample program
[sample]
key1=test1
key2=test2
key3=test3
# sample2 is sample2
[sample2]
samp1=value1
samp2=value2
