
# 字符串

C语言中的字符串与Python3中的字符串并不是一回事这需要注意.Python3中的字符串准确的说应该叫`Unicode`,而C语言中的其实更类似Python2中的str,也就是以ASCII编码的字节串,因此不具备读写非英语的能力;在C中要使用`utf-8`.后文中我们以`字符串`代表C中的字符串.

字符串是特殊的数组,可以理解为他是由字符为元素组成的数组,处理方式和数组其实一样.定义字符串一般有这样几种方式:

```c
char str1[] = "asdfg";

char str2[10] = "asdfg";

char str3[10];
strcpy(str3,"abcd");
```

需要注意的是字符串一旦定义就只能一个元素一个元素的修改内容了,这样很麻烦,因此一般赋值会用`string.h`中的`strcpy`函数

## UTF-8,UTF-16,UTF-32支持

在C99标准中,我们可以在字符串中使用`utf-8`字符,用法就是在字符串前加上`u8`或在字符串中使用utf-8编码的字符串


```c
#include <stdio.h>

int main(void)
{
    char s1[] = "a猫?"; // 或 "a\u732B\U0001F34C"
    char s2[] = u8"a猫?";

    printf("%s\n",s1);
    printf("%s\n",s2);
}
```

    a猫?
    a猫?



```c

```
