#ifndef _TTS6440_GF_H
#define _TTS6440_GF_H
#include <cstdlib>

template <unsigned p> struct tts6440_gf;
template <unsigned p> inline const tts6440_gf<p> reduce(const tts6440_gf<p> &a){return a.v-((a.v+p/2)*tts6440_gf<p>::R>>16)*p;}
template <unsigned p> inline const tts6440_gf<p> principal(const tts6440_gf<p> &a){short t = a%(short)p; return t<0?t+p:t;}
template <unsigned p> inline const tts6440_gf<p> invert(const tts6440_gf<p> &a){return tts6440_gf<p>::inv_table[reduce(a).v];}

template <unsigned p> struct tts6440_gf{
	short v;

	static const unsigned short R = 65536 / p;
	inline tts6440_gf<p>(){}
	inline tts6440_gf<p>(const tts6440_gf<p>& a):v(a.v){}
	inline tts6440_gf<p>(const short &a):v(a){}
	inline tts6440_gf<p>& operator=(const tts6440_gf<p> &b) {v=b.v; return *this;}
	inline tts6440_gf<p>& operator+=(const tts6440_gf<p> &b) {return *this = *this + b;}
	inline tts6440_gf<p>& operator-=(const tts6440_gf<p> &b) {return *this = *this - b;}
	inline tts6440_gf<p>& operator*=(const tts6440_gf<p> &b) {return *this = *this * b;}
	inline tts6440_gf<p>& operator/=(const tts6440_gf<p> &b) {return *this = *this / b;}

	inline const tts6440_gf<p> operator+(const tts6440_gf<p> &b) const {return v+b.v;}
	inline const tts6440_gf<p> operator-(const tts6440_gf<p> &b) const {return v-b.v;}
	inline const tts6440_gf<p> operator*(const tts6440_gf<p> &b) const {return v*b.v;}
	inline const tts6440_gf<p> operator/(const tts6440_gf<p> &b) const {return *this*invert(b);}
	inline const tts6440_gf<p> operator-() const {return -v;}

	inline bool operator==(const tts6440_gf<p> &b) const {return !(*this-b);}
	inline bool operator!=(const tts6440_gf<p> &b) const {return *this-b;}

	inline operator bool() const {return reduce(*this).v;}

	static const short _inv_table[];
	static const short * const inv_table;

	static inline tts6440_gf<p> rand() {return ::rand()%p-p/2;}
	static inline tts6440_gf<p> rand_unit() {short tmp = ::rand()%(p-1)-p/2; return tmp<0?tmp:(tmp+1);}
};


template<> const short tts6440_gf<31>::_inv_table[] = {1, -15, -10, 8, -6, -5, 9, 4, 7, -3, -14, 13, 12, -11, -2, 2, 11, -12, -13, 14, 3, -7, -4, -9, 5, 6, -8, 10, 15, -1, 0, 1, -15, -10, 8, -6, -5, 9, 4, 7, -3, -14, 13, 12, -11, -2, 2, 11, -12, -13, 14, 3, -7, -4, -9, 5, 6, -8, 10, 15, -1};
template<unsigned p> const short * const tts6440_gf<p>::inv_table = _inv_table + p - 1;

#endif
