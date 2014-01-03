#ifndef rrAutoWorkerH
#define rrAutoWorkerH
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrRoadRunner.h"
#include "rrOSSpecifics.h"
#include "rrpExporter.h"
#include "../rrAutoInterface/rrAutoData.h"
#include "auto_utils.h"
//---------------------------------------------------------------------------

namespace rrauto
{
class RRAuto;
}

namespace autoplugin
{
typedef void (*ThreadCB)(void*, void*);

class AutoPlugin;

using rr::RoadRunnerData;

class AutoWorker : public Poco::Runnable
{
    public:
                                    AutoWorker(AutoPlugin& host);
                                   ~AutoWorker();
//        void                        assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData);
        bool                        start(bool runInThread = false);
        void                        run();
        bool                        isRunning();

    protected:
        Poco::Thread                mThread;

//        //Callbacks
//        ThreadCB                    threadEnterCB;
//        ThreadCB                    threadExitCB;
//        void*                       mUserData;       //Used for plugin callbacks..

        AutoPlugin&                 mTheHost;
        rrauto::RRAuto&             mRRAuto;        //This class communicate with auto
        AutoData&                   mAutoData;


        bool                        setup();
        RoadRunnerData              createModelData();
        RoadRunnerData              createResidualsData();
};
}
#endif
