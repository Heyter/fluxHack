#pragma once
#include "InterfaceBase.h"
#include "..//PatternScanning.h"
/*
	DrawSetColor
	DrawFilledRect
	DrawOutlinedRect
	DrawLine
	CreateFont
	SetFontGlyphSet
	SetDrawTextFont
	SetDrawTextColor
	SetDrawTextPos
	DrawPrintText

*/
class CSurface : public InterfaceBase
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 11)(this, r, g, b, a);
	}

	void DrawFilledRect(int x, int y, int x2, int y2)
	{
		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 12)(this, x, y, x2, y2);
	}

	void DrawOutlinedRect(int x, int y, int x2, int y2)
	{

		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 14)(this, x, y, x2, y2);
	}

	void DrawLine(int x, int y, int x2, int y2)
	{

		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 15)(this, x, y, x2, y2);
	}
#undef CreateFont
	unsigned int CreateFont()
	{
		typedef unsigned int(__thiscall* fn)(void*);
		return VMT::getvfunc<fn>(this, 66)(this);
	}

	bool SetFontGlyphSet(unsigned long font, char const* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int rangeMin = 0, int rangeMax = 0)
	{
		typedef bool(__thiscall* fn)(void*, unsigned int, const char*, int, int, int, int, int, int, int);
		return VMT::getvfunc<fn>(this, 67)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, rangeMin, rangeMax);
	}
	void SetTextFont(unsigned long font)
	{
		typedef void(__thiscall* fn)(void*, unsigned long);
		VMT::getvfunc<fn>(this, 17)(this, font);
	}

	void SetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* fn)(void*, int, int, int, int);
		VMT::getvfunc<fn>(this, 19)(this, r, g, b, a);
	}

	void SetTextPos(int x, int y)
	{
		typedef void(__thiscall* fn)(void*, int, int);
		VMT::getvfunc<fn>(this, 20)(this, x, y);
	}

	void DrawPrintText(const wchar_t* Text, int Len, int DrawType = 0)
	{
		typedef void(__thiscall* fn)(void*, wchar_t const*, int, int);
		VMT::getvfunc<fn>(this, 22)(this, Text, Len, DrawType);
	}
	
	void PushMakeCurrent(unsigned int panel, bool useInsets) {
		typedef void(__thiscall* fn)(void*, unsigned int, bool);
		VMT::getvfunc<fn>(this, 8)(this, panel, useInsets);
	}
	
	void PopMakeCurrent(unsigned int panel) {
		typedef void(__thiscall* fn)(void*, unsigned int);
		VMT::getvfunc<fn>(this, 9)(this, panel);
	}

	void StartDrawing()
	{
		using StartDrawingFn = void(__thiscall*)(void*);
		static StartDrawingFn StartDrawing;

		if (!StartDrawing)
			StartDrawing = (StartDrawingFn)(FindPattern("vguimatsurface.dll", "55 8B EC 64 A1 ?? ?? ?? ?? 6A FF 68 ?? ?? ?? ?? 50 64 89 25 ?? ?? ?? ?? 83 EC 14"));

		StartDrawing(this);
	}
	void FinishDrawing()
	{
		using FinishDrawingFn = void(__thiscall*)(void*);
		static FinishDrawingFn FinishDrawing;

		if (!FinishDrawing)
			FinishDrawing = (FinishDrawingFn)(FindPattern("vguimatsurface.dll", "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 64 89 25 ?? ?? ?? ?? 51 56 6A 00"));

		FinishDrawing(this);
	}
	
}; extern CSurface* g_pSurface;

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};