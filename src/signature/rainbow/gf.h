#ifndef _RAINBOW_GF_H
#define _RAINBOW_GF_H
#include <cstdlib>

template <unsigned p> struct rainbow_gf;
template <unsigned p> inline const rainbow_gf<p> reduce(const rainbow_gf<p> &a){return a.v-((a.v+p/2)*rainbow_gf<p>::R>>16)*p;}
template <unsigned p> inline const rainbow_gf<p> principal(const rainbow_gf<p> &a){short t = a%(short)p; return t<0?t+p:t;}
template <unsigned p> inline const rainbow_gf<p> invert(const rainbow_gf<p> &a){return rainbow_gf<p>::inv_table[reduce(a).v];}

template <unsigned p> struct rainbow_gf{
	short v;

	static const unsigned short R = 65536 / p;
	inline rainbow_gf<p>(){}
	inline rainbow_gf<p>(const rainbow_gf<p>& a):v(a.v){}
	inline rainbow_gf<p>(const short &a):v(a){}
	inline rainbow_gf<p>& operator=(const rainbow_gf<p> &b) {v=b.v; return *this;}
	inline rainbow_gf<p>& operator+=(const rainbow_gf<p> &b) {return *this = *this + b;}
	inline rainbow_gf<p>& operator-=(const rainbow_gf<p> &b) {return *this = *this - b;}
	inline rainbow_gf<p>& operator*=(const rainbow_gf<p> &b) {return *this = *this * b;}
	inline rainbow_gf<p>& operator/=(const rainbow_gf<p> &b) {return *this = *this / b;}

	inline const rainbow_gf<p> operator+(const rainbow_gf<p> &b) const {return v+b.v;}
	inline const rainbow_gf<p> operator-(const rainbow_gf<p> &b) const {return v-b.v;}
	inline const rainbow_gf<p> operator*(const rainbow_gf<p> &b) const {return v*b.v;}
	inline const rainbow_gf<p> operator/(const rainbow_gf<p> &b) const {return *this*invert(b);}
	inline const rainbow_gf<p> operator-() const {return -v;}

	inline bool operator==(const rainbow_gf<p> &b) const {return !(*this-b);}
	inline bool operator!=(const rainbow_gf<p> &b) const {return *this-b;}

	inline operator bool() const {return reduce(*this).v;}

	static const short _inv_table[];
	static const short * const inv_table;

	static inline rainbow_gf<p> rand() {return ::rand()%p-p/2;}
	static inline rainbow_gf<p> rand_unit() {short tmp = ::rand()%(p-1)-p/2; return tmp<0?tmp:(tmp+1);}
};


template<> const short rainbow_gf<31>::_inv_table[] = {1, -15, -10, 8, -6, -5, 9, 4, 7, -3, -14, 13, 12, -11, -2, 2, 11, -12, -13, 14, 3, -7, -4, -9, 5, 6, -8, 10, 15, -1, 0, 1, -15, -10, 8, -6, -5, 9, 4, 7, -3, -14, 13, 12, -11, -2, 2, 11, -12, -13, 14, 3, -7, -4, -9, 5, 6, -8, 10, 15, -1};
template<unsigned p> const short * const rainbow_gf<p>::inv_table = _inv_table + p - 1;

#endif
