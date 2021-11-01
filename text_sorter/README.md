# Text Sorter

* [Description](#Description)   
* [Installation](#Installation)    
* [Usage](#Usage)    
* [Example of usage](#Example-of-usage)   


## Description

This program sorts your text file and writes results to another file.   
At first it writes the text sorted lexico-graphically by begining of strings.   
Next it writes the text sorted by line endings.  
And finally the program writes the file in its original form.



## Installation

To use this program download source files and compile them by typing    
`gcc main.c TextSort.c -o TextSort`     
in Linux terminal terminal. 

> __Note:__  
> This programs uses UNIX system calls and can only work on UNIX-like operation systems.



## Usage

To use this program: 
1. Run the program by typing.   
`./TextSort`   
2. Enter the input file path. 
3. Enter the name of output file.



## Example of usage

    $ gcc main.c TextSort.c -o TextSort
    $ ./TextSort
    Enter the input file path:
    text.txt
    Enter the output file path:
    output,txt

It must looks like **[this](example/ "Example of results")**.



# Good luck!
