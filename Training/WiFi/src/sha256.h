#ifndef SHA256_H
#define SHA256_H

#define SHA256_LEN 32 // bytes, i.e. 256 bits

void sha256(unsigned char * text, unsigned char hash[SHA256_LEN]);

#endif   // SHA256_H