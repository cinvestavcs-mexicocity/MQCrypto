#define RAINBOW16242020_SK_BYTES 94384
#define RAINBOW16242020_PK_BYTES 102912
#define RAINBOW16242020_SIG_BYTES 32
#define RAINBOW16242020_HASH_BYTES 20

#define RAINBOW16242020_TEST_ROUND 1000
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
	
	for(int i=0;i<(RAINBOW16242020_M>>1);i++) 
		if(m[i] != check[i]) {
			std::cout<<"Sign verify: Fail\n";
			return -100;
		}

	std::cout<<"Sign verify: OK\n";
	
	return 0;

}