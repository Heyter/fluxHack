#pragma warning(disable: 4099)
#pragma warning(disable: 4227)
#pragma warning(disable: 4244)
#pragma warning(disable: 4800)
#pragma warning(disable: 4101)
#include <Windows.h>
#include <experimental/filesystem> // C++-standard header file name
#include <fstream>      // std::ifstream
#include <lua.hpp>
#include <LuaBridge.h>
#include "LuaEngine.h"
#include "sdk\InterfaceManager.hpp"
#include "exports\Exports.h"
#include "exports\LuaFiles.h"
#include "sdk\interface\Panel.h"
#include "Drawing.h"
#include "GLuaSharedApi.h"
#include "sdk\interface\Globals.h"
#include "sdk\Console.h"
#include "sdk/interface/HLClient.h"
#include <shlobj.h>

using namespace std::experimental::filesystem;

ConsoleManager g_c;

LUAInterfaces g_Interfaces;
LUAUtils g_Utils;
CDrawing g_Drawing;
CGLuaInterface g_GLuaIn;

Vector Vector::ToScreen() {
	Vector screen(0, 0, 0);
	Vector point = *this;
	const VMatrix &worldToScreen = g_pEngine->WorldToScreenMatrix();
	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	float w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	if (w >= 0.001f)
	{
		int iWidth, iHeight;
		g_pEngine->GetScreenSize(iWidth, iHeight);

		screen.x *= 1.0f / w;
		screen.y *= 1.0f / w;

		float x = iWidth / 2;
		float y = iHeight / 2;
		x += 0.5 * screen.x * iWidth + 0.5;
		y -= 0.5 * screen.y * iHeight + 0.5;
		screen.x = x;
		screen.y = y;

		return screen;
	}
	
	screen.x *= 100000;
	screen.y *= 100000;

	return screen;
	
}
typedef void(__cdecl *MsgFn)(char const* pMsg, va_list);
void Msg(char const* msg)
{
	if (!msg)
		return;
	static MsgFn oMsg = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg");
	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	oMsg(buffer, list);
}

#include "sdk/PatternScanning.h"

static path GetHomeDirectory() {
	path path;

	PWSTR winpath = nullptr;
	SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &winpath);

	path = std::wstring(winpath);

	return path;
}

static path GetWorkDirectory() {
	const auto& home = GetHomeDirectory();

	if (home.empty()) {
		throw filesystem_error("home directory path is empty",
			std::error_code(ENOENT, std::system_category()));
	}

	return (home / "gmod_lua");
}

static std::string ReadFile(const std::string& path) {
	auto iff = std::ifstream(GetWorkDirectory() / path);

	if (!iff.is_open()) {
		throw filesystem_error("file does not exist",
			std::error_code(ENOENT, std::system_category()));
	}

	std::stringstream ss;
	ss << iff.rdbuf();
	iff.close();

	return ss.str();
}

static void IncludeLua(std::string file) {
	g_pLuaEngine->ExecuteFileBuffer(file.c_str(), ReadFile(file.c_str()));
}

static void RegEverything(lua_State* L)
{
	using namespace luabridge;
	getGlobalNamespace(L)
		.beginNamespace("Game")
			.addFunction("include", &IncludeLua)
			.addFunction("Msg", &Msg)
			.addVariable("Interfaces", &g_Interfaces, false)
			.addVariable("Utils", &g_Utils, false)
			.addVariable("Drawing", &g_Drawing, false)
			.addVariable("GLuaStack", &g_GLuaIn, false)
			.beginClass<LUAInterfaces>("__Interfaces")
				.addFunction("GetEngine", &LUAInterfaces::GetEngine)
				.addFunction("GetEntityList", &LUAInterfaces::GetEntityList)
			.endClass()
			.beginClass<LUAEngine>("EngineInterface")
				.addFunction("GetScreenSize", &LUAEngine::GetScreenSize)
				.addFunction("GetLocalPlayer", &LUAEngine::GetLocalPlayer)
				.addFunction("GetMaxClients", &LUAEngine::GetMaxClients)
				.addFunction("IsInGame", &LUAEngine::IsInGame)
				.addFunction("IsTakingScreenshot", &LUAEngine::IsTakingScreenshot)
				.addProperty("viewangles", &LUAEngine::GetViewAngles, &LUAEngine::SetViewAngles)
				.addFunction("ExecuteClientCmd", &LUAEngine::ExecuteClientCmd)
			.endClass()
			.beginClass<Vector>("Vector")
				.addConstructor<void(*)()>()
				.addConstructor<void(*)(float, float, float)>()
				.addFunction("__eq", &Vector::operator==)
				.addFunction("__add", &Vector::operator+)
				.addFunction("__sub", &Vector::sub)
				.addFunction("__mul", &Vector::mul)
				.addFunction("__div", &Vector::div)
				.addData("x", &Vector::x)
				.addData("y", &Vector::y)
				.addData("z", &Vector::z)
				.addFunction("Length", &Vector::Length)
				.addFunction("Angle", &Vector::Angle)
				.addFunction("Distance", &Vector::DistTo)
				.addFunction("ToScreen", &Vector::ToScreen)
				.addFunction("Lerp", &Vector::lerp)
				.addFunction("Clamp", &Vector::ClampAngle)
			.endClass()
			.beginClass<Vec2>("Vec2")
				.addConstructor<void(*)()>()
				.addConstructor<void(*)(float, float)>()
				.addData("x", &Vec2::x)
				.addData("y", &Vec2::y)
			.endClass()
			.beginClass<LuaUserCmd>("UserCmd")
				.addProperty("viewangles", &LuaUserCmd::GetViewAngles, &LuaUserCmd::SetViewAngles)
				.addFunction("IsKeyDown", &LuaUserCmd::IsKeyDown)
				.addFunction("GetButtons", &LuaUserCmd::GetButtons)
				.addFunction("RemoveKey", &LuaUserCmd::RemoveKey)
				.addFunction("SetButtons", &LuaUserCmd::SetButtons)
			.endClass()
			.beginClass<LUAEntity>("Entity")
				.addFunction("IsValid", &LUAEntity::IsValid)
				.addFunction("GetPos", &LUAEntity::GetPos)
				.addFunction("GetHealth", &LUAEntity::GetHealth)
				.addFunction("WaterLevel", &LUAEntity::WaterLevel)
				.addFunction("GetMoveType", &LUAEntity::GetMoveType)
				.addFunction("GetFlags", &LUAEntity::GetFlags)
				.addFunction("GetMaxHealth", &LUAEntity::GetMaxHealth)
				.addFunction("IsDormant", &LUAEntity::IsDormant)
				.addFunction("IsAlive", &LUAEntity::IsAlive)
				.addFunction("Alive", &LUAEntity::Alive)
				.addFunction("IsPlayer", &LUAEntity::IsPlayer)
				.addFunction("IsReal", &LUAEntity::IsReal)
				.addFunction("Nick", &LUAEntity::GetName)
				.addFunction("EntIndex", &LUAEntity::entIndex)
			.endClass()
			.beginClass<LUAEntityList>("EntityList")
				.addFunction("GetEntity", &LUAEntityList::GetEntity)
				.addFunction("GetHighestEntityIndex", &LUAEntityList::GetHighestEntityIndex)
			.endClass()
			.beginClass<LUAUtils>("__Utils")
				.addFunction("LocalPlayer", &LUAUtils::LocalPlayer)
				.addFunction("GetSchemeFont", &LUAUtils::GetSchemeFont)
			.endClass()
			.beginClass<CDrawing>("__Drawing")
				.addFunction("DrawString", &CDrawing::DrawString)
				.addFunction("CreateFontGlyph", &CDrawing::CreateFontGlyph)
				.addFunction("DrawLine", &CDrawing::DrawLine)
				.addFunction("DrawFilledRect", &CDrawing::DrawFilledRect)
				.addFunction("DrawOutlinedRect", &CDrawing::DrawOutlinedRect)
				.addFunction("DrawRect", &CDrawing::DrawRect)
				.addFunction("SetDrawColor", &CDrawing::SetDrawColor)
				.addFunction("StartDrawing_ViewRender", &CDrawing::StartDrawing)
				.addFunction("FinishDrawing_ViewRender", &CDrawing::FinishDrawing)
			.endClass()
			.beginClass<CGLuaInterface>("__GLuaStack")
				.addFunction("SetField", &CGLuaInterface::SetField)
				.addFunction("GetField", &CGLuaInterface::GetField)
				.addFunction("GetCFunction", &CGLuaInterface::GetCFunction)
				.addFunction("PushSpecial", &CGLuaInterface::PushSpecial)
				.addFunction("Push", &CGLuaInterface::Push)
				.addFunction("Pop", &CGLuaInterface::Pop)
				.addFunction("Call", &CGLuaInterface::Call)
				.addFunction("PCall", &CGLuaInterface::PCall)
				.addFunction("ThrowError", &CGLuaInterface::ThrowError)
				.addFunction("GetNumber", &CGLuaInterface::GetNumber)
				.addFunction("GetBool", &CGLuaInterface::GetBool)
				.addFunction("GetString", &CGLuaInterface::GetString)
				.addFunction("IsType", &CGLuaInterface::IsType)
				.addFunction("GetType", &CGLuaInterface::GetType)
				.addFunction("GetLuaInterface", &CGLuaInterface::GetLuaInterface)
				.addFunction("PushNil", &CGLuaInterface::PushNil)
				.addFunction("PushString", &CGLuaInterface::PushString)
				.addFunction("PushNumber", &CGLuaInterface::PushNumber)
				.addFunction("PushBool", &CGLuaInterface::PushBool)
				.addFunction("SetTable", &CGLuaInterface::SetTable)
				.addFunction("GetTable", &CGLuaInterface::GetTable)
			.endClass()
		.endNamespace();
}

typedef void(__thiscall* PaintTraverseFn)(void*,unsigned int, bool, bool);
PaintTraverseFn oPaintTraverse;
void __fastcall hkPaintTraverse(void* thisptr,void*, unsigned int a, bool b, bool c)
{
	oPaintTraverse(thisptr, a, b, c);
	
	//if ((strcmp(g_pPanel->GetName(a), "HudGMOD")))
		//return;

	LOCKLUA();
	if (!g_pLuaEngine->L())
		return;
	using namespace luabridge;
	LuaRef hook = getGlobal(g_pLuaEngine->L(), "hook");
	if (hook["Call"].isFunction())
	{
		try {
			hook["Call"]("Paint");
		}
		catch (LuaException const& e) {}
	}
	else
	{
		printf("ERR: PT - hook.Call not found!\n");
	}
}

static struct vrect_t
{
	int x, y, width, height;
	vrect_t* pnext;
};

typedef void(__thiscall* ViewRenderFn)(void*, vrect_t* rect);
ViewRenderFn oViewRender = nullptr;
void __fastcall hkViewRender(CHLClient* pl, void* edx, vrect_t* rect) {
	oViewRender(pl, rect);

	if (!g_pEngine->IsInGame())
		return;

	static BYTE* IsRecordingMovie = *(BYTE**)(FindPattern("engine.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? D9 45 18") + 0x3E);
	if (g_pEngine->IsTakingScreenshot() || *IsRecordingMovie)
		return;
		

	LOCKLUA();
	if (!g_pLuaEngine->L())
		return;
	using namespace luabridge;
	LuaRef hook = getGlobal(g_pLuaEngine->L(), "hook");
	if (hook["Call"].isFunction())
	{
		try {
			hook["Call"]("PaintSG");
		}
		catch (LuaException const& e) {}
	}
	else
	{
		printf("ERR: PT - hook.Call not found!\n");
	}
}

typedef bool(__thiscall* CreateMoveFn)(void*, float, CUserCmd*);
CreateMoveFn oCreateMove = nullptr;

bool __fastcall hkCreateMove(void* cl, void* edx, float frametime, CUserCmd* cmd) {
	oCreateMove(cl, frametime, cmd);
	
	LOCKLUA();
	if (!g_pLuaEngine->L())
		return false;
	
	using namespace luabridge;
	LuaRef hook = getGlobal(g_pLuaEngine->L(), "hook");
	if (hook["Call"].isFunction())
	{
		try {
			hook["Call"]("CreateMove", LuaUserCmd(cmd), frametime);
		}
		catch (LuaException const& e) {}
	}
	else
	{
		printf("ERR: PT - hook.Call not found!\n");
	}
	
	oCreateMove(cl, frametime, cmd);

	return false;
}

static void loadscript(std::vector<std::string> args)
{
	LOCKLUA();
	g_pLuaEngine->ExecuteFileBuffer(args[1].c_str(), ReadFile(args[1].c_str()));
	printf("Executed %s!\n", args[1].c_str());
}

static VMT* clientmode = nullptr;
static VMT* client = nullptr;
static VMT* panel = nullptr;

void StartThread();
static void reset(std::vector<std::string> args) {
	(ViewRenderFn)client->unhookFunction(26);
	(PaintTraverseFn)panel->unhookFunction(16);
	(CreateMoveFn)clientmode->unhookFunction(21);
	
	client->shutdown();
	panel->shutdown();
	clientmode->shutdown();
	
	StartThread();
}

void StartThread()
{
	LOCKLUA();

	g_pLuaEngine->Reset();
	InterfaceManager::GetInterfaces();
	g_pEngine->ExecuteClientCmd("clear");
	
	clientmode = new VMT(g_pClientMode);
	clientmode->init();
	clientmode->setTableHook();
	oCreateMove = (CreateMoveFn)clientmode->hookFunction(21, hkCreateMove);

	client = new VMT(g_pClient);
	client->init();
	client->setTableHook();
	oViewRender = (ViewRenderFn)client->hookFunction(26, hkViewRender);

	panel = new VMT(g_pPanel);
	panel->init();
	panel->setTableHook();
	oPaintTraverse = (PaintTraverseFn)panel->hookFunction(41, hkPaintTraverse);

	RegEverything(g_pLuaEngine->L());
	g_pLuaEngine->ExecuteString(LUA_INJECT_CODE.c_str());
	g_pLuaEngine->ExecuteFileBuffer("init.lua", ReadFile("init.lua"));
}

static bool panic_key = false;
static void panic(std::vector<std::string> args) { panic_key = true; };
static void help_cmd(std::vector<std::string> args) {
	printf("Commands:\n");
	for (auto it : g_c.m_commands)
		printf("-> %s\n", it.first.c_str());
}

void ConsoleThread(void* dll)
{
	g_c.AddItem("load", loadscript);
	g_c.AddItem("reset", reset);
	g_c.AddItem("panic", panic);
	g_c.AddItem("help", help_cmd);
	
	while (!panic_key) {
		g_c.Think();
	}
	
	FreeLibraryAndExitThread(HMODULE(dll), 0);
}


void InitConsole(char* name)
{
	AllocConsole();
	HWND hwnd = GetConsoleWindow();
	HMENU hMenu = GetSystemMenu(hwnd, FALSE);
	if (hMenu) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);

	SetConsoleTitle(name);
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

BOOL WINAPI DllMain(HINSTANCE hMod, DWORD dwReason, LPVOID reserved)
{
	switch(dwReason) {
		case DLL_PROCESS_ATTACH:
			InitConsole("LUA");
			DisableThreadLibraryCalls(hMod);
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartThread, 0, 0, 0);
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ConsoleThread, hMod, 0, 0);
			break;
		case DLL_PROCESS_DETACH:
			(ViewRenderFn)client->unhookFunction(26);
			(PaintTraverseFn)panel->unhookFunction(16);
			(CreateMoveFn)clientmode->unhookFunction(21);
			
			client->shutdown();
			panel->shutdown();
			clientmode->shutdown();
			
			FreeConsole();
			g_pLuaEngine->Close();
			if (g_pLuaEngine) {
				delete g_pLuaEngine;
				g_pLuaEngine = nullptr;
			}
			
			break;
		default:
			break;
	}
	return TRUE;
}