# TutorialFoCLang

本攻略是介绍C/C++,不是语法书而是特定使用场景下的入门参考手册.本文针对的对象是已经熟练使用python的数据科学工作人员.分为3个主要应用场景:

1. 使用C++编写数学模型
2. 封装用C++编写的模型为python可调用的模块
3. 在嵌入式设备上使用C++
4. 将模型落地为服务

因为这个场景的限制,本书会涉及的内容只是C和一小部分的c++.而且许多针对大型程序的方法论以及语法将在这边不适用.本书基本不涉及设计模式,基本不涉及一些数据结构与算法的实现,这本书不适合计算机专业的人看,因为老实说计算机专业的人和数据科学专业的人对于程序,算法的理解有很大不同.

不过作为一本入门书,当然还是会带有语法,只是这些语法会结合例子使用.

这本数的结构大致会和讲python那本一致,内容也偏向实用,希望的是一本书看完就可以在与本书相同的场景下使用C/C++.并且对整个C/C++的基本原理有个大致了解.

之所以此处混用C和C++是因为除非万不得已本人实在不想用C++写任何东西,因此C++只涉及到一点应用层的东西(比如将模型封装为grpc),没错,本文中C++就是个工具人.由于要两种语言混用,因此本文将忽视掉他们语法不兼容的部分,并以C++的为准(比如`auto`关键字).总结本书的编程风格,将符合如下特点:

+ C语言风格作为主干
+ C++一些语言作为补充增加表达能力
+ 针对跨平台编程,但不涉及嵌入式部分
+ 针对写简单应用和简单模块,不涉及复杂业务逻辑
+ 代码可维护性优先于性能
+ 尽量使用现代工具

## 本书的主要内容

本文的每一章都将以一个或多个完整例子开始,这章的内容就是逐步的完善这个例子.

由于C/C++有许多内容都是共用的很多时候C++被看作是C的扩展,因此本文不可避免的需要两种语言都介绍到.而语法方面由于C++一直都在更新语法,所以本文也需要与时俱进.但C++的标准和其编译器的实现经常是不同步的,因此对于版本就需要有取舍.本文写作的时间点是2021年8月,这个时候多数操作系统上提供的编译器都还无法完全支持c++17标准,但C++17标准实际上已经相当成熟了.本文以C++11作为基础介绍,C++23作为最高的支持版本.

主要内容主要包括:

1. 工具链
2. 数据模型
3. 基本语法
4. 宏与编译期操作
5. 实用库与对应用法
6. 多线程
7. 与python交互

## 符号标注说明

章节文章的目录标题上有时会有一些特殊符号,他们是有特殊含义的,包括:

+ 标题前的`[+]`标明是C++内容
+ 标题前的`[*]`标明是扩展内容,不感兴趣的可以跳过
+ 标题后的`[C++17]`,`[C11]`等标明支持的版本,一些编译器需要在编译时加上对应的特性标签才能编译通过因此有必要指出特性的版本出处