#pragma once
#include "BaseLuaExport.h"
#include "../sdk/CUserCmd.h"

class LuaUserCmd: public BaseLuaExport
{
public:
	LuaUserCmd(CUserCmd* cmd) : m_pCmd(cmd) {}

	operator CUserCmd*() {
		return m_pCmd;
	}

	int GetButtons() {
		return m_pCmd->buttons;
	}
	void SetButtons(int nFlagMask) {
		m_pCmd->buttons |= nFlagMask;
	}
	bool IsKeyDown(int key) {
		return m_pCmd->buttons & key;
	}
	void RemoveKey(int key) {
		m_pCmd->buttons &= ~key;
	}

	Vector GetViewAngles() const
	{
		return m_pCmd->viewangles;
	}

	void SetViewAngles(Vector& ang) {
		m_pCmd->viewangles = ang;
	}
private:
	CUserCmd* m_pCmd;
};