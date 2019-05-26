#pragma once
#include <TFG/Foo/Foo.h>

namespace TFG
{

namespace Foo
{

class MyObject
{
public:
	MyObject(Foo &, uint32_t const in_Param0);
	~MyObject();

	void MyMethod(uint32_t const in_Param0);

private:
	DISALLOW_COPY_AND_ASSIGN(MyObject);
};

} // namespace Foo

} // namespace TFG
