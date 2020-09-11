// DriveView.cpp : implementation of the CDriveView class
//

#include "stdafx.h"
#include "Explorer.h"

#include "ExplorerDoc.h"
#include "DriveView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDriveView

IMPLEMENT_DYNCREATE(CDriveView, CTreeView)

BEGIN_MESSAGE_MAP(CDriveView, CTreeView)
	//{{AFX_MSG_MAP(CDriveView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDriveView construction/destruction

CDriveView::CDriveView()
{
	// TODO: add construction code here

}

CDriveView::~CDriveView()
{
}

BOOL CDriveView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

int CDriveView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// �̹��� ����Ʈ ����
	m_imgDrives.Create(IDB_DRIVES, 16, 1, RGB(0, 128, 128));
	// �̹��� ����Ʈ�� Ʈ�� ��Ʈ�� ����
	GetTreeCtrl().SetImageList(&m_imgDrives, TVSIL_NORMAL);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDriveView drawing

void CDriveView::OnDraw(CDC* pDC)
{
	CExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


void CDriveView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	InitDriveView();
}

/////////////////////////////////////////////////////////////////////////////
// CDriveView diagnostics

#ifdef _DEBUG
void CDriveView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CDriveView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CExplorerDoc* CDriveView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExplorerDoc)));
	return (CExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDriveView message handlers

// �ý��ۿ� ��ġ�� ��ũ ����̺� ���� ���
int CDriveView::InitDriveView()
{
	int nPos = 0;
	int nDrives = 0;
	CString strDrive = "?:\\";

	// ���� �ý��ۿ� ��ġ�Ǿ� �ִ� ��ũ ����̺� ���� ���
	DWORD dwDriveList = ::GetLogicalDrives();

	while(dwDriveList)
	{
		if(dwDriveList & 1)
		{
			// "C:\\"�� ���� ����̺긦 ǥ���ϴ� ���ڿ��� ����
			strDrive.SetAt(0, 'A'+nPos);
			// ����̺� ���ڿ��� Ʈ�� ��Ʈ�ѿ� ���� �߰�
			if(AddDriveNode(strDrive))
				nDrives++;
		}
		// ��� 1��Ʈ �̵�
		dwDriveList >>= 1;
		nPos++;
	}
	return nDrives;
}


// ����̺� ���ڿ��� Ʈ�� ��Ʈ���� ���� �߰�
BOOL CDriveView::AddDriveNode(CString &strDrive)
{
	CString str;
	HTREEITEM hItem;
	static BOOL bFirst = TRUE;

	// ��ũ ����̺��� Ÿ���� ����
	UINT nType = ::GetDriveType((LPCTSTR)strDrive);

	switch(nType)
	{
		// �÷��� ��ũ ����̺��� ���
		case DRIVE_REMOVABLE:
			hItem = GetTreeCtrl().InsertItem(strDrive, ID_FLOPPY, ID_FLOPPY);
			GetTreeCtrl().InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
			break;
		// �ϵ� ��ũ ����̺��� ���
		case DRIVE_FIXED:
			hItem = GetTreeCtrl().InsertItem(strDrive, ID_HARDDISK, ID_HARDDISK);
			SetButtonState(hItem, strDrive);
			if(bFirst)
			{
				GetTreeCtrl().SelectItem(hItem);
				GetTreeCtrl().Expand(hItem, TVE_EXPAND);
				bFirst = FALSE;
			}
			break;
		// ��Ʈ��ũ ����̺��� ���
		case DRIVE_REMOTE:
			hItem = GetTreeCtrl().InsertItem(strDrive, ID_NETDRIVE, ID_NETDRIVE);
			SetButtonState(hItem, strDrive);
			break;
		// CD �� ����̺��� ���
		case DRIVE_CDROM:
			hItem = GetTreeCtrl().InsertItem(strDrive, ID_CDROM, ID_CDROM);
			GetTreeCtrl().InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
			break;
		// RAM ����̺��� ���
		case DRIVE_RAMDISK:
			hItem = GetTreeCtrl().InsertItem(strDrive, ID_HARDDISK, ID_HARDDISK);
			SetButtonState(hItem, strDrive);
			break;
		default:
			return FALSE;
	}
	return TRUE;
}


// ���� ���丮 ���� ���� Ȯ��
BOOL CDriveView::SetButtonState(HTREEITEM hItem, CString strPath)
{
	if(strPath.Right(1) != "\\") strPath += "\\";
	strPath += "*.*";
	
	CString strDirName;
	CFileFind filefind;
	BOOL bContinue;
	if(!(bContinue = filefind.FindFile(strPath)))
		return FALSE;

	// ������ ���丮�� ������ ���ʷ� �о��
	while(bContinue)
	{
		bContinue = filefind.FindNextFile();
		// ���丮�̸�...
		if(filefind.IsDirectory())
		{
			strDirName = filefind.GetFileName();
			if((strDirName != ".") && (strDirName != ".."))
			{
				// "."�� ".."�� �ƴ� ���丮�̸� ���̳�带 �ϳ� �߰��ϰ� ���� ����
				GetTreeCtrl().InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
				break;
			}
		}
	}
	filefind.Close();
	return TRUE;
}

// ����� Ȯ��, ��Ҹ� �˸��� Ʈ�� ��Ʈ���� ��� �޽��� ó��
void CDriveView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// Ȯ��ǰų� ��ҵ� ��带 ����
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	// Ȯ��ǰų� ��ҵ� ��尡 ��Ÿ���� ���丮���� ����
	CString str = GetPathFromNode(hItem);

	*pResult = FALSE;

	// ��尡 Ȯ��Ǵ� ���
	if(pNMTreeView->action == TVE_EXPAND)
	{
		// �ռ� �߰��ߴ� ���� ��� ����
		DeleteFirstChild(hItem);
		// ��¥ ���丮 ������ �о� ǥ��
		if(AddDir(hItem, str) == 0)
			*pResult = TRUE;
	}
	else	// ��尡 ��ҵǴ� ���
	{
		// ��� ���� ��带 ����
		DeleteAllChildren(hItem);
		// ���ڰ� ����� ��ư ǥ�ø� ���� ���� ��� �߰�
		if(GetTreeCtrl().GetParentItem(hItem) == NULL)
			GetTreeCtrl().InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
		else	// ���� ���丮 ���� ���� Ȯ��
			SetButtonState(hItem, str);
	}
}

//�־��� ��尡 ��Ÿ���� ���丮�� ���
CString CDriveView::GetPathFromNode(HTREEITEM hItem)
{
	CString strResult = GetTreeCtrl().GetItemText(hItem);

	HTREEITEM hParent;
	// ��Ʈ ��带 ���� ������ ����...
	while((hParent = GetTreeCtrl().GetParentItem(hItem)) != NULL)
	{
		CString str = GetTreeCtrl().GetItemText(hParent);
		if(str.Right(1) != "\\")
			str += "\\";
		strResult = str + strResult;
		hItem = hParent;
	}
	return strResult;
}

// �ϳ��� �ڽ� ��� �����ϱ�
void CDriveView::DeleteFirstChild(HTREEITEM hParent)
{
	HTREEITEM hItem;
	if((hItem = GetTreeCtrl().GetChildItem(hParent)) != NULL)
		GetTreeCtrl().DeleteItem(hItem);
}

// ��� �ڽ� ��� �����ϱ�
void CDriveView::DeleteAllChildren(HTREEITEM hParent)
{
	HTREEITEM hItem;
	if((hItem = GetTreeCtrl().GetChildItem(hParent)) == NULL)
		return;

	do
	{
		HTREEITEM hNextItem = GetTreeCtrl().GetNextSiblingItem(hItem);
		GetTreeCtrl().DeleteItem(hItem);
		hItem = hNextItem;
	} while (hItem != NULL);
}


int CDriveView::AddDir(HTREEITEM hItem, CString &strPath)
{
	CString strFindPath;
	HTREEITEM hNewItem;
	int nCount = 0;

	// ������ ���丮 �ڿ� "\\*.*"�� ������
	// ��: strPath = "C:\\Temp"�̸�, strFindPath = "C:\\Temp\\*.*"�� ��
	strFindPath = strPath;
	if(strFindPath.Right(1) != "\\") strFindPath += "\\";
	strFindPath += "*.*";

	CString strFileName, strNewPath; 
	CFileFind filefind;

	// Ž���� ���丮 ����
	BOOL bContinue;
	if(!(bContinue = filefind.FindFile(strFindPath)))
	{
		if(GetTreeCtrl().GetParentItem(hItem) == NULL)
			GetTreeCtrl().InsertItem("", ID_CLOSEDFOLDER, ID_CLOSEDFOLDER, hItem);
		return 0;
	}

	// ���丮 Ž��
	while(bContinue)
	{
		bContinue = filefind.FindNextFile();
		// ���丮�� ��졦
		if(filefind.IsDirectory()) 
		{
			strFileName = filefind.GetFileName();
			if(strFileName != "." && strFileName != "..")
			{
				// ��� �߰�
				hNewItem = GetTreeCtrl().InsertItem((LPCTSTR)strFileName, ID_CLOSEDFOLDER, ID_OPENFOLDER, hItem);
				
				strNewPath = strPath;
				if(strNewPath.Right(1) != "\\")	strNewPath += "\\";
				
				// �߰��� ��尡 ���� ���丮�� �������� Ȯ��
				strNewPath += strFileName;
				SetButtonState(hNewItem, strNewPath);
				nCount++;
			}
		}
	}

	filefind.Close();
	return nCount;
}


// ���õ� ���丮 ���� ���θ� ����Ʈ�信 �˸���
void CDriveView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString strPath = GetPathFromNode(pNMTreeView->itemNew.hItem);
	GetDocument()->UpdateAllViews(this, (LPARAM)(LPCTSTR)strPath);
}


