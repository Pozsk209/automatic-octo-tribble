#include "SM2.h"
#include "SM3.h"
#include <iostream>
#include <winsock2.h> 
#include <Ws2tcpip.h>
#include <cstdlib>
#pragma comment(lib,"ws2_32.lib") 
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

using namespace std;

extern ZZ n, Gx, Gy, a, p;

int main()
{
    //初始化WSA 
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }
    //创建套接字 
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (slisten == INVALID_SOCKET)
    {
        printf("socket error !");
        return 0;
    }
    //绑定IP和端口 
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
    }
    //开始监听 
    if (listen(slisten, 5) == SOCKET_ERROR)
    {
        printf("listen error !");
        return 0;
    }
    //循环接收数据 
    SOCKET sClient;
    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);
    char revData[255];
    
    ZZ d2, inv_d2, Px1, Py1, Px2, Py2, Px, Py, Gx1, Gy1;
    ZZ e, k2, k3, Qx1, Qy1, Qx2, Qy2, Qx3, Qy3, x1, y1, r, s2, s3;
    while (true)
    {
        //cout << n << endl;
        printf("等待连接...\n");
        sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
        if (sClient == INVALID_SOCKET)
        {
            printf("accept error !");
            continue;
        }
        char ip[100];
        if (inet_ntop(AF_INET, &remoteAddr.sin_addr.S_un.S_addr, ip, 100) == NULL)
        {
            cout << "error" << endl;
            return -1;
        }
        cout << ip << endl; //宽字符记得用宽字符的方式进行输出
        //连接成功，接收数据 
        

        //接收P1x,P1y,长度66字节（32+2+32）
        int ret1 = recv(sClient, revData, 255, 0);
        if (ret1 > 0)
        {
            revData[ret1] = 0x00;
            //printf(revData);
        }
        
        string Ps(ret1, '0');
        for (int i = 0; i < ret1; i++) Ps[i] = revData[i];
        Px1 = bin_to_ZZ(hex_to_bin(Ps.substr(0, Ps.find("||", 0, 2))));
        Ps.erase(0, Ps.find("||", 0, 2) + 2);
        Py1 = bin_to_ZZ(hex_to_bin(Ps));

        d2 = RandomBnd(n - 2) + 1;
        inv_d2 = invmod(d2, n);
        mutipoint(Px1, Py1, inv_d2, a, p, Px2, Py2);
        mutipoint(Gx, Gy, n - 1, a, p, Gx1, Gy1);
        addition(Px2, Py2, Gx1, Gy1, Px, Py, a, p);
        cout << "Px:" << Px << endl << "Py:" << Py << endl;
        
        //-----------第1，2步完成---------

        int ret2 = recv(sClient, revData, 255, 0);
        if (ret2 > 0)
        {
            revData[ret2] = 0x00;
           // printf(revData);
        }
        string Qs(ret2, '0');
        for (int i = 0; i < ret2; i++) Qs[i] = revData[i];
        Qx1 = bin_to_ZZ(hex_to_bin(Qs.substr(0, Qs.find("||", 0, 2))));
        Qs.erase(0, Qs.find("||", 0, 2) + 2);
        Qy1 = bin_to_ZZ(hex_to_bin(Qs.substr(0, Qs.find("||", 0, 2))));
        Qs.erase(0, Qs.find("||", 0, 2) + 2);
        e = bin_to_ZZ(hex_to_bin(Qs));

        k2 = RandomBnd(n - 2) + 1;
        mutipoint(Gx, Gy, k2, a, p, Qx2, Qy2);
        k3 = RandomBnd(n - 2) + 1;
        mutipoint(Qx1, Qy1, k3, a, p, Qx3, Qy3);
        addition(Qx3, Qy3, Qx2, Qy2, x1, y1, a, p);
        string sendstr;
        r = (x1 + e) % n;
        s2 = (d2 * k3) % n;
        s3 = d2 * (r + k2) % n;

        sendstr = bin_to_hex(ZZ_to_bin(r)) + "||" + bin_to_hex(ZZ_to_bin(s2)) + "||" + bin_to_hex(ZZ_to_bin(s3));

        //发送数据 
        const char* sendData = sendstr.c_str();
        send(sClient, sendData, strlen(sendData), 0);
        system("pause");
        closesocket(sClient);
        break;
    }
    closesocket(slisten);
    WSACleanup();
    return 0;
}
