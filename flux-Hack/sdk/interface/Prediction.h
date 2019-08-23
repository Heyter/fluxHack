#pragma once
#include "InterfaceBase.h"
#include "../CUserCmd.h"
#include "../CBaseEntity.h"

class CMoveData
{
public:
	char pad_0000[4]; //0x0000
	uint32_t m_nPlayerHandle; //0x0004
	int32_t m_nImpulseCommand; //0x0008
	Vector m_vecViewAngles; //0x000C
	Vector m_vecAbsViewAngles; //0x0018
	int32_t m_nButtons; //0x0024
	int32_t m_nOldButtons; //0x0028
	float m_flForwardMove; //0x002C
	float m_flSideMove; //0x0030
	float m_flUpMove; //0x0034
	float m_flMaxSpeed; //0x0038
	float m_flClientMaxSpeed; //0x003C
	Vector m_vecVelocity; //0x0040
	Vector m_vecAngles; //0x004C
	Vector m_vecOldAngles; //0x0058
	float m_outStepHeight; //0x0064
	Vector m_outWishVel; //0x0068
	Vector m_outJumpVel; //0x0074
	Vector m_vecConstraintCenter; //0x0080
	float m_flConstraintRadius; //0x008C
	float m_flConstraintWidth; //0x0090
	float m_flConstraintSpeedFactor; //0x0094
	Vector m_vecAbsOrigin; //0x0098
};

//VClientPrediction client.dll
class CPrediction: public InterfaceBase
{
public:
	void SetupMove(CBaseEntity* ent, CUserCmd* cmd, void* move, CMoveData *movedata)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, CUserCmd*, void*, CMoveData*);
		VMT::getvfunc<fn>(this, 18)(this, ent, cmd, move, movedata);
	}

	void FinishMove(CBaseEntity* ent, CUserCmd* cmd, CMoveData *movedata)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, CUserCmd*, CMoveData*);
		VMT::getvfunc<fn>(this, 19)(this, ent, cmd, movedata);
	}
};
extern CPrediction* g_pPrediction;

//GameMovement client.dll
class CMovement: public InterfaceBase
{
public:
	void ProcessMovement(CBaseEntity* ent, CMoveData* movedata)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*, CMoveData*);
		VMT::getvfunc<fn>(this, 1)(this, ent, movedata);
	}

	void StartTrackPredictionErrors(CBaseEntity* ent)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*);
		VMT::getvfunc<fn>(this, 2)(this, ent);
	}

	void FinishTrackPredictionErrors(CBaseEntity* ent)
	{
		typedef void(__thiscall* fn)(void*, CBaseEntity*);
		VMT::getvfunc<fn>(this, 3)(this, ent);
	}
};
extern CMovement* g_pMovement;