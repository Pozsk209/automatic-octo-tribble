#include <iostream>
#include <algorithm>
#include <cstring>
#include "SM2.h"
#include "NTL/ZZ.h"

using namespace std;
using namespace NTL;

int main()
{
    ZZ a = to_ZZ("115792089210356248756420345214020892766250353991924191454421193933289684991996");
    ZZ b = to_ZZ("18505919022281880113072981827955639221458448578012075254857346196103069175443");
    ZZ p = to_ZZ("115792089210356248756420345214020892766250353991924191454421193933289684991999");
    ZZ n = to_ZZ("115792089210356248756420345214020892766061623724957744567843809356293439045923");
    ZZ Gx = to_ZZ("22963146547237050559479531362550074578802567295341616970375194840604139615431");
    ZZ Gy = to_ZZ("85132369209828568825618990617112496413088388631904505083283536607588877201568");

    ZZ dB;//私钥dB
    ZZ xB, yB;//公钥xB,yB
    //-------公钥私钥生成--------------
    dB = RandomBnd(n - 2) + 1;
    mutipoint(Gx, Gy, dB, a, p, xB, yB);
    string input, output;
    string c1, c2, c3;
    cin >> input;
    encrypt(input, xB, yB, a, b, p, c1, c2, c3);
    cout << c1 << endl << c2 << endl << c3 << endl;
    decrypt(c1, c2, c3, a, b, p, dB, output);
    cout << output << endl;
    return 0;
}