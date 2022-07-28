#pragma once
#include <iostream>
#include <cstring>
#include "SM2.h"
#include "SM3.h"
#include "SM4.h"

using namespace std;

string PGP_sign(string message, string password, string SM4cipher);

string PGP_vrfy(string cipher, ZZ xB, ZZ yB);

string PGP_enc(string plain, ZZ xB, ZZ yB);

string PGP_dec(string cipher, ZZ dB);