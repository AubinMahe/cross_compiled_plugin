# Cross compiled plugin

Services offered by a main are called back by a dynamic library plugin.

* Application.exe is compiled by VisualStudio 2017
* Application is compiled by g++ (10)
* Plugin.dll is cross compiled under GNU/Linux by MinGW
* libPlugin.so is compiled by g++ (10)

The plugin is loaded by dlopen/LoadLibrary, the symbol is mapped with dlsym/GetProcAddress.

The problem was to export some symbols located in Application(.exe): the solution is at linker level by using arguments `-rdynamic -Wl,--dynamic-list=main-api`.

To run this sample: `make`
Once executed under GNU/Linux, build the VisualStudio project and execute run.bat which replace the cross compiled `Application.exe` by the native one.
