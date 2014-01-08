#ifndef c_plugin_demoH
#define c_plugin_demoH
#include "rrpExporter.h"
#include "rrp_types.h"
//-----------------------------------------------------------
#define cc __cdecl

//Global Plugin Data
RRHandle            gRR;
RRPluginHandle      gPlugin;
RRPropertyHandle    gDemoProperty;
RRPropertiesHandle  gProperties;
char*               text;
const int           textLen = 2048;

//Each of these functions need to be present in order to load a plugin written in C
const char*     RR_PLUGIN_DECLSPEC cc getImplementationLanguage(void);
const char*     RR_PLUGIN_DECLSPEC cc getName(void);
const char*     RR_PLUGIN_DECLSPEC cc getCategory(void);
bool            RR_PLUGIN_DECLSPEC cc setupCPlugin(RRPluginHandle aPlugin);
bool            RR_PLUGIN_DECLSPEC cc execute(void* userData);
bool            RR_PLUGIN_DECLSPEC cc destroyPlugin(RRPluginHandle plugin);
const char*     RR_PLUGIN_DECLSPEC cc getListOfCPluginPropertyNames(RRPluginHandle plugin);
void*           RR_PLUGIN_DECLSPEC cc getCPluginProperty(const char* name);

#endif



