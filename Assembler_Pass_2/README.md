You will implement, using C, pass 2 of an assembler for the machine architecture described in the SIC System Programmer's guide provided as reference for this course.  Your project will accept 1 command-line argument, which is a file that contains a SIC assembler program. Project 2 will create and output an object file in the format described in the SIC Object File Format document.  YOUR OBJECT FILE MUST SUPPORT RELOCATION.

Remember that to generate SIC object code, we will need to make two passes through the assembly file. Pass one generated the symbol table and pass two uses the symbol table and op codes to generate the object file.   

There are some validation checks which should be done during pass two (checks  which really cannot be done during pass one). You should carefully consider the errors in the SIC assembly file input that can and should be discovered during pass two and when encountered in the source file,  generate an appropriate error message and stop the assembly process. There are errors which pass two should be able to detect and report (all of which should HALT the assembly process and generate an error message). 

For example: if an error is found with the input assembly file, you should stop the process of creating the symbol table and output the following:

ASSEMBLY ERROR:

<The contents of the source line of assembly which contains the error><CrLf>

Line <line #> <Description of Error Encountered><CrLf>

If the SIC assembly file is valid, then project 2 should create and write the appropriate object file

---------------------------------------------------------------------------------------------------------------

I will build you project by typing make.  I will then run your project by typing project2 <filename>

where <filename> is the name of a SIC assembler source code file.

If assembly is completed with no errors, the object file that is created should be the name of the input file with .obj added to the end of the filename.

the turnin code for this project is:  System_Software_Project2

You need to shar your .c and makefile and documentation.txt  and turnin the sharfile to me. 
Additionally, I expect you to include a documentation.txt file in your sharfile. Discuss your design choices. What errors in the SIC assembly source are you able to detect?

Caveats:
1. If the SIC program assembles correctly, I should see NO output to the screen from pass2. The object file should be creating using the naming convention in the project specification.

2. If the SIC program has an error which prevents assembly, the object file should NOT be created. Additionally, I should only see an error message output to the screen with the error that occurred.

References:

1. The SIC System Programmer's Reference
2. The SIC Assembly Language Specification
3. The SIC Object File Format Specification (SCOFF)