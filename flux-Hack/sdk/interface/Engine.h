#pragma once
#include "InterfaceBase.h"
#include "../Vector.h"
#include "../math.h"
class player_info_t
{
public:
	char			name[32];
	int				userID;
	char			guid[32 + 1];
	long			friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	int				customFiles[4];
	unsigned char	filesDownloaded;
};

struct VMatrix
{
public:
	float* operator[](int i) { return m[i]; }
	const float* operator[](int i) const { return m[i]; }
	float *Base() { return &m[0][0]; }
	const float *Base() const { return &m[0][0]; }

	float	m[4][4];
};
class CEngine : public InterfaceBase
{
public:
	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* fn)(void*, int&, int&);
		VMT::getvfunc<fn>(this, 5)(this, width, height);
	}

	bool GetPlayerInfo(int index, player_info_t* info)
	{
		typedef bool(__thiscall* fn)(void*, int, player_info_t*);
		return VMT::getvfunc<fn>(this, 8)(this, index, info);
	}
	
	int GetMaxClients() {
		typedef int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 21)(this);
	}

	int GetLocalPlayer()
	{
		typedef int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 12)(this);
	}
	
	bool IsInGame() {
		typedef bool(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 26)(this);
	}

	bool IsTakingScreenshot() {
		typedef bool(__thiscall * fn)(void*);
		return VMT::getvfunc<fn>(this, 85)(this);
	}

	void ExecuteClientCmd(const char* cmd)
	{
		typedef void(__thiscall* fn)(void*, const char*);
		VMT::getvfunc<fn>(this, 102)(this, cmd);
	}

	void SetViewAngles(Vector& angles)
	{
		typedef void(__thiscall* fn)(void*, Vector&);
		VMT::getvfunc<fn>(this, 20)(this, angles);
	}

	void GetViewAngles(Vector& angles)
	{
		typedef void(__thiscall* fn)(void*, Vector&);
		VMT::getvfunc<fn>(this, 19)(this, angles);
	}

	VMatrix WorldToScreenMatrix()
	{
		typedef VMatrix(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 36)(this);
	}
};
extern CEngine* g_pEngine;