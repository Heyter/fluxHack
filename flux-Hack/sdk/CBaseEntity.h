#pragma once

#include "Vector.h"
#include "math.h"
#include "VMT.h"
#include "Offsets.h"

#define	FL_ONGROUND					(1 << 0)	
#define FL_DUCKING					(1 << 1)	
#define	FL_WATERJUMP				(1 << 2)	
#define FL_ONTRAIN					(1 << 3)
#define FL_INRAIN					(1 << 4)	
#define FL_FROZEN					(1 << 5)
#define FL_ATCONTROLS				(1 << 6)
#define	FL_CLIENT					(1 << 7)	
#define FL_FAKECLIENT				(1 << 8)	
#define	FL_INWATER					(1 << 9)										
#define	FL_FLY						(1 << 10)	
#define	FL_SWIM						(1 << 11)
#define	FL_CONVEYOR					(1 << 12)
#define	FL_NPC						(1 << 13)
#define	FL_GODMODE					(1 << 14)
#define	FL_NOTARGET					(1 << 15)
#define	FL_AIMTARGET				(1 << 16)	
#define	FL_PARTIALGROUND			(1 << 17)	
#define FL_STATICPROP				(1 << 18)
#define FL_GRAPHED					(1 << 19) 
#define FL_GRENADE					(1 << 20)
#define FL_STEPMOVEMENT				(1 << 21)	
#define FL_DONTTOUCH				(1 << 22)
#define FL_BASEVELOCITY				(1 << 23)
#define FL_WORLDBRUSH				(1 << 24)	
#define FL_OBJECT					(1 << 25) 
#define FL_KILLME					(1 << 26)
#define FL_ONFIRE					(1 << 27)	
#define FL_DISSOLVING				(1 << 28)
#define FL_TRANSRAGDOLL				(1 << 29)
#define FL_UNBLOCKABLE_BY_PLAYER	(1 << 30) 
#define GETNETVAR(retntype, funcname, offset) \
	retntype funcname() { \
		return *(retntype*)(this + offset); }
		
class IClientNetworkable
{
public:
	int entIndex() {
		typedef int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 9)(this);
	}
};

class CBaseEntity
{
public:
	GETNETVAR(int, GetFlags, offsets.m_fFlags);
	GETNETVAR(int, WaterLevel, offsets.m_nWaterLevel);

	int GetHealth() {
		typedef int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 108)(this);
	}
	
	int GetMaxHealth() {
		typedef int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 109)(this);
	}

	Vector& GetAbsOrigin() {
		typedef Vector&(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 9)(this);
	}

	bool IsDormant() {
		typedef bool(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 48)(this);
	}

	/*GETNETVAR(unsigned char, GetLifeState, offsets.m_lifeState);
	bool IsAlive() {
		return (GetLifeState() == 0);
	}*/
	
	bool IsAlive() {
		typedef bool(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 129)(this);
	}

	bool IsPlayer() {
		typedef bool(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 130)(this);
	}
	
	IClientNetworkable* GetClientNetworkable() {
		typedef IClientNetworkable*(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 4)(this);
	}
	
	int entIndex() {
		return GetClientNetworkable()->entIndex();
	}
};