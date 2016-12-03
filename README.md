# turtleSHELL

## Features:
*   Forks and executes commands!
*   Parses multiple commands on one line!
*   Redirects using > and <!
*   Pipe-able!

## Attempted:
* trying to get autofill (tabbing to fill in words)
* attempted to do all of the redirections (but did not get to all of them :( )

## Bugs:
* To use multiple commands, there must be no spaces between each command and the semi-colon. For example, ls -l;echo hello;echo bye should work fine but ls -l; echo hello; echo bye will not work.
* Sometimes you have to type "exit" multiple times to exit. Sometimes you don't.

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

/**********************************************************************
DEBLANK: removes extra white space from around a given string  
Takes as input: input string  
Returns: "trimmed" output string 
**********************************************************************/

/*********************************************************************
READIN: reads (fgets) input from the terminal into the string buffer 
Takes as input: input string buffer 
Returns: none 
*********************************************************************/

/********************************************************************
DECISIONMAKER: takes in string and after searching for certain 
characters such as '<', '>', or '|' determines the appropriate action 
to take. calls on exec.  
Takes as input: input string  
Returns: none 
********************************************************************/

/*******************************************************************
PETERPIPER: separates the string by '|', adjusts redirection, and 
executes after forking  
Takes as input: input string  
Returns: none 
********************************************************************/

/*******************************************************************
REDIRECTR: Splits input string on ">", conducts the
appropriate redirection (including opening files), and executes 
Takes as input: input string  
Returns: none 
********************************************************************/

/*******************************************************************
REDIRECTL: Splits input string on "<", conducts the
appropriate redirection (including opening files), and executes
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
```
### main.c:
```
/*******************************************************************
MAIN: runs the program. gets the shell party started.
Takes as input: none  
Returns: none 
********************************************************************/
```
