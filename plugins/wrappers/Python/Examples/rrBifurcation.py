import ctypes
from rrPlugins_CAPI import *
from rrPlugins import *

try:
    #Create a roadrunner instance
    rr = roadrunner.RoadRunner()
    
    sbmlModel ="../../models/bistable.xml"
    
    rr.load(sbmlModel)
                    
    #Get an auto2000 plugin object
    auto = Plugin("rrp_auto2000")
    
    rrHandle = cast(int(rr.this), c_void_p)
    assignRoadRunnerInstance(auto.plugin, rrHandle)    
    #Load Auto plugin
    if not auto.plugin:
        print 'LastError: ' + getLastError()
        exit()
    
    print getPluginInfo(auto.plugin)
    
    #Set Auto Propertys
    auto.setProperty("ScanDirection", "Negative")
    auto.setProperty("PrincipalContinuationParameter", "k3")
    auto.setProperty("PCPLowerBound", 0.2)
    auto.setProperty("PCPUpperBound", 1.2)
    
    #get handle to a parameter
    print 'PCPLowerBound = ' + `auto.PCPLowerBound`
           
    #Execute the plugin
    auto.execute()
    
    biFurcationDiagram = auto.BiFurcationDiagram
    if biFurcationDiagram:
        print biFurcationDiagram
    
    print "done"

except Exception as e:  
    print "There was a problem: " + `e`
    