#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

EXPORT int CALLBACK GetNumber () ;
EXPORT void CALLBACK SetNumber (int a) ;

