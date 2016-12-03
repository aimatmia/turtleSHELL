# turtleSHELL

##Features:
	* Forks and executes commands!
	* Parses multiple commands on one line!
	* Redirects using >, <!

##Attempted:
	* TBD
##Bugs:
	* something

##Files and What they Do:
###trex.c:
'/*******************************************************
PROMPT: gets current directory and user. prints out the appropriate command line prompt depending on whether the hostname is null or not.
Takes as input: none
Returns: none
*******************************************************/

/*******************************************************
CD: takes in a path string, modifies the string if needed
and changes the directory to the path string
Takes as input: input string (path)
Returns: int (returns 0 upon completion)
*******************************************************/

/*******************************************************
DEBLANK: removes extra white space from around a given 
           string
Takes as input: input string
Returns: "trimmed" output string
*******************************************************/

/*******************************************************
READIN: reads (fgets) input from the terminal into the
string buffer
Takes as input: input string buffer
Returns: none
*******************************************************/

/*******************************************************
DECISIONMAKER: takes in string and after searching for 
certain characters such as '<', '>', or '|' determines the
appropriate action to take. calls on exec.
Takes as input: input string
Returns: none
*******************************************************/

/*******************************************************
PETERPIPER: separates the string by '|', adjusts redirection,
and executes after forking
Takes as input: input string
Returns: none
*******************************************************/

/*******************************************************
EXEC: TBD by Mia
Takes as input: takes in pointer to array of pointers, two 
ints (input and output file descriptors)
Returns: none
*******************************************************/
'

###main.c:
/*******************************************************
MAIN: Runs turtleSHELL
Takes as input: none
Returns: none
*******************************************************/

