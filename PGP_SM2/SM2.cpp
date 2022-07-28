#include <iostream>
#include <algorithm>
#include <cstring>
#include "SM3.h"
#include "NTL/ZZ.h"

using namespace std;
using namespace NTL;

ZZ a = to_ZZ("115792089210356248756420345214020892766250353991924191454421193933289684991996");
ZZ b = to_ZZ("18505919022281880113072981827955639221458448578012075254857346196103069175443");
ZZ p = to_ZZ("115792089210356248756420345214020892766250353991924191454421193933289684991999");
ZZ n = to_ZZ("115792089210356248756420345214020892766061623724957744567843809356293439045923");
ZZ Gx = to_ZZ("22963146547237050559479531362550074578802567295341616970375194840604139615431");
ZZ Gy = to_ZZ("85132369209828568825618990617112496413088388631904505083283536607588877201568");
//ZZ dB;
//ZZ xB, yB;


string ZZ_to_bin(ZZ x)
{
    string s;
    long len;
    len = NumBits(x);
    for (int i = 0; i < len; i++) 
    {
        if (bit(x, i)) s.append("1");
        else s.append("0");
    }
    reverse(s.begin(), s.end());
    return s;
}

ZZ bin_to_ZZ(string x)
{
    ZZ ret;
    long lent = x.length();
    ret = RandomLen_ZZ(lent);
    for (int i = 0; i < lent; i++)
    {
        SetBit(ret, lent - 1 - i);
        if (x[i] == '0') SwitchBit(ret, lent - 1 - i);
    }
    return ret;
}

string bin_to_hex(string x)
{
    reverse(x.begin(), x.end());
    string ret, tmps;
    long len = x.length();
    tmps = x;
    if (len %4 !=0) tmps += string(4 - len % 4, '0');
    len = tmps.length();
    char c[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
    int tmp = 0, tot = 0;
    for (int i = 0; i < len; i+=4)
    {
        tot = ((int)tmps[i] & 1) + (((int)tmps[i + 1] & 1) << 1) + (((int)tmps[i + 2] & 1) << 2) + (((int)tmps[i + 3] & 1) << 3);
        ret+=c[tot];
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

string hex_to_bin(string x)
{
    reverse(x.begin(), x.end());
    string ret;
    long len = x.length();
    for (int i = 0; i < len; i++)
    {
        string tmps(4,'0');
        int tmp = 0, base = 8;
        if (x[i] >= '0' && x[i] <= '9') tmp = x[i] - '0';
        else tmp = x[i] - 'a' + 10;
        for (int j = 3; j >= 0; j--)
        {
            if (tmp >= base)
            {
                tmps[j] = '1';
                tmp -= base;
            }
            base >>= 1;
        }
        ret += tmps;
    }
    while (ret[ret.length()-1] == '0') ret.erase(ret.length() - 1, 1);
    reverse(ret.begin(), ret.end());
    return ret;
}

ZZ invmod(ZZ a, ZZ n)
{
    ZZ x, b, d = to_ZZ("1");
    XGCD(d, x, b, n, a);
    return b;
}

void addition(ZZ x1, ZZ y1, ZZ x2, ZZ y2, ZZ& x3, ZZ& y3, ZZ a, ZZ p)
{
    //cout << "add:" << x1 << endl << x2 << endl;
    ZZ lambda;
    lambda = 0;
    if (x1 == x2 && y1 == p - y2)
    {
        cout << "Addition False" << endl;
        return;
    }
    else if (x1 == 0 || x2 == 0)
    {
        if (x1 == 0)
        {
            x3 = x2;
            y3 = y2;
            return;
        }
        if (x2 == 0)
        {
            x3 = x1;
            y3 = y1;
            return;
        }
    }
    else
    {
        if (x1 != x2)
        {
            lambda = ((y2 - y1) * invmod(x2 - x1, p)) % p;
        }
        else
        {
            lambda = (((3 * x1 * x1 + a) % p) * invmod(2 * y1, p)) % p;
        }
        x3 = (lambda * lambda - x1 - x2) % p;
        y3 = (lambda * (x1 - x3) - y1) % p;
    }
}
/*
void mutipoint(ZZ x, ZZ y, ZZ k, ZZ a, ZZ p, ZZ& qx, ZZ& qy)
{
    long len;
    len = NumBits(k);
    qx = x;
    qy = y;
    for (int i = len-1; i > 0 ; i--)//注意字符串遍历顺序
    {
        addition(qx, qy, qx, qy, qx, qy, a, p);
        if (bit(k, i) == 1)
        {
            addition(qx, qy, x, y, qx, qy, a, p);
        }
    }
}
*/

void mutipoint(ZZ x, ZZ y, ZZ k, ZZ a, ZZ p, ZZ& qx, ZZ& qy)
{
    long len;
    len = NumBits(k);
    //cout <<"k:" << k << endl;
    ZZ zx = to_ZZ("0");
    ZZ zy = to_ZZ("0");
    ZZ tx = x;
    ZZ ty = y;
    
    for (int i = 0; i < len ; i++)//注意字符串遍历顺序
    {
        if (bit(k, i) == 1)
        {
            addition(zx, zy, tx, ty, zx, zy, a, p);
        }
        addition(tx, ty, tx, ty, tx, ty, a, p);
        //cout <<i<<endl<< zx << endl << zy << endl;
    }
    qx = zx;
    qy = zy;
}

string kdf(string z, unsigned int klen)
{
    /*
    * z是十六进制字符串
    */
    unsigned int ct = 1, len;
    len = klen / 256 + 1;
    string k;
    for (int i = 0; i < len; i++)
    {
        k = k + SM3(bin_to_hex(z) + to_hex(ct, 8));
        ct += 1;
    }
    k = string(((256 - (hex_to_bin(k).length()) % 256)) % 256, '0') + hex_to_bin(k);
    k.erase(klen, k.length()-klen);
    return k;
}

void SM2_enc(string m, ZZ xB, ZZ yB, string& c1, string& c2, string& c3)
{
    /*
    * m为16进制字符串
    */
    unsigned long plen, klen;
    ZZ k, x1, y1, x2, y2;
    string t, sx1, sy1, sx2, sy2;
    plen = bin_to_hex(ZZ_to_bin(p)).length();
    m = string(4 - ((hex_to_bin(m).length() % 4)) % 4, '0') + hex_to_bin(m);
    klen = m.length();
    while (true)
    {
        k = RandomBnd(n - 1) + 1;
       // while (k == dB)
       // {
       //     k = RandomBnd(n - 1) + 1;
       // }
        ZZ zt;
        long lent;
        mutipoint(xB, yB, k, a, p, x2, y2);
        sx2 = ZZ_to_bin(x2);
        sy2 = ZZ_to_bin(y2);
        sx2 = string(256 - sx2.length(), '0') + sx2;
        sy2 = string(256 - sy2.length(), '0') + sy2;
        t = kdf(sx2 + sy2, klen);
        zt = bin_to_ZZ(t);
        if (zt != 0)
        {
            break;
        }
    }
    mutipoint(Gx, Gy, k, a, p, x1, y1);
    sx1 = string(plen - bin_to_hex(ZZ_to_bin(x1)).length(), '0') + bin_to_hex(ZZ_to_bin(x1));
    sy1 = string(plen - bin_to_hex(ZZ_to_bin(y1)).length(), '0') + bin_to_hex(ZZ_to_bin(y1));
    cout << sx1 << endl;
    c1 = "04" + sx1 + sy1;
    c2 = string((klen / 4) - bin_to_hex(ZZ_to_bin(bin_to_ZZ(m) ^ bin_to_ZZ(t))).length(), '0') + bin_to_hex(ZZ_to_bin(bin_to_ZZ(m) ^ bin_to_ZZ(t)));
    c3 = SM3(bin_to_hex(sx2 + m + sy2));
}

void SM2_dec(string c1, string c2, string c3, ZZ dB, string& output)
{
    long lenc1, klen;
    ZZ x1, y1, x2, y2;
    string sx2, sy2, t, m, u;
    c1.erase(0, 2);
    lenc1 = c1.length();
    x1 = bin_to_ZZ(hex_to_bin(c1.substr(0, lenc1 / 2)));
    y1 = bin_to_ZZ(hex_to_bin(c1.substr(lenc1 / 2, lenc1 - lenc1 / 2)));
    if (PowerMod(y1, 2, p) != (PowerMod(x1, 3, p) + a * x1 + b) % p) cout << "dec1 False" << endl;
    mutipoint(x1, y1, dB, a, p, x2, y2);
    sx2 = ZZ_to_bin(x2);
    sy2 = ZZ_to_bin(y2);
    sx2 = string(256 - sx2.length(), '0') + sx2;
    sy2 = string(256 - sy2.length(), '0') + sy2;
    klen = c2.length() * 4;
    t = kdf(sx2 + sy2, klen);
    if (bin_to_ZZ(t) == 0) cout << "dec2 False" << endl;
    m = string(klen - ZZ_to_bin(bin_to_ZZ(hex_to_bin(c2)) ^ bin_to_ZZ(t)).length(), '0') + ZZ_to_bin(bin_to_ZZ(hex_to_bin(c2)) ^ bin_to_ZZ(t));
    u = SM3(bin_to_hex(sx2 + m + sy2));
    if (u != c3) cout << "dec3 False" << endl;
    output = bin_to_hex(m);
}

void keyGen(ZZ& dB, ZZ& xB, ZZ& yB) 
{
    dB = RandomBnd(n - 2) + 1;//私钥dB
    mutipoint(Gx, Gy, dB, a, p, xB, yB);//公钥xB,yB
    //cout << dB << " " << xB << " " << yB << endl;
}

void SM2_sign(string m, ZZ xB, ZZ yB, ZZ dB, string& Za, string& sign_r, string& sign_s)
{
    //m为16进制字符串
    string E;
    ZZ k, x1, y1, r, s;
    Za = SM3(bin_to_hex(ZZ_to_bin(a)) + bin_to_hex(ZZ_to_bin(b)) + bin_to_hex(ZZ_to_bin(Gx)) + bin_to_hex(ZZ_to_bin(xB)) + bin_to_hex(ZZ_to_bin(yB)));
    E = SM3(Za + m);
    while (true)
    {
        k = RandomBnd(n - 2) + 1;
        mutipoint(Gx, Gy, k, a, p, x1, y1);
        r = (bin_to_ZZ(hex_to_bin(E)) % n + x1 % n) % n;
        if (r == 0 || r + k == n) continue;
        s = (invmod(1 + dB, n) * ((k - r * dB) % n)) % n;
        if (s == 0) continue;
        break;
    }
    sign_r = bin_to_hex(ZZ_to_bin(r));
    sign_s = bin_to_hex(ZZ_to_bin(s));
   // cout << "vrfy" << (r+s) %n << endl;
}

bool SM2_vrfy(string sign_r, string sign_s, string Za, string m, ZZ xB, ZZ yB) 
{
    ZZ r, s, t, xG, yG, xP, yP, x1, y1, R;
    string E;
    r = bin_to_ZZ(hex_to_bin(sign_r));//相同
    if (r < 1 || r > n - 1) return false;
    s = bin_to_ZZ(hex_to_bin(sign_s));//相同
    if (s < 1 || s > n - 1) return false;
    E = SM3(Za + m);//相同
    t = (r + s) % n;//相同
    if (t == 0) return false;
    mutipoint(Gx, Gy, s, a, p, xG, yG);
    mutipoint(xB, yB, t, a, p, xP, yP);
    addition(xG, yG, xP, yP, x1, y1, a, p);
    //mutipoint(Gx, Gy, s+t*dB, a, p, x1, y1);
    //cout <<"vrfy" << x1 << endl;
    R = (bin_to_ZZ(hex_to_bin(E)) + x1) % n;
    if (R == r) return true;
    else return false;
}