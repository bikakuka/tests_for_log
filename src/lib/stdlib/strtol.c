/**
 * @file
 *
 * @brief convert a string to a long integer
 * @see stdlib.h
 *
 * @date 24.11.09
 * @author Nikolay Korotky
 */
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#define	ULONG_MAX	((unsigned long)(~0L))		/* 0xFFFFFFFF */
#define	LONG_MAX	((long)(ULONG_MAX >> 1))	/* 0x7FFFFFFF */
#define	LONG_MIN	((long)(~LONG_MAX))		/* 0x80000000 */
#if 1
long strtol(const char *nptr, char **endptr, int base) {
	const char *s = nptr;
	unsigned long acc;
	int c;
	unsigned long cutoff;
	int neg = 0, any, cutlim;

	/*
	* Skip white space and pick up leading +/- sign if any.
	* If base is 0, allow 0x for hex and 0 for octal, else
	* assume decimal; if base is already 16, allow 0x.
	*/
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+') {
		c = *s++;
	}
	if ((base == 0 || base == 16) &&
			c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	/****************/
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c)) {
			c -= '0';
		} else if (isalpha(c)) {
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		} else {
			break;
		}
		if (c >= base) {
			break;
		}
		if ((any < 0) || (acc > cutoff) || ((acc == cutoff) && (c > cutlim))) {
			any = -1;
		} else {
			any = 1;
			acc *= base;
			acc += c;
		}
			}
	/****************/
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
		errno = ERANGE;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? s - 1 : nptr);
		return (acc);
}
#endif
