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
`time ./wspace programs/is_prime.ws >/dev/null   `|  27.594s|  27.345s|  0.213s
`time ./compile f programs/is_prime.ws >/dev/null`|  0.954s |  0.942s |  0.007s
`time ./compile t programs/is_prime.ws >/dev/null`|  0.248s |  0.244s |  0.002s

Speed-up relative to original interpreter

Command                                           | real    | user    | sys
:-------------------------------------------------|--------:|--------:|--------:
`time ./wspace programs/is_prime.ws >/dev/null   `|       1x|       1x|       1x
`time ./compile f programs/is_prime.ws >/dev/null`|  28.925x|  29.029x|  30.429x
`time ./compile t programs/is_prime.ws >/dev/null`| 111.266x| 112.070x|   106.5x

Instructions per second (234248149 / real time)


Name                         | Instructions per second
:----------------------------|-----------------------:
Original interpreter         |              8,489,097
This repository's interpreter|            394,357,153
JIT compiler                 |            944,548,987

## Acknowledgments
Inspired by [whitespace-rs](https://github.com/CensoredUsername/whitespace-rs)

