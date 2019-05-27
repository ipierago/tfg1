#pragma once
#include <TFG/Foo/Foo.h>

namespace TFG
{

namespace Foo
{

class MyObject
{
public:
	MyObject(uint32_t const in_Param0);
	~MyObject();

	void MyMethod(uint32_t const in_Param0);

private:
	DISALLOW_COPY_AND_ASSIGN(MyObject);

	uint32_t const m_Param0;
};

} // namespace Foo

} // namespace TFG
