#include <iostream>
#include "rrStringList.h"
#include "rrUtils.h"
#include "rrc_api.h"
#include "rrp_api.h"
using namespace rr;
using namespace rrc;
using namespace rrp;
using namespace std;

int main()
{
    try
    {
        enableLoggingToConsole();
        setLogLevel("LOG_Debug");
        logMsg(clInfo, "Log message....");

        RRPluginManagerHandle pm = createPluginManager("..\\plugins");
        loadPlugins(pm);

        char* list = getPluginNames(pm);
        if(!list)
        {
            throw("No plugins was loaded");
        }

        StringList names = splitString(list, ",");
        rr::freeText(list);

        for(int i = 0; i < names.size(); i++)
        {
            cout << names[i] << endl;
        }

        RRPluginHandle plugin = getPlugin(pm, "C Plugin Demo");
        if(!plugin)
        {
            throw("Demo plugin could not be loaded");
        }


        //Retrieve any properties that the plugin has
        char* properties = getListOfPluginPropertyNames(plugin);
        if(!properties)
        {
            throw("Plugin do not have any properties. For this demo, this is an error!");
        }

        StringList propNames(properties, " ");
        for(int i = 0; i < propNames.size(); i++)
        {
            cout << propNames[i] << endl;
        }


        if(!executePlugin(plugin))
        {
            throw("There was a problem executing the plugin...");
        }

//        cout<< endl << "After Execute: "<< info << endl;

        char* pluginInfo = getPluginInfo(plugin);

        if(pluginInfo)
        {
            cout<<pluginInfo;
            rr::freeText(pluginInfo);
        }

//        freeRRInstance(rri);

        //This will also unload all plugins..
        freePluginManager(pm);
    }
    catch(const char* msg)
    {
        clog<<"There was a problem: "<<msg;
    }
    return 0;
}



