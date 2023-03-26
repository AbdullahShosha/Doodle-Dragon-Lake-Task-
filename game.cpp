

#include "Framework.h"
#include <iostream>
static int BackGroundMover = 0;
/* Test Framework realization */
class MyFramework : public Framework {

public:
	int Width, Heigh;
	bool IsFullScreen = false;
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = 320;
		height = 512;
		fullscreen = false;
	}

	virtual bool Init() {

		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
        //drawTestBackground();
		const char* BackgroundPath = "C:\\Users\\abdoh\\source\\repos\\Doodle\\data\\jungle-bck@2x.png";
		Sprite* Background = createSprite(BackgroundPath);
		getScreenSize(Width, Heigh);

		setSpriteSize(Background, Width, Heigh);
		drawSprite(Background, 0, BackGroundMover);
		drawSprite(Background, 0, -Heigh+BackGroundMover);
		BackGroundMover++;
		if (BackGroundMover == Heigh)
			BackGroundMover = 0;
		destroySprite(Background);
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {

	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {

	}

	virtual void onKeyPressed(FRKey k) {
	}

	virtual void onKeyReleased(FRKey k) {
	}
	
	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}
};

int main(int argc, char *argv[])
{
	MyFramework* FW = new MyFramework();
	
	//FW->PreInit(Width, Heigh, IsFullScreen);
	FW->Init();
	run(FW);
	
	return 0;
}
