#include <iostream>
#include "rrUtils.h"
#include "rrException.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
#pragma hdrstop

using namespace std;
using namespace rr;
using namespace rrp;
int main()
{
    try
    {
        Property<double> aProperty(23.45, "TestProp", "The Hint", "The Descr", "TheAlias", false);

        cout<<aProperty;
    }
    catch(const Exception& ex)
    {
        cout<<ex.what();
    }
}
