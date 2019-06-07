#pragma once
#include <tfg/tfg.h>

namespace TFG
{

namespace Foo
{

class MyObject;
class MyOtherObject;
class IMyOtherObject;

class Foo
{
public:
    Foo(uint32_t const in_Param0);
    ~Foo();

private:
    static uint32_t s_Param0;

    friend class MyObject;
    friend class MyOtherObject;
    friend class IMyOtherObject;
};

} // namespace Foo

} // namespace TFG
