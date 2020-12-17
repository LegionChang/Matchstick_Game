
// Matchstick_GameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Matchstick_Game.h"
#include "Matchstick_GameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//判断是否按下鼠标左键
bool btnDown = false;
//判断游戏是否结束
bool gameover = true;
//判断拖动的是哪一根火柴(两位数，十位存放火柴是哪一位[个十百千]，个位存放火柴是8中的哪一个)
static int matchstickNo;
//按照个十百千存放
static int numStart[4] = {0};
static int numMax [4] = {0};
static int numMin [4] = {0};
//当前游戏的数字对应火柴的显示状况,按个十百千存放
static int gameNum[7][4] = {1};

//用于显示数字
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

//用于判断是否是一个数,吧7位0、1看作对应的二进制数，转换成10进制数
static int judgeNum[10] = {
//	0		1		2		3		4		5		6		7		8		9	
	63,		6,		91,		79,		102,	109,	125,	7,		127,	111
};

//自移动数组
static int moveSelf[2][10] = {
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,
	6,		-1,		3,		2,		-1,		3,		0,		-1,		-1,		0,
	9,		-1,		-1,		5,		-1,		-1,		9,		-1,		-1,		6
};
//外添加数组
static int moveIn[2][10] = {
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,
	8,		7,		-1,		9,		-1,		6,		8,		-1,		-1,		8,
	-1,		-1,		-1,		-1,		-1,		9,		-1,		-1,		-1,		-1
};
//自删除数组
static int moveOut[2][10] = {
//	0,		1,		2,		3,		4,		5,		6,		7,		8,		9,
	-1,		-1,		-1,		-1,		-1,		-1,		5,		1,		0,		3,
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		9,		5
};

/****************************算法使用函数*******************************/
//存数字
void SaveNum(int inputNum[4]);
//拆分数字
int * GetUnitNum(int inputNum);
//合并数字
int GetCompleteNum(int inputNum[4]);
//判断是否可以开始游戏
BOOL NumIsOk();
//找最小
int GetMinNum();
//找最大
int GetMaxNum();

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMatchstick_GameDlg 对话框




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


// CMatchstick_GameDlg 消息处理程序

BOOL CMatchstick_GameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//隐藏用于移动的火柴
	m_move_right.ShowWindow(SW_HIDE);
	m_move_left.ShowWindow(SW_HIDE);
	m_move_up.ShowWindow(SW_HIDE);
	m_move_down.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_ANS)->EnableWindow(FALSE);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMatchstick_GameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMatchstick_GameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**************************鼠标按下*****************************/
void CMatchstick_GameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (gameover == false)
	{
		CRect rectPic;
		POINT pt = point;
		ClientToScreen(&pt);
		//判断要移动哪根火柴
		for (int i=0; i<4; ++i)
		{
			for (int j=0; j<7; ++j)
			{
				CRect rtTemp;
				CWnd * curMatchstick = GetDlgItem(1800 - 100*i + j + 1);
				curMatchstick->GetWindowRect(rtTemp);
				if ( rtTemp.PtInRect(pt) && gameNum[j][i] == 1) //鼠标按住了某根火柴
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

/**************************鼠标弹起*****************************/
void CMatchstick_GameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
						//判断显示出来的是否是一个数字

						gameNum[j][i] = 1;
						gameNum[matchstickNo%10][matchstickNo/10] = 0;
						int finalNum = 0;
						bool isCorrectNum = true; //如果4个数字都是正确的，则值为true
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
							AfxMessageBox(L"您得到的数字为："+ str);
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
					isBreak = true;//鼠标在某个火柴区域内，辅助跳出双重循环
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
			AfxMessageBox(L"火柴位置判断失误！");
			break;
		}
		btnDown = false;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

/**************************移动鼠标*****************************/
void CMatchstick_GameDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rtMvU, rtMvR, rtMvD, rtMvL;
	GetDlgItem(IDC_STC_MV_UP)->GetWindowRect(rtMvU);
	GetDlgItem(IDC_STC_MV_RIGHT)->GetWindowRect(rtMvR);
	GetDlgItem(IDC_STC_MV_DOWN)->GetWindowRect(rtMvD);
	GetDlgItem(IDC_STC_MV_LEFT)->GetWindowRect(rtMvL);

	POINT pt = point;


	if (btnDown) //鼠标按住了某个火柴
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
				if ( rtTemp.PtInRect(pt)) //移动到某根火柴的区域
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
							AfxMessageBox(L"火柴位置判断失误！");
							break;
						}
					}
					break;
				} else { //不在火柴的区域，或该区域的火柴已经是显示状态
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
				AfxMessageBox(L"火柴位置判断失误！");
				break;
			}

		}
		
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

/**************************开始游戏*****************************/
void CMatchstick_GameDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
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
			//显示数字
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
			AfxMessageBox(L"该数字无法进行游戏！");
		}
	} else {
		AfxMessageBox(L"请输入4位数字！");
	}
}


/**************************存数字*****************************/
void SaveNum(int inputNum[4])
{
	for (int i=0; i<4; ++i)
	{
		numStart[i] = inputNum[i];
	}
}

/**************************拆分数字*****************************/
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

/**************************合并数字*****************************/
int GetCompleteNum(int inputNum[4])
{
	int num =0;
	for (int i=3; i>=0; --i)
	{
		num = num * 10 + inputNum[i];
	}
	return num;
}

/**************************判断是否可以开始游戏*****************************/
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
	//每一个数字都在（1,4,7,8）中
	if (numStart[0]!=1 && numStart[1]!=1 && numStart[2]!=1 && numStart[3]!=1)
	{
		return false;
	}
	//4个数字中至少有一个是1
	for (int i=0; i<4; i++)
	{
		if (numStart[i]==7 || numStart[i]==8)
		{
			return true;
		}
	}
	return false;
}

/**************************找最小数*****************************/
int GetMinNum()
{
	int selfMin = 10000;

	//初始化最小数组
	for (int i=0; i<4; ++i)
	{
		numMin[i] = numStart[i];
	}
	//在自变化中找最小，最小数存放于selfMin中
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
	//在外变化中找最小,最小存放于selfMin中
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

/**************************找最大数*****************************/
int GetMaxNum()
{
	int selfMax = 0;

	//初始化最大数组
	for (int i=0; i<4; ++i)
	{
		numMax[i] = numStart[i];
	}

	//在自变化中找最大，最大数存放于selfMax中
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

	//在外变化中找最大
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

/**************************显示答案*****************************/
void CMatchstick_GameDlg::ShowAnswer()
{
	// TODO: 在此添加控件通知处理程序代码
	int intMinNum = GetMinNum();
	int intMaxNum = GetMaxNum();
	CString strMax, strMin;
	strMax.Format(L"%d", intMaxNum);
	strMin.Format(L"%d", intMinNum);
	GetDlgItem(IDC_STC_MAX)->SetWindowText(strMax);
	GetDlgItem(IDC_STC_MIN)->SetWindowText(strMin);
}

/**************************重新开始*****************************/
void CMatchstick_GameDlg::Restart()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedBtnStart();
}


BOOL CMatchstick_GameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return 0;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
