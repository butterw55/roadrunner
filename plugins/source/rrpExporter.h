#ifndef rrpExporterH
#define rrpExporterH

#if defined(_WIN32) || defined(__WIN32__)
    #if defined(STATIC_PLUGINS_API)
        #define RRP_DECLSPEC
    #else
        #if defined(EXPORT_PLUGINS_API)
            #define RRP_DECLSPEC __declspec(dllexport)
        #else
            #define RRP_DECLSPEC __declspec(dllimport)
        #endif
    #endif
#else
    #define RRP_DECLSPEC
#endif

//Helper macro for plugins
#if defined(_WIN32) || defined(__WIN32__)
    #if defined(EXPORT_RR_PLUGIN)
        #define RR_PLUGIN_DECLSPEC __declspec(dllexport)
    #else
        #define RR_PLUGIN_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define RR_PLUGIN_DECLSPEC
#endif


#endif

