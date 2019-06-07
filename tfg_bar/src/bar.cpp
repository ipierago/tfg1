#include "bar.h"

TFG_LOG_MODULE_SETUP("tfg_bar")

TFG_LOG_FILE_SETUP()

namespace TFG
{
namespace Bar
{

uint32_t Bar::s_Param0;

Bar::Bar(uint32_t const in_Param0)
{
	TFG_LOG_PRINTF_DEBUG("test test test");
    s_Param0 = in_Param0;
}

Bar::~Bar()
{
}
} // namespace Bar

} // namespace TFG
