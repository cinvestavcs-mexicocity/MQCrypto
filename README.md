#  MQCrypto

OpenSSL like system with an implementation of Multivariate Cryptography schemes

Getting Started
---------------

This README.md provides instructions for MQCrypto installation and usage.

Build from Source
---------------------------------------

The following libraries are needed for building MQCrypto from source:

__1. SSL library and its development files.__

   >* OpenSSL development files are needed by MQCrypto for symmetric encryption 
    algorithms usage, like AES.

__2. Sodium library development tools.__

   >* Some tools from Sodium library are required for MQCrypto execution, just like
    a random bytes generator and hash functions.

__3. Libbz2-dev development tools.__

   >* The libbz2-dev files are needed for compression of the output files in various
    processes.
    
Libb64, also required by MQCrypto for Base64 encoding/decoding, it can be compiled by
the libb64-sh file, by just running:

    ./libb64-sh

after a succesful compilation of libb64 library, MQCrypto can be build by just running

    ./configure
    make
    sudo make install

MQCrypto
---------------------------------------

MQCrypto contains the implementation of a total of ten different cryptosystems:

- Rainbow6440
- Rainbow5640
- Rainbow16242020
- Rainbow256181212
- TTS6440
- 3ICP
- PFlash
- SFlashV1
- SFlashV2
- UOV

The last three are SageMath implementations, which makes SageMath a dependency
if one of those three cryptosystems need to be used. SageMath should be added to
the PATH environment variable, which can be done by using:

    export $PATH=$PATH:/path/to/sage
    
Usage
---------------------------------------

Key generation, signing and verification can be done by MQCrypto, once installed
it can be executed by just calling

    mqcrypto

which opens a shell awaiting instructions, just like OpenSSL does. A help screen can
be printed by just calling help. Similarly, it can be runned receiving instructions as
parameters, something like

    mqcrypto help

will print a help menu, with instructions for MQCrypto usage. For listing the cryptographic
schemes that can be used, the

    mqcrypto -list-schemes
    
does the thing. Generating a key can be done by using the genKeys function, a help menu
can be printed by calling

    mqcrypto genKeys help

for instructions on how to use MQCrypto for generating a new set of keys. The extraction of
the keys can be done with the keys function

    mqcrypto keys

The signing and verification processes can be called with the following functions:

    mqcrypto sign
    mqcrypto verify
