
// Matchstick_GameDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CMatchstick_GameDlg �Ի���
class CMatchstick_GameDlg : public CDialogEx
{
// ����
public:
	CMatchstick_GameDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MATCHSTICK_GAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CStatic m_move_right;
	CStatic m_move_left;
	CStatic m_move_up;
	CStatic m_move_down;

	afx_msg void OnBnClickedBtnStart();
	afx_msg void ShowAnswer();
	afx_msg void Restart();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
