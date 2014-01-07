#include <iostream>
#include "rrException.h"
#include "rrPluginManager.h"
#pragma hdrstop

using namespace std;
using namespace rr;
using namespace rrp;
int main()
{
    try
    {
        PluginManager pm("..\\plugins");
        cout<<"==== Loading Plugins =====\n";
        if(pm.load() == 0)
        {
            cout<<"No plugins were loaded";
        }
        else
        {
            cout<<"Loaded "<<pm.getNumberOfPlugins()<<" plugins\n";
        }
        cout<<"\n==== End of Loading Plugins =====\n\n";
        cout<<"Plugin Info ============\n"<<pm;
        pm.unload();
        return 0;

    }
    catch(const Exception& ex)
    {
        cout<<ex.what();
    }
}
