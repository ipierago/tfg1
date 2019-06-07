#include "esutil.h"
#include "client.h"

void android_main ( struct android_app *pApp )
{
      esUtil_android_main(pApp, Client_esMain);
}
