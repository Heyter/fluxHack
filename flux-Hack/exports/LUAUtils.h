#pragma once
#include "BaseLuaExport.h"
#include "../sdk/interface/Engine.h"
#include "../sdk/Vector.h"
#include "../sdk/CBaseEntity.h"
#include "../sdk/interface/Scheme.h"
#include "../sdk/CUserCmd.h"
#include "Exports.h"

class LUAUtils : public BaseLuaExport
{
public:
	LUAEntity LocalPlayer() {
		return LUAEntity(g_pClientEntList->GetClientEntity(g_pEngine->GetLocalPlayer()));
	}
	
	unsigned long GetSchemeFont(const char *tag, const char *fontName, bool proportional = false) {;
		if (!tag || !fontName)
			return 0;
		return g_pScheme->GetIScheme(g_pScheme->GetScheme(tag))->GetFont(fontName, proportional);
	}
	
	void Prediction(CUserCmd* cmd, CBaseEntity* player) { // later
		return;
	}
private:
	__forceinline float DotProduct(const Vector v1, const float* v2)
	{
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}

	void VectorTransform(const Vector in1, const matrix3x4_t& in2, Vector& out)
	{
		out[0] = DotProduct(in1, in2[0]) + in2[0][3];
		out[1] = DotProduct(in1, in2[1]) + in2[1][3];
		out[2] = DotProduct(in1, in2[2]) + in2[2][3];
	}
};
extern LUAUtils g_Utils;
