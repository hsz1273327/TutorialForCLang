/** 
* @file                              array.h
* @brief                             动态数组
* @detail 用于异常处理,这是一个简单的实现,
* @par  依赖        assert.h
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
* @par 添加注释                       hsz
*/
/* $Id$ */
#ifndef ARRAY_INCLUDED
#define ARRAY_INCLUDED

/** 
* @typedef         struct Array_L *Array_T
* @brief                             数组结构的指针
* @author                            David R. Hanson
* @date                              2018-1-9
* @version                           0.0.1
* @par LICENSE                       MIT
*/
typedef struct Array_L* Array_T;

/** 
* @fn       Array_T    Array_new (int length, int size)
* @param [length]          int      数组长度
* @param [size]            int      数组中元素占用空间大小
* @return                  Array_T  动态数组的首元素指针
* @brief     创建一个动态数组
* @detail    新建一个动态数组,内容都初始化为0
*/
extern Array_T Array_new(int length, int size);

/** 
* @fn        void Array_free(Array_T *array)
* @param [array]     Array_T *      动态数组的首地址   
* @brief     将动态数组释放空间
*/
extern void Array_free(Array_T* array);

/** 
* @fn       int Array_length(Array_T array)
* @param [array]      Array_T       动态数组对象 
* @return             int           动态数组的长度
* @brief     求动态数组长度
*/
extern int Array_length(Array_T array);

/** 
* @fn        int Array_size(Array_T array)
* @param [array]      Array_T       动态数组对象 
* @return             int           元素占用字节数
* @brief     求动态数组元素大小
*/
extern int Array_size(Array_T array);

/** 
* @fn        void* Array_get(Array_T array, int i)
* @param [array]      Array_T       动态数组对象 
* @param [i]          int           偏移量 
* @return             void *        返回指定位置的元素内容
* @brief     取出i位置的元素,取出来的对象需要进行强制类型转换
*/
extern void* Array_get(Array_T array, int i);

/** 
* @fn        void* Array_put(Array_T array, int i, void* elem)
* @param [array]      Array_T       动态数组对象 
* @param [i]          int           要替换元素的偏移量 
* @param [elem]       void *        要替换的元素
* @return             void *        返回替换后的元素内容 
* @brief     将异常压入栈
*/
extern void* Array_put(Array_T array, int i, void* elem);

/** 
* @fn        void Array_resize(Array_T array, int length)
* @param [array]               Array_T          动态数组对象   
* @param [length]              int              数组长度   
* @brief     将异常压入栈
*/
extern void Array_resize(Array_T array, int length);

/** 
* @fn        Array_T Array_copy(Array_T array, int length)
* @param [array]               Array_T          动态数组对象   
* @param [length]              int              数组长度
* @return                      Array_T          新动态数组的首地址
* @brief     复制动态数组
*/
extern Array_T Array_copy(Array_T array, int length);

/** 
* @fn        void ArrayRep_init(Array_T array, int length, int size, void *ary)
* @param [array]               Array_T          动态数组对象    
* @param [length]              int              数组长度   
* @param [size]                int              元素占用字节数  
* @param [ary]                 void *           替换后的元素   
* @brief     
*/
extern void ArrayRep_init(Array_T array, int length, int size, void* ary);

/** 
* @def                                ARRAY(length,type)
* @param [length]     int            异常帧指针
* @param [type]                      动态数组中元素类型
* @return            Array_T         动态数组的首元素指针
* @brief
* @detial                               
*/
#define ARRAY(length, type) \
    Array_new(length, sizeof(type))

/** 
* @def                                ARRAY(length,type)
* @param [length]     int            异常帧指针
* @param [type]                      动态数组中元素类型
* @brief
* @detial                               
*/
#define ARRAYREP(array, length, type, ary) \
    ArrayRep_init(array, length, sizeof(type), ary)

#endif