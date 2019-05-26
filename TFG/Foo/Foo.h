#pragma once
#include <TFG/TFG.h>

namespace TFG
{

namespace Foo
{

class MyObject;

class Foo
{
public:
    Foo(::TFG::TFG &, uint32_t const in_Param0);
    ~Foo();

private:
    DISALLOW_COPY_AND_ASSIGN(Foo);
};

} // namespace Foo

} // namespace TFG
