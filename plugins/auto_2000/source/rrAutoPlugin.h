#ifndef rrAutoPluginH
#define rrAutoPluginH
#include <vector>
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrCPPPlugin.h"
#include "rrRoadRunner.h"
#include "rrPluginManager.h"
#include "../rrAutoInterface/rrAutoData.h"
#include "../rrAutoInterface/rrRRAuto.h"
#include "rrAutoWorker.h"
#include "rrAutoPluginExporter.h"
//---------------------------------------------------------------------------

namespace autoplugin
{

using namespace rrauto;

//If plugin is loaded by a program that is binary compatible witht he plugin, we could use class functions directly.
class AutoPlugin : public CPPPlugin
{
    public:
                                                AutoPlugin(rr::RoadRunner* aRR = NULL, const PluginManager* pm = NULL);
                                               ~AutoPlugin();
        //Data input
        void                                    setScanDirection(ScanDirection dir);
        void                                    setPrincipalContinuationParameter(const string& para);
        void                                    setPCPLowerBound(double lb);
        void                                    setPCPUpperBound(double ub);
        void                                    setSBML(const string& sbml);

        bool                                    execute(void* inputData = NULL, bool useThread = false);

        string                                  getResult();
        string                                  getConstants();
        bool                                    resetPlugin();
        bool                                    setInputData(void* data);


        string                                  getStatus();
        bool                                    isWorking();
        RRAuto&                                 getRRAuto();



    protected:
        Capability                              mAuto;
        Parameter<string>                       mTempFolder;
        Parameter<string>                       mSBML;                  //This is the model
        Parameter<rrauto::ScanDirection>        mScanDirection;         //How auto sweeps the parameter

        Parameter<string>                       mPrincipalContinuationParameter;
        Parameter<double>                       mPCPLowerBound;
        Parameter<double>                       mPCPUpperBound;
        Parameter<string>                       mBiFurcationDiagram;    //This is generated data
        Parameter<AutoData>                     mAutoData;              //This plugin generates a lot of data

        RRAuto                                  mRRAuto;                //The interface to auto. Takes mAutoData as reference
        //Utility functions for the thread
        string                                  getTempFolder();
        string                                  getSBML();

        AutoData&                               getAutoData();

        //The worker
        AutoWorker                              mAutoWorker;

        //So make it a friend
        friend class AutoWorker;

};

extern "C"
{
RR_PLUGIN_DECLSPEC AutoPlugin* plugins_cc       createPlugin(rr::RoadRunner* aRR, const PluginManager* rrp);
RR_PLUGIN_DECLSPEC const char* plugins_cc       getImplementationLanguage();
}

}

namespace rrp
{
template<>
inline std::string Parameter< rrauto::ScanDirection >::getType() const
{
    return "ScanDirection";
}

template<>
inline string Parameter< rrauto::ScanDirection >::getValueAsString() const
{
    return mValue == rrauto::sdPositive ? "Positive" : "Negative";
}

template<>
inline void Parameter< rrauto::ScanDirection >::setValueFromString(const string& val)
{
    mValue = compareNoCase(val, "Positive") == 0 ? rrauto::sdPositive : rrauto::sdNegative;
}

template<>
inline void Parameter< rrauto::ScanDirection >::setValue(const rrauto::ScanDirection& val)
{
    mValue = (val);
}
}



#endif
