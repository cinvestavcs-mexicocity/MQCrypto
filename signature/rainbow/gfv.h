#ifndef _RAINBOW_GFV_H
#define _RAINBOW_GFV_H
#include <emmintrin.h>
#include "gf.h"

template <unsigned p> struct rainbow_gfv{
	__m128i v;

	static const unsigned short R = 65536 / p;
	inline rainbow_gfv<p>(){}
	inline rainbow_gfv<p>(const rainbow_gfv<p>& a):v(a.v){}
	inline rainbow_gfv<p>(const __m128i& a):v(a){}
	inline rainbow_gfv<p>(const rainbow_gf<p>& a):v(_mm_set1_epi16(a.v)){}
	inline rainbow_gfv<p>& operator=(const rainbow_gfv<p> &b) {v=b.v; return *this;}
	inline rainbow_gfv<p>& operator+=(const rainbow_gfv<p> &b) {return *this = *this + b;}
	inline rainbow_gfv<p>& operator-=(const rainbow_gfv<p> &b) {return *this = *this - b;}
	inline rainbow_gfv<p>& operator*=(const rainbow_gfv<p> &b) {return *this = *this * b;}
	inline rainbow_gfv<p>& operator/=(const rainbow_gfv<p> &b) {return *this = *this / b;}

	inline const rainbow_gfv<p> operator+(const rainbow_gfv<p> &b) const {return _mm_add_epi16(this->v, b.v);}
	inline const rainbow_gfv<p> operator-(const rainbow_gfv<p> &b) const {return _mm_sub_epi16(this->v, b.v);}
	inline const rainbow_gfv<p> operator*(const rainbow_gfv<p> &b) const {return _mm_mullo_epi16(this->v, b.v);}
	inline const rainbow_gfv<p> operator/(const rainbow_gfv<p> &b) const {return *this * invertv(reducev(b));}
	inline const rainbow_gfv<p> operator-() const {return _mm_sub_epi16(_mm_setzero_si128(), this->v);}

	inline const rainbow_gf<p> operator[](unsigned i) const {return ((short *)&v)[i];}
};

template <unsigned p> inline const rainbow_gfv<p> reducev(const rainbow_gfv<p> &a);
template <unsigned p> inline const rainbow_gfv<p> principalv(const rainbow_gfv<p> &a);
template <unsigned p> inline const rainbow_gfv<p> invertv(const rainbow_gfv<p> &a); /* use with caution, A must be reduced. */

template <unsigned p> inline const rainbow_gfv<p> reducev(const rainbow_gfv<p> &a){
	return _mm_sub_epi16(a.v, _mm_mullo_epi16(_mm_mulhi_epi16(_mm_add_epi16(a.v, _mm_set1_epi16((p-1)/2)), _mm_set1_epi16(rainbow_gfv<p>::R)), _mm_set1_epi16(p)));
}

template <> inline const rainbow_gfv<31> reducev(const rainbow_gfv<31> &a){
	return _mm_sub_epi16(a.v, _mm_mullo_epi16(_mm_mulhi_epi16(_mm_add_epi16(a.v, _mm_set1_epi16(16)), _mm_set1_epi16(rainbow_gfv<31>::R)), _mm_set1_epi16(31)));
}

template <> inline const rainbow_gfv<31> principalv(const rainbow_gfv<31> &a){
	rainbow_gfv<31> tmp = reducev(a);
	return _mm_sub_epi16(tmp.v, _mm_mullo_epi16(_mm_mulhi_epi16(_mm_add_epi16(tmp.v, _mm_set1_epi16(15)), _mm_set1_epi16(rainbow_gfv<31>::R)), _mm_set1_epi16(31)));
}

template <> inline const rainbow_gfv<31> invertv(const rainbow_gfv<31> &a){
	rainbow_gfv<31> result = reducev(a * a * a);
	result = reducev(a * result * result);
	result = reducev(result * result);	
	return reducev(a * result * result);
}

#endif
