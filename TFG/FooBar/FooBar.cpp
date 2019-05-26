#include "FooBar.h"

#include <TFG/Foo/Foo.h>
#include <TFG/Foo/MyObject.h>
#include <TFG/Foo/IMyOtherObject.h>

#include <TFG/Bar/Bar.h>
#include <TFG/Bar/MyObject.h>

namespace TFG
{

namespace FooBar
{

int Main()
{
    ::TFG::TFG s_TFG(0);
    Foo::Foo s_Foo(s_TFG, 0);
    Bar::Bar s_Bar(s_TFG, 0);
    {
        Foo::MyObject s_MyObject(s_Foo, 0);
        Foo::IMyOtherObject s_IMyOtherObject(s_Foo, 0);
    }
    {
        Bar::MyObject s_MyObject(s_Bar, 0);
    }
    return 0;
}

} // namespace FooBar

} // namespace TFG
