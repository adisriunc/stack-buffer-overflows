# Stack Buffer Overflows in C

I explored how writing beyond a local array can corrupt
stack memory, how that corruption can become a security vulnerability,
and how programmers defend against it.

## What I looked into

- How unchecked copying can write beyond a stack buffer.
- Why C strings need space for a null terminator.
- How an overwritten return address could redirect execution.
- How stack canaries and a non-executable stack help block attacks.
- How a bounds check prevents an oversized copy.

## Code examples

### vulnerable.c

Copies a command-line argument into an eight-byte array using strcpy
without checking its length.

### fixed.c

Adds a length check before the copy. Any input requiring more than eight
bytes, including the null terminator, is rejected.

## How to compile

Run these commands in a terminal from the folder containing both files.
These commands require GCC and explicitly enable stack canaries.

gcc -Wall -Wextra -O0 -fstack-protector-all vulnerable.c -o vulnerable
gcc -Wall -Wextra -O0 -fstack-protector-all fixed.c -o fixed

## How to run

Try input that fits:

./vulnerable AAA
./fixed AAA

Then try some oversized input:

./vulnerable AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
./fixed AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

The vulnerable program writes beyond its array. With stack protection
enabled, it may report stack smashing and terminate. The exact behavior
depends on the environment. An out-of-bounds write is undefined
behavior in C.

The revised program rejects the oversized input before copying it, fixing the issue.

This example demonstrates an overflow attempt and its prevention.

## Why this can become a security vulnerability

In a classic code-injection attack, input contains machine-code bytes.
An attacker attempts to overwrite the saved return address so it points
to those bytes. If the memory is executable and other protections do
not stop the attempt, returning from the function could execute the
attacker's instructions.

## Two defenses

### Stack canary

Compiler-generated code places a guard value in a protected stack
frame and checks it before returning. If an overwrite changes the
guard, the program terminates.

Limitation: A corruption that leaves the canary unchanged can go undetected.

### Non-executable stack (NX)

The operating system marks stack memory as non-executable, and the CPU
blocks attempts to execute instructions stored there.

Limitation: NX does not prevent memory corruption or every attack
that redirects execution to existing executable code.

## Preventing the copying error

The fixed program checks:

strlen(input) >= sizeof name

strlen excludes the null terminator. Rejecting lengths of eight or
more leaves room for seven bytes of text plus the terminator.

## Key takeaway

Stack canaries detect certain overwrites. NX blocks execution from
non-executable stack memory. A correct bounds check prevents this
oversized copy from happening.

## Presentation

See presentation.pdf for the slides.

## Resources

- MITRE: Stack-based Buffer Overflow
  https://cwe.mitre.org/data/definitions/121.html

- GCC: Instrumentation Options
  https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html

- Ubuntu: Process and Memory Protections
  https://documentation.ubuntu.com/security/security-features/process-memory/

## AI assistance

I used ChatGPT to help explain concepts and check for accuracy in my understanding of them. My separate AI reflection describes
the learning process.
