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
## Example:
```sh
$> WORD1 | WORD2 > WORD3

                                      ( REDIRECT_OUTPUT )
                                    /                    \
                                   /                      \
                             ( PIPE )                   ( WORD3 )
                            /        \
                           /          \
                      ( WORD1 )      ( WORD2 )
```

The pseudo-not-so-pseudo code:
```c
void traverse(ASTNode *node)
{
    if node == NULL:
        return ;
    traverse(node->left);
    traverse(node->right);
    // do stuff.
}
```
Will traverse the tree from the top, going down until it reaches a leaf. Notice how each node in the tree has its special action, and that all leaf-nodes are words.
In this simple case, the program will reach the left child of the `PIPE` node. Will check for its parent's special behaviour, execute accordingly, and resolve that branch of the tree. After those nodes are eliminated (by whichever method you choose), it will fetch the output of `WORD2` in redirect it to `WORD3`.

### Execution is where all the checks for existing commands, files, and access rights are done. The input can literally be the example above, and it will still get to the execution part. Why? Because it is syntactically correct.
