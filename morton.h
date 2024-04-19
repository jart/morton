#ifndef MORTON_H_
#define MORTON_H_

#if defined(__GNUC__) || (defined(__has_attribute) && __has_attribute(const))
#define MORTON_PURE __attribute__((__const__))
#else
#define MORTON_PURE
#endif

struct Morton {
    unsigned lo;
    unsigned hi;
};

struct Morton unmorton(unsigned long long) MORTON_PURE;
unsigned long long morton(unsigned, unsigned) MORTON_PURE;

#endif /* MORTON_H_ */
