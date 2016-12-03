# turtleSHELL

## Features:
*   Forks and executes commands!
*   Parses multiple commands on one line!
*   Redirects using single(>, <) or multiple(< >, > <) !
*   Pipe-able!

## Attempted:
* tried to get autofill (tabbing to fill in words)
* didn't have time to implement >> and << redirections 

## Bugs:
* Only works with single spaces within one command (ls -l), not with (ls     -l)
* But can have trailing and leading spaces for any command(     ls -l       ;       echo hello     )

##NOTES BEFORE USE:
* Put only one space within a command

## Files and What they Do:

### trex.c
```
/**********************************************************************
PROMPT: gets current directory and user. prints out the appropriate 
command line prompt depending on whether the hostname is null or not.  
Takes as input: none  
Returns: none 
***********************************************************************/  

/**********************************************************************
CD: takes in a path string, modifies the string if needed and changes 
the directory to the path string.  
Takes as input: input string (path)  
Returns: int (returns 0 upon completion) 
***********************************************************************/

/*********************************************************************
READIN: reads (fgets) input from the terminal into the string buffer 
Takes as input: input string buffer 
Returns: none 
*********************************************************************/

/*********************************************************************
TRIM: trims white spaces
Takes as input: char *
Returns: char *
*********************************************************************/

/********************************************************************
PARSE: takes in string and parses using ";" then sends each individual 
parsed line to exec_1com. 
calls on exec_1com.  
Takes as input: input string  
Returns: none 
********************************************************************/

/*******************************************************************
PETERPIPER: separates the string by '|', adjusts redirection, and 
executes after forking
calls on exec_1com
Takes as input: input string  
Returns: none 
********************************************************************/

/*******************************************************************
REDIRECT: Splits input string on both ">" and "<", and decides the 
sequence. conducts the appropriate redirection (including opening files),
and executes 
calls on exec_1com
Takes as input: input string  
Returns: none 
********************************************************************/

/*******************************************************************
EXEC: deals with cd, exit, and calls on execvp to execute
commands
Takes as input: takes in pointer to array of pointers, two ints 
(input and output file descriptors)  
Returns: none 
********************************************************************/

/*******************************************************************
EXEC_1COM: deals with exec cases with redirection and pipes as well
as just a simple command.
calls on redirect, peterpiper, and exec.
Takes as input: takes in pointer to array of pointers, two ints 
(input and output file descriptors)  
Returns: none 
********************************************************************/
```
### main.c:
```
/*******************************************************************
MAIN: runs the program. gets the shell party started.
Takes as input: none  
Returns: none 
********************************************************************/
```
