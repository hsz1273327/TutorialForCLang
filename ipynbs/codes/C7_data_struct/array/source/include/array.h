/* $Id$ */
#ifndef ARRAY_INCLUDED
#define ARRAY_INCLUDED

typedef struct Array_L *Array_T;
extern Array_T    Array_new (int length, int size);
extern void Array_free(Array_T *array);
extern int Array_length(Array_T array);
extern int Array_size  (Array_T array);
extern void *Array_get(Array_T array, int i);
extern void *Array_put(Array_T array, int i, void *elem);
extern void Array_resize(Array_T array, int length);
extern Array_T    Array_copy  (Array_T array, int length);
extern void ArrayRep_init(Array_T array, int length, int size, void *ary);

#endif