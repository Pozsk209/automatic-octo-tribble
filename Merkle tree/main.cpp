#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#include <iostream>
#include <openssl/sha.h>
#include <cstring>
using namespace std;
#define MAXN 1024
struct Tree
{
	Tree* Lnode = nullptr;
	Tree* Rnode = nullptr;
	Tree* Fnode = nullptr;
	string MTH;
};

Tree MT[MAXN];
string d[MAXN];
int idx_data = 0, idx_path = 0, idx_proof = 0, idx_trans = 0;

int mapath[MAXN];

int trans[MAXN];

int translate(int idx)
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

string build(int st,int ed, int md, int idx)
{
	if (ed == st)
	{
		string s;
		s.push_back(char(0x00));
		s += d[idx_data];
		trans[md] = idx;
		//cout <<idx << ":" << md << endl;
		MT[idx].Lnode = nullptr;
		MT[idx].Rnode = nullptr;
		MT[idx].MTH = sha256(s);
		return MT[idx].MTH;
	}
	else
	{
		string s;
		s.push_back(char(0x01));
		s += build(st, md, (st + md) / 2, idx * 2 + 1) + build(md + 1, ed, (ed + md + 1) / 2, idx * 2 + 2);

		MT[idx].Lnode = &MT[idx * 2 + 1];
		MT[idx].Rnode = &MT[idx * 2 + 2];
		if (idx != 0) MT[idx].Fnode = &MT[(idx - 1) / 2];
		else MT[idx].Fnode = nullptr;
		
		MT[idx].MTH = sha256(s);
		
		//cout <<idx<<":" << MT[idx].MTH << endl;
		return MT[idx].MTH;
	}
}


void map(int idx)//审计路径
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
		int parity = (idx % 2) * 2;//用于找到邻居节点
		printf("%d ", idx - 1 + parity);
		mapath[idx_path] = idx - 1 + parity;
		idx_path++;
		map((idx - 1) / 2);
	}
	return;
}

bool mcproof(int idx)
{
	string s_idx;
	s_idx = MT[idx].MTH;
	map(idx);
	for (int i = 0; i < idx_proof; i++)
	{
		if (idx % 2 == 0) s_idx = sha256(MT[i].MTH + s_idx);
		else s_idx = sha256(s_idx + MT[i].MTH);
		idx = (idx - 1) / 2;
	}
	if (MT[0].MTH == s_idx) return true;
	else return false;
}

int main()
{
	
	int n;
	string s;
	cin >> n;
	//for (int i = 0; i < n; i++) cin >> d[i];
	
	s = build(0, n-1, (n-1) / 2, 0);
	for (int i = 0; i < n; i++) cout << trans[i] << endl;
	map(8);
	mcproof(8);
	return 0;
}