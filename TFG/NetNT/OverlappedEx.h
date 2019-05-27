#pragma once
#include "NetNT.h"

namespace TFG
{

namespace NetNT
{

struct OverlappedEx
{
    OVERLAPPED overlapped;
    Std_TypeId _typeid;
    void *object_p;
    void *context_p;
};

} // namespace NetNT

} // namespace TFG
