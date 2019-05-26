#include "Foo.h"
#include "MyObject.h"

namespace TFG
{

namespace Foo
{

MyObject::MyObject(Foo &in_Foo, uint32_t const in_Param0) : m_Foo(in_Foo)
{
}

MyObject::~MyObject()
{
}

void MyObject::MyMethod(uint32_t const in_Param0)
{
}

} // namespace Foo

} // namespace TFG
