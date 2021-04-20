.PHONY: all clean

all: lib/Application lib/libPlugin.so lib/Application.exe lib/Plugin.dll
	cd lib && ./Application
	cp /usr/lib/gcc/x86_64-w64-mingw32/7.3-win32/libgcc_s_seh-1.dll lib
	cp /usr/lib/gcc/x86_64-w64-mingw32/7.3-win32/libstdc++-6.dll    lib
	cd lib && wine ./Application.exe

clean:
	rm -fr lib Debug x64 .vs

lib/Application: Application.cpp
	@mkdir -p $$(dirname $@)
	g++ -o $@ $< -rdynamic -Wl,--dynamic-list=main-api -ldl

lib/Application.exe: Application.cpp
	@mkdir -p $$(dirname $@)
	x86_64-w64-mingw32-g++-win32 -DEXPORT -o $@ $< -Wl,--subsystem,windows,--out-implib,lib/libApplication.a

lib/libPlugin.so: Plugin.cpp
	@mkdir -p $$(dirname $@)
	g++ -o $@ $< -fvisibility=hidden -fpic -shared

lib/Plugin.dll: Plugin.cpp lib/Application.exe
	@mkdir -p $$(dirname $@)
	x86_64-w64-mingw32-g++-win32 -o $@ $< -fvisibility=hidden -fpic -shared -L lib -lApplication
