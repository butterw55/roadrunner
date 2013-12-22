#pragma hdrstop
#include "rrLogger.h"
#include "rrException.h"
#include "lm.h"
#include "../../wrappers/C/rrc_api.h"
#include "../../wrappers/C/rrc_utilities.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerData.h"
#include "rrUtils.h"
#include "lmfit_doc.h"
//---------------------------------------------------------------------------

//Below defines are from the LMFIT lib.. convert to constants later on
/* machine-dependent constants from float.h */
#define LM_MACHEP     DBL_EPSILON   /* resolution of arithmetic */
#define LM_DWARF      DBL_MIN       /* smallest nonzero number */
#define LM_SQRT_DWARF sqrt(DBL_MIN) /* square should not underflow */
#define LM_SQRT_GIANT sqrt(DBL_MAX) /* square should not overflow */
#define LM_USERTOL    30*LM_MACHEP  /* users are recommended to require this */

namespace lmfit
{
using namespace rr;
using namespace rrc;


LM::LM(rr::RoadRunner* aRR)
:
CPPPlugin(                  "Levenberg-Marquardt", "Fitting",       aRR, NULL),
mLMFit(                     "LMFit",                                "Fit Model Parameters Using the Levenberg-Marquardt Algorithm"),    //The 'capability'
mSBML(                      "<none>",               "SBML",                                 "SBML, i.e. the model to be used in the fitting"),
mObservedData(              RoadRunnerData(),       "ExperimentalData",                     "Data object holding Experimental data: Provided by client"),
mModelData(                 RoadRunnerData(),       "FittedData",                           "Data object holding model data: Handed to client"),
mResidualsData(             RoadRunnerData(),       "Residuals",                            "Data object holding residuals: Handed to client"),
mInputParameterList(        Parameters(),           "InputParameterList",                   "List of parameters to fit"),
mOutputParameterList(       Parameters(),           "OutputParameterList",                  "List of parameters that was fittedt"),
mObservedDataSelectionList( StringList(),           "ExperimentalDataSelectionList",        "Experimental data selection list"),
mModelDataSelectionList(    StringList(),           "FittedDataSelectionList",              "Fitted data selection list"),
mNorm(                      -1.0,                   "Norm",                                 "Norm of fitting. An estimate of goodness of fit"),
mNrOfIter(                  -1,                     "NrOfIter",                             "Number of iterations"),
mLMWorker(*this),
mLMData(mLMWorker.mLMData),


ftol(                      LM_USERTOL,              "ftol"       ,                           " relative error desired in the sum of squares. "),
xtol(                      LM_USERTOL,              "xtol"       ,                           " relative error between last two approximations. "),
gtol(                      LM_USERTOL,              "gtol"       ,                           " orthogonality desired between fvec and its derivs. "),
epsilon(                   LM_USERTOL,              "epsilon"    ,                           " step used to calculate the jacobian. "),
stepbound(                 100.,                    "stepbound"  ,                           " initial bound to steps in the outer loop. "),
maxcall(                   100,                     "maxcall"    ,                           " maximum number of iterations. "),
scale_diag(                1,                       "scale_diag" ,                           " UNDOCUMENTED, TESTWISE automatical diag rescaling? "),
printflags(                0,                       "printflags" ,                           " OR'ed to produce more noise ")

{
    mVersion = "1.0";
    //Setup the plugins capabilities
    mLMFit.addParameter(&mSBML);
    mLMFit.addParameter(&mObservedData);
    mLMFit.addParameter(&mModelData);
    mLMFit.addParameter(&mResidualsData);
    mLMFit.addParameter(&mInputParameterList);
    mLMFit.addParameter(&mOutputParameterList);
    mLMFit.addParameter(&mObservedDataSelectionList);
    mLMFit.addParameter(&mModelDataSelectionList);
    mLMFit.addParameter(&mNorm);
    mLMFit.addParameter(&mNrOfIter);

    //Add the lmfit parameters

    mLMFit.addParameter(&ftol);
    mLMFit.addParameter(&xtol);
    mLMFit.addParameter(&gtol);
    mLMFit.addParameter(&epsilon);
    mLMFit.addParameter(&stepbound);
    mLMFit.addParameter(&maxcall);
    mLMFit.addParameter(&scale_diag);
    mLMFit.addParameter(&printflags);

    mCapabilities.add(mLMFit);
    //Allocate model and Residuals data
    mResidualsData.setValue(new RoadRunnerData());
    mModelData.setValue(new RoadRunnerData());
    
    //Setup so the Norm parameter is a reference of the NORM parameter in the lmDataStructure
    mNorm.getValueReference() = mLMWorker.mLMData.mNorm;
}

LM::~LM()
{}

bool LM::isWorking()
{
    return mLMWorker.isRunning();
}

unsigned char* LM::getManualAsPDF() const
{
    return pdf_doc;
}

unsigned int LM::getPDFManualByteSize()
{
    return sizeofPDF;
}

StringList LM::getObservedDataSelectionList()
{
    return mObservedDataSelectionList.getValue();
}

string LM::getStatus()
{
    stringstream msg;
    msg<<Plugin::getStatus();    
    msg <<getResult();
    return msg.str();
}

string LM::getImplementationLanguage()
{
    return lmfit::getImplementationLanguage();
}

bool LM::resetPlugin()
{
    if(mLMWorker.isRunning())
    {
        return false;
    }

    mTerminate = false;


//    if(mObservedData.getValue())
//    {
//        mObservedData.getValueReference()->clear();
//    }
//
//    if(mModelData.getValue())
//    {
//        mModelData.getValue()->clear();
//    }
//
//    if(mResidualsData.getValue())
//    {
//        mResidualsData.getValueReference()->clear();
//    }

    mInputParameterList.getValueReference().clear();
    mOutputParameterList.getValueReference().clear();
    mObservedDataSelectionList.getValueReference().clear();
    mModelDataSelectionList.getValueReference().clear();
    return true;
}

string LM::getSBML()
{
    return mSBML.getValue();
}

string LM::getResult()
{
    stringstream msg;
    Parameters& pars = mOutputParameterList.getValueReference();

    for(int i = 0; i < pars.count(); i++)
    {
        msg<<pars[i]->asString();
    }
    msg<<"Norm: "<<mNorm.getValue();
    return msg.str();
}

bool LM::execute(bool inThread)
{
    try
    {
        Log(lInfo)<<"Executing the LM plugin";
        mLMWorker.start(inThread);
        return true;
    }
    catch(const Exception& ex)
    {
        Log(lError) << "There was a problem in the execute of the LMFIT plugin: " << ex.getMessage();
        throw(ex);
    }
    catch(...)
    {
        Log(lError) << "There was an unknown problem in the execute of the LMFIT plugin.";
        throw("There was an unknown problem in the execute of the LMFIT plugin.");
    }
}

// Plugin factory function
LM* plugins_cc createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
    return new LM(aRR);
}

const char* plugins_cc getImplementationLanguage()
{
    return "CPP";
}

}

