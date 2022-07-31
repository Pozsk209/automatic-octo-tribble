#include <iostream>
#include "SM3.h"
using namespace std;

string Randstr(int len)
{
	string ret(len, '0');
	for (int i = 0; i < len; i++)
	{
		ret[i] = '0' + rand() % 10;
	}
	return ret;
}
int main()
{
	srand((unsigned int)time(0));
	string str1,str2, h1, h2;
	while (true)
	{
		str2 = "";
		str1 = Randstr(16);
		cout << str1 << endl;
		h1 = SM3(str1);
		for (int i = 0; i < h1.length(); i++)
		{
			if (h1[i] >= '1' && h1[i] <= '9') str2 = str2 + "3" + h1[i];
			else str2 = str2 + "6" + (char)(h1[i] - 'a' + 1);
		}
		h2 = SM3(str2);
		if (h1 == h2)
		{
			cout << "Åö×²³É¹¦" << endl;
			cout << str1 << endl << str2 << endl;
			break;
		}
	}
	return 0;
}