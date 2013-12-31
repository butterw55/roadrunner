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
mLMFit(                     "LMFit",                                "Fit Model Propertys Using the Levenberg-Marquardt Algorithm"),    //The 'capability'
mSBML(                      "<none>",               "SBML",                                 "SBML document as a string. Model to be used in the fitting"),
mExperimentalData(          RoadRunnerData(),       "ExperimentalData",                     "Data object holding Experimental data: Provided by client"),
mModelData(                 RoadRunnerData(),       "FittedData",                           "Data object holding model data: Handed to client"),
mResidualsData(             RoadRunnerData(),       "Residuals",                            "Data object holding residuals: Handed to client"),
mInputParameterList(         Properties(),          "InputParameterList",                    "List of parameters to fit"),
mOutputParameterList(        Properties(),          "OutputParameterList",                   "List of parameters that was fittedt"),
mExperimentalDataSelectionList( StringList(),       "ExperimentalDataSelectionList",        "Experimental data selection list"),
mModelDataSelectionList(    StringList(),           "FittedDataSelectionList",              "Fitted data selection list"),
mNorm(                      -1.0,                   "Norm",                                 "Norm of fitting. An estimate of goodness of fit"),
mNrOfIter(                  -1,                     "NrOfIter",                             "Number of iterations"),
mLMWorker(*this),
mLMData(mLMWorker.mLMData),

//The following Properties are the members of lmfits control_structure.
//Changing their default values may be needed depending on the problem.
ftol(                      LM_USERTOL,              "ftol"       ,                           " Relative error desired in the sum of squares. "),
xtol(                      LM_USERTOL,              "xtol"       ,                           " Relative error between last two approximations. "),
gtol(                      LM_USERTOL,              "gtol"       ,                           " Orthogonality desired between fvec and its derivs. "),
epsilon(                   LM_USERTOL,              "epsilon"    ,                           " Step used to calculate the jacobian. "),
stepbound(                 100.,                    "stepbound"  ,                           " Initial bound to steps in the outer loop. "),
patience(                   100,                     "patience"    ,                           " Maximum number of iterations. "),
scale_diag(                 1,                       "scale_diag" ,                           " UNDOCUMENTED, TESTWISE automatical diag rescaling? ")
//verbosity(                  0,                       "verbosity" ,                           " OR'ed to produce more noise ")

{
    mVersion = "1.0";
    //Setup the plugins capabilities
    mLMFit.addProperty(&mSBML);
    mLMFit.addProperty(&mExperimentalData);
    mLMFit.addProperty(&mModelData);
    mLMFit.addProperty(&mResidualsData);
    mLMFit.addProperty(&mInputParameterList);
    mLMFit.addProperty(&mOutputParameterList);
    mLMFit.addProperty(&mExperimentalDataSelectionList);
    mLMFit.addProperty(&mModelDataSelectionList);
    mLMFit.addProperty(&mNorm);
    mLMFit.addProperty(&mNrOfIter);

    //Add the lmfit parameters
    mLMFit.addProperty(&ftol);
    mLMFit.addProperty(&xtol);
    mLMFit.addProperty(&gtol);
    mLMFit.addProperty(&epsilon);
    mLMFit.addProperty(&stepbound);
    mLMFit.addProperty(&patience);
    mLMFit.addProperty(&scale_diag);
//    mLMFit.addProperty(&verbosity);

    mProperties.add(mLMFit);
    //Allocate model and Residuals data
    mResidualsData.setValue(new RoadRunnerData());
    mModelData.setValue(new RoadRunnerData());
    
    mHint ="Parameter fitting using the Levenberg-Marquardt algorithm";
    mDescription="The Levenberg-Marquardt plugin is used to fit a proposed \
SBML models parameters to experimental data. \
The current implementation is based on the lmfit C library by Joachim Wuttke. \
The Plugin has numerous parameters for fine tuning the algorithm. See the embedded PDF for more information. \
";
    //The function below assigns property descriptions
    assignPropertyDescriptions();
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

StringList LM::getExperimentalDataSelectionList()
{
    return mExperimentalDataSelectionList.getValue();
}

string LM::getStatus()
{
    stringstream msg;
    msg<<Plugin::getStatus();    
    
    msg<<"\nFitting parameters: "<<mInputParameterList;
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


//    if(mExperimentalData.getValue())
//    {
//        mExperimentalData.getValueReference()->clear();
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
    mExperimentalDataSelectionList.getValueReference().clear();
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
    Properties& pars = mOutputParameterList.getValueReference();

    for(int i = 0; i < pars.count(); i++)
    {
        msg<<pars[i]->getName()<<" = "<< pars[i]->getValueAsString()<<"\n";
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

void LM::assignPropertyDescriptions()
{
    stringstream s;
s << "The SBML property should be assigned the (XML) \
text that defines the SBML model that is used to fit parameters.";
mSBML.setDescription(s.str()); 
s.str("");

s << "Experimental data contains the data to be used for fitting input.";
mExperimentalData.setDescription(s.str());          
s.str("");

s << "Model data is calculated after the fitting algorithm finishes. It uses the obtained model parameters as input.";
mModelData.setDescription(s.str());                  
s.str("");

s << "Residuals data contains the differencies between the Experimental data and the ModelData.";
mResidualsData.setDescription(s.str());               
s.str("");

s << "The input parameter list holds the parameters, and their initial values that are to be fitted, e.g. k1, k2. \
The input parameters are properties of the input SBML model";
mInputParameterList.setDescription(s.str());           
s.str("");

s << "The output parameter list holds the resulting fitted parameter(s)";
mOutputParameterList.setDescription(s.str());           
s.str("");

s << "The data input may contain multiple columns of data. The Experimental data selection list \
should contain the columns in the input data that is intended to be used in the fitting.";
mExperimentalDataSelectionList.setDescription(s.str());  
s.str("");

s << "The model data selection list contains the selections for which model data will be genereated.  \
Model data can only be generated for selections present in the experimental data selectionlist.";
mModelDataSelectionList.setDescription(s.str());
s.str("");

s << "The norm is an output variable indicating the goodness of fit. The smaller value, the better fit.";
mNorm.setDescription(s.str());    
s.str("");

s << "The number of iterations wil hold the number of iterations of the internal fitting routine.";
mNrOfIter.setDescription(s.str()); 
s.str("");

    //Add the lmfit parameters
s << "ftol is a nonnegative input variable. Termination occurs when \
both the actual and predicted relative reductions in the sum \
of squares are at most ftol. Therefore, ftol measures the \
relative error desired in the sum of squares.";
    ftol.setDescription(s.str()); 
s.str("");

s << "xtol is a nonnegative input variable. Termination occurs when \
the relative error between two consecutive iterates is at \
most xtol. Therefore, xtol measures the relative error desired \
in the approximate solution.";
xtol.setDescription(s.str());      
s.str("");

s << "gtol is a nonnegative input variable. Termination occurs when \
the cosine of the angle between fvec and any column of the \
jacobian is at most gtol in absolute value. Therefore, gtol \
measures the orthogonality desired between the function vector \
and the columns of the jacobian.";
    gtol.setDescription(s.str());   
s.str("");

s << "Step used to calculate the Jacobian.";
    epsilon.setDescription(s.str()); 
s.str("");

s << "Initial bound to steps in the outer loop.";
    stepbound.setDescription(s.str());
s.str("");

s << "Maximum number of iterations.";
    patience.setDescription(s.str());
s.str("");

s << "UNDOCUMENTED, TESTWISE automatical diag rescaling? ";
    scale_diag.setDescription(s.str());
s.str("");

//s << "ORed to produce more noise.";
//    verbosity.setDescription(s.str());
//s.str("");

}
}

