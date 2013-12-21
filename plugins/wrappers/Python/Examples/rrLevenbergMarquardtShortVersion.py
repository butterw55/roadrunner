from rrPlugins_CAPI import *
import rrPlugins as rrp

lm = rrp.Plugin ("rrp_lm")
##========== LMFIT EVENT FUNCTIONS ==================
def pluginIsProgressing(progressMessage):
    print progressMessage
progressEvent =  NotifyStringEvent(pluginIsProgressing)
assignOnProgressEvent(lm.plugin, progressEvent)
##===================================================

experimentalData = lm.loadDataSeries ("testData.dat")

lm.setParameter ("ExperimentalData", experimentalData)
lm.setParameter ("SBML", lm.readAllText("../../models/sbml_test_0001.xml"))

# Add the parameters that we're going to fit and the initial value
lm.setParameter ("InputParameterList", ["k1", 0.2])

lm.setParameter("FittedDataSelectionList", "[S1] [S2]")
lm.setParameter("ExperimentalDataSelectionList", "[S1] [S2]")

# Execute lmfit plugin
res = lm.execute()

## ----------- Uncomment the following to test in a thread
##res = lm.executeEx(True)
##while isPluginWorking(lm.plugin):
##    print 'Working'

# Get the experimental data as a numpy array
experimentalData = experimentalData.AsNumpy

# Get the fitted and residual data
fittedData = lm.getParameter ("FittedData").AsNumpy
residuals  = lm.getParameter ("Residuals").AsNumpy

rrp.plot (fittedData[:,[0,1]], myColor="blue", myLinestyle="-", myMarker="", myLabel="S1 Fitted")
rrp.plot (fittedData[:,[0,2]], myColor="blue", myLinestyle="-", myMarker="", myLabel="S2 Fitted")
rrp.plot (residuals[:,[0,1]], myColor="blue", myLinestyle="None", myMarker="x", myLabel="S1 Residual")
rrp.plot (residuals[:,[0,2]], myColor="red", myLinestyle="None", myMarker="x", myLabel="S2 Residual")
rrp.plot (experimentalData[:,[0,1]], myColor="red", myLinestyle="", myMarker="*", myLabel="S1 Data")
rrp.plot (experimentalData[:,[0,2]], myColor="blue", myLinestyle="", myMarker="*", myLabel="S2 Data")
rrp.plt.show()

print getPluginResult(lm.plugin)



