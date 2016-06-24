#include "Search.h"

Search* Search::m_pSearch = 0;

Search::Search()
:m_hWnd(0)
,m_ClientWidth(300)
,m_ClientHeight(200)
{
	InitializeWindow();

	// Set de new WNDPROC for the button, and store the old one
	m_procOld = (WNDPROC) SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG) ProcStatic);

	// Store 'this' as data for the Button object so that the static PROC can call the member proc
	SetWindowLong(m_hWnd, GWL_USERDATA, (LONG) this);
}
Search::~Search()
{
	DestroyWindow(m_hWnd);
}

void Search::InitializeWindow()
{
	if (m_hWnd != 0) return;

	static TCHAR szWindowClass[] = _T("PE Search");

	//HMENU menu;
	/*WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = Search::ProcStatic;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = NULL;
    wcex.hIcon          = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));

	// register the app
	if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            szWindowClass,
            NULL);
        PostQuitMessage(0);
    }*/

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, true);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	m_hWnd = CreateWindow(
		_T("Playlist Extractor"),
		_T("Search"),
		WS_OVERLAPPEDWINDOW,// | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		NULL,
		NULL
	);

	if (!m_hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Search window"),
			NULL);
		return;
	}
}

Search* Search::GetSingleton()
{
	if (m_pSearch == 0) m_pSearch = new Search();
	return m_pSearch;
}

LRESULT Search::ProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#pragma warning(disable: 4312)
	return reinterpret_cast<Search*>(GetWindowLong(hWnd, GWL_USERDATA))->Proc(hWnd, msg, wParam, lParam);
	#pragma warning(default: 4312)
}

LRESULT Search::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
    HDC hdc;

	switch (msg)
	{
	case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
		//MAINAPP->AppPaint(hdc);
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
		//MAINAPP->AppEnd();
        PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		//MAINAPP->AppStart();
		return 0;
	
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Search::OpenWnd()
{
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
}
void Search::CloseWnd()
{
	//look at
	ShowWindow(m_hWnd, 0);
	UpdateWindow(m_hWnd);
}

void Search::CallAction(Callable* caller)
{
}
