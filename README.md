# Minishell

**Minishell** is a 42 project focused on building a minimal Bash-like shell. This shell interprets and executes commands with support for piping, redirections, and environment variable expansion. The project also includes built-in commands and advanced features as bonuses.

---

## Features
  - Redirections and HEREDOC: `>`, `>>`, `<`, `<< DELIM`
  - Piping: `|`
  - Command execution using `fork(2)` and `execve(2)`
  - Signal handling: `SIGINT`, `SIGQUIT`, `EOF`
  - Built-in commands:
    - `echo` (with `-n`)
    - `cd` (relative or absolute paths)
    - `pwd` (no options)
    - `export` (no options)
    - `unset` (no options)
    - `env` (no arguments)
    - `exit` (no options)
  - Environment variable expansion using `$VAR_NAME`

---

## Building and Running the Project

### Prerequisites
- **C Compiler** (e.g., `gcc`)
- **Makefile** for compilation
- **Unix-based system** (Linux or macOS)

### Compilation
Run the following command:
```sh
make
```

### Running
Start the shell by executing:
```sh
./minishell
```

---

## Structure

The project is divided into three main phases:

### 1. **Tokenizer**
The tokenizer breaks down the user input into manageable components (tokens).

#### Workflow:
1. Read the input string.
2. Tokenize the input into a list/queue/stack of structured tokens ready for parsing.

---

### 2. **Parser**
The parser verifies syntax and builds an **Abstract Syntax Tree (AST)** that represents the hierarchical structure of the command.

#### Example:
```sh
$> WORD1 | WORD2 > WORD3
```

The resulting tree might look like this:
```
                             ( PIPE )
                            /        \
                      ( WORD1 )      ( OUTPUT_REDIRECTION )
                                      [ WORD2 ]    [ WORD3 ]
```

#### Notes:
- The AST's structure depends on your execution strategy. You can adapt it to simplify I/O redirection or argument handling.
- Syntax errors are handled here. If the input is invalid, an error is returned, and execution stops.

---

### 3. **Interpreter**
The interpreter traverses the AST and executes the commands.

#### Execution Workflow:
1. Perform a **post-order depth-first traversal** of the AST.
2. At each node:
   - Execute commands using `fork(2)` and `execve(2)`.
   - Handle built-in commands internally if applicable.
   - Process I/O redirections or pipes based on the node's type.

#### Example:
For the AST:
```
                                      ( REDIRECT_OUTPUT )
                                    /                    \
                             ( PIPE )                   ( WORD3 )
                            /        \
                      ( WORD1 )      ( WORD2 )
```

The traversal and execution follow this process:
1. Traverse `WORD1` and execute it.
2. Pipe its output to `WORD2`.
3. Redirect the result of `WORD2` to `WORD3`.

#### Simplified Code:
```c
void traverse(ASTNode *node)
{
    if (node == NULL)
        return;
    traverse(node->left);
    traverse(node->right);
    execute_node(node);
}
```

---

## Notes on Implementation

- **Error Handling:** The shell ensures syntactic correctness during parsing. Runtime errors (e.g., invalid commands) are handled during execution.
- **Customization:** You are free to structure the AST and handle I/O redirections in ways that suit your implementation style. We opted for a different organisation of the AST in our Minishell than the one you see because we relied on some neat tricks for processing I/O operations. However, keep in mind that most AST configurations are valid, and it's your processor that determines how you construct the AST. Be smart, as some configurations are easier to deal with than others!

---

This project provides an excellent opportunity to explore Unix system calls, process management, and parsing algorithms. Have fun building your shell!
