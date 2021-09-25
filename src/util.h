/* util.h */

#define LENGTH(X) ((sizeof X) / (sizeof X[0]))
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
