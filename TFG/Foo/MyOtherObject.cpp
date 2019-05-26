#include "Foo.h"
#include "MyOtherObject.h"

namespace TFG
{

namespace Foo
{

MyOtherObject::MyOtherObject(Foo &in_Foo, uint32_t const in_Param0) : m_Foo(in_Foo)
{
}

MyOtherObject::~MyOtherObject()
{
}

void MyOtherObject::MyMethod(uint32_t const in_Param0)
{
}

} // namespace Foo

} // namespace TFG
