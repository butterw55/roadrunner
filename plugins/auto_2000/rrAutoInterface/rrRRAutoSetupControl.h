#ifndef rrRRAutoSetupControlH
#define rrRRAutoSetupControlH
#include "rrAutoInputConstants.h"
#include "rrRRAutoExporter.h"
//---------------------------------------------------------------------------

namespace rrauto
{

class RRAuto;
//Corresponds to Frank Bergmann SetupControl.cs, UI class

class RRA_DECLSPEC SetupControl
{
    friend RRAuto;

    public:
                        SetupControl();
        string          getConstantsAsString();

    protected:
        InputConstants  mInputConstants;
        bool            mCalculateSteadyState;
        bool            mRunContinuation;
        bool            mDirectionPositive;
};

}
#endif
