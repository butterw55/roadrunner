import ctypes
from rrPlugins_CAPI import *
from rrPlugins import *

try:
    #Create a plugin manager
    pm = createPluginManager()

    #Create a roadrunner instance
    rr = roadrunner.RoadRunner()
    
    sbmlModel ="../../models/bistable.xml"
    
    rr.load(sbmlModel)
        
    #Get a auto_2000 plugin object
    auto = Plugin("rrp_auto")
    
    #Load Auto plugin
    if not auto.plugin:
        print 'LastError: ' + getLastError()
        exit()
    
    print getPluginInfo(plugin)
    
    #Set Auto Propertys
    setPluginProperty(plugin, "ScanDirection", "Negative")
    setPluginProperty(plugin, "PrincipalContinuationProperty", "k3")
    setPluginProperty(plugin, "PCPLowerBound", "0.2")
    setPluginProperty(plugin, "PCPUpperBound", "1.2")
    
    #get handle to a parameter
    paraHandle = getPluginProperty(plugin,"PCPLowerBound")
    test = getPropertyValueAsString(paraHandle)
    print 'Current value is ' + test
    
    #Execute the plugin
    executePlugin(plugin)
    
    biFurcationDiagram = getPluginProperty(plugin, "BiFurcationDiagram")
    if biFurcationDiagram:
        print `getPropertyValueAsString(biFurcationDiagram)`
    
    print "done"

except Exception as e:  
    print "There was a problem: " + `e`
    