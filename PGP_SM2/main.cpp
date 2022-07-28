#include <iostream>
#include <cstring>
#include "SM2.h"
#include "SM3.h"
#include "SM4.h"
#include "PGP.h"

using namespace std;

int main()
{
	string input, key, d;
	ZZ dB, xB, yB;
	string cipher, plain, signiture, verify;
	key = "0123456789abcdeffedcba9876543210";
	input = "0123456789abcdeffedcba9876543210";
	KeyExtension(key);
	keyGen(dB, xB, yB);
	d = bin_to_hex(ZZ_to_bin(dB)) + "||" + bin_to_hex(ZZ_to_bin(xB)) + "||" + bin_to_hex(ZZ_to_bin(yB));
	//cipher = SM4_enc(input, key);
	//cout << SM4_dec(cipher, key);
	
	cipher = PGP_enc(input, xB, yB);
	cout << cipher << endl;
	plain = PGP_dec(cipher, dB);
	cout << plain << endl;

	input = "0123456789abcdeffedcba9876543210";
	signiture = PGP_sign(input, key, d);
	cout << signiture << endl;
	verify = PGP_vrfy(signiture, xB, yB);
	cout << verify << endl;
	return 0;
}