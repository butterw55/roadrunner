#ifndef rrPluginUtilsH
#define rrPluginUtilsH
#include <vector>
#include <string>
#include "rrpExporter.h"
#include "rrStringList.h"
#include "rrSelectionRecord.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------

namespace rrp
{
    
using std::vector;
using std::string;
RRP_DECLSPEC rr::StringList      getRecordsAsStrings(const vector<rr::SelectionRecord>& folder);

//TODO: Clean up the following template functions
//These templates should be able to replace all 'format' functions above.. TEST
template <class T>
string formatN(const string& src, const T& arg1)
{
    string newString(src);
    string tok1("{0}");
    newString = substituteN(newString, tok1, arg1);
    return newString;
}

template <class A, class B>
string formatN(const string& src, const A& arg1, const B& arg2)
{
    string newString(src);
    string tok1("{0}"), tok2("{1}");

    newString = substituteN(newString, tok1, arg1);
    newString = substituteN(newString, tok2, arg2);
    return newString;
}

template <class A, class B, class C>
string formatN(const string& src, const A& arg1, const B& arg2, const C& arg3)
{
    string newString(src);
    string tok1("{0}"), tok2("{1}"), tok3("{2}");

    newString = substituteN(newString, tok1, arg1, -1);
    newString = substituteN(newString, tok2, arg2, -1);
    newString = substituteN(newString, tok3, arg3, -1);
    return newString;
}

template <class A, class B, class C, class D>
string formatN(const string& src, const A& arg1, const B& arg2, const C& arg3, const D& arg4)
{
    string newString(src);
    string  tok1("{0}"), tok2("{1}"), tok3("{2}"),
            tok4("{3}");

    newString = substituteN(newString, tok1, arg1, -1);
    newString = substituteN(newString, tok2, arg2, -1);
    newString = substituteN(newString, tok3, arg3, -1);
    newString = substituteN(newString, tok4, arg4, -1);
    return newString;
}

template <class A, class B, class C, class D, class E>
string formatN(const string& src, const A& arg1, const B& arg2, const C& arg3, const D& arg4, const E& arg5)
{
    string newString(src);
    string  tok1("{0}"), tok2("{1}"), tok3("{2}"),
            tok4("{3}"), tok5("{4}");

    newString = substituteN(newString, tok1, arg1);
    newString = substituteN(newString, tok2, arg2, -1);
    newString = substituteN(newString, tok3, arg3, -1);
    newString = substituteN(newString, tok4, arg4, -1);
    newString = substituteN(newString, tok5, arg5, -1);
    return newString;
}

template <class A, class B, class C, class D, class E, class F>
string formatN(const string& src, const A& arg1, const B& arg2, const C& arg3, const D& arg4, const E& arg5, const F& arg6)
{
    string newString(src);
    string  tok1("{0}"), tok2("{1}"), tok3("{2}"),
            tok4("{3}"), tok5("{4}"), tok6("{5}");

    newString = substituteN(newString, tok1, arg1, -1);
    newString = substituteN(newString, tok2, arg2, -1);
    newString = substituteN(newString, tok3, arg3, -1);
    newString = substituteN(newString, tok4, arg4, -1);
    newString = substituteN(newString, tok5, arg5, -1);
    newString = substituteN(newString, tok6, arg6, -1);

    return newString;
}

template <class A, class B, class C, class D, class E, class F, class G>
string formatN(const string& src, const A& arg1, const B& arg2, const C& arg3, const D& arg4, const E& arg5, const F& arg6, const G& arg7)
{
    string newString(src);
    string  tok1("{0}"), tok2("{1}"), tok3("{2}"),
            tok4("{3}"), tok5("{4}"), tok6("{5}"),
            tok7("{6}");

    newString = substituteN(newString, tok1, arg1, -1);
    newString = substituteN(newString, tok2, arg2, -1);
    newString = substituteN(newString, tok3, arg3, -1);
    newString = substituteN(newString, tok4, arg4, -1);
    newString = substituteN(newString, tok5, arg5, -1);
    newString = substituteN(newString, tok6, arg6, -1);
    newString = substituteN(newString, tok7, arg7, -1);

    return newString;
}

template <class A, class B, class C, class D, class E, class F, class G, class H>
string formatN(const string& src, const A& arg1, const B& arg2, const C& arg3, const D& arg4, const E& arg5, const F& arg6, const G& arg7, const H& arg8)
{
    string newString(src);
    string  tok1("{0}"), tok2("{1}"), tok3("{2}"),
            tok4("{3}"), tok5("{4}"), tok6("{5}"),
            tok7("{6}"), tok8("{7}");

    newString = substituteN(newString, tok1, arg1, -1);
    newString = substituteN(newString, tok2, arg2, -1);
    newString = substituteN(newString, tok3, arg3, -1);
    newString = substituteN(newString, tok4, arg4, -1);
    newString = substituteN(newString, tok5, arg5, -1);
    newString = substituteN(newString, tok6, arg6, -1);
    newString = substituteN(newString, tok7, arg7, -1);
    newString = substituteN(newString, tok8, arg8, -1);

    return newString;
}

template <class T>
inline string substituteN(const string& src, const string& thisOne, const T& withThisOne, const int& howMany= -1)
{
    return rr::substitute(src, thisOne, rr::toString(withThisOne), howMany);
}

template<>
inline string substituteN<double>(const string& src, const string& thisOne, const double& withThisOne, const int& howMany)
{
    return rr::substitute(src, thisOne, rr::toString(withThisOne, "%G"), howMany);
}
}
#endif
