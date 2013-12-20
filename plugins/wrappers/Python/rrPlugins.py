##@Module rrPlugins_CAPI

import rrPlugins_CAPI as rrp 

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
        
    def getnumpy (self):
        return rrp.getNumpyData (self._data)

    data = property (__getHandle)
    

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
           rrp.setPluginParameter (self.plugin, name, value)
        
    def getParameter (self, name):
        handle = rrp.getPluginParameter (self.plugin, name)
        return rrp.getParameter (handle)        
        
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
        
    def loadPlugins(self):
        rrp.loadPlugins (self.pluginsManager)
        
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
        