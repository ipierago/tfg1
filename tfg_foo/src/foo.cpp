#include "stdafx.h"
#include "foo.h"

TFG_LOG_MODULE_SETUP("TFG_Foo")

TFG_LOG_FILE_SETUP()


namespace TFG
{
namespace Foo
{

uint32_t Foo::s_Param0;

Foo::Foo(uint32_t const in_Param0)
{
	TFG_LOG_FUNCTION();
	TFG_LOG_PRINTF_DEBUG("");
	TFG_LOG_PRINTF_TRACE("");
    s_Param0 = in_Param0;
}

Foo::~Foo()
{
}

} // namespace Foo

} // namespace TFG
