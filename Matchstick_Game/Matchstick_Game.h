
// Matchstick_Game.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMatchstick_GameApp:
// �йش����ʵ�֣������ Matchstick_Game.cpp
//

class CMatchstick_GameApp : public CWinApp
{
public:
	CMatchstick_GameApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMatchstick_GameApp theApp;