## cpp-roller
A simple program designed to bundle multi-file C++ programs into a single file, allowing them to be submitted on platforms such as codeforces.
It is intended to be used together with custom algorithm libraries, reducing the reliance on copy-pase "presets" that are often used in competitive programming.

# Assumptions
The cpp-roller program makes certain assumptions about how your code is written.
It only "rolls" the include statements which use quotation marks (that's how it differentiates
system headers from file includes). It also groups angle bracket includes together and puts them
at the beginning of the output file, so using odd headers that you want included in some files but
not in others might have unforeseen consequences. This mostly applies to naming convention clashes.

# Known issues and limitations
- Includes that are generated as a result of macro expansion might break in certain cases due to the text-based nature of this tool. In general, it is recommended to have all include directives be top-level

# Usage
The basic way to use the program is by running 
```bash
cpproll <sources>
```
Below is a complete list of available options:
- ```-o <filename>``` sets the name for the output file. 
If left unspecified, the name is rolled.out
- ```-I <pathstring>``` sets the include directories (pathstring is a collection of paths, separated by either colons or semicolons depending on the system)
- ```-v``` prints the program version
- ```-h``` prints the help message
- ```-f``` forces the program to overwrite the output even if the file already exists
- ```-n``` forces the program to finish execution if output would overwrite an existing file
