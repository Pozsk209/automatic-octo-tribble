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
    
    ZZ d2, inv_d2, T1, T2;
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
        
        string T1s(ret1, '0');
        ZZ dx1, dy1, T2x1, T2y1;
        for (int i = 0; i < ret1; i++) T1s[i] = revData[i];
        dx1 = bin_to_ZZ(hex_to_bin(T1s.substr(0, T1s.find("||", 0, 2))));
        T1s.erase(0, T1s.find("||", 0, 2) + 2);
        dy1 = bin_to_ZZ(hex_to_bin(T1s));

        d2 = RandomBnd(n - 2) + 1;
        inv_d2 = invmod(d2, n);

        T2x1 = (inv_d2 * dx1) % n;
        T2y1 = (inv_d2 * dy1) % n;

        string sendstr;

        sendstr = bin_to_hex(ZZ_to_bin(T2x1)) + "||" + bin_to_hex(ZZ_to_bin(T2y1));

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
