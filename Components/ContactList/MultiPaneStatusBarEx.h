/////////////////////////////////////////////////////////////////////////////////////////
// 
//	MultiPaneStatusBarEx.h
//
//  Author: Pablo Aliskevicius.
//  Copyright (C) 2004 Pablo Aliskevicius.
//
//  The code and information  is provided  by the  author and  copyright  holder 'as-is',
//  and any express or implied  warranties,  including,  but not  limited to, the implied
//  warranties of  merchantability and  fitness for a particular purpose  are disclaimed.
//  In no event shall the author or copyright holders be liable for any direct, indirect,
//  incidental, special, exemplary, or consequential damages (including,  but not limited
//  to, procurement of substitute goods or services;  loss of use,  data, or profits;  or
//  business  interruption)  however caused  and on any  theory of liability,  whether in
//  contract, strict liability,  or tort  (including negligence or otherwise)  arising in
//  any way out of the use of this software,  even if advised of the  possibility of such
//  damage.
//
/////////////////////////////////////////////////////////////////////////////////////////
//
// Classes in this file:
//
// CProgressBarInPaneImpl<T> - CMPSBarWithProgress, 
//							   CMPSBarWithProgressAndBMP, 
//                            [CMPSBarWithProgressAndAnimation - not implemented],
//							   CMPSBarWithAll
// CBitmapInPaneImpl<T>     -  CMPSBarWithBitmaps,
//                     		   CMPSBarWithProgressAndBMP,
//           				  [CMPSBarWithAnimAndBMP - not implemented ],
//							   CMPSBarWithAll
// CAnimationInPaneImpl<T>   - CMPSBarWithAnimation,
//                            [CMPSBarWithProgressAndAnimation - not implemented],
//           				  [CMPSBarWithAnimAndBMP - not implemented ],
//                             CMPSBarWithAll

// MultiPaneStatusBarWithProgress.h: interface for the CMPSBarWithProgress class.
//
// This class extends WTL::CMultiPaneStatusBarCtrl to support creating and updating a 
// progress bar in one of its panes, and moving that progress bar around.
//
// Usage:
// ======
//
//    Wherever you'd create a CMultiPaneStatusBarCtrl, create one of the CMPSBarWithXXXXX
//    classes instead (the one which provides the functionality you'll use).
//
//    Showing progress bars:
//	  ----------------------
//
//	     Show a progress bar using ProgCreate()	
//		 Hide it using ProgDestroyWindow()
//		 Display progress using ProgSetPos(), or ProgStepIt() after	ProgSetStep()
//		 All other functions of WTL::CProgressBarCtrl, version 7.1, are also exposed.
//
//    Showing bitmaps:
//	  ----------------
//
//		 Use SetBitmap() to display a bitmap. There are two versions of the function.
//		 Use DestroyBitmap() to hide the bitmap, and optionally call DestroyObject() 
//		 on its handle.
//
//    Showing animations:
//	  -------------------
//
//		 Use AnimCreate() to display an animation.
//	  	 Use AnimDestroyWindow() to close an animation.
//
//    Showing anything (I don't like it, so there's an #ifdef):
//	  ---------------------------------------------------------
//
//       #define MULTI_PANE_WITH_ANY before including this file.
//
//		 Use CMPSBarWithAll as your member variable.
//
//		 Use AttachWindow() to attach an HWND to a pane.
//		 Use DetachWindow() to detach that HWND from that pane.
//
//
/////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIPANESTATUSBARWITHPROGRESS_H__D2F37B4C_6E3D_450D_94B5_B14D377226FA__INCLUDED_)
#define AFX_MULTIPANESTATUSBARWITHPROGRESS_H__D2F37B4C_6E3D_450D_94B5_B14D377226FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __ATLCTRLS_H__
#error You have to include <ATLCTRLS.H> for CProgressBarCtrl, CStatic and CAnimateCtrl.
#endif
#ifndef __ATLCTRLX_H__
#error You have to include <ATLCTRLX.H> for CMultiPaneStatusBarCtrlImpl<T>.
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Implementation classes, meant to be inherited from (mixins).
/////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class CProgressBarInPaneImpl
{
	public:
		typedef CProgressBarInPaneImpl<T> CThisClass;
		// Constructor, destructor...
		CProgressBarInPaneImpl<T>(): m_iProgressPane(-1) 
		{
		}
		virtual ~CProgressBarInPaneImpl<T>() 
		{
			ProgDestroyWindow();
		}
		// UpdatePanesLayout() override, to handle resizing the progress bar whenever relevant
		BOOL UpdatePanesLayout(void)
		{
			if (m_iProgressPane != -1)
			{
				T* pt = static_cast<T*>(this);
				RECT rc;
				pt->GetRect(m_iProgressPane, &rc);
				// ::InflateRect(&rc, -1, -1); 
				m_Progress.MoveWindow(&rc);
			}
			return TRUE; // Mixed function.
		}
		
		BEGIN_MSG_MAP(CProgressBarInPaneImpl<T>)
			MESSAGE_HANDLER(SB_SIMPLE, OnSimple)
		END_MSG_MAP()
			
		LRESULT OnSimple(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			if (::IsWindow(m_Progress.m_hWnd))
				m_Progress.ShowWindow(wParam ? SW_HIDE: SW_SHOW);

			bHandled = FALSE;
			return 0;
		}
		//////////////////////////////////////////////////////////////////////////////////
		// Prog* functions enable access to the contained CProgressBarCtrl. 
		// We'll expose most of its functionality, and a few additional bits.
		//////////////////////////////////////////////////////////////////////////////////

		BOOL ProgCreate(int iPane,         // Status pane where we'll create the progress bar.
			int nMin = 0, int nMax = 100,  // Progress bar initial range
			DWORD dwStyle = WS_CHILD | WS_VISIBLE | PBS_SMOOTH, // Progress bar styles
			DWORD dwExStyle = 0
			) 
		{
			// Check there is such a pane
			T* pt = static_cast<T*>(this);
			ATLASSERT(::IsWindow(pt->m_hWnd));
			if (iPane >= pt->m_nPanes)
				return FALSE;
			// Check there is not a progress bar already open.
			if (::IsWindow(m_Progress.m_hWnd))
				return FALSE;
			// Get the pane's rectangle
			RECT rc;
			pt->GetRect( iPane, &rc );
			// ::InflateRect(&rc, -1, -1); 
			
			// Create the window, using the status bar (this) as a parent.
			m_Progress.Create ( pt->m_hWnd, rc, NULL, dwStyle,  dwExStyle);
			// Set the progress bar's range and position
			m_Progress.SetRange ( nMin, nMax ); 
			m_Progress.SetPos ( nMin );   
			m_Progress.SetStep ( 1 );
			// Hold this, we'll need it to move around.
			m_iProgressPane = iPane;
			return TRUE;
		}
		// This function can be used to close a progress bar, after ending whatever 
		// lengthy operation justified opening it to begin with.
		void ProgDestroyWindow(void)
		{
			if (::IsWindow(m_Progress.m_hWnd))
			{
				m_Progress.ShowWindow(SW_HIDE);
				m_Progress.DestroyWindow();
			}
			m_iProgressPane = -1;
			m_Progress.m_hWnd = NULL;
		}
		// Just in case. 
		int   ProgGetPane() const                 { return m_iProgressPane; }
		//////////////////////////////////////////////////////////////////////////////////
		// CProgressBarCtrl functionality (WTL version 7.1):
		// CWindow functionality in CProgressBarCtrl is hidden by design.
		//
		DWORD ProgSetRange(int nLower, int nUpper)  { return m_Progress.SetRange(nLower, nUpper); }
		int   ProgSetPos(int nPos)                  { return m_Progress.SetPos(nPos); }
		int   ProgOffsetPos(int nPos)               { return m_Progress.OffsetPos(nPos); }
		int   ProgSetStep(int nStep)                { return m_Progress.SetStep(nStep); }
		UINT  ProgGetPos() const                    { return m_Progress.GetPos(); }
		
		void  ProgGetRange(PPBRANGE pPBRange) const { m_Progress.GetRange(pPBRange); } 
		int   ProgGetRangeLimit(BOOL bLimit)  const { return m_Progress.GetRangeLimit(bLimit); }
		DWORD ProgSetRange32(int nMin, int nMax)    { return m_Progress.SetRange32(nMin, nMax); }
		
#if (_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
		COLORREF ProgSetBarColor(COLORREF clr)      { return m_Progress.SetBarColor(clr); }
		COLORREF ProgSetBkColor(COLORREF clr)       { return m_Progress.SetBkColor(clr); }
#endif //(_WIN32_IE >= 0x0400) && !defined(_WIN32_WCE)
		
#if (_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)
		BOOL ProgSetMarquee(BOOL bMarquee, UINT uUpdateTime = 0U)
													{ return m_Progress.SetMarquee(bMarquee, uUpdateTime); }
#endif //(_WIN32_WINNT >= 0x0501) && defined(PBM_SETMARQUEE)
		int ProgStepIt()                            { return m_Progress.StepIt(); }
		
	protected:
	//////////////////////////////////////////////////////////////////////////////////
	// Member variables (of course, protected)
	//
		CProgressBarCtrl m_Progress;  // This is the contained control.
		int m_iProgressPane;          // Pane ordinal where the progress bar resides, or -1 when off.
	
}; // CProgressBarInPaneImpl


// This one also handles bitmaps

template <class T> class CBitmapInPaneImpl
{
public:
	// You can have up to 15 bitmaps per status bar.
	enum { MAX_MANAGED = 15 };

	BEGIN_MSG_MAP(CBitmapInPaneImpl<T>)
		MESSAGE_HANDLER(SB_SIMPLE, OnSimple)
	END_MSG_MAP()
	
	// Show bitmaps when the status bar is in multi pane state, hide them when it's simple.
	LRESULT OnSimple(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int sw = (wParam) ? SW_HIDE: SW_SHOW;
		// Hide all children (static bitmaps)
		for (int i = 0; i < MAX_MANAGED; i++)
		{
			if (m_Manager[i].hw)
				::ShowWindow(m_Manager[i].hw, sw);
		}
		bHandled = false;
		return 0;
	}
	
	// Move bitmaps around to stay on top of their panes.
	BOOL UpdatePanesLayout(void)
	{
		RECT rc;	
		T *pT = static_cast<T*>(this);
		for (int i = 0;   i < MAX_MANAGED; i++)
		{
			if (m_Manager[i].hw)
			{
				pT->GetRect( m_Manager[i].iPane, &rc );
				// ::InflateRect(&rc, -1, -1); 
				CStatic cs(m_Manager[i].hw);
				cs.MoveWindow(&rc);
				cs.Invalidate();
			}
		} // for
		return TRUE;
	}
	// Add a bitmap to the status bar, in a pane chosen by ordinal.
	BOOL SetBitmap(int iPane,    // Zero based ordinal (not resource ID) of the chosen pane.
		           HBITMAP hbmp, // Handle to a bitmap.
		           bool bManage = false) // If true, DestroyObject() will be called on the HBITMAP.
	{
		if (NULL == hbmp)
			return false;
		// If the pane was in use, release it.
		if (!DestroyBitmap(iPane))
			return FALSE;
		// Use the first available array entry to put the selected bitmap.
		T *pT = static_cast<T *>(this);
		for (int i = 0; i < MAX_MANAGED; i++)
		{
			if (!m_Manager[i].hw)
			{
				RECT rct;
				pT->GetRect( iPane, &rct);
				// ::InflateRect(&rct, -1, -1);
				CStatic cs;
				cs.Create(pT->m_hWnd, rct,  NULL,
					WS_CHILDWINDOW | WS_VISIBLE | WS_DISABLED | SS_BITMAP | SS_LEFT | SS_SUNKEN, 
					WS_EX_NOPARENTNOTIFY, WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
				cs.SetBitmap(hbmp);
				m_Manager[i].hw = cs.m_hWnd;
				m_Manager[i].iPane = iPane;
				m_Manager[i].hbmp = hbmp;
				m_Manager[i].bManage = bManage;
				UpdatePanesLayout();
				return TRUE;
			}
		}
		return FALSE;
	}
	// Same as the former, but loads the bitmap from a resource.
	BOOL SetBitmap(int iPane, int iBmpResource)
	{
		HBITMAP hb = ::LoadBitmap(_Module.m_hInstResource, MAKEINTRESOURCE(iBmpResource));
		if (NULL == hb)
			return FALSE;
		// The caller has no access to the bitmap's handle, we MUST manage it from here.
		return SetBitmap(iPane, hb, true);
	}
	// If there is a bitmap in the selected pane, destroy it.
	BOOL DestroyBitmap(int iPane)
	{
		T *pT = static_cast<T *>(this);
		// Check we're really talking about a pane.
		if (iPane < 0 || iPane >= pT->m_nPanes)
			return FALSE;
		// Find the pane in the array.
		for (int i = 0; i < MAX_MANAGED; i++)
		{
			if (m_Manager[i].iPane == iPane)
			{
				// If the pane is in use, release whatever is there.
				if (m_Manager[i].bManage)
					::DeleteObject(m_Manager[i].hbmp);
				::DestroyWindow(m_Manager[i].hw);
				// Initialize array entry to suitable values.        
				m_Manager[i].Clear();
				break;
			}
		}
		return TRUE; // The pane is free.
	}
	CBitmapInPaneImpl<T>()
	{
	}
	
	virtual ~CBitmapInPaneImpl<T>()
	{
		// Destroy all windows, release all managed bitmaps.
		for (int i = 0; i < MAX_MANAGED; i++)
		{
			if (m_Manager[i].hw)
				::DestroyWindow(m_Manager[i].hw);
			if (m_Manager[i].bManage)
				::DeleteObject(m_Manager[i].hbmp);
		}
	}
	
   protected:
		// This class ties one bitmap/HWND/pane.	   
	   class CBmpManager
	   {
	   public:
		   HWND hw;
		   HBITMAP hbmp;
		   int iPane;
		   bool bManage; 
		   CBmpManager(): hw(NULL), hbmp(NULL), bManage(false), iPane(-1)
		   {}
		   void Clear(void)
		   {
				hbmp = NULL;
				hw = NULL;
				bManage = false;
				iPane = -1;
		   }
	   };  // class CBmpManager
	   CBmpManager m_Manager[MAX_MANAGED]; // 15 are more than enough, I hope.
	   
}; // class CBitmapInPaneImpl


template <class T>
class CAnimationInPaneImpl
{
public:
	CAnimationInPaneImpl<T>(): m_iAnimPane(-1) {}
	virtual ~CAnimationInPaneImpl<T>()
	{
		AnimDestroyWindow();
	}
	void AnimDestroyWindow(void)
	{
		if (::IsWindow(m_Anim.m_hWnd))
		{
			m_Anim.Close();
			m_Anim.ShowWindow(SW_HIDE);
			m_Anim.DestroyWindow();
		}
		m_Anim.m_hWnd = NULL;
		m_iAnimPane = -1;
	}
	BEGIN_MSG_MAP(CMPSBarWithBitmaps<TParent>)
		MESSAGE_HANDLER(SB_SIMPLE, OnSimple)
	END_MSG_MAP()
		
	// The animation should be displayed only when the status bar is in multiple pane mode.
	LRESULT OnSimple(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (::IsWindow(m_Anim.m_hWnd))
			m_Anim.ShowWindow(wParam ? SW_HIDE: SW_SHOW);

		bHandled = FALSE;
		return 0;
	}

	// Move the animation around when the pane on top of which it lies also moves.
    BOOL UpdatePanesLayout(void)
	{
		if (m_iAnimPane != -1)
		{
			T* pt = static_cast<T*>(this);
			RECT rc;
			pt->GetRect(m_iAnimPane, &rc);
			::InflateRect(&rc, -1, -1); 
			m_Anim.MoveWindow(&rc);
		}
		return TRUE; // Mixed function.
	}
	// Create an animation.
	BOOL AnimCreate(int iPane,         // Status pane where we'll create the progress bar.
#if _WTL_VER >= 0x0710	// It's nice when WTL stuff makes it into ATL, we can keep hoping...  ;-)
		ATL::_U_STRINGorID FileName,   // Path to an AVI file, or resouce holding it.
#else
		WTL::_U_STRINGorID FileName,   // Path to an AVI file, or resouce holding it.
#endif
		DWORD dwStyle =	ACS_TRANSPARENT | ACS_CENTER | ACS_AUTOPLAY, // OR to this if adding styles
		DWORD dwExStyle = 0
		) 
	{
		// Check there is such a pane
		T* pt = static_cast<T*>(this);
		ATLASSERT(::IsWindow(pt->m_hWnd));
		if (0 > iPane || iPane >= pt->m_nPanes)
			return FALSE;
		// Check there is not an animation already open.
		if (::IsWindow(m_Anim.m_hWnd))
			return FALSE;
		// Get the pane's rectangle
		RECT rc;
		pt->GetRect( iPane, &rc );
		::InflateRect(&rc, -1, -1); 
	
		// Create the window, using the status bar (this) as a parent.
		m_Anim.Create ( pt->m_hWnd, rc, NULL, WS_VISIBLE | WS_CHILD | dwStyle,  dwExStyle); 
		if (!m_Anim.Open(FileName))
			return FALSE;
		if (!m_Anim.Play(0, -1, -1))
			return FALSE;
		// Hold this, we'll need it to move around.
		m_iAnimPane = iPane;
		return TRUE;
	}
	
	//  Simple containment
	BOOL AnimSeek(UINT nTo)                        { return m_Anim.Seek(nTo); }
	BOOL AnimStop()                                { m_Anim.Stop(); }
	BOOL AnimPlay(UINT nFrom, UINT nTo, UINT nRep) { return m_Anim.Play(nFrom, nTo, nRep); }
	
protected:
	CAnimateCtrl m_Anim;	  // The contained control
    int m_iAnimPane;          // Pane ordinal where the progress bar resides, or -1 when off.
};


template <class T> class CAnyhingInPaneImpl
{
public:
	// You can have up to 15 free-style controls per status bar.
	enum { MAX_ANY = 15 };

	BEGIN_MSG_MAP(CAnyhingInPaneImpl<T>)
		MESSAGE_HANDLER(SB_SIMPLE, OnSimple)
	END_MSG_MAP()
	
	// Show bitmaps when the status bar is in multi pane state, hide them when it's simple.
	LRESULT OnSimple(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int sw = (wParam) ? SW_HIDE: SW_SHOW;
		// Hide all children (static bitmaps)
		for (int i = 0; i < MAX_ANY; i++)
		{
			if (m_AnyManager[i].hw)
				::ShowWindow(m_AnyManager[i].hw, sw);
		}
		bHandled = false;
		return 0;
	}
	
	// Move free-style controls around to stay on top of their panes.
	BOOL UpdatePanesLayout(void)
	{
		RECT rc;	
		T *pT = static_cast<T*>(this);
		for (int i = 0;   i < MAX_ANY; i++)
		{
			if (m_AnyManager[i].hw)
			{
				pT->GetRect( m_AnyManager[i].iPane, &rc );
				::InflateRect(&rc, -1, -1); 
				CWindow cs(m_AnyManager[i].hw);
				cs.MoveWindow(&rc);
				cs.Invalidate();
			}
		} // for
		return TRUE;
	}
	// Add a window to the status bar, in a pane chosen by ordinal.
	BOOL AddAny(int iPane, // Zero based ordinal (not resource ID) of the chosen pane.
		        HWND hw, // Handle to a window, whatever control you like (or a dialog of yours)...
		        bool bManage = false) // If true, DestroyWindow() will be called on the HWND.
	{
		if (!::IsWindow(hw))
			return false;
		// If the pane was in use, release it.
		if (!FreeAny(iPane))
			return FALSE;
		// Use the first available array entry to put the selected bitmap.
		T *pT = static_cast<T *>(this);
		for (int i = 0; i < MAX_ANY; i++)
		{
			if (!m_AnyManager[i].hw)
			{
				RECT rct;
				pT->GetRect( iPane, &rct);
				::InflateRect(&rct, -1, -1);
				CWindow wnd;
				wnd.Attach(hw);
				wnd.MoveWindow(&rct);
				m_AnyManager[i].hw = hw;
				m_AnyManager[i].iPane = iPane;
				m_AnyManager[i].bManage = bManage;
				UpdatePanesLayout();
				return TRUE;
			}
		}
		return FALSE;
	}
	// If there is a window in the selected pane, hide it and unlink it from the pane.
	BOOL FreeAny(int iPane)
	{
		T *pT = static_cast<T *>(this);
		// Check we're really talking about a pane.
		if (iPane < 0 || iPane >= pT->m_nPanes)
			return FALSE;
		// Find the pane in the array.
		for (int i = 0; i < MAX_ANY; i++)
		{
			if (m_AnyManager[i].iPane == iPane)
			{
				// If the pane is in use, release whatever is there.
				if (m_AnyManager[i].bManage && ::IsWindow(m_AnyManager[i].hw))
					::DestroyWindow(m_AnyManager[i].hw);
				::ShowWindow(m_AnyManager[i].hw, SW_HIDE);
				// Initialize array entry to suitable values.        
				m_AnyManager[i].Clear();
				break;
			}
		}
		return TRUE; // The pane exists, and is free.
	}
	CAnyhingInPaneImpl<T>()
	{
	}
	
	virtual ~CAnyhingInPaneImpl<T>()
	{
		// Destroy all windows, release all managed bitmaps.
		for (int i = 0; i < MAX_ANY; i++)
		{
			if (m_AnyManager[i].bManage && ::IsWindow(m_AnyManager[i].hw))
				::DestroyWindow(m_AnyManager[i].hw);
		}
	}
	
   protected:
		// This class ties one bitmap/HWND/pane.	   
	   class CAnyManager
	   {
	   public:
		   HWND hw;
		   short iPane;
		   bool bManage; 
		   CAnyManager(): hw(NULL), bManage(false), iPane(-1)
		   {}
		   void Clear(void)
		   {
				hw = NULL;
				bManage = false;
				iPane = -1;
		   }
	   };  // class CBmpManager
	   CAnyManager m_AnyManager[MAX_ANY]; // 15 are more than enough, I hope.
	   
}; // class CAnyhingInPaneImpl



/////////////////////////////////////////////////////////////////////////////////////////
// Concrete classes, you can use them as member variables.
/////////////////////////////////////////////////////////////////////////////////////////

// This class adds progress bar functionality to a multi pane status bar
class CMPSBarWithProgress: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgress>,
	  public CProgressBarInPaneImpl<CMPSBarWithProgress>

{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithProgress"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithProgress)
		CHAIN_MSG_MAP(CProgressBarInPaneImpl<CMPSBarWithProgress>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgress>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgress>::UpdatePanesLayout();
		CProgressBarInPaneImpl<CMPSBarWithProgress>::UpdatePanesLayout();
		return ret;
	}
};	// class CMPSBarWithProgress


// This class adds bitmap functionality (up to 15) to a multi pane status bar
class CMPSBarWithBitmaps:
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithBitmaps>,
	  public CBitmapInPaneImpl<CMPSBarWithBitmaps>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithBitmaps"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithBitmaps)
		CHAIN_MSG_MAP(CBitmapInPaneImpl<CMPSBarWithBitmaps>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithBitmaps>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithBitmaps>::UpdatePanesLayout();
 		CBitmapInPaneImpl<CMPSBarWithBitmaps>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithBitmaps

// This class adds progress bar and bitmap functionality to a multi pane status bar
class CMPSBarWithProgressAndBMP: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgressAndBMP>,
	  public CProgressBarInPaneImpl<CMPSBarWithProgressAndBMP>,
	  public CBitmapInPaneImpl<CMPSBarWithProgressAndBMP>

{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithProgressAndBMP"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithProgressAndBMP)
		CHAIN_MSG_MAP(CProgressBarInPaneImpl<CMPSBarWithProgressAndBMP>)
		CHAIN_MSG_MAP(CBitmapInPaneImpl<CMPSBarWithProgressAndBMP>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgressAndBMP>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithProgressAndBMP>::UpdatePanesLayout();
 		CBitmapInPaneImpl<CMPSBarWithProgressAndBMP>::UpdatePanesLayout();
		CProgressBarInPaneImpl<CMPSBarWithProgressAndBMP>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithProgressAndBMP

// This class adds animation functionality to a multi pane status bar
class CMPSBarWithAnimation: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithAnimation>,
	  public CAnimationInPaneImpl<CMPSBarWithAnimation>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithAnimation"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithAnimation)
		CHAIN_MSG_MAP(CAnimationInPaneImpl<CMPSBarWithAnimation>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithAnimation>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithAnimation>::UpdatePanesLayout();
		CAnimationInPaneImpl<CMPSBarWithAnimation>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithAnimation

// This class adds progress bar, bitmap and animation functionality to a multi pane status bar
class CMPSBarWithPrg_BMP_Anim: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithPrg_BMP_Anim>,
	  public CProgressBarInPaneImpl<CMPSBarWithPrg_BMP_Anim>,
	  public CBitmapInPaneImpl<CMPSBarWithPrg_BMP_Anim>,
	  public CAnimationInPaneImpl<CMPSBarWithPrg_BMP_Anim>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithPrg_BMP_Anim"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithPrg_BMP_Anim)
		CHAIN_MSG_MAP(CProgressBarInPaneImpl<CMPSBarWithPrg_BMP_Anim>)
		CHAIN_MSG_MAP(CAnimationInPaneImpl<CMPSBarWithPrg_BMP_Anim>)
		CHAIN_MSG_MAP(CBitmapInPaneImpl<CMPSBarWithPrg_BMP_Anim>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithPrg_BMP_Anim>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithPrg_BMP_Anim>::UpdatePanesLayout();
 		CBitmapInPaneImpl<CMPSBarWithPrg_BMP_Anim>::UpdatePanesLayout();
 		CAnimationInPaneImpl<CMPSBarWithPrg_BMP_Anim>::UpdatePanesLayout();
		CProgressBarInPaneImpl<CMPSBarWithPrg_BMP_Anim>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithAll

// This class adds progress bar, bitmap and animation functionality to a multi pane status bar
class CMPSBarWithAll: 
      public CMultiPaneStatusBarCtrlImpl<CMPSBarWithAll>,
	  public CProgressBarInPaneImpl<CMPSBarWithAll>,
	  public CBitmapInPaneImpl<CMPSBarWithAll>,
	  public CAnimationInPaneImpl<CMPSBarWithAll>,
	  public CAnyhingInPaneImpl<CMPSBarWithAll>
{
public:

    DECLARE_WND_SUPERCLASS(_T("CMPSBarWithAll"), GetWndClassName())

	BEGIN_MSG_MAP(CMPSBarWithAll)
		CHAIN_MSG_MAP(CProgressBarInPaneImpl<CMPSBarWithAll>)
		CHAIN_MSG_MAP(CAnimationInPaneImpl<CMPSBarWithAll>)
		CHAIN_MSG_MAP(CBitmapInPaneImpl<CMPSBarWithAll>)
		CHAIN_MSG_MAP(CAnyhingInPaneImpl<CMPSBarWithAll>)
		CHAIN_MSG_MAP(CMultiPaneStatusBarCtrlImpl<CMPSBarWithAll>)
	END_MSG_MAP()

	BOOL UpdatePanesLayout(void)
	{
		BOOL ret = CMultiPaneStatusBarCtrlImpl<CMPSBarWithAll>::UpdatePanesLayout();
		CAnyhingInPaneImpl<CMPSBarWithAll>::UpdatePanesLayout();
 		CBitmapInPaneImpl<CMPSBarWithAll>::UpdatePanesLayout();
 		CAnimationInPaneImpl<CMPSBarWithAll>::UpdatePanesLayout();
		CProgressBarInPaneImpl<CMPSBarWithAll>::UpdatePanesLayout();
		return ret;
	}
}; // CMPSBarWithAll

#endif // !defined(AFX_MULTIPANESTATUSBARWITHPROGRESS_H__D2F37B4C_6E3D_450D_94B5_B14D377226FA__INCLUDED_)
