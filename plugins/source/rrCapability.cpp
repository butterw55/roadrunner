#pragma hdrstop
#include <sstream>
#include "rrProperty.h"
#include "rrCapability.h"

using namespace std;

namespace rrp
{

Capability::Capability(const string& name, const string& description)
:
mName(name),
//mMethod(method),
mDescription(description),
mProperties(false)
{}

Capability::~Capability()
{}

Capability::Capability(const Capability& from)
:
mName(from.mName),
//mMethod(from.mMethod),
mDescription(from.mDescription),
mProperties(from.mProperties)
{}

void Capability::setup(const string& name, const string& descr)
{
    mName = name;
//    mMethod = method;
    mDescription = descr;
}

Properties* Capability::getProperties()
{
    return &mProperties;
}

PropertyBase& Capability::operator[](const int& i)
{

    return *(mProperties[i]);
}

string Capability::getName() const
{
    return mName;
}

string Capability::getDescription() const
{
    return mDescription;
}

//string Capability::getMethod() const
//{
//    return mMethod;
//}

u_int Capability::nrOfProperties() const
{
    return mProperties.count();
}

void Capability::addProperty(PropertyBase* me)
{
    mProperties.add(me);
}

string Capability::asString()  const
{
    stringstream caps;
    caps<<"Name: " << mName <<endl;
//    caps<<"Method: " << mMethod<<endl;
    caps<<"Description: " << mDescription<<endl;

    for(int i = 0; i < nrOfProperties(); i++)
    {
        caps << *(mProperties[i]) <<endl;
    }
    return caps.str();
}

PropertyBase* Capability::getProperty(const string& paraName)
{
    for(u_int i = 0; i < mProperties.count(); i++)
    {
        if(mProperties[i] && mProperties[i]->getName() == paraName)
        {
            return mProperties[i];
        }
    }
    return NULL;
}

ostream& operator <<(ostream& os, const Capability& caps)
{
    os<<"Properties for capability: "<<caps.mName<<"\n";

    for(u_int i = 0; i < caps.nrOfProperties(); i++)
    {
        os<< *(caps.mProperties[i]);

        if(i < caps.nrOfProperties() -1 )
        {
            os<<"  "<<endl;
        }
    }
    return os;
}

}
