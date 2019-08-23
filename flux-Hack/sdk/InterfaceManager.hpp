#pragma once

typedef void *(__cdecl *CreateInterfaceFn)(const char *name, int *found);
class InterfaceManager
{
public:
	static void GetInterfaces();
	static void* GetInterface(char* modulename, char* interfacename);
};

extern void* g_pClientMode;