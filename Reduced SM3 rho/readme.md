# 1 代码说明

使用自己编写的SM3，并在此基础上修改为弱化版的SM3，目前可以实现32bit的碰撞。唯一的缺点就是改的SM3过于弱小，攻破速度非常快。

# 2 运行指导

创建工程，运行main.cpp下的main函数即可，无需进行其他操作。

# 3 代码截图

main函数代码，这也是实现rho method 方法，类似分解整数的pollard rho ,str2每次比str1多算一步hash，如果绕圈的话，str2必定会追上str1达到碰撞。

![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/ro1.PNG)

以下是攻击结果

![image](https://raw.githubusercontent.com/Pozsk209/automatic-octo-tribble/main/pic/ro2.PNG)
