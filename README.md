# Whitespace JIT
A [Whitespace](https://esolangs.org/wiki/Whitespace) compiler/interpreter written in C++. The JIT compiler supports `x86_64` machines with System V AMD64 ABI calling conventions. The interpreter should be more universally usable. For some sample Whitespace programs, see the [Esolangs page](https://esolangs.org/wiki/Whitespace) or my [Whitespace programs repository](https://github.com/drebelsky/whitespace-programs).

## Usage

### Dependencies
* `make`
* `g++`

### Compiling
To compile, just run `make`.

### Running
The default `make` rule will compile the `compile` executable. The `compile` executable's first argument is `t` (to use the JIT compiler) or `f` (to use the interpreter); the second argument is the filename.

```
Usage: ./compile <use_jit:t|f> <filename>
```


## Speed Comparison
Below are the comparison runs for 1. [the original whitespace interpreter (version 0.3)](https://web.archive.org/web/20150717140342/http://compsoc.dur.ac.uk/whitespace/download.php), 2. this repository's interpreter, 3. this repository's JIT compiler. The timing was done using `bash`'s builtin `time` for running the three programs with stdout redirected to `/dev/null` on [`is_prime.ws`](https://github.com/drebelsky/whitespace-jit/blob/main/programs/is_prime.ws), a simple whitespace program that calculates the primes up to 20,000 using the naive O(n^2) algorithm. This program requires the execution of 234,248,149 instructions.

Command                                           | real    | user    | sys
:-------------------------------------------------|--------:|--------:|--------:
`time ./wspace programs/is_prime.ws >/dev/null   `|  27.274s|  24.875s|  0.219s
`time ./compile f programs/is_prime.ws >/dev/null`|  0.684s |  0.641s |  0.016s
`time ./compile t programs/is_prime.ws >/dev/null`|  0.223s |  0.188s |  0.016s

Speed-up relative to original interpreter

Command                                           | real    | user    | sys
:-------------------------------------------------|--------:|--------:|--------:
`time ./wspace programs/is_prime.ws >/dev/null   `|       1x|       1x|       1x
`time ./compile f programs/is_prime.ws >/dev/null`|  39.874x|  38.807x|  13.688x
`time ./compile t programs/is_prime.ws >/dev/null`| 178.789x| 132.314x|  13.688x

Instructions per second (234248149 / real time)

Name                         | Instructions per second
:----------------------------|-----------------------:
Original interpreter         |              8,588,698
This repository's interpreter|            342,468,054
JIT compiler                 |          1,050,440,130

## Acknowledgments
Inspired by [whitespace-rs](https://github.com/CensoredUsername/whitespace-rs)

