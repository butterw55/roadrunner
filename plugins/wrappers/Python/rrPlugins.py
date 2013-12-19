##@Module rrPluginsObj

import rrPlugins

__version__ = 0.5.0

class Plugins:

    def __init__(self, pluginName):
        self.pluginManager = rrPlugins.createPluginManager()
        self.pluginName = pluginName
        self.plugin = rrPlugins.loadPlugin (self.pluginManager, pluginName)
        
    def setParameter(self, name, value):
        rrPlugins.setPluginParameter (self.plugin, name, value)
        
    def getParameter (self, name):
        handle = rrPlugins.getPluginParameter (self.plugin, name)
        return rrPlugins.getParameter (handle)        
        
    def loadTimeSeries (self, fileName):
        rrDataHandle = rrPlugins.createRoadRunnerDataFromFile (fileName)
        return rrPlugins.getNumpyData (rrDataHandle)
        
    def loadTimeSeriesHandle (self, fileName):
        return rrPlugins.createRoadRunnerDataFromFile (fileName)
        
    def execute (self):
        rrPlugins.executePlugin (self.plugin)
        
    def plotTimeSeriesHandle (self, rrDataHandle):
        hdr = rrPlugins.getRoadRunnerDataColumnHeader(rrDataHandle)
        npData = rrPlugins.getNumpyData(rrDataHandle)
        rrPlugins.plotRoadRunnerData(npData, hdr)
        
    def loadPlugins():
        rrPlugins.loadPlugins (self.pluginsManager)
        
if __name__=='__main__':
    import roadrunner
    
    print "Starting Test"
    
    p = Plugins ("rrp_add_noise")
    p.setParameter ("Sigma", 0.0001)
    print p.getParameter ("Sigma")
    
    data = p.loadTimeSeriesHandle (".\Examples\\testData.dat")  
    p.plotTimeSeriesHandle (data)
    p.setParameter ("InputData", data)
    p.execute()
    p.plotTimeSeriesHandle (data)
    
    print "Test Finished"
        