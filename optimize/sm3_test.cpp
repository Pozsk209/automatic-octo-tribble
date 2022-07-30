#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "sm3.h"
#include <string>
#include <iostream>

// 这个是可用的
int main(int argc, char* argv[])
{
	LARGE_INTEGER cpuFreq;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	double runTime = 0.0;
	QueryPerformanceFrequency(&cpuFreq);

	// string转成unsigned char即可
	// 再封装一次
	std::string input_str;
	//std::cin >> input_str;
	input_str = "abc";
	int ilen = input_str.size();


	unsigned char* input = (unsigned char*)input_str.c_str();
	unsigned char output[32];

	printf("Message:\n");
	printf("%s\n", input);

	//------------------------------------
	QueryPerformanceCounter(&startTime);

	sm3(input, ilen, output);

	QueryPerformanceCounter(&endTime);
	runTime = (((endTime.QuadPart - startTime.QuadPart) * 1000.0f) / cpuFreq.QuadPart);
	//-------------------------------------
	printf("Hash:\n   ");
	for (int i = 0; i < 32; i++)
	{
		printf("%02x", output[i]);
		// 打空格
		if (((i + 1) % 4) == 0) printf(" ");
	}
	printf("\n");

	printf("time:%.4f ms\n", runTime);
	return 0;
}