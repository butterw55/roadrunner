#include "rrc_api.h"
#include "rrp_api.h"
#include "c_plugin_demo.h"

const char* cc getImplementationLanguage()
{
    return "C";
}

bool cc destroyPlugin(RRPluginHandle plugin)
{
    //Free any data that the plugin generated..
    freeText(text);
    freeProperty(gDemoProperty);
    freeProperties(gProperties);
}

const char* cc getName()
{
    return "C Plugin Demo";
}

const char* cc getCategory()
{
    return "Demos";
}

const char* cc getListOfCPluginPropertyNames(RRPluginHandle plugin)
{
    char* names = getNamesFromPropertyList(gProperties);
    return names;
}

void*  cc getCPluginProperty(const char* name)
{
    if(gPlugin)
    {
        return getProperty(gProperties, name);
    }
    else
    {
        return 0;
    }
}

bool cc setupCPlugin(RRPluginHandle aPlugin)
{
    gPlugin = aPlugin;
    gDemoProperty = createProperty("Demo Property", "string", "Demo Hint", 0);
    setPropertyByString(gDemoProperty, "Intial Demo Property Value");
    //Add the property to the property container
    gProperties = createPropertyList();

    addPropertyToList(gProperties, gDemoProperty);
    return true;
}

bool cc execute()
{
    char* text2;
    text = createTextMemory(textLen);
    gRR = getRRHandleFromPlugin(gPlugin);

    strcpy(text, "CPP API Version: ");
    text2 = getCPPAPIVersion(gRR);
    if(text2)
    {
        strcat(text, text2);
        freeText(text2);
    }
    else
    {
        return false;
    }

    setPropertyByString(gDemoProperty, text);
    return true;
}


