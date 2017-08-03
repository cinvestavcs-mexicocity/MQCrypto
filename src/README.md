# MQCrypto
OpenSSL like system with an implementation of Multivariate Cryptography schemes

**List of dependencies**

- openssl
- libsodium
- asn1c
- libb64
- libbzip2

**Compile:**

```g++ -O2 mqcrypto.cpp -o mqcrypto -fno-strict-aliasing -Isignature/ -Iasn1/ -I. -Iencryption/ asn1/*.c b64/libb64.a /usr/local/lib/libsodium.a -I /usr/local/ssl/include/ -lcrypto -Iheaders/ -std=c++11 /usr/local/lib/libbz2.a```

```g++ mqcrypto.cpp -o mqcrypto -Isignature/ -Iasn1/ -I. -Iencryption/ asn1/*.cpp b64/libb64.a -lsodium -I /usr/local/ssl/include/ -lcrypto -std=c++11 -lbz2 -Wno-write-strings -Wno-deprecated-register -Wno-format-security -fno-strict-aliasing -O2```
