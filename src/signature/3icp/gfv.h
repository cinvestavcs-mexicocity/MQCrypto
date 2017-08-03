#ifndef _IC3_GFV_H
#define _IC3_GFV_H
#include <emmintrin.h>
#include "gf.h"

template <unsigned p> struct ic3_gfv{
	__m128i v;

	static const unsigned short R = 65536 / p;
	inline ic3_gfv<p>(){}
	inline ic3_gfv<p>(const ic3_gfv<p>& a):v(a.v){}
	inline ic3_gfv<p>(const __m128i& a):v(a){}
	inline ic3_gfv<p>(const ic3_gf<p>& a):v(_mm_set1_epi16(a.v)){}
	inline ic3_gfv<p>& operator=(const ic3_gfv<p> &b) {v=b.v; return *this;}
	inline ic3_gfv<p>& operator+=(const ic3_gfv<p> &b) {return *this = *this + b;}
	inline ic3_gfv<p>& operator-=(const ic3_gfv<p> &b) {return *this = *this - b;}
	inline ic3_gfv<p>& operator*=(const ic3_gfv<p> &b) {return *this = *this * b;}
	inline ic3_gfv<p>& operator/=(const ic3_gfv<p> &b) {return *this = *this / b;}

	inline const ic3_gfv<p> operator+(const ic3_gfv<p> &b) const {return _mm_add_epi16(this->v, b.v);}
	inline const ic3_gfv<p> operator-(const ic3_gfv<p> &b) const {return _mm_sub_epi16(this->v, b.v);}
	inline const ic3_gfv<p> operator*(const ic3_gfv<p> &b) const {return _mm_mullo_epi16(this->v, b.v);}
	inline const ic3_gfv<p> operator/(const ic3_gfv<p> &b) const {return *this * invertv(reducev(b));}
	inline const ic3_gfv<p> operator-() const {return _mm_sub_epi16(_mm_setzero_si128(), this->v);}

	inline const ic3_gf<p> operator[](unsigned i) const {return ((short *)&v)[i];}
};

template <unsigned p> inline const ic3_gfv<p> reducev(const ic3_gfv<p> &a);
template <unsigned p> inline const ic3_gfv<p> principalv(const ic3_gfv<p> &a);
template <unsigned p> inline const ic3_gfv<p> invertv(const ic3_gfv<p> &a); /* use with caution, A must be reduced. */

template <unsigned p> inline const ic3_gfv<p> reducev(const ic3_gfv<p> &a){
	return _mm_sub_epi16(a.v, _mm_mullo_epi16(_mm_mulhi_epi16(_mm_add_epi16(a.v, _mm_set1_epi16((p-1)/2)), _mm_set1_epi16(ic3_gfv<p>::R)), _mm_set1_epi16(p)));
}

template <> inline const ic3_gfv<31> reducev(const ic3_gfv<31> &a){
	return _mm_sub_epi16(a.v, _mm_mullo_epi16(_mm_mulhi_epi16(_mm_add_epi16(a.v, _mm_set1_epi16(16)), _mm_set1_epi16(ic3_gfv<31>::R)), _mm_set1_epi16(31)));
}

template <> inline const ic3_gfv<31> principalv(const ic3_gfv<31> &a){
	ic3_gfv<31> tmp = reducev(a);
	return _mm_sub_epi16(tmp.v, _mm_mullo_epi16(_mm_mulhi_epi16(_mm_add_epi16(tmp.v, _mm_set1_epi16(15)), _mm_set1_epi16(ic3_gfv<31>::R)), _mm_set1_epi16(31)));
}

template <> inline const ic3_gfv<31> invertv(const ic3_gfv<31> &a){
	ic3_gfv<31> result = reducev(a * a * a);
	result = reducev(a * result * result);
	result = reducev(result * result);	
	return reducev(a * result * result);
}

#endif
