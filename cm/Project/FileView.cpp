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

// 리스트뷰의 스타일 설정
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

// 리스트뷰 초기화
int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 운영체제가 관리하는 이미지 리스트 얻기
	GetSystemImageList();
	// 리스트 컨트롤에 컬럼 추가
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

// 운영체제가 관리하는 이미지 리스트 얻기
void CFileView::GetSystemImageList()
{
	HIMAGELIST	hSystemImageList;
	SHFILEINFO	info;

	// 운영체제가 관리하는 작은 아이콘 이미지 리스트와 연결
	hSystemImageList = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T("C:\\"),
		0, &info, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	m_imgSmallList.Attach(hSystemImageList);
	m_imgSmallList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	// 운영체제가 관리하는 작은 아이콘 이미지 리스트와 연결
	hSystemImageList = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T("C:\\"),
		0, &info, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_ICON);
	m_imgLargeList.Attach(hSystemImageList);
	m_imgLargeList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	// 리스트 컨트롤과 이미지 리스트 연결
	GetListCtrl().SetImageList(&m_imgLargeList, LVSIL_NORMAL);
	GetListCtrl().SetImageList(&m_imgSmallList, LVSIL_SMALL);
}


void CFileView::OnDestroy()
{
	CListView::OnDestroy();
	FreeItemMemory();

	// 운영체제가 관리하는 이미지 리스트와 연결 해제
	m_imgSmallList.Detach();
	m_imgLargeList.Detach();
}

// 리스트 컨트롤에 컬럼 추가
void CFileView::InsertColumn()
{
	GetListCtrl().InsertColumn(0, "파일명", LVCFMT_LEFT, 200);
	GetListCtrl().InsertColumn(1, "크기", LVCFMT_RIGHT, 100);
	GetListCtrl().InsertColumn(2, "바뀐 날짜", LVCFMT_CENTER, 200);
}

// 리스트뷰의 초기화면 표시
void CFileView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	// 이전에 표시되고 있던 데이터를 위한 메모리 해제
	FreeItemMemory();
	// 이전에 표시되고 있던 내용 삭제
	GetListCtrl().DeleteAllItems();
	// 지정된 디렉토리의 서브 디렉토리와 파일을 표시
	SetListData("C:\\");
}

// 트리뷰에서 선택된 디렉토리가 바뀔 때마다 리스트뷰 갱신
void CFileView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (lHint)
	{
		// 이전에 표시되고 있던 데이터를 위한 메모리 해제
		FreeItemMemory();
		// 이전에 표시되고 있던 내용 삭제
		GetListCtrl().DeleteAllItems();
		// 지정된 디렉토리의 서브 디렉토리와 파일을 표시
		SetListData((LPCTSTR)lHint);
	}
	else
		CListView::OnUpdate(pSender, lHint, pHint);
}

// 지정된 디렉토리의 서브 디렉토리와 파일을 표시
int CFileView::SetListData(LPCTSTR pszPath)
{
	// 지정된 디렉토리 뒤에 "\\*.*"를 덧붙임
	// 예: pszPath = "C:\\Temp"이면, strPath = "C:\\Temp\\*.*"가 됨
	CString strPath = pszPath;
	if (strPath.Right(1) != "\\") strPath += "\\";
	strPath += "*.*";

	// 지정된 디렉토리의 파일을 차례로 읽어옴
	CFileFind filefind;
	BOOL bContinue;
	if (!(bContinue = filefind.FindFile(strPath)))
		return -1;
	int nCount = 0;

	CString strFilename;
	// 디렉토리 탐색
	while (bContinue)
	{
		bContinue = filefind.FindNextFile();
		strFilename = filefind.GetFileName();
		if (strFilename != "." && strFilename != "..")
			AddItem(nCount++, &filefind);  // 리스트 컨트롤에 추가
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
	// 데이터를 저장할 구조체의 메모리 공간 할당
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
	// 데이터 설정
	pItem->strFileName = pFileFind->GetFileName();
	pItem->nFileSize = pFileFind->GetLength();
	pItem->strFilePath = pFileFind->GetFilePath();
	pFileFind->GetLastWriteTime(pItem->timeLastWriteTime);
	// 리스트 컨트롤에 항목을 추가하기 위해 LV_ITEM 구조체 설정
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.iItem = nIndex;
	lvi.iSubItem = 0;
	lvi.iImage = GetIconIndex(pFileFind->GetFilePath());
	lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.lParam = (LPARAM)pItem;

	// 리스트 컨트롤에 데이터 저장
	if (GetListCtrl().InsertItem(&lvi) == -1)
		return FALSE;

	return TRUE;
}



// 데이터를 저장하는데 할당한 메모리를 해제
void CFileView::FreeItemMemory()
{
	for (int i = 0; i<GetListCtrl().GetItemCount(); i++)
		delete (FILEITEM *)GetListCtrl().GetItemData(i);
}

// 이미지 리스트의 인덱스 얻음
int CFileView::GetIconIndex(CString FileName)
{
	SHFILEINFO    sfi;
	SHGetFileInfo((LPCTSTR)FileName, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	return sfi.iIcon;
}


// 리스트 컨트롤에 파일 정보 표시, LVN_GETDISPINFO 통지 메시지에 대한 핸들러 함수
void CFileView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	CString str;
	if (pDispInfo->item.mask & LVIF_TEXT)
	{
		FILEITEM *pItem = (FILEITEM *)pDispInfo->item.lParam;

		switch (pDispInfo->item.iSubItem)
		{
		case 0:	// 파일명
			::lstrcpy(pDispInfo->item.pszText, (LPCTSTR)pItem->strFileName);
			break;
		case 1: // 크기
			str.Format("%u", pItem->nFileSize);
			::lstrcpy(pDispInfo->item.pszText, (LPCTSTR)str);
			break;
		case 2: // 바뀐 날짜
			str = pItem->timeLastWriteTime.Format("%Y-%m-%d %H:%M");
			::lstrcpy(pDispInfo->item.pszText, (LPCTSTR)str);
			break;
		}
	}
}

// 리스트 컨트롤의 내용 정렬, LVN_COLUMNCLICK 통지 메시지의 핸들러 함수
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

	switch (nSubItem)
	{
	case 0:		// 파일명으로 정렬
		nResult = pItem1->strFileName.CompareNoCase(pItem2->strFileName);
		break;
	case 1: 	// 파일 크기로 정렬
		nResult = pItem1->nFileSize - pItem2->nFileSize;
		break;
	case 2: 	// 파일의 마지막 변경 일자로 정렬
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


FILEITEM* CFileView::GetSelectItem(CString cPath)
{
	FILEITEM * items[10];
	int index = 0;
	int count = 0;
	while (GetListCtrl().GetNextItem(-1, LVNI_SELECTED))
	{
		index = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
		if (index == -1)
			return NULL;

		FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(index);
		items[count++] = item;
	}
	if (items != NULL)
		return *items;
	return NULL;
}


void CFileView::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CString path = GetDocument()->GetPathName();
	FILEITEM * item;
	item = GetSelectItem(path);
	CString fileName = _T("");
	//int index = 0;
	//int delCount = 0;
	//int temp=0;
	//while (GetListCtrl().GetNextItem(-1, LVNI_SELECTED))
	//{
	//	index = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	//	if (index == -1)
	//		return;
	//	FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(index);
	//	OutputDebugString("EditCut Path : " + item->strFilePath+ "\n");
	//	// 삭제 여부 재확인
	//	fileName = item->strFileName;
	//	fileName.Format("%s를 삭제하시겠습니까?", fileName);
	//	temp = AfxMessageBox(fileName, MB_OKCANCEL);
	//	if (temp != IDOK)
	//		continue;
	//	else
	//	{
	//		// 파일 삭제(리스트 & File)
	//		GetListCtrl().DeleteItem(index);
	//		CFile::Remove(fileName);
	//	}
	//}

	// 디렉토리 탐색
	int nItem = GetListCtrl().GetItemCount();
	int itemCount = 0;
	for (itemCount; itemCount<nItem; itemCount++){
		if (GetListCtrl().GetItemState(itemCount, LVIS_SELECTED) == LVIS_SELECTED){
			FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(itemCount);

			// 삭제 여부 재확인
			CString fileName = item->strFileName;
			fileName.Format("%s를 삭제하시겠습니까?", fileName);
			int temp = AfxMessageBox(fileName, MB_OKCANCEL);
			if (temp != 1) continue;

			// 파일 삭제(리스트 & File)
			GetListCtrl().DeleteItem(itemCount);
			CString path = item->strFilePath;
			CFile::Remove(path);
			itemCount--;
		}
		else continue;
	}
	if (GetListCtrl().GetItemCount() != itemCount)
	{
		CString countStr; countStr.Format("파일 %d개 삭제 완료", GetListCtrl().GetItemCount()-itemCount);
		MessageBox(countStr, "성공", MB_ICONSTOP);
	}
	else
	{
		MessageBox("삭제할 파일을 선택해주세요.", "실패", MB_ICONSTOP);
	}
}


void CFileView::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	// 디렉토리 탐색
	for (int nItem = 0; nItem < GetListCtrl().GetItemCount(); nItem++){
		if (GetListCtrl().GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED){
			FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(nItem);
			copyPath = item->strFilePath;
			copyName = item->strFileName;
			break;
		}
		else continue;
	}
}


void CFileView::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (copyPath == "" || copyName == ""){
		MessageBox("복사 할 파일을 선택해주세요.", "실패", MB_ICONSTOP);
		return;
	}

	// 디렉토리 탐색
	for (int nItem = 0; nItem < GetListCtrl().GetItemCount(); nItem++){
		if (GetListCtrl().GetItemState(nItem, LVIS_SELECTED) == LVIS_SELECTED){
			FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(nItem);
			CString pastePath = item->strFilePath + "\\" + copyName;
			BOOL res = CopyFile(copyPath, pastePath, FALSE);
			// TRUE : 같은 파일 있으면 fail, FALSE : 덮어쓰기
			CString openPath = GetDocument()->GetPathName();
			OutputDebugString("Get Document Path : " + openPath + "\n");

			if (!res) MessageBox("파일 복사 실패.", "실패!", MB_ICONSTOP);
			else copyPath = ""; copyName = ""; return;
		}
		else continue;
	}
	CString openPath = GetDocument()->GetPathName();
	OutputDebugString("Get Document Path : " + openPath+"\n");
	MessageBox("복사 할 폴더를 선택해주세요.", "실패", MB_ICONSTOP);
}



void CFileView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
	int index = GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if (index == -1) return; // 공백 선택 시 종료
	FILEITEM *item = (FILEITEM *)GetListCtrl().GetItemData(index);

	CString result = GetListCtrl().GetItemText(index, 0);
	CString temp;
	temp.Format(_T("index: %d, path: %s\n"), index,item->strFilePath);

	OutputDebugString(temp);
	*pResult = 0;
	// 클릭한 
	CString path = item->strFilePath;
	CFileFind filefind;
	BOOL bContinue = filefind.FindFile(path);
	if (!bContinue) return;

	filefind.FindNextFile();
	if (filefind.IsDirectory()){ // 디렉터리일 때
		GetDocument()->SetPathName(path);
		OutputDebugString("DBClick Document Path : " + path + "\n");
		
		// 이전에 표시되고 있던 데이터를 위한 메모리 해제
		FreeItemMemory();
		// 이전에 표시되고 있던 내용 삭제
		GetListCtrl().DeleteAllItems();
		// 새 경로로 FileView 설정
		SetListData(path);
	}
	else{// 파일일 때
		::ShellExecuteA(NULL, _T("open"), _T(path), NULL, NULL, SW_SHOW);
	}
}

void CFileView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
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

		pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, point.x, point.y, GetParent());
	}
}



