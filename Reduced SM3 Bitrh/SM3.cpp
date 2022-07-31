#include <iostream>
#include <cstring>
#include <string>

using namespace std;

string iv = "7380166f";

string to_hex(unsigned int x, unsigned int length)
{
	char c[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
	string tmp(length,'0');
	unsigned int r = 0, i = length - 1;
	while (x != 0)
	{
		r = x % 16;
		tmp[i] = c[r];
		--i;
		x >>= 4;
	}
	return tmp;
}

unsigned long to_dec(string s)
{
	unsigned long len = 0, base = 1, ret = 0;
	len = s.length();
	for (int i = len - 1; i >= 0; i--)
	{
		int num = 0;
		if (s[i] >= '0' && s[i] <= '9') num = s[i] - '0';
		else num = s[i] - 'a' + 10;
		ret += num * base;
		base <<= 4;
	}
	return ret;
}

unsigned long t(int j)
{
	if (j < 16) return 0x79cc4519;
	else return 0x7a879d8a;
}

unsigned long csl(unsigned long x, int k)
{
	if (k == 0 || k==32) return x;
	else
	{
		unsigned long ret = 0, tmp = 0;
		tmp = x >> (32 - k);
		ret = x << k;
		ret += tmp;
		return ret;
	}
}

unsigned long ff(unsigned long x, unsigned long y, unsigned long z, int j)
{
	if (j < 16) return x ^ y ^ z;
	else return (x & y) | (y & z) | (z & x);
}

unsigned long gg(unsigned long x, unsigned long y, unsigned long z, int j)
{
	if (j < 16)	return x ^ y ^ z;
	else return (x & y) | (~x & z);
}

unsigned long p0(unsigned long x)
{
	return x ^ csl(x, 9) ^ csl(x, 17);
}
	
unsigned long p1(unsigned long x)
{
	return x ^ csl(x, 15) ^ csl(x, 23);
}

string fill(string m) 
{
	/*
	* 输入为十六进制字符串，没有0x符号，小写
	*/
	unsigned int l, k, len;
	len = m.length();
	l = len * 4;
	m = m + '8';
	k = 112 - (m.length() % 128);
	m = m + string(k, '0') + to_hex(l, 16);
	//m = m + '0' * k + '{:016x}'.format(l)
	return m;
}

void grouping(string m, string b[])
{
	int n;
	n = m.length() / 32;
	for (int i = 0; i < n; i++) b[i].append(m.substr(i * 32, 32));
}

void extend(string bi, unsigned long w[])
{
	for (int i = 0; i < 132; i++)
	{
		if (i < 16)
		{
			w[i] = to_dec(bi);
		}
		else if (i < 68)
		{
			w[i] = p1(w[i - 16] ^ w[i - 9] ^ csl(w[i - 3], 15)) ^ csl(w[i - 13], 7) ^ w[i - 6];
		}
		else
		{
			w[i] = w[i - 68] ^ w[i - 64];
		}
	}
}

string cf(string vi, string bi)
{
	string tmp, ret(8, '0');
	unsigned long w[132] = { 0 };
	unsigned long a, b, c, d, e, f, g, h;
	unsigned long ss1 = 0, ss2 = 0, tt1 = 0, tt2 = 0;
	extend(bi, w);
	a = to_dec(vi.substr(0, 8));
	for (int i = 0; i < 16; i++)
	{
		unsigned long long tmpnum = 0;
		ss1 = csl((csl(a, 12) + a + csl(t(i), i)) & 0xFFFFFFFF, 7);
		ss2 = ss1 ^ csl(a, 12);
		tt1 = (a + ss2 + w[i + 68]) & 0xFFFFFFFF;//同上
		tt2 = (a + ss1 + w[i]) & 0xFFFFFFFF;//同上
		a = csl(a, 9);
		a += tt1;
		a = a^p0(tt2);
	}
	tmp = to_hex(a ^ to_dec(vi), 8);

	for (int i = 0; i < 8; i++)
	{
		ret[i] = tmp[i] ^ vi[i];
	}
	return tmp;
}

string iteration(string b[], int num) 
{
	string v;
	v = iv;
	for (int i = 0; i < num; i++)
	{
		v = cf(v, b[i]);
	}
	return v;
}

string SM3(string m) 
{
	m = fill(m);
	int num = 0, len = 0;
	len = m.length();
	num = len / 32;
	string ret;
	string  *b = new string[num];
	grouping(m, b);
	ret = iteration(b, num);
	delete[]b;
	return ret;
}

