
#if defined(CG_UI)
    #if defined(STATIC_PLUGIN)
        #pragma comment(lib, "roadrunner-static.lib")
        #pragma comment(lib, "rrplugins-static.lib")
        #pragma comment(lib, "libsbml-static")
        #pragma comment(lib, "libxml2_xe.lib")
    #else
        #pragma comment(lib, "roadrunner.lib")
        #pragma comment(lib, "rrplugins.lib")
    #endif
    #pragma comment(lib, "poco_foundation-static.lib")
#endif

