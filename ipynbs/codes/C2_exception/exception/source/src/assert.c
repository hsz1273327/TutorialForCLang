/**
 * @file                              assert.c
 * @brief                             断言库
 * @detail  断言库,抛出一个异常结构体的实例
 * @version                            1.2
 * @par Copyright (c) 1994,1995,1996,1997 by David R. Hanson
 * @par LICENSE                        MIT
 * @par requirements:                  except.h
 * @par update:                        hsz
 * @par update date:                   2018-01-18
 */
#include "assert.h"
#include "except.h"

const Except_T Assert_Failed = {"Assertion failed"};
// 围绕assert的括弧防止被宏展开
void(assert)(int e) { assert(e); }
