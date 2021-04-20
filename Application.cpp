#include "Application.h"

#ifdef _WIN32
#  define PLUGIN_PATH "Plugin.dll"
#  include <windows.h>
#  include <stdio.h>

HMODULE dlopen( const char * filepath, int ) {
   return LoadLibrary( filepath );
}
#  define RTLD_LAZY 0x00001
#  define RTLD_NOW  0x00002

const char * dlerror( void ) {
   static char buffer[2000];
   DWORD le = GetLastError();
   if( le == 193 ) {
      snprintf( buffer, sizeof( buffer ), "%d: 32 bits DLL can't be loaded by a 64 bits executable.", le );
   }
   else {
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, le, 0, buffer, sizeof( buffer ), 0 );
   }
   return buffer;
}

FARPROC dlsym( HMODULE hModule, const char * symbol ) {
   return GetProcAddress( hModule, symbol );
}

int dlclose( HMODULE hModule ) {
   return FreeLibrary( hModule ) ? 0 : -1;
}
#else
#  define PLUGIN_PATH "./libPlugin.so"
#  include <dlfcn.h>
#  define HMODULE void *
#endif

#include <iostream>
#include <map>
#include <string>

typedef std::map<std::string, Service> Services;

static Services services;

extern "C" API void AddService( const char * name, Service service ) {
   std::cerr << "AddService( " << name << " )" << std::endl;
   services[name] = service;
}

int main( void ) {
   try {
      HMODULE hLib = dlopen( PLUGIN_PATH, RTLD_LAZY );
      if( hLib ) {
         typedef bool ( * RegisterServices )( void );
         RegisterServices registerServices = (RegisterServices)dlsym( hLib, "RegisterServices" );
         if( registerServices ) {
            if( registerServices()) {
               Services::iterator it = services.find( "MyService" );
               if( it != services.end()) {
                  Service service = it->second;
                  int retCode = service( 0 );
                  std::cerr << "'MyService' returns " << retCode << std::endl;
               }
               else {
                  std::cerr << "'MyService' not found!" << std::endl;
               }
            }
            else {
               std::cerr << "Registration failed" << std::endl;
            }
         }
         else {
            std::cerr << "dlsym error: " << dlerror() << std::endl;
         }
         dlclose( hLib );
      }
      else {
         std::cerr << "dlopen error: " << dlerror() << std::endl;
      }
      return 0;
   }
   catch( const std::exception & x ) {
      std::cerr << x.what() << std::endl;
   }
   return 1;
}
