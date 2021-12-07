
// MFCChatClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCChatClient.h"
#include "MFCChatClientDlg.h"
#include "afxdialogex.h"

#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//	TRACE(_T(" "), GetLastError());

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCChatClientDlg 对话框



CMFCChatClientDlg::CMFCChatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCHATCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_client = nullptr;
	m_list;
	m_tm;
	m_input;
	m_buf[0] = { 0 };
	m_uadd = 0;
	m_WordColorCombo;
}

void CMFCChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_LIST, m_list);
	DDX_Control(pDX, IDC_SENDMSG_EDIT, m_input);
	DDX_Control(pDX, IDC_COLOUR_COMBO, m_WordColorCombo);
}

BEGIN_MESSAGE_MAP(CMFCChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT_BTN, &CMFCChatClientDlg::OnBnClickedConnectBtn)
	ON_BN_CLICKED(IDC_SEND_BTN, &CMFCChatClientDlg::OnBnClickedSendBtn)
	ON_BN_CLICKED(IDC_SAVENAME_BTN, &CMFCChatClientDlg::OnBnClickedSavenameBtn)
	ON_BN_CLICKED(IDC_AUTOSEND_RADIO, &CMFCChatClientDlg::OnBnClickedAutosendRadio)
	ON_BN_CLICKED(IDC_CLEARMSG_BTN, &CMFCChatClientDlg::OnBnClickedClearmsgBtn)
	ON_BN_CLICKED(IDC_AUTOSEND_CHECK, &CMFCChatClientDlg::OnBnClickedAutosendCheck)
	ON_BN_CLICKED(IDC_DISCONNECT_BTN, &CMFCChatClientDlg::OnBnClickedDisconnectBtn)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMFCChatClientDlg 消息处理程序

BOOL CMFCChatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//初始化控件为不能点击和能点击
	GetDlgItem(IDC_DISCONNECT_BTN)->EnableWindow(FALSE); //断开按钮
	GetDlgItem(IDC_CONNECT_BTN)->EnableWindow(TRUE);	 //连接按钮
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(FALSE);		//消息发送按钮
	GetDlgItem(IDC_CLEARMSG_BTN)->EnableWindow(FALSE);	//清除历史消息
	GetDlgItem(IDC_SAVENAME_BTN)->EnableWindow(FALSE);	//确认呢称按钮

	//初始化字体颜色
	m_WordColorCombo.AddString(_T("黑色"));
	m_WordColorCombo.AddString(_T("红色"));
	m_WordColorCombo.AddString(_T("蓝色"));
	m_WordColorCombo.AddString(_T("绿色"));
	//设置默认字体颜色
	m_WordColorCombo.SetCurSel(0); //黑色
	SetDlgItemText(IDC_COLOUR_COMBO, _T("黑色"));

	//设置默认端口值
	GetDlgItem(IDC_PORT_EDIT)->SetWindowText(_T("5000"));
	//设置默认IP值
	GetDlgItem(IDC_IPADDRESS)->SetWindowText(_T("127.0.0.1"));

	//启动时从配置文件中读取呢称
	//读取文件
	WCHAR wszName[MAX_PATH] = { 0 };
	WCHAR strPath[MAX_PATH] = { 0 };
	//获取当前路径
	GetCurrentDirectoryW(MAX_PATH, strPath);
	CString strFilePath;
	strFilePath.Format(_T("%ls//Test.ini"), strPath);
	DWORD dwNum = GetPrivateProfileStringW(_T("CLIENT"), _T("NAME"), NULL, wszName, MAX_PATH, strFilePath);
	if (dwNum > 0) {
		//设置编辑框内容
		SetDlgItemText(IDC_NAME_EDIT, wszName);
		UpdateData(FALSE);
	}
	else {
		WritePrivateProfileStringW(_T("CLIENT"), _T("NAME"), _T("客户端"), strFilePath);
		//设置编辑框内容
		SetDlgItemText(IDC_NAME_EDIT, _T("客户端"));
		UpdateData(FALSE);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCChatClientDlg::OnPaint()
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
		//绘制背景图片
		CPaintDC dc(this); // 用于绘制的设备上下文
		CRect rect;
		GetClientRect(&rect);
		CDC dcBmp;
		dcBmp.CreateCompatibleDC(&dcBmp);
		CBitmap bmpBackGround;
		bmpBackGround.LoadBitmap(IDB_BMP2_BITMAP);
		BITMAP bBitmap;
		bmpBackGround.GetBitmap(&bBitmap);
		CBitmap* pbmpOld = dcBmp.SelectObject(&bmpBackGround);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, bBitmap.bmWidth, bBitmap.bmHeight, SRCCOPY);

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//连接
void CMFCChatClientDlg::OnBnClickedConnectBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	//初始化控件为不能点击和能点击
	GetDlgItem(IDC_DISCONNECT_BTN)->EnableWindow(TRUE); //断开按钮
	GetDlgItem(IDC_CONNECT_BTN)->EnableWindow(FALSE);	 //连接按钮
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(TRUE);		//消息发送按钮
	GetDlgItem(IDC_CLEARMSG_BTN)->EnableWindow(TRUE);	//清除历史消息
	GetDlgItem(IDC_SAVENAME_BTN)->EnableWindow(TRUE);	//确认呢称按钮

	//从控件里面获取端口和IP
	CString strPort, strIP;
	GetDlgItem(IDC_PORT_EDIT)->GetWindowText(strPort);
	GetDlgItem(IDC_IPADDRESS)->GetWindowText(strIP);

	//CString转char*
	USES_CONVERSION;
	LPCSTR szPort = (LPCSTR)T2A(strPort);
	LPCSTR szIP = (LPCSTR)T2A(strIP);

	int iPort = _ttoi(strPort);

	//创建套接字对象
	m_client = new CMySocket;

	//创建套接字
	if (!m_client->Create()) return;

	//连接网络
	if (m_client->Connect(strIP, iPort) != SOCKET_ERROR) return;
}

//消息合并
CString CMFCChatClientDlg::CatShowString(CString strInfo, CString strMsg)
{
	CString strTime;
	CTime tmNow;
	tmNow = CTime::GetCurrentTime();
	strTime = tmNow.Format("%X ");

	CString strShow;
	strShow = strTime + strShow;
	strShow += strInfo;
	strShow += strMsg;

	return strShow;
}

//消息发送
void CMFCChatClientDlg::OnBnClickedSendBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取编辑框内容
	CString strTmpMsg;
	GetDlgItem(IDC_SENDMSG_EDIT)->GetWindowTextW(strTmpMsg);

	CString strName;
	GetDlgItem(IDC_NAME_EDIT)->GetWindowTextW(strName);
	strTmpMsg = strName + _T(" : ") + strTmpMsg;

	USES_CONVERSION;
	char* szSendBuf = T2A(strTmpMsg);

	//发送给服务端
	m_client->Send(szSendBuf, MAX_SERVER_BUF, 0);

	//消息显示到列表框
	/*
	CString strShow = _T("我 : ");
	CString strTime;
	m_tm = CTime::GetCurrentTime();
	strTime = m_tm.Format("%X ");

	strShow = strTime + strShow;
	strShow += strTmpMsg;
	*/
	CString strShow;
	CString strInfo = _T("");
	strShow = CatShowString(_T(""), strTmpMsg);

	m_list.AddString(strShow);
	UpdateData(FALSE);

	GetDlgItem(IDC_SENDMSG_EDIT)->SetWindowTextW(_T(" "));
}

//保存呢称到配置文件
void CMFCChatClientDlg::OnBnClickedSavenameBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strName;
	//读取编辑框内容
	GetDlgItemText(IDC_NAME_EDIT, strName);
	if (strName.GetLength() <= 0)
	{
		MessageBox(_T("呢称不能为空!"));
		return;
	}

	//判断是否修改呢称
	if (IDOK == AfxMessageBox(_T("确认修改呢称吗?"), MB_OKCANCEL))
	{
		//保存到文件
		WCHAR strPath[MAX_PATH] = { 0 };
		CString strFilePath;
		//获取当前路径
		GetCurrentDirectoryW(MAX_PATH, strPath);
		strFilePath.Format(_T("%ls//Test.ini"), strPath);
		//保存内容到文件
		WritePrivateProfileStringW(_T("CLIENT"), _T("NAME"), strName, strFilePath);
	}
}

//单选框
void CMFCChatClientDlg::OnBnClickedAutosendRadio()
{
	// TODO: 在此添加控件通知处理程序代码

	//当选中单选框再次单击单选框时
	if (((CButton*)GetDlgItem(IDC_AUTOSEND_RADIO))->GetCheck())
	{
		//将单选框设置为未选中
		((CButton*)GetDlgItem(IDC_AUTOSEND_RADIO))->SetCheck(FALSE);
	}
	else {
		//否则为选中
		((CButton*)GetDlgItem(IDC_AUTOSEND_RADIO))->SetCheck(TRUE);
	}
	m_uadd = 1;
}

//清除历史消息
void CMFCChatClientDlg::OnBnClickedClearmsgBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	//清除历史记录数据
	m_list.ResetContent();
}

//复选框
void CMFCChatClientDlg::OnBnClickedAutosendCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	// 
	//当选中复选框再次单击复选框时
	if (((CButton*)GetDlgItem(IDC_AUTOSEND_CHECK))->GetCheck())
	{
		//将复选框设置为未选中
		((CButton*)GetDlgItem(IDC_AUTOSEND_CHECK))->SetCheck(FALSE);
	}
	else {
		//否则为选中
		((CButton*)GetDlgItem(IDC_AUTOSEND_CHECK))->SetCheck(TRUE);
	}
	m_uadd = 2;
}

//断开
void CMFCChatClientDlg::OnBnClickedDisconnectBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	//初始化控件为不能点击和能点击
	GetDlgItem(IDC_DISCONNECT_BTN)->EnableWindow(FALSE); //断开按钮
	GetDlgItem(IDC_CONNECT_BTN)->EnableWindow(TRUE);	 //连接按钮
	GetDlgItem(IDC_SEND_BTN)->EnableWindow(FALSE);		//消息发送按钮
	GetDlgItem(IDC_CLEARMSG_BTN)->EnableWindow(FALSE);	//清除历史消息
	GetDlgItem(IDC_SAVENAME_BTN)->EnableWindow(FALSE);	//确认呢称按钮

	//处理回收资源
	m_client->Close();
	if (m_client != NULL)
	{
		delete m_client;
		m_client = NULL;
	}

	//显示到列表框
	CString strShow;
	strShow = CatShowString(_T(""), _T("与服务端断开成功!"));

	m_list.AddString(strShow);
	UpdateData(FALSE);
}

//设置字体颜色
HBRUSH CMFCChatClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	//获取字体颜色控件变量
	CString strColor;
	m_WordColorCombo.GetWindowTextW(strColor);

	if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SENDMSG_EDIT == pWnd->GetDlgCtrlID())
	{
		if (strColor == _T("黑色"))
		{
			pDC->SetTextColor(RGB(0, 0, 0));
		}
		else if (strColor == _T("红色"))
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		else if (strColor == _T("蓝色"))
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
		else if (strColor == _T("绿色"))
		{
			pDC->SetTextColor(RGB(0, 255, 0));
		}
	}
	/*
	if (strColor == _T("黑色"))
	{
		if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SENDMSG_EDIT == pWnd->GetDlgCtrlID())
		{
			pDC->SetTextColor(RGB(0, 0, 0));
		}
	}
	else if (strColor == _T("红色"))
	{
		if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SENDMSG_EDIT == pWnd->GetDlgCtrlID())
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
	}
	else if (strColor == _T("蓝色"))
	{
		if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SENDMSG_EDIT == pWnd->GetDlgCtrlID())
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
	}
	else if (strColor == _T("绿色"))
	{
		if (IDC_MSG_LIST == pWnd->GetDlgCtrlID() || IDC_SENDMSG_EDIT == pWnd->GetDlgCtrlID())
		{
			pDC->SetTextColor(RGB(0, 255, 0));
		}
	}
	*/
	return hbr;
}

//规避回车键
BOOL CMFCChatClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//按下键盘-回车键
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;

	//按下键盘-空格键
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) return TRUE;

	//添加快捷键 如Ctrl + 按键
	//按下键盘且Ctrl且按键
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetKeyState(VK_CONTROL) < 0)
		{
			if (pMsg->wParam == 'X' || pMsg->wParam == 'x')
			{
				CDialog::OnOK();
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
