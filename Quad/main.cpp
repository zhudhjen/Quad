#include "GameApp.h"
#include "GameView.h"
#include "TitleView.h"
#include <iostream>

using namespace irr;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	IrrlichtDevice *device = createDevice(irr::video::EDT_OPENGL, core::dimension2d<u32>(800, 600),	16, false, false, false, NULL);

	if (!device)
		return 1;

	std::cout << "Device generated" << std::endl;

	device->setWindowCaption(L"Quad");
	device->setResizable(false);

	TitleView* title_view = new TitleView(device);
	GameView* game_view = new GameView(device);

	GameApp<TitleView, GameView> game(title_view, game_view, device);
	title_view->initialize();

	game.Initialize();
	game.run();
	game.clean();

	return 0;
}
