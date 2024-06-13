# Grammar for Rift (to be updated)

expression     → literal
               | unary
               | binary
               | grouping ;

literal        → NUMBER | STRING | "true" | "false" | "nil" ;
grouping       → "(" expression ")" ;
unary          → ( "-" | "!" ) expression ;
binary         → expression operator expression ;
operator       → "==" | "!=" | "<" | "<=" | ">" | ">="
               | "+"  | "-"  | "*" | "/" ;

## Flat

expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;


# Precedence and Associativity

Name  |	Operators |	Associates
------|------------|------------
Equality |	== != |	Left

Comparison |	> >= < <= |	Left

Term |	- + |	Left

Factor |	/ * |	Left

Unary |	! - |	Right

## Precedence

1. expression
2. equality
3. comparison
4. term
5. factor
6. unary
7. primary

## Associativity

...