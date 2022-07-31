# 1 代码说明

使用自己编写的SM3，并在此基础上修改为弱化版的SM3，可以实现32bit碰撞，但是碰撞难度取决于SM3的弱化程度，因此这也表明SM3弱化太严重。

# 2 运行指导

需要用这三个文件创建工程，运行main.cpp下的main函数即可。

# 3 代码截图

首先，我们是为了寻找碰撞，因此我们设置两个str——str1和str2，两个字符串随机生成并检测两者hash，如果碰撞成功，则输出结果。

生日攻击的代码：

![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/Bth1.PNG)

运行结果

![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/Bth2.PNG)


