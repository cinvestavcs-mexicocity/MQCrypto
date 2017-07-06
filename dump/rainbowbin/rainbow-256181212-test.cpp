#include "gf.h"
#include "blas.h"
#include "rainbow.h"
#include <ctime>

/////////// sizes.h //////////////
#define RAINBOW256181212_SK_BYTES 23408
#define RAINBOW256181212_PK_BYTES 30240
#define RAINBOW256181212_SIG_BYTES 42
#define RAINBOW256181212_HASH_BYTES 24
/////////// sizes.h ////////////////


#define RAINBOW256181212_TEST_ROUND 1000
#define RAINBOW256181212_GEN_KEY_ROUND 50

#define RAINBOW256181212_GF 256
#define RAINBOW256181212_N 42
#define RAINBOW256181212_M 24
#define RAINBOW256181212_P 0
#define RAINBOW256181212_SCHEME "rainbow 18 12 12"

typedef rainbow<RAINBOW256181212_GF,18,12,12> RAINBOW256181212_PKC;



int rainbow256181212_keypair( unsigned char sk[RAINBOW256181212_SK_BYTES] , unsigned long long * 
sklen , unsigned char pk[RAINBOW256181212_PK_BYTES] , unsigned long long * pklen )
{
	RAINBOW256181212_PKC & pkc = * RAINBOW256181212_PKC::get_instance();
	pkc.gen_key( pk , sk );
	* sklen = RAINBOW256181212_SK_BYTES;
	* pklen = RAINBOW256181212_PK_BYTES;
	return 0;
}


int rainbow256181212_signature(
  unsigned char sm[RAINBOW256181212_SIG_BYTES],unsigned long long *smlen,
  const unsigned char m[RAINBOW256181212_HASH_BYTES],const unsigned long long mlen,
  const unsigned char sk[RAINBOW256181212_SK_BYTES],const unsigned long long sklen 
)
{
	if( sklen != RAINBOW256181212_SK_BYTES ) return -1;
	if (mlen > RAINBOW256181212_HASH_BYTES) return -1;

	unsigned char h[RAINBOW256181212_M];
	for(int i=0;i<RAINBOW256181212_M;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif

	* smlen = RAINBOW256181212_SIG_BYTES;

	gfv<RAINBOW256181212_GF,RAINBOW256181212_N> x;
	gfv<RAINBOW256181212_GF,RAINBOW256181212_M> y;
	memcpy( &y , h ,RAINBOW256181212_M );
	RAINBOW256181212_PKC & pkc = * RAINBOW256181212_PKC::get_instance();
//	if( 0 != pkc.private_map( x , sk , y )) return -1;
	pkc.private_map( x , sk , y );
	memcpy( sm , &x , RAINBOW256181212_N );

	return 0;
}

int rainbow256181212_verification(
    const unsigned char m[RAINBOW256181212_HASH_BYTES],const unsigned long long mlen,
    const unsigned char sm[RAINBOW256181212_SIG_BYTES],const unsigned long long smlen,
    const unsigned char pk[RAINBOW256181212_PK_BYTES],const unsigned long long pklen )
{
	if (smlen != RAINBOW256181212_SIG_BYTES) return -100;
	if (mlen != RAINBOW256181212_HASH_BYTES) return -100;
	if( pklen != RAINBOW256181212_PK_BYTES ) return -1;

	unsigned char h[RAINBOW256181212_M];
	for(int i=0;i<RAINBOW256181212_M;i++) h[i]=m[i];
#ifdef _USE_SHA1_
	SHA1(m,mlen,h);
#endif
	gfv<RAINBOW256181212_GF,RAINBOW256181212_N> x;
	gfv<RAINBOW256181212_GF,RAINBOW256181212_M> y;
	memcpy( &x , sm ,RAINBOW256181212_N );
	RAINBOW256181212_PKC & pkc = * RAINBOW256181212_PKC::get_instance();
	pkc.public_map( y , pk , x );

	unsigned char check[RAINBOW256181212_M];
	memcpy( check , &y , RAINBOW256181212_M );
	for(int i=0;i<RAINBOW256181212_M;i++) if(h[i] != check[i]) return -100;

	return 0;

}



#if 1
int main()
{
	srand(time(0));
	unsigned char m[RAINBOW256181212_HASH_BYTES];
	unsigned long long mlen = RAINBOW256181212_HASH_BYTES;
	unsigned char sm[RAINBOW256181212_SIG_BYTES];
	unsigned long long smlen;
	unsigned char pk[RAINBOW256181212_PK_BYTES];
	unsigned long long pklen;
	unsigned char sk[RAINBOW256181212_SK_BYTES];
	unsigned long long sklen;

	for(int i=0;i<RAINBOW256181212_HASH_BYTES;i++) 
		m[i]=rand()%256;
	rainbow256181212_keypair( sk , &sklen , pk , &pklen );
	for( int i=0;i<RAINBOW256181212_HASH_BYTES;i++) 
		printf("%x " , (unsigned) m[i]); printf("\n");
	rainbow256181212_signature(sm,&smlen,m,mlen,sk,sklen);
	for( int i=0;i<RAINBOW256181212_SIG_BYTES;i++) 
		printf("%x " , (unsigned) sm[i]); printf("\n");
	int i = rainbow256181212_verification(m,mlen,sm,smlen,pk,pklen);
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
	
	RAINBOW256181212_PKC & pkc = * RAINBOW256181212_PKC::get_instance();

	unsigned size_pub = pkc.sizeof_public_key();
	unsigned size_pri = pkc.sizeof_private_key();
	printf("%s : %d --> %d  , whith RAINBOW256181212_P=%d\n" , RAINBOW256181212_SCHEME , RAINBOW256181212_N , RAINBOW256181212_M , RAINBOW256181212_P );
	printf("pub: %d , pri: %d \n" , size_pub , size_pri );

	byte * pub = new byte [size_pub];
	byte * pri = new byte [size_pri];
	pkc.gen_key( pub , pri );

	gfv<RAINBOW256181212_GF,RAINBOW256181212_N-RAINBOW256181212_P> x = gfv<RAINBOW256181212_GF,RAINBOW256181212_N-RAINBOW256181212_P>::rand();
	gfv<RAINBOW256181212_GF,RAINBOW256181212_M> y;

	printf("inv q/q map:\n");
	y = gfv<RAINBOW256181212_GF,RAINBOW256181212_M>::rand();
	y.dump( stdout );
	pkc.private_map( x , pri , y );
	x.dump( stdout );
	pkc.public_map( y , pub , x );
	y.dump( stdout );

	gfv<RAINBOW256181212_GF,RAINBOW256181212_N-RAINBOW256181212_P> x1;
	gfv<RAINBOW256181212_GF,RAINBOW256181212_M> y1;

	unsigned sign_err = 0;
	unsigned enc_err = 0;

	for( int i = 0 ; i < RAINBOW256181212_TEST_ROUND ; i++ )
	{
//		if( i<RAINBOW256181212_GEN_KEY_ROUND) BENCHMARK(gen,(pkc.gen_key(pub,pri)) );

//		x = gfv<RAINBOW256181212_GF,RAINBOW256181212_N-RAINBOW256181212_P>::rand();
//		BENCHMARK(pub_map,( pkc.public_map(y,pub,x) ));
//		BENCHMARK(sec_map,( pkc.private_map(x1,pri,y) ));
//		for( unsigned j = RAINBOW256181212_N-RAINBOW256181212_P ; j-- ;  ) if(x[j]!=x1[j]) { enc_err++; break; }
//		if( enc_err > 0 && enc_err < 4 ) { puts("enc/dec error."); x.dump( stdout ); y.dump( stdout ); x1.dump( stdout ); }

//		y = gfv<RAINBOW256181212_GF,RAINBOW256181212_M>::rand();
//		BENCHMARK(sec_map,( pkc.private_map(x,pri,y) ));
//		BENCHMARK(pub_map,( pkc.public_map(y1,pub,x) ));
//		for( unsigned j = RAINBOW256181212_M ; j-- ;  ) if(y[j]!=y1[j]) { sign_err++; break; }
//		if( sign_err > 0 && sign_err < 4 ) { puts("sign/veri error."); y.dump( stdout ); x.dump( stdout ); y1.dump( stdout ); }

	}
//	printf("\nenc/dec error:%d/%d\nsign/veri error:%d/%d\n",enc_err,RAINBOW256181212_TEST_ROUND,sign_err,RAINBOW256181212_TEST_ROUND);
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
