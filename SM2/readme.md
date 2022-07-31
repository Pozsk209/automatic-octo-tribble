# 1 代码说明

该SM2算法从零开始构建，使用C++语言，除使用NTL大数运算库外，未使用其他模块如Openssl，GMssl等。可以实现SM2加密解密，SM2签名，SM2验证。

没有点压缩！！！

# 2 运行指导

- 创建工程，将文件夹下所有代码放入工程

- 配置NTL库

  现给出详细配置过程。因配置不当而产生的运行错误，一切后果必须由测试方负责。
  
  第一步，下载，选择11.5.1：
  [https://libntl.org/ ](https://libntl.org/download.html)
  
  第二步，右键项目--->属性--->C/C++--->常规--->附加包含目录，选择下载目录下 WinNTL11.5.1 下 include 文件夹的路径；
  ![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/p1.png)
  
  第三步，参考下面的方法生成 NTL.lib 文件
  https://blog.csdn.net/qq_32112707/article/details/101267193
  
  第四步，右键项目--->属性-->选择链接器--->常规--->附加库目录，选择 NTL.lib 文件所在的路径。注意VS平台活动环境是否与NTL.lib生成环境相同（编写方测试使用Debug win32）；
  
  ![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/p2.png)

  
  第五步，右键项目--->属性-->选择链接器--->输入--->附加依赖项，加上 NTL.lib，选择确定；
  
  ![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/p3.png)
  
  第五步，右键项目--->属性--->配置属性--->C/C++--->常规--->SDL 检查，设置为否；
    
  ![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/p4.png)


  第六步，保存。

- 调用

    请调用SM2.cpp文件下的SM2_enc函数和SM2_dec函数
  
    对于SM2_enc函数
    
    void SM2_enc(string m, ZZ xB, ZZ yB, string& c1, string& c2, string& c3);
    
    参数：m为加密明文，十六进制小写字符串， xB,yB为公钥，c1,c2,c3为密文，请在调用前设置好，程序会将密文写入其中。
    
    对于SM2_dec函数
    
    void SM2_dec(string c1, string c2, string c3, ZZ dB, string& output);
    
    参数：c1,c2,c3为密文，十六进制小写字符串，dB为私钥，output为输出明文。
  
    具体使用方法见下方程序截图
   
# 3 程序截图

     首先我们看调用函数的main函数，图是旧版，按照旧版解释，新版的请看上方，参数有所变化
     
       ![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/Sm21.PNG)

    可以看到，为了使用SM2 ，需要提前设置NTL库。
    
    首先生成私钥和公钥，调用RandomBnd函数生成（1，n-1）间的随机数得到私钥dB，并使用dB用点乘函数mutipoint计算公钥xB，yB。在新版的SM2中，两函数用Keygen（dB，xB，yB）函数代替。
    
    然后使用加密函数，旧版加密叫encrypt，对应新版SM2_enc 旧版 encrypt(input, xB, yB, a, b, p, c1, c2, c3)需要传这些参数，新版可以不传a,b,p; 秘密在c1c2c3中。
    
    解密函数，旧版叫decrypt，对应新版SM2_dec,旧版    decrypt(c1, c2, c3, a, b, p, dB, output), 新版无需传a,b,p;
    
    输入十六进制明文616263，对应zfc"abc",得到密文和解密明文
    
 ![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/Sm22.PNG)

