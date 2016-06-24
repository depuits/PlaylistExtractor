#include "PlaylistExtractor.h"
//other windows
#include "Search.h"
#define SEARCH (Search::GetSingleton())

PlaylistExtractor* PlaylistExtractor::m_pApp = 0;

//methods
PlaylistExtractor::PlaylistExtractor():
CallerPtr(),
m_hAppInst(0),
m_hMainWnd(0),

m_ClientWidth(410),
m_ClientHeight(410),

m_isCopie(false),

m_sBugReport(_T("To report a bug send a mail to\ncolmanjoeri@gmail.com\nor go to\nhttps://sourceforge.net/projects/playlistextract/")),
m_sAbout(_T("Made by Joeri Colman\nVersion: 0.05")),

m_pLBMusicList(0),
m_pTxtDirection(0),
m_pBtnCopie(0),
m_pBtnCopieDir(0),

m_pTBMain(0),
m_pProgBarCopying(0),

m_sHardDrive(_T(""))
{
}
PlaylistExtractor::~PlaylistExtractor()
{
}

PlaylistExtractor* PlaylistExtractor::GetSingleton()
{
	if (m_pApp == 0) m_pApp = new PlaylistExtractor();
	return m_pApp;
}

void PlaylistExtractor::InitApp(HINSTANCE hInstance, int showCmd)
{
	ENGINE->SetFont(TEXT("Verdana"), false, false, false, 14);

	m_hAppInst = hInstance;

	InitMainWindow(showCmd);
}

void PlaylistExtractor::InitMainWindow(int showCmd)
{
	static TCHAR szWindowClass[] = _T("Playlist Extractor");

	//HMENU menu;
	WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = PlaylistExtractor::WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = m_hAppInst;
    wcex.hIcon          = LoadIcon(m_hAppInst, MAKEINTRESOURCE(IDI_ICON));
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
    }

	// createwindow
	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application dows not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	//menu = LoadMenu(hInstance, MAKEINTRESOURCE(ID_MENU));

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, true);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	//menu
	HMENU menu = LoadMenu(m_hAppInst, MAKEINTRESOURCE(ID_MENU));

	m_hMainWnd = CreateWindow(
		szWindowClass,
		szWindowClass,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		HWND_DESKTOP,
		menu,
		m_hAppInst,
		NULL
	);
	if (!m_hMainWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			szWindowClass,
			NULL);
        PostQuitMessage(0);
	}

	//move window to the center of the screen
	//calc wnd pos
	int dH, dW;
	ENGINE->GetDesktopResolution(dW, dH);
	POINT pos;
	pos.x = (dW/2) - width/2;
	pos.y = (dH/2) - height/2;
	//move
	MoveWindow(m_hMainWnd, pos.x, pos.y, width, height, false);

	// Display window
	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(m_hMainWnd,
		showCmd);
	UpdateWindow(m_hMainWnd);
}

int PlaylistExtractor::Run()
{
	MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK PlaylistExtractor::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MAINAPP->SetHwnd(hWnd);
	PAINTSTRUCT ps;
    HDC hdc;

	//set min size for window
	if(message == WM_GETMINMAXINFO)
	{
		MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;

		RECT R = { 0, 0, 410, 410};
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, true);
			int width  = R.right - R.left;
			int height = R.bottom - R.top;

		pMinMax->ptMinTrackSize.x = width;
		pMinMax->ptMinTrackSize.y = height;

		return 0;
	}

	//other messages
    switch (message)
    {
	case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
		MAINAPP->AppPaint(hdc);
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
		MAINAPP->AppEnd();
        PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		MAINAPP->AppStart();
		return 0;
	case WM_COMMAND:
		switch(wParam)
		{	
		case IDM_OPEN:
			MAINAPP->OpenList();
			return 0;
		case IDM_CLOSE:
			MAINAPP->CloseList();
			return 0;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			return 0;

		case IDM_COPY:
			MAINAPP->CopyAbort();
			return 0;
		case IDM_FIND:
			//SEARCH->OpenWnd();
			return 0;

		case IDM_ABOUT:
			MAINAPP->StartAbout();
			return 0;
		case IDM_BUGR:
			MAINAPP->StartBug();
			return 0;
		default:
			return 0;
		}
		break;
	case WM_SIZE:
		// Save the new client area dimensions.
		MAINAPP->SetClientWidth(LOWORD(lParam));
		MAINAPP->SetClientHeight(HIWORD(lParam));

		MAINAPP->OnResize();

		return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
}

// app functions
void PlaylistExtractor::AppStart()
{
	ENGINE->SetDefaultFont(m_hMainWnd);

	// toolbar
	m_pTBMain = new Toolbar(m_hMainWnd);
	m_pTBMain->Show();

	// lists
	m_pLBMusicList = new ListView(m_hMainWnd);
	m_pLBMusicList->Show();
	ENGINE->SetDefaultFont(m_pLBMusicList->GetWindow());

	//textbox
	m_pTxtDirection = new TextBox(_T("C:\\"), m_hMainWnd);
	m_pTxtDirection->SetEnabled(false);
	m_pTxtDirection->Show();
	ENGINE->SetDefaultFont(m_pTxtDirection->GetWindow());

	// buttons
	m_pBtnCopie = new Button(m_hMainWnd, _T("Copy files"));
	m_pBtnCopie->Show();
	m_pBtnCopie->SetActionListener(this);
	ENGINE->SetDefaultFont(m_pBtnCopie->GetWindow());
	
	m_pBtnCopieDir = new Button(m_hMainWnd, _T("Change"));
	m_pBtnCopieDir->Show();
	m_pBtnCopieDir->SetActionListener(this);
	ENGINE->SetDefaultFont(m_pBtnCopieDir->GetWindow());

	// progbar
	m_pProgBarCopying = new ProgBar(m_hMainWnd);
	m_pProgBarCopying->SetStep(1);

	//disable btns
	m_pTBMain->DisableBtnClose();
	
	OnResize();
}
void PlaylistExtractor::AppPaint(HDC hdc)
{
	ENGINE->DrawString(_T("Select the files to copy:"), 10, 45, hdc);
	ENGINE->DrawString(_T("Copy to:"), 10, m_ClientHeight - 55, hdc);

	//tstringstream stream;
	//stream << "Total size: " << ENGINE->ByteToMegaBytes(Totalsize()) << " mB.";

	//ENGINE->DrawString(stream.str().c_str(), 10, m_ClientHeight - 30, hdc);
}
void PlaylistExtractor::AppEnd()
{
	delete m_pLBMusicList;

	delete m_pTxtDirection;

	delete m_pBtnCopie;
	delete m_pBtnCopieDir;

	delete m_pProgBarCopying;
}

void PlaylistExtractor::ReadInList(tstring str)
{
	tstring line, name;

	tifstream myfile (str.c_str());
	if (myfile.is_open())
	{
		getline (myfile, line);
		//m3u, m3u8 and files look a like
		if (line == _T("#EXTM3U") || str.substr(str.find_last_of('.')) == _T(".m3u8") || str.substr(str.find_last_of('.')) == _T(".m3u"))
		{
			do
			{
				if (line.length() <= 0)
					continue;

				TCHAR check = *line.substr(0, 1).c_str();
				if (check == '#')
					continue;
				
				name = line.substr(line.find_last_of('\\') +1);
				name = name.substr(0, name.find_last_of('.'));

				if (line.length() > 0)
				{
					m_SongList.push_front(Song(name, line));

					tstringstream stream;
					stream << ENGINE->ByteToKiloBytes(ENGINE->GetFileSize(/*m_sHardDrive +*/ line)) << " KB";
					m_pLBMusicList->AddItem(name , stream.str());
				}

				getline (myfile, line);
			}
			while (!myfile.eof());
		}
		//wpl
		else if(line.substr(2, 3) == _T("wpl") || str.substr(str.find_last_of('.')) == _T(".wpl"))
		{
			while (line.substr(line.find_first_of('<')+1, 3) != _T("seq") && !myfile.eof())
				getline (myfile, line);

			getline (myfile, line);

			while (line.substr(line.find_first_of('<')+2, 3) != _T("seq") && !myfile.eof())
			{
				name = line.substr(line.find_first_of('"')+1);
				line = name.substr(0, name.find_first_of('"'));
				name = line.substr(line.find_last_of('\\')+1);
				name = name.substr(0, name.find_last_of('.'));
				
				//line == dir
				m_SongList.push_front(Song(name, line));

				tstringstream stream;
				stream << ENGINE->ByteToKiloBytes(ENGINE->GetFileSize(m_sHardDrive + line)) << " KB";
				m_pLBMusicList->AddItem(name, stream.str());
				
				getline (myfile, line);
			}

			m_pTBMain->DisableBtnOpen();			
			EnableMainMenuItems(false, true);
		}
		//pls
		else //if (line == _T("[playlist]") + (TCHAR)13)
		{
			while (!myfile.eof())
			{
				getline (myfile, line);
				name = line.substr(line.find_first_of('=') +1);
				getline (myfile, line);
				line = line.substr(line.find_first_of('=') +1);

				tstringstream stream;
				stream << ENGINE->ByteToKiloBytes(ENGINE->GetFileSize(m_sHardDrive + name)) << " KB";
				m_pLBMusicList->AddItem(line , stream.str());

				m_SongList.push_front(Song(line, name));
				
				getline (myfile, line);
			}
			m_pLBMusicList->RemoveItem(m_pLBMusicList->Count() - 1);
		}
		myfile.close();
	}
	else
		MessageBox(NULL, _T("Failed to read in the playlist.\n"), _T("ERROR"), MB_OK | MB_ICONERROR);

	ENGINE->Repaint(m_hMainWnd);
}

unsigned long PlaylistExtractor::Totalsize()
{
	tstring sFileDir;
	unsigned long size = 0; 
	for (unsigned i = 0; i < m_SongList.size(); ++i)
	{
		sFileDir = m_sHardDrive + m_SongList[i].strDir;
		size += ENGINE->GetFileSize(sFileDir);
	}
	return size;
}

// resize function
void PlaylistExtractor::OnResize()
{
	// toolbar
	m_pTBMain->SetBounds(0, 0, m_ClientWidth, 20);

	// lists
	m_pLBMusicList->SetBounds(10, 65, m_ClientWidth - 20, m_ClientHeight - 130);
	
	//textbox
	m_pTxtDirection->SetBounds(75, m_ClientHeight - 57, m_ClientWidth - 150, 20);
	
	// buttons
	if (m_isCopie)
		m_pBtnCopie->SetBounds( (m_ClientWidth) - 70, m_ClientHeight - 30, 60, 20);
	else
		m_pBtnCopie->SetBounds( (m_ClientWidth/2) - 40, m_ClientHeight - 30, 80, 20);
	
	m_pBtnCopieDir->SetBounds(m_ClientWidth - 70, m_ClientHeight - 57, 60, 20);
	
	// progbar
	m_pProgBarCopying->SetBounds(10, m_ClientHeight - 30, m_ClientWidth - 90, 20);
}

void PlaylistExtractor::OpenList()
{
	tstring tmp = ENGINE->OpenFileWnd(m_hMainWnd);
	m_sHardDrive = tmp.substr(0, tmp.find_first_of('\\'));

	m_pTBMain->EnableBtnClose();
	EnableMainMenuItems(true, true);

	ReadInList(tmp);
}

void PlaylistExtractor::CloseList()
{
	//reset list
	m_pLBMusicList->DeleteAll();
	m_SongList.clear();

	//enable open btns
	m_pTBMain->EnableBtnOpen();
	m_pTBMain->DisableBtnClose();
	
	EnableMainMenuItems(true, false);
}

void PlaylistExtractor::CopyAbort()
{
	if (m_isCopie)
		m_isCopie = false;
	else
	{
		//start thread
		HANDLE hThread;
		hThread = CreateThread(NULL,  // no security attributes 
								0,                // use default stack size 
								(LPTHREAD_START_ROUTINE) CopyThreadProc,
								NULL, // param to thread func 
								CREATE_SUSPENDED, // creation flag 
								NULL);       // 
  			 
		ResumeThread(hThread);
			
		SetThreadPriority(hThread,THREAD_PRIORITY_HIGHEST);	
	}
}

//////////////////////////////////////////////
// CallerPtr //// CallerPtr //// CallerPtr //
//////////////////////////////////////////////
void PlaylistExtractor::CallAction(Callable* caller)
{
	// chang copy dir
	if (caller == m_pBtnCopieDir)
		ChangCopyDir();

	// the actual copying
	if (caller == m_pBtnCopie)
		CopyAbort();
}

////////////////
// CopyThread //
////////////////
DWORD WINAPI PlaylistExtractor::CopyThreadProc(void *param)
{
	return MAINAPP->DoCopy();
}
int PlaylistExtractor::DoCopy()
{
	StartCopy();

	tstring sFileDir;
	tstring sNewFileName;
	tstring sCopieDir = m_pTxtDirection->GetText();
	BOOL bSucces = true;

	for (unsigned i = 0; i < m_SongList.size(); ++i)
	{
		//m_pLBMusicList->SetSelItem(i);

		if (!m_isCopie) 
		{
			EndCopy();
			return 0;
		}

		if( m_pLBMusicList->GetCheckedState(i))
		{
			// look for pls file
			if(m_SongList[i].strDir.substr(0, 1) == _T("."))
				sFileDir = m_SongList[i].strDir;
			else
				sFileDir = /*m_sHardDrive +*/ m_SongList[i].strDir;
			sNewFileName = sCopieDir + sFileDir.substr(sFileDir.find_last_of('\\'));

			bSucces = CopyFile(sFileDir.c_str(), sNewFileName.c_str(), false);
			//DWORD error = GetLastError();
			//ENGINE->DispErr(error);

			if (!bSucces) 
			{
				tstringstream message;
				message << "Failed to copy the file " << sFileDir;
				MessageBox(m_hMainWnd, message.str().c_str(), _T("Copy Failed"), MB_ICONERROR);
			}
			m_pLBMusicList->SetCheckedState(i, false);
		}
		//update progbar
		m_pProgBarCopying->AddStep();
		m_pProgBarCopying->Update();
	}
	EndCopy();

	return 0;
}
void PlaylistExtractor::StartCopy()
{
	m_pBtnCopie->SetText(_T("Abort"));
	m_isCopie = true;

	//replace buttons
	OnResize();

	// disable buttons / list
	//m_pLBMusicList->SetEnabled(false);
	m_pBtnCopieDir->SetEnabled(false);
	EnableMainMenuItems(false, false);

	m_pTBMain->DisableBtnOpen();
	m_pTBMain->DisableBtnClose();
	m_pTBMain->BtnCopyToAbort();

	//show progbar and set range
	m_pProgBarCopying->SetRange(m_SongList.size());
	m_pProgBarCopying->Show();
}
void PlaylistExtractor::EndCopy()
{
	m_pProgBarCopying->Hide();

	// enable buttons / list
	//m_pLBMusicList->SetEnabled(true);
	m_pBtnCopieDir->SetEnabled(true);
	EnableMainMenuItems(true, true);

	
	m_pTBMain->EnableBtnOpen();
	m_pTBMain->EnableBtnClose();
	m_pTBMain->BtnAbortToCopy();

	m_pBtnCopie->SetText(_T("Copy files"));
	m_isCopie = false;

	//replace buttons
	OnResize();
}
void PlaylistExtractor::EnableMainMenuItems(bool open, bool close)
{ 
	HMENU hResMenu = LoadMenu(m_hAppInst, MAKEINTRESOURCE(ID_MENU));//g_hinst->Handle to the module containing the menu resource to be loaded.

	HMENU hMenu = GetSubMenu(hResMenu,0);
	if (open)
		EnableMenuItem(hMenu, IDM_OPEN, MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(hMenu, IDM_OPEN, MF_BYCOMMAND | MF_GRAYED);

	if (close)
		EnableMenuItem(hMenu, IDM_CLOSE, MF_BYCOMMAND | MF_ENABLED);
	else
		EnableMenuItem(hMenu, IDM_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	hMenu = GetSubMenu(hResMenu,1);
	ModifyMenu(hMenu, IDM_COPY, MF_BYCOMMAND | MF_STRING, IDM_COPY, _T("&Abort Copy"));
	//DWORD error = GetLastError();
	//ENGINE->DispErr(error);

	SetMenu (m_hMainWnd, hResMenu);
}


//start other windows
void PlaylistExtractor::StartAbout()
{
	MessageBox(m_hMainWnd, m_sAbout.c_str(), _T("About"), MB_OK | MB_ICONASTERISK);
}
void PlaylistExtractor::StartBug()
{
	MessageBox(m_hMainWnd, m_sBugReport.c_str(), _T("Bugreport"), MB_OK | MB_ICONASTERISK);
}

void PlaylistExtractor::SetHwnd(HWND hwnd)
{
	m_hMainWnd = hwnd;
}
HWND PlaylistExtractor::GetHwnd()
{
	return m_hMainWnd;
}


void PlaylistExtractor::ChangCopyDir()
{	
	TCHAR szFolder[MAX_PATH];
	//szFolder = (LPTSTR)m_pTxtDirection->GetText().c_str();

	ENGINE->GetFolderSelection(m_hMainWnd, szFolder, TEXT("Please select a folder to copy the files to.")) ;

	m_pTxtDirection->SetText(szFolder);
	ENGINE->Repaint(m_hMainWnd);
}

void PlaylistExtractor::SetClientHeight(int height)
{
	m_ClientHeight = height;
}
void PlaylistExtractor::SetClientWidth(int width)
{
	m_ClientWidth = width;
}