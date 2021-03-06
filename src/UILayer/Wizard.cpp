/* 
 * $Id: Wizard.cpp 12458 2009-04-27 10:31:25Z huby $
 * 
 * this file is part of eMule
 * Copyright (C)2002-2006 Merkur ( strEmail.Format("%s@%s", "devteam", "emule-project.net") / http://www.emule-project.net )
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "stdafx.h"
#include "emule.h"
#include "PreferencesDlg.h"
#include "Wizard.h"
#include "emuledlg.h"
#include "StatisticsDlg.h"
#include "opcodes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CConnectionWizardDlg dialog

IMPLEMENT_DYNAMIC(CConnectionWizardDlg, CDlgPageWizard)

BEGIN_MESSAGE_MAP(CConnectionWizardDlg, CDlgPageWizard)
	ON_BN_CLICKED(IDC_WIZ_APPLY_BUTTON, OnBnClickedApply)
	ON_BN_CLICKED(IDC_WIZ_CANCEL_BUTTON, OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_WIZ_XP_RADIO, OnBnClickedWizRadioOsNtxp)
	//ON_BN_CLICKED(IDC_WIZ_ME_RADIO, OnBnClickedWizRadioUs98me)
	ON_BN_CLICKED(IDC_WIZ_LOWDOWN_RADIO, OnBnClickedWizLowdownloadRadio)
	ON_BN_CLICKED(IDC_WIZ_MEDIUMDOWN_RADIO, OnBnClickedWizMediumdownloadRadio)
	ON_BN_CLICKED(IDC_WIZ_HIGHDOWN_RADIO, OnBnClickedWizHighdownloadRadio)
	ON_NOTIFY(NM_CLICK, IDC_PROVIDERS, OnNMClickProviders)
	ON_BN_CLICKED(IDC_KBITS, OnBnClickedKbits)
	ON_BN_CLICKED(IDC_KBYTES, OnBnClickedKbytes)
END_MESSAGE_MAP()

//CConnectionWizardDlg::CConnectionWizardDlg(CWnd* /*pParent*/ /*=NULL*/)
//	: CDlgPageWizard(CConnectionWizardDlg::IDD/*, pParent*/)
//{
//	m_iOS = 0;
//	m_iTotalDownload = 0;
//	m_icnWnd = NULL;
//}

CConnectionWizardDlg::CConnectionWizardDlg(LPCTSTR pszCaption, LPCTSTR pszHeaderTitle, LPCTSTR pszHeaderSubTitle)
	: CDlgPageWizard(CConnectionWizardDlg::IDD, pszCaption, pszHeaderTitle, pszHeaderSubTitle)
{
	m_iOS = 0;
	m_iTotalDownload = 0;
	m_icnWnd = NULL;
	m_iConnectionType = 0;
}


CConnectionWizardDlg::~CConnectionWizardDlg()
{
	if (m_icnWnd)
		VERIFY( DestroyIcon(m_icnWnd) );
}

void CConnectionWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgPageWizard::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROVIDERS, m_provider);
	//DDX_Radio(pDX, IDC_WIZ_XP_RADIO, m_iOS);
	DDX_Radio(pDX, IDC_WIZ_LOWDOWN_RADIO, m_iTotalDownload);

	DDX_Control(pDX, IDC_EDIT_MAXHALFCONN, m_MaxHalfConnEdit);
	DDX_Control(pDX, IDC_EDIT_MAXCONN_PER5, m_MaxNewConnPer5Edit);
	
}


void CConnectionWizardDlg::OnBnClickedKbits()
{
	// TODO: Add your control notification handler code here
	int upload, download;
	TCHAR buffer[510];

	int nID = GetCheckedRadioButton(IDC_KBITS, IDC_KBYTES);
	if (nID == IDC_KBITS)
		return;

	CheckRadioButton(IDC_KBITS, IDC_KBYTES, IDC_KBITS);
	if (GetDlgItem(IDC_WIZ_TRUEDOWNLOAD_BOX)->GetWindowTextLength())
	{ 
		GetDlgItem(IDC_WIZ_TRUEDOWNLOAD_BOX)->GetWindowText(buffer, 20);
		download = _tstoi(buffer);
		download *= 8;
		SetDlgItemInt(IDC_WIZ_TRUEDOWNLOAD_BOX, download, FALSE);
	}

	if (GetDlgItem(IDC_WIZ_TRUEUPLOAD_BOX)->GetWindowTextLength())
	{ 
		GetDlgItem(IDC_WIZ_TRUEUPLOAD_BOX)->GetWindowText(buffer, 20);
		upload = _tstoi(buffer);
		upload *= 8;
		SetDlgItemInt(IDC_WIZ_TRUEUPLOAD_BOX, upload, FALSE);
	}	
}

void CConnectionWizardDlg::OnBnClickedKbytes()
{
	// TODO: Add your control notification handler code here
	int upload, download;
	TCHAR buffer[510];
	
	int nID = GetCheckedRadioButton(IDC_KBITS, IDC_KBYTES);
	if (nID == IDC_KBYTES)
		return;

	CheckRadioButton(IDC_KBITS, IDC_KBYTES, IDC_KBYTES);

	if (GetDlgItem(IDC_WIZ_TRUEDOWNLOAD_BOX)->GetWindowTextLength())
	{ 
		GetDlgItem(IDC_WIZ_TRUEDOWNLOAD_BOX)->GetWindowText(buffer, 20);
		download = _tstoi(buffer);
		download /= 8;
		SetDlgItemInt(IDC_WIZ_TRUEDOWNLOAD_BOX, download, FALSE);
	}
	

	if (GetDlgItem(IDC_WIZ_TRUEUPLOAD_BOX)->GetWindowTextLength())
	{ 
		GetDlgItem(IDC_WIZ_TRUEUPLOAD_BOX)->GetWindowText(buffer, 20);
		upload = _tstoi(buffer);
		upload /= 8;
		SetDlgItemInt(IDC_WIZ_TRUEUPLOAD_BOX, upload, FALSE);
	}	
}

void CConnectionWizardDlg::OnBnClickedApply()
{
	UpdateData();

	if (!ValidateInputs())
		return;


	CString temp;
	GetDlgItem(IDC_EDIT_MAXHALFCONN)->GetWindowText(temp);

	int iHalfOpenConn = (int)_tstoi(temp);
	ChangeHalfOpenConn(iHalfOpenConn);

	temp.Empty();

	GetDlgItem(IDC_EDIT_MAXCONN_PER5)->GetWindowText(temp);

	int iMaxNewConnPer5 = (int)_tstoi(temp);

	if (iMaxNewConnPer5 != (int) thePrefs.GetMaxConperFive())
		thePrefs.SetMaxConsPerFive(iMaxNewConnPer5 ? iMaxNewConnPer5 : DFLT_MAXCONPERFIVE);


	//VC-fengwen on 2008/03/19 <begin> :
	int upload, download;
	CString	str;

	GetDlgItem(IDC_WIZ_TRUEDOWNLOAD_BOX)->GetWindowText(str);
	download = _tstoi(str);

	GetDlgItem(IDC_WIZ_TRUEUPLOAD_BOX)->GetWindowText(str);
	upload = _tstoi(str);

	if (IsDlgButtonChecked(IDC_KBITS))
	{
		upload = upload / 8;
		download = download / 8;
	}

	thePrefs.SetSpeedValues(m_provider.GetSelectionMark(), download, FALSE, 0, upload, FALSE, 0);
	theApp.emuledlg->statisticswnd->SetARange(false, thePrefs.maxGraphUploadRate);
	theApp.emuledlg->statisticswnd->SetARange(true, thePrefs.maxGraphDownloadRate);
	//VC-fengwen on 2008/03/19 <end> :

	//{begin}VC-dgkang 2008??6??30??
	if (upload > 0 && download > 0)
	{
		/*
		if (m_iOS == 1)
			thePrefs.maxconnections = 50;
		else{
			if (upload <= 7)
				thePrefs.maxconnections = 80;
			else if (upload < 12)
				thePrefs.maxconnections = 200;
			else if (upload < 25)
				thePrefs.maxconnections = 400;
			*/
		if (upload < 37)
			thePrefs.maxconnections = 600;
		else
			thePrefs.maxconnections = 800;	
	//{end}VC-dgkang
		if (m_iOS == 1)
			download = download/2;

		if (download <= 7)
		{
			switch (m_iTotalDownload)
			{
				case 0:
					thePrefs.maxsourceperfile = 100;
					break;
				case 1:
					thePrefs.maxsourceperfile = 60;
					break;
				case 2:
					thePrefs.maxsourceperfile = 40;
					break;
			}
		}
		else if (download < 62)
		{
			switch (m_iTotalDownload)
			{
				case 0:
					thePrefs.maxsourceperfile = 300;
					break;
				case 1:
					thePrefs.maxsourceperfile = 200;
					break;
				case 2:
					thePrefs.maxsourceperfile = 100;
					break;
			}
		}
		else if (download < 187)
		{
			switch (m_iTotalDownload)
			{
				case 0:
					thePrefs.maxsourceperfile = 500;
					break;
				case 1:
					thePrefs.maxsourceperfile = 400;
					break;
				case 2:
					thePrefs.maxsourceperfile = 350;
					break;
			}
		}
		else if (download <= 312)
		{
			switch (m_iTotalDownload)
			{
				case 0:
					thePrefs.maxsourceperfile = 800;
					break;
				case 1:
					thePrefs.maxsourceperfile = 600;
					break;
				case 2:
					thePrefs.maxsourceperfile = 400;
					break;
			}
		}
		else
		{
			switch (m_iTotalDownload)
			{
				case 0:
					thePrefs.maxsourceperfile = 1000;
					break;
				case 1:
					thePrefs.maxsourceperfile = 750;
					break;
				case 2:
					thePrefs.maxsourceperfile = 500;
					break;
			}
		}
	}

	//Chocobo Start
	//????????????????added by Chocobo on 2006.08.02
	//??????????
	if (m_provider.GetSelectionMark() == 6 ){// Added by VeryCD -- GGSoSo
		thePrefs.maxupload = UNLIMITED;
		thePrefs.maxdownload = UNLIMITED;
	}
	//Chocobo End

	RecommendHalpOpenChanging();

	theApp.emuledlg->preferenceswnd->m_wndConnection.LoadSettings();
//	CDlgPageWizard::OnOK();
}

void CConnectionWizardDlg::OnBnClickedCancel()
{
	CDlgPageWizard::OnCancel();
}

void CConnectionWizardDlg::OnBnClickedWizRadioOsNtxp()
{
	m_iOS = 0;
}

void CConnectionWizardDlg::OnBnClickedWizRadioUs98me()
{
	m_iOS = 1;
}

void CConnectionWizardDlg::OnBnClickedWizLowdownloadRadio()
{
	m_iTotalDownload = 0;
}

void CConnectionWizardDlg::OnBnClickedWizMediumdownloadRadio()
{
	m_iTotalDownload = 1;
}

void CConnectionWizardDlg::OnBnClickedWizHighdownloadRadio()
{
	m_iTotalDownload = 2;
}

void CConnectionWizardDlg::OnBnClickedWizResetButton()
{
	SetDlgItemInt(IDC_WIZ_TRUEDOWNLOAD_BOX, 0, FALSE);
	SetDlgItemInt(IDC_WIZ_TRUEUPLOAD_BOX, 0, FALSE);
}

BOOL CConnectionWizardDlg::OnInitDialog()
{
	CDlgPageWizard::OnInitDialog();
	InitWindowStyles(this);

	m_MaxHalfConnEdit.SetMaxWholeDigits(3);
	m_MaxNewConnPer5Edit.SetMaxWholeDigits(3);
	m_MaxHalfConnEdit.SetRange(RANGE_MAXHALFOPEN_MIN, RANGE_MAXHALFOPEN_MAX);
	m_MaxNewConnPer5Edit.SetRange(RANGE_MAXCONN_PER5_MIN, RANGE_MAXCONN_PER5_MAX);

	CString strBuffer;
	strBuffer.Format(_T("%d"), thePrefs.GetMaxHalfConnections());
	GetDlgItem(IDC_EDIT_MAXHALFCONN)->SetWindowText(strBuffer);

	strBuffer.Empty();

	strBuffer.Format(_T("%d"), thePrefs.GetMaxConperFive());
	GetDlgItem(IDC_EDIT_MAXCONN_PER5)->SetWindowText(strBuffer);

	m_ttc.Create(this);
	m_ttc.SetDelayTime(TTDT_AUTOPOP, INFINITE);
	m_ttc.SetDelayTime(TTDT_INITIAL, 100);
	m_ttc.AddTool(GetDlgItem(IDC_STATIC_MAXHALFCONN), GetResString(IDS_TIP_HALFCONN));
	m_ttc.AddTool(GetDlgItem(IDC_EDIT_MAXHALFCONN), GetResString(IDS_TIP_HALFCONN));
	m_ttc.AddTool(GetDlgItem(IDC_STATIC_MAXCONN_PER5), GetResString(IDS_TIP_MAXCONN_PER5));
	m_ttc.AddTool(GetDlgItem(IDC_EDIT_MAXCONN_PER5), GetResString(IDS_TIP_MAXCONN_PER5));

	if (!CanChangeHalfOpenConn())
		GetDlgItem(IDC_EDIT_MAXHALFCONN)->EnableWindow(FALSE);


	m_iConnectionType = thePrefs.GetConnectionType();

	if(m_iConnectionType > 6)
	{
		m_iConnectionType = 0;
	}

	SetIcon(m_icnWnd = theApp.LoadIcon(_T("Wizard")), FALSE);

	if (::DetectWinVersion()==_WINVER_95_ || ::DetectWinVersion()==_WINVER_98_ || ::DetectWinVersion()==_WINVER_ME_){
	//	CheckRadioButton(IDC_WIZ_XP_RADIO, IDC_WIZ_ME_RADIO, IDC_WIZ_ME_RADIO);
		m_iOS = 1;
	}
	else{
	//	CheckRadioButton(IDC_WIZ_XP_RADIO, IDC_WIZ_ME_RADIO, IDC_WIZ_XP_RADIO);
		m_iOS = 0;
	}
	CheckRadioButton(IDC_WIZ_LOWDOWN_RADIO, IDC_WIZ_HIGHDOWN_RADIO, IDC_WIZ_LOWDOWN_RADIO);
	CheckRadioButton(IDC_KBITS, IDC_KBYTES, IDC_KBYTES);

	SetDlgItemInt(IDC_WIZ_TRUEDOWNLOAD_BOX, 0, FALSE);
	SetDlgItemInt(IDC_WIZ_TRUEUPLOAD_BOX, 0, FALSE);

	m_provider.InsertColumn(0, GetResString(IDS_PW_CONNECTION), LVCFMT_LEFT, 100);
	m_provider.InsertColumn(1, GetResString(IDS_WIZ_DOWN), LVCFMT_LEFT, 85);
	m_provider.InsertColumn(2, GetResString(IDS_WIZ_UP), LVCFMT_LEFT, 85);
	m_provider.SetExtendedStyle(m_provider.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_provider.InsertItem(0, GetResString(IDS_UNKNOWN));m_provider.SetItemText(0,1,_T(""));m_provider.SetItemText(0,2,_T(""));
	m_provider.InsertItem(1, GetResString(IDS_WIZARD_CUSTOM));m_provider.SetItemText(1,1,GetResString(IDS_WIZARD_ENTERBELOW));m_provider.SetItemText(1,2,GetResString(IDS_WIZARD_ENTERBELOW));
	//Chocobo Start
	//????????????????added by Chocobo on 2006.08.02
	//????????????????
	m_provider.InsertItem(2,_T("ADSL(512K)"));m_provider.SetItemText(2,1,_T("512"));m_provider.SetItemText(2,2,_T("512"));
 	m_provider.InsertItem(3,_T("ADSL(1MB)"));m_provider.SetItemText(3,1,_T("1024"));m_provider.SetItemText(3,2,_T("512"));
 	m_provider.InsertItem(4,_T("ADSL(2MB)"));m_provider.SetItemText(4,1,_T("2048"));m_provider.SetItemText(4,2,_T("512"));
 	m_provider.InsertItem(5,_T("ADSL(8MB)"));m_provider.SetItemText(5,1,_T("8196"));m_provider.SetItemText(5,2,_T("512"));
 	m_provider.InsertItem(6,_T("Lan"));m_provider.SetItemText(6,1,_T(""));m_provider.SetItemText(6,2,_T(""));
	/*	m_provider.InsertItem(2,_T("56-k Modem"));m_provider.SetItemText(2,1,_T("56"));m_provider.SetItemText(2,2,_T("56"));
	m_provider.InsertItem(3,_T("ISDN"));m_provider.SetItemText(3,1,_T("64"));m_provider.SetItemText(3,2,_T("64"));
	m_provider.InsertItem(4,_T("ISDN 2x"));m_provider.SetItemText(4,1,_T("128"));m_provider.SetItemText(4,2,_T("128"));

	m_provider.InsertItem(5,_T("T DSL 1000 (T,Arcor,Freenet,1&1)"));m_provider.SetItemText(5,1,_T("1024"));m_provider.SetItemText(5,2,_T("128"));
	m_provider.InsertItem(6,_T("T DSL 1500 (T)"));m_provider.SetItemText(6,1,_T("1536"));m_provider.SetItemText(6,2,_T("192"));
	m_provider.InsertItem(7,_T("T DSL 2000 (T,Arcor,Freenet,Tiscali,Alice)"));m_provider.SetItemText(7,1,_T("2048"));m_provider.SetItemText(7,2,_T("192"));
	m_provider.InsertItem(8,_T("Versatel DSL 2000"));m_provider.SetItemText(8,1,_T("2048"));m_provider.SetItemText(8,2,_T("384"));

	m_provider.InsertItem(9,_T("T-DSL 3000 (T,Arcor)"));m_provider.SetItemText(9,1,_T("3072"));m_provider.SetItemText(9,2,_T("384"));
	m_provider.InsertItem(10,_T("T DSL 6000 (T,Arcor)"));m_provider.SetItemText(10,1,_T("6016"));m_provider.SetItemText(10,2,_T("576"));
	m_provider.InsertItem(11,_T("  DSL 6000 (Tiscali,Freenet,1&1)"));m_provider.SetItemText(11,1,_T("6016"));m_provider.SetItemText(11,2,_T("572"));
	m_provider.InsertItem(12,_T("  DSL 6000 (Lycos,Alice)"));m_provider.SetItemText(12,1,_T("6016"));m_provider.SetItemText(12,2,_T("512"));
	m_provider.InsertItem(13,_T("Versatel DSL 6000"));m_provider.SetItemText(13,1,_T("6144"));m_provider.SetItemText(13,2,_T("512"));

	m_provider.InsertItem(14,_T("Cable"));m_provider.SetItemText(14,1,_T("187"));m_provider.SetItemText(16,2,_T("32"));
	m_provider.InsertItem(15,_T("Cable"));m_provider.SetItemText(15,1,_T("187"));m_provider.SetItemText(17,2,_T("64"));
	m_provider.InsertItem(16,_T("T1"));m_provider.SetItemText(16,1,_T("1500"));m_provider.SetItemText(18,2,_T("1500"));
	m_provider.InsertItem(17,_T("T3+"));m_provider.SetItemText(17,1,_T("44 Mbps"));m_provider.SetItemText(19,2,_T("44 Mbps"));
	*/
	//Chocobo End
	m_provider.SetSelectionMark(m_iConnectionType);
	m_provider.SetItemState(m_iConnectionType, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	SetCustomItemsActivation();

	NMHDR* pNMHDR = new NMHDR;

	pNMHDR->code = NM_CLICK;
	pNMHDR->hwndFrom = GetDlgItem(IDC_PROVIDERS)->GetSafeHwnd();
	pNMHDR->idFrom = IDC_PROVIDERS;

	SendMessage(WM_NOTIFY, IDC_PROVIDERS, (LPARAM)(LPNMHDR)pNMHDR);

	Localize();

	return TRUE;
}

void CConnectionWizardDlg::OnNMClickProviders(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	SetCustomItemsActivation();


	UINT up, down;
	//MODIFIED by VC-fengwen on 2008/03/19 <begin> :
	//switch (m_provider.GetSelectionMark())
	//{
	//	case  0: down=   0;up=   0; m_iConnectionType = 0;break;
	//	case  1: down= ((thePrefs.maxGraphDownloadRate * 1024) + 500) / 1000 * 8; up= ((thePrefs.GetMaxGraphUploadRate(true) * 1024) + 500) / 1000 * 8;  m_iConnectionType = 1; break;
	//	//Chocobo Start
	//	//????????????????added by Chocobo on 2006.08.02
	//	//????????
	//	case  2: down=   512;up=   512; m_iConnectionType = 2; break;
	//	case  3: down=   1024;up=  512;  m_iConnectionType = 3; break;
	//	case  4: down=   2048;up=  512;  m_iConnectionType = 4; break;
	//	case  5: down=   8196;up=  512;  m_iConnectionType = 5; break;
	//	case  6: down=   2048;up=  2048;  m_iConnectionType = 6;break;
	//	/*case  2: down=   56;	up=   33; break;
	//	case  3: down=   64;	up=   64; break;
	//	case  4: down=  128;	up=  128; break;
	//	case  5: down= 1024;	up=  128; break;
	//	case  6: down= 1536;	up=  192; break;
	//	case  7: down= 2048;	up=  192; break;
	//	case  8: down= 2048;	up=  384; break;
	//	case  9: down= 3072;	up=  384; break;
	//	case 10: down= 6016;	up=  576; break;
	//	case 11: down= 6016;	up=  572; break;
	//	case 12: down= 6016;	up=  512; break;
	//	case 13: down= 6144;	up=  512; break;
	//	case 14: down=  187;	up=   32; break;
	//	case 15: down=  187;	up=   64; break;
	//	case 16: down= 1500;	up= 1500; break;
	//	case 17: down=44000;	up=44000; break;*/
	//	//Chocobo End
	//	default: return;
	//}
	BOOL bDownLimit, bUpLimit;
	UINT uDownLimit, uUpLimit;
	thePrefs.GetSpeedValues(m_provider.GetSelectionMark(), down, bDownLimit, uDownLimit, up, bUpLimit, uUpLimit);
	//MODIFIED by VC-fengwen on 2008/03/19 <begin> :


	SetDlgItemInt(IDC_WIZ_TRUEDOWNLOAD_BOX, down, FALSE);
	SetDlgItemInt(IDC_WIZ_TRUEUPLOAD_BOX, up, FALSE);
	CheckRadioButton(IDC_KBITS, IDC_KBYTES, IDC_KBYTES);

	thePrefs.SetConnectionType(m_iConnectionType);

	*pResult = 0;
}

void CConnectionWizardDlg::Localize()
{
	SetWindowText(GetResString(IDS_WIZARD));
	//GetDlgItem(IDC_WIZ_OS_FRAME)->SetWindowText(GetResString(IDS_WIZ_OS_FRAME));
	GetDlgItem(IDC_WIZ_CONCURENTDOWN_FRAME)->SetWindowText(GetResString(IDS_CONCURDWL));
	GetDlgItem(IDC_WIZ_HOTBUTTON_FRAME)->SetWindowText(GetResString(IDS_WIZ_CTFRAME));
	GetDlgItem(IDC_WIZ_TRUEUPLOAD_TEXT)->SetWindowText(GetResString(IDS_WIZ_TRUEUPLOAD_TEXT));
	GetDlgItem(IDC_WIZ_TRUEDOWNLOAD_TEXT)->SetWindowText(GetResString(IDS_WIZ_TRUEDOWNLOAD_TEXT));
	GetDlgItem(IDC_KBITS)->SetWindowText(GetResString(IDS_KBITSSEC));
	GetDlgItem(IDC_KBYTES)->SetWindowText(GetResString(IDS_KBYTESSEC));
	GetDlgItem(IDC_WIZ_APPLY_BUTTON)->SetWindowText(GetResString(IDS_PW_APPLY));
	GetDlgItem(IDC_WIZ_CANCEL_BUTTON)->SetWindowText(GetResString(IDS_CANCEL));

	GetDlgItem(IDC_FRM_CONN_PARAM)->SetWindowText(GetResString(IDS_WIZ_CONN_PARAM));
	GetDlgItem(IDC_STATIC_MAXHALFCONN)->SetWindowText(GetResString(IDS_MAXHALFOPENCONS));
	GetDlgItem(IDC_STATIC_MAXCONN_PER5)->SetWindowText(GetResString(IDS_MAXCON5SECLABEL));

	m_ttc.UpdateTipText(GetResString(IDS_MAXHALFOPENCONS) + _T(":\n\n") + GetResString(IDS_TIP_HALFCONN), GetDlgItem(IDC_STATIC_MAXHALFCONN));
	m_ttc.UpdateTipText(GetResString(IDS_MAXHALFOPENCONS) + _T(":\n\n") + GetResString(IDS_TIP_HALFCONN), GetDlgItem(IDC_EDIT_MAXHALFCONN));
	m_ttc.UpdateTipText(GetResString(IDS_MAXCON5SECLABEL) + _T(":\n\n") + GetResString(IDS_TIP_MAXCONN_PER5), GetDlgItem(IDC_STATIC_MAXCONN_PER5));
	m_ttc.UpdateTipText(GetResString(IDS_MAXCON5SECLABEL) + _T(":\n\n") + GetResString(IDS_TIP_MAXCONN_PER5), GetDlgItem(IDC_EDIT_MAXCONN_PER5));
}


void CConnectionWizardDlg::SetCustomItemsActivation()
{
	BOOL bActive = m_provider.GetSelectionMark() == 1;
	GetDlgItem(IDC_WIZ_TRUEUPLOAD_BOX)->EnableWindow(bActive);
	GetDlgItem(IDC_WIZ_TRUEDOWNLOAD_BOX)->EnableWindow(bActive);
	GetDlgItem(IDC_KBITS)->EnableWindow(bActive);
	GetDlgItem(IDC_KBYTES)->EnableWindow(bActive);
}

LRESULT CConnectionWizardDlg::OnWizardNext()
{
	if (!ValidateInputs())
		return -1;

	OnBnClickedApply();
	return 0;
}

BOOL CConnectionWizardDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ??????????????????/??????????
	m_ttc.RelayEvent(pMsg);

	return CDlgPageWizard::PreTranslateMessage(pMsg);
}

void CConnectionWizardDlg::RecommendHalpOpenChanging()
{
	if (CanChangeHalfOpenConn()
		&& thePrefs.GetMaxHalfConnections() < 256)
	{
		int iRet = MessageBox(GetResString(IDS_HALFOPENCONN_SUGGEST), GetResString(IDS_CAPTION), MB_YESNO);
		if (IDYES == iRet)
		{
			ChangeHalfOpenConn(256);

			CString strBuffer;
			strBuffer.Format(_T("%d"), thePrefs.GetMaxHalfConnections());
			GetDlgItem(IDC_EDIT_MAXHALFCONN)->SetWindowText(strBuffer);
		}
	}
}
BOOL CConnectionWizardDlg::ValidateInputs()
{
	return (m_MaxHalfConnEdit.Validate()
			&& m_MaxNewConnPer5Edit.Validate());
}