# Minishell
Ahmed and Emran!
the program main structur:

reading the line -> return the user input as  a string.
string line -> tokenization split the line by spaces -> return tokens list and spicify every token type
token list -> parser: 
    -pasre every token and ensur that it is valid as asyntax and its grammer
    -put every command in its location in the tree
    -> return the tree
tree -> excution: excute the commands on the tree