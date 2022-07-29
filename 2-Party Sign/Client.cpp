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

    ZZ d1, inv_d1, Px1, Py1, k1, Qx1, Qy1, s, s2, s3, r;
    string Z, M, e;
    M = "0123456789abcdeffedcba9876543210";
    Z = "abcdef";//双方标识符，此处必须用十六进制字符串表示

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

        d1 = RandomBnd(n - 2) + 1;
        inv_d1 = invmod(d1, n);
        mutipoint(Gx, Gy, inv_d1, a, p, Px1, Py1);
        
        string sendstr1 = bin_to_hex(ZZ_to_bin(Px1)) + "||" + bin_to_hex(ZZ_to_bin(Py1));

        //char data[1024];
        //cin >> data;
        
        const char* sendData1;
        sendData1 = sendstr1.c_str();   //string转const char*
        send(sclient, sendData1, strlen(sendData1), 0);

        e = SM3(Z + M);

        k1 = RandomBnd(n - 2) + 1;
        mutipoint(Gx, Gy, k1, a, p, Qx1, Qy1);
        string sendstr2 = bin_to_hex(ZZ_to_bin(Qx1)) + "||" + bin_to_hex(ZZ_to_bin(Qy1)) + "||" + e;

        const char* sendData2;
        sendData2 = sendstr2.c_str();   //string转const char*
        send(sclient, sendData2, strlen(sendData2), 0);

        char recData[255];
        int ret = recv(sclient, recData, 255, 0);
        if (ret > 0) {
            recData[ret] = 0x00;
            //printf(recData);
        }
        string rssstr(ret, '0');
        for (int i = 0; i < ret; i++) rssstr[i] = recData[i];
        r = bin_to_ZZ(hex_to_bin(rssstr.substr(0, rssstr.find("||", 0, 2))));
        rssstr.erase(0, rssstr.find("||", 0, 2) + 2);
        s2 = bin_to_ZZ(hex_to_bin(rssstr.substr(0, rssstr.find("||", 0, 2))));
        rssstr.erase(0, rssstr.find("||", 0, 2) + 2);
        s3 = bin_to_ZZ(hex_to_bin(rssstr));
        s = ((d1 * k1) * s2 + d1 * s3 - r) % n;
        if (s != 0 || s != n - r)
        {
            cout << "s:" << bin_to_hex(ZZ_to_bin(s)) << endl
                << "r:" << bin_to_hex(ZZ_to_bin(s)) << endl;
        }
        else
            cout << "error" << endl;
        system("pause");
        closesocket(sclient);
        break;
    }
    WSACleanup();
    return 0;
}
