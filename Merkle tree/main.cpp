#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#include <iostream>
#include <openssl/sha.h>
#include <cstring>
#include "SM3.h"

using namespace std;
#define MAXN 102400
struct Tree
{
	Tree* Lnode = nullptr;
	Tree* Rnode = nullptr;
	Tree* Fnode = nullptr;
	string MTH;
};

// Tree MT[MAXN];
Tree* MT = new Tree[262200];

string d[MAXN];

long long idx_data = 0, idx_path = 0, idx_proof = 0, idx_trans = 0;

long long mapath[MAXN];

long long trans[MAXN];

long long translate(long long idx)
{
	return 1;
}

string sha256(const string str)
{
	char buf[2];
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);
	std::string NewString = "";
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		//sprintf(buf, "%02x", hash[i]);
		NewString = NewString + buf;
	}
	return NewString;
}

string build(long long st, long long ed, long long md, long long idx)
{
	if (ed == st)
	{
		string s;
		s = d[ed];
		//cout << ed << ":" << d[ed] << endl;
		trans[md] = idx;
		//cout <<idx << ":" << md << endl;
		MT[idx].Lnode = nullptr;
		MT[idx].Rnode = nullptr;
		MT[idx].MTH = SM3(s);
		return MT[idx].MTH;
	}
	else
	{
		string s;
		s += build(st, md, (st + md) / 2, idx * 2 + 1) + build(md + 1, ed, (ed + md + 1) / 2, idx * 2 + 2);

		MT[idx].Lnode = &MT[idx * 2 + 1];
		MT[idx].Rnode = &MT[idx * 2 + 2];
		if (idx != 0) MT[idx].Fnode = &MT[(idx - 1) / 2];
		else MT[idx].Fnode = nullptr;
		
		MT[idx].MTH = SM3(s);
		
		//cout <<idx<<":" << MT[idx].MTH << endl;
		return MT[idx].MTH;
	}
}


void map(long long idx)//审计路径
{

	if (idx == 0)
	{
		printf("\n");
		idx_proof = idx_path;
		idx_path = 0;
		return;
	}
	else
	{
		long long parity = (idx % 2) * 2;//用于找到邻居节点
		//printf("%d ", idx - 1 + parity);
		mapath[idx_path] = idx - 1 + parity;
		idx_path++;
		map((idx - 1) / 2);
	}
	return;
}

bool mcproof(string value,long long idx , long long broidx[])
{
	string s_idx;
	s_idx = SM3(value);
	map(idx);
	for (int i = 0; i < idx_proof; i++)
	{
		if (idx % 2 == 0) s_idx = SM3(MT[broidx[i]].MTH + s_idx);
		else s_idx = SM3(s_idx + MT[broidx[i]].MTH);
		idx = (idx - 1) / 2;
	}
	if (MT[0].MTH == s_idx) return true;
	else return false;
}

int main()
{
	long long n;
	string s;
	cin >> n;
	for (int i = 0; i < n; i++) cin >> d[i];//每个叶子节点的值，顺序不论上下，一律从左到右
	
	s = build(0, n-1, (n-1) / 2, 0);
	for (int i = 0; i < n; i++) cout << MT[trans[i]].MTH << endl;//给出每个叶子的hash
	map(14);//输入的是标号
	cout << mcproof("14" ,14, mapath) << endl;//输入值
	delete[]MT;
	return 0;
}