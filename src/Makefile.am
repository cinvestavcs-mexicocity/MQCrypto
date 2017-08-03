AUTOMAKE_OPTIONS = subdir-objects

SUBDIRS = sage

AM_FLAGS = -O2 -std=c++11 -mmmx -msse -msse2 -msse3 -msse4.1
WARNING_FLAGS = -Wno-write-strings -Wno-deprecated-register -Wno-format-security -fno-strict-aliasing -Wno-unused-result

bin_PROGRAMS = mqcrypto

mqcrypto_SOURCES = \
        mqcrypto.cpp \
        asn1/ASN1Encode.cpp \
        asn1/BIT_STRING.cpp \
        asn1/IA5String.cpp \
        asn1/MPKCKeypair.cpp \
        asn1/MPKCMessage.cpp \
        asn1/MPKCPrivateKey.cpp \
        asn1/MPKCPublicKey.cpp \
        asn1/MPKCSignature.cpp \
        asn1/OCTET_STRING.cpp \
        asn1/asn_codecs_prim.cpp \
        asn1/ber_decoder.cpp \
        asn1/ber_tlv_length.cpp \
        asn1/ber_tlv_tag.cpp \
        asn1/constr_SEQUENCE.cpp \
        asn1/constr_TYPE.cpp \
        asn1/constraints.cpp \
        asn1/der_encoder.cpp \
        asn1/per_decoder.cpp \
        asn1/per_encoder.cpp \
        asn1/per_opentype.cpp \
        asn1/per_support.cpp \
        asn1/xer_decoder.cpp \
        asn1/xer_encoder.cpp \
        asn1/xer_support.cpp
mqcrypto_CPPFLAGS = @openssl_CFLAGS@ \
        @libsodium_CFLAGS@ \
        -I. \
        -Isignature \
        -Iencryption \
        -Iasn1 \
        $(AM_FLAGS) \
        $(WARNING_FLAGS)
mqcrypto_LDFLAGS = -msse4.1
mqcrypto_LDADD = @openssl_LIBS@ \
        @libsodium_LIBS@ \
        b64/libb64.a
