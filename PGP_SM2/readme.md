# 1 代码说明
用SM2,SM3,SM4算法实现PGP加密。
# 2 运行指导
- 创建工程，将文件夹下所有代码放入工程

- 配置NTL库

  现给出详细配置过程。因配置不当而产生的运行错误，一切后果必须由测试方负责。
  
  第一步，下载，选择11.5.1：
  [https://libntl.org/ ](https://libntl.org/download.html)
  
  第二步，右键项目--->属性--->C/C++--->常规--->附加包含目录，选择下载目录下 WinNTL11.5.1 下 include 文件夹的路径；
  
  第三步，参考下面的方法生成 NTL.lib 文件
  https://blog.csdn.net/qq_32112707/article/details/101267193
  
  第四步，右键项目--->属性-->选择链接器--->常规--->附加库目录，选择 NTL.lib 文件所在的路径。注意VS平台活动环境是否与NTL.lib生成环境相同（编写方测试使用Debug win32）；
  
  第五步，右键项目--->属性-->选择链接器--->输入--->附加依赖项，加上 NTL.lib，选择确定；
  
  第五步，右键项目--->属性--->配置属性--->C/C++--->常规--->SDL 检查，设置为否；

  第六步，保存。

- 调用

  加密解密请调用PGP.cpp文件下的PGP_enc函数和PGP_dec函数；签名使用PGP_sign函数，验证使用PGP_vrfy函数。
  
  对于PGP_enc：
  
  string PGP_enc(string plain, ZZ xB, ZZ yB); 
  
  输入：
  
  第一个参数为待加密信息，请输入128bit十六进制小写字符串，xB，yB为SM2公钥，长度各为256bit。 
  
  输出：
  
  PGP格式字符串。格式：c1||c3||c2||cipher。请注意，该字符串包含符号“||”。
  
  对于PGP_dec：
  
  string PGP_dec(string cipher, ZZ dB); 
  
  输入：
  
  第一个参数为待解密信息，请输入格式形如c1||c3||c2||cipher的十六进制小写字符串，dB为SM2私钥，长度为256bit。 
  
  输出：
  
  明文，格式为128bit小写十六进制字符串。
