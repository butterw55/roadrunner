
#if defined(CG_UI)
    #if defined(STATIC_PLUGIN)
        #pragma comment(lib, "rre_api-static.lib")
        #pragma comment(lib, "rrplugins_api-static.lib")
        #pragma comment(lib, "rrplugins_c_api-static.lib")
        #pragma comment(lib, "rrAutoInterface-static.lib")

    #else
        #pragma comment(lib, "roadrunner.lib")
        #pragma comment(lib, "rrplugins_api.lib")
    #endif

    #pragma comment(lib, "poco_foundation-static.lib")
#endif


