# 与Python交互

python原生提供了[C语言的API](https://docs.python.org/zh-cn/3/c-api/index.html),让它成了一门非常方便的胶水语言.很多程序都将它嵌入其中,将python作为内置脚本语言.

C程序中调用python一般被称为`嵌入Python(Embedding Python)`可以分为两种:

1. C中调用Python模块.这种模式下Python仅仅只是嵌入了C,仅需要在C中可以调用Python模块进行数据传递即可,并不会直接与应用程序进行交互,并不会C高性能,python适合快速开发,这适合那种需要性能但一些对性能要求没那么高的逻辑部分需要经常变更需求的场景.比如说我们希望有一个高性能接口包装一个接口固定的算法,但这个算法需要快速实现快速迭代,python写的业务逻辑是固定的嵌入在C程序中的.

2. 在C程序中嵌入python解释器,让python脚本可以从外部传入改变C程序中的对象.这通常在大型项目中才会用到,相当于让python作为程序的内置脚本,通常还需要提供几个内置对象供python处理.

这部分是相当麻烦的,因此是扩展内容.但对于打算[用Cython](https://blog.hszofficial.site/TutorialForCython/)的读者建议先看下有个概念.