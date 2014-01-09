#pragma hdrstop
#include <sstream>
#include "rrStringUtils.h"
#include "rrPropertyBase.h"
//#include "rrProperty.h"
//---------------------------------------------------------------------------

using namespace std;
namespace rrp
{

PropertyBase::PropertyBase(const string& type, const string& name, const string& hint, const string& descr)
:
mName(name),
mHint(hint),
mDescription(descr),
mType(type)
{}

PropertyBase::~PropertyBase()
{}

PropertyBase::PropertyBase(const PropertyBase& pp)
{
    //We could assert here, assigning incompatible types    
    mName = pp.mName;
    mType = pp.mType;
    mHint = pp.mHint;
}

PropertyBase& PropertyBase::operator=(const PropertyBase& rhs)
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

string PropertyBase::getType() const
{
    return mType;
}

string PropertyBase::getName() const
{
    return mName;
}

string PropertyBase::getHint() const
{
    return mHint;
}

string PropertyBase::getDescription() const
{
    return mDescription;
}

void PropertyBase::setDescription(const string& descr)
{
    mDescription = descr;
}

void PropertyBase::setHint(const string& hint)
{
    mHint = hint;
}

void* PropertyBase::getValueHandle()
{
    return NULL;
}

string PropertyBase::getValueAsString() const
{
    stringstream val;
    val<<"The type:"<<getType()<<" do not have a string representation.";
    return val.str();
}

ostream& operator<<(ostream& stream, const PropertyBase& outMe)
{
    stream<<outMe.asString();   //virtual friend idiom
    return stream;
}

string PropertyBase::asString() const
{
    stringstream val;
    val<<"Name: "<<     mName<<endl;
    val<<"Type: "<<     getType()<<endl;
    val<<"Value: "<<    getValueAsString()<<endl;
    val<<"Hint: "<<     mHint<<endl;
    return val.str();
}



}
