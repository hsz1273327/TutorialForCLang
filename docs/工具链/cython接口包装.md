
# python接口包装

我们一般用C的一个主要目的就是为python提供扩展,在python攻略中已经介绍了使用cython为python写扩展的方式,但这种方式一般是以python为主体的,而如果以C为主体,那么通常就和这种的方式有所区别,具体在:使用cython的包装模式,通过`pyd`文件声明接口,再使用`pyx`模块将项目包装为面向对象的python模块.这种方式

## 使用cython包装模式

为什么使用这种方式呢?主要是目前来看cython的包装模式是几个主流python实现都支持的方式,而且相对简单.接着我们还以binary vector为例进行包装

+ `./code/cython_wrapper/python/pybinaryvector.pyx`
    ```python
    #cython: language_level=3
    # distutils: language = c

    cdef extern from "binary_vector.h":
        struct BINARY_VECTOR:
            float x
            float y
            
        ctypedef BINARY_VECTOR *BINARY_VECTOR_P
        
        BINARY_VECTOR_P VEC_init(float,float)
        void VEC_del(BINARY_VECTOR_P)
        
        float VEC_mod(BINARY_VECTOR_P)
        BINARY_VECTOR_P VEC_add(BINARY_VECTOR_P,BINARY_VECTOR_P)
        float VEC_mul(BINARY_VECTOR_P,BINARY_VECTOR_P)

    cdef class BinaryVector:
        cdef BINARY_VECTOR_P _vec
        
        def __cinit__(self,float x,float y):
            self._vec = VEC_init(x,y)
            if self._vec is NULL:
                raise MemoryError
            
        def __dealloc__(self):
            if self._vec is not NULL:
                VEC_del(self._vec)
                
        property x:
            def __get__(self):
                return self._vec.x
            def __set__(self,new_x):
                self._vec.x = new_x
        property y:
            def __get__(self):
                return self._vec.y
            def __set__(self,new_y):
                self._vec.y = new_y
        cpdef mul(self,BinaryVector that):
            return VEC_mul(self._vec,that._vec)
                
        def __mul__(self,BinaryVector that):
            return self.mul(that)
            
        def add(self,BinaryVector that):
            cdef BINARY_VECTOR_P ptr
            ptr = VEC_add(self._vec,that._vec)
            result = BinaryVector(ptr.x,ptr.y)
            if ptr is not NULL:
                VEC_del(ptr)
            return result
        def __add__(self,BinaryVector that):
            return self.add(that)
        def mod(self):
            return VEC_mod(self._vec)
    ```

+ `./code/cython_wrapper/python/setup.py`

    ```python
    %%writefile 
    from distutils.core import setup
    from pathlib import Path
    from distutils.extension import Extension
    from Cython.Build import cythonize
    from Cython.Distutils import build_ext

    self_path = Path(__file__).resolve()
    self_dir = self_path.parent
    project_dir = self_dir.parent
    lib_dir = project_dir.joinpath("build/lib")
    inc_dir = project_dir.joinpath("build/inc")
    source_dir = project_dir.joinpath("python")

    extension = Extension(
        "pybinaryvector",
        sources=[str(source_dir.joinpath("pybinaryvector.pyx"))],
        include_dirs=[str(inc_dir)],
        libraries = ["vector"],
        library_dirs = [str(lib_dir)]
    )

    setup(
        cmdclass={'build_ext': build_ext},
        ext_modules=cythonize(extension),
    )
    ```

在项目根目录下先用cmake编译静态库,之后再进入python文件夹使用`python setup.py build_ext --inplace`编译动态库让python可以调用


```bash
python ./code/cython_wrapper/python/setup.py build_ext --inplace
```

    running build_ext
    building 'pybinaryvector' extension
    gcc -Wno-unused-result -Wsign-compare -Wunreachable-code -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -I/Users/huangsizhe/Lib/conda/anaconda3/include -arch x86_64 -I/Users/huangsizhe/Lib/conda/anaconda3/include -arch x86_64 -I/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/cython_wrapper/build/inc -I/Users/huangsizhe/Lib/conda/anaconda3/include/python3.7m -c /Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/cython_wrapper/python/pybinaryvector.c -o build/temp.macosx-10.7-x86_64-3.7/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/cython_wrapper/python/pybinaryvector.o
    gcc -bundle -undefined dynamic_lookup -L/Users/huangsizhe/Lib/conda/anaconda3/lib -arch x86_64 -L/Users/huangsizhe/Lib/conda/anaconda3/lib -arch x86_64 -arch x86_64 build/temp.macosx-10.7-x86_64-3.7/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/cython_wrapper/python/pybinaryvector.o -L/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/cython_wrapper/build/lib -lvector -o /Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/pybinaryvector.cpython-37m-darwin.so

