
// Matchstick_GameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Matchstick_Game.h"
#include "Matchstick_GameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//�ж��Ƿ���������
bool btnDown = false;
//�ж���Ϸ�Ƿ����
bool gameover = true;
//�ж��϶�������һ�����(��λ����ʮλ��Ż������һλ[��ʮ��ǧ]����λ��Ż����8�е���һ��)
static int matchstickNo;
//���ո�ʮ��ǧ���
static int numStart[4] = {0};
static int numMax [4] = {0};
static int numMin [4] = {0};
//��ǰ��Ϸ�����ֶ�Ӧ������ʾ״��,����ʮ��ǧ���
static int gameNum[7][4] = {1};

//������ʾ����
static int showNum[7][10] = {
//	0		1		2		3		4		5		6		7		8		9	
	1,		0,		1,		1,		0,		1,		1,		1,		1,		1,
	1,		1,		1,		1,		1,		0,		0,		1,		1,		1,
	1,		1,		0,		1,		1,		1,		1,		1,		1,		1,
	1,		0,		1,		1,		0,		1,		1,		0,		1,		1,
	1,		0,		1,		0,		0,		0,		1,		0,		1,		0,
	1,		0,		0,		0,		1,		1,		1,		0,		1,		1,
	0,		0,		1,		1,		1,		1,		1,		0,		1,		1
};

//�����ж��Ƿ���һ����,��7λ0��1������Ӧ�Ķ���������ת����10������
static int judgeNum[10] = {
//	0		1		2		3		4		5		6		7		8		9	
	63,		6,		91,		79,		102,	109,	125,	7,		127,	111
};

//���ƶ�����
static int moveSelf[2][10] = {
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,
	6,		-1,		3,		2,		-1,		3,		0,		-1,		-1,		0,
	9,		-1,		-1,		5,		-1,		-1,		9,		-1,		-1,		6
};
//���������
static int moveIn[2][10] = {
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,
	8,		7,		-1,		9,		-1,		6,		8,		-1,		-1,		8,
	-1,		-1,		-1,		-1,		-1,		9,		-1,		-1,		-1,		-1
};
//��ɾ������
static int moveOut[2][10] = {
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,
	-1,		-1,		-1,		-1,		-1,		-1,		5,		1,		0,		3,
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		9,		5
};

/****************************�㷨ʹ�ú���*******************************/
//������
void SaveNum(int inputNum[4]);
//�������
int * GetUnitNum(int inputNum);
//�ϲ�����
int GetCompleteNum(int inputNum[4]);
//�ж��Ƿ���Կ�ʼ��Ϸ
BOOL NumIsOk();
//����С
int GetMinNum();
//�����
int GetMaxNum();

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMatchstick_GameDlg �Ի���




CMatchstick_GameDlg::CMatchstick_GameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMatchstick_GameDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMatchstick_GameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_MV_RIGHT, m_move_right);
	DDX_Control(pDX, IDC_STC_MV_LEFT, m_move_left);
	DDX_Control(pDX, IDC_STC_MV_UP, m_move_up);
	DDX_Control(pDX, IDC_STC_MV_DOWN, m_move_down);
}

BEGIN_MESSAGE_MAP(CMatchstick_GameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_START, &CMatchstick_GameDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_ANS, &CMatchstick_GameDlg::ShowAnswer)
	ON_BN_CLICKED(IDC_BTN_RESTART, &CMatchstick_GameDlg::Restart)
END_MESSAGE_MAP()


// CMatchstick_GameDlg ��Ϣ�������

BOOL CMatchstick_GameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//���������ƶ��Ļ��
	m_move_right.ShowWindow(SW_HIDE);
	m_move_left.ShowWindow(SW_HIDE);
	m_move_up.ShowWindow(SW_HIDE);
	m_move_down.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ANS)->EnableWindow(FALSE);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMatchstick_GameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMatchstick_GameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMatchstick_GameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**************************��갴��*****************************/
void CMatchstick_GameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (gameover == false)
	{
		CRect rectPic;
		POINT pt = point;
		ClientToScreen(&pt);
		//�ж�Ҫ�ƶ��ĸ����
		for (int i=0; i<4; ++i)
		{
			for (int j=0; j<7; ++j)
			{
				CRect rtTemp;
				CWnd * curMatchstick = GetDlgItem(1800 - 100*i + j + 1);
				curMatchstick->GetWindowRect(rtTemp);
				if ( rtTemp.PtInRect(pt) && gameNum[j][i] == 1) //��갴ס��ĳ�����
				{
					btnDown = true;
					curMatchstick->ShowWindow(SW_HIDE);
					matchstickNo = i*10 + j;
					switch (j)
					{
					case 1:
					case 2:
						m_move_down.ShowWindow(SW_SHOW);
						break;
					case 4:
					case 5:
						m_move_up.ShowWindow(SW_SHOW);
						break;
					case 3:
					case 6:
						m_move_left.ShowWindow(SW_SHOW);
						break;
					case 0:
						m_move_right.ShowWindow(SW_SHOW);
						break;
					}
					break;
				}
			}
		}
	}//if(gameover == false)

	CDialogEx::OnLButtonDown(nFlags, point);
}

/**************************��굯��*****************************/
void CMatchstick_GameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (gameover == false)
	{
		POINT pt = point;
		ClientToScreen(&pt);
		for (int i=0; i<4; ++i)
		{
			bool isBreak = false;
			for (int j=0; j<7; ++j)
			{
				CRect rtTemp;
				CWnd * curMatchstick = GetDlgItem(1800 - 100*i + j + 1);
				curMatchstick->GetWindowRect(rtTemp);
				if (rtTemp.PtInRect(pt))
				{
					if (gameNum[j][i]==0)
					{
						//�ж���ʾ�������Ƿ���һ������

						gameNum[j][i] = 1;
						gameNum[matchstickNo%10][matchstickNo/10] = 0;
						int finalNum = 0;
						bool isCorrectNum = true; //���4�����ֶ�����ȷ�ģ���ֵΪtrue
						for (int a=3; a>=0; --a)
						{
							int resNum = -1;
							int intJudgeNum = 0;
							for (int b=6; b>=0; --b)
							{
								intJudgeNum = intJudgeNum*2 +gameNum[b][a];
							}
							for (int c=0; c<10;)
							{
								if (intJudgeNum == judgeNum[c])
								{
									finalNum = finalNum * 10 + c;
									break;
								}
								 ++c;
								 if (c == 10)
								 {
									 isCorrectNum = false;
								 }
							}
							if (isCorrectNum == false)
							{
								break;
							}
						}
						if (isCorrectNum)
						{
							curMatchstick->ShowWindow(SW_SHOW);
							GetDlgItem(1800 - 100*(matchstickNo/10) + matchstickNo%10 + 1)->ShowWindow(SW_HIDE);

							CString str;
							str.Format(L"%d",finalNum);
							AfxMessageBox(L"���õ�������Ϊ��"+ str);
							gameover = true;
							
						} else {
							curMatchstick->ShowWindow(SW_HIDE);
							GetDlgItem(1800 - 100*(matchstickNo/10) + matchstickNo%10 + 1)->ShowWindow(SW_SHOW);
							
						}
						gameNum[j][i] = 0;
						gameNum[matchstickNo%10][matchstickNo/10] = 1;
					} else {
						GetDlgItem(1800 - 100*(matchstickNo/10) + matchstickNo%10 + 1)->ShowWindow(SW_SHOW);
					}
					isBreak = true;//�����ĳ����������ڣ���������˫��ѭ��
					break;
				} else {
					GetDlgItem(1800 - 100*(matchstickNo/10) + matchstickNo%10 + 1)->ShowWindow(SW_SHOW);
				}
			}
			if (isBreak)
			{
				break;
			}
		}
		switch (matchstickNo%10)
		{
		case 1:
		case 2:
			m_move_down.ShowWindow(SW_HIDE);
			break;
		case 4:
		case 5:
			m_move_up.ShowWindow(SW_HIDE);
			break;
		case 3:
		case 6:
			m_move_left.ShowWindow(SW_HIDE);
			break;
		case 0:
			m_move_right.ShowWindow(SW_HIDE);
			break;
		default:
			AfxMessageBox(L"���λ���ж�ʧ��");
			break;
		}
		btnDown = false;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

/**************************�ƶ����*****************************/
void CMatchstick_GameDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rtMvU, rtMvR, rtMvD, rtMvL;
	GetDlgItem(IDC_STC_MV_UP)->GetWindowRect(rtMvU);
	GetDlgItem(IDC_STC_MV_RIGHT)->GetWindowRect(rtMvR);
	GetDlgItem(IDC_STC_MV_DOWN)->GetWindowRect(rtMvD);
	GetDlgItem(IDC_STC_MV_LEFT)->GetWindowRect(rtMvL);

	POINT pt = point;


	if (btnDown) //��갴ס��ĳ�����
	{
		ClientToScreen(&pt);

		bool flag = false;

		for (int i=0; i<4; ++i)
		{
			for (int j=0; j<7; ++j)
			{
				CRect rtTemp;
				CWnd * curMatchstick = GetDlgItem(1800 - 100*i + j + 1);
				curMatchstick->GetWindowRect(rtTemp);
				if ( rtTemp.PtInRect(pt)) //�ƶ���ĳ����������
				{
					if (gameNum[j][i] == 0)
					{
						flag = true;
						curMatchstick->ShowWindow(SW_SHOW);
				
						switch (matchstickNo%10)
						{
						case 1:
						case 2:
							m_move_down.ShowWindow(SW_HIDE);
							break;
						case 4:
						case 5:
							m_move_up.ShowWindow(SW_HIDE);
							break;
						case 3:
						case 6:
							m_move_left.ShowWindow(SW_HIDE);
							break;
						case 0:
							m_move_right.ShowWindow(SW_HIDE);
							break;
						default:
							AfxMessageBox(L"���λ���ж�ʧ��");
							break;
						}
					}
					break;
				} else { //���ڻ������򣬻������Ļ���Ѿ�����ʾ״̬
					if (gameNum[j][i] == 0)
					{
						curMatchstick->ShowWindow(SW_HIDE);
					}
				}
			}//for
		}//for

		if (flag == false)
		{
			ScreenToClient(&pt);
			switch (matchstickNo%10)
			{
			case 1:
			case 2:
				m_move_down.ShowWindow(SW_SHOW);
				m_move_down.MoveWindow(pt.x, pt.y, rtMvD.Width(), rtMvD.Height());
				break;
			case 4:
			case 5:
				m_move_up.ShowWindow(SW_SHOW);
				m_move_up.MoveWindow(pt.x, pt.y, rtMvU.Width(), rtMvU.Height());
				break;
			case 3:
			case 6:
				m_move_left.ShowWindow(SW_SHOW);
				m_move_left.MoveWindow(pt.x, pt.y, rtMvL.Width(), rtMvL.Height());
				break;
			case 0:
				m_move_right.ShowWindow(SW_SHOW);
				m_move_right.MoveWindow(pt.x, pt.y,rtMvR.Width(), rtMvR.Height());
				break;
			default:
				AfxMessageBox(L"���λ���ж�ʧ��");
				break;
			}

		}
		
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

/**************************��ʼ��Ϸ*****************************/
void CMatchstick_GameDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	gameover = false;
	GetDlgItem(IDC_STC_MAX)->SetWindowText(L"****");
	GetDlgItem(IDC_STC_MIN)->SetWindowText(L"****");

	CString sNum;
	GetDlgItem(IDC_EDT_START_NUM)->GetWindowText(sNum);
	int num_suan;
	num_suan = _ttoi(sNum);

	if (num_suan >= 0 && num_suan <= 9999)
	{
		static int * pNum;
		pNum = GetUnitNum(num_suan);
		SaveNum(pNum);

		if (NumIsOk())
		{
			//��ʾ����
			for (int i=0; i<4; ++i)
			{
				for (int j=0; j<7; ++j)
				{
					if (showNum[j][numStart[i]] == 0)
					{
						GetDlgItem(1800 - 100*i + j + 1)->ShowWindow(SW_HIDE);
						gameNum[j][i] = 0;
					} else {
						GetDlgItem(1800 - 100*i + j + 1)->ShowWindow(SW_SHOW);
						gameNum[j][i] = 1;
					}
				}
			}
			GetDlgItem(IDC_BTN_ANS)->EnableWindow(TRUE);
			
		} else {
			AfxMessageBox(L"�������޷�������Ϸ��");
		}
	} else {
		AfxMessageBox(L"������4λ���֣�");
	}
}


/**************************������*****************************/
void SaveNum(int inputNum[4])
{
	for (int i=0; i<4; ++i)
	{
		numStart[i] = inputNum[i];
	}
}

/**************************�������*****************************/
int * GetUnitNum(int inputNum)
{
	static int res[4] = {0};
	for (int i=0; i<4; ++i)
	{
		res[i] = inputNum % 10;
		inputNum = inputNum / 10;
	}
	return res;
}

/**************************�ϲ�����*****************************/
int GetCompleteNum(int inputNum[4])
{
	int num =0;
	for (int i=3; i>=0; --i)
	{
		num = num * 10 + inputNum[i];
	}
	return num;
}

/**************************�ж��Ƿ���Կ�ʼ��Ϸ*****************************/
BOOL NumIsOk()
{
	for (int i=0; i<4; ++i)
	{
		if (numStart[i]==1 || numStart[i]==4 || numStart[i]==7 || numStart[i]==8)
		{

		} else{
			return true;
		}
	}
	//ÿһ�����ֶ��ڣ�1,4,7,8����
	if (numStart[0]!=1 && numStart[1]!=1 && numStart[2]!=1 && numStart[3]!=1)
	{
		return false;
	}
	//4��������������һ����1
	for (int i=0; i<4; i++)
	{
		if (numStart[i]==7 || numStart[i]==8)
		{
			return true;
		}
	}
	return false;
}

/**************************����С��*****************************/
int GetMinNum()
{
	int selfMin = 10000;

	//��ʼ����С����
	for (int i=0; i<4; ++i)
	{
		numMin[i] = numStart[i];
	}
	//���Ա仯������С����С�������selfMin��
	for (int i=0; i<4; ++i)
	{
		int preNum = numStart[i];
		if ( moveSelf[0][preNum] != -1 )
		{
			int temp = numMin[i];
			numMin[i] = moveSelf[0][preNum];
			if (GetCompleteNum(numMin) < selfMin)
			{
				selfMin = GetCompleteNum(numMin);
			}
			numMin[i] = temp;
		}
	}
	//����仯������С,��С�����selfMin��
	for (int i=0; i<4; ++i)
	{
		int preNumIn = numStart[i];
		for (int j=0; j<4; ++j)
		{
			if (i==j)
			{
				continue;
			}
			int preNumOut = numStart[j];
			if (moveIn[0][preNumIn]!=-1 && moveOut[0][preNumOut]!=-1)
			{
				int temp1 = numMin[i];
				int temp2 = numMin[j];
				numMin[i] = moveIn[0][preNumIn];
				numMin[j] = moveOut[0][preNumOut];
				if (GetCompleteNum(numMin) < selfMin)
				{
					selfMin = GetCompleteNum(numMin);
				}
				numMin[i] = temp1;
				numMin[j] = temp2;
			}
		}
	}

	return selfMin;

}

/**************************�������*****************************/
int GetMaxNum()
{
	int selfMax = 0;

	//��ʼ���������
	for (int i=0; i<4; ++i)
	{
		numMax[i] = numStart[i];
	}

	//���Ա仯�����������������selfMax��
	for (int i=0; i<4; ++i)
	{
		int preNum = numStart[i];
		if ( moveSelf[0][preNum] != -1 )
		{
			int temp = numMax[i];

			if (moveSelf[1][preNum] != -1)
			{
				numMax[i] = moveSelf[1][preNum];
			}else{
				numMax[i] = moveSelf[0][preNum];
			}
			if (GetCompleteNum(numMax) > selfMax)
			{
				selfMax = GetCompleteNum(numMax);
			}
			numMax[i] = temp;
		}
	}

	//����仯�������
	for (int i=0; i<4; ++i)
	{
		int preNumIn = numStart[i];
		for (int j=0; j<4; ++j)
		{
			if (i==j)
			{
				continue;
			}
			int preNumOut = numStart[j];
			if (moveIn[0][preNumIn]!=-1 && moveOut[0][preNumOut]!=-1)
			{
				int temp1 = numMax[i];
				int temp2 = numMax[j];

				if (moveIn[1][preNumIn]!=-1)
				{
					numMax[i] = moveIn[1][preNumIn];
				} else {
					numMax[i] = moveIn[0][preNumIn];
				}

				if (moveOut[1][preNumOut]!=-1)
				{
					numMax[j] = moveOut[1][preNumOut];
				} else {
					numMax[j] = moveOut[0][preNumOut];
				}

				if (GetCompleteNum(numMax) > selfMax)
				{
					selfMax = GetCompleteNum(numMax);
				}

				numMax[i] = temp1;
				numMax[j] = temp2;
			}
		}
	}

	return selfMax;
}

/**************************��ʾ��*****************************/
void CMatchstick_GameDlg::ShowAnswer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int intMinNum = GetMinNum();
	int intMaxNum = GetMaxNum();
	CString strMax, strMin;
	strMax.Format(L"%d", intMaxNum);
	strMin.Format(L"%d", intMinNum);
	GetDlgItem(IDC_STC_MAX)->SetWindowText(strMax);
	GetDlgItem(IDC_STC_MIN)->SetWindowText(strMin);
}

/**************************���¿�ʼ*****************************/
void CMatchstick_GameDlg::Restart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedBtnStart();
}


BOOL CMatchstick_GameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return 0;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
