#include "Includes.h"
//app
#include "PlaylistExtractor.h"

//-----------------------------------------------------------------
// Function called by Windows
//-----------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	MAINAPP->InitApp(hInstance, showCmd);

	return MAINAPP->Run();
}
