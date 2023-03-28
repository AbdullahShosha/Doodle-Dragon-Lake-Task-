

#include "Framework.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>


static bool IsMovingRight = false;
static bool IsMovingLeft = false;
static bool IsJumping = false;
static int JumpHeight = 200;
const char* BackgroundPath     = "data\\2X\\bck@2x.png";
const char* DoodleLeftPath     = "data\\2X\\bunny-left.png";
const char* DoodleRightPath    = "data\\2X\\bunny-right.png";
const char* DoodleIdlePath     = "data\\2X\\bunny-puca.png";
const char* PlatformPath       = "data\\2X\\Platform.png";

Sprite* DoodleLeft;
Sprite* DoodleRight;
Sprite* DoodleIdle;


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
	unsigned int time = 0;
	unsigned int DeltaTime = 0;
	int CameraMover = 0;
	int PlayerMover = 0;
	float jump = 0;
	Drawable* Background;
	Drawable* Doodle;
	Drawable* Platform;
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		Background  = new Drawable(BackgroundPath, 320, 512, 0, 0);
		Doodle = new Drawable(DoodleIdlePath, 62, 60, Background->Width / 2 - 32, Background->Height / 2 - 30);
		Platform = new Drawable(PlatformPath, 68, 14, 0, 0);
		std::cout << " wee";
		width = Background->Width;
		height = Background->Height;
		fullscreen = false;
	}

	virtual bool Init() {

		Background ->sprite  = createSprite(Background->Path);
		Doodle ->sprite  = createSprite(Doodle->Path);
		Platform   ->sprite    = createSprite(Platform->Path);
		DoodleLeft = createSprite(DoodleLeftPath);
		DoodleRight = createSprite(DoodleRightPath);
		DoodleIdle = createSprite(DoodleIdlePath);

		setSpriteSize(Background->sprite, Background->Width, Background->Height);
		setSpriteSize(Doodle->sprite, Doodle->Width, Doodle->Height);
		setSpriteSize(Platform->sprite, Platform->Width, Platform->Height);
		setSpriteSize(DoodleLeft, 62, 60);
		setSpriteSize(DoodleRight, 62, 60);
		setSpriteSize(DoodleIdle, 62, 60);
		return true;
	}

	virtual void Close() {
		destroySprite(Background ->sprite);
		destroySprite(Doodle ->sprite);
		destroySprite(Platform   ->sprite);
	}

	virtual bool Tick() {
		//seting frame limit to 50   --1/50 * 1000--
		DeltaTime = getTickCount() - time;
		time = getTickCount();
		if (DeltaTime < 20)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(20 - DeltaTime));
		}
		drawTestBackground();
		jump += 0.2;
		Doodle->PosY += jump;
		if (Doodle->PosY > Background->Height-Doodle->Height) jump = -10;
		drawSprite(Background->sprite, 0, CameraMover);
		drawSprite(Background->sprite, 0, -Background->Height + CameraMover);
		drawSprite(Doodle->sprite, Doodle->PosX + PlayerMover, Doodle->PosY++);		drawSprite(Doodle->sprite, -Doodle->PosX + PlayerMover - Doodle->Width, Doodle->PosY );
		drawSprite(Platform->sprite, Background->Width / 2 - Platform->Width / 2, Background->Height - Platform->Height / 2);
		
		//CameraMover++;
		
		if (CameraMover == Background->Height)
			CameraMover = 0;
		
		if (IsMovingLeft)
		{
			if (PlayerMover == 0)
			{
				PlayerMover = Background->Width;
			}
			PlayerMover-=3;
		}
		else if (IsMovingRight)
		{
			if (PlayerMover  == Background->Width)
			{
				PlayerMover = 0;
			}
			PlayerMover+=3;
		}
		
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
			Doodle->sprite = DoodleRight;
			IsMovingRight = true;
		}
		else if (k == FRKey::LEFT)
		{
			Doodle->sprite = DoodleLeft;
			IsMovingLeft = true;
		}
	}

	virtual void onKeyReleased(FRKey k) {
		if (k == FRKey::RIGHT)
		{
			IsMovingRight = false;
		}
		else if (k == FRKey::LEFT)
		{
			IsMovingLeft = false;
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
