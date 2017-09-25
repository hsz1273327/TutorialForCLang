
# C++的基本形式

C++可以看做是C语言的一个威力加强版,因此C++向下兼容多数C语言的特性,如果把c++按c的写法来写,一个最简单的C++语言程序源文件如下:


```python
%%writefile src/c1/helloworldc.cpp

#include <stdio.h>

int main(void){
    printf("hello world");
    return 0;
}
```

    Overwriting src/c1/helloworldc.cpp


上面是名为`helloworld.cpp`的源文件,和python一样,C++语言需要有源文件,源文件就是人能看得懂的写给机器执行的文件
这个源文件有如下几个部分:

+ 引入头文件,c和c++中的头文件引入都是在标记位置直接复制文件中的内容,这会引起一些循环依赖之类的问题,这部分会在后面的<头文件与多文件编译>部分着重理解.
+ 入口函数main函数,函数会在后面详细讲解,这边只要先记住形式,入口函数有固定的写法.这边先记住是这样就好.
+ 入口函数在`{}`块包围中有用分号断句的语句这些语句就是函数要执行的步骤了,C语言是面向过程语言,说简单点就是写好编译好了程序就按顺序一步一步执行

## C++源程序的编译

和python不同,C++不是动态语言,必须静态的编译成可执行文件才能运行,python使用pvm解释生成字节码执行,而C++使用编译工具直接将源文件编译为机器可以执行的二进制文件.

由于平台不同,常见的C++编译器也不同,linux下最常见的是g++工具,mac下是clang,而window则是使用集成开发环境vs来编译,本文在mac下编写,所以使用的是clang,但g++和clang基本一样,所以linux下也可以按这个来.

编译上面的源文件


```python
!g++-7 -o bin/helloworldc src/c1/helloworldc.cpp
```


```python
!./bin/helloworldc
```

    hello world

一个更加典型的C++helloworld:


```python
%%writefile src/c1/helloworld.cpp:

#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

namespace helloworld {
    class Helloworld {
        private:
        string name;
        public:
        void print(){
                cout << "hello world " << (this->name) << "!" << endl;
        }
        Helloworld(){
                cout << "输入你的名字" << endl;
                std::cin >> (this->name);
        }
        Helloworld(string name){
                this->name = name;
        }
    };
}

int main(){
        namespace h=helloworld;
        h::Helloworld hello;
        hello.print();
        return 0;
}
```
```

    Overwriting src/c1/helloworld.cpp:



```python
!g++-7 -o bin/helloworldc src/c1/helloworld.cpp
```


```python
!./bin/helloworld
```

    hello world

mac下g++其实是连接到clang的, g++使用`-o`参数指出编译出来的文件放置地址.

## C++中的注释

不同于python,c++的行注释使用`//`,而段注释使用`/**/`

## 标准库引用

C/C++都使用`#include <...>`引入模块,但C的标准库使用`#include <xxx.h>`,而C++的标准库使用`#include <xxx>`


## using关键字和命名空间

命名空间是C中没有的概念,pythoner可以将其理解为python中的模块或者包的概念,他的用处就是区分不用来源的同名变量.有点类似一个定语,比如三班有小明四班也有小明,在各自班,老师喊小明都不会有歧义,但如果全校广播就会造成混乱.所以全校广播的话就会说是找三班的小明或者找四班的小明.命名空间就是类似几班的这种定语.

C++在引用机制上和C是一样的,都是将多份不同的文件中的源文件直接复制到一起,也就类似于全校广播了,那怎么办?使用命名空间就可以解决了.由于命名空间引入,C++对大型项目可以有更好的支持.


### 创建命名空间

正如例子中一样,我们使用

```C++
namespace <name> {
    xxx
}
```

来创建命名空间,这个空间内的所有变量常量函数类都将不能直接访问,而要借助命名空间才能访问

### 使用命名空间

使用命名空间又两种方式:

1. 直接使用
    
    上面例子中`std::cin`就是典型,其中`<namespace>::<object>`代表所属关系,可以和python中的这种引用模块的方式做类比:
    
    ```python
    import numpy
    
    numpy.add(1,2)
    ```
    
    `::`符号就类似python模块引用中的`.`,起到表明从属关系的作用.
    
2. 将命名空间中的对象引入当前空间

    使用`using <namespace>::<object>;`就可以将命名空间中的对象引入当前的变量空间,这就有点类似python中的`from numpy import add`,这样的坏处就是容易污染当前变量空间

    

### 为命名空间创建别名

依然是使用`namespace`关键字,使用如下表达式:`namespace <new_name>=<old_name>;`这样就可以使用`<new_name>`代替`<old_name>`了,不过要注意,这只是别名,并不是替换.

## 输入输出流

C++中使用流这个概念来描述输入输出,在标准库中有`iostream`来处理标准输入输出流,如例子中一样,
iostream包括4个io对象:

1. 输入流cin
2. 输出cout
3. log信息clog
4. 标准错误cerr

输出流需要使用`endl`作为结尾

具体的使用方法可以看上面例子代码,其中`<<`称为输出运算符,`>>`称为输入运算符

`iostream`与`stdio.h`相比,增加了可扩展性,因为是流,但同时效率相比`stdio.h`差了一些.

C中通用的输出函数包括:

+ `printf(char content[])` **带格式控制**的字符串输出
+ `fprintf(stdxxx,char content[])` 可以指定流的带格式控制字符串输出
+ `puts(char content[])` 只用来输出字符串，没有格式控制,**会自动换行**
+ `fputs(file,char content[])` 可以指定流的puts函数

与之对应的是C中通用的输入函数,包括:

+ `scanf('%d',&x)`将输入的内容传入某段内存地址,scanf函数需要指定能读入的字符数
+ `fscanf(file,'%d',&x)`将输入的内容传入某段内存地址,scanf函数需要指定能读入的字符数,需要指定流
+ `fgets(content, sizeof(content),stdin)` 一般配合sizeof使用,以避免缓冲区溢出的问题

## 字符串

C中只有字符,字符串则是字符数组,在C++中标准库`string`包含了针对string类型的一系列方法,虽然与python相比C++的字符串处理能力很弱,但已经比C好多了,他们包括:

1. `.c_str()`转化为C风格的字符串
2. `.length()`获取字符串长度
3. `+` 字符串相加就是拼接
4. `[<i>]`访问字符串中的某位的字符
5. `.insert(<pos>, <str_var>)`在指定位插入字符串
6. `.erase(<pos>[, <len>])` 在指定位删除一定长度的字符串内容
7. `.substr(<pos>, <len>) `pos 为要提取的子字符串的起始下标，len 为要提取的子字符串的长度。
8. `.find(<str>[, <pos>])`从pos位置开始向后找字符串是否匹配,str可以是std::string也可以是C的字符串,与之相似的是`.rfind(<str>[, <pos>])`不同之处就是查找从右向左
9. `.find_first_of(<str>)`,字符串str在被查字符串中第一次出现的位置
