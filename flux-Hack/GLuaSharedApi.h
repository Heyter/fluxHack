#pragma once
#include "sdk\interface\GLua.h"

class CGLuaInterface
{
public:	
	void GetField(int iStackPos, const char* strName) {
		glua->GetField(iStackPos, strName);
	}
	
	void SetField(int iStackPos, const char* strName) {
		glua->SetField(iStackPos, strName);
	}
	
	auto GetCFunction(int iStackPos) {
		return (CGLuaFunc*)glua->GetCFunction(iStackPos);
	}

	void PushSpecial(int iType) {
		glua->PushSpecial(iType);
	}

	void Push(int iStackPos) {
		glua->Push(iStackPos);
	}

	void Pop(int iAmt) {
		glua->Pop(iAmt);
	}

	void Call(int iArgs, int iResults) {
		glua->Call(iArgs, iResults);
	}
	
	int PCall(int iArgs, int iResults, int iErrorFunc) {
		return glua->PCall(iArgs, iResults, iErrorFunc);
	}
	
	void ThrowError(const char* strError) {
		glua->ThrowError(strError);
	}
	
	double GetNumber(/*IGLuaInterface* lua, */int iStackPos) {
		return glua->GetNumber(iStackPos);
	}

	bool GetBool(int iStackPos) {
		return glua->GetBool(iStackPos);
	}

	const char* GetString(int iStackPos, unsigned int* iOutLen) {
		return glua->GetString(iStackPos, iOutLen);
	}

	bool IsType(int iStackPos, int iType) {
		return glua->IsType(iStackPos, iType);
	}

	int GetType(int iStackPos) {
		return glua->GetType(iStackPos);
	}

	void GetLuaInterface(GLuaInterfaceType type) {
		glua = g_pGLuaShared->GetLuaInterface(type);
	}
	
	void PushNil() {
		glua->PushNil();
	}
	
	void PushString(const char* val, unsigned int iLen) {
		glua->PushString(val, iLen);
	}
	
	void PushNumber(double val) {
		glua->PushNumber(val);
	}
	
	void PushBool(bool val) {
		glua->PushBool(val);
	}
	
	void SetTable(int i) {
		glua->SetTable(i);
	}
	
	void GetTable(int iStackPos) {
		glua->GetTable(iStackPos);
	}

	double CurTime() { // example
		auto GLUA = g_pGLuaShared->GetLuaInterface(GLuaInterfaceType::CLIENT);
		GLUA->PushSpecial(SPECIAL_GLOB);
		GLUA->GetField(-1, "CurTime");
		GLUA->Call(0, 1);
		double curtime = GLUA->GetNumber(-1);
		GLUA->Pop(2); // Curtime, SPECIAL_GLOB

		return curtime;
	}
private:
	IGLuaInterface* glua;
}; extern CGLuaInterface g_GLuaIn;