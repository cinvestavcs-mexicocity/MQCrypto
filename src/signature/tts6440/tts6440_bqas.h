#ifndef _TTS6440_BQAS_H
#define _TTS6440_BQAS_H

#include <cstdio>
#include "tts6440_gfv.h"

template <unsigned p, unsigned m> struct tts6440_VEC{
	static unsigned const M = (m+7)/8;
	tts6440_gfv<p> c[M];

	inline tts6440_VEC<p,m>& operator= (const tts6440_VEC<p,m> &b){for (unsigned i = M; i--;) c[i] = b.c[i]; return *this; }

	inline tts6440_VEC<p,m>& operator+= (const tts6440_VEC<p,m> &b){for (unsigned i = M; i--;) c[i] += b.c[i]; return *this;}
	inline tts6440_VEC<p,m>& operator-= (const tts6440_VEC<p,m> &b){for (unsigned i = M; i--;) c[i] -= b.c[i]; return *this;}
	inline tts6440_VEC<p,m>& operator*= (const tts6440_gf<p> &b){tts6440_gfv<p> _b = b; for (unsigned i = M; i--;) c[i] *= _b; return *this;}
	inline tts6440_VEC<p,m>& operator/= (const tts6440_gf<p> &b){tts6440_gfv<p> _b = invert(b); for (unsigned i = M; i--;) c[i] *= _b; return *this;}

	inline const tts6440_VEC<p,m> operator+ (const tts6440_VEC<p,m> &b) const {tts6440_VEC<p,m> r = *this; return r+=b;}
	inline const tts6440_VEC<p,m> operator- (const tts6440_VEC<p,m> &b) const {tts6440_VEC<p,m> r = *this; return r-=b;}
	inline const tts6440_VEC<p,m> operator* (const tts6440_gf<p> &b) const {tts6440_VEC<p,m> r = *this; return r*=b;}
	inline const tts6440_VEC<p,m> operator/ (const tts6440_gf<p> &b) const {tts6440_VEC<p,m> r = *this; return r/=b;}

	inline tts6440_gf<p>& operator[](unsigned i) {return ((tts6440_gf<p>*)c)[i];}
	inline const tts6440_gf<p>& operator[](unsigned i) const {return ((const tts6440_gf<p>*)c)[i];}

	inline void reduce() {for(unsigned i = M; i--;) c[i] = reducev(c[i]);}
	inline void principal(){for(unsigned i = M; i--;) c[i] = principalv(c[i]);}

	void dump(FILE *f) const;
};

template <unsigned p, unsigned m, unsigned n> struct tts6440_MLPS : public tts6440_VEC<p, m>{
	static unsigned const M = (m+7)/8;
	static unsigned const N = (n+7)/8;
	tts6440_gfv<p> l[M][N*8];
	void dump(FILE *f) const;
};

template <unsigned p, unsigned m, unsigned n> void eval(tts6440_VEC<p,m> *r, 
	const tts6440_MLPS<p,m,n> *qq, const tts6440_VEC<p,n> *w);

template <unsigned p, unsigned m, unsigned n> struct tts6440_MQPS : public tts6440_MLPS<p, m, n>{
	static unsigned const M = (m+7)/8;
	static unsigned const N = (n+7)/8;
	tts6440_gfv<p> q[M][n*(n+1)/2];
};

template <unsigned p, unsigned m, unsigned n> void eval(tts6440_VEC<p,m> *r, 
	const tts6440_MQPS<p,m,n> *qq, const tts6440_VEC<p,n> *w);

template <unsigned p, unsigned m, unsigned n> struct tts6440_MQPSv2 {
	static unsigned const M = (m+3)/4;
	static unsigned const N = ((n+1)*(n+2)/2+7)/8*4;
	tts6440_gfv<p> q[M][N];
};

template <unsigned p, unsigned m, unsigned n> void eval(tts6440_VEC<p,m> *r, 
	const tts6440_MQPSv2<p,m,n> *qq, const tts6440_VEC<p,n> *w);

template <unsigned p, unsigned m, unsigned n> struct tts6440_MQPSv3 {
	static unsigned const M = (m+3)/4;
	static unsigned const N = ((n+1)*(n+2)/2+23)/24*4;
	tts6440_gfv<p> q[M][N];
};

template <unsigned p, unsigned m, unsigned n> void eval(tts6440_VEC<p,m> *r, 
	const tts6440_MQPSv3<p,m,n> *qq, const tts6440_VEC<p,n> *w);

template <unsigned p, unsigned n> void zero_vec(tts6440_VEC<p, n> *tts6440_vec);
template <unsigned p, unsigned m, unsigned n> void zero_linear(tts6440_MLPS<p,m,n> *mat);
template <unsigned p, unsigned m, unsigned n> void zero_quad(tts6440_MQPS<p,m,n> *quad);

template <unsigned p, unsigned n> void rand_vec(tts6440_VEC<p, n> *v);
template <unsigned p, unsigned n> void tts6440_rand_affine_inv(tts6440_MLPS<p, n, n> *m, tts6440_MLPS<p, n, n> *inv);

#include <cstdio>
#include <cstdlib>
// #include "bqas.h"
#include "tts6440_transpose8.h"

template <unsigned m, unsigned n>
void eval(tts6440_VEC<31,m> *r, const tts6440_MLPS<31,m,n> *poly, const tts6440_VEC<31,n> *w){
	for (unsigned k = tts6440_MLPS<31,m,n>::M; k--;){
		r->c[k] = poly->c[k];
		for (unsigned i = n; i--;)
			r->c[k] += poly->l[k][i] * (*w)[i];
		r->c[k] = reducev(r->c[k]);
	}
}

template <unsigned m, unsigned n>
void eval(tts6440_VEC<31,m> *r, const tts6440_MQPS<31,m,n> *poly, const tts6440_VEC<31,n> *w){
	for (unsigned k = tts6440_MQPS<31,m,n>::M; k--;){
		r->c[k] = poly->c[k];
		for (unsigned i = n, l = n*(n+1)/2, j; (j=i--);){
			tts6440_gfv<31> t = poly->l[k][i];
			while(j--)
				t += poly->q[k][--l] * (*w)[j];
			r->c[k] += reducev(t) * (*w)[i];
		}
		r->c[k] = reducev(r->c[k]);
	}
}

template <unsigned p, unsigned m, unsigned n> void eval(tts6440_VEC<p,m> *r, const tts6440_MQPSv2<p,m,n> *poly, const tts6440_VEC<p,n> *w){
	static unsigned const N = ((n+1)*(n+2)/2+7)/8;

	__m128i wij[N];
	short *_wij = (short *)wij;
	*_wij++ = 1;
	for (unsigned i = 0; i < n; i++){
		short _wi = (*w)[i].v;
		const __m128i wi = _mm_set1_epi16(_wi);

		*_wij++ = _wi;
		for (unsigned j = 0; j < i/8+1; j++){
			const __m128i wj = ((__m128i*)w->c)[j];
			_mm_storeu_si128((__m128i*)(_wij + 8*j), _mm_mullo_epi16(wi, wj));
		}
		_wij += i+1;
	}
	while (_wij < ((short *)wij)+N*8)
		*_wij++ = 0;

	const __m128i mask = _mm_set1_epi32(1023);
	tts6440_gfv<p> *res = &r->c[tts6440_VEC<p,m>::M];
	__m128i acchi = _mm_setzero_si128();
	for (unsigned k = tts6440_MQPSv2<p,m,n>::M; k--;){
		__m128i acc = _mm_setzero_si128();

		__m128i *q = (__m128i*)&poly->q[k][tts6440_MQPSv2<p,m,n>::N];
		for (unsigned i = N; i--;){
			__m128i wiji = wij[i];
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 255), *--q));
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 170), *--q));
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 85), *--q));
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 0), *--q));
		}
		acc = _mm_add_epi32(_mm_srai_epi32(acc, 10), _mm_and_si128(acc, mask));
		if (k & 1)
			acchi = acc;
		else
			*--res = reducev((tts6440_gfv<p>)_mm_packs_epi32(acc, acchi));
	}
}

template <unsigned p, unsigned m, unsigned n> void eval(tts6440_VEC<p,m> *r, const tts6440_MQPSv3<p,m,n> *poly, const tts6440_VEC<p,n> *w){
	static unsigned const N = ((n+1)*(n+2)/2+23)/24*3;

	__m128i wij[N];
	short *_wij = (short *)wij;
	*_wij++ = 1;
	for (unsigned i = 0; i < n; i++){
		short _wi = (*w)[i].v;
		const __m128i wi = _mm_set1_epi16(_wi);

		*_wij++ = _wi;
		for (unsigned j = 0; j < i/8+1; j++){
			const __m128i wj = ((__m128i*)w->c)[j];
			_mm_storeu_si128((__m128i*)(_wij + 8*j), _mm_mullo_epi16(wi, wj));
		}
		_wij += i+1;
	}
	while (_wij < ((short *)wij)+N*8)
		*_wij++ = 0;

	const __m128i mask = _mm_set1_epi32(1023);
	tts6440_gfv<p> *res = &r->c[tts6440_VEC<p,m>::M];
	__m128i acchi = _mm_setzero_si128();
	for (unsigned k = tts6440_MQPSv3<p,m,n>::M; k--;){
		__m128i acc = _mm_setzero_si128();

		__m128i *q = (__m128i*)&poly->q[k][tts6440_MQPSv3<p,m,n>::N];
		for (unsigned i = N; i;){
			__m128i wiji = wij[--i];
			__m128i qqq = *--q;
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 255), _mm_srai_epi16(qqq, 11)));
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 170), _mm_srai_epi16(_mm_slli_epi16(qqq, 6),11)));
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 85), _mm_srai_epi16(_mm_slli_epi16(qqq, 11),11)));
			qqq = *--q;
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 0), _mm_srai_epi16(qqq, 11)));
			wiji = wij[--i];
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 255), _mm_srai_epi16(_mm_slli_epi16(qqq, 6),11)));
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 170), _mm_srai_epi16(_mm_slli_epi16(qqq, 11),11)));
			qqq = *--q;
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 85), _mm_srai_epi16(qqq, 11)));
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 0), _mm_srai_epi16(_mm_slli_epi16(qqq, 6),11)));
			wiji = wij[--i];
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 255), _mm_srai_epi16(_mm_slli_epi16(qqq, 11),11)));
			qqq = *--q;
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 170), _mm_srai_epi16(qqq, 11)));
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 85), _mm_srai_epi16(_mm_slli_epi16(qqq, 6),11)));
			acc = _mm_add_epi32(acc, _mm_madd_epi16(_mm_shuffle_epi32(wiji, 0), _mm_srai_epi16(_mm_slli_epi16(qqq, 11),11)));
		}
		acc = _mm_add_epi32(_mm_srai_epi32(acc, 10), _mm_and_si128(acc, mask));
		if (k & 1)
			acchi = acc;
		else
			*--res = reducev((tts6440_gfv<p>)_mm_packs_epi32(acc, acchi));
	}
}

template <unsigned p, unsigned n>
void zero_vec(tts6440_VEC<p, n> *tts6440_vec){
	for(unsigned i = tts6440_VEC<p,n>::M; i--;)
		tts6440_vec->c[i] = _mm_setzero_si128();
}

template <unsigned p, unsigned m, unsigned n>
void zero_linear(tts6440_MLPS<p,m,n> *mat){
	for (unsigned j = tts6440_MLPS<p,m,n>::M; j--;)
		for (unsigned i = n; i--;)
			mat->l[j][i] = _mm_setzero_si128();
}

template <unsigned p, unsigned m, unsigned n>
void zero_quad(tts6440_MQPS<p,m,n> *quad){
	for (unsigned j = tts6440_MQPS<p,m,n>::M; j--;)
		for (unsigned i = n*(n+1)/2; i--;)
			quad->q[j][i] = _mm_setzero_si128();
}

template <unsigned p, unsigned n>
void rand_vec(tts6440_VEC<p, n> *tts6440_vec){
	for(unsigned i = n; i--;)
		((tts6440_gf<p> *)tts6440_vec->c)[i] = tts6440_gf<p>::rand();
}

template <unsigned p, unsigned n>
void transpose(tts6440_MLPS<p,n,n> *mat){
	for (unsigned i = tts6440_MLPS<p,n,n>::N; i--;)
		tts6440_transpose8(&mat->l[i][i<<3].v, &mat->l[i][i<<3].v);
	__m128i tmp[8];
	for (unsigned j = tts6440_MLPS<p,n,n>::N; j--;)
		for (unsigned i = j; i--;){
			for (unsigned w = 8; w--;)
				tmp[w] = mat->l[j][(i<<3)+w].v;
			tts6440_transpose8(&mat->l[j][i<<3].v, &mat->l[i][j<<3].v);
			tts6440_transpose8(&mat->l[i][j<<3].v, tmp);
		}
}

static void tts6440_swap(unsigned &a, unsigned &b){
	unsigned c = a;
	a = b;
	b = c;
}

template <unsigned p, unsigned m, unsigned n>
static void mat_col_scale(tts6440_MLPS<p, m, n> *mat, unsigned col, tts6440_gf<p> s){
	tts6440_gfv<p> sv = s;
	for (unsigned i = tts6440_MLPS<p,m,n>::M; i--;)
		mat->l[i][col] = reducev(mat->l[i][col] * sv);
}

template <unsigned p, unsigned m, unsigned n>
static void mat_col_add(tts6440_MLPS<p, m, n> *mat, unsigned col, unsigned from, tts6440_gf<p> s){
	tts6440_gfv<p> sv = s;
	for (unsigned i = tts6440_MLPS<p,m,n>::M; i--;)
		mat->l[i][col] = reducev(mat->l[i][col] + mat->l[i][from] * sv);
}

template <unsigned p, unsigned n>
void tts6440_rand_affine_inv(tts6440_MLPS<p, n, n> *mat, tts6440_MLPS<p, n, n> *inv){
	zero_linear(mat);
	zero_linear(inv);

	unsigned reorder[n];
	for (unsigned i = n; i--;)
		reorder[i] = i;
	for (unsigned i = n; i--;){
		unsigned r = rand()%(i+1);
		tts6440_swap(reorder[r], reorder[i]);
	}

	tts6440_gf<p> (*mat_)[tts6440_MLPS<p,n,n>::N * 8][8] = (tts6440_gf<p> (*)[tts6440_MLPS<p,n,n>::N * 8][8]) mat->l;
	tts6440_gf<p> (*inv_)[tts6440_MLPS<p,n,n>::N * 8][8] = (tts6440_gf<p> (*)[tts6440_MLPS<p,n,n>::N * 8][8]) inv->l;
	for (unsigned j = n; j--;)
		mat_[j>>3][reorder[j]][j&7] = inv_[j>>3][reorder[j]][j&7] = (tts6440_gf<p>)1;

	for (unsigned i = n; i--;){
		tts6440_gf<p> scale = tts6440_gf<p>::rand_unit();
		mat_col_scale(mat, i, scale);
		mat_col_scale(inv, i, invert(scale));
		for (unsigned j = n; j--;){
			if (i == j)
				continue;
			scale = tts6440_gf<p>::rand();
			mat_col_add(mat, j, i, scale);
			mat_col_add(inv, i, j, -scale);
		}
	}

	transpose(inv);

	rand_vec(mat);
	zero_vec(inv);

	tts6440_VEC<p, n> tmp1, tmp2;
	eval(&tmp1, mat, inv);
	eval(&tmp2, inv, &tmp1);
	for (unsigned j = tts6440_VEC<p,n>::M; j--;)
		inv->c[j] = -tmp2.c[j];
}

template <unsigned p, unsigned m> void tts6440_VEC<p, m>::dump(FILE *f) const{
	for (unsigned i = 0; i < m; i++)
		fprintf (f, "%hd ", ((short *)this->c)[i]);
	fprintf (f, "\n");
}

template <unsigned p, unsigned m, unsigned n> void tts6440_MLPS<p, m, n>::dump(FILE *f) const{
	short (*mat_)[tts6440_MLPS<p,m,n>::N * 8][8] = (short (*)[tts6440_MLPS<p,m,n>::N * 8][8])this->l;
	for (unsigned j = 0; j < m; j++){
		for (unsigned i = 0; i < n; i++)
			fprintf (f, "%hd ", mat_[j>>3][i][j&7]);
		fprintf (f, " %hd\n", ((short *)this->c)[j]);
	}
}

#endif
