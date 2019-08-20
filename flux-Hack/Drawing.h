#pragma once
#define FONT_HEIGHT 16
#include "sdk\interface\Surface.h"
#include <Windows.h>
#include <codecvt>

class CDrawing
{
public:

	void DrawString(unsigned long font, float x, float y,  const char* text)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wtext = converter.from_bytes(text);

		g_pSurface->SetTextPos(x, y);
		g_pSurface->SetTextFont(font);
		g_pSurface->DrawPrintText(wtext.c_str(), wtext.size());
	}
	
	void DrawString2(unsigned long font, int x, int y, int r, int g, int b, int a, const char *text)
	{
		if (text == nullptr)
			return;

		va_list va_alist;
		char szBuffer[1024] = { '\0' };
		wchar_t szString[1024] = { '\0' };

		va_start(va_alist, text);
		vsprintf_s(szBuffer, text, va_alist);
		va_end(va_alist);

		wsprintfW(szString, L"%S", szBuffer);
		g_pSurface->SetTextPos(x, y);
		g_pSurface->SetTextFont(font);
		g_pSurface->SetTextColor(r, g, b, a);
		g_pSurface->DrawPrintText(szString, wcslen(szString));
	}
	
	unsigned long CreateFontGlyph(const char* fontname, float size, bool bold, bool italic, bool outlined, bool dropshadow) {
		unsigned long font = g_pSurface->CreateFont();
		int flags = (italic ? FONTFLAG_ITALIC : 0) | (outlined ? FONTFLAG_OUTLINE : 0) | (dropshadow ? FONTFLAG_DROPSHADOW : 0) | FONTFLAG_ANTIALIAS;
		g_pSurface->SetFontGlyphSet(font, fontname, size, bold ? FW_BOLD : FW_MEDIUM, 0, 0, flags);
		return font;
	}

	void DrawLine(float x, float y, float x2, float y2)
	{
		g_pSurface->DrawLine(x, y, x2, y2);
	}

	void DrawFilledRect(float x, float y, float w, float h)
	{
		g_pSurface->DrawFilledRect(x, y, x + w, y + h);
	}

	void DrawOutlinedRect(float x, float y, float w, float h, float r, float g, float b, float a)
	{
		g_pSurface->DrawOutlinedRect(x, y, x + w, y + h);
		g_pSurface->DrawSetColor(r, g, b, a);
		x -= 1; y -= 1;
		w += 2; h += 2;
		g_pSurface->DrawOutlinedRect(x, y, x + w, y + h);
		x += 2; y += 2;
		w -= 4; h -= 4;
		g_pSurface->DrawOutlinedRect(x, y, x + w, y + h);
		g_pSurface->DrawSetColor(m_r, m_g, m_b, m_a);
	}

	void DrawRect(float x, float y, float w, float h)
	{
		g_pSurface->DrawOutlinedRect(x, y, x + w, y + h);
	}

	void SetDrawColor(float r, float g, float b, float a)
	{
		m_r = r; m_g = g; m_b = b; m_a = a;
		g_pSurface->DrawSetColor(r, g, b, a);
		g_pSurface->SetTextColor(r, g, b, a);
	}
	
	void StartDrawing() {
		g_pSurface->StartDrawing();
	}
	
	void FinishDrawing() {
		g_pSurface->FinishDrawing();
	}
private:
	int m_r, m_g, m_b, m_a;
}; extern CDrawing g_Drawing;