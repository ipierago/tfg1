#include "FooBar.h"

#include <TFG/Foo/Foo.h>
#include <TFG/Foo/MyObject.h>
#include <TFG/Foo/IMyOtherObject.h>

#include <TFG/Bar/Bar.h>
#include <TFG/Bar/MyObject.h>

#include <TFG/TFG_log.h>

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

	TFG::Log::SetGlobalLevel(TFG::Level_Trace);
	CreateAndDestroyObjects();

	TFG::Log::SetGlobalLevel(TFG::Level_Warning);
	CreateAndDestroyObjects();

    TFG_Deinit();
    return 0;
}

} // namespace FooBar

} // namespace TFG
