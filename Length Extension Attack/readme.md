# 1 代码说明

SM3的长度扩展攻击的实现。使用的SM3为自己实现的，未使用第三方模块。在此基础上修改某些函数，以达到攻击效果。

# 2 运行指导

- 复制文件到工程中，运行main函数。

 随后会需要输入hash值和附加的字符串。
 
 举例：
 
 hashiv = 66c7f0f462eeedd9d1f2d46bdc10e4e24167c4875cf2f7a2297da02b8f4ba8e0(字符串“abc”的sm3hash，abc的十六进制字符串表示为616263)
 
 append = aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa616263 这里用a来填充，后方在加入append的字符串，616263，十六进制长度应该确保为len(data+padding+append)，这就要求随机填充长度为（data+padding）的数据。
 
 将append传入函数string SM3_attack(string m)中。该函数会对append进行自动分组计算SM3.分组计算的时候会调用iteration_attack函数，这是攻击的核心。
 
 string iteration_attack(string b[], int num) b为分组字符串，num为分组个数。
 
进行攻击的原理为：首先对append进行分组，共有num个分组，对num-1个分组进行sm3运算。当开始计算最后一个分组前，将sm3的向量修改为hashiv的值，由此实现攻击。

# 3 运行截图

该截图是调用代码


该截图是运行过程，第一行输入Hash值，第二行输入填充并附加的字符串（这里输入的字符串有填充就说明已经知道了salt的长度，所以不用输入salt长度

  
