from rrPlugins_CAPI import *
import ctypes
import rrPlugins as rrp

lm = rrp.Plugin ("rrp_lm")
##========== LMFIT EVENT FUNCTIONS ==================
def pluginIsProgressing(msg, lmP):
    print msg #Uncomment if more info is wanted
    # The plugin don't know what a python object is.
    # We need to cast it here, to a proper python object
    lmObject = cast(lmP, ctypes.py_object).value
    print 'Iterations = ' + `lmObject.getProperty("NrOfIter")` + '\tNorm = ' + `lmObject.getProperty("Norm")`

progressEvent =  NotifyPluginEvent(pluginIsProgressing)
theId = id(lm)

#In the assignOnProgress, we pass the identity of the plugin as last argument.
#It is later on retrieved in the plugin Event handler, see above
assignOnProgressEvent(lm.plugin, progressEvent, None, theId)
##===================================================

#Set a lmfit parametere. Printflags control the output in the callback message
#lm.setProperty("printflags", 2)
#lm.setProperty("ftol", 2.e-2)
experimentalData = lm.loadDataSeries ("testData.dat")

lm.setProperty ("ExperimentalData", experimentalData)
lm.setProperty ("SBML", lm.readAllText("sbml_test_0001.xml"))

# Add the parameters that we're going to fit and the initial value
lm.setProperty ("InputParameterList", ["k1", 10.2])

lm.setProperty("FittedDataSelectionList", "[S1] [S2]")
lm.setProperty("ExperimentalDataSelectionList", "[S1] [S2]")


# Execute lmfit plugin
res = lm.execute()

## ----------- Uncomment the following to test in a thread
##res = lm.executeEx(True)
##while isPluginWorking(lm.plugin):
##    print 'Working'

# Get the experimental data as a numpy array
experimentalData = experimentalData.AsNumpy

# Get the fitted and residual data
fittedData = lm.getProperty ("FittedData").AsNumpy
residuals  = lm.getProperty ("Residuals").AsNumpy

rrp.plot (fittedData[:,[0,1]], myColor="blue", myLinestyle="-", myMarker="", myLabel="S1 Fitted")
rrp.plot (fittedData[:,[0,2]], myColor="blue", myLinestyle="-", myMarker="", myLabel="S2 Fitted")
rrp.plot (residuals[:,[0,1]], myColor="blue", myLinestyle="None", myMarker="x", myLabel="S1 Residual")
rrp.plot (residuals[:,[0,2]], myColor="red", myLinestyle="None", myMarker="x", myLabel="S2 Residual")
rrp.plot (experimentalData[:,[0,1]], myColor="red", myLinestyle="", myMarker="*", myLabel="S1 Data")
rrp.plot (experimentalData[:,[0,2]], myColor="blue", myLinestyle="", myMarker="*", myLabel="S2 Data")
rrp.plt.show()

print getPluginResult(lm.plugin)
print getPluginCategory(lm.plugin)
print getPluginDescription(lm.plugin)
print getPluginHint(lm.plugin)
#Observe that getPluginProperties prings out the value for each parameter, including the parameters that are RoadRunnerData
print getPluginStatus(lm.plugin)


