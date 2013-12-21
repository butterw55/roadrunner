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

class Event(object):

    def __init__(self):
        self.handlers = []

    def add(self, handler):
        self.handlers.append(handler)
        return self

    def remove(self, handler):
        self.handlers.remove(handler)
        return self

    def fire(self, sender, earg=None):
        for handler in self.handlers:
            handler(sender, earg)

    __iadd__ = add
    __isub__ = remove
    __call__ = fire



# ------------------------------------------------------------------------

_pluginManager = rrp.createPluginManager()
_pluginsAlreadyLoaded = False

class Plugin:

    _OnProgressList = Event()
    _onProgressEvent = 0

    def __init__(self, pluginName):
        self.pluginName = pluginName
        self.plugin = rrp.loadPlugin (_pluginManager, pluginName)

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

    def listOfParameters (self):
        nameList = rrp.getListOfPluginParameterNames (self.plugin)
        aList = []
        for i in range (0, len (nameList)):
            name = nameList[i]
            handle = rrp.getPluginParameter(self.plugin, nameList[i])
            hint = rrp.getParameterHint(handle)
            aList.append ([name, hint])
        return aList

    def loadDataSeriesAsNumPy (self, fileName):
        rrDataHandle = rrp.createRoadRunnerDataFromFile (fileName)
        return rrp.getNumpyData (rrDataHandle)

    def loadDataSeries (self, fileName):
        handle = rrp.createRoadRunnerDataFromFile (fileName)
        return DataSeries(handle)

    def OnProgress (self, f):
        global _onProgressEvent

        _onProgressEvent =  rrp.NotifyIntIntEvent(f)
        rrp.assignOnProgressEvent(self.plugin, _onProgressEvent)

    def execute (self):
        return rrp.executePlugin (self.plugin)

    def executeEx (self, inThread):
        return rrp.executePluginEx (self.plugin, inThread)

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

    @staticmethod
    def listOfPlugins():
        global _pluginsAlreadyLoaded
        # Hack to get round bug in loadPlugins
        if not _pluginsAlreadyLoaded:
           rrp.loadPlugins (_pluginManager)
           _pluginsAlreadyLoaded = True

        aList = []
        names = rrp.getPluginLibraryNames (_pluginManager)
        n = rrp.getNumberOfPlugins (_pluginManager)
        # This is a hack to get round thelack of metadata in the plugin
        # Will be resolved in next revision of library
        for i in range (0, n):
            handle = rrp.getPlugin(_pluginManager, names[i])
            info = rrp.getPluginInfo (handle)
            info = info.split ("\n")
            hint = info[2]
            hint = hint.replace("Category......................", "")
            aList.append ([names[i], hint])
        return aList

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

    def sayHello (arg1, arg2):
        print "Hello"

    print "Starting Test"

    p = Plugin ("rrp_add_noise")
    pl = p.listOfParameters()
    for item in pl:
        print item

    p.OnProgress (sayHello)
#    p.setParameter ("Sigma", 0.0001)
#
#    series = p.loadDataSeries (".\\Examples\\testData.dat")
#    p.plotTimeSeriesHandle (series)
#    p.setParameter ("InputData", series)
#    p.execute()
#    p.plotTimeSeriesHandle (series)

    print "Test Finished"
