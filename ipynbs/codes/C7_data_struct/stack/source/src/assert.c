static char rcsid[] = "$Id$";
#include "assert.h"

const Except_T Assert_Failed = { "Assertion failed" };
void (assert)(int e){ //围绕assert的括弧防止被宏展开
    assert(e);
}