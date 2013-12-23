import matplotlib.pyplot as plot
from numpy import *
from rrPlugins_CAPI import *

#Create a plugin manager
pm = createPluginManager()

#Create a roadrunner instance
rr = roadrunner.RoadRunner()

# Create some data
result = rr.load("../../models/sbml_test_0001.xml")
rr.simulate(0, 10, 512)

#Each roadrunner instance has an embedded dataobject
#get a handle to it by the following function
rrDataHandle = getRoadRunnerDataHandle(rr)

#Load the 'noise' plugin in order to add some noise to the data
noisePlugin = loadPlugin(pm, "rrp_add_noise")

#Set data "input" parameter, named InputData
dataPara = getPluginParameter(noisePlugin, "InputData")
setParameter(dataPara, rrDataHandle)

#get parameter for the 'size' of the noise
#Set size of noise
setPluginParameter(noisePlugin,"Sigma", 1.e-5)

#Execute the noise plugin which will add some noise to the data
executePlugin(noisePlugin)

#Retrieve data from NOISE plugin, pass this to the LMFIt plugin
inputDataHandle = getParameter(dataPara)
inputData = getNumpyData(inputDataHandle)

#Load the LevenbergMarquardt plugin
lmPlugin = loadPlugin(pm, "rrp_lm")

#Setup the plugin for minimization
#set input sbml model
sbml = getText("../../models/sbml_test_0001.xml")
setPluginParameter(lmPlugin, "SBML", sbml)

#See documentation for available parameters
experimentalData    = getPluginParameter(lmPlugin, "ExperimentalData");
paraList            = getPluginParameter(lmPlugin, "InputParameterList");

#Add parameters to fit
para1 = createParameter("k1", "double", "", 0.2)
addParameterToList(paraList, para1)

#Input Data
setRoadRunnerDataParameter(experimentalData, inputDataHandle)

#set species to fit
species = "[S1] [S2]"
paraHandle = getPluginParameter(lmPlugin, "FittedDataSelectionList");
setParameterByString(paraHandle, species)

#Get species list in observed data
paraHandle = getPluginParameter(lmPlugin, "ExperimentalDataSelectionList");
setParameterByString(paraHandle, species)

#Check plugin status, input
print '=========================== Levenberg-Marquardt report before minimization '
print getPluginStatus(lmPlugin)

#Execute lmfit plugin
res = executePluginEx(lmPlugin)

print '=========================== Levenberg-Marquardt report after minimization '
print getPluginStatus(lmPlugin)

# Look at the data
x = inputData[:,0] #result['time']
y1Input = inputData[:,1]
y2Input = inputData[:,2]

# Look at the data
dataPHandle = getPluginParameter(lmPlugin, "FittedData");
dataHandle = getParameterValue(dataPHandle)
npData = getNumpyData(dataHandle)
S1Model = npData[:,1]
S2Model = npData[:,2]

dataPHandle = getPluginParameter(lmPlugin, "Residuals");
dataHandle = getParameterValue(dataPHandle)
npData = getNumpyData(dataHandle)
s1Residual= npData[:,1]
s2Residual= npData[:,2]

plot.plot(x, y1Input,'*r', label="S1 Input")
plot.plot(x, S1Model, '-r', label="S1 Fitted")

plot.plot(x, y2Input,'*b', label="S2 Input")
plot.plot(x, S2Model, '-b', label="S2 Fitted")

plot.plot(x, s1Residual,'xr',  label="S1 Residual")
plot.plot(x, s2Residual, 'xb', label="S2 Residual")

plot.legend(bbox_to_anchor=(1.05, 1), loc=1, borderaxespad=0.)
plot.show()

print '=========== Minimization Result Follows\n' + getPluginResult(lmPlugin)
unLoadPlugins(pm)
print "done"
