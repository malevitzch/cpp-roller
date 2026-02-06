## cpp-roller
A simple program designed to bundle multi-file C++ programs into a single file, allowing
them to be submitted on platforms such as codeforces.

# Assumptions
The cpp-roller program makes certain assumptions about how your code is written.
It only "rolls" the include statements which use quotation marks (that's how it differentiates
system headers from file includes). This is just a text-based tool, so any conditional incldues,
such as those inside an ifndef clause, might cause unexpected behavior and should be avoided.
