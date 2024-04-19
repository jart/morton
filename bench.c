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

#include <stdio.h>
#include <time.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

static inline int rand32(void) {
    static unsigned long long lcg = 1;
    lcg *= 6364136223846793005;
    lcg += 1442695040888963407;
    return lcg >> 32;
}

#ifdef _WIN32
static long long GetQueryPerformanceFrequency() {
    LARGE_INTEGER t;
    QueryPerformanceFrequency(&t);
    return t.QuadPart;
}
static long long GetQueryPerformanceCounter() {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return t.QuadPart;
}
#endif

static long long micros(void) {
#ifndef _WIN32
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000 + (ts.tv_nsec + 999) / 1000;
#else
    static long long timer_freq = GetQueryPerformanceFrequency();
    static long long timer_start = GetQueryPerformanceCounter();
    return ((GetQueryPerformanceCounter() - timer_start) * 1000000) / timer_freq;
#endif
}

#define N 1000000
#define M 4

unsigned A[N][M];
unsigned B[N][M];
unsigned long long C[N][M];

int main() {
    int n = 10000;
    long long t1, t2;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j)
            A[i][j] = rand32();
        for (int j = 0; j < M; ++j)
            B[i][j] = rand32();
        for (int j = 0; j < M; ++j)
            C[i][j] = rand32();
    }

    // compute memory overhead
    t1 = micros();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j)
            C[i][j] = A[i][j] ^ B[i][j];
    }
    t2 = micros();
    long long overhead = t2 - t1;

    // benchmark morton()
    t1 = micros();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j)
            C[i][j] = morton(A[i][j], B[i][j]);
    }
    t2 = micros();
    printf("%10g ns morton()\n", (t2 - t1 - overhead) * 1e3 / N / M);

    // benchmark unmorton()
    t1 = micros();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            struct Morton m = unmorton(C[i][j]);
            A[i][j] = m.lo;
            B[i][j] = m.hi;
        }
    }
    t2 = micros();
    printf("%10g ns unmorton()\n", (t2 - t1 - overhead) * 1e3 / N / M);
}
