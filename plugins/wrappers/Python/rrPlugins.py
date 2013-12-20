##@Module rrPlugins_CAPI

import rrPlugins_CAPI as rrp 
import matplotlib.pyplot as plt

__version__ = "0.5.01"

class DataSeries(object):
    
    _data = 0
    
    def __init__ (self, handle):
        self._data = handle
        
    def __del__ (self):
        if (self._data != 0): 
            try:            
               rrp.freeRoadRunnerData (self._data)
            except:
                print "Exception"
            self._data = 0
    
    def __getHandle (self):
        return self._data
        
    def __AsNumpy (self):
        return rrp.getNumpyData (self._data)

    data = property (__getHandle)
    AsNumpy = property (__AsNumpy)
    

# ------------------------------------------------------------------------
        
class Plugins:

    def __init__(self, pluginName):
        self.pluginManager = rrp.createPluginManager()
        self.pluginName = pluginName
        self.plugin = rrp.loadPlugin (self.pluginManager, pluginName)
               
    def setParameter(self, name, value):
        if (isinstance (value, DataSeries)):        
           rrp.setPluginParameter (self.plugin, name, value.data)
        else:
           handle  = rrp.getPluginParameter(self.plugin, name);
           t1 = rrp.getParameterType (handle)
           if (t1 == "listOfParameters"):
              if isinstance (value, list):
                 if len(value) != 2:
                    raise TypeError ("Expecting two elements in the parameter list")
                 if not isinstance(value[0], str):
                     raise TypeError("Expecting parameter name in first element of list")
                 if (not isinstance(value[1], float)) and (isinstance(value[1], int)):
                     raise TypeError("Expecting floating value in second element of list")               
                 para1 = rrp.createParameter(value[0], "double", "", value[1])
                 rrp.addParameterToList (handle, para1)
              else:
                 raise  TypeError ("Expecting a list in setParameter")
           else:
              rrp.setPluginParameter (self.plugin, name, value)
        
    def getParameter (self, name):
        handle = rrp.getPluginParameter (self.plugin, name)
        value = rrp.getParameter (handle)        
        if (rrp.getParameterType(handle) == "roadRunnerData"):
            return DataSeries (value)
        else:        
           return value         
        
    def loadDataSeriesAsNumPy (self, fileName):
        rrDataHandle = rrp.createRoadRunnerDataFromFile (fileName)
        return rrp.getNumpyData (rrDataHandle)
        
    def loadDataSeries (self, fileName):
        handle = rrp.createRoadRunnerDataFromFile (fileName)
        return DataSeries(handle)
        
    def execute (self):
        return rrp.executePlugin (self.plugin)

    def executeEx (self):        
        return rrp.executePluginEx (self.plugin)
        
    def plotTimeSeriesHandle (self, dataSeries):
        if (isinstance (dataSeries, DataSeries)):        
           hdr = rrp.getRoadRunnerDataColumnHeader(dataSeries.data)
           npData = rrp.getNumpyData(dataSeries.data)
           rrp.plotRoadRunnerData(npData, hdr)
        else:
           raise TypeError ("Expecting DataSeries type")
           
    def readAllText(self, fName):
        file = open(fName, 'r')
        str = file.read()
        file.close()
        return str
        
    def loadPlugins(self):
        rrp.loadPlugins (self.pluginsManager)
        
        
       
def extractColumn (data, index):
    return data[:,index]
    
def plot (data, myColor="red", myLinestyle="None", myMarker="None", myLabel=""):
    columns = data.shape[1]
    for i in range(columns-1):
        p = plt.plot (data[:,0], data[:,i+1])
        plt.setp (p, color=myColor, marker=myMarker, linestyle = myLinestyle, linewidth=1, label=myLabel)
        plt.legend(bbox_to_anchor=(1.05, 1), loc=1, borderaxespad=0.)
        return p
         
if __name__=='__main__':
    
    print "Starting Test"
    
    p = Plugins ("rrp_add_noise")
    p.setParameter ("Sigma", 0.0001)
    
    series = p.loadDataSeries (".\\Examples\\testData.dat") 
    p.plotTimeSeriesHandle (series)
    p.setParameter ("InputData", series)
    p.execute()
    p.plotTimeSeriesHandle (series)
    
    print "Test Finished"
        