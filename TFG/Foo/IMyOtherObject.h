#pragma once
#include <TFG/Foo/Foo.h>

namespace TFG
{

namespace Foo
{

class IMyOtherObject
{
public:
	IMyOtherObject(Foo &, uint32_t const in_Param0);
	~IMyOtherObject();

	void MyMethod(uint32_t const in_Param0);

protected:
	DISALLOW_COPY_AND_ASSIGN(IMyOtherObject);
	std::shared_ptr<MyOtherObject> m_MyOtherObjectPtr;
};

} // namespace Foo

} // namespace TFG
