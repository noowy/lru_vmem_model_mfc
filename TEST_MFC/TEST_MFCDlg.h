#ifndef TEST_MFCDLG_HPP
#define TEST_MFCDLG_HPP
// TEST_MFCDlg.h : header file
//

#pragma once
class MemoryController;

struct CLoadedPage
{
	CStatic *frame;
	CStatic *idxView;
	int index;
	~CLoadedPage() { delete frame; delete idxView; };
};


// CTESTMFCDlg dialog
class CTESTMFCDlg : public CDialogEx
{
// Construction
public:
	CTESTMFCDlg(CWnd* pParent = nullptr);	// standard constructor
	~CTESTMFCDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	MemoryController *memController;

	int current_pages_amount = 0;
	int current_queue_size = 0;
	int queue_internal_level = 0;
	int page_faults = 0;
	
	const int MAX_ALLOWED_QUEUE_SIZE = 36;
	const int MAX_PAGE_POOL_SIZE = 11;

	std::vector<CEdit*> page_queue_view;
	std::vector<CLoadedPage*> page_pool_view;
	
	std::vector<int> page_queue;
	std::vector<int> page_pool;

	void getPageQueueViewState();
public:
	void replacePageInPool(const int& pageIdx, const int& replacementPage);
	void addPageToPool(const int& pageIdx);
	int popPageFromQueue();
	int getPagePoolSize();
	int getCurrentQueueSize();
	void updatePagePoolView();
	void updatePageQueueView();
	void incrementPageFaults();

	afx_msg void OnBnClickedButton1();
	CStatic m_Current_Pages;
	afx_msg void OnBnClickedButton2();
	CStatic m_Page_Queue;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CStatic page_faults_view;
	afx_msg void OnBnClickedButton5();
};

#endif