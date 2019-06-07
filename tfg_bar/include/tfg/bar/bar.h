#pragma once
#include <tfg/tfg.h>

namespace TFG
{

namespace Bar
{

class MyObject;

class Bar
{
public:
    Bar(uint32_t const in_Param0);
    ~Bar();

private:
    DISALLOW_COPY_AND_ASSIGN(Bar);

    static uint32_t s_Param0;
};

} // namespace Bar

} // namespace TFG
