
// TEST_MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TEST_MFC.h"
#include "TEST_MFCDlg.h"
#include "afxdialogex.h"
#include "MemoryController.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int page_call_counter = 0;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
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


// CTESTMFCDlg dialog



CTESTMFCDlg::CTESTMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_MFC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memController = new MemoryController(this);
}

CTESTMFCDlg::~CTESTMFCDlg()
{
	for (auto it = page_queue_view.begin(); it != page_queue_view.end(); it++)
	{
		delete *it;
	}
	for (auto it = page_pool_view.begin(); it != page_pool_view.end(); it++)
	{
		delete *it;
	}
}

void CTESTMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CURR_PAGES, m_Current_Pages);
	DDX_Control(pDX, IDC_PAGE_QUEUE, m_Page_Queue);
	DDX_Control(pDX, IDC_PAGE_FAULTS, page_faults_view);
}

BEGIN_MESSAGE_MAP(CTESTMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTESTMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTESTMFCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTESTMFCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTESTMFCDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTESTMFCDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CTESTMFCDlg message handlers

BOOL CTESTMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTESTMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTESTMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTESTMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTESTMFCDlg::OnBnClickedButton1()
{
	if (memController->isLaunched)
	{
		MessageBox(_T("You can not add more pages while the process is Running \
					   Please pause the process to add more pages."));
		return;
	}

	if (current_pages_amount > MAX_PAGE_POOL_SIZE - 1) // -1 for count starts from 0
	{
		MessageBox(_T("This is a demo version. \
					  Please buy full version to have more than 11 pages loaded"));
		return;
	}
	CStatic *frame = new CStatic();
	frame->Create(
					NULL, 
					WS_CHILD | WS_VISIBLE | SS_BLACKFRAME, 
					CRect(10 + (current_pages_amount * 50), 30, 50 + (current_pages_amount * 50), 220), 
					&m_Current_Pages
					);
	CStatic *idxView = new CStatic();
	idxView->Create(
			NULL,
			WS_CHILD | WS_VISIBLE | ES_CENTER,
			CRect(CPoint(8, 87), CSize(24, 16)),
			frame
		);
	CLoadedPage *loaded_page = new CLoadedPage();
	loaded_page->frame = frame;
	loaded_page->idxView = idxView;
	loaded_page->index = 10;
	page_pool_view.push_back(loaded_page);
	current_pages_amount++;
}


void CTESTMFCDlg::updatePagePoolView()
{
	CString num_str;
	LPTSTR buffer;
	auto page = page_pool.begin();
	int num_len;

	for (auto it = page_pool_view.begin(); it != page_pool_view.end(); it++)
	{
		if (page == page_pool.end())
		{
			(*it)->idxView->SetWindowTextW(NULL);
			(*it)->index = NULL;
			continue;
		}

		(*it)->index = *page;
		num_len = std::to_string(*page).length();
		num_str = std::to_string(*page).c_str();
		buffer = num_str.GetBuffer(num_len);

		(*it)->idxView->SetWindowTextW(buffer);
		
		page++;
	}
}

void CTESTMFCDlg::updatePageQueueView()
{
	CString num_str;
	LPTSTR buffer;
	auto page = page_queue.begin();
	int num_len;

	for (auto it = page_queue_view.begin(); it != page_queue_view.end(); it++)
	{
		if (page == page_queue.end())
		{
			(*it)->SetWindowTextW(nullptr);
			continue;
		}

		num_len = std::to_string(*page).length();
		num_str = std::to_string(*page).c_str();
		buffer = num_str.GetBuffer(num_len);

		(*it)->SetWindowTextW(buffer);

		page++;
	}
}

void CTESTMFCDlg::incrementPageFaults()
{
	page_faults++;
	LPTSTR buffer;
	CString num_str;
	int num_len;

	num_len = std::to_string(page_faults).length();
	num_str = std::to_string(page_faults).c_str();
	buffer = num_str.GetBuffer(num_len);

	page_faults_view.SetWindowTextW(buffer);
}

void CTESTMFCDlg::getPageQueueViewState()
{
	page_queue.clear();

	int len;
	CString str;
	LPTSTR buffer;
	int num;
	for (int i = 0; i < page_queue_view.size(); i++)
	{
		len = page_queue_view[i]->LineLength(page_queue_view[i]->LineIndex(0));
		buffer = str.GetBuffer(len);
		page_queue_view[i]->GetLine(0, buffer, len);
		buffer[len] = '\0';
		num = _ttoi(str);

		if (num == 0)
			continue;

		if (num < 0)
		{
			MessageBox(_T("Page index cannot be lower than zero."));
			page_queue.clear();
			return;
		}

		page_queue.push_back(num);
		//page_queue.insert(page_queue.begin(), num);
	}
}

void CTESTMFCDlg::replacePageInPool(const int & pageIdx, const int & replacementPage)
{
	for (int i = 0; i < page_pool.size(); i++)
	{
		if (page_pool[i] == pageIdx)
		{
			page_pool.at(i) = replacementPage;
			return;
		}
	}
}

void CTESTMFCDlg::addPageToPool(const int & pageIdx)
{
	page_pool.insert(page_pool.begin(), pageIdx);
}

int CTESTMFCDlg::popPageFromQueue()
{
	if (page_queue.empty())
		return EMPTY_QUEUE;

	int page = page_queue.front();
	page_queue.erase(page_queue.begin());
	return page;
}

int CTESTMFCDlg::getPagePoolSize()
{
	return page_pool_view.size();
}

int CTESTMFCDlg::getCurrentQueueSize()
{
	return page_queue.size();;
}

void CTESTMFCDlg::OnBnClickedButton2()
{
	if (memController->isLaunched)
	{
		MessageBox(_T("You can not add more pages while the process is Running \
					   Please pause the process to add more pages."));
		return;
	}

	if (current_queue_size > MAX_ALLOWED_QUEUE_SIZE - 1) // -1 for the count starts from 0 
	{
		MessageBox(_T("This is a demo version. \
					  Please buy full version to have more than 36 pages in queue"));
		return;
	}
	if (current_queue_size % (MAX_ALLOWED_QUEUE_SIZE / 2) == 0 && current_queue_size > 0)
		queue_internal_level = 32;

	CEdit *page = new CEdit();
	page->Create(
		ES_CENTER | ES_NUMBER | WS_CHILD | WS_VISIBLE | WS_TABSTOP | !WS_BORDER,
		CRect(8 + ((current_queue_size % (MAX_ALLOWED_QUEUE_SIZE / 2)) * 30),
				18 + queue_internal_level, 
				32 + ((current_queue_size % (MAX_ALLOWED_QUEUE_SIZE / 2)) * 30),
				46 + queue_internal_level),
		&m_Page_Queue,
		2000 + current_queue_size
	);
	page_queue_view.push_back(page);
	current_queue_size++;
}


void CTESTMFCDlg::OnBnClickedButton3()
{
	getPageQueueViewState();
	memController->onProcessLaunched();
}


void CTESTMFCDlg::OnBnClickedButton4()
{
	if (memController->isLaunched)
	{
		MessageBox(_T("You can not run a single step while the process is Running \
					   Please pause the process to add more pages."));
		return;
	}

	getPageQueueViewState();
	memController->onPageCalled();
	page_call_counter++;
}


void CTESTMFCDlg::OnBnClickedButton5()
{
	memController->onProcessPaused();
}
