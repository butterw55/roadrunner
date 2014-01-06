#pragma hdrstop
#include "libxml/tree.h"
#include "libxml/xpath.h"
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
    this->mProperties = cpyMe.mProperties;
    this->mPropertiesIter = this->mProperties.begin();
}

bool Properties::clear()
{
    if(mCanClientClearList == false)
    {
        return false;
    }

    for(int i = 0; i < mProperties.size(); i++)
    {
        if(mProperties[i].second == true)
        {
            delete mProperties[i].first;
        }
    }
    mProperties.clear();
    return true;
}

void Properties::add(PropertyBase* me, bool own)
{
    mProperties.push_back( pair<PropertyBase*, bool>(me, own) );
}

StringList Properties::getNames() const
{
    StringList list;
    for(int i = 0; i < count(); i++)
    {
        list.add(mProperties[i].first->getName());
    }
    return list;
}

const PropertyBase* Properties::operator[](const int& i) const
{
    return mProperties[i].first;
}

PropertyBase* Properties::operator[](const int& i)
{
    return mProperties[i].first;
}

u_int Properties::count() const
{
    return mProperties.size();
}

PropertyBase* Properties::getProperty(const string& paraName)
{
    for(int i = 0; i < count(); i++)
    {
        if(paraName == mProperties[i].first->getName())
        {
            return mProperties[i].first;
        }
    }
    return NULL;
}

PropertyBase* Properties::getFirst()
{
    mPropertiesIter = mProperties.begin();
    if(mPropertiesIter != mProperties.end())
    {
        return (*mPropertiesIter).first;
    }
    return NULL;
}

PropertyBase* Properties::getCurrent()
{
    if(mPropertiesIter != mProperties.end())
    {
        return (*mPropertiesIter).first;
    }
    return NULL;
}

PropertyBase* Properties::getNext()
{
    mPropertiesIter++;
    if(mPropertiesIter != mProperties.end())
    {
        return (*mPropertiesIter).first;
    }
    return NULL;
}

PropertyBase* Properties::getPrevious()
{
    mPropertiesIter--;
    if(mPropertiesIter != mProperties.end())
    {
        return (*mPropertiesIter).first;
    }
    return NULL;
}

bool Properties::setProperty(const string& name, const string& value)
{
    PropertyBase* aProperty = getProperty(name);

    if(!aProperty)
    {
        return false;
    }
    aProperty->setValueFromString(value);
    return true;
}

string Properties::asXML()
{
    xmlDocPtr doc = NULL;           /* document pointer */
    xmlNodePtr root_node = NULL;
    LIBXML_TEST_VERSION;

    doc = xmlNewDoc(BAD_CAST "1.0");
    xmlKeepBlanksDefault(0);
    root_node = xmlNewNode(NULL, BAD_CAST "pluginProperties");
    xmlDocSetRootElement(doc, root_node);

    for(int i = 0; i < count(); i++)
    {
        xmlNodePtr parameters = xmlNewChild(root_node, NULL, BAD_CAST "properties",NULL);

        //Add parameters within each capability
        for(int j = 0; j < count(); j++)
        {
            xmlNodePtr paraNode = xmlNewChild(parameters, NULL, BAD_CAST "para", NULL);

            //Proiperty by property
            PropertyBase* property = const_cast<PropertyBase*>((mProperties[j].first));

            xmlNewProp(paraNode, BAD_CAST "name",           BAD_CAST property->getName().c_str());
            xmlNewProp(paraNode, BAD_CAST "value",          BAD_CAST property->getValueAsString().c_str());
            xmlNewProp(paraNode, BAD_CAST "type",           BAD_CAST property->getType().c_str());
            xmlNewProp(paraNode, BAD_CAST "hint",           BAD_CAST property->getHint().c_str());
            xmlNewProp(paraNode, BAD_CAST "description",    BAD_CAST property->getDescription().c_str());
        }
    }

    xmlChar *xmlbuff;
    int buffersize;
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);

    Log(rr::lDebug)<<(char*) xmlbuff;

    string xml = xmlbuff != NULL ? string((char*) xmlbuff) : string("");
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return xml;
}


ostream& operator<<(ostream& stream, const Properties& props)
{
    for(int i = 0; i < props.count(); i++)
    {
        PropertyBase* bProp = props.mProperties[i].first;
        stream << *(bProp);
        
    }
    return stream;
}

}