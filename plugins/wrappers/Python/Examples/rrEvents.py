import os
import roadrunner
import matplotlib.pyplot as plot
import numpy
from rrPlugins_CAPI import *

#Create a roadrunner instance
rr = roadrunner.RoadRunner()

#Create a plugin manager
pm = createPluginManager()

def pluginStarted():
    print 'The plugin was started'

def pluginIsProgressing(progress):
    nr = progress
    print '\nPlugin progress:' + `nr` +' %'

def pluginIsFinished():
    print 'The plugin did finish'

#Check if model file exists
sbmlModel ="../../models/bistable.xml"
if os.path.exists(sbmlModel):
    model = open(sbmlModel, 'r').read()
    rr.load(model)
else:
    print "The sbml model file: " + sbmlModel +" can't be found"
    exit()

timeStart = 0
timeEnd = 10
numPoints = 500
rr.simulate(timeStart, timeEnd, numPoints)

#Load the 'noise' plugin in order to add some noise to the data
plugin = loadPlugin(pm, "rrp_add_noise")

print getPluginInfo(plugin)

#get parameter for noise 'size'
sigmaHandle = getPluginParameter(plugin, "Sigma")

aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
setParameter(sigmaHandle, 0.02)

cb_func1 =  NotifyEvent(pluginStarted)
assignOnStartedEvent(plugin,  cb_func1)

cb_func2 =  NotifyIntEvent(pluginIsProgressing)
assignOnProgressEvent(plugin, cb_func2)

cb_func3 =  NotifyEvent(pluginIsFinished)
assignOnFinishedEvent(plugin, cb_func3)

#Assign data to the plugin
pluginData = getPluginParameter(plugin,"InputData")
setParameter(pluginData, getRoadRunnerDataHandle(rr))

#Execute the noise plugin which will add some noise to the (internal) data
executePluginEx(plugin)

#Retrieve data from plugin

rrData = getNumpyData(getParameter(pluginData))
colNames = getRoadRunnerDataColumnHeader(getParameter(pluginData))
plotRoadRunnerData(rrData, colNames)
unLoadPlugins(pm)
unLoadAPI()

print "done"
