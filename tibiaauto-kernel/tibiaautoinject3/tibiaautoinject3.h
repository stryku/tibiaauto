// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the TIBIAAUTOINJECT3_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// TIBIAAUTOINJECT3_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef TIBIAAUTOINJECT3_EXPORTS
#define TIBIAAUTOINJECT3_API __declspec(dllexport)
#else // ifdef TIBIAAUTOINJECT3_EXPORTS
#define TIBIAAUTOINJECT3_API __declspec(dllimport)
#endif // ifdef TIBIAAUTOINJECT3_EXPORTS

TIBIAAUTOINJECT3_API LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);