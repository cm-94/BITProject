// FileView.cpp : implementation of the CFileView class
//

#include "stdafx.h"
#include "Explorer.h"

#include "ExplorerDoc.h"
#include "FileView.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

IMPLEMENT_DYNCREATE(CFileView, CListView)

BEGIN_MESSAGE_MAP(CFileView, CListView)
	//{{AFX_MSG_MAP(CFileView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EDIT_CUT, &CFileView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CFileView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CFileView::OnEditPaste)
	// View Click Event
	//	ON_NOTIFY_REFLECT(NM_CLICK, &CFileView::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CFileView::OnNMDblclk)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileView construction/destruction

CFileView::CFileView()
{
	// TODO: add construction code here
	sortOps = 1;
}

CFileView::~CFileView()
{
}

// ����Ʈ���� ��Ÿ�� ����
BOOL CFileView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	cs.style |= LVS_SHAREIMAGELISTS;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFileView drawing

void CFileView::OnDraw(CDC* pDC)
{
	CExplorerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
	refCtrl.InsertItem(0, "Item!");

	// TODO: add draw code for native data here
	sortOps = 1;
}

/////////////////////////////////////////////////////////////////////////////
// CFileView diagnostics

#ifdef _DEBUG
void CFileView::AssertValid() const
{
	CListView::AssertValid();
}

void CFileView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CExplorerDoc* CFileView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CExplorerDoc)));
	return (CExplorerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileView message handlers
void CFileView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

// ����Ʈ�� �ʱ�ȭ
int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �ü���� �����ϴ� �̹��� ����Ʈ ���
	GetSystemImageList();
	// ����Ʈ ��Ʈ�ѿ� �÷� �߰�
	InsertColumn();
	return 0;
}

HGLOBAL WINAPI CopyHandle(HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	DWORD dwLen = ::GlobalSize((HGLOBAL)h);
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

	if (hCopy != NULL)
	{
		void* lpCopy = ::GlobalLock((HGLOBAL)hCopy);
		void* lp = ::GlobalLock((HGLOBAL)h);
		memcpy(lpCopy, lp, dwLen);
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}

// �ü���� �����ϴ� �̹��� ����Ʈ ���
void CFileView::GetSystemImageList()
{
	HIMAGELIST	hSystemImageList;
	SHFILEINFO	info;

	// �ü���� �����ϴ� ���� ������ �̹��� ����Ʈ�� ����
	hSystemImageList = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T("C:\\"),
		0, &info, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	m_imgSmallList.Attach(hSystemImageList);
	m_imgSmallList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	// �ü���� �����ϴ� ���� ������ �̹��� ����Ʈ�� ����
	hSystemImageList = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T("C:\\"),
		0, &info, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_ICON);
	m_imgLargeList.Attach(hSystemImageList);
	m_imgLargeList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	// ����Ʈ ��Ʈ�Ѱ� �̹��� ����Ʈ ����
	GetListCtrl().SetImageList(&m_imgLargeList, LVSIL_NORMAL);
	GetListCtrl().SetImageList(&m_imgSmallList, LVSIL_SMALL);
}


void CFileView::OnDestroy()
{
	CListView::OnDestroy();
	FreeItemMemory();

	// �ü���� �����ϴ� �̹��� ����Ʈ�� ���� ����
	m_imgSmallList.Detach();
	m_imgLargeList.Detach();
}

// ����Ʈ ��Ʈ�ѿ� �÷� �߰�
void CFileView::InsertColumn()
{
	GetListCtrl().InsertColumn(0, "���ϸ�", LVCFMT_LEFT, 200);
	GetListCtrl().InsertColumn(1, "ũ��", LVCFMT_RIGHT, 100);
	GetListCtrl().InsertColumn(2, "�ٲ� ��¥", LVCFMT_CENTER, 200);
}

// ����Ʈ���� �ʱ�ȭ�� ǥ��
void CFileView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	SetListData("C:\\");
}

// Ʈ���信�� ���õ� ���丮�� �ٲ� ������ ����Ʈ�� ����
void CFileView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint)
	{
		// ������ ǥ�õǰ� �ִ� �����͸� ���� �޸� ����
		FreeItemMemory();
		// ������ ǥ�õǰ� �ִ� ���� ����
		GetListCtrl().DeleteAllItems();
		// ������ ���丮�� ���� ���丮�� ������ ǥ��
		SetListData((LPCTSTR)lHint);
	}
	else
		CListView::OnUpdate(pSender, lHint, pHint);
}

// ������ ���丮�� ���� ���丮�� ������ ǥ��
int CFileView::SetListData(LPCTSTR pszPath)
{
	// ������ ���丮 �ڿ� "\\*.*"�� ������
	// ��: pszPath = "C:\\Temp"�̸�, strPath = "C:\\Temp\\*.*"�� ��
	CString strPath = pszPath;
	if (strPath.Right(1) != "\\") strPath += "\\";
	strPath += "*.*";

	// ������ ���丮�� ������ ���ʷ� �о��
	CFileFind filefind;
	BOOL bContinue;
	if (!(bContinue = filefind.FindFile(strPath)))
		return -1;
	int nCount = 0;

	CString strFilename;
	// ���丮 Ž��
	while (bContinue)
	{
		bContinue = filefind.FindNextFile();
		strFilename = filefind.GetFileName();
		if (strFilename != "." && strFilename != "..")
			AddItem(nCount++, &filefind);  // ����Ʈ ��Ʈ�ѿ� �߰�
	}

	filefind.Close();
	return nCount;
}

void CFileView::SwitchSortOps(){
	if (sortOps>0) sortOps = 0;
	else sortOps = 1;
}

BOOL CFileView::AddItem(int nIndex, CFileFind *pFileFind)
{
	// �����͸� ������ ����ü�� �޸� ���� �Ҵ�
	FILEITEM *pItem;
	try
	{
		pItem = new FILEITEM;
	}
	catch (CMemoryException *e)
	{
		e->Delete();
		return FALSE;
	}
	// ������ ����
	pItem->strFileName = pFileFind->GetFileName();
	pItem->nFileSize = pFileFind->GetLength();
	pItem->strFilePath = pFileFind->GetFilePath();
	pFileFind->GetLastWriteTime(pItem->timeLastWriteTime);
	// ����Ʈ ��Ʈ�ѿ� �׸��� �߰��ϱ� ���� LV_ITEM ����ü ����
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.iItem = nIndex;
	lvi.iSubItem = 0;
	lvi.iImage = GetIconIndex(pFileFind->GetFilePath());
	lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.lParam = (LPARAM)pItem;

	// ����Ʈ ��Ʈ�ѿ� ������ ����
	if (GetListCtrl().InsertItem(&lvi) == -1)
		return FALSE;

	return TRUE;
}



// �����͸� �����ϴµ� �Ҵ��� �޸𸮸� ����
void CFileView::FreeItemMemory()
{
	for (int i = 0; i<GetListCtrl().GetItemCount(); i++)
		delete (FILEITEM *)GetListCtrl().GetItemData(i);
}

// �̹��� ����Ʈ�� �ε��� ����
int CFileView::GetIconIndex(CString FileName)
{
	SHFILEINFO    sfi;
	SHGetFileInfo((LPCTSTR)FileName, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	return sfi.iIcon;
}


// ����Ʈ ��Ʈ�ѿ� ���� ���� ǥ��, LVN_GETDISPINFO ���� �޽����� ���� �ڵ鷯 �Լ�
void CFileView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	CString str;
	if (pDispInfo->item.mask & LVIF_TEXT)
	{
		FILEITEM *pItem = (FILEITEM *)pDispInfo->item.lParam;

		switch (pDispInfo->item.iSubItem)
		{
		case 0:	// ���ϸ�
			::lstrcpy(pDispInfo->item.pszText, (LPCTSTR)pItem->strFileName);
			break;
		case 1: // ũ��
			str.Format("%u", pItem->nFileSize);
			::lstrcpy(pDispInfo->item.pszText, (LPCTSTR)str);
			break;
		case 2: // �ٲ� ��¥
			str = pItem->timeLastWriteTime.Format("%Y-%m-%d %H:%M");
			::lstrcpy(pDispInfo->item.pszText, (LPCTSTR)str);
			break;
		case 3: // ���� ���
			str = pItem->strFileName;
			::lstrcpy(pDispInfo->item.pszText, (LPCTSTR)str);
		}
	}
}

// ����Ʈ ��Ʈ���� ���� ����, LVN_COLUMNCLICK ���� �޽����� �ڵ鷯 �Լ�
void CFileView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//GetListCtrl().SortItems(CompareFunc, pNMListView->iSubItem);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	GetListCtrl().SortItems(CompareFunc, MAKEWPARAM(pNMListView->iSubItem, sortOps));
	SwitchSortOps();
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int nSubItem = LOWORD(lParamSort);
	int nType = HIWORD(lParamSort);

	int nResult;
	FILEITEM *pItem1 = (FILEITEM *)lParam1;
	FILEITEM *pItem2 = (FILEITEM *)lParam2;
	CString s = pItem1->strFilePath;
	OutputDebugString("ss:" + s + "\n");
	switch (nSubItem)
	{
	case 0:		// ���ϸ����� ����
		nResult = pItem1->strFileName.CompareNoCase(pItem2->strFileName);
		break;
	case 1: 	// ���� ũ��� ����
		nResult = pItem1->nFileSize - pItem2->nFileSize;
		break;
	case 2: 	// ������ ������ ���� ���ڷ� ����
		if (pItem1->timeLastWriteTime > pItem2->timeLastWriteTime)
			nResult = 1;
		else if (pItem1->timeLastWriteTime < pItem2->timeLastWriteTime)
			nResult = -1;
		else
			nResult = 0;
		if (nType == -1) nResult *= -1;
		break;
	}
	if (nType == 1) return nResult;
	else if (nType == 0) return nResult *= -1;
}


void CFileView::OnEditCut()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CString strPath = "C:\\";
	if (strPath.Right(1) != "\\") strPath += "\\";
	strPath += "*.*";
	// ������ ���丮�� ������ ���ʷ� �о��
	CFileFind filefind;
	BOOL bContinue;
	bContinue = filefind.FindFile(strPath);
	if (!bContinue) return;
	// ���丮 Ž��
	int delCount = 0;

	for (int nItem = 0; nItem < GetListCtrl().GetItemCount(); nItem++){
		if (filefind.FindNextFile()){
			if (GetListCtrl().GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED){
				FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(nItem);

				// ���� ���� ��Ȯ��
				CString fileName = item->strFileName;
				fileName.Format("%s�� �����Ͻðڽ��ϱ�?", fileName);
				int temp = AfxMessageBox(fileName, MB_OKCANCEL);
				if (temp != 1) continue;

				// ���� ����(����Ʈ & File)
				GetListCtrl().DeleteItem(nItem);
				CString path = item->strFilePath;
				CFile::Remove(path);
				nItem--;
				delCount++;
			}
			else continue;
		}
	}
	filefind.Close();
	if (delCount == 0) MessageBox("������ ������ �������ּ���.", "����", MB_ICONSTOP);
	else{
		CString countStr; countStr.Format("���� %d�� ���� �Ϸ�",delCount);
		MessageBox(countStr, "����", MB_ICONSTOP);
		
	}
}


void CFileView::OnEditCopy()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	CString strPath = "C:\\";
	if (strPath.Right(1) != "\\") strPath += "\\";
	strPath += "*.*";

	// ������ ���丮�� ������ ���ʷ� �о��
	CFileFind filefind;
	BOOL bContinue;
	bContinue = filefind.FindFile(strPath);

	// ���丮 Ž��
	int count = 0;
	memset(copyPath, NULL,sizeof(copyPath));
	memset(copyName, NULL, sizeof(copyName));
	for (int nItem = 0; nItem < GetListCtrl().GetItemCount(); nItem++){
		if (filefind.FindNextFile()){
			if (GetListCtrl().GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED){
				FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(nItem);
				copyPath[count] = &item->strFilePath;
				copyName[count] = &item->strFileName;
				count++;
				continue;
			}
			else continue;
		}
	}
	filefind.Close();
	if(count==0) MessageBox("������ ã�� �� �����ϴ�.", "����", MB_ICONSTOP);
}


void CFileView::OnEditPaste()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	//if (copyPath[0] == NULL || copyName[0] == NULL){
	//	MessageBox("���� �� ������ �������ּ���.", "����", MB_ICONSTOP);
	//	return;
	//}
	CString strPath = "C:\\";
	if (strPath.Right(1) != "\\") strPath += "\\";
	strPath += "*.*";

	// ������ ���丮�� ������ ���ʷ� �о��
	CFileFind filefind;
	BOOL bContinue;
	bContinue = filefind.FindFile(strPath);
	int count = 0;
	// ���丮 Ž��
	for (int nItem = 0; nItem < GetListCtrl().GetItemCount(); nItem++){
		if (filefind.FindNextFile()){
			if (GetListCtrl().GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED){				
				if (copyPath[count] == NULL || copyName[count] == NULL){
					MessageBox("���� �� ������ �������ּ���.", "����", MB_ICONSTOP);
					return;
				}
				// �迭 �ֱ� ���� �ʿ�
				FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(nItem);
				CString pastePath = item->strFilePath + "\\" + *copyName[count];

				BOOL res = CopyFile(*copyPath[count], pastePath, FALSE);// TRUE : ���� ���� ������ fail, FALSE : �����
				count++;
				if (!res) MessageBox("���� ���� ����.", "����!", MB_ICONSTOP);
			}
			else continue;
		}
		else MessageBox("������ ã�� �� �����ϴ�.", "����", MB_ICONSTOP); break;
	}
	memset(copyPath, NULL, sizeof(copyPath));
	memset(copyName, NULL, sizeof(copyName));
	filefind.Close();
	if (count==0) MessageBox("���� �� ������ �������ּ���.", "����", MB_ICONSTOP);
}


void CFileView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
	int index = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (index == -1) return; // ���� ���� �� ����
	FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(index);

	CString result = GetListCtrl().GetItemText(index, 0);
	CString temp;
	temp.Format(_T("index: %d, result: %s, path: %s\n"), index, result, item->strFilePath);

	OutputDebugString(temp);
	*pResult = 0;

	CString path = item->strFilePath;
	CFileFind filefind;
	BOOL bContinue = filefind.FindFile(path);
	// ������ ǥ�õǰ� �ִ� �����͸� ���� �޸� ����
	if (!bContinue) return;
	filefind.FindNextFileA();
	if (filefind.IsDirectory()){
		FreeItemMemory();
		// ������ ǥ�õǰ� �ִ� ���� ����
		GetListCtrl().DeleteAllItems();
		SetListData(path);
	}
	else{
		::ShellExecuteA(NULL, _T("open"), _T(path), NULL, NULL, SW_SHOW);
	}
}

void CFileView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	int index = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (index == -1) {
		return;
	}
	else{
		FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(index);
		CMenu popup;
		CMenu* pMenu;

		popup.LoadMenu(IDR_POPUP);
		pMenu = popup.GetSubMenu(0);

		pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, point.x, point.y, this);
	}



}
