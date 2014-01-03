#pragma hdrstop
#include "libxml/tree.h"
#include "libxml/xpath.h"
#include "rrException.h"
#include "rrPropertyBase.h"
#include "rrProperty.h"
#include "rrLogger.h"
#include "rrCapabilities.h"
#include "rrCapability.h"

namespace rrp
{
using namespace std;
using rr::StringList;

Capabilities::Capabilities()//const string& name, const string& description)
//:
//mName(name),
//mDescription(description)
{}

Capabilities::~Capabilities()
{}

void Capabilities::add(Capability& capability)
{
    mProperties.push_back(&capability);
}

void Capabilities::clear()
{
    mProperties.clear();
}

Capability* Capabilities::operator[](int i)
{
    if(mProperties.size())
    {
        return (mProperties[i]);
    }
    return NULL;
}

Capability* Capabilities::get(const string& capName)
{
    for(int i = 0; i < count(); i++)
    {
        Capability* aCap = (mProperties[i]);
        if(aCap && aCap->getName() == capName)
        {
            return aCap;
        }
    }
    return NULL;
}

StringList Capabilities::asStringList()
{
    StringList caps;

    //Add capabilitys
    for(int i = 0; i < count(); i++)
    {
        Capability& aCapability = *(mProperties[i]);
        caps.add(aCapability.getName());
    }

    return caps;
}

string Capabilities::info() const
{
    stringstream st;
    vector<Capability*>::iterator iter;

    for(iter = mProperties.begin(); iter != mProperties.end(); iter++)
    {
        Capability* aCap = (*iter);
        st<<(*aCap);
    }
    return st.str();
}

u_int Capabilities::count()
{
    return mProperties.size();
}

//Not giving a capability name, search for first parameter with name 'name'
bool Capabilities::setProperty(const string& name, const string& value)
{
    for(int i = 0; i < count(); i++)
    {
        Capability* capability = mProperties[i];

        if(!capability)
        {
            return false;
        }

        Properties* paras = capability[i].getProperties();

        if(paras)
        {
            PropertyBase* aProperty = paras->getProperty(name);
            if(aProperty)
            {
                aProperty->setValueFromString(value);
                return true;
            }
        }
    }

    return false;
}

string Capabilities::asXML()
{
    xmlDocPtr doc = NULL;           /* document pointer */
    xmlNodePtr root_node = NULL;
    LIBXML_TEST_VERSION;

    doc = xmlNewDoc(BAD_CAST "1.0");
    xmlKeepBlanksDefault(0);
    root_node = xmlNewNode(NULL, BAD_CAST "pluginProperties");
    xmlDocSetRootElement(doc, root_node);

    //Add capabilitys
    for(int i = 0; i < count(); i++)
    {
        Capability& aCapability = *(mProperties[i]);
        xmlNodePtr parameters = xmlNewChild(root_node, NULL, BAD_CAST "parameters",NULL);

        //Add parameters within each capability
        for(int j = 0; j < aCapability.nrOfProperties(); j++)
        {
            xmlNodePtr paraNode = xmlNewChild(parameters, NULL, BAD_CAST "para", NULL);

            PropertyBase* parameter = const_cast<PropertyBase*>(&(aCapability[j]));
            xmlNewProp(paraNode, BAD_CAST "name",           BAD_CAST parameter->getName().c_str());
            xmlNewProp(paraNode, BAD_CAST "value",          BAD_CAST parameter->getValueAsString().c_str());            
            xmlNewProp(paraNode, BAD_CAST "type",           BAD_CAST parameter->getType().c_str());
            xmlNewProp(paraNode, BAD_CAST "hint",           BAD_CAST parameter->getHint().c_str());
            xmlNewProp(paraNode, BAD_CAST "description",    BAD_CAST parameter->getDescription().c_str());
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

ostream& operator<<(ostream& stream, const Capabilities& caps)
{
//    stream<<"Capability: "<<caps.mName<<endl;
//    stream<<"Description: "<<caps.mDescription<<endl;
    stream<<caps.info();
    return stream;
}

}
