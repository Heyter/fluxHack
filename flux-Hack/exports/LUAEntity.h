#pragma once
#include "BaseLuaExport.h"
#include "../sdk/CBaseEntity.h"

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
	
	int GetMaxHealth() {
		return m_pEnt->GetMaxHealth();
	}

	bool IsDormant() {
		return m_pEnt->IsDormant();
	}

	bool IsAlive() {
		return m_pEnt->IsAlive();
	}
	
	bool IsPlayer() {
		return m_pEnt->IsPlayer();
	}

	bool IsReal() {
		return (IsValid() && IsAlive() && !IsDormant());
	}

	operator CBaseEntity*()
	{
		return m_pEnt;
	}

	std::string GetName(int entNum)
	{
		player_info_t info;
		g_pEngine->GetPlayerInfo(entNum, &info);
		return std::string(info.name);
	}
private:
	CBaseEntity* m_pEnt;
};
