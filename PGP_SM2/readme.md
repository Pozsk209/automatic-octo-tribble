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
  
  第一个参数为待加密信息，请输入128bit十六进制小写字符串，xB，yB为SM2公钥，格式各为256bit十六进制小写字符串。 
  
  输出：
  
  PGP格式字符串。格式：c1||c3||c2||cipher。请注意，该字符串包含符号“||”，符号不为十六进制。
  
  对于PGP_dec：
  
  string PGP_dec(string cipher, ZZ dB); 
  
  输入：
  
  第一个参数为待解密信息，请输入格式形如c1||c3||c2||cipher的十六进制小写字符串，前三者为SM2密文，第四者为SM4待解密信息。dB为SM2私钥，格式为256bit十六进制小写字符串。 
  
  输出：
  
  明文，格式为128bit小写十六进制字符串。

  对于PGP_sign：
  
  string PGP_sign(string message, string password, string SM4cipher);
  
  输入：
  
  第一个参数为待签名信息，请输入十六进制小写字符串，长度不固定；password为口令，无特殊要求；SM4cipher为SM2密钥，格式：私钥||公钥x点||公钥y点，注意，字符串含有符号||，除符号||外，其余为十六进制小写，没有点压缩。 
  
  输出：
  
  SM2格式签名，格式：r||s||Za||message，前三者为SM2签名输出，第四为明文。
  
  对于 PGP_vrfy：
  
string PGP_vrfy(string cipher, ZZ xB, ZZ yB);
  
  输入：
  
  第一个参数为签名字符串，格式：r||s||Za||message，前三者为SM2签名输出，第四为明文，xB,yB为公钥x，y点。 
  
  输出：
  
  字符串，如果验证成功，会输出字符串，否则输出"Verification Failed"。

# 3 运行截图
