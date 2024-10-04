# Const Propagation
For most functions, it's east to determine if something is a constant expression because the const-ness of values 
will not generally depend on the type with which the function is instantiated. However, in the case that a variable 
is dependent on another template, then there is a possibility that the constness will change on which specialization 
is used.

One possible solution would be to apply const-propagation at the level of the virtual bytecode. This would make it 
part of the translation step.
