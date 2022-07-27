#include <iostream>
#include "sm3.h"
using namespace std;

int main()
{
	int iv[8] = {
	0x7380166F, 0x4914B2B9, 0x172442D7, 0xDA8A0600, 0xA96F30BC, 0x163138AA, 0xE38DEE4D, 0xB0FB0E4E 
	};
	string input_str;
	cout << "输入附加信息：" << endl;
	cin >> input_str;

	unsigned long long m_len;
	string m_hash;
	int ilen = input_str.size();
	unsigned char* input = (unsigned char*)input_str.c_str();
	unsigned char output[32];
	cout << "输入长度：" << endl;
	cin >> m_len;
	cout << "输入HASH值：" << endl; 
	for (int i=0;i<8;i++) cin >>hex >> iv[i];
	for (int i = 0; i < 8; i++) cout<< hex << iv[i];
	cout << endl;
	sm3(input, ilen, output, iv);



	printf("Hash:\n   ");
	for (int i = 0; i < 32; i++)
	{
		printf("%02x", output[i]);
		// 打空格
		if (((i + 1) % 4) == 0) printf(" ");
	}
	printf("\n");
	return 0;
}