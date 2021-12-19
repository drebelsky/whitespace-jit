# Whitespace JIT
A whitespace compiler/interpreter written in C++. The JIT compiler supports `x86_64` machines with System V AMD64 ABI calling conventions. The interpreter should be more universally usable.

## Usage

### Dependencies
* `make`
* `gc++

### Compiling
To compile just run `make`.

### Running
The default `make` rule will compile the `compile` executable. The `compile` executable's first argument is `t` (to use the JIT compiler) or `f` (to use the interpreter); the second argument is the filename.

```
Usage: ./compile <use_jit:t|f> <filename>
```


## Speed Comparison
Below are the comparison runs for 1. [the original whitespace interpreter (version 0.3)](https://web.archive.org/web/20150717140342/http://compsoc.dur.ac.uk/whitespace/download.php), 2. this repository's interpreter, 3. this repository's JIT compiler. The timing was done using `bash`'s builtin `time` for running the three programs with stdout redirected to `/dev/null` on [`is_prime.ws`](https://github.com/drebelsky/whitespace-jit/blob/main/programs/is_prime.ws), a simple whitespace program that calculates the primes up to 20,000 using the naive O(n^2) algorithm.

Command                                           | real    | user    | sys
:-------------------------------------------------|--------:|--------:|--------:
`time ./compile f programs/is_prime.ws >/dev/null`|0m0.684s |0m0.641s |0m0.016s
`time ./compile t programs/is_prime.ws >/dev/null`|0m0.223s |0m0.188s |0m0.016s
`time ./wspace programs/is_prime.ws >/dev/null   `|0m27.274s|0m24.875s|0m0.219s

## Acknowledgments
Inspired by [whitespace-rs](https://github.com/CensoredUsername/whitespace-rs)

