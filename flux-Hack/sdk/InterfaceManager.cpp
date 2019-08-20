#include "InterfaceManager.hpp"
#include <iostream>
#include <Windows.h>
#include "interface/Engine.h"
#include "interface\EntityList.h"
#include "Address.h"
#include "interface\Globals.h"
#include "interface\Predictionandmovement.h"
#include "interface\ModelInfo.h"
#include "interface\EngineTrace.h"
#include "interface\Panel.h"
#include "interface\Surface.h"
#include "interface\GLua.h"
#include "interface\Scheme.h"

void print(std::string Message, ...)
{
	char buffer[1024];
	va_list vlist;
	va_start(vlist, Message);
	_vsnprintf(buffer, sizeof(buffer), Message.c_str(), vlist);
	va_end(vlist);

	printf("%s", buffer);
}

void* g_pClient;
void* g_pClientMode;
void* g_pViewRender;
void InterfaceManager::GetInterfaces()
{
	g_pClient = GetInterface("client.dll", "VClient");
	g_pEngine = (CEngine*)GetInterface("engine.dll", "VEngineClient");
	g_pClientEntList = (EntityList*)GetInterface("client.dll", "VClientEntityList");
	g_pPrediction = (CPrediction*)GetInterface("client.dll", "VClientPrediction");
	g_pMovement = (CMovement*)GetInterface("client.dll", "GameMovement");
	g_pModelInfo = (SDKModelInfo*)GetInterface("engine.dll", "VModelInfoClient");
	g_pEngineTrace = (SDKTrace*)GetInterface("engine.dll", "EngineTraceClient");
	g_pPanel = (CPanel*)GetInterface("vgui2.dll", "VGUI_Panel");
	g_pSurface = (CSurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface");
	g_pGLuaShared = (IGLuaShared*)GetInterface("lua_shared.dll", "LUASHARED"); // GLua
	g_pScheme = (ISchemeManager*)GetInterface("vgui2.dll", "VGUI_Scheme");
	

	Address CDLLTable = ((Address)g_pClient).To<DWORD*>();
	Address pShutdown = (CDLLTable.As<DWORD*>())[10];
	g_pClientMode = *pShutdown.GetOffset(0x5).To<DWORD**>();
	print("g_pClientMode found 0x%X\n", g_pClientMode);
	
	pShutdown = (CDLLTable.As<DWORD*>())[0];
	g_pGlobals = *pShutdown.GetOffset(0x55).To<CGlobals**>();
	print("g_pGlobals found 0x%X\n", g_pGlobals);
	
	pShutdown = (CDLLTable.As<DWORD*>())[27];
	g_pViewRender = *pShutdown.GetOffset(0x5).To<void***>();
	print("g_pViewRender found 0x%X\n", g_pViewRender);
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
