#pragma once
#include "netnt.h"

namespace TFG
{

namespace NetNT
{

struct OverlappedEx
{
    OVERLAPPED overlapped;
    TFG_TypeId _typeid;
    void *object_p;
    void *context_p;
};

} // namespace NetNT

} // namespace TFG
