/* -*- mode:c;indent-tabs-mode:nil;c-basic-offset:4;coding:utf-8 -*-
   vi: set et ft=c ts=4 sts=4 sw=4 fenc=utf-8 :vi

   Copyright 2024 Justine Alexandra Roberts Tunney

   Permission to use, copy, modify, and/or distribute this software for
   any purpose with or without fee is hereby granted, provided that the
   above copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
   AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
   DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
   PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
   TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
   PERFORMANCE OF THIS SOFTWARE. */

#include "morton.h"

#include <assert.h>

static inline unsigned rand32(void) {
    /* Knuth, D.E., "The Art of Computer Programming," Vol 2,
       Seminumerical Algorithms, Third Edition, Addison-Wesley, 1998,
       p. 106 (line 26) & p. 108 */
    static unsigned long long lcg = 1;
    lcg *= 6364136223846793005;
    lcg += 1442695040888963407;
    return lcg >> 32;
}

int main() {
    assert(morton(0, 0) == 0);
    assert(morton(0, 1) == 1);
    assert(morton(1, 0) == 2);
    assert(morton(1, 1) == 3);
#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
    assert(morton(0b0011, 0b0000) == 0b1010);
    assert(morton(0b0000, 0b0011) == 0b0101);
    assert(morton(0b1100, 0b0011) == 0b10100101);
#endif
    assert(morton(0x347210d1u, 0xc6843fadu) == 0x5a346a180755e653ull);
    int N = 2000;
    for (int i = 0; i < N; ++i) {
        unsigned x = rand32();
        for (int j = 0; j < N; ++j) {
            unsigned y = rand32();
            unsigned long long z = morton(x, y);
            struct Morton m = unmorton(z);
            assert(m.lo == x);
            assert(m.hi == y);
        }
    }
}
