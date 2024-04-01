# Minishell
Ahmed and Emran!

## the program main structure:

- Reading the line -> return the user input as  a string.
- String line -> tokenization split the line by spaces -> return token-list and specify every token type
### token list -> parser: 
    - Parse every token and ensure that it is valid in its syntax and its grammer
    - Put every command in its location in the tree
    -> return the tree
### tree -> execution: execute the commands on the tree
    - Do a recursive, post-order depth-first search on the tree until you reach a leaf node.
    - Use `fork(2)` and `execve(2)` to execute commands.
    - Upon failure, check whether the commands are builtins. If so, go to the builtin behaviour of those commands.
    - The recursion will naturally resolve all tree nodes in ascending order.
```

                                      ( REDIRECT_OUTPUT )
                                    /                    \
                                   /                      \
                             ( PIPE )                   ( WORD )
                            /        \
                           /          \
                      ( WORD )      ( WORD )
```
