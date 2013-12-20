#pragma hdrstop
#include <sstream>
#include "rrStringUtils.h"
#include "rrPluginParameter.h"
#include "rrParameter.h"
//---------------------------------------------------------------------------

using namespace std;
namespace rrp
{

PluginParameter::PluginParameter(const string& type, const string& name, const string& hint)
:
mName(name),
mHint(hint),
mType(type)
{}

PluginParameter::~PluginParameter()
{}

PluginParameter::PluginParameter(const PluginParameter& pp)
{
    //We could assert here, assigning incompatible types    
    mName = pp.mName;
    mType = pp.mType;
    mHint = pp.mHint;
}

PluginParameter& PluginParameter::operator=(const PluginParameter& rhs)
{
    if((this) == &rhs)
    {
        return *(this);
    }

    mName = rhs.mName;
    mType = rhs.mType;
    mHint = rhs.mHint;
    return *(this);
}

string PluginParameter::getType() const
{
    return mType;
}

string PluginParameter::getName() const
{
    return mName;
}

string PluginParameter::getHint() const
{
    return mHint;
}

string PluginParameter::getDescription() const
{
    return mDescription;
}

void PluginParameter::setDescription(const string& descr)
{
    mDescription = descr;
}

void PluginParameter::setHint(const string& hint)
{
    mHint = hint;
}

void* PluginParameter::getValueHandle()
{
    return NULL;
}

string PluginParameter::getValueAsString() const
{
    stringstream val;
    val<<"The type:"<<getType()<<" do not have a string representation.";
    return val.str();
}

ostream& operator<<(ostream& stream, const PluginParameter& outMe)
{
    stream<<outMe.asString();   //virtual friend idiom
    return stream;
}

string PluginParameter::asString() const
{
    stringstream val;
    val<<"Name: "<<     mName<<endl;
    val<<"Type: "<<     getType()<<endl;
    val<<"Value: "<<    getValueAsString()<<endl;
    val<<"Hint: "<<     mHint<<endl;
    return val.str();
}



}
