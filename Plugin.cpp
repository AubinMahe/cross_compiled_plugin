#include "Application.h"

#undef API
#if defined __linux__
#  define API extern "C" __attribute((visibility("default")))
#elif defined _WIN32
#  define API extern "C" __declspec(dllexport)
#else
#  error Unsupported platform
#endif

#include <iostream>
#include <vector>

static int ServiceOffer( void * arg ) {
   std::cerr << "ServiceOffer|entry" << std::endl;
   std::vector<void *> v;
   v.push_back( arg );
   v.push_back( arg );
   v.push_back( arg );
   size_t val = v.size();
   std::cerr << "ServiceOffer|exit, val: " << val << std::endl;
   return 0;
}

API bool RegisterServices( void ) {
   AddService( "MyService", ServiceOffer );
   return true;
}
