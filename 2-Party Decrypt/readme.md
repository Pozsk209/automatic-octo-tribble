# 1 代码说明

用TCP SM2和SM3实现2Party Signature，TCP采用WinSock2，SM2和SM3纯手写。
主程序分为Client.cpp和Server.cpp 两个都需要生成可执行文件，启动时先启动Server。
编译时需要使用NTL库，编写的时候是Debug x86环境下的。

注意，TCP程序引用自https://blog.csdn.net/weixin_35882236/article/details/117060085，在此基础上实现通信

# 2 运行指导
- 创建工程，将文件夹下所有代码放入工程

- 配置NTL库

  现给出详细配置过程。
  
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

  打开Clinet.cpp和Server.cpp 发现两个main，需要分别生成可执行文件，然后向客户端输入SM2的密文，也就是c1,c2,c3。会有提示。就可以进行解密。具体演示看截图
  
# 3 运行截图

  ![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/dec.PNG)
  
  这里规定d1的逆和d2的逆的积为2，因此公钥就是G，私钥是1，明文是61626364，可以知道程序是成功解密

  
  
  
