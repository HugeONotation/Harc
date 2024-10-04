# Registers
The bytecode would have to target a machine with a specific amount of registers. The amount of registers used is 
dependent on the current target. There are also different registers use for different data types. There are also 
registers which alias parts of other registers, or which are simply aliases altogether. There is also the often 
singular carry, parity flag, auxiliary carry flag, zero flat, sign flag, overflow tag.

The aliasing of certain registers for parts of another is a challenge to handle.

## Solution
### Register Categories
* there will be a set of registers which are used to

### Register Files
* it is assumed that no more than 256 registers of any category are available on the target architecture

### Function calls
* in order to be ABI agnostic, it will be necessary to make function calls not follow any particular convention
* perhaps an internal calling convention can be called to 