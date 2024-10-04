# Semantic IR
Harc will utilize a semantic IR as an alternative to an AST. The semantic IR 
representation aims to be a more cache-friendly approach to performing the 
necessary transformations.

Functions will maintain a list of objects and a list of operations which are 
used in the function body.

Each object will have a lifetime which are 

Objects will have represented as an index into a list of objects which are 
used during a function.

All instructions are a combination of an operation index, and object indices.


