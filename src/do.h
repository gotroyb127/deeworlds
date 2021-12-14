/* do.h */

#define do2(m) \
	do { \
		m(0, x) \
		m(1, y) \
	} while (0)
#define do2e(e, j) \
	( \
		(e(0, x)) j \
		(e(1, y)) \
	)

#define do3r(r, b, a, j) \
	b r(0, x) a j \
	b r(1, y) a j \
	b r(2, z) a
#define do3rv(r, b, a, j, ...) \
	b r(0, x, __VA_ARGS__) a j \
	b r(1, y, __VA_ARGS__) a j \
	b r(2, z, __VA_ARGS__) a

#define do3(m) \
	do { \
		m(0, x) \
		m(1, y) \
		m(2, z) \
	} while (0)
#define do3v(m, ...) \
	do { \
		m(0, x, __VA_ARGS__) \
		m(1, y, __VA_ARGS__) \
		m(2, z, __VA_ARGS__) \
	} while (0)

#define do3e(e, j) \
	( \
		(e(0, x)) j \
		(e(1, y)) j \
		(e(2, z)) \
	)
#define do3ev(e, j, ...) \
	( \
		(e(0, x, __VA_ARGS__)) j \
		(e(1, y, __VA_ARGS__)) j \
		(e(2, z, __VA_ARGS__)) \
	)

#define do2xz(m) \
	do { \
		m(0, x) \
		m(2, z) \
	} while (0)

#define do4(m) \
	do { \
		m(0, x) \
		m(1, y) \
		m(2, z) \
		m(3, w) \
	} while (0)
#define do4v(m, ...) \
	do { \
		m(0, x, __VA_ARGS__) \
		m(1, y, __VA_ARGS__) \
		m(2, z, __VA_ARGS__) \
		m(3, w, __VA_ARGS__) \
	} while (0)

#define do4e(e, j) \
	( \
		(e(0, x)) j \
		(e(1, y)) j \
		(e(2, z)) j \
		(e(3, w)) \
	)
#define do4ev(e, j, ...) \
	( \
		(e(0, x, __VA_ARGS__)) j \
		(e(1, y, __VA_ARGS__)) j \
		(e(2, z, __VA_ARGS__)) j \
		(e(3, w, __VA_ARGS__)) \
	)
