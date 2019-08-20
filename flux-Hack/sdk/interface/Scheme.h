#pragma once
#include "InterfaceBase.h"

class IScheme: public InterfaceBase
{
public:
	unsigned long GetFont(const char *fontName, bool proportional)
	{
		typedef unsigned long(__thiscall* fn)(void*, const char *, bool);
		return VMT::getvfunc<fn>(this, 3)(this, fontName, proportional);
	}
};

class ISchemeManager: public InterfaceBase
{
public:
	unsigned long GetScheme(const char* tag) {
		typedef unsigned long(__thiscall * fn)(void*, const char*);
		return VMT::getvfunc<fn>(this, 5)(this, tag);
	}
	
	IScheme* GetIScheme(unsigned long scheme) {
		typedef IScheme*(__thiscall* fn)(void*, unsigned long);
		return VMT::getvfunc<fn>(this, 8)(this, scheme);
	}
}; extern ISchemeManager* g_pScheme;