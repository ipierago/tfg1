#include "foobar.h"

#include <tfg/foo/foo.h>
#include <tfg/foo/myobject.h>
#include <tfg/foo/imyotherobject.h>

#include <tfg/bar/bar.h>
#include <tfg/bar/myobject.h>

#include <tfg/log/log.h>

namespace TFG
{

namespace FooBar
{
	namespace {
		void CreateAndDestroyObjects()
		{
			Foo::Foo s_Foo(0);
			Bar::Bar s_Bar(0);
			{
				Foo::MyObject s_MyObject(0);
				Foo::IMyOtherObject s_IMyOtherObject(0);
			}
			{
				Bar::MyObject s_MyObject(0);
			}
		}
	}


int Main()
{
    TFG_Init();

	CreateAndDestroyObjects();

	TFG::Log::SetGlobalLevel(TFG::Log::Level_Trace);
	CreateAndDestroyObjects();

	TFG::Log::SetGlobalLevel(TFG::Log::Level_Warning);
	CreateAndDestroyObjects();

    TFG_Deinit();
    return 0;
}

} // namespace FooBar

} // namespace TFG
