#cython: language_level=3
# distutils: language = c
# distutils: sources = binary_vector.c


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