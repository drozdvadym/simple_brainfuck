# simple_brainfuck

A simple brainfuck interpreter in C lang.

### Build

```gcc -Ofast main.c -o simple_bfc```

or

```clang -Ofast main.c -o simple_bfc```


### Usage

```./simple_bfc path_to_src```

#### Example

```
> cat bench.bf
>++[<+++++++++++++>-]<[[>+>+<<-]>[<+>-]++++++++
[>++++++++<-]>.[-]<<>++++++++++[>++++++++++[>++
++++++++[>++++++++++[>++++++++++[>++++++++++[>+
+++++++++[-]<-]<-]<-]<-]<-]<-]<-]++++++++++.
```

```
> time ./simple_bfc bench.bf

ZYXWVUTSRQPONMLKJIHGFEDCBA

real	0m1.467s
user	0m1.467s
sys	0m0.000s
```

