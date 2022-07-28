#include <iostream>
#include <cstring>
#include "SM2.h"
#include "SM3.h"
#include "SM4.h"

using namespace std;

string PGP_sign(string message, string password, string SM4cipher)
{
	string hm, Za, sign_r, sign_s, sign, SM4_key,key , plain;
	ZZ s_dB, s_xB, s_yB;
	/*string salt = "12345678987654321";//每个用户随机生成，此处为简便起见同时便于实现算法，采用固定字符串，可以改随机
	SM4_key = SM3(password + salt);//用于SM4解密
	key = SM4_key.substr(0, 128);
	KeyExtension(key);
	plain = SM4_dec(SM4cipher, key);
	*/
	plain = SM4cipher;
	//------------获取签名密钥------------
	s_dB = bin_to_ZZ(hex_to_bin(plain.substr(0, plain.find("||", 0, 2))));
	plain.erase(0, plain.find("||", 0, 2) + 2);
	s_xB = bin_to_ZZ(hex_to_bin(plain.substr(0, plain.find("||", 0, 2))));
	plain.erase(0, plain.find("||", 0, 2) + 2);
	s_yB = bin_to_ZZ(hex_to_bin(plain));
	hm = SM3(message);
	SM2_sign(hm, s_xB, s_yB, s_dB, Za, sign_r, sign_s);
	sign = sign_r + "||" + sign_s + "||" + Za + "||" + message;//需要保证r和s为256位
	return sign;
}

string PGP_vrfy(string cipher, ZZ xB, ZZ yB)
{
	string sign_r, sign_s, message, hm, Za;
	sign_r = cipher.substr(0, cipher.find("||", 0, 2));
	cipher.erase(0, cipher.find("||", 0, 2) + 2);
	sign_s = cipher.substr(0, cipher.find("||", 0, 2));
	cipher.erase(0, cipher.find("||", 0, 2) + 2);
	Za = cipher.substr(0, cipher.find("||", 0, 2));
	cipher.erase(0, cipher.find("||", 0, 2) + 2);
	message = cipher;
	hm = SM3(message);
	if (SM2_vrfy(sign_r, sign_s, Za, hm, xB, yB))
	{
		return message;
	}
	else return "Verification Failed";
}

string PGP_enc(string plain, ZZ xB, ZZ yB)
{
	string key, c1, c2, c3, ret, cipher;
	long lc1, lc2, lc3;
	key = "0123456789abcdeffedcba9876543210";//为便于检查设置密钥为固定，可以改成随机
	KeyExtension(key);
	cipher = SM4_enc(plain, key);
	SM2_enc(key, xB, yB, c1, c2, c3);
	ret = c1 + "||" + c3 + "||" + c2 + "||" + cipher;
	return ret;
}

string PGP_dec(string cipher, ZZ dB)
{
	string c1, c2, c3, key, msg, ret;
	c1 = cipher.substr(0, cipher.find("||", 0, 2));
	cipher.erase(0, cipher.find("||", 0, 2) + 2);
	c3 = cipher.substr(0, cipher.find("||", 0, 2));
	cipher.erase(0, cipher.find("||", 0, 2) + 2);
	c2 = cipher.substr(0, cipher.find("||", 0, 2));
	cipher.erase(0, cipher.find("||", 0, 2) + 2);
	msg = cipher;
	SM2_dec(c1, c2, c3, dB, key);
	key = hex_to_bin(key);
	key = string(128 - key.length(), '0') + key;
	key = bin_to_hex(key);
	KeyExtension(key);
	ret = SM4_dec(msg, key);
	return ret;
}