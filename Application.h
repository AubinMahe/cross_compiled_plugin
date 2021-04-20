#pragma once

typedef int ( * Service )( void * context );

#ifdef _WIN32
#  ifdef EXPORT
#  define API __declspec(dllexport)
#  else
#  define API __declspec(dllimport)
#  endif
#else
#  define API
#endif

extern "C" API void AddService( const char * name, Service service );
