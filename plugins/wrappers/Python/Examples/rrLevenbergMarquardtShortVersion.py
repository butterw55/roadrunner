import matplotlib.pyplot as plot
from numpy import *
import rrPlugins
import rrPlugins_CAPI

lm = rrPlugins.Plugins ("rrp_lm")

experimentalData = lm.loadDataSeries ("testData.dat")

lm.setParameter ("ExperimentalData", experimentalData.data)
lm.setParameter ("SBML", rrPlugins_CAPI.getText("sbml_test_0001.xml"))

# Add parameters to fit
paraList = rrPlugins_CAPI.getPluginParameter(lm.plugin, "InputParameterList");
para1 = rrPlugins_CAPI.createParameter("k1", "double", "", 0.2)
rrPlugins_CAPI.addParameterToList (paraList, para1)

lm.setParameter("FittedDataSelectionList", "[S1] [S2]")
lm.setParameter("ExperimentalDataSelectionList", "[S1] [S2]")

# Execute lmfit plugin
res = lm.execute()

# Get the experimental data as a numpy array
experimentalData = experimentalData.getnumpy()

# Get the fitted and residual data
fittedData = rrPlugins.DataSeries (lm.getParameter ("FittedData")).getnumpy()
residuals = rrPlugins.DataSeries (lm.getParameter ("Residuals")).getnumpy()

y1Input = experimentalData[:,1]
y2Input = experimentalData[:,2]

S1Model = fittedData[:,1]
S2Model = fittedData[:,2]

s1Residual= residuals[:,1]
s2Residual= residuals[:,2]

plot.plot(x, y1Input,'*r', label="S1 Input")
plot.plot(x, S1Model, '-r', label="S1 Fitted")

plot.plot(x, y2Input,'*b', label="S2 Input")
plot.plot(x, S2Model, '-b', label="S2 Fitted")

plot.plot(x, s1Residual,'xr',  label="S1 Residual")
plot.plot(x, s2Residual, 'xb', label="S2 Residual")

plot.legend(bbox_to_anchor=(1.05, 1), loc=1, borderaxespad=0.)
plot.show()

