#include <iostream>
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
        string fldr("../plugins");
        PluginManager pm(fldr);
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

        cout<<"\n\n Plugin Loading Info ============\n\n"<<pm;

        //Get info about each plugin
        cout<<"\n\nIndividual Plugin Info ============\n\n";
        for(int i =0; i < pm.getNumberOfPlugins(); i++)
        {
            Plugin* aPlugin = pm[i];
            if(aPlugin)
            {
                cout<<aPlugin->getInfo();
            }
        }
        pm.unload();
        return 0;

    }
    catch(const Exception& ex)
    {
        cout<<ex.what();
    }
}
