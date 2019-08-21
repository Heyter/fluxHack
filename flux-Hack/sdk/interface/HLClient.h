#pragma once
#include "InterfaceBase.h"
#include "../recv.h"

class CHLClient: public InterfaceBase
{
public:
	ClientClass* GetAllClasses() {
		typedef ClientClass*(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 8)(this);
	}
	
}; extern CHLClient* g_pClient;