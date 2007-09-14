#ifndef UTIL_HPP
#define UTIL_HPP

inline int mod(int a, int b) {
	int r = a % b;
	return r < 0 ? b + r : r;
}

#endif