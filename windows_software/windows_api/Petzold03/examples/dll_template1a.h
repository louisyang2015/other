#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

EXPORT BOOL CALLBACK CenterTextA (HDC, PRECT, PCSTR) ;
EXPORT BOOL CALLBACK CenterTextW (HDC, PRECT, PCWSTR) ;

#ifdef UNICODE
#define CenterText CenterTextW
#else
#define CenterText CenterTextA
#endif
