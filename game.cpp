

#include "Framework.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

int Window_Width;
int Window_Height;
float Width_Scalar;
float Height_Scalar;
unsigned int Best_Score_Platforms = 0, Best_Score_Distance = 0,
			 Score_Platforms = 0,Score_Distance = 0;
static bool IsMovingRight = false;
static bool IsMovingLeft = false;
const char* BackgroundPath = "data\\2X\\bck@2x.png";
const char* DoodleLeftPath = "data\\2X\\bunny-left.png";
const char* DoodleRightPath= "data\\2X\\bunny-right.png";
const char* DoodleIdlePath = "data\\2X\\bunny-puca.png";
const char* PlatformPath   = "data\\2X\\Platform.png";
const char* EnemyPath	   = "data\\2X\\Monster.png";

Sprite* DoodleLeft;
Sprite* DoodleRight;
Sprite* DoodleIdle;
Sprite* Enemy;

std::random_device rd;
std::mt19937 gen(rd());

/* Test Framework realization */


struct Drawable 
{
	const char* Path;
	Sprite* sprite;
	int Width, Height, PosX, PosY;
	Drawable(const char* path,int width,int height,int posx,int posy)
		:Path(path),Width(width),Height(height),PosX(posx),PosY(posy){}
	Drawable() = default;
};
class MyFramework : public Framework {

public:
	int CloneXpos;
	unsigned int time = 0;
	unsigned int DeltaTime = 0;
	int CameraMover = 0;
	int PlayerMover = 0;
	float jump = 0;
	int Number_Of_Platforms = 30;
	Drawable* Background;
	Drawable* Doodle;
	Drawable* Platform;
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		Width_Scalar = Window_Width / 640.0;
		Height_Scalar = Window_Height / 1024.0;
		Background  = new Drawable(BackgroundPath, Window_Width, Window_Height, 0, 0);
		Doodle = new Drawable(DoodleIdlePath, 62 * Width_Scalar, 60 * Height_Scalar, Background->Width / 2 - (31 * Width_Scalar), Background->Height / 2 - (30 * Height_Scalar));
		CloneXpos = Doodle->PosX;
		Platform = new Drawable[Number_Of_Platforms];
		width = Background->Width;
		height = Background->Height;
		fullscreen = false;
	}

	virtual bool Init() {
		std::uniform_int_distribution<> distr1(Doodle->Height, Background->Height);
		std::uniform_int_distribution<> distr2(0, Background->Width-(68*Width_Scalar));
		std::uniform_int_distribution<> distr3(0,1);
		for (int i = 0; i < Number_Of_Platforms; i++)
		{
			Platform[i].Path = PlatformPath;
			Platform[i].Width = 68 * Width_Scalar;
			Platform[i].Height = 14 * Height_Scalar;
			Platform[i].sprite = createSprite(PlatformPath);
			setSpriteSize(Platform[i].sprite, Platform[i].Width, Platform[i].Height);
			Platform[i].PosX = distr2(gen);
			Platform[i].PosY = distr1(gen);
		}
		Background ->sprite  = createSprite(Background->Path);
		Doodle ->sprite  = createSprite(Doodle->Path);
		DoodleLeft = createSprite(DoodleLeftPath);
		DoodleRight = createSprite(DoodleRightPath);
		DoodleIdle = createSprite(DoodleIdlePath);

		setSpriteSize(Background->sprite, Background->Width, Background->Height);
		setSpriteSize(Doodle->sprite, Doodle->Width, Doodle->Height);
		setSpriteSize(DoodleLeft,  Doodle->Width, Doodle->Height);
		setSpriteSize(DoodleRight, Doodle->Width, Doodle->Height);
		setSpriteSize(DoodleIdle , Doodle->Width ,Doodle->Height);
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
		//drawTestBackground();
		jump += 0.2;
		Doodle->PosY += jump;
		if (Doodle->PosY > Background->Height-Doodle->Height) ResetLevel();
		drawSprite(Background->sprite, 0, CameraMover);

		//jumping
		for (int i = 0; i < Number_Of_Platforms; i++)
		{
			if ((Doodle->PosX + Doodle->Width - 15 > Platform[i].PosX) 
				&&(Doodle->PosX + Doodle->Width-15 < Platform[i].PosX + Platform[i].Width)
				&&(Doodle->PosY + Doodle->Height > Platform[i].PosY) 
				&&(Doodle->PosY + Doodle->Height < Platform[i].PosY + Platform[i].Height)
				&&jump > 0)
				jump = -10 * Height_Scalar;
			drawSprite(Platform[i].sprite, Platform[i].PosX, Platform[i].PosY);
		}
		//Moving left and right
		if (IsMovingLeft)
		{
			if (Doodle->PosX + Doodle->Width < 0)
			{
				Doodle->PosX += Background->Width;
			}
			if (Doodle->PosX < 0)
			{
				CloneXpos = Doodle->PosX + Background->Width;
			}
			Doodle->PosX -= 3;
			CloneXpos -= 3;
		}
		else if (IsMovingRight)
		{
			if (Doodle->PosX > Background->Width)
			{
				Doodle->PosX -= Background->Width;
			}
			if (Doodle->PosX + Doodle->Width > Background->Width)
			{
				CloneXpos = Doodle->PosX - Background->Width;
			}
			Doodle->PosX += 3;
			CloneXpos += 3;
		}
		//moving the Platforms as the player goes higher
		if (Doodle->PosY < Background->Height / 2)
		{
			Score_Distance++;
			for (int i = 0; i < Number_Of_Platforms; i++)
			{
				Doodle->PosY = Background->Height / 2;
				Platform[i].PosY = Platform[i].PosY - jump;
				if (Platform[i].PosY > Background->Height)
				{
					Score_Platforms++;
					Platform[i].PosY = 0;
					Platform[i].PosX = rand() % (Background->Width - Platform[i].Width);
				}
			}
			std::cout << Score_Distance << " " << Score_Platforms << " " 
					  << Best_Score_Distance << " " <<Best_Score_Platforms << '\n';
		}
		drawSprite(Doodle->sprite, Doodle->PosX, Doodle->PosY);	
		drawSprite(Doodle->sprite, CloneXpos, Doodle->PosY);
		if (CameraMover == Background->Height)
			CameraMover = 0;
		return false;
	}
	void ResetLevel()
	{
		std::uniform_int_distribution<> distr1(Doodle->Height, Background->Height);
		std::uniform_int_distribution<> distr2(0, Background->Width - (68 * Width_Scalar));
		if (Score_Platforms > Best_Score_Platforms)
			Best_Score_Platforms = Score_Platforms;
		if (Score_Distance > Best_Score_Distance)
			Best_Score_Distance = Score_Distance;
		Score_Platforms = 0, Score_Distance = 0;
		Doodle->PosX = Background->Width / 2 - (31 * Width_Scalar);
		Doodle->PosY = Background->Height / 2 - (30 * Height_Scalar);
		CloneXpos = Doodle->PosX;
		for (int i = 0; i < Number_Of_Platforms; i++)
		{
			Platform[i].PosX = distr2(gen);
			Platform[i].PosY = distr1(gen);
		}
	}
	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {

	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		if (button == FRMouseButton::RIGHT)
		{
			std::uniform_int_distribution<> distr1(Doodle->Height, Background->Height);
			std::uniform_int_distribution<> distr2(0, Background->Width - 68);
			for (int i = 0; i < Number_Of_Platforms; i++)
			{
				Platform[i].PosX = distr2(gen);
				Platform[i].PosY = distr1(gen);
			}
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
	std::cout << "Preferred size 320*512 or 640*1024\n";
	std::cout << "Width :";
	std::cin >> Window_Width;
	std::cout << "Height :";
	std::cin >> Window_Height;
	MyFramework* FW = new MyFramework();
	
	//FW->PreInit(Width, Heigh, IsFullScreen);
	run(FW);
	
	return 0;
}
