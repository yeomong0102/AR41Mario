
#include "Font.h"

CFont::CFont()	:
	m_FontInfo{},
	m_hFont(0),
	m_hPrevFont(0)
{
}

CFont::~CFont()
{
	if (m_hFont)
		DeleteObject(m_hFont);
}

bool CFont::LoadFont(const TCHAR* FontName, int Width, int Height)
{
    /*
    #define FW_DONTCARE         0
    #define FW_THIN             100
    #define FW_EXTRALIGHT       200
    #define FW_LIGHT            300
    #define FW_NORMAL           400
    #define FW_MEDIUM           500
    #define FW_SEMIBOLD         600
    #define FW_BOLD             700
    #define FW_EXTRABOLD        800
    #define FW_HEAVY            900
    */
    m_FontInfo.lfWidth = (LONG)Width;
    m_FontInfo.lfHeight = (LONG)Height;
    m_FontInfo.lfCharSet = HANGEUL_CHARSET;
    m_FontInfo.lfWeight = FW_NORMAL;    // ����
    m_FontInfo.lfItalic = 0;            // ����
    m_FontInfo.lfEscapement = 1;        // ����
    m_FontInfo.lfUnderline = 0;         // ����
    m_FontInfo.lfStrikeOut = 0;         // ��Ҽ�
    m_FontInfo.lfPitchAndFamily = 2;    // �ڰ�

    lstrcpy(m_FontInfo.lfFaceName, FontName);

    m_hFont = CreateFontIndirect(&m_FontInfo);

	return m_hFont == nullptr ? false : true;
}

void CFont::SetFont(HDC hDC)
{
    m_hPrevFont = (HFONT)SelectObject(hDC, m_hFont);
}

void CFont::ResetFont(HDC hDC)
{
    SelectObject(hDC, m_hPrevFont);
}
