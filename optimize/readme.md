# 1 代码说明

SM3使用的是课程现有的，在此基础上用AVX指令集和循环展开优化，在Debug x86环境下，时间由36ms加快到4ms到6ms左右，最快提升9倍

# 2 运行指导

创建工程，在sm3_test.cpp下运行main函数即可，如果想输入字符串，可以在sm3_test.cpp第21，22行左右修改input字符串，用cin输入。

# 3 程序截图

首先发现，sm3核心是sm3_start、sm3_process和sm3_finish 先对而这其中，sm3_process代码量最大，能改的地方最多。

以下是对T数组初始化，用AVX指令，由于是unsigned long类型，用epi32 赋值即可，一次性可以赋值8个元素。无需对齐。

![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/op1.PNG)

同样地，可以用AVX指令对W进行运算。但是AVX整型没有异或运算，因此为了匹配32位数字，只能使用_mm256_xor_ps命令，用单精度浮点数进行异或。

![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/op2.PNG)

另外展示循环展开，主要用于sm3的64轮轮函数。实验发现，循环展开为8时达到平衡点，如果一次展开超过8次循环，提升效率并不高。

![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/op3.PNG)

![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/op4.PNG)

最后是结果展示，input="abc";

![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/op5.PNG)
