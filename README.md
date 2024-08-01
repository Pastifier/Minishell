# Minishell

Minishell is a 42 project where you have to create a simple bash interpreter with the following features:
- Redirections.
- Piping.
- Command execution.
- Handling of the SIGINT, EOF/SIGQUIT, SIGQUIT behaviours (using signals).
- Implementing:
- echo with option -n
- cd with only a relative or absolute path
- pwd with no options
- export with no options
- unset with no options
- env with no options or arguments
- exit with no options
- `$` (dollar-sign) expansion of variable expressions (only expansion of environment variables is required).
- BONUS: handling `&&` and `||` with parentheses for priority.
- BONUS: Wildcards should work for the current directory.

## The program's main structure:

## Tokenizer:
    - Reading the line -> return the user input as a string.
    - tokenization -> make a list/queue/stack of tokens that you can arrange in abstract-form to feed your interpreter/proccessor.
## Tokens -> parser:
    - Parse every token and ensure that it is valid in its syntax and its grammar.
    - Create an Abstract Syntax Tree (commonly referred to as AST) that represents the code in a simple hierarchical structure.
    -> return the tree
#### Example:
```sh
$> WORD1 | WORD2 > WORD3
```
    Will return this tree:
```
                             ( PIPE )
                            /        \
                           /          \
                      ( WORD1 )      ( WORD2 )
                      [ ARGS ]        [ ARGS ]
                                              \
                                                \
                                        ( OUTPUT_REDIRECTION )
                                             [ WORD ]
```
### The parsing step is where all the syntax is checked for. A syntax error will stop the ongoing operation and will return an error!
### The way you decide to execute the command pipeline will be the determining factor of your tree's structure. For example: some people might find that making the output redirection a parent to the pipe more logical, but we use a little trick when it comes to I/O redirections and so, we decided to structure it this way. Some people might find that each word should be just a word, and that its children nodes are its "arguments" if it were a command for example. Point is: Some shapes might be easier to work with than others depending on your processing methods, but it's all up to you in the end!

## AST -> Interpreter/Processor: execute the commands on the tree
    - Do a recursive, post-order depth-first search on the tree until you reach a leaf node.
    - Use `fork(2)` and `execve(2)` to execute commands.
    - Upon failure, check whether the commands are builtins. If so, go to the builtin behaviour of those commands.
    - The recursion will naturally resolve all tree nodes in ascending order.
#### Example (Let's go with a simpler tree structure to illustrate the idea):
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
In this simple case, the program will reach the left child of the `PIPE` node. Will check for its parent's special behaviour, execute accordingly, and resolve that branch of the tree. After those nodes are eliminated (by whichever method you choose), it will fetch the output of `WORD2` and redirect it to `WORD3`.

### Execution is where all the checks for existing commands, files, and access rights are done. The input can literally be the example above, and it will still get to the execution part. Why? Because it is syntactically correct.
