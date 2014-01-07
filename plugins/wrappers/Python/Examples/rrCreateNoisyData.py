import roadrunner
from rrPlugins_CAPI import *
import rrPlugins as rrp

noisePlugin = rrp.Plugin ("rrp_add_noise")

print noisePlugin.listOfProperties()

# Create a roadrunner instance
rr = roadrunner.RoadRunner()
rr.load("sbml_test_0001.xml")

# Generate data
rr.simulate(0, 10, 511) # Want 512 points

# The plugin will need a handle to the underlying roadrunner data
d = rrp.getRoadRunnerData (rr)

noisePlugin.InputData = d

# Get parameter for the 'size' of the noise
noisePlugin.Sigma = 3.e-5

noisePlugin.execute ()

numpydata = noisePlugin.InputData.AsNumpy;

rrp.plot (numpydata[:,[0,2]], myColor="blue", myLinestyle="-", myMarker="", myLabel="S1")

rrp.show()

d.writeDataSeries ("testData2.dat")

d.readDataSeries ("testData2.dat")
print "done"
