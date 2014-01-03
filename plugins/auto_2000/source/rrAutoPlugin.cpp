#pragma hdrstop
#include "rrLogger.h"
#include "rrAutoPlugin.h"
#include "rrc_api.h"
#include "rrc_utilities.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerData.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------
namespace autoplugin
{

using namespace rr;
using namespace rrc;

AutoPlugin::AutoPlugin(rr::RoadRunner* aRR, const PluginManager* pm)
:
CPPPlugin("Auto-2000", "Bifurcation", aRR, pm),
//The Capability
mAuto(                          "Auto",                                 "<none>",               "Bifurcation"),
mTempFolder(                    "TempFolder",                           "<none>",               "Tempfolder used by auto"),
mSBML(                          "SBML",                                 "<none>",               "SBML, i.e. the model to be used to analyze"),
mScanDirection(                 "ScanDirection",                        sdPositive,             "Direction of parameter scan"),
mPrincipalContinuationParameter("PrincipalContinuationParameter",       "<none>",               "Principal Continuation Parameter"),
mPCPLowerBound(                 "PCPLowerBound",                        0,                      "Principal Continuation Parameter Lower Bound"),
mPCPUpperBound(                 "PCPUpperBound",                        0,                      "Principal Continuation Parameter Upper Bound"),
mBiFurcationDiagram(            "BiFurcationDiagram",                   "<none>",               "BifurcationDiagram"),
mAutoData(                      "AutoData",                             AutoData(),             "Data structure holding auto data"),
mRRAuto(aRR, mAutoData.getValueReference()),
mAutoWorker(*this)
{
    //Setup the plugins capabilities
    mAuto.addParameter(&mTempFolder);
    mAuto.addParameter(&mSBML);
    mAuto.addParameter(&mAutoData);
    mAuto.addParameter(&mScanDirection);
    mAuto.addParameter(&mPrincipalContinuationParameter);
    mAuto.addParameter(&mPCPLowerBound);
    mAuto.addParameter(&mPCPUpperBound);
    mAuto.addParameter(&mBiFurcationDiagram);
    mCapabilities.add(mAuto);
}

AutoPlugin::~AutoPlugin()
{}

RRAuto& AutoPlugin::getRRAuto()
{
    return mRRAuto;
}

void AutoPlugin::setScanDirection(ScanDirection dir)
{
    mScanDirection.setValue(dir);
}

void AutoPlugin::setPrincipalContinuationParameter(const string& para)
{
    mPrincipalContinuationParameter.setValue(para);
}

void AutoPlugin::setPCPLowerBound(double val)
{
    mPCPLowerBound.setValue(val);
}

void AutoPlugin::setPCPUpperBound(double val)
{
    mPCPUpperBound.setValue(val);
}

void AutoPlugin::setSBML(const string& val)
{
    mSBML.setValue(val);
}

string AutoPlugin::getConstants()
{
    return mRRAuto.getConstantsAsString();
}

bool AutoPlugin::isWorking()
{
    return mAutoWorker.isRunning();
}

string AutoPlugin::getStatus()
{
    stringstream msg;
    msg<<Plugin::getStatus();
    msg<<"TempFolder: "<<mTempFolder<<"\n";
    msg<<"SBML: "<<mSBML<<"\n";
    AutoData* autoData = (AutoData*) (mAutoData.getValueAsPointer());
    msg<<"MinData"<<(*autoData)<<"\n";
    return msg.str();
}

bool AutoPlugin::resetPlugin()
{
    if(mAutoWorker.isRunning())
    {
        return false;
    }

    AutoData& data = getAutoData();
//    data.reset();
    return true;
}

AutoData& AutoPlugin::getAutoData()
{
    return *(mAutoData.getValuePointer());
}

string AutoPlugin::getTempFolder()
{
    return mTempFolder.getValue();
}

string AutoPlugin::getSBML()
{
    return mSBML.getValue();
}

string AutoPlugin::getResult()
{
    stringstream msg;
    AutoData& data = getAutoData();

    msg<<data;
    return msg.str();
}

bool AutoPlugin::setInputData(void* inputData)
{
    return true;
}

bool AutoPlugin::execute(void* inputData, bool useThread)
{
    Log(lInfo)<<"Executing the AutoPlugin plugin";

    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread
    mAutoWorker.assignCallBacks(mWorkStartedCB, mWorkFinishedCB, mUserData);

    mAutoWorker.start(useThread);
    return true;
}

// Plugin factory function
AutoPlugin* plugins_cc createPlugin(rr::RoadRunner* aRR, const PluginManager* pm)
{
    //allocate a new object and return it
    return new AutoPlugin(aRR, pm);
}


const char* plugins_cc getImplementationLanguage()
{
    return "CPP";
}

}

