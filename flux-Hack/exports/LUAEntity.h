#pragma once
#include "BaseLuaExport.h"
#include "../sdk/CBaseEntity.h"
#include "../sdk/interface/GLua.h"

class LUAEntity : public BaseLuaExport
{
public:
	LUAEntity(CBaseEntity* ent) : m_pEnt(ent) {}

	bool IsValid() {
		return m_pEnt;
	}

	Vector GetPos() {
		return m_pEnt->GetAbsOrigin();
	}

	int GetHealth() {
		return m_pEnt->GetHealth();
	}
	
	int WaterLevel() {
		return m_pEnt->WaterLevel();
	}
	
	int GetFlags() {
		return m_pEnt->GetFlags();
	}
	
	int GetMaxHealth() {
		return m_pEnt->GetMaxHealth();
	}

	bool IsDormant() {
		return m_pEnt->IsDormant();
	}

	bool IsAlive() {
		return m_pEnt->IsAlive();
	}
	
	bool Alive() {
		return m_pEnt->GetHealth() > 0;
	}
	
	bool IsPlayer() {
		return m_pEnt->IsPlayer();
	}

	bool IsReal() {
		return (IsValid() && IsAlive() && !IsDormant());
	}

	operator CBaseEntity*() {
		return m_pEnt;
	}
	
	int entIndex() {
		return m_pEnt->entIndex();
	}

	std::string GetName() {
		player_info_t info;
		g_pEngine->GetPlayerInfo(entIndex(), &info);
		return std::string(info.name);
	}
	
	int GetMoveType()
	{
		auto lua = g_pGLuaShared->GetLuaInterface(GLuaInterfaceType::CLIENT);
		lua->PushSpecial(SPECIAL_GLOB);
		lua->GetField(-1, "Entity");
		lua->PushNumber(entIndex());
		lua->Call(1, 1);
		
		lua->GetField(-1, "GetMoveType");
		lua->Push(-2);
		lua->Call(1, 1);
		
		int type = lua->GetNumber();
		lua->Pop(3);
		
		return type;
		
	}
private:
	CBaseEntity* m_pEnt;
};
