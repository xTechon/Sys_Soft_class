You will implement, using C, pass 1 of an assembler for the machine architecture 
described in the SIC System Programmer's guide provided as reference for this course.
Your project will accept 1 command-line argument, which is a file that contains a 
SIC assembler program. Project 1 will create and output a symbol table as a result 
of analyzing the input SIC assembly file.

There are some validation checks which should be done during pass one. You should 
carefully consider the errors in the SIC assembly file input that can and should be 
discovered during pass one and when encountered in the source file,  generate an 
appropriate error message and stop the assembly process. 

For example: if an error is found with the input assembly file, you should stop 
the process of creating the symbol table and output the following:

ASSEMBLY ERROR:

<The contents of the source line of assembly which contains the error><CrLf>

Line <line #> <Description of Error Encountered><CrLf>

If the SIC assembly file is valid, then project 1 should output, its symbol table. 
For each symbol, there should be one line of output: the symbol name followed by a 
<tab> character, followed by the hexadecimal address of the symbol, followed by a <CrLf>.



I will build you project by typing make.  I will then run your project by typing project1 <filename>
where <filename> is the name of a SIC assembler file.
the turnin code for this project is:  System_Software_Project1


References:
1. The SIC System Programmer's Reference
2. The SIC Assembly Language Specification

TODOs:
DONE Basic File Reading
DONE Symbol Checking
DONE Directive Validation
- Directive Checking
- Structure to contain Symbol Table
DONE Make instruction File
- Read Instruction File
- Store Instructions into hashtable
- Location Counter tracking in hexadecimal
- Assembly instruction validation
- Assembly instruction checking
- Generate Symbol Table
- Print Symbol Table
