/* util.h */

#define NELM(X) (sizeof (X) / sizeof (X)[0])
#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#define CAT_(A, B) A##B
#define CAT(A, B) CAT_(A, B)
#define ARG_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8
#define VA_NARGS(...) ARG_8(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

#define FPARS(type, ...) \
	CAT(FPARS_, VA_NARGS(__VA_ARGS__))(type, __VA_ARGS__)
#define FPARS_2(type, _1, _2) \
	type _1, type _2
#define FPARS_3(type, _1, _2, _3) \
	FPARS_2(type, _1, _2), type _3
#define FPARS_4(type, _1, _2, _3, _4) \
	FPARS_3(type, _1, _2, _3), type _4
#define FPARS_5(type, _1, _2, _3, _4, _5) \
	FPARS_4(type, _1, _2, _3, _4), type _5
#define FPARS_6(type, _1, _2, _3, _4, _5, _6) \
	FPARS_5(type, _1, _2, _3, _4, _5), type _6
#define FPARS_7(type, _1, _2, _3, _4, _5, _6, _7) \
	FPARS_6(type, _1, _2, _3, _4, _5, _6), type _7
#define FPARS_7(type, _1, _2, _3, _4, _5, _6, _7) \
	FPARS_6(type, _1, _2, _3, _4, _5, _6), type _7
#define FPARS_8(type, _1, _2, _3, _4, _5, _6, _7, _8) \
	FPARS_7(type, _1, _2, _3, _4, _5, _6, _7), type _8
