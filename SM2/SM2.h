#pragma once
#include <iostream>
#include <algorithm>
#include <cstring>
#include "SM3.h"
#include "NTL/ZZ.h"

using namespace std;
using namespace NTL;

string ZZ_to_bin(ZZ x);

ZZ bin_to_ZZ(string x);

string bin_to_hex(string x);

string hex_to_bin(string x);

ZZ invmod(ZZ a, ZZ n);

void addition(ZZ x1, ZZ y1, ZZ x2, ZZ y2, ZZ& x3, ZZ& y3, ZZ a, ZZ p);

void mutipoint(ZZ x, ZZ y, ZZ k, ZZ a, ZZ p, ZZ& qx, ZZ& qy);

string kdf(string z, unsigned int klen);

void encrypt(string m, ZZ xB, ZZ yB, ZZ a, ZZ b, ZZ p, string& c1, string& c2, string& c3);

void decrypt(string c1, string c2, string c3, ZZ a, ZZ b, ZZ p, ZZ dB, string& output);

void keyGen(ZZ dB, ZZ xB, ZZ yB);