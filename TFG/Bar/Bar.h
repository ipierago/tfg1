#pragma once
#include <TFG/TFG.h>

namespace TFG
{

namespace Bar
{

class MyObject;

class Bar
{
public:
    Bar(::TFG::TFG &, uint32_t const in_Param0);
    ~Bar();

private:
    DISALLOW_COPY_AND_ASSIGN(Bar);
};

} // namespace Bar

} // namespace TFG
