#pragma hdrstop
#pragma argsused
#include <iostream>
#include "rrException.h"
#include "rrRoadRunner.h"
#include "rrPlugin.h"
#include "rrUtils.h"
#include "rrLogger.h"
using namespace rr;
using namespace std;

int main()
{
	try
    {
        //Create a RoadRunner object
        RoadRunner rr("r:\\temp");
        LogOutput::mLogToConsole = true;

     	//Load a model
        rr.loadSBMLFromFile("r:\\models\\test_1.xml");

        //Get the plugin manager
        PluginManager& plugins = rr.getPluginManager();

        //if(!plugins.load("c_plugin_demo"))
        if(!plugins.load())
        {
            clog<<"Failed loading plugins..\n";
            return -1;
        }

		Plugin* demoPlugin = plugins.getPlugin("c_plugin_demo");
        if(!demoPlugin)
        {
        	throw("Demo plugin could not be retrieved from the Plugin Manger");
        }
        char* rrInfo;
        if(!demoPlugin->execute(&rrInfo))
        {
        	cout<<"There was a problem executing the plugin...";
        }

        cout<< endl << "After Execute: "<< rrInfo << endl;
        freeText(rrInfo);

        cout<<demoPlugin->getInfo();
        plugins.unload();
        pause(true);
    }
    catch(const rr::Exception& ex)
    {
    	clog<<"There was a problem: "<<ex.what();
    }
	return 0;
}

#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
#pragma comment(lib, "poco_foundation-static.lib")