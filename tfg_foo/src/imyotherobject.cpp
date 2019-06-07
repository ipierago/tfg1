#include "stdafx.h"
#include "foo.h"
#include "imyotherobject.h"
#include "myotherobject.h"

namespace TFG
{

namespace Foo
{

IMyOtherObject::IMyOtherObject(uint32_t const in_Param0) : m_MyOtherObjectPtr(new MyOtherObject(in_Param0))
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
