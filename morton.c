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

#ifdef __BMI2__
#include <immintrin.h>
#endif

/**
 * Interleaves bits.
 *
 *     morton(0b1100,0b0011)
 *              │││└──┐││││
 *              ││└─┐ │││││
 *              │└┐ │ │││││
 *              │┌─────┘│││
 *              │││ │ │ │││
 *              │││┌────┘││
 *              │││││ │  ││
 *              │││││┌───┘│
 *              │││││││┌──┘
 *            0b10100101
 */
unsigned long long morton(unsigned hi, unsigned lo) {
#ifdef __BMI2__
    return _pdep_u64(lo, 0x5555555555555555) |
           _pdep_u64(hi, 0xAAAAAAAAAAAAAAAA);
#else
    unsigned long long xu = lo;
    unsigned long long yu = hi;
    xu = (xu | xu << 020) & 0x0000FFFF0000FFFF;
    xu = (xu | xu << 010) & 0x00FF00FF00FF00FF;
    xu = (xu | xu << 004) & 0x0F0F0F0F0F0F0F0F;
    xu = (xu | xu << 002) & 0x3333333333333333;
    xu = (xu | xu << 001) & 0x5555555555555555;
    yu = (yu | yu << 020) & 0x0000FFFF0000FFFF;
    yu = (yu | yu << 010) & 0x00FF00FF00FF00FF;
    yu = (yu | yu << 004) & 0x0F0F0F0F0F0F0F0F;
    yu = (yu | yu << 002) & 0x3333333333333333;
    yu = (yu | yu << 001) & 0x5555555555555555;
    return xu | yu << 1;
#endif
}

static inline unsigned unmortoner(unsigned long long x) {
    x &= 0x5555555555555555;
    x = (x | x >> 001) & 0x3333333333333333;
    x = (x | x >> 002) & 0x0F0F0F0F0F0F0F0F;
    x = (x | x >> 004) & 0x00FF00FF00FF00FF;
    x = (x | x >> 010) & 0x0000FFFF0000FFFF;
    x = (x | x >> 020) & 0x00000000FFFFFFFF;
    return x;
}

/**
 * Deinterleaves bits.
 */
struct Morton unmorton(unsigned long long z) {
    struct Morton m;
#ifdef __BMI2__
    m.hi = _pext_u64(z, 0x5555555555555555);
    m.lo = _pext_u64(z, 0xAAAAAAAAAAAAAAAA);
#else
    m.hi = unmortoner(z);
    m.lo = unmortoner(z >> 1);
#endif
    return m;
}
