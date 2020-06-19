﻿
// PhpShellCmdDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CPhpShellCmdDlg 对话框
class CPhpShellCmdDlg : public CDialog
{
// 构造
public:
	CPhpShellCmdDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PHPSHELLCMD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAttack();
	afx_msg void OnBnClickedBtnCheck();
	CEdit m_editCmd;
	// 输出结果编辑框
	CEdit m_output;
	CEdit m_adress;
};
