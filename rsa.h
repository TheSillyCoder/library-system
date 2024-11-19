#pragma once

#include <stdio.h>

#define PUBLIC_KEY 11
#define PRIVATE_KEY 284531
#define PRIME_PROD 348977
int encryptFile(char* file, char* out_file, int public_key, unsigned long prime_product);
int decryptFile(char* infile, char* outfile, int private_key, unsigned long prime_product);
unsigned long int encrypt(unsigned long int msg, int public_key, unsigned long prime_product);
unsigned long int decrypt(unsigned long int enc, int private_key, unsigned prime_product);
unsigned long* setkeys();
unsigned long int returnRandom(unsigned long int min, unsigned long int max);
unsigned long int diff(unsigned long int x, unsigned long int y);
unsigned long int gcd(unsigned long int x, unsigned long int y);