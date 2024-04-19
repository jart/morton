# Morton

This repository implements bit interleaving in C/C++.

     morton(0b1100,0b0011)
              │││└──┐││││
              ││└─┐ │││││
              │└┐ │ │││││
              │┌─────┘│││
              │││ │ │ │││
              │││┌────┘││
              │││││ │  ││
              │││││┌───┘│
              │││││││┌──┘
            0b10100101

By default a SWAR implementation is used which takes 2 nanoseconds. If
`-mbmi2` is available, then `morton()` and `unmorton()` take less than
one nanosecond. If no compiler optimizations are used then these funcs
take 10 nanoseconds. The cost is constant since this implementation is
fully branchless.

## See Also

- <https://en.wikipedia.org/wiki/Z-order_curve>
- <https://graphics.stanford.edu/~seander/bithacks.html>
- <https://stackoverflow.com/questions/30539347/2d-morton-code-encode-decode-64bits>
