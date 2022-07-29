#include <iostream>
#include "SM3.h"
using namespace std;

extern string iv;

string hashiv, append;

string iteration_attack(string b[], int num)
{
	string v;
	v = iv;
	for (int i = 0; i < num; i++)
	{
		if (i == num - 1) v = hashiv;
		cout << v << endl;
		v = cf(v, b[i]);
	}
	return v;
}

string SM3_attack(string m)
{
	m = fill(m);
	int num = 0, len = 0;
	len = m.length();
	num = len / 128;
	cout << m << endl;
	string ret;
	string* b = new string[num];
	grouping(m, b);
	ret = iteration_attack(b, num);
	delete[]b;
	return ret;
}

int main()
{
	/*
	* 举例：
	* hashiv = 66c7f0f462eeedd9d1f2d46bdc10e4e24167c4875cf2f7a2297da02b8f4ba8e0(字符串“abc”的sm3hash)
	* append = aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa616263
	* result = ACCF73FFC14F32535D8814377CF8CAD2238C905FBDB50528036F5BEF72052639
	*/
	cin >> hashiv;
	cin >> append;//输入的十六进制长度应该确保为len(data+padding+append)，这就要求随机填充长度为（data+padding）的数据。
	cout << SM3_attack(append) << endl;
	return 0;
}
