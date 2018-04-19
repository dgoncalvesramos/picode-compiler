# What is it ?
  Picode compiler is an asm compiler for picode fake-invented asm language. It is a school compilation project developed during my           second year at ENSIIE engineering school.  

# Compilation
  Use `$ make` in /src   

# Usage
 `$ ./bin/aspico [options] test_files/file.asm`    
  
  Options:  
  `-d` Generates a dump human readable file of the asm instructions.  
  `-o <file_name>` Allows you to rename the picode binary compiled file (default name is picode).  
  `-t` Prints the symbol table in your terminal.  
  `-h` Prints a textual help in your terminal.  
  
 # How to test your picode binary compiled file ?
   You can test your picode binary compiled file with the vm picode give in the /vmpicode folder.  
   
   ## Usage :
   `$ ./vmpicode your_binary_file`  
   
   
   I do not own the source code of the vmpicode.   
