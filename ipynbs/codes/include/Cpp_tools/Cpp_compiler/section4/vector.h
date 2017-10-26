#ifndef BINARY_VECTOR_H
#define BINARY_VECTOR_H//一般是文件名的大写 头文件结尾写上一行


#ifdef __cplusplus
extern "C" {
#endif
struct _Vector__BinaryVector;//将BinaryVector对象包装到一个结构体中

typedef struct _Vector__BinaryVector Vector__BinaryVector;
 
Vector__BinaryVector * Vector__BinaryVector_GetInstance(void); //构造结构体实例的函数
void Vector__BinaryVector_ReleaseInstance(Vector__BinaryVector **);//析构结构体实例的函数
//获取字面量
float Vector__BinaryVector_getInstanceX(Vector__BinaryVector *);
float Vector__BinaryVector_getInstanceY(Vector__BinaryVector *); 
//设置字面量
void Vector__BinaryVector_setInstanceX(Vector__BinaryVector *,float);
void Vector__BinaryVector_setInstanceY(Vector__BinaryVector *,float);
void Vector__BinaryVector_setInstance(Vector__BinaryVector *,float,float);

//将实例方法包装为函数
float Vector__BinaryVector_mod(Vector__BinaryVector *);
float Vector__BinaryVector_mul( Vector__BinaryVector * , Vector__BinaryVector *); 
Vector__BinaryVector * Vector__BinaryVector_add(Vector__BinaryVector *, Vector__BinaryVector *);

#ifdef __cplusplus
}
#endif
#endif