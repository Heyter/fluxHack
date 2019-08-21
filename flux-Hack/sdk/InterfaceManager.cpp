#include "InterfaceManager.hpp"
#include <iostream>
#include <Windows.h>
#include "interface/HLClient.h"
#include "interface/Engine.h"
#include "interface\EntityList.h"
#include "Address.h"
#include "interface\Globals.h"
#include "interface\Panel.h"
#include "interface\Surface.h"
#include "interface\GLua.h"
#include "interface\Scheme.h"
#include "NetVarManager.h"
#include "Offsets.h"
#include "UtilsFuncs.h"

static void HookOffsets() {
	CNetworkedVariableManager* NVMGR = new CNetworkedVariableManager();
	NVMGR->DumpNetvars();
	
	offsets.m_lifeState = NVMGR->GetOffset("DT_BasePlayer", "m_lifeState");
}

void InterfaceManager::GetInterfaces()
{
	g_pClient = (CHLClient*)GetInterface("client.dll", "VClient");
	g_pEngine = (CEngine*)GetInterface("engine.dll", "VEngineClient");
	g_pClientEntList = (EntityList*)GetInterface("client.dll", "VClientEntityList");
	g_pPanel = (CPanel*)GetInterface("vgui2.dll", "VGUI_Panel");
	g_pSurface = (CSurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface");
	g_pGLuaShared = (IGLuaShared*)GetInterface("lua_shared.dll", "LUASHARED"); // GLua
	g_pScheme = (ISchemeManager*)GetInterface("vgui2.dll", "VGUI_Scheme");
	
	Address CDLLTable = ((Address)g_pClient).To<DWORD*>();
	Address pShutdown = (CDLLTable.As<DWORD*>())[0];
	g_pGlobals = *pShutdown.GetOffset(0x55).To<CGlobals**>();
	print("g_pGlobals found 0x%X\n", g_pGlobals);
	
	HookOffsets();
}

void* InterfaceManager::GetInterface(char * modulename, char * interfacename)
{
	void*  Interface = nullptr;
	char PossibleInterfaceName[1024];

	HMODULE hmodule = GetModuleHandleA(modulename);
	CreateInterfaceFn create = (CreateInterfaceFn)GetProcAddress(hmodule, "CreateInterface");

	for (int i = 1; i < 100; i++)
	{
		sprintf(PossibleInterfaceName, "%s0%d", interfacename, i);
		Interface = (void*)create(PossibleInterfaceName, 0);
		if (Interface)
		{
			print("%s found 0x%X\n", PossibleInterfaceName, Interface);
			break;
		}

		sprintf(PossibleInterfaceName,"%s00%d", interfacename, i);
		Interface = create(PossibleInterfaceName, 0);
		if (Interface)
		{
			print("%s found 0x%X\n", PossibleInterfaceName, Interface);
			break;
		}
	}

	return Interface;
}
