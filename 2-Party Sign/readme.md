用TCP SM2和SM3实现2Party Signature，TCP采用WinSock2，SM2和SM3纯手写。
主程序分为Client.cpp和Server.cpp 两个都需要生成可执行文件，启动时先启动Server。
编译时需要使用NTL库，编写的时候是Debug x86环境下的。
