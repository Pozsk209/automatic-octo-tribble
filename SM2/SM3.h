#pragma once
#include <iostream>
#include <cstring>
#include <string>
using namespace std;

string to_hex(unsigned int x, unsigned int length);

unsigned long to_dec(string s);

unsigned long t(int j);

unsigned long csl(unsigned long x, int k);

unsigned long ff(unsigned long x, unsigned long y, unsigned long z, int j);

unsigned long gg(unsigned long x, unsigned long y, unsigned long z, int j);

unsigned long p0(unsigned long x);

unsigned long p1(unsigned long x);

string fill(string m);

void grouping(string m, string b[]);

void extend(string bi, unsigned long w[]);

string cf(string vi, string bi);

string iteration(string b[], int num);

string sm3(string m);