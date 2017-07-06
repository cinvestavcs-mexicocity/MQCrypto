#include "gf.h"
#include "blas.h"
#include "rainbow.h"
#include <ctime>

/////////// sizes.h //////////////
#define RAINBOW16242020_SK_BYTES 94384
#define RAINBOW16242020_PK_BYTES 102912
#define RAINBOW16242020_SIG_BYTES 32
#define RAINBOW16242020_HASH_BYTES 20
/////////// sizes.h ////////////////


#define RAINBOW6440_TEST_ROUND 1000
#define RAINBOW16242020_GEN_KEY_ROUND 50

#define RAINBOW16242020_GF 16
#define RAINBOW16242020_N 64
#define RAINBOW16242020_M 40
#define RAINBOW16242020_P 0
#define RAINBOW16242020_SCHEME "rainbow 16,24,20,20"

typedef rainbow<RAINBOW16242020_GF,24,20,20> RAINBOW16242020_PKC;



int rainbow16242020_keypair( unsigned char sk[RAINBOW16242020_SK_BYTES] , unsigned long long * 
sklen , unsigned char pk[RAINBOW16242020_PK_BYTES] , unsigned long long * pklen )
{
	RAINBOW16242020_PKC & pkc = * RAINBOW16242020_PKC::get_instance();
	pkc.gen_key( pk , sk );
	* sklen = RAINBOW16242020_SK_BYTES;
	* pklen = RAINBOW16242020_PK_BYTES;
	return 0;
}


int rainbow16242020_signature(
  unsigned char sm[RAINBOW16242020_SIG_BYTES],unsigned long long *smlen,
  const unsigned char m[RAINBOW16242020_HASH_BYTES],const unsigned long long mlen,
  const unsigned char sk[RAINBOW16242020_SK_BYTES],const unsigned long long sklen 
)
{
	if( sklen != RAINBOW16242020_SK_BYTES ) return -1;
	if (mlen > RAINBOW16242020_HASH_BYTES) return -1;

	unsigned char h[RAINBOW16242020_M];
	unsigned char mm = 0xf;
	for(int i=0;i<RAINBOW16242020_M;i+=2){
		h[i] = m[i>>1]&mm;
		h[i+1] = m[i>>1]>>4;
	}

	* smlen = RAINBOW16242020_SIG_BYTES;

	gfv<RAINBOW16242020_GF,RAINBOW16242020_N> x;
	gfv<RAINBOW16242020_GF,RAINBOW16242020_M> y;
	memcpy( &y , h , RAINBOW16242020_M );
	RAINBOW16242020_PKC & pkc = * RAINBOW16242020_PKC::get_instance();
//	if( 0 != pkc.private_map( x , sk , y )) return -1;
//	y.dump( stdout );
	pkc.private_map( x , sk , y );
//	x.dump( stdout );

	unsigned char * r = (unsigned char *)&x;

	for(int i=0;i<(RAINBOW16242020_N>>1);i++) sm[i]=r[i<<1]|(r[(i<<1)+1]<<4);

	return 0;
}

int rainbow16242020_verification(
    const unsigned char m[RAINBOW16242020_HASH_BYTES],const unsigned long long mlen,
    const unsigned char sm[RAINBOW16242020_SIG_BYTES],const unsigned long long smlen,
    const unsigned char pk[RAINBOW16242020_PK_BYTES],const unsigned long long pklen )
{
	if (smlen != RAINBOW16242020_SIG_BYTES) return -100;
	if (mlen != RAINBOW16242020_HASH_BYTES) return -100;
	if( pklen != RAINBOW16242020_PK_BYTES ) return -1;

	unsigned char h[RAINBOW16242020_N];
	unsigned char mm = 0xf;
	for(int i=0;i<RAINBOW16242020_N;i+=2) { 
		h[i]=sm[i>>1]&mm;
		h[i+1]=(sm[i>>1]>>4); 
	}

	gfv<RAINBOW16242020_GF,RAINBOW16242020_N> x;
	gfv<RAINBOW16242020_GF,RAINBOW16242020_M> y;
	memcpy( &x , h ,RAINBOW16242020_N );
	RAINBOW16242020_PKC & pkc = * RAINBOW16242020_PKC::get_instance();
//	x.dump(stdout);
	pkc.public_map( y , pk , x );
//	y.dump(stdout);
	unsigned char * r = (unsigned char *)& y;

	unsigned char check[RAINBOW16242020_M>>1];
	for(int i=0;i<(RAINBOW16242020_M>>1);i++) { check[i] = r[i<<1]|(r[(i<<1)+1]<<4); }
	for(int i=0;i<(RAINBOW16242020_M>>1);i++) if(m[i] != check[i]) return -100;

	return 0;

}

#if 1
int main()
{
	srand(time(0));
	unsigned char m[RAINBOW16242020_HASH_BYTES];
	unsigned long long mlen = RAINBOW16242020_HASH_BYTES;
	unsigned char sm[RAINBOW16242020_SIG_BYTES];
	unsigned long long smlen;
	unsigned char pk[RAINBOW16242020_PK_BYTES];
	unsigned long long pklen;
	unsigned char sk[RAINBOW16242020_SK_BYTES];
	unsigned long long sklen;

	for(int i=0;i<RAINBOW16242020_HASH_BYTES;i++) 
		m[i]=rand()%256;
	rainbow16242020_keypair( sk , &sklen , pk , &pklen );
	for( int i=0;i<RAINBOW16242020_HASH_BYTES;i++) 
		printf("%x " , (unsigned) m[i]); printf("\n");
	rainbow16242020_signature(sm,&smlen,m,mlen,sk,sklen);
	for( int i=0;i<RAINBOW16242020_SIG_BYTES;i++) 
		printf("%x " , (unsigned) sm[i]); printf("\n");
	int i = rainbow16242020_verification(m,mlen,sm,smlen,pk,pklen);
	printf("verify: %d\n" , i );
	// printf("%d\n",i);
	return 0;
}

#endif



#if 0
int main()
{
//	char buf[100];
//	struct benchmark pub_map;
//	struct benchmark sec_map;
//	struct benchmark gen;
//	bm_init(&pub_map);
//	bm_init(&sec_map);
//	bm_init(&gen);
	
//	srand(time(NULL));
	
	RAINBOW16242020_PKC & pkc = * RAINBOW16242020_PKC::get_instance();

	unsigned size_pub = pkc.sizeof_public_key();
	unsigned size_pri = pkc.sizeof_private_key();
	printf("%s : %d --> %d  , whith RAINBOW16242020_P=%d\n" , RAINBOW16242020_SCHEME , RAINBOW16242020_N , RAINBOW16242020_M , RAINBOW16242020_P );
	printf("pub: %d , pri: %d \n" , size_pub , size_pri );

	byte * pub = new byte [size_pub];
	byte * pri = new byte [size_pri];
	pkc.gen_key( pub , pri );

	gfv<RAINBOW16242020_GF,RAINBOW16242020_N-RAINBOW16242020_P> x = gfv<RAINBOW16242020_GF,RAINBOW16242020_N-RAINBOW16242020_P>::rand();
	gfv<RAINBOW16242020_GF,RAINBOW16242020_M> y;

	printf("inv q/q map:\n");
	y = gfv<RAINBOW16242020_GF,RAINBOW16242020_M>::rand();
	y.dump( stdout );
	pkc.private_map( x , pri , y );
	x.dump( stdout );
	pkc.public_map( y , pub , x );
	y.dump( stdout );

	gfv<RAINBOW16242020_GF,RAINBOW16242020_N-RAINBOW16242020_P> x1;
	gfv<RAINBOW16242020_GF,RAINBOW16242020_M> y1;

	unsigned sign_err = 0;
	unsigned enc_err = 0;

	for( int i = 0 ; i < RAINBOW6440_TEST_ROUND ; i++ )
	{
//		if( i<RAINBOW16242020_GEN_KEY_ROUND) BENCHMARK(gen,(pkc.gen_key(pub,pri)) );

//		x = gfv<RAINBOW16242020_GF,RAINBOW16242020_N-RAINBOW16242020_P>::rand();
//		BENCHMARK(pub_map,( pkc.public_map(y,pub,x) ));
//		BENCHMARK(sec_map,( pkc.private_map(x1,pri,y) ));
//		for( unsigned j = RAINBOW16242020_N-RAINBOW16242020_P ; j-- ;  ) if(x[j]!=x1[j]) { enc_err++; break; }
//		if( enc_err > 0 && enc_err < 4 ) { puts("enc/dec error."); x.dump( stdout ); y.dump( stdout ); x1.dump( stdout ); }

//		y = gfv<RAINBOW16242020_GF,RAINBOW16242020_M>::rand();
//		BENCHMARK(sec_map,( pkc.private_map(x,pri,y) ));
//		BENCHMARK(pub_map,( pkc.public_map(y1,pub,x) ));
//		for( unsigned j = RAINBOW16242020_M ; j-- ;  ) if(y[j]!=y1[j]) { sign_err++; break; }
//		if( sign_err > 0 && sign_err < 4 ) { puts("sign/veri error."); y.dump( stdout ); x.dump( stdout ); y1.dump( stdout ); }

	}
//	printf("\nenc/dec error:%d/%d\nsign/veri error:%d/%d\n",enc_err,RAINBOW6440_TEST_ROUND,sign_err,RAINBOW6440_TEST_ROUND);
//	bm_dump(buf, sizeof(buf), &gen);
//	printf("gen key\n%s\n", buf);
//	bm_dump(buf, sizeof(buf), &pub_map);
//	printf("pub map\n%s\n", buf);
//	bm_dump(buf, sizeof(buf), &sec_map);
//	printf("pri map\n%s\n", buf);
	

	delete [] pub;
	delete [] pri;
	return 0;	
}
#endif
