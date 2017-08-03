#ifndef _IC3_TOWER_H
#define _IC3_TOWER_H
#include <cstdio>
#include "gf.h"
#include "gfv.h"

template <unsigned p, unsigned pow>
struct ic3_gftower;

template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> reduce(const ic3_gftower<p,pow> &a);

template <unsigned p, unsigned pow>
const ic3_gftower<p,pow> invert(const ic3_gftower<p,pow> &a);
template <>
const ic3_gftower<31,18> invert(const ic3_gftower<31,18> &a);
template <>
const ic3_gftower<31,15> invert(const ic3_gftower<31,15> &a);
template <>
const ic3_gftower<31,10> invert(const ic3_gftower<31,10> &a);
template <>
const ic3_gftower<31,9> invert(const ic3_gftower<31,9> &a);
//below incorrect
template <>
const ic3_gftower<31,30> invert(const ic3_gftower<31,30> &a);
template <>
const ic3_gftower<31,45> invert(const ic3_gftower<31,45> &a);
template <>
const ic3_gftower<31,54> invert(const ic3_gftower<31,54> &a);
//above incorrect

template <unsigned p, unsigned pow>
const ic3_gftower<p,pow> sqrt(const ic3_gftower<p,pow> &a);
template <>
const ic3_gftower<31,18> sqrt(const ic3_gftower<31,18> &a);
template <>
const ic3_gftower<31,15> sqrt(const ic3_gftower<31,15> &a);
template <>
const ic3_gftower<31,10> sqrt(const ic3_gftower<31,10> &a);
template <>
const ic3_gftower<31,9> sqrt(const ic3_gftower<31,9> &a);

template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> mult_u(const ic3_gftower<p,pow> &a);

template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> powpk(const ic3_gftower<p,pow> &a, unsigned k);

template <unsigned p, unsigned pow>
void _mult(ic3_gftower<p,pow> *c, const ic3_gftower<p,pow> *a, const ic3_gftower<p,pow> *b);

template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> square(const ic3_gftower<p,pow> &a);

template <unsigned p, unsigned pow>
void sqrt_n_inv(ic3_gftower<p,pow> *s, ic3_gftower<p,pow> *i, const ic3_gftower<p,pow> &a);
template <>
void sqrt_n_inv(ic3_gftower<31,18> *s, ic3_gftower<31,18> *i, const ic3_gftower<31,18> &a);
template <>
void sqrt_n_inv(ic3_gftower<31,15> *s, ic3_gftower<31,15> *i, const ic3_gftower<31,15> &a);
template <>
void sqrt_n_inv(ic3_gftower<31,10> *s, ic3_gftower<31,10> *i, const ic3_gftower<31,10> &a);
template <>
void sqrt_n_inv(ic3_gftower<31,9> *s, ic3_gftower<31,9> *i, const ic3_gftower<31,9> &a);

template <unsigned p, unsigned pow>
struct ic3_gftower {
	typedef ic3_gftower<p, pow> same;
	static const unsigned N = (pow+7)/8;

	inline same& operator= (const same &b){for (unsigned i = N;i--;)v[i]=b.v[i];return *this;}
	inline same& operator+=(const same &b){for (unsigned i = N;i--;)v[i]+=b.v[i]; return *this;}
	inline same& operator-=(const same &b){for (unsigned i = N;i--;)v[i]-=b.v[i]; return *this;}
	inline same& operator*=(const same &b){_mult(this, this, &b);return *this;}
	inline same& operator/=(const same &b){return *this*=invert(b);}
	inline same& operator*=(const ic3_gf<p> &b){ic3_gfv<p> bv=b;for (unsigned i = N;i--;)v[i]*=bv;return *this;}
	inline same& operator/=(const ic3_gf<p> &b){return *this *= invert(b);}

	inline const same operator+(const same &b) const {same r;for (unsigned i = N;i--;)r.v[i]=v[i]+b.v[i]; return r;}
	inline const same operator-(const same &b) const {same r;for (unsigned i = N;i--;)r.v[i]=v[i]-b.v[i];return r;}
	inline const same operator*(const same &b) const {same r;_mult(&r, this, &b); return r;}
	inline const same operator/(const same &b) const {same r;_mult(&r, this, &invert(b)); return r;}
	inline const same operator*(const ic3_gf<p> &b) const {same r;ic3_gfv<p> bv=b;for (unsigned i = N; i--;)r.v[i]=v[i]*bv;return r;}
	inline const same operator/(const ic3_gf<p> &b) const {same r;ic3_gfv<p> bv=invert(b);for (unsigned i = N; i--;)r.v[i]=v[i]*bv;return r;}
	inline const same operator-() const {same r;for (unsigned i = N;i--;)r.v[i]=-v[i];return r;}

	inline bool operator!=(const same &b) const {return *this-b;}
	inline bool operator==(const same &b) const {return !(*this!=b);}

	inline operator bool() const {bool r = false; same tmp=reduce(*this); for (unsigned i = (pow * 2 + sizeof(unsigned long) - 1) / sizeof(unsigned long); i--;) r = r || ((unsigned long*)tmp.v)[i]; return r;}

	void dump(FILE* f) const {for (unsigned i=0;i<pow;i++) fprintf(f, "%hd ", ((short*)v)[i]);fprintf(f,"\n");}

	static inline const same rand() {same r;ic3_gf<p> *rv=(ic3_gf<p>*)&r.v;for(unsigned i=0;i<pow;i++)rv[i]=ic3_gf<p>::rand();for(unsigned i=pow;i<N*8;i++)rv[i]=0;return r;}
	static inline const same load(const void *ptr) {same r; memcpy(&r.v, ptr, sizeof(ic3_gf<p>[pow])); for(unsigned i=pow;i<N*8;i++)((ic3_gf<p>*)&r.v)[i]=0;return r;}
	ic3_gfv<p> v[N];
	
	static inline const same inv( const same &a) { return invert<p,pow>(a); }
	static inline const same powch(const same &a, unsigned k) { return powpk<p,pow>(a,k); }
	inline void rd() { *this = reduce(*this); }
	inline int toInt() const {same tmp=reduce(*this);int r=0; for(int i=0;i<3;i++) r += ((short*)tmp.v)[i]; return r; }
	static const same one;
};

static const short _ic3_gf_31_56_one[56] __attribute__ ((aligned (16))) = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

template <> const ic3_gftower<31,9> ic3_gftower<31,9>::one = *((ic3_gftower<31,9> *)_ic3_gf_31_56_one);
template <> const ic3_gftower<31,10> ic3_gftower<31,10>::one = *((ic3_gftower<31,10> *)_ic3_gf_31_56_one);
template <> const ic3_gftower<31,15> ic3_gftower<31,15>::one = *((ic3_gftower<31,15> *)_ic3_gf_31_56_one);
template <> const ic3_gftower<31,18> ic3_gftower<31,18>::one = *((ic3_gftower<31,18> *)_ic3_gf_31_56_one);
template <> const ic3_gftower<31,30> ic3_gftower<31,30>::one = *((ic3_gftower<31,30> *)_ic3_gf_31_56_one);
template <> const ic3_gftower<31,45> ic3_gftower<31,45>::one = *((ic3_gftower<31,45> *)_ic3_gf_31_56_one);
template <> const ic3_gftower<31,54> ic3_gftower<31,54>::one = *((ic3_gftower<31,54> *)_ic3_gf_31_56_one);

template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> operator*(const ic3_gf<p> &a, const ic3_gftower<p,pow> &b){return b*a;}

template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> reduce(const ic3_gftower<p,pow> &a){
	ic3_gftower<p,pow> r;
	for (unsigned i = ic3_gftower<p,pow>::N; i--;)
		r.v[i] = reducev(a.v[i]);
	return r;
}

template <>
inline const ic3_gftower<31,18> powpk(const ic3_gftower<31,18> &a, unsigned k){
	ic3_gftower<31,18> r;
	short *v = (short *)a.v;

	switch (k){
	case 0:
		return a;
	case 1:
		r.v[0].v = _mm_set_epi16(v[13],v[6],v[17],v[10],v[3],v[14],v[7],v[0]);
		r.v[0].v = _mm_mullo_epi16(r.v[0].v, _mm_set_epi16(14,-6,-10,-9,-5,2,8,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_set_epi16(v[15],v[8],v[1],v[12],v[5],v[16],v[9],v[2]);
		r.v[1].v = _mm_mullo_epi16(r.v[1].v, _mm_set_epi16(6,-7,3,5,-11,-8,-1,-4));
		r.v[1] = reducev(r.v[1]);
		r.v[2].v = _mm_insert_epi16(_mm_insert_epi16(r.v[2].v,v[11],1),v[4],0);
		r.v[2].v = _mm_mullo_epi16(r.v[2].v, _mm_set_epi16(0,0,0,0,0,0,4,-15));	
		r.v[2] = reducev(r.v[2]);
		return r;
	case 2:
		r.v[0].v = _mm_set_epi16(v[1],v[6],v[11],v[16],v[3],v[8],v[13],v[0]);
		r.v[0].v = _mm_mullo_epi16(r.v[0].v, _mm_set_epi16(11,5,-9,10,-6,-14,-12,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_set_epi16(v[15],v[2],v[7],v[12],v[17],v[4],v[9],v[14]);
		r.v[1].v = _mm_mullo_epi16(r.v[1].v, _mm_set_epi16(5,-3,-7,-6,-14,-4,1,-8));
		r.v[1] = reducev(r.v[1]);
		r.v[2].v = _mm_insert_epi16(_mm_insert_epi16(r.v[2].v,v[5],1),v[10],0);
		r.v[2].v = _mm_mullo_epi16(r.v[2].v, _mm_set_epi16(0,0,0,0,0,0,-13,11));	
		r.v[2] = reducev(r.v[2]);
		return r;
	case 4:
		r.v[0].v = _mm_set_epi16(v[13],v[6],v[17],v[10],v[3],v[14],v[7],v[0]);
		r.v[0].v = _mm_mullo_epi16(r.v[0].v, _mm_set_epi16(-8,-6,2,-14,5,-12,-9,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_set_epi16(v[15],v[8],v[1],v[12],v[5],v[16],v[9],v[2]);
		r.v[1].v = _mm_mullo_epi16(r.v[1].v, _mm_set_epi16(-6,11,-15,5,-4,-9,1,-7));
		r.v[1] = reducev(r.v[1]);
		r.v[2].v = _mm_insert_epi16(_mm_insert_epi16(r.v[2].v,v[11],1),v[4],0);
		r.v[2].v = _mm_mullo_epi16(r.v[2].v, _mm_set_epi16(0,0,0,0,0,0,-7,-13));	
		r.v[2] = reducev(r.v[2]);
		return r;
	case 8:
		r.v[0].v = _mm_set_epi16(v[1],v[6],v[11],v[16],v[3],v[8],v[13],v[0]);
		r.v[0].v = _mm_mullo_epi16(r.v[0].v, _mm_set_epi16(-4,5,-14,2,-6,-8,10,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_set_epi16(v[15],v[2],v[7],v[12],v[17],v[4],v[9],v[14]);
		r.v[1].v = _mm_mullo_epi16(r.v[1].v, _mm_set_epi16(5,-15,11,-6,-8,-7,1,-9));
		r.v[1] = reducev(r.v[1]);
		r.v[2].v = _mm_insert_epi16(_mm_insert_epi16(r.v[2].v,v[5],1),v[10],0);
		r.v[2].v = _mm_mullo_epi16(r.v[2].v, _mm_set_epi16(0,0,0,0,0,0,-3,-4));
		r.v[2] = reducev(r.v[2]);
		return r;
	default:
		while(1) ; //shall not reach
	}
}

template <>
inline const ic3_gftower<31,15> powpk(const ic3_gftower<31,15> &a, unsigned k)
{
	ic3_gftower<31,15> r;

	switch (k)
	{
	case 0:
		return a;
	case 1:
		r.v[0].v = _mm_mullo_epi16(a.v[0].v, _mm_set_epi16(10,8,-6,-11,16,-12,9,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_mullo_epi16(a.v[1].v, _mm_set_epi16(0,7,-13,2,14,5,4,-3));
		r.v[1] = reducev(r.v[1]);
		return r;
	case 2:
		r.v[0].v = _mm_mullo_epi16(a.v[0].v, _mm_set_epi16(7,2,5,-3,8,-11,-12,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_mullo_epi16(a.v[1].v, _mm_set_epi16(0,-13,14,4,10,-6,16,9));
		r.v[1] = reducev(r.v[1]);
		return r;
	case 3:
		r.v[0].v = _mm_mullo_epi16(a.v[0].v, _mm_set_epi16(8,16,1,2,4,8,16,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_mullo_epi16(a.v[1].v, _mm_set_epi16(0,2,4,8,16,1,2,4));
		r.v[1] = reducev(r.v[1]);
		return r;
	case 4:
		r.v[0].v = _mm_mullo_epi16(a.v[0].v, _mm_set_epi16(-13,4,-6,9,2,-3,-11,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_mullo_epi16(a.v[1].v, _mm_set_epi16(0,14,10,16,7,5,8,-12));
		r.v[1] = reducev(r.v[1]);
		return r;
	case 6:
		r.v[0].v = _mm_mullo_epi16(a.v[0].v, _mm_set_epi16(2,8,1,4,16,2,8,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_mullo_epi16(a.v[1].v, _mm_set_epi16(0,4,16,2,8,1,4,16));
		r.v[1] = reducev(r.v[1]);
		return r;
	default:
		while(1) ; //shall not reach
	}
}

template <>
inline const ic3_gftower<31,10> powpk(const ic3_gftower<31,10> &a, unsigned k)
{
	ic3_gftower<31,10> r;

	switch (k)
	{
	case 0:
		return a;
	case 1:
		r.v[0].v = _mm_mullo_epi16(a.v[0].v, _mm_set_epi16(15,4,-1,8,-2,16,-4,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_mullo_epi16(a.v[1].v, _mm_set_epi16(0,0,0,0,0,0,-8,2));
		r.v[1] = reducev(r.v[1]);
		return r;
	case 2:
		r.v[0].v = _mm_mullo_epi16(a.v[0].v, _mm_set_epi16(8,16,1,2,4,8,16,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_mullo_epi16(a.v[1].v, _mm_set_epi16(0,0,0,0,0,0,2,4));
		r.v[1] = reducev(r.v[1]);
		return r;
	case 4:
		r.v[0].v = _mm_mullo_epi16(a.v[0].v, _mm_set_epi16(2,8,1,4,16,2,8,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_mullo_epi16(a.v[1].v, _mm_set_epi16(0,0,0,0,0,0,4,16));
		r.v[1] = reducev(r.v[1]);
		return r;
	default:
		while(1) ; //shall not reach
	}
}

template <>
inline const ic3_gftower<31,9> powpk(const ic3_gftower<31,9> &a, unsigned k)
{
	ic3_gftower<31,9> r;
	short *v = (short *)a.v;

	switch (k)
	{
	case 0:
		return a;
	case 1:
		r.v[0].v = _mm_set_epi16(v[4],v[6],v[8],v[1],v[3],v[5],v[7],v[0]);
		r.v[0].v = _mm_mullo_epi16(r.v[0].v, _mm_set_epi16(-7,5,-8,-4,-6,-9,2,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_insert_epi16(r.v[1].v,v[2],0);
		r.v[1].v = _mm_mullo_epi16(r.v[1].v, _mm_set_epi16(0,0,0,0,0,0,0,16));
		r.v[1] = reducev(r.v[1]);
		return r;
	case 2:
		r.v[0].v = _mm_set_epi16(v[1],v[6],v[2],v[7],v[3],v[8],v[4],v[0]);
		r.v[0].v = _mm_mullo_epi16(r.v[0].v, _mm_set_epi16(-3,-6,-4,-8,5,10,-14,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_insert_epi16(r.v[1].v,v[5],0);
		r.v[1].v = _mm_mullo_epi16(r.v[1].v, _mm_set_epi16(0,0,0,0,0,0,0,11));
		r.v[1] = reducev(r.v[1]);
		return r;
	case 4:
		r.v[0].v = _mm_set_epi16(v[4],v[6],v[8],v[1],v[3],v[5],v[7],v[0]);
		r.v[0].v = _mm_mullo_epi16(r.v[0].v, _mm_set_epi16(11,5,-9,-7,-6,-14,-12,1));
		r.v[0] = reducev(r.v[0]);
		r.v[1].v = _mm_insert_epi16(r.v[1].v,v[2],0);
		r.v[1].v = _mm_mullo_epi16(r.v[1].v, _mm_set_epi16(0,0,0,0,0,0,0,-13));
		r.v[1] = reducev(r.v[1]);
		return r;
	default:
		while(1) ; //shall not reach
	}
}

template <>
inline const ic3_gftower<31,18> mult_u(const ic3_gftower<31,18> &a)
{
	ic3_gftower<31,18> r;
	r.v[0].v = _mm_insert_epi16(_mm_slli_si128(a.v[0].v, 2), _mm_extract_epi16(a.v[2].v, 1) * 3, 0);
	r.v[1].v = _mm_or_si128(_mm_slli_si128(a.v[1].v, 2),_mm_srli_si128(a.v[0].v, 14));
	r.v[2].v = _mm_or_si128(_mm_srli_si128(_mm_slli_si128(a.v[2].v, 14), 12),_mm_srli_si128(a.v[1].v, 14));
	return r;
}

template <>
inline const ic3_gftower<31,15> mult_u(const ic3_gftower<31,15> &a)
{
	ic3_gftower<31,15> r;

	r.v[0].v = _mm_insert_epi16(_mm_slli_si128(a.v[0].v, 2), _mm_extract_epi16(a.v[1].v, 6) * 3, 0);
	r.v[1].v = _mm_or_si128(_mm_srli_si128(_mm_slli_si128(a.v[1].v, 4), 2),_mm_srli_si128(a.v[0].v, 14));
	return r;
}

template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> square(const ic3_gftower<p,pow> &a){return a*a;}

template <>
const ic3_gftower<31,18> square(const ic3_gftower<31,18> &a);
template <>
const ic3_gftower<31,15> square(const ic3_gftower<31,15> &a);
template <>
const ic3_gftower<31,10> square(const ic3_gftower<31,10> &a);
template <>
const ic3_gftower<31,9> square(const ic3_gftower<31,9> &a);
template <>
const ic3_gftower<31,30> square(const ic3_gftower<31,30> &a);
template <>
const ic3_gftower<31,45> square(const ic3_gftower<31,45> &a);
template <>
const ic3_gftower<31,54> square(const ic3_gftower<31,54> &a);

#include <emmintrin.h>
#ifdef __SSSE3__
#include <tmmintrin.h>
#endif
#include "tower.h"

inline static __m128i ic3_extract(__m128i a, int i){
	if (i == 0)
		return _mm_shuffle_epi32(_mm_unpacklo_epi16(a, a), 0);
	else if (i == 1)
		return _mm_shuffle_epi32(_mm_unpacklo_epi16(a, a), 85);
	else if (i == 2)
		return _mm_shuffle_epi32(_mm_unpacklo_epi16(a, a), 170);
	else if (i == 3)
		return _mm_shuffle_epi32(_mm_unpacklo_epi16(a, a), 255);
	else if (i == 4)
		return _mm_shuffle_epi32(_mm_unpackhi_epi16(a, a), 0);
	else if (i == 5)
		return _mm_shuffle_epi32(_mm_unpackhi_epi16(a, a), 85);
	else if (i == 6)
		return _mm_shuffle_epi32(_mm_unpackhi_epi16(a, a), 170);
	else if (i == 7)
		return _mm_shuffle_epi32(_mm_unpackhi_epi16(a, a), 255);
	while(1) ; // shall not reach
}

#ifdef __SSSE3__
inline static __m128i ic3_srli2 (__m128i a1, __m128i a0, int i){
	if (i == 0)
		return _mm_alignr_epi8(a1, a0, 0);
	if (i == 1)
		return _mm_alignr_epi8(a1, a0, 1*2);
	else if (i == 2)
		return _mm_alignr_epi8(a1, a0, 2*2);
	else if (i == 3)
		return _mm_alignr_epi8(a1, a0, 3*2);
	else if (i == 4)
		return _mm_alignr_epi8(a1, a0, 4*2);
	else if (i == 5)
		return _mm_alignr_epi8(a1, a0, 5*2);
	else if (i == 6)
		return _mm_alignr_epi8(a1, a0, 6*2);
	else
		return _mm_alignr_epi8(a1, a0, 7*2);
	// return _mm_alignr_epi8(a1, a0, i*2);
}
#else
//inline static __m128i ic3_srli2 (__m128i a1, __m128i a0, int i){
//return _mm_or_si128(_mm_slli_si128(a1, 16-i*2),_mm_srli_si128(a0, i*2));
//}
#define ic3_srli2(a1,a0,i) (_mm_or_si128(_mm_slli_si128((a1),(16-(i)*2)),_mm_srli_si128((a0),((i)*2))))
#endif

#define ADD(a,b) (_mm_add_epi16((a),(b)))
#define MUL(a,b) (_mm_mullo_epi16((a),(b)))
#define SL(a,i) (_mm_slli_si128((a),(i)*2))
#define SR(a,i) (_mm_srli_si128((a),(i)*2))

template <>
void _mult(ic3_gftower<31,18> *c, const ic3_gftower<31,18> *a, const ic3_gftower<31,18> *b){
	__m128i c0, c1, c2, c3, c4;

	{
		__m128i a0, a1;
		__m128i t0, t1;
		__m128i bs;

		a0 = a->v[0].v; a1 = a->v[1].v;

		bs = ic3_extract(b->v[0].v, 0); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c0 = t0; c1 = t1;
		bs = ic3_extract(b->v[0].v, 1); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c0 = ADD(c0, SL(t0, 1)); c1 = ADD(c1, ic3_srli2(t1, t0, 7)); c2 = SR(t1, 7);
		bs = ic3_extract(b->v[0].v, 2); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c0 = ADD(c0, SL(t0, 2)); c1 = ADD(c1, ic3_srli2(t1, t0, 6)); c2 = ADD(c2, SR(t1, 6));
		bs = ic3_extract(b->v[0].v, 3); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c0 = ADD(c0, SL(t0, 3)); c1 = ADD(c1, ic3_srli2(t1, t0, 5)); c2 = ADD(c2, SR(t1, 5));
		bs = ic3_extract(b->v[0].v, 4); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c0 = ADD(c0, SL(t0, 4)); c1 = ADD(c1, ic3_srli2(t1, t0, 4)); c2 = ADD(c2, SR(t1, 4));
		bs = ic3_extract(b->v[0].v, 5); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c0 = ADD(c0, SL(t0, 5)); c1 = ADD(c1, ic3_srli2(t1, t0, 3)); c2 = ADD(c2, SR(t1, 3));
		bs = ic3_extract(b->v[0].v, 6); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c0 = ADD(c0, SL(t0, 6)); c1 = ADD(c1, ic3_srli2(t1, t0, 2)); c2 = ADD(c2, SR(t1, 2));
		bs = ic3_extract(b->v[0].v, 7); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c0 = ADD(c0, SL(t0, 7)); c1 = ADD(c1, ic3_srli2(t1, t0, 1)); c2 = ADD(c2, SR(t1, 1));
		bs = ic3_extract(b->v[1].v, 0); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c1 = ADD(c1, t0); c2 = ADD(c2, t1);
		bs = ic3_extract(b->v[1].v, 1); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c1 = ADD(c1, SL(t0, 1)); c2 = ADD(c2, ic3_srli2(t1, t0, 7)); c3 = SR(t1, 7);
		bs = ic3_extract(b->v[1].v, 2); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c1 = ADD(c1, SL(t0, 2)); c2 = ADD(c2, ic3_srli2(t1, t0, 6)); c3 = ADD(c3, SR(t1, 6));
		bs = ic3_extract(b->v[1].v, 3); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c1 = ADD(c1, SL(t0, 3)); c2 = ADD(c2, ic3_srli2(t1, t0, 5)); c3 = ADD(c3, SR(t1, 5));
		bs = ic3_extract(b->v[1].v, 4); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c1 = ADD(c1, SL(t0, 4)); c2 = ADD(c2, ic3_srli2(t1, t0, 4)); c3 = ADD(c3, SR(t1, 4));
		bs = ic3_extract(b->v[1].v, 5); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c1 = ADD(c1, SL(t0, 5)); c2 = ADD(c2, ic3_srli2(t1, t0, 3)); c3 = ADD(c3, SR(t1, 3));
		bs = ic3_extract(b->v[1].v, 6); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c1 = ADD(c1, SL(t0, 6)); c2 = ADD(c2, ic3_srli2(t1, t0, 2)); c3 = ADD(c3, SR(t1, 2));
		bs = ic3_extract(b->v[1].v, 7); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c1 = ADD(c1, SL(t0, 7)); c2 = ADD(c2, ic3_srli2(t1, t0, 1)); c3 = ADD(c3, SR(t1, 1));
		bs = ic3_extract(b->v[2].v, 0); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c2 = ADD(c2, t0); c3 = ADD(c3, t1);
		bs = ic3_extract(b->v[2].v, 1); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
		c2 = ADD(c2, SL(t0, 1)); c3 = ADD(c3, ic3_srli2(t1, t0, 7)); c4 = SR(t1, 7);
	}
	{
		__m128i a0, a1, a2;
		__m128i t0, t1, t2;
		__m128i bs;

		a0 = b->v[0].v; a1 = b->v[1].v; a2 = b->v[2].v;
		bs = ic3_extract(a->v[2].v, 0); t0 = MUL(a0, bs); t1 = MUL(a1, bs); t2 = MUL(a2, bs);
		c2 = ADD(c2, t0); c3 = ADD(c3, t1); c4 = ADD(c4, t2);
		bs = ic3_extract(a->v[2].v, 1); t0 = MUL(a0, bs); t1 = MUL(a1, bs); t2 = MUL(a2, bs);
		c2 = ADD(c2, SL(t0, 1)); c3 = ADD(c3, ic3_srli2(t1, t0, 7)); c4 = ADD(c4, ic3_srli2(t2, t1, 7));
	}

	__m128i three = _mm_set1_epi16(3);
	c0 = ADD(c0, MUL(ic3_srli2(c3, c2, 2),three));
	c1 = ADD(c1, MUL(ic3_srli2(c4, c3, 2),three));
	c2 = ADD(c2, MUL(SR(c4, 2), three));
	c->v[0] = reducev((ic3_gfv<31>) c0);
	c->v[1] = reducev((ic3_gfv<31>) c1);
	c->v[2] = reducev((ic3_gfv<31>) SR(SL(c2,6),6));
}

template <>
const ic3_gftower<31,18> square(const ic3_gftower<31,18> &a){
	__m128i c0, c1, c2, c3, c4;

	__m128i a0, a1, a2;
	__m128i t0, t1, t2;
	__m128i bs, bs2;

	a0 = a.v[0].v; a1 = a.v[1].v; a2 = a.v[2].v;

	bs = ic3_extract(a.v[0].v, 0); t0 = MUL(a0, bs);
	c0 = t0;
	bs = ic3_extract(a.v[0].v, 1); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 1)); c1 = SR(t0, 7);
	bs = ic3_extract(a.v[0].v, 2); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 2)); c1 = ADD(c1, SR(t0, 6));
	bs = ic3_extract(a.v[0].v, 3); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 3)); c1 = ADD(c1, SR(t0, 5));
	bs = ic3_extract(a.v[0].v, 4); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 4)); c1 = ADD(c1, SR(t0, 4));
	bs = ic3_extract(a.v[0].v, 5); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 5)); c1 = ADD(c1, SR(t0, 3));
	bs = ic3_extract(a.v[0].v, 6); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 6)); c1 = ADD(c1, SR(t0, 2));
	bs = ic3_extract(a.v[0].v, 7); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 7)); c1 = ADD(c1, SR(t0, 1));

	bs = ic3_extract(a.v[1].v, 0); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, t0); c2 = t1;
	bs = ic3_extract(a.v[1].v, 1); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 1)); c2 = ADD(c2, ic3_srli2(t1, t0, 7)); c3 = SR(t1, 7);
	bs = ic3_extract(a.v[1].v, 2); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 2)); c2 = ADD(c2, ic3_srli2(t1, t0, 6)); c3 = ADD(c3, SR(t1, 6));
	bs = ic3_extract(a.v[1].v, 3); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 3)); c2 = ADD(c2, ic3_srli2(t1, t0, 5)); c3 = ADD(c3, SR(t1, 5));
	bs = ic3_extract(a.v[1].v, 4); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 4)); c2 = ADD(c2, ic3_srli2(t1, t0, 4)); c3 = ADD(c3, SR(t1, 4));
	bs = ic3_extract(a.v[1].v, 5); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 5)); c2 = ADD(c2, ic3_srli2(t1, t0, 3)); c3 = ADD(c3, SR(t1, 3));
	bs = ic3_extract(a.v[1].v, 6); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 6)); c2 = ADD(c2, ic3_srli2(t1, t0, 2)); c3 = ADD(c3, SR(t1, 2));
	bs = ic3_extract(a.v[1].v, 7); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 7)); c2 = ADD(c2, ic3_srli2(t1, t0, 1)); c3 = ADD(c3, SR(t1, 1));

	bs = ic3_extract(a.v[2].v, 0); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs2); t2 = MUL(a2, bs);
	c2 = ADD(c2, t0); c3 = ADD(c3, t1); c4 = t2;
	bs = ic3_extract(a.v[2].v, 1); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs2); t2 = MUL(a2, bs);
	c2 = ADD(c2, SL(t0, 1)); c3 = ADD(c3, ic3_srli2(t1, t0, 7)); c4 = ADD(c4, ic3_srli2(t2, t1, 7));

	__m128i three = _mm_set1_epi16(3);
	c0 = ADD(c0, MUL(ic3_srli2(c3, c2, 2),three));
	c1 = ADD(c1, MUL(ic3_srli2(c4, c3, 2),three));
	c2 = ADD(c2, MUL(SR(c4, 2), three));

	ic3_gftower<31,18> c;
	c.v[0] = reducev((ic3_gfv<31>) c0);
	c.v[1] = reducev((ic3_gfv<31>) c1);
	c.v[2] = reducev((ic3_gfv<31>) SR(SL(c2,6),6));
	return c;
}

template <>
void _mult(ic3_gftower<31,15> *c, const ic3_gftower<31,15> *a, const ic3_gftower<31,15> *b){
	__m128i c0, c1, c2, c3;

	__m128i a0, a1;
	__m128i t0, t1;
	__m128i bs;

	a0 = a->v[0].v; a1 = a->v[1].v;

	bs = ic3_extract(b->v[0].v, 0); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c0 = t0; c1 = t1;
	bs = ic3_extract(b->v[0].v, 1); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c0 = ADD(c0, SL(t0, 1)); c1 = ADD(c1, ic3_srli2(t1, t0, 7)); c2 = SR(t1, 7);
	bs = ic3_extract(b->v[0].v, 2); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c0 = ADD(c0, SL(t0, 2)); c1 = ADD(c1, ic3_srli2(t1, t0, 6)); c2 = ADD(c2, SR(t1, 6));
	bs = ic3_extract(b->v[0].v, 3); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c0 = ADD(c0, SL(t0, 3)); c1 = ADD(c1, ic3_srli2(t1, t0, 5)); c2 = ADD(c2, SR(t1, 5));
	bs = ic3_extract(b->v[0].v, 4); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c0 = ADD(c0, SL(t0, 4)); c1 = ADD(c1, ic3_srli2(t1, t0, 4)); c2 = ADD(c2, SR(t1, 4));
	bs = ic3_extract(b->v[0].v, 5); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c0 = ADD(c0, SL(t0, 5)); c1 = ADD(c1, ic3_srli2(t1, t0, 3)); c2 = ADD(c2, SR(t1, 3));
	bs = ic3_extract(b->v[0].v, 6); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c0 = ADD(c0, SL(t0, 6)); c1 = ADD(c1, ic3_srli2(t1, t0, 2)); c2 = ADD(c2, SR(t1, 2));
	bs = ic3_extract(b->v[0].v, 7); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c0 = ADD(c0, SL(t0, 7)); c1 = ADD(c1, ic3_srli2(t1, t0, 1)); c2 = ADD(c2, SR(t1, 1));
	bs = ic3_extract(b->v[1].v, 0); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, t0); c2 = ADD(c2, t1);
	bs = ic3_extract(b->v[1].v, 1); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 1)); c2 = ADD(c2, ic3_srli2(t1, t0, 7)); c3 = SR(t1, 7);
	bs = ic3_extract(b->v[1].v, 2); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 2)); c2 = ADD(c2, ic3_srli2(t1, t0, 6)); c3 = ADD(c3, SR(t1, 6));
	bs = ic3_extract(b->v[1].v, 3); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 3)); c2 = ADD(c2, ic3_srli2(t1, t0, 5)); c3 = ADD(c3, SR(t1, 5));
	bs = ic3_extract(b->v[1].v, 4); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 4)); c2 = ADD(c2, ic3_srli2(t1, t0, 4)); c3 = ADD(c3, SR(t1, 4));
	bs = ic3_extract(b->v[1].v, 5); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 5)); c2 = ADD(c2, ic3_srli2(t1, t0, 3)); c3 = ADD(c3, SR(t1, 3));
	bs = ic3_extract(b->v[1].v, 6); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 6)); c2 = ADD(c2, ic3_srli2(t1, t0, 2)); c3 = ADD(c3, SR(t1, 2));

	__m128i three = _mm_set1_epi16(3);
	c0 = ADD(c0, MUL(ic3_srli2(c2, c1, 7),three));
	c1 = ADD(c1, MUL(ic3_srli2(c3, c2, 7),three));
	c->v[0] = reducev((ic3_gfv<31>) c0);
	c->v[1] = reducev((ic3_gfv<31>) SR(SL(c1,1),1));
}

template <>
const ic3_gftower<31,15> square(const ic3_gftower<31,15> &a){
	__m128i c0, c1, c2, c3;

	__m128i a0, a1;
	__m128i t0, t1;
	__m128i bs, bs2;

	a0 = a.v[0].v; a1 = a.v[1].v;

	bs = ic3_extract(a.v[0].v, 0); t0 = MUL(a0, bs);
	c0 = t0;
	bs = ic3_extract(a.v[0].v, 1); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 1)); c1 = SR(t0, 7);
	bs = ic3_extract(a.v[0].v, 2); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 2)); c1 = ADD(c1, SR(t0, 6));
	bs = ic3_extract(a.v[0].v, 3); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 3)); c1 = ADD(c1, SR(t0, 5));
	bs = ic3_extract(a.v[0].v, 4); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 4)); c1 = ADD(c1, SR(t0, 4));
	bs = ic3_extract(a.v[0].v, 5); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 5)); c1 = ADD(c1, SR(t0, 3));
	bs = ic3_extract(a.v[0].v, 6); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 6)); c1 = ADD(c1, SR(t0, 2));
	bs = ic3_extract(a.v[0].v, 7); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 7)); c1 = ADD(c1, SR(t0, 1));

	bs = ic3_extract(a.v[1].v, 0); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, t0); c2 = t1;
	bs = ic3_extract(a.v[1].v, 1); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 1)); c2 = ADD(c2, ic3_srli2(t1, t0, 7)); c3 = SR(t1, 7);
	bs = ic3_extract(a.v[1].v, 2); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 2)); c2 = ADD(c2, ic3_srli2(t1, t0, 6)); c3 = ADD(c3, SR(t1, 6));
	bs = ic3_extract(a.v[1].v, 3); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 3)); c2 = ADD(c2, ic3_srli2(t1, t0, 5)); c3 = ADD(c3, SR(t1, 5));
	bs = ic3_extract(a.v[1].v, 4); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 4)); c2 = ADD(c2, ic3_srli2(t1, t0, 4)); c3 = ADD(c3, SR(t1, 4));
	bs = ic3_extract(a.v[1].v, 5); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 5)); c2 = ADD(c2, ic3_srli2(t1, t0, 3)); c3 = ADD(c3, SR(t1, 3));
	bs = ic3_extract(a.v[1].v, 6); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 6)); c2 = ADD(c2, ic3_srli2(t1, t0, 2)); c3 = ADD(c3, SR(t1, 2));

	__m128i three = _mm_set1_epi16(3);
	c0 = ADD(c0, MUL(ic3_srli2(c2, c1, 7),three));
	c1 = ADD(c1, MUL(ic3_srli2(c3, c2, 7),three));

	ic3_gftower<31,15> c;
	c.v[0] = reducev((ic3_gfv<31>) c0);
	c.v[1] = reducev((ic3_gfv<31>) SR(SL(c1,1),1));

	return c;
}

template <>
void _mult(ic3_gftower<31,10> *c, const ic3_gftower<31,10> *a, const ic3_gftower<31,10> *b)
{
	__m128i c0, c1, c2;

	__m128i a0, a1;
	__m128i t0, t1;
	__m128i bs;

	a0 = a->v[0].v;

	bs = ic3_extract(b->v[0].v, 0); t0 = MUL(a0, bs);
	c0 = t0;
	bs = ic3_extract(b->v[0].v, 1); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 1)); c1 = SR(t0, 7);
	bs = ic3_extract(b->v[0].v, 2); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 2)); c1 = ADD(c1, SR(t0, 6));
	bs = ic3_extract(b->v[0].v, 3); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 3)); c1 = ADD(c1, SR(t0, 5));
	bs = ic3_extract(b->v[0].v, 4); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 4)); c1 = ADD(c1, SR(t0, 4));
	bs = ic3_extract(b->v[0].v, 5); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 5)); c1 = ADD(c1, SR(t0, 3));
	bs = ic3_extract(b->v[0].v, 6); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 6)); c1 = ADD(c1, SR(t0, 2));
	bs = ic3_extract(b->v[0].v, 7); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 7)); c1 = ADD(c1, SR(t0, 1));
	bs = ic3_extract(b->v[1].v, 0); t0 = MUL(a0, bs);
	c1 = ADD(c1, t0);
	bs = ic3_extract(b->v[1].v, 1); t0 = MUL(a0, bs);
	c1 = ADD(c1, SL(t0, 1)); c2 = SR(t0, 7);

	a0 = b->v[0].v; a1 = b->v[1].v;
	
	bs = ic3_extract(a->v[1].v, 0); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, t0); c2 = ADD(c2, t1);
	bs = ic3_extract(a->v[1].v, 1); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 1)); c2 = ADD(c2, ic3_srli2(t1, t0, 7));

	__m128i three = _mm_set1_epi16(3);
	c0 = ADD(c0, MUL(ic3_srli2(c2, c1, 2),three));
	c1 = ADD(c1, MUL(SR(c2, 2),three));

	c->v[0] = reducev((ic3_gfv<31>) c0);
	c->v[1] = reducev((ic3_gfv<31>) SR(SL(c1,6),6));
}

template <>
const ic3_gftower<31,10> square(const ic3_gftower<31,10> &a)
{
	__m128i c0, c1, c2;

	__m128i a0, a1;
	__m128i t0, t1;
	__m128i bs, bs2;

	a0 = a.v[0].v; a1 = a.v[1].v;

	bs = ic3_extract(a.v[0].v, 0); t0 = MUL(a0, bs);
	c0 = t0;
	bs = ic3_extract(a.v[0].v, 1); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 1)); c1 = SR(t0, 7);
	bs = ic3_extract(a.v[0].v, 2); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 2)); c1 = ADD(c1, SR(t0, 6));
	bs = ic3_extract(a.v[0].v, 3); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 3)); c1 = ADD(c1, SR(t0, 5));
	bs = ic3_extract(a.v[0].v, 4); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 4)); c1 = ADD(c1, SR(t0, 4));
	bs = ic3_extract(a.v[0].v, 5); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 5)); c1 = ADD(c1, SR(t0, 3));
	bs = ic3_extract(a.v[0].v, 6); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 6)); c1 = ADD(c1, SR(t0, 2));
	bs = ic3_extract(a.v[0].v, 7); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 7)); c1 = ADD(c1, SR(t0, 1));

	bs = ic3_extract(a.v[1].v, 0); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, t0); c2 = t1;
	bs = ic3_extract(a.v[1].v, 1); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, SL(t0, 1)); c2 = ADD(c2, ic3_srli2(t1, t0, 7));

	__m128i three = _mm_set1_epi16(3);
	c0 = ADD(c0, MUL(ic3_srli2(c2, c1, 2),three));
	c1 = ADD(c1, MUL(SR(c2, 2),three));

	ic3_gftower<31,10> c;
	c.v[0] = reducev((ic3_gfv<31>) c0);
	c.v[1] = reducev((ic3_gfv<31>) SR(SL(c1,6),6));

	return c;
}

template <>
void _mult(ic3_gftower<31,9> *c, const ic3_gftower<31,9> *a, const ic3_gftower<31,9> *b)
{
	__m128i c0, c1, c2;

	__m128i a0, a1;
	__m128i t0, t1;
	__m128i bs;

	a0 = a->v[0].v;

	bs = ic3_extract(b->v[0].v, 0); t0 = MUL(a0, bs);
	c0 = t0;
	bs = ic3_extract(b->v[0].v, 1); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 1)); c1 = SR(t0, 7);
	bs = ic3_extract(b->v[0].v, 2); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 2)); c1 = ADD(c1, SR(t0, 6));
	bs = ic3_extract(b->v[0].v, 3); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 3)); c1 = ADD(c1, SR(t0, 5));
	bs = ic3_extract(b->v[0].v, 4); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 4)); c1 = ADD(c1, SR(t0, 4));
	bs = ic3_extract(b->v[0].v, 5); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 5)); c1 = ADD(c1, SR(t0, 3));
	bs = ic3_extract(b->v[0].v, 6); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 6)); c1 = ADD(c1, SR(t0, 2));
	bs = ic3_extract(b->v[0].v, 7); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 7)); c1 = ADD(c1, SR(t0, 1));
	bs = ic3_extract(b->v[1].v, 0); t0 = MUL(a0, bs);
	c1 = ADD(c1, t0);

	a0 = b->v[0].v; a1 = b->v[1].v;

	bs = ic3_extract(a->v[1].v, 0); t0 = MUL(a0, bs); t1 = MUL(a1, bs);
	c1 = ADD(c1, t0); c2 = t1;

	__m128i three = _mm_set1_epi16(3);
	c0 = ADD(c0, MUL(ic3_srli2(c2, c1, 1),three));

	c->v[0] = reducev((ic3_gfv<31>) c0);
	c->v[1] = reducev((ic3_gfv<31>) SR(SL(c1,7),7));
}

template <>
const ic3_gftower<31,9> square(const ic3_gftower<31,9> &a)
{
	__m128i c0, c1, c2;

	__m128i a0, a1;
	__m128i t0, t1;
	__m128i bs, bs2;

	a0 = a.v[0].v; a1 = a.v[1].v;

	bs = ic3_extract(a.v[0].v, 0); t0 = MUL(a0, bs);
	c0 = t0;
	bs = ic3_extract(a.v[0].v, 1); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 1)); c1 = SR(t0, 7);
	bs = ic3_extract(a.v[0].v, 2); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 2)); c1 = ADD(c1, SR(t0, 6));
	bs = ic3_extract(a.v[0].v, 3); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 3)); c1 = ADD(c1, SR(t0, 5));
	bs = ic3_extract(a.v[0].v, 4); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 4)); c1 = ADD(c1, SR(t0, 4));
	bs = ic3_extract(a.v[0].v, 5); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 5)); c1 = ADD(c1, SR(t0, 3));
	bs = ic3_extract(a.v[0].v, 6); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 6)); c1 = ADD(c1, SR(t0, 2));
	bs = ic3_extract(a.v[0].v, 7); t0 = MUL(a0, bs);
	c0 = ADD(c0, SL(t0, 7)); c1 = ADD(c1, SR(t0, 1));

	bs = ic3_extract(a.v[1].v, 0); bs2 = ADD(bs, bs); t0 = MUL(a0, bs2); t1 = MUL(a1, bs);
	c1 = ADD(c1, t0); c2 = t1;

	__m128i three = _mm_set1_epi16(3);
	c0 = ADD(c0, MUL(ic3_srli2(c2, c1, 1),three));

	ic3_gftower<31,9> c;
	c.v[0] = reducev((ic3_gfv<31>) c0);
	c.v[1] = reducev((ic3_gfv<31>) SR(SL(c1,7),7));

	return c;
}

template <>
void _mult(ic3_gftower<31,30> *c, const ic3_gftower<31,30> *a, const ic3_gftower<31,30> *b) //GF(31^15)[t]/t^2-u
{
	ic3_gftower<31,15> a0, a1, b0, b1;
	ic3_gftower<31,15> c0, c1;
	ic3_gftower<31,15> ab0, ab1;

	a0.v[0].v = a->v[0].v; a0.v[1].v = SR(SL(a->v[1].v,1),1);
	a1.v[0].v = ic3_srli2(a->v[2].v, a->v[1].v, 7); a1.v[1].v = SR(SL(ic3_srli2(a->v[3].v, a->v[2].v, 7),1),1);
	b0.v[0].v = b->v[0].v; b0.v[1].v = SR(SL(b->v[1].v,1),1);
	b1.v[0].v = ic3_srli2(b->v[2].v, b->v[1].v, 7); b1.v[1].v = SR(SL(ic3_srli2(b->v[3].v, b->v[2].v, 7),1),1);

	ab1 = a1*b1; ab0 = a0*b0;
	c1 = reduce( reduce(a1+a0)*(b1+b0) - ab1 - ab0 );
	c0 = reduce( mult_u(ab1) + (ab0) );

	//need reduce?
//	c0.v[0] = reducev((ic3_gfv<31>) c0.v[0].v); c0.v[1] = reducev((ic3_gfv<31>) c0.v[1].v);
//	c1.v[0] = reducev((ic3_gfv<31>) c1.v[0].v); c1.v[1] = reducev((ic3_gfv<31>) c1.v[1].v);
	c->v[0].v = c0.v[0].v; c->v[1].v = ic3_srli2(c1.v[0].v, SL(c0.v[1].v, 1), 1);
	c->v[2].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 1); c->v[3].v = SR(c1.v[1].v, 1);
}

template <>
const ic3_gftower<31,30> square(const ic3_gftower<31,30> &a)
{
	ic3_gftower<31,15> a0, a1;
	ic3_gftower<31,15> c0, c1;
	ic3_gftower<31,30> c;

	a0.v[0].v = a.v[0].v; a0.v[1].v = SR(SL(a.v[1].v,1),1);
	a1.v[0].v = ic3_srli2(a.v[2].v, a.v[1].v, 7); a1.v[1].v = SR(SL(ic3_srli2(a.v[3].v, a.v[2].v, 7),1),1);
	
	c1 = a0*a1; c1 = reduce(c1+c1); c0 = reduce( mult_u(square(a1)) + square(a0) );

	c.v[0].v = c0.v[0].v; c.v[1].v = ic3_srli2(c1.v[0].v, SL(c0.v[1].v, 1), 1);
	c.v[2].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 1); c.v[3].v = SR(c1.v[1].v, 1);

	return c;
}

template <>
void _mult(ic3_gftower<31,45> *c, const ic3_gftower<31,45> *a, const ic3_gftower<31,45> *b) //GF(31^15)[t]/t^3-u
{
	ic3_gftower<31,15> a0, a1, a2, b0, b1, b2;
	ic3_gftower<31,15> c0, c1, c2;
	ic3_gftower<31,15> ab0, ab1, ab2, ab01, ab02, ab12;

	a0.v[0].v = a->v[0].v; a0.v[1].v = SR(SL(a->v[1].v,1),1); 
	a1.v[0].v = ic3_srli2(a->v[2].v, a->v[1].v, 7); a1.v[1].v = SR(SL(ic3_srli2(a->v[3].v, a->v[2].v, 7),1),1);
	a2.v[0].v = ic3_srli2(a->v[4].v, a->v[3].v, 6); a2.v[1].v = SR(SL(ic3_srli2(a->v[5].v, a->v[4].v, 6),1),1);
	b0.v[0].v = b->v[0].v; b0.v[1].v = SR(SL(b->v[1].v,1),1);
	b1.v[0].v = ic3_srli2(b->v[2].v, b->v[1].v, 7); b1.v[1].v = SR(SL(ic3_srli2(b->v[3].v, b->v[2].v, 7),1),1);
	b2.v[0].v = ic3_srli2(b->v[4].v, b->v[3].v, 6); b2.v[1].v = SR(SL(ic3_srli2(b->v[5].v, b->v[4].v, 6),1),1);

	ab0 = a0*b0; ab1 = a1*b1; ab2 = a2*b2; ab01 = reduce(a0+a1)*(b0+b1); ab02 = reduce(a0+a2)*(b0+b2); ab12 = reduce(a1+a2)*(b1+b2);
	c0 = reduce( ab0 + mult_u(ab12-ab1-ab2) );
	c1 = reduce( (ab01-ab0-ab1) + mult_u(ab2) );
	c2 = reduce( (ab02-ab0-ab2) + (ab1) );

	c->v[0].v = c0.v[0].v; c->v[1].v = ic3_srli2(c1.v[0].v, SL(c0.v[1].v, 1), 1);
	c->v[2].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 1); c->v[3].v = ic3_srli2(c2.v[0].v, SL(c1.v[1].v, 1), 2);
	c->v[4].v = ic3_srli2(c2.v[1].v, c2.v[0].v, 2); c->v[5].v = SR(c2.v[1].v, 2);
}

template <>
const ic3_gftower<31,45> square(const ic3_gftower<31,45> &a)
{
	ic3_gftower<31,15> a0, a1, a2;
	ic3_gftower<31,15> c0, c1, c2;
	ic3_gftower<31,45> c;

	a0.v[0].v = a.v[0].v; a0.v[1].v = SR(SL(a.v[1].v,1),1);
	a1.v[0].v = ic3_srli2(a.v[2].v, a.v[1].v, 7); a1.v[1].v = SR(SL(ic3_srli2(a.v[3].v, a.v[2].v, 7),1),1);
	a2.v[0].v = ic3_srli2(a.v[4].v, a.v[3].v, 6); a2.v[1].v = SR(SL(ic3_srli2(a.v[5].v, a.v[4].v, 6),1),1);

	c2 = a2*a0; c2 = reduce( c2+c2+square(a1) );
	c1 = a1*a0; c1 = reduce( c1+c1+mult_u(square(a2)) );
	c0 = a2*a1; c0 = reduce( mult_u(c0+c0) + square(a0) );

	c.v[0].v = c0.v[0].v; c.v[1].v = ic3_srli2(c1.v[0].v, SL(c0.v[1].v, 1), 1);
	c.v[2].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 1); c.v[3].v = ic3_srli2(c2.v[0].v, SL(c1.v[1].v, 1), 2);
	c.v[4].v = ic3_srli2(c2.v[1].v, c2.v[0].v, 2); c.v[5].v = SR(c2.v[1].v, 2);

	return c;
}



template <>
void _mult(ic3_gftower<31,54> *c, const ic3_gftower<31,54> *a, const ic3_gftower<31,54> *b) //GF(31^18)[t]/t^3-u
{
	ic3_gftower<31,18> a0, a1, a2, b0, b1, b2;
	ic3_gftower<31,18> c0, c1, c2;
	ic3_gftower<31,18> ab0, ab1, ab2, ab01, ab02, ab12;

	a0.v[0].v = a->v[0].v; a0.v[1].v = a->v[1].v; a0.v[2].v = SR(SL(a->v[2].v,6),6);
	a1.v[0].v = ic3_srli2(a->v[3].v, a->v[2].v, 2); a1.v[1].v = ic3_srli2(a->v[4].v, a->v[3].v, 2); a1.v[2].v = SR(SL(a->v[4].v,4),6);
	a2.v[0].v = ic3_srli2(a->v[5].v, a->v[4].v, 4); a2.v[1].v = ic3_srli2(a->v[6].v, a->v[5].v, 4); a2.v[2].v = SR(SL(a->v[6].v,2),6);
	b0.v[0].v = b->v[0].v; b0.v[1].v = b->v[1].v; b0.v[2].v = SR(SL(b->v[2].v,6),6);
	b1.v[0].v = ic3_srli2(b->v[3].v, b->v[2].v, 2); b1.v[1].v = ic3_srli2(b->v[4].v, b->v[3].v, 2); b1.v[2].v = SR(SL(b->v[4].v,4),6);
	b2.v[0].v = ic3_srli2(b->v[5].v, b->v[4].v, 4); b2.v[1].v = ic3_srli2(b->v[6].v, b->v[5].v, 4); b2.v[2].v = SR(SL(b->v[6].v,2),6);

	ab0 = a0*b0; ab1 = a1*b1; ab2 = a2*b2; ab01 = reduce(a0+a1)*(b0+b1); ab02 = reduce(a0+a2)*(b0+b2); ab12 = reduce(a1+a2)*(b1+b2);
	c0 = reduce( ab0 + mult_u(ab12-ab1-ab2) );
	c1 = reduce( (ab01-ab0-ab1) + mult_u(ab2) );
	c2 = reduce( (ab02-ab0-ab2) + (ab1) );

	c->v[0].v = c0.v[0].v; c->v[1].v = c0.v[1].v; c->v[2].v = ic3_srli2(c1.v[0].v, SL(c0.v[2].v, 6), 6);
	c->v[3].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 6); c->v[4].v = ic3_srli2(c2.v[0].v, SL(ic3_srli2(c1.v[2].v, c1.v[1].v, 6),4), 4);
	c->v[5].v = ic3_srli2(c2.v[1].v, c2.v[0].v, 4); c->v[6].v = SR(SL(ic3_srli2(c2.v[2].v, c2.v[1].v,4),2),2);
}

template <>
const ic3_gftower<31,54> square(const ic3_gftower<31,54> &a)
{
	ic3_gftower<31,18> a0, a1, a2;
	ic3_gftower<31,18> c0, c1, c2;
	ic3_gftower<31,54> c;

	a0.v[0].v = a.v[0].v; a0.v[1].v = a.v[1].v; a0.v[2].v = SR(SL(a.v[2].v,6),6);
	a1.v[0].v = ic3_srli2(a.v[3].v, a.v[2].v, 2); a1.v[1].v = ic3_srli2(a.v[4].v, a.v[3].v, 2); a1.v[2].v = SR(SL(a.v[4].v,4),6);
	a2.v[0].v = ic3_srli2(a.v[5].v, a.v[4].v, 4); a2.v[1].v = ic3_srli2(a.v[6].v, a.v[5].v, 4); a2.v[2].v = SR(SL(a.v[6].v,2),6);

	c2 = a2*a0; c2 = reduce( c2+c2+square(a1) );
	c1 = a1*a0; c1 = reduce( c1+c1+mult_u(square(a2)) );
	c0 = a2*a1; c0 = reduce( mult_u(c0+c0) + square(a0) );

	c.v[0].v = c0.v[0].v; c.v[1].v = c0.v[1].v; c.v[2].v = ic3_srli2(c1.v[0].v, SL(c0.v[2].v, 6), 6);
	c.v[3].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 6); c.v[4].v = ic3_srli2(c2.v[0].v, SL(ic3_srli2(c1.v[2].v, c1.v[1].v, 6),4), 4);
	c.v[5].v = ic3_srli2(c2.v[1].v, c2.v[0].v, 4); c.v[6].v = SR(SL(ic3_srli2(c2.v[2].v, c2.v[1].v,4),2),2);

	return c;
}


template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> mult_u10(const ic3_gftower<p,pow> &a);

template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> mult_u20(const ic3_gftower<p,pow> &a);

template <unsigned p, unsigned pow>
inline const ic3_gftower<p,pow> mult_u15(const ic3_gftower<p,pow> &a);

template <>
inline const ic3_gftower<31,18> mult_u20(const ic3_gftower<31,18> &a)
{
	__m128i three = _mm_set1_epi16(3);
	ic3_gfv<31> v0m3 = reducev( (ic3_gfv<31>)_mm_mullo_epi16( a.v[0].v , three ));
	ic3_gfv<31> v1m3 = reducev( (ic3_gfv<31>)_mm_mullo_epi16( a.v[1].v , three ));
	ic3_gftower<31,18> r;
	r.v[0] = reducev( (ic3_gfv<31>)_mm_or_si128( _mm_slli_si128(v0m3.v,4) , _mm_mullo_epi16(a.v[2].v , _mm_set_epi16(0,0,0,0,0,0,9,9))));
	r.v[1].v = _mm_or_si128( _mm_slli_si128(v1m3.v,4) , _mm_srli_si128(v0m3.v,12) );
	r.v[2].v = _mm_srli_si128( v1m3.v , 12 );
	return r;
}

template <>
inline const ic3_gftower<31,18> mult_u10(const ic3_gftower<31,18> &a)
{
	ic3_gftower<31,18> r;
	r.v[0] = reducev( (ic3_gfv<31>)_mm_mullo_epi16( a.v[1].v , _mm_set1_epi16(3)) );
	r.v[1] = reducev( (ic3_gfv<31>)_mm_or_si128( _mm_slli_si128(a.v[0].v,4) 
							,_mm_mullo_epi16(a.v[2].v,_mm_set_epi16(0,0,0,0,0,0,3,3))));
	r.v[2].v = _mm_srli_si128(a.v[0].v, 12);
	return r;
}

template <>
inline const ic3_gftower<31,15> mult_u20(const ic3_gftower<31,15> &a)
{
	ic3_gfv<31> v1 = reducev( (ic3_gfv<31>)_mm_mullo_epi16( a.v[1].v , _mm_set_epi16( 0,9,9,9,9,9,3,3 )));
	ic3_gfv<31> v0 = reducev( (ic3_gfv<31>)_mm_mullo_epi16( a.v[0].v , _mm_set1_epi16(3) ));
	ic3_gftower<31,15> r;
	r.v[0].v = _mm_or_si128( _mm_slli_si128(v0.v,10),_mm_srli_si128(v1.v,4) );
	r.v[1].v = _mm_or_si128( _mm_srli_si128(v0.v,6), _mm_srli_si128(_mm_slli_si128(v1.v,12),2) );
	return r;
}

template <>
inline const ic3_gftower<31,15> mult_u15(const ic3_gftower<31,15> &a)
{
	__m128i three = _mm_set1_epi16(3);
	ic3_gftower<31,15> r;
	r.v[0] = reducev( (ic3_gfv<31>)_mm_mullo_epi16(a.v[0].v,three) );
	r.v[1] = reducev( (ic3_gfv<31>)_mm_mullo_epi16(a.v[1].v,three) );
	return r;
}

template <>
inline const ic3_gftower<31,15> mult_u10(const ic3_gftower<31,15> &a)
{
	ic3_gfv<31> v0m3 = reducev( (ic3_gfv<31>)_mm_mullo_epi16( a.v[0].v , _mm_set_epi16(3,3,3,1,1,1,1,1)) );
	ic3_gfv<31> v1m3 = reducev( (ic3_gfv<31>)_mm_mullo_epi16( a.v[1].v , _mm_set1_epi16(3) ));
	ic3_gftower<31,15> r;
	r.v[0].v = _mm_or_si128( _mm_srli_si128(v0m3.v,10),_mm_slli_si128(v1m3.v,6) );
	r.v[1].v = _mm_or_si128( _mm_srli_si128(_mm_slli_si128(v1m3.v,2),12) , _mm_srli_si128(  _mm_slli_si128(v0m3.v,6),2) );
	return r;
}

template <>
inline const ic3_gftower<31,30> powpk(const ic3_gftower<31,30> &a, unsigned )
{
	ic3_gftower<31,15> a0, a1;
	ic3_gftower<31,15> c0, c1;
	ic3_gftower<31,30> c;

	a0.v[0].v = a.v[0].v; a0.v[1].v = SR(SL(a.v[1].v,1),1);
	a1.v[0].v = ic3_srli2(a.v[2].v, a.v[1].v, 7); a1.v[1].v = SR(SL(ic3_srli2(a.v[3].v, a.v[2].v, 7),1),1);
	
	c1 = mult_u15<31,15>(powpk<31,15>(a1,1)); c0 = powpk<31,15>(a0,1);

	c.v[0].v = c0.v[0].v; c.v[1].v = ic3_srli2(c1.v[0].v, SL(c0.v[1].v, 1), 1);
	c.v[2].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 1); c.v[3].v = SR(c1.v[1].v, 1);

	return c;
}

template <>
inline const ic3_gftower<31,45> powpk(const ic3_gftower<31,45> &a, unsigned )
{
	ic3_gftower<31,15> a0, a1, a2;
	ic3_gftower<31,15> c0, c1, c2;
	ic3_gftower<31,45> c;

	a0.v[0].v = a.v[0].v; a0.v[1].v = SR(SL(a.v[1].v,1),1);
	a1.v[0].v = ic3_srli2(a.v[2].v, a.v[1].v, 7); a1.v[1].v = SR(SL(ic3_srli2(a.v[3].v, a.v[2].v, 7),1),1);
	a2.v[0].v = ic3_srli2(a.v[4].v, a.v[3].v, 6); a2.v[1].v = SR(SL(ic3_srli2(a.v[5].v, a.v[4].v, 6),1),1);

	c2 = mult_u20<31,15>(powpk<31,15>(a2,1));
	c1 = mult_u10<31,15>( powpk<31,15>(a1,1));
	c0 = powpk<31,15>(a0,1);

	c.v[0].v = c0.v[0].v; c.v[1].v = ic3_srli2(c1.v[0].v, SL(c0.v[1].v, 1), 1);
	c.v[2].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 1); c.v[3].v = ic3_srli2(c2.v[0].v, SL(c1.v[1].v, 1), 2);
	c.v[4].v = ic3_srli2(c2.v[1].v, c2.v[0].v, 2); c.v[5].v = SR(c2.v[1].v, 2);

	return c;
}

template <>
inline const ic3_gftower<31,54> powpk(const ic3_gftower<31,54> &a, unsigned )
{
	ic3_gftower<31,18> a0, a1, a2;
	ic3_gftower<31,18> c0, c1, c2;
	ic3_gftower<31,54> c;

	a0.v[0].v = a.v[0].v; a0.v[1].v = a.v[1].v; a0.v[2].v = SR(SL(a.v[2].v,6),6);
	a1.v[0].v = ic3_srli2(a.v[3].v, a.v[2].v, 2); a1.v[1].v = ic3_srli2(a.v[4].v, a.v[3].v, 2); a1.v[2].v = SR(SL(a.v[4].v,4),6);
	a2.v[0].v = ic3_srli2(a.v[5].v, a.v[4].v, 4); a2.v[1].v = ic3_srli2(a.v[6].v, a.v[5].v, 4); a2.v[2].v = SR(SL(a.v[6].v,2),6);

	c2 = mult_u20<31,18>(powpk<31,18>(a2,1));
	c1 = mult_u10<31,18>( powpk<31,18>(a1,1));
	c0 = powpk<31,18>(a0,1);

	c.v[0].v = c0.v[0].v; c.v[1].v = c0.v[1].v; c.v[2].v = ic3_srli2(c1.v[0].v, SL(c0.v[2].v, 6), 6);
	c.v[3].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 6); c.v[4].v = ic3_srli2(c2.v[0].v, SL(ic3_srli2(c1.v[2].v, c1.v[1].v, 6),4), 4);
	c.v[5].v = ic3_srli2(c2.v[1].v, c2.v[0].v, 4); c.v[6].v = SR(SL(ic3_srli2(c2.v[2].v, c2.v[1].v,4),2),2);
	return c;
}

template <>
const ic3_gftower<31,18> invert(const ic3_gftower<31,18> &a){
	ic3_gftower<31, 18> a2 = square(a);
	ic3_gftower<31, 18> a3 = a2 * a;
	ic3_gftower<31, 18> a6 = square(a3);
	ic3_gftower<31, 18> a12 = square(a6);
	ic3_gftower<31, 18> a24 = square(a12);
	ic3_gftower<31, 18> a30 = a24 * a6;
	ic3_gftower<31, 18> a26 = a24 * a2;
	ic3_gftower<31, 18> a29 = a26 * a3;

	ic3_gftower<31, 18> r, a3000;
	r = (a3000 = powpk(a30, 1)) * a30;
	r = powpk(r,2) * r;
	r = powpk(r,4) * r;
	r = powpk(r,8) * r;
	r = powpk(r,2) * a3000 * a29;
	return r;
}

template <>
const ic3_gftower<31,15> invert(const ic3_gftower<31,15> &a)
{
	ic3_gftower<31, 15> a2 = square(a);
	ic3_gftower<31, 15> a3 = a2 * a;
	ic3_gftower<31, 15> a6 = square(a3);
	ic3_gftower<31, 15> a12 = square(a6);
	ic3_gftower<31, 15> a24 = square(a12);
	ic3_gftower<31, 15> a30 = a24 * a6;
	ic3_gftower<31, 15> a26 = a24 * a2;
	ic3_gftower<31, 15> a29 = a26 * a3;

	ic3_gftower<31, 15> r, a3000;
	r = (a3000 = powpk(a30, 1) * a30);
	r = powpk(r, 1) * a30;
	r = powpk(r, 3) * r;
	r = powpk(r, 6) * r;
	r = powpk(r, 3) * powpk(a3000, 1) * a29;
	return r;
}

template <>
const ic3_gftower<31,10> invert(const ic3_gftower<31,10> &a)
{
	ic3_gftower<31, 10> a2 = square(a);
	ic3_gftower<31, 10> a3 = a2 * a;
	ic3_gftower<31, 10> a6 = square(a3);
	ic3_gftower<31, 10> a12 = square(a6);
	ic3_gftower<31, 10> a24 = square(a12);
	ic3_gftower<31, 10> a30 = a24 * a6;
	ic3_gftower<31, 10> a26 = a24 * a2;
	ic3_gftower<31, 10> a29 = a26 * a3;

	ic3_gftower<31, 10> r, a3000;
	r = (a3000 = powpk(a30, 1)) * a30;
	r = powpk(r,2) * r;
	r = powpk(r,4) * r;
	r = powpk(r,2) * a3000 * a29;
	return r;
}

template <>
const ic3_gftower<31,9> invert(const ic3_gftower<31,9> &a)
{
	ic3_gftower<31, 9> a2 = square(a);
	ic3_gftower<31, 9> a3 = a2 * a;
	ic3_gftower<31, 9> a6 = square(a3);
	ic3_gftower<31, 9> a12 = square(a6);
	ic3_gftower<31, 9> a24 = square(a12);
	ic3_gftower<31, 9> a30 = a24 * a6;
	ic3_gftower<31, 9> a26 = a24 * a2;
	ic3_gftower<31, 9> a29 = a26 * a3;

	ic3_gftower<31, 9> r;
	r = powpk(a30, 1) * a30;
	r = powpk(r,2) * r;
	r = powpk(r,4) * r;
	r = powpk(r,1) * a29;
	return r;
}

template <>
const ic3_gftower<31,30> invert(const ic3_gftower<31,30> &a)
{
	ic3_gftower<31,15> a0, a1;
	ic3_gftower<31,15> c0, c1;
	ic3_gftower<31,15> d;
	ic3_gftower<31,30> c, cc;
//	ic3_gftower<31,15> zero;

	a0.v[0].v = a.v[0].v; a0.v[1].v = SR(SL(a.v[1].v,1),1);
	a1.v[0].v = ic3_srli2(a.v[2].v, a.v[1].v, 7); a1.v[1].v = SR(SL(ic3_srli2(a.v[3].v, a.v[2].v, 7),1),1);
	d = invert( mult_u(square(a1)) - square(a0) );
//	zero.v[0].v = _mm_setzero_si128(); zero.v[1].v = _mm_setzero_si128();

	c1 = reduce(a1*d); c0 = reduce( (-a0)*d );
	c.v[0].v = c0.v[0].v; c.v[1].v = ic3_srli2(c1.v[0].v, SL(c0.v[1].v, 1), 1);
	c.v[2].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 1); c.v[3].v = SR(c1.v[1].v, 1);
//	c.dump(stdout);

//	cc = c*a;
//	cc.dump(stdout);
//	cc = cc*a;
//	cc.dump(stdout);
	return c;
}

template <>
const ic3_gftower<31,45> invert(const ic3_gftower<31,45> &a)
{
	ic3_gftower<31,15> a0, a1, a2;
	ic3_gftower<31,15> c0, c1, c2;
	ic3_gftower<31,15> d, d0, d1, d2;
	ic3_gftower<31,45> c, cc;

	a0.v[0].v = a.v[0].v; a0.v[1].v = SR(SL(a.v[1].v,1),1);
	a1.v[0].v = ic3_srli2(a.v[2].v, a.v[1].v, 7); a1.v[1].v = SR(SL(ic3_srli2(a.v[3].v, a.v[2].v, 7),1),1);
	a2.v[0].v = ic3_srli2(a.v[4].v, a.v[3].v, 6); a2.v[1].v = SR(SL(ic3_srli2(a.v[5].v, a.v[4].v, 6),1),1);
////need modified
	d0 = square(a0) - mult_u(a1*a2); d1 = mult_u(square(a2)) - (a0*a1); d2 = square(a1) - (a0*a2);
	d = invert( (a0*d0) + mult_u(a2*d1) + mult_u(a1*d2) );

	c0 = reduce(d0*d); c1 = reduce(d1*d); c2 = reduce(d2*d);
////
	c.v[0].v = c0.v[0].v; c.v[1].v = ic3_srli2(c1.v[0].v, SL(c0.v[1].v, 1), 1);
	c.v[2].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 1); c.v[3].v = ic3_srli2(c2.v[0].v, SL(c1.v[1].v, 1), 2);
	c.v[4].v = ic3_srli2(c2.v[1].v, c2.v[0].v, 2); c.v[5].v = SR(c2.v[1].v, 2);

//	cc = c*a;
//	cc.dump(stdout);
	return c;
}

template <>
const ic3_gftower<31,54> invert(const ic3_gftower<31,54> &a)
{
	ic3_gftower<31,18> a0, a1, a2;
	ic3_gftower<31,18> c0, c1, c2;
	ic3_gftower<31,18> d, d0, d1, d2;
	ic3_gftower<31,54> c, cc;

	a0.v[0].v = a.v[0].v; a0.v[1].v = a.v[1].v; a0.v[2].v = SR(SL(a.v[2].v,6),6);
	a1.v[0].v = ic3_srli2(a.v[3].v, a.v[2].v, 2); a1.v[1].v = ic3_srli2(a.v[4].v, a.v[3].v, 2); a1.v[2].v = SR(SL(a.v[4].v,4),6);
	a2.v[0].v = ic3_srli2(a.v[5].v, a.v[4].v, 4); a2.v[1].v = ic3_srli2(a.v[6].v, a.v[5].v, 4); a2.v[2].v = SR(SL(a.v[6].v,2),6);
////need modified
	d0 = square(a0) - mult_u(a1*a2); d1 = mult_u(square(a2)) - (a0*a1); d2 = square(a1) - (a0*a2);
	d = invert( (a0*d0) + mult_u(a2*d1) + mult_u(a1*d2) );

	c0 = reduce(d0*d); c1 = reduce(d1*d); c2 = reduce(d2*d);
////
	c.v[0].v = c0.v[0].v; c.v[1].v = c0.v[1].v; c.v[2].v = ic3_srli2(c1.v[0].v, SL(c0.v[2].v, 6), 6);
	c.v[3].v = ic3_srli2(c1.v[1].v, c1.v[0].v, 6); c.v[4].v = ic3_srli2(c2.v[0].v, SL(ic3_srli2(c1.v[2].v, c1.v[1].v, 6),4), 4);
	c.v[5].v = ic3_srli2(c2.v[1].v, c2.v[0].v, 4); c.v[6].v = SR(SL(ic3_srli2(c2.v[2].v, c2.v[1].v,4),2),2);

	return c;
}

static const short _ic3_sqrt_table_31_18[32][2][24] __attribute__ ((aligned (16))) = 
	{{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{5,0,0,0,0,0,0,0,0,-15,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-8,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-13,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{5,0,0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-11,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{5,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-4,0,0,0,0,0,0,0,0,-6,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-13,0,0,0,0,0,0,0,0,-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{2,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-4,0,0,0,0,0,0,0,0,-4,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-7,0,0,0,0,0,0,0,0,-4,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-11,0,0,0,0,0,0,0,0,-3,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-10,0,0,0,0,0,0,0,0,-8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-14,0,0,0,0,0,0,0,0,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,0,-14,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-4,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{10,0,0,0,0,0,0,0,0,-8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-12,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{7,0,0,0,0,0,0,0,0,-4,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{2,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-2,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-13,0,0,0,0,0,0,0,0,-6,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{4,0,0,0,0,0,0,0,0,-6,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-7,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{11,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{6,0,0,0,0,0,0,0,0,-8,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{13,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-10,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-5,0,0,0,0,0,0,0,0,-15,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{11,0,0,0,0,0,0,0,0,-12,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-5,0,0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{11,0,0,0,0,0,0,0,0,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{13,0,0,0,0,0,0,0,0,-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{10,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{11,0,0,0,0,0,0,0,0,-3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{6,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{4,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{7,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-2,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-13,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{7,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-2,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{10,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-12,0,0,0,0,0,0,0,0,-10,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,0,0,0,0,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{4,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-10,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-14,0,0,0,0,0,0,0,0,-13,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-7,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{11,0,0,0,0,0,0,0,0,-3,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{2,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-4,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-4,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{13,0,0,0,0,0,0,0,0,-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-11,0,0,0,0,0,0,0,0,-3,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{5,0,0,0,0,0,0,0,0,-9,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{-13,0,0,0,0,0,0,0,0,-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-5,0,0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{5,0,0,0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{-8,0,0,0,0,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}}};
static const ic3_gftower<31,18> (&ic3_sqrt_table_31_18)[32][2] = *(ic3_gftower<31,18>(*)[32][2])&_ic3_sqrt_table_31_18;

static const short _ic3_sqrt_table_31_10[32][2][16] __attribute__ ((aligned (16))) =
	{{{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
	{{5,0,0,0,0,-15,0,0,0,0,0,0,0,0,0,0},{-8,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}},
	{{-13,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0},{5,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0}},
	{{-11,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0},{5,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0}},
	{{-4,0,0,0,0,-6,0,0,0,0,0,0,0,0,0,0},{-13,0,0,0,0,-5,0,0,0,0,0,0,0,0,0,0}},
	{{2,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0},{-4,0,0,0,0,-4,0,0,0,0,0,0,0,0,0,0}},
	{{-7,0,0,0,0,-4,0,0,0,0,0,0,0,0,0,0},{-11,0,0,0,0,-3,0,0,0,0,0,0,0,0,0,0}},
	{{-10,0,0,0,0,-8,0,0,0,0,0,0,0,0,0,0},{-14,0,0,0,0,13,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,-14,0,0,0,0,0,0,0,0,0,0},{-4,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0}},
	{{10,0,0,0,0,-8,0,0,0,0,0,0,0,0,0,0},{-12,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0}},
	{{7,0,0,0,0,-4,0,0,0,0,0,0,0,0,0,0},{2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}},
	{{-2,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0},{-13,0,0,0,0,-6,0,0,0,0,0,0,0,0,0,0}},
	{{4,0,0,0,0,-6,0,0,0,0,0,0,0,0,0,0},{-7,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0}},
	{{11,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0},{6,0,0,0,0,-8,0,0,0,0,0,0,0,0,0,0}},
	{{13,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0},{-10,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0}},
	{{-5,0,0,0,0,-15,0,0,0,0,0,0,0,0,0,0},{11,0,0,0,0,-12,0,0,0,0,0,0,0,0,0,0}},
	{{-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,14,0,0,0,0,0,0,0,0,0,0}},
	{{-5,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0},{11,0,0,0,0,12,0,0,0,0,0,0,0,0,0,0}},
	{{13,0,0,0,0,-5,0,0,0,0,0,0,0,0,0,0},{10,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0}},
	{{11,0,0,0,0,-3,0,0,0,0,0,0,0,0,0,0},{6,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0}},
	{{4,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0},{7,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0}},
	{{-2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},{-13,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0}},
	{{7,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0},{-2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}},
	{{10,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0},{-12,0,0,0,0,-10,0,0,0,0,0,0,0,0,0,0}},
	{{0,0,0,0,0,14,0,0,0,0,0,0,0,0,0,0},{4,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0}},
	{{-10,0,0,0,0,8,0,0,0,0,0,0,0,0,0,0},{-14,0,0,0,0,-13,0,0,0,0,0,0,0,0,0,0}},
	{{-7,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0},{11,0,0,0,0,-3,0,0,0,0,0,0,0,0,0,0}},
	{{2,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},{-4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0}},
	{{-4,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0},{13,0,0,0,0,-5,0,0,0,0,0,0,0,0,0,0}},
	{{-11,0,0,0,0,-3,0,0,0,0,0,0,0,0,0,0},{5,0,0,0,0,-9,0,0,0,0,0,0,0,0,0,0}},
	{{-13,0,0,0,0,-5,0,0,0,0,0,0,0,0,0,0},{-5,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0}},
	{{5,0,0,0,0,15,0,0,0,0,0,0,0,0,0,0},{-8,0,0,0,0,-1,0,0,0,0,0,0,0,0,0,0}}};
static const ic3_gftower<31,10> (&ic3_sqrt_table_31_10)[32][2] = *(ic3_gftower<31,10>(*)[32][2])&_ic3_sqrt_table_31_10;

template <>
const ic3_gftower<31,18> sqrt(const ic3_gftower<31,18> &a){
	ic3_gftower<31, 18> a2 = square(a);
	ic3_gftower<31, 18> a4 = square(a2);
	ic3_gftower<31, 18> a7 = a4 * a2 * a;
	ic3_gftower<31, 18> a8 = square(a4);
	ic3_gftower<31, 18> a15 = a8 * a7;
	ic3_gftower<31, 18> a23 = a15 * a8;

	ic3_gftower<31, 18> r = powpk(powpk(a7,1)*a15,1)*a23;
	r = powpk(r,4) * r;
	r = powpk(r,8) * r;
	r = powpk(r,2) * a7;

	ic3_gftower<31, 18> annoy = a * square(r);
	for (unsigned i = 32; i--;)
		if (ic3_sqrt_table_31_18[i][0] == annoy)
			return a * r * ic3_sqrt_table_31_18[i][1];
	return a * r; // wrong answer.
}

template <>
const ic3_gftower<31,15> sqrt(const ic3_gftower<31,15> &a)
{
	ic3_gftower<31, 15> a2 = square(a);
	ic3_gftower<31, 15> a4 = square(a2);
	ic3_gftower<31, 15> a7 = a4 * a2 * a;
	ic3_gftower<31, 15> a8 = square(a4);
	ic3_gftower<31, 15> a23 = square(a8) * a7;
	ic3_gftower<31, 15> a240 = powpk(a7, 1) * a23;

	ic3_gftower<31, 15> r = powpk(a240, 4) * powpk(a240, 2) * a240;
	r = powpk(r, 6) * r;
	r = powpk(r, 3) * powpk(a240, 1) * a7;

	ic3_gftower<31, 15> annoy = a * square(r);
	if(annoy == ic3_gftower<31, 15>::one)
		return a * r;
	return a * r; //wrong ansnwer
}

template <>
const ic3_gftower<31,10> sqrt(const ic3_gftower<31,10> &a)
{
	ic3_gftower<31, 10> a2 = square(a);
	ic3_gftower<31, 10> a4 = square(a2);
	ic3_gftower<31, 10> a7 = a4 * a2 * a;
	ic3_gftower<31, 10> a8 = square(a4);
	ic3_gftower<31, 10> a15 = a8 * a7;
	ic3_gftower<31, 10> a23 = a15 * a8;

	ic3_gftower<31, 10> r = powpk(powpk(a7,1)*a15,1)*a23;
	r = powpk(r,4) * r;
	r = powpk(r,2) * a7;

	ic3_gftower<31, 10> annoy = a * square(r);
	for (unsigned i = 32; i--;)
		if (ic3_sqrt_table_31_10[i][0] == annoy)
			return a * r * ic3_sqrt_table_31_10[i][1];
	return a * r; // wrong answer
}

template <>
const ic3_gftower<31,9> sqrt(const ic3_gftower<31,9> &a)
{
	ic3_gftower<31, 9> a2 = square(a);
	ic3_gftower<31, 9> a4 = square(a2);
	ic3_gftower<31, 9> a7 = a4 * a2 * a;
	ic3_gftower<31, 9> a8 = square(a4);
	ic3_gftower<31, 9> a23 = square(a8) * a7;

	ic3_gftower<31, 9> r = powpk(a7, 1) * a23;
	r = powpk(r, 2) * r;
	r = powpk(r, 4) * r;
	r = powpk(r, 1) * a7;

	ic3_gftower<31, 9> annoy = a * square(r);
	if (annoy == ic3_gftower<31,9>::one)
		return a * r;
	return a * r; //wrong answer
}

template <>
void sqrt_n_inv(ic3_gftower<31,18> *s, ic3_gftower<31,18> *i, const ic3_gftower<31,18> &a){
	ic3_gftower<31, 18> a2 = square(a);
	ic3_gftower<31, 18> a4 = square(a2);
	ic3_gftower<31, 18> a6 = a4 * a2;
	ic3_gftower<31, 18> a7 = a6 * a;
	ic3_gftower<31, 18> a8 = square(a4);
	ic3_gftower<31, 18> a15 = a8 * a7;
	ic3_gftower<31, 18> a23 = a15 * a8;
	ic3_gftower<31, 18> a29 = a23 * a6;
	ic3_gftower<31, 18> a30 = square(a15);

	ic3_gftower<31, 18> r, a3000;
	r = (a3000 = powpk(a30, 1)) * a30;
	r = powpk(r,2) * r;
	r = powpk(r,4) * r;
	r = powpk(r,8) * r;
	r = powpk(r,2) * a3000 * a29;
	*i = r;

	r = powpk(powpk(a7,1)*a15,1)*a23;
	r = powpk(r,4) * r;
	r = powpk(r,8) * r;
	r = powpk(r,2) * a7;

	ic3_gftower<31, 18> annoy = a * square(r);
	for (unsigned i = 32; i--;)
		if (ic3_sqrt_table_31_18[i][0] == annoy){
			*s = a * r * ic3_sqrt_table_31_18[i][1];
			return;
		}
	*s = a * r; // wrong answer.
}

template <>
void sqrt_n_inv(ic3_gftower<31,15> *s, ic3_gftower<31,15> *i, const ic3_gftower<31,15> &a)
{
	ic3_gftower<31, 15> a2 = square(a);
	ic3_gftower<31, 15> a4 = square(a2);
	ic3_gftower<31, 15> a6 = a4 * a2;
	ic3_gftower<31, 15> a7 = a6 * a;
	ic3_gftower<31, 15> a8 = square(a4);
	ic3_gftower<31, 15> a15 = a8 * a7;
	ic3_gftower<31, 15> a23 = a15 * a8;
	ic3_gftower<31, 15> a29 = a23 * a6;
	ic3_gftower<31, 15> a30 = square(a15);
	ic3_gftower<31, 15> a240 = powpk(a7, 1) * a23;

	ic3_gftower<31, 15> r, a3000;
	r = (a3000 = powpk(a30, 1) * a30);
	r = powpk(r, 1) * a30;
	r = powpk(r, 3) * r;
	r = powpk(r, 6) * r;
	r = powpk(r, 3) * powpk(a3000, 1) * a29;
	*i = r;

	r = powpk(a240, 4) * powpk(a240, 2) * a240;
	r = powpk(r, 6) * r;
	r = powpk(r, 3) * powpk(a240, 1) * a7;

	ic3_gftower<31, 15> annoy = a * square(r);
	if(annoy == ic3_gftower<31, 15>::one)
	{
		*s = a * r;
		return;
	}
	*s = a * r; //wrong ansnwer
}

template <>
void sqrt_n_inv(ic3_gftower<31,10> *s, ic3_gftower<31,10> *i, const ic3_gftower<31,10> &a)
{
	ic3_gftower<31, 10> a2 = square(a);
	ic3_gftower<31, 10> a4 = square(a2);
	ic3_gftower<31, 10> a6 = a4 * a2;
	ic3_gftower<31, 10> a7 = a6 * a;
	ic3_gftower<31, 10> a8 = square(a4);
	ic3_gftower<31, 10> a15 = a8 * a7;
	ic3_gftower<31, 10> a23 = a15 * a8;
	ic3_gftower<31, 10> a29 = a23 * a6;
	ic3_gftower<31, 10> a30 = square(a15);

	ic3_gftower<31, 10> r, a3000;
	r = (a3000 = powpk(a30, 1)) * a30;
	r = powpk(r,2) * r;
	r = powpk(r,4) * r;
	r = powpk(r,2) * a3000 * a29;
	*i = r;

	r = powpk(powpk(a7,1)*a15,1)*a23;
	r = powpk(r,4) * r;
	r = powpk(r,2) * a7;

	ic3_gftower<31, 10> annoy = a * square(r);
	for (unsigned i = 32; i--;)
		if (ic3_sqrt_table_31_10[i][0] == annoy)
		{
			*s = a * r * ic3_sqrt_table_31_10[i][1];
			return;
		}
	*s = a * r; // wrong answer
}

template <>
void sqrt_n_inv(ic3_gftower<31,9> *s, ic3_gftower<31,9> *i, const ic3_gftower<31,9> &a)
{
	ic3_gftower<31, 9> a2 = square(a);
	ic3_gftower<31, 9> a4 = square(a2);
	ic3_gftower<31, 9> a6 = a4 * a2;
	ic3_gftower<31, 9> a7 = a6 * a;
	ic3_gftower<31, 9> a8 = square(a4);
	ic3_gftower<31, 9> a15 = a8 * a7;
	ic3_gftower<31, 9> a23 = a15 * a8;
	ic3_gftower<31, 9> a29 = a23 * a6;
	ic3_gftower<31, 9> a30 = square(a15);

	ic3_gftower<31, 9> r;
	r = powpk(a30, 1) * a30;
	r = powpk(r,2) * r;
	r = powpk(r,4) * r;
	r = powpk(r,1) * a29;
	*i = r;

	r = powpk(a7,1)*a23;
	r = powpk(r, 2) * r;
	r = powpk(r, 4) * r;
	r = powpk(r, 1) * a7;

	ic3_gftower<31, 9> annoy = a * square(r);
	if (annoy == ic3_gftower<31,9>::one)
	{
		*s =  a * r;
		return;
	}
	*s = a * r; //wrong answer
}

#undef ADD
#undef MUL
#undef SL
#undef SR

#endif
