#ifndef _IC3_BQAS_H
#define _IC3_BQAS_H

#include <cstdio>
#include "gfv.h"

template <unsigned p, unsigned m> struct ic3_VEC{
	static unsigned const M = (m+7)/8;
	ic3_gfv<p> c[M];

	inline ic3_VEC<p,m>& operator= (const ic3_VEC<p,m> &b){for (unsigned i = M; i--;) c[i] = b.c[i]; return *this; }

	inline ic3_VEC<p,m>& operator+= (const ic3_VEC<p,m> &b){for (unsigned i = M; i--;) c[i] += b.c[i]; return *this;}
	inline ic3_VEC<p,m>& operator-= (const ic3_VEC<p,m> &b){for (unsigned i = M; i--;) c[i] -= b.c[i]; return *this;}
	inline ic3_VEC<p,m>& operator*= (const ic3_gf<p> &b){ic3_gfv<p> _b = b; for (unsigned i = M; i--;) c[i] *= _b; return *this;}
	inline ic3_VEC<p,m>& operator/= (const ic3_gf<p> &b){ic3_gfv<p> _b = invert(b); for (unsigned i = M; i--;) c[i] *= _b; return *this;}

	inline const ic3_VEC<p,m> operator+ (const ic3_VEC<p,m> &b) const {ic3_VEC<p,m> r = *this; return r+=b;}
	inline const ic3_VEC<p,m> operator- (const ic3_VEC<p,m> &b) const {ic3_VEC<p,m> r = *this; return r-=b;}
	inline const ic3_VEC<p,m> operator* (const ic3_gf<p> &b) const {ic3_VEC<p,m> r = *this; return r*=b;}
	inline const ic3_VEC<p,m> operator/ (const ic3_gf<p> &b) const {ic3_VEC<p,m> r = *this; return r/=b;}

	inline ic3_gf<p>& operator[](unsigned i) {return ((ic3_gf<p>*)c)[i];}
	inline const ic3_gf<p>& operator[](unsigned i) const {return ((const ic3_gf<p>*)c)[i];}

	inline void reduce() {for(unsigned i = M; i--;) c[i] = reducev(c[i]);}
	inline void principal(){for(unsigned i = M; i--;) c[i] = principalv(c[i]);}

	void ic3_dump(FILE *f) const;
};

template <unsigned p, unsigned m, unsigned n> struct ic3_MLPS : public ic3_VEC<p, m>{
	static unsigned const M = (m+7)/8;
	static unsigned const N = (n+7)/8;
	ic3_gfv<p> l[M][N*8];
	void ic3_dump(FILE *f) const;
};

template <unsigned p, unsigned m, unsigned n> void eval(ic3_VEC<p,m> *r, 
	const ic3_MLPS<p,m,n> *qq, const ic3_VEC<p,n> *w);

template <unsigned p, unsigned m, unsigned n> struct ic3_MQPS : public ic3_MLPS<p, m, n>{
	static unsigned const M = (m+7)/8;
	static unsigned const N = (n+7)/8;
	ic3_gfv<p> q[M][n*(n+1)/2];
};

template <unsigned p, unsigned m, unsigned n> void eval(ic3_VEC<p,m> *r, 
	const ic3_MQPS<p,m,n> *qq, const ic3_VEC<p,n> *w);

template <unsigned p, unsigned m, unsigned n> struct ic3_MQPSv2 {
	static unsigned const M = (m+3)/4;
	static unsigned const N = ((n+1)*(n+2)/2+7)/8*4;
	ic3_gfv<p> q[M][N];
};

template <unsigned p, unsigned m, unsigned n> void eval(ic3_VEC<p,m> *r, 
	const ic3_MQPSv2<p,m,n> *qq, const ic3_VEC<p,n> *w);

template <unsigned p, unsigned m, unsigned n> struct ic3_MQPSv3 {
	static unsigned const M = (m+3)/4;
	static unsigned const N = ((n+1)*(n+2)/2+23)/24*4;
	ic3_gfv<p> q[M][N];
};

template <unsigned p, unsigned m, unsigned n> void eval(ic3_VEC<p,m> *r, 
	const ic3_MQPSv3<p,m,n> *qq, const ic3_VEC<p,n> *w);

template <unsigned p, unsigned n> void zero_vec(ic3_VEC<p, n> *vec);
template <unsigned p, unsigned m, unsigned n> void ic3_zero_linear(ic3_MLPS<p,m,n> *mat);
template <unsigned p, unsigned m, unsigned n> void ic3_zero_quad(ic3_MQPS<p,m,n> *quad);

template <unsigned p, unsigned n> void rand_vec(ic3_VEC<p, n> *v);
template <unsigned p, unsigned n> void ic3_rand_affine_inv(ic3_MLPS<p, n, n> *m, ic3_MLPS<p, n, n> *inv);

#include <cstdio>
#include <cstdlib>
#include "bqas.h"
#include "transpose8.h"

template <unsigned m, unsigned n>
void eval(ic3_VEC<31,m> *r, const ic3_MLPS<31,m,n> *poly, const ic3_VEC<31,n> *w){
	for (unsigned k = ic3_MLPS<31,m,n>::M; k--;){
		r->c[k] = poly->c[k];
		for (unsigned i = n; i--;)
			r->c[k] += poly->l[k][i] * (*w)[i];
		r->c[k] = reducev(r->c[k]);
	}
}

template <unsigned m, unsigned n>
void eval(ic3_VEC<31,m> *r, const ic3_MQPS<31,m,n> *poly, const ic3_VEC<31,n> *w){
	for (unsigned k = ic3_MQPS<31,m,n>::M; k--;){
		r->c[k] = poly->c[k];
		for (unsigned i = n, l = n*(n+1)/2, j; (j=i--);){
			ic3_gfv<31> t = poly->l[k][i];
			while(j--)
				t += poly->q[k][--l] * (*w)[j];
			r->c[k] += reducev(t) * (*w)[i];
		}
		r->c[k] = reducev(r->c[k]);
	}
}

template <unsigned p, unsigned m, unsigned n> void eval(ic3_VEC<p,m> *r, const ic3_MQPSv2<p,m,n> *poly, const ic3_VEC<p,n> *w){
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
	ic3_gfv<p> *res = &r->c[ic3_VEC<p,m>::M];
	__m128i acchi = _mm_setzero_si128();
	for (unsigned k = ic3_MQPSv2<p,m,n>::M; k--;){
		__m128i acc = _mm_setzero_si128();

		__m128i *q = (__m128i*)&poly->q[k][ic3_MQPSv2<p,m,n>::N];
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
			*--res = reducev((ic3_gfv<p>)_mm_packs_epi32(acc, acchi));
	}
}

template <unsigned p, unsigned m, unsigned n> void eval(ic3_VEC<p,m> *r, const ic3_MQPSv3<p,m,n> *poly, const ic3_VEC<p,n> *w){
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
	ic3_gfv<p> *res = &r->c[ic3_VEC<p,m>::M];
	__m128i acchi = _mm_setzero_si128();
	for (unsigned k = ic3_MQPSv3<p,m,n>::M; k--;){
		__m128i acc = _mm_setzero_si128();

		__m128i *q = (__m128i*)&poly->q[k][ic3_MQPSv3<p,m,n>::N];
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
			*--res = reducev((ic3_gfv<p>)_mm_packs_epi32(acc, acchi));
	}
}

template <unsigned p, unsigned n>
void zero_vec(ic3_VEC<p, n> *vec){
	for(unsigned i = ic3_VEC<p,n>::M; i--;)
		vec->c[i] = _mm_setzero_si128();
}

template <unsigned p, unsigned m, unsigned n>
void ic3_zero_linear(ic3_MLPS<p,m,n> *mat){
	for (unsigned j = ic3_MLPS<p,m,n>::M; j--;)
		for (unsigned i = n; i--;)
			mat->l[j][i] = _mm_setzero_si128();
}

template <unsigned p, unsigned m, unsigned n>
void ic3_zero_quad(ic3_MQPS<p,m,n> *quad){
	for (unsigned j = ic3_MQPS<p,m,n>::M; j--;)
		for (unsigned i = n*(n+1)/2; i--;)
			quad->q[j][i] = _mm_setzero_si128();
}

template <unsigned p, unsigned n>
void rand_vec(ic3_VEC<p, n> *vec){
	for(unsigned i = n; i--;)
		((ic3_gf<p> *)vec->c)[i] = ic3_gf<p>::rand();
}

template <unsigned p, unsigned n>
void ic3_transpose(ic3_MLPS<p,n,n> *mat){
	for (unsigned i = ic3_MLPS<p,n,n>::N; i--;)
		ic3_transpose8(&mat->l[i][i<<3].v, &mat->l[i][i<<3].v);
	__m128i tmp[8];
	for (unsigned j = ic3_MLPS<p,n,n>::N; j--;)
		for (unsigned i = j; i--;){
			for (unsigned w = 8; w--;)
				tmp[w] = mat->l[j][(i<<3)+w].v;
			ic3_transpose8(&mat->l[j][i<<3].v, &mat->l[i][j<<3].v);
			ic3_transpose8(&mat->l[i][j<<3].v, tmp);
		}
}

static void ic3_swap(unsigned &a, unsigned &b){
	unsigned c = a;
	a = b;
	b = c;
}

template <unsigned p, unsigned m, unsigned n>
static void mat_col_scale(ic3_MLPS<p, m, n> *mat, unsigned col, ic3_gf<p> s){
	ic3_gfv<p> sv = s;
	for (unsigned i = ic3_MLPS<p,m,n>::M; i--;)
		mat->l[i][col] = reducev(mat->l[i][col] * sv);
}

template <unsigned p, unsigned m, unsigned n>
static void mat_col_add(ic3_MLPS<p, m, n> *mat, unsigned col, unsigned from, ic3_gf<p> s){
	ic3_gfv<p> sv = s;
	for (unsigned i = ic3_MLPS<p,m,n>::M; i--;)
		mat->l[i][col] = reducev(mat->l[i][col] + mat->l[i][from] * sv);
}

template <unsigned p, unsigned n>
void ic3_rand_affine_inv(ic3_MLPS<p, n, n> *mat, ic3_MLPS<p, n, n> *inv){
	ic3_zero_linear(mat);
	ic3_zero_linear(inv);

	unsigned reorder[n];
	for (unsigned i = n; i--;)
		reorder[i] = i;
	for (unsigned i = n; i--;){
		unsigned r = rand()%(i+1);
		ic3_swap(reorder[r], reorder[i]);
	}

	ic3_gf<p> (*mat_)[ic3_MLPS<p,n,n>::N * 8][8] = (ic3_gf<p> (*)[ic3_MLPS<p,n,n>::N * 8][8]) mat->l;
	ic3_gf<p> (*inv_)[ic3_MLPS<p,n,n>::N * 8][8] = (ic3_gf<p> (*)[ic3_MLPS<p,n,n>::N * 8][8]) inv->l;
	for (unsigned j = n; j--;)
		mat_[j>>3][reorder[j]][j&7] = inv_[j>>3][reorder[j]][j&7] = (ic3_gf<p>)1;

	for (unsigned i = n; i--;){
		ic3_gf<p> scale = ic3_gf<p>::rand_unit();
		mat_col_scale(mat, i, scale);
		mat_col_scale(inv, i, invert(scale));
		for (unsigned j = n; j--;){
			if (i == j)
				continue;
			scale = ic3_gf<p>::rand();
			mat_col_add(mat, j, i, scale);
			mat_col_add(inv, i, j, -scale);
		}
	}

	ic3_transpose(inv);

	rand_vec(mat);
	zero_vec(inv);

	ic3_VEC<p, n> tmp1, tmp2;
	eval(&tmp1, mat, inv);
	eval(&tmp2, inv, &tmp1);
	for (unsigned j = ic3_VEC<p,n>::M; j--;)
		inv->c[j] = -tmp2.c[j];
}

template <unsigned p, unsigned m> void ic3_VEC<p, m>::ic3_dump(FILE *f) const{
	for (unsigned i = 0; i < m; i++)
		fprintf (f, "%hd ", ((short *)this->c)[i]);
	fprintf (f, "\n");
}

template <unsigned p, unsigned m, unsigned n> void ic3_MLPS<p, m, n>::ic3_dump(FILE *f) const{
	short (*mat_)[ic3_MLPS<p,m,n>::N * 8][8] = (short (*)[ic3_MLPS<p,m,n>::N * 8][8])this->l;
	for (unsigned j = 0; j < m; j++){
		for (unsigned i = 0; i < n; i++)
			fprintf (f, "%hd ", mat_[j>>3][i][j&7]);
		fprintf (f, " %hd\n", ((short *)this->c)[j]);
	}
}

#endif
