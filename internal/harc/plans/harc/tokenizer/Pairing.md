# Pairing
One possible method of creating a resilient tokenizer and ease the burden on 
the parser would be to have the tokenizer match parentheses, curly brackets, 
square brackets, and angle brackets up front.

If these token are not paired up properly, then this will mean that the 
source is flawed. However, it may still be useful to continue with 
compilation to provide better error handling.

Checking if tokens are not matched properly is easy since it would just be a 
matter of pushing left tokens onto a stack and popping when the 
corresponding right token is found. If the token to be popped doesn't match 
up with the token at the top of the stack then we know that there is an error. 

However, this doesn't answer whether the stray token is extraneous or if 
there is a missing token to pair with. Additional relevant information could 
be deduced while parsing, however, this would be contradictory to the 
current design goals.

Another way would be to utilize indentation and partial pairing of grammar 
constructs as a heuristic for which tokens should be matched with each other.

## Useful rules:
* function declarations begin with the `func` keyword and no rules 
  exist in Harmonia's grammar which would have balanced token encompass one 
  or more function declarations, meaning that at every sight of the `func` 
  keyword, the balancing stack may be reset.
* similarly, struct declarations begins with the `struct` keyword and the 
  same pertinent details apply

## Edge Cases
* care must be taken to ensure that shift operators and nested template 
  parameters do not get mistaken for each other . e.g. 
  `std.Vector<std.Vector<i32>>` would get tokenized as:
  `std` `.` `Vector` `<` `std` `.` `Vector` `<` `i32` `>>`
  where the last pair of `>>` is recognized as a right-shift operator 
  instead of two consecutive right-angle brackets

  A naive solution would be to allow the right-shift token to match multiple 
  left-angle brackets, but this may lead to spurious matches. e.g.
  `std.Array<std.Array<i32, 128 >> 2>, 16>` is valid code but the `>>` 
  token should not match the two left angle-bracket tokens.

  When parsing, there would be enough information for this ambiguity to be 
  resolved because the `>>` token could be successfully parsed as part of a 
  binary operation.

  The current approach will be that when the tokenizer encounters two 
  consecutive `>` codepoints, then it will check to see if the two tokens at 
  the top of the balancing stack are both L_ANGLE_BRACKET. If so, then it 
  will interpret them as two R_ANGLE_BRACKET tokens, instead of a single 
  RIGHT_SHIFT token.

## Heuristics:
* Two different lines which contain paired tokens will have the 
  same indentation
* 

```
func[] foo() -> () {
    if true {
        std.printf("Hello, World!");
    }

    let std.Vector v = {0:i32..7:i32};
  
    // What should this syntax represent?
    // A 0, and a std.Range object?
    let t = (0, 1..7);

    let weight = 3:i32;

    if 1 is in v {
        
    }

    for (int i = 0; i < 10; ++i) {
        std.printf("Orange");
    }
}
```
