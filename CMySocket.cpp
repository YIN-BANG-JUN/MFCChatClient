#include "pch.h"
#include "CMySocket.h"

#include "MFCChatClient.h"
#include "MFCChatClientDlg.h"


CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
}

void CMySocket::OnConnect(int nErrorCode)
{
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();
	/*
	CString str;
	dlg->m_tm = CTime::GetCurrentTime();
	str = dlg->m_tm.Format("%X ");
	str += _T("与服务器连接成功!");
	*/
	CString strShow;
	CString strInfo = _T("");
	CString strMsg = _T("与服务端连接成功!");
	strShow = dlg->CatShowString(strInfo, strMsg);
	dlg->m_list.AddString(strShow);

	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();

	//接收数据到字符数组
	char szRecvBuf[MAX_SERVER_BUF] = { 0 };
	Receive(szRecvBuf, MAX_SERVER_BUF, 0);

	//显示接收到的字符
	USES_CONVERSION;
	CString strRecvMsg = A2W(szRecvBuf);
	/*
	CString strShow = _T("服务端 : ");
	CString strTime;
	dlg->m_tm = CTime::GetCurrentTime();
	strTime = dlg->m_tm.Format("%X ");

	strShow = strTime + strShow;
	strShow += strRecvMsg;
	*/
	CString strShow;
	CString strInfo = _T("服务端 : ");
	strShow = dlg->CatShowString(strInfo, strRecvMsg);

	dlg->m_list.AddString(strShow);

	if (dlg->m_uadd == 1)
	{
		//选中单选框后自动回复
		if (((CButton*)dlg->GetDlgItem(IDC_AUTOSEND_RADIO))->GetCheck())
		{
			//自动回复
			//1.获取编辑框内容
			CString strAutoSendMsg;
			dlg->GetDlgItem(IDC_AUTOSENDMSG_EDIT)->GetWindowTextW(strAutoSendMsg);
			//2.封包+组格式
			CString strName;
			dlg->GetDlgItem(IDC_NAME_EDIT)->GetWindowTextW(strName);
			CString strMsg = strName + _T(" : [自动回复] : ") + strAutoSendMsg;

			//3.回复
			char* szSendBuf = T2A(strMsg);
			dlg->m_client->Send(szSendBuf, MAX_SERVER_BUF, 0);

			//4.更新显示
			CString strShow;
			strShow = dlg->CatShowString(_T(""), strMsg);
			dlg->m_list.AddString(strShow);
			dlg->m_list.UpdateData(FALSE);
		}
	}
	else if (dlg->m_uadd == 2)
	{
		//选中复选框后自动回复
		if (((CButton*)dlg->GetDlgItem(IDC_AUTOSEND_CHECK))->GetCheck())
		{
			//自动回复
			//1.获取编辑框内容
			CString strAutoSendMsg;
			dlg->GetDlgItem(IDC_AUTOSENDMSG_EDIT)->GetWindowTextW(strAutoSendMsg);
			//2.封包+组格式
			CString strName;
			dlg->GetDlgItem(IDC_NAME_EDIT)->GetWindowTextW(strName);
			CString strMsg = strName + _T(" : [自动回复] : ") + strAutoSendMsg;

			//3.回复
			char* szSendBuf = T2A(strMsg);
			dlg->m_client->Send(szSendBuf, MAX_SERVER_BUF, 0);

			//4.更新显示
			CString strShow;
			strShow = dlg->CatShowString(_T(""), strMsg);
			dlg->m_list.AddString(strShow);
			dlg->m_list.UpdateData(FALSE);
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}
