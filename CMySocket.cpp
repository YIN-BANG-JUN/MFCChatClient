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
	str += _T("����������ӳɹ�!");
	*/
	CString strShow;
	CString strInfo = _T("");
	CString strMsg = _T("���������ӳɹ�!");
	strShow = dlg->CatShowString(strInfo, strMsg);
	dlg->m_list.AddString(strShow);

	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode)
{
	CMFCChatClientDlg* dlg = (CMFCChatClientDlg*)AfxGetApp()->GetMainWnd();

	//�������ݵ��ַ�����
	char szRecvBuf[MAX_SERVER_BUF] = { 0 };
	Receive(szRecvBuf, MAX_SERVER_BUF, 0);

	//��ʾ���յ����ַ�
	USES_CONVERSION;
	CString strRecvMsg = A2W(szRecvBuf);
	/*
	CString strShow = _T("����� : ");
	CString strTime;
	dlg->m_tm = CTime::GetCurrentTime();
	strTime = dlg->m_tm.Format("%X ");

	strShow = strTime + strShow;
	strShow += strRecvMsg;
	*/
	CString strShow;
	CString strInfo = _T("����� : ");
	strShow = dlg->CatShowString(strInfo, strRecvMsg);

	dlg->m_list.AddString(strShow);

	if (dlg->m_uadd == 1)
	{
		//ѡ�е�ѡ����Զ��ظ�
		if (((CButton*)dlg->GetDlgItem(IDC_AUTOSEND_RADIO))->GetCheck())
		{
			//�Զ��ظ�
			//1.��ȡ�༭������
			CString strAutoSendMsg;
			dlg->GetDlgItem(IDC_AUTOSENDMSG_EDIT)->GetWindowTextW(strAutoSendMsg);
			//2.���+���ʽ
			CString strName;
			dlg->GetDlgItem(IDC_NAME_EDIT)->GetWindowTextW(strName);
			CString strMsg = strName + _T(" : [�Զ��ظ�] : ") + strAutoSendMsg;

			//3.�ظ�
			char* szSendBuf = T2A(strMsg);
			dlg->m_client->Send(szSendBuf, MAX_SERVER_BUF, 0);

			//4.������ʾ
			CString strShow;
			strShow = dlg->CatShowString(_T(""), strMsg);
			dlg->m_list.AddString(strShow);
			dlg->m_list.UpdateData(FALSE);
		}
	}
	else if (dlg->m_uadd == 2)
	{
		//ѡ�и�ѡ����Զ��ظ�
		if (((CButton*)dlg->GetDlgItem(IDC_AUTOSEND_CHECK))->GetCheck())
		{
			//�Զ��ظ�
			//1.��ȡ�༭������
			CString strAutoSendMsg;
			dlg->GetDlgItem(IDC_AUTOSENDMSG_EDIT)->GetWindowTextW(strAutoSendMsg);
			//2.���+���ʽ
			CString strName;
			dlg->GetDlgItem(IDC_NAME_EDIT)->GetWindowTextW(strName);
			CString strMsg = strName + _T(" : [�Զ��ظ�] : ") + strAutoSendMsg;

			//3.�ظ�
			char* szSendBuf = T2A(strMsg);
			dlg->m_client->Send(szSendBuf, MAX_SERVER_BUF, 0);

			//4.������ʾ
			CString strShow;
			strShow = dlg->CatShowString(_T(""), strMsg);
			dlg->m_list.AddString(strShow);
			dlg->m_list.UpdateData(FALSE);
		}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}
