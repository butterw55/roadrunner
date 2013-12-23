#pragma hdrstop
#include <sstream>
#include <iomanip>
#include "rrUtils.h"
#include "rrPlugin.h"
#include "rrProperty.h"
//---------------------------------------------------------------------------

namespace rrp
{
using namespace std;
Plugin::Plugin( const string&           name,
                const string&           category,
                RoadRunner*             aRR,
                const string&           language,
                const PluginManager*    pm)
:
mName(name),
mAuthor("Totte Karlsson"),
mCategory(category),
mDescription("<none>"),
mHint("<none>"),
mVersion("0.0"),
mCopyright("Totte Karlsson, Herbert Sauro, Systems Biology, UW 2012-2014"),
mRR(aRR),
mWorkStartedEvent(NULL),
mWorkProgressEvent(NULL),
mWorkFinishedEvent(NULL),
mProperties(),//name, category),
mImplementationLanguage(language),
mPM(pm),
mTerminate(false),
mIsWorking(false)
{}

Plugin::~Plugin()
{}

void Plugin::terminate()
{
    if(!mIsWorking)
    {
        Log(lWarning) << "Can't terminate a non working plugin..";
        return;
    }
    mTerminate = true;
}

bool Plugin::isBeingTerminated()
{
    return mTerminate;
}

bool Plugin::wasTerminated()
{
    if(mTerminate)
    {
        if(mIsWorking == false)
        {
            return true;
        }
        else
        {
            //Still working...
            return false;
        }
    }
    return false;
}

bool Plugin::assignRoadRunnerInstance(RoadRunner* rr)
{
    mRR = rr;
    return mRR ? true : false;
}

RoadRunner* Plugin::getRoadRunnerInstance()
{
    return mRR;
}

bool Plugin::resetPlugin()
{
    //Do whats needed in descendants
    return true;
}

string Plugin::getPluginPropertiesAsXML()
{
    return mProperties.asXML();
}

bool Plugin::assignInput(void* userData)
{
    //Do whats needed in descendants
    mClientData = userData;
    return true;
}

bool Plugin::assignOnStartedEvent(PluginEvent startedFnc, void* userData1, void* userData2)
{
    mWorkStartedEvent = startedFnc;
    mWorkStartedData1 = userData1;
    mWorkStartedData2 = userData2;
    return true;
}

bool Plugin::assignOnProgressEvent(PluginEvent progress, void* userData1, void* userData2)
{
    mWorkProgressEvent = progress;
    mWorkProgressData1 = userData1;
    mWorkProgressData2 = userData2;
    return true;
}

bool Plugin::assignOnFinishedEvent(PluginEvent endFnc, void* userData1, void* userData2)
{
    mWorkFinishedEvent = endFnc;
    mWorkFinishedData1 = userData1;
    mWorkFinishedData2 = userData2;
    return true;
}

bool Plugin::isWorking()
{
    return false;
}

void Plugin::setLibraryName(const string& libName)
{
    mLibraryName = libName;
}

bool Plugin::setProperty(const string& nameOf, const char* value)
{
    if(!mProperties.count())
    {
        return false;
    }

    string val(value);
    return mProperties.setProperty(nameOf, val);
}

bool Plugin::setProperty(const string& nameOf, const char* value, Capability& capability)
{
    //Go trough the parameter container and look for parameter
    for(int i = 0; i < capability.nrOfProperties(); i++)
    {
        PropertyBase* aPar = const_cast<PropertyBase*>( &(capability[i]) );

        if(aPar->getName() == nameOf)
        {
//            if(dynamic_cast< Property<int>* >(aProperty))
//            {
//                Property<int> *aIntPar = dynamic_cast< Property<int>* >(aProperty);
//                int aVal = rr::ToInt(value);
                aPar->setValueFromString( value);
//                return true;
            //}
        }
    }
    return false;
}


string Plugin::getName()
{
    return mName;
}

string Plugin::getLibraryName()
{
    return mLibraryName;
}

string Plugin::getAuthor()
{
    return mAuthor;
}

string Plugin::getCategory()
{
    return mCategory;
}

string Plugin::getDescription()
{
    return mDescription;
}

string Plugin::getHint()
{
    return mHint;
}

string Plugin::getVersion()
{
    return mVersion;
}

string Plugin::getCopyright()
{
    return mCopyright;
}

//Overload in descendants and add info there.
string Plugin::getStatus()
{
    stringstream msg;
    msg<<"The following is a status message for Plugin: "<<mName<<"\n";
    return msg.str();
}

//Obs. subclasses may over ride this function and add more info
string Plugin::getInfo()
{
    stringstream msg;
    msg<<setfill('.');
    msg<<setw(30)<<left<<"Name"<<mName<<"\n";
    msg<<setw(30)<<left<<"Author"<<mAuthor<<"\n";
    msg<<setw(30)<<left<<"Category"<<mCategory<<"\n";
    msg<<setw(30)<<left<<"Description"<<mDescription<<"\n";
    msg<<setw(30)<<left<<"Hint"<<mHint<<"\n";
    msg<<setw(30)<<left<<"Version"<<mVersion<<"\n";
    msg<<setw(30)<<left<<"Copyright"<<mCopyright<<"\n";
    return msg.str();
}

string Plugin::getExtendedInfo()
{
    stringstream msg;
    msg<<getInfo();
    msg<<"\nProperty Info\n";
    msg<<(*getProperties());
    return msg.str();
}

unsigned int Plugin::getPDFManualByteSize()
{
    return 0;
}

unsigned char* Plugin::getManualAsPDF() const
{
    return NULL;
}

StringList Plugin::getPropertyNames()
{
    StringList names;        
    //For now, if capName is "" return all
    for(int i = 0; i < mProperties.count(); i++)
    {
        Properties* paras = mProperties[i]->getProperties();
        
        names.Append(paras->getNames());
    }    

    return names;
}

Properties* Plugin::getProperties()
{
    //For now, if capName is "" return all
    for(int i = 0; i < mProperties.count(); i++)
    {
        return mProperties[i]->getProperties();
    }    

    return NULL;
}

PropertyBase* Plugin::getProperty(const string& para, const string& capability)
{
    //If capability string is empty, search all capabilites
    if(capability.size())
    {
        Capability* cap = mProperties.get(capability);
        return cap ? cap->getProperty(para) : NULL;
    }
    else    //Search all capabilities
    {
        for(int i = 0; i < mProperties.count(); i++)
        {
            if(mProperties[i]->getProperty(para))
            {
                return mProperties[i]->getProperty(para);
            }
        }
    }
    return NULL;
}

PropertyBase* Plugin::getProperty(const string& para, Capability& capability)
{
    return capability.getProperty(para);
}

//Capability* Plugin::getCapability(const string& name)
//{
//    for(int i = 0; i < mProperties.count(); i++)
//    {
//        if(mProperties[i]->getName() == name)
//        {
//            return (mProperties[i]);
//        }
//    }
//    return NULL;
//}

string Plugin::getResult()
{
    return "This plugin don't have any result..";
}

}

