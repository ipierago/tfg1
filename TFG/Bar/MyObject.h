#pragma once
#include <TFG/Bar/Bar.h>

namespace TFG
{

namespace Bar
{

class MyObject
{
public:
	MyObject(Bar &, uint32_t const in_Param0);
	~MyObject();

	void MyMethod(uint32_t const in_Param0);

private:
	DISALLOW_COPY_AND_ASSIGN(MyObject);
};

} // namespace Bar

} // namespace TFG
