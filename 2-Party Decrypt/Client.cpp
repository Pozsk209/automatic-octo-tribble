#include "SM2.h"
#include "SM3.h"
#include <iostream>
#include<WinSock2.h>
#include<cstdio>
#include<cstring>
#include <Ws2tcpip.h>
#include <cstdlib>
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

using namespace std;

extern ZZ n, Gx, Gy, a, p;

int main()
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0)
    {
        return 0;
    }

    ZZ d1, inv_d1, Tx1, Ty1, C1, T2, zx1, zy1;

    while (true) 
    {
        SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sclient == INVALID_SOCKET)
        {
            printf("invalid socket!");
            return 0;
        }
        sockaddr_in serAddr;
        serAddr.sin_family = AF_INET;
        serAddr.sin_port = htons(8888);
        inet_pton(AF_INET, "127.0.0.1", &serAddr.sin_addr.S_un.S_addr);
        if (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
        {  //连接失败
            printf("connect error !");
            closesocket(sclient);
            return 0;
        }
        //连接成功，开始生成并发送
        cout << "连接成功";

        string c1, c2, c3;
        cout << "请输入c1:" << endl;
        cin >> c1;
        cout << "请输入c2:" << endl;
        cin >> c2;
        cout << "请输入c3:" << endl;
        cin >> c3;
        c1.erase(0, 2);
        long lenc1;
        lenc1 = c1.length();
        zx1 = bin_to_ZZ(hex_to_bin(c1.substr(0, lenc1 / 2)));//x1 = d1inv*d2inv*x1
        zy1 = bin_to_ZZ(hex_to_bin(c1.substr(lenc1 / 2, lenc1 - lenc1 / 2)));//y1 = d1inv*d2inv*y1
        d1 = RandomBnd(n - 2) + 1;
        inv_d1 = invmod(d1, n);
        inv_d1 = to_ZZ("1");
        Tx1 = (inv_d1 * zx1) % p;
        Ty1 = (inv_d1 * zy1) % p;
        string sendstr1 = bin_to_hex(ZZ_to_bin(Tx1)) + "||" + bin_to_hex(ZZ_to_bin(Ty1));

        const char* sendData1;
        sendData1 = sendstr1.c_str();   //string转const char*
        send(sclient, sendData1, strlen(sendData1), 0);

        char recData[255];
        int ret = recv(sclient, recData, 255, 0);
        if (ret > 0) {
            recData[ret] = 0x00;
            //printf(recData);
        }
        string T2str(ret, '0');
        for (int i = 0; i < ret; i++) T2str[i] = recData[i];

        string output;
        long  klen;
        ZZ x1, y1, x2, y2;
        string sx2, sy2, t, m, u;

        x1 = bin_to_ZZ(hex_to_bin(T2str.substr(0, T2str.find("||", 0, 2))));//x1 = d1inv*d2inv*x1
        T2str.erase(0, T2str.find("||", 0, 2) + 2);
        y1 = bin_to_ZZ(hex_to_bin(T2str));//y1 = d1inv*d2inv*y1
        //cout << x1 << endl << y1 << endl;
        x2 = (x1 - zx1) % p;
        y2 = (y1 - zy1) % p;
        //cout << x2 << endl << y2 << endl;

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
        cout << output << endl;
        system("pause");
        
        closesocket(sclient);
        break;
    }
    WSACleanup();
    return 0;
}
