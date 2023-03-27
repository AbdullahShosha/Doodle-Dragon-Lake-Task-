

#include "Framework.h"
#include <iostream>
#include <random>


static int BackGroundMover = 0;
/* Test Framework realization */

int RandomGen(int l,int h)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(l, h);
	return dis(gen);
}
struct Drawable 
{
	const char* Path;
	Sprite* sprite;
	int Width, Height, PosX, PosY;
	Drawable(const char* path,int width,int height,int posx,int posy)
		:Path(path),Width(width),Height(height),PosX(posx),PosY(posy){}
};
class MyFramework : public Framework {

public:
	Drawable* Background;
	Drawable* DoodleLeft;
	Drawable* DoodleRight;
	Drawable* DoodleIdle;
	Drawable* Platform;
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		Background  = new Drawable("C:\\Users\\abdoh\\source\\repos\\Doodle\\data\\jungle-bck@2x.png",320,512,0,0);
		DoodleLeft  = new Drawable("C:\\Users\\abdoh\\source\\repos\\Doodle\\data\\space-left.png", 62, 60, 0, 0);
		DoodleRight = new Drawable("C:\\Users\\abdoh\\source\\repos\\Doodle\\data\\space-right.png", 62, 60, 0, 0);
		DoodleIdle  = new Drawable("C:\\Users\\abdoh\\source\\repos\\Doodle\\data\\space-puca.png", 62, 60, 0, 0);
		Platform    = new Drawable("C:\\Users\\abdoh\\source\\repos\\Doodle\\data\\platform.png", 68, 14, 0, 0);
		std::cout << " wee";
		width = Background->Width;
		height = Background->Height;
		fullscreen = false;
	}

	virtual bool Init() {

		Background ->sprite  = createSprite(Background->Path);
		DoodleLeft ->sprite  = createSprite(DoodleLeft->Path);
		DoodleRight->sprite = createSprite(DoodleRight->Path);
		DoodleIdle ->sprite  = createSprite(DoodleIdle->Path);
		Platform   ->sprite    = createSprite(Platform->Path);

		setSpriteSize(Background->sprite, Background->Width, Background->Height);
		setSpriteSize(DoodleIdle->sprite, DoodleIdle->Width, DoodleIdle->Height);
		setSpriteSize(DoodleLeft->sprite, DoodleLeft->Width, DoodleLeft->Height);
		setSpriteSize(DoodleRight->sprite, DoodleRight->Width, DoodleRight->Height);
		setSpriteSize(Platform->sprite, Platform->Width, Platform->Height);
		return true;
	}

	virtual void Close() {
		destroySprite(Background ->sprite);
		destroySprite(DoodleLeft ->sprite);
		destroySprite(DoodleRight->sprite);
		destroySprite(DoodleIdle ->sprite);
		destroySprite(Platform   ->sprite);
	}

	virtual bool Tick() {
        //drawTestBackground();
		
		drawSprite(Background->sprite, 0, BackGroundMover);
		drawSprite(Background->sprite, 0, -Background->Height+BackGroundMover);
		drawSprite(DoodleIdle->sprite, Background->Width / 2 - DoodleIdle->Width / 2, Background->Height / 2 - DoodleIdle->Height / 2);
		BackGroundMover++;
		if (BackGroundMover == Background->Height)
			BackGroundMover = 0;
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {

	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		if (button == FRMouseButton::LEFT)
		{

		}
	}

	virtual void onKeyPressed(FRKey k) {
		if (k == FRKey::RIGHT)
		{

		}
		else if (k == FRKey::LEFT)
		{

		}
	}

	virtual void onKeyReleased(FRKey k) {
		if (k == FRKey::RIGHT)
		{

		}
		else if (k == FRKey::LEFT)
		{

		}
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
	run(FW);
	
	return 0;
}
