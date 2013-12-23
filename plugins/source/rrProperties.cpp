#pragma hdrstop
#include "rrLogger.h"
#include "rrProperties.h"

namespace rrp
{
using namespace rr;

Properties::Properties(bool canClientClear)
    :mCanClientClearList(canClientClear)
{}

                                        
Properties::~Properties()
{
    clear();
}

Properties::Properties(const Properties& cpyMe)
{
    this->mCanClientClearList = true;
    this->mParas = cpyMe.mParas;
    this->mParasIter = this->mParas.begin();
}

bool Properties::clear()
{
    if(mCanClientClearList == false)
    {
        return false;
    }

    for(int i = 0; i < mParas.size(); i++)
    {
        if(mParas[i].second == true)
        {
            delete mParas[i].first;
        }
    }
    mParas.clear();
    return true;
}

void Properties::add(PropertyBase* me, bool own)
{
    mParas.push_back( pair<PropertyBase*, bool>(me, own) );
}

StringList Properties::getNames() const
{
    StringList list;
    for(int i = 0; i < count(); i++)
    {
        list.add(mParas[i].first->getName());
    }
    return list;
}

const PropertyBase* Properties::operator[](const int& i) const
{
    return mParas[i].first;
}

PropertyBase* Properties::operator[](const int& i)
{
    return mParas[i].first;
}

u_int Properties::count() const
{
    return mParas.size();
}

PropertyBase* Properties::getProperty(const string& paraName)
{
    for(int i = 0; i < count(); i++)
    {
        if(paraName == mParas[i].first->getName())
        {
            return mParas[i].first;
        }
    }
    return NULL;
}

PropertyBase* Properties::getFirst()
{
    mParasIter = mParas.begin();
    if(mParasIter != mParas.end())
    {
        return (*mParasIter).first;
    }
    return NULL;
}

PropertyBase* Properties::getCurrent()
{
    if(mParasIter != mParas.end())
    {
        return (*mParasIter).first;
    }
    return NULL;
}

PropertyBase* Properties::getNext()
{
    mParasIter++;
    if(mParasIter != mParas.end())
    {
        return (*mParasIter).first;
    }
    return NULL;
}

PropertyBase* Properties::getPrevious()
{
    mParasIter--;
    if(mParasIter != mParas.end())
    {
        return (*mParasIter).first;
    }
    return NULL;
}

ostream& operator<<(ostream& stream, const Properties& paras)
{
    for(int i = 0; i < paras.count(); i++)
    {
        
        stream << paras.mParas[i].first;
        
    }
    return stream;
}

}
