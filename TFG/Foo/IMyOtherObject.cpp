#include "Foo.h"
#include "IMyOtherObject.h"
#include "MyOtherObject.h"

namespace TFG
{

namespace Foo
{

IMyOtherObject::IMyOtherObject(Foo &in_Foo, uint32_t const in_Param0) : m_MyOtherObjectPtr(new MyOtherObject(in_Foo, in_Param0))
{
}

IMyOtherObject::~IMyOtherObject()
{
}

void IMyOtherObject::MyMethod(uint32_t const in_Param0)
{
    m_MyOtherObjectPtr->MyMethod(in_Param0);
}

} // namespace Foo

} // namespace TFG
