//Link to RoadRunner
#if defined(CG_IDE) 
    #if defined(STATIC_RR_AUTO)
        #pragma comment(lib, "roadrunner-static.lib")
        #pragma comment(lib, "libAuto-static.lib")
        #pragma comment(lib, "rre_api-static.lib")
        #pragma comment(lib, "rr-libstruct-static.lib")
        #pragma comment(lib, "libsbml-static.lib")
        #pragma comment(lib, "libxml2_xe.lib")

    #else
        #pragma comment(lib, "roadrunner.lib")
        #pragma comment(lib, "rre_api.lib")
        #pragma comment(lib, "libAuto.lib")
    #endif
#endif


