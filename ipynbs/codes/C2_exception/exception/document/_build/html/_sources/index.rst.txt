.. exception documentation master file, created by
   sphinx-quickstart on Wed Jan 10 21:47:52 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to exception's documentation!
=====================================

异常处理
-----------

C语言没有正紧的异常,一般来说是通过自己规定的返回码来标识异常的.

一般来说,程序中的错误可以分为3种:

* 用户错误
    
    预期会发生的错误,用户的特定操作就会导致特定的错误,比如注册邮箱时填入的内容没有"@"这种.这种错误通常应该是程序主体就要处理的.通过返回特定的错误码进行提示.
    
    
* 运行时错误

    运行时错误从来都是非预期的,表明程序出了bug,程序必须要结束.通常需要使用断言来捕获这种错误
    
    
* 异常

    介于洪湖错误和程序bug之间,异常可能是比较罕见的错误,比如算数运算时候的溢出情况.通常异常不返回错误码,而是由恢复代码处理,类似于非局部的goto.
    

C语言中标准库提供了`setjmp`和`longjmp`是建立结构化异常处理的基础,具体来说,`setjmp`实例化一个处理程序,`longjmp`引发一个异常

try-except代码块
--------------------

try-except代码块在现代编程语言中几乎是标配,在实践中也被证明非常有用.然而C中没有.
"C语言接口与实现"一书中有一个使用宏的实现,接下来会写一个库专门用于实现try-except代码块.不过有几个受限的使用形式:

* `TRY-END_TRY`  

    相当于`{xxx}`语句不变只是引入一个新的作用域
    
* `TRY-ELSE-END_TRY`  

    相当于python中的`try-except Exception`,只要有异常就处理,不管是啥异常
    
* `TRY-EXCEPT-...-END_TRY` 
    
    相当于python中的`try-except E_1-except E_2-...`
    
    
* `TRY-EXCEPT-...-ELSE-END_TRY` 
    
    注意这与python中不同,相当于python中的`try-except E_1-...- except Exception`
    
    
* `TRY-EXCEPT-FINALLY-END_TRY`

    相当于python中的`try-except E_1-finally`无论是否有异常都会执行FINALLY,
    这个用法可以像python中的with语句一样提供一个上下文.
    
* `TRY-EXCEPT-ELSE-FINALLY-END_TRY`

    相当于python中的`try-except E_1-except Exception-finally`无论是否有异常都会执行FINALLY,
    这个用法可以像python中的with语句一样提供一个上下文.

* 如果`TRY`语句中需要执行返回,那么必须使用宏`RETURN`返回,

    不过需要注意如果代码段中有assert,且断言失败,那用RETURN回引起分段异常.


"C语言接口与实现"一书中提供的这组宏显然还是比较粗糙而且脆弱的,宏毕竟不会检查运行时错误,但一般用用足够.
C语言中并不提倡大量使用异常,设计算法的时候这组宏也应当被谨慎的使用.

这个模块在代码中有收录,它与下面的断言收录在一起.


断言
---------------------

C语言标准库有头文件`assert.h`将`assert(e)`定义为宏,`assert(e)`会计算表达式e的值,
如果e为0则向标准错误`stderr`写出诊断信息,并调用标准库函数abort退出程序执行.
诊断信息包含失败的断言文本和断言出现的坐标,该信息的格式是具体实现定义的.
`assert(0)`可以用户指明"不可能发生"的情况,当然如下也对:


.. code:: python

    assert(!"ptr==NULL -- can't happen")


"C语言接口与实现"一书中提供了一个标准库"assert.h"的替代,用它的好处是如果断言失败不会向stderr打印消息,
也不会跳出程序,也不会提供表达式e的文本,而是抛出上文中定义的`Assert_Failed`异常.


例子
----------

.. code:: python

    int _test_finally(int a){
        int result = 0;
        TRY
            if (a > 0){
                RAISE(USER_EXCEPTION);
            } else if (a == 0){
                assert(0);
            } else {
                result = 4;
            }
        EXCEPT(USER_EXCEPTION)
            result = 1;
        ELSE
            result = 2;
        FINALLY
            result = 3;
        END_TRY;
        return result;
    }



C API
==========

except
----------------

.. doxygenfile:: except.h


assert
--------------

.. doxygenfile:: assert.h


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
