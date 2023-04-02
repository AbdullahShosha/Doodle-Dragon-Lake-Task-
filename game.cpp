

#include "Framework.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <windows.h>

unsigned int BulletTimer = 0, ShootingStartTime = 0, BulletSpeed = 6 ,BulletStartPosX, BulletStartPosY;
double   BulletXMovement = 0.0, BulletYMovement = 0.0, DoneDistanceX = 0.0, DoneDistanceY = 0.0,
TargetPlayerDistance;
bool BulletIsMoving = false;

int MouseXpos, MouseYpos;

//Higher number means less enemies
int FindingEnemyProbability = 5 ,OneUseProbability = 5;

int Window_Width;
int Window_Height;
double Width_Scalar;
double Height_Scalar;
unsigned int Best_Score_Platforms = 0, Best_Score_Distance = 0 , Best_Score_Enemies_killed,
			 Score_Platforms = 0,Score_Distance = 0 , Score_Enemies_killed =0;
static bool IsMovingRight = false;
static bool IsMovingLeft = false;

const char* BackgroundPath = "data\\X\\jungle-bck@2x.png";
const char* DoodleLeftPath = "data\\X\\bunny-left.png";
const char* DoodleRightPath= "data\\X\\bunny-right.png";
const char* DoodleIdlePath = "data\\X\\bunny-puca.png";
const char* PlatformPath   = "data\\X\\Platform.png";
const char* Platform2Path   = "data\\X\\Platform2.png";
const char* EnemyPath	   = "data\\X\\Monster.png";
const char* BulletPath     = "data\\X\\bubble@2x.png";
const char* ImmunityShiledPath = "data\\X\\Shield.png";

Sprite* DoodleLeft;
Sprite* DoodleRight;
Sprite* DoodleIdle;
Sprite* MainPlatfom;
Sprite* OneusePlatfom;
Sprite* Enemy;
Sprite* bullet;

std::random_device rd;
std::mt19937 gen(rd());

//Handles for changine the score on console
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD pos = { 0, 4 };

/* Test Framework realization */


struct Drawable 
{
	const char* Path;
	Sprite* sprite;
	int Width, Height, PosX, PosY;
	Drawable(const char* path,int width,int height,int posx,int posy)
		:Path(path),Width(width),Height(height),PosX(posx),PosY(posy){}
	Drawable() = default;

	bool IsCollidingWith(Drawable another, int offset)
	{
		if ((((PosX + Width - offset > another.PosX) && (PosX + Width - offset < another.PosX + another.Width)) ||// left collision
			((	PosX + offset > another.PosX) && (PosX + offset < another.PosX +  another.Width)))// right collision
			&& (PosY + Height > another.PosY)
			&& (PosY + Height < another.PosY + another.Height))
		{
			return true;
		}
		return false;
	}
};
class MyFramework : public Framework {

public:
	static const int Number_Of_Platforms = 30;

	int CloneXpos;
	unsigned int time = 0;
	unsigned int DeltaTime = 0;
	float jump = 0;
	
	Drawable* Background;
	Drawable* Doodle;
	Drawable* Platform;
	Drawable* Enemy;
	Drawable* Bullet;
	Drawable* ImmunityShield;
	bool* EnemyExist;
	bool* PlatformExists;
	bool* Oneuse;
	bool IsPlayerImmune = true;


	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		Width_Scalar = Window_Width / 640.0;
		Height_Scalar = Window_Height / 1024.0;
		Background  = new Drawable(BackgroundPath, Window_Width, Window_Height, 0, 0);
		Doodle = new Drawable(DoodleIdlePath, 62 * Width_Scalar, 60 * Height_Scalar, Background->Width / 2 - (31 * Width_Scalar), Background->Height / 2 - (30 * Height_Scalar));
		ImmunityShield = new Drawable(ImmunityShiledPath, 30, 30, 0, 0);
		CloneXpos = Doodle->PosX;
		Platform = new Drawable[Number_Of_Platforms];
		Enemy	 = new Drawable[Number_Of_Platforms];
		EnemyExist = new bool[Number_Of_Platforms];
		PlatformExists = new bool[Number_Of_Platforms];
		Oneuse = new bool[Number_Of_Platforms];
		width = Window_Width;
		height = Window_Height;
		fullscreen = false;
	}

	virtual bool Init() {
		std::uniform_int_distribution<> PlatformRndmYpos(Doodle->Height, Background->Height);
		std::uniform_int_distribution<> PlatformRndmXpos(0, Background->Width-(68*Width_Scalar));
		//1 enemy for every FindingEnemyProbability
		for (int i = 0; i < Number_Of_Platforms; i++)
		{
			Platform[i].Path = PlatformPath;
			Enemy[i].Path = EnemyPath;
			Platform[i].Width = 68 * Width_Scalar;
			Platform[i].Height = 14 * Height_Scalar;
			Enemy[i].Width = 41 * Width_Scalar;
			Enemy[i].Height = 26 * Height_Scalar;
			Platform[i].sprite = createSprite(PlatformPath);
			Enemy[i].sprite = createSprite(EnemyPath);
			setSpriteSize(Platform[i].sprite, Platform[i].Width, Platform[i].Height);
			setSpriteSize(Enemy[i].sprite, Enemy[i].Width, Enemy[i].Height);
			Platform[i].PosX = PlatformRndmXpos(gen);
			std::uniform_int_distribution<> EnemyRndmXpos(Platform[i].PosX, Platform[i].PosX+Platform[i].Width - Enemy[i].Width);
			Enemy[i].PosX = EnemyRndmXpos(gen);
			Platform[i].PosY = PlatformRndmYpos(gen);
			Enemy[i].PosY = Platform[i].PosY - Enemy[i].Height;
			PlatformExists[i] = true;
			if ((rand()% FindingEnemyProbability) == 0)
				EnemyExist[i] = true;
			else EnemyExist[i] = false;
			if ((rand() % OneUseProbability) == 0)
			{
				Oneuse[i] = true;
				Platform[i].sprite = createSprite(Platform2Path);
				setSpriteSize(Platform[i].sprite, Platform[i].Width, Platform[i].Height);
			}
			else Oneuse[i] = false;
		}
		Background ->sprite  = createSprite(Background->Path);
		Doodle ->sprite  = createSprite(Doodle->Path);
		DoodleLeft = createSprite(DoodleLeftPath);
		DoodleRight = createSprite(DoodleRightPath);
		DoodleIdle = createSprite(DoodleIdlePath);
		OneusePlatfom = createSprite(Platform2Path);
		MainPlatfom = createSprite(PlatformPath);
		ImmunityShield->sprite = createSprite(ImmunityShiledPath);

		setSpriteSize(Background->sprite, Background->Width, Background->Height);
		setSpriteSize(Doodle->sprite, Doodle->Width, Doodle->Height);
		setSpriteSize(DoodleLeft,  Doodle->Width, Doodle->Height);
		setSpriteSize(DoodleRight, Doodle->Width, Doodle->Height);
		setSpriteSize(DoodleIdle , Doodle->Width ,Doodle->Height);
		setSpriteSize(OneusePlatfom, Platform[0].Width, Platform[0].Height);
		setSpriteSize(MainPlatfom, Platform[0].Width, Platform[0].Height);
		setSpriteSize(ImmunityShield->sprite, 30, 30);

		Bullet = new Drawable;
		Bullet->Path = BulletPath;
		Bullet->sprite = createSprite(BulletPath);
		setSpriteSize(Bullet->sprite, 8, 8);
		Bullet->Width = 8;Bullet->Height = 8;
		Bullet->PosX = Doodle->PosX + (Doodle->Width / 2) - Bullet->Width/2;
		Bullet->PosY = Doodle->PosY - Bullet->Height;
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
		drawSprite(Background->sprite, 0, 0);
		//Player Movement
		jump += 0.2;
		Doodle->PosY += jump;
		if (Doodle->PosY > Background->Height-Doodle->Height) ResetLevel();

		//jumping
		for (int i = 0; i < Number_Of_Platforms; i++)
		{
			if (Doodle->IsCollidingWith(Platform[i],15) && jump > 0)
			{
				if (PlatformExists[i] && Oneuse[i])
				{
					PlatformExists[i] = false;
					EnemyExist[i] = false;
				}
				jump = -10 * Height_Scalar;
			}
			//killing Monsters
			// the 15 here is offset to ignore the ears and doodle gun
			if    (Doodle->IsCollidingWith(Enemy[i], 15) && EnemyExist[i])
			{
				if ((jump > 0) || IsPlayerImmune)
				{
					EnemyExist[i] = false;
					Score_Enemies_killed++;
				}
				else ResetLevel();
			}
			if (Oneuse[i])Platform[i].sprite = OneusePlatfom;
			else Platform[i].sprite = MainPlatfom;
			if(PlatformExists[i])
			drawSprite(Platform[i].sprite, Platform[i].PosX, Platform[i].PosY);
			if(EnemyExist[i])
			drawSprite(Enemy[i].sprite, Enemy[i].PosX, Platform[i].PosY - Enemy[i].Height);
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
				Platform[i].PosY -= jump;
				Enemy[i].PosY -= jump;
				if (Platform[i].PosY > Background->Height)
				{
					Score_Platforms++;
					Platform[i].PosY = 0;
					Platform[i].PosX = rand() % (Background->Width - Platform[i].Width);
					std::uniform_int_distribution<> EnemyRndmXpos(Platform[i].PosX, Platform[i].PosX + Platform[i].Width - Enemy[i].Width);
					Enemy[i].PosX = EnemyRndmXpos(gen);
					Enemy[i].PosY = Platform[i].PosY - Enemy[i].Height;
					PlatformExists[i] = true;
					if ((rand() % FindingEnemyProbability) == 0)
						EnemyExist[i] = true;
					else EnemyExist[i] = false;
					if ((rand() % OneUseProbability) == 0)
						Oneuse[i] = true;
					else Oneuse[i] = false;
				}
			}
			SetConsoleCursorPosition(hConsole, pos);
			std::cout << "Distance\t Platforms\t Enemies\t Max Distance\t Max Platforms\t Max Enemies\n";
			std::cout << Score_Distance << "\t\t" << Score_Platforms << "\t\t" << Score_Enemies_killed << "\t\t"
					  << Best_Score_Distance << "\t\t" << Best_Score_Platforms << "\t\t" << Best_Score_Enemies_killed << '\n';
		}
		drawSprite(Doodle->sprite, Doodle->PosX, Doodle->PosY);	
		drawSprite(Doodle->sprite, CloneXpos, Doodle->PosY);
		if (IsPlayerImmune)
			drawSprite(ImmunityShield->sprite, Doodle->PosX +Doodle->Width/2 - ImmunityShield->Width/2, Doodle->PosY + Doodle->Height / 2 - ImmunityShield->Height/2 );
		if (BulletIsMoving)
		{
			//std::cout << "BulletXMovement " << BulletXMovement << "\nBulletYMovement" << BulletYMovement;
			DoneDistanceX += BulletXMovement;
			DoneDistanceY += BulletYMovement;
			Bullet->PosX = BulletStartPosX + round(DoneDistanceX);
			if (Bullet->PosX >= Background->Width)Bullet->PosX -= Background->Width;
			if (Bullet->PosX <= 0)Bullet->PosX += Background->Width;
			Bullet->PosY = BulletStartPosY + round(DoneDistanceY);
			//std::cout << "BulletStartPosX " << BulletStartPosX << "\nBulletStartPosX" << BulletStartPosX;
			drawSprite(Bullet->sprite, Bullet->PosX , Bullet->PosY);

			if ((getTickCount() - ShootingStartTime) >= 3000 || Bullet->PosY < 0 || Bullet->PosY > Background->Height)
			{
				BulletIsMoving = false;
			}
			for (int i = 0; i < Number_Of_Platforms; i++)
			{
				if (Bullet->IsCollidingWith(Enemy[i], 0) && EnemyExist[i])
				{
					EnemyExist[i] = false;
					Score_Enemies_killed++;
					BulletIsMoving = false;
				}
			}
		}
		else
		{
			BulletStartPosX = Doodle->PosX + (Doodle->Width / 2) - Bullet->Width / 2;
			BulletStartPosY = Doodle->PosY - Bullet->Height;
			Bullet->PosX = Doodle->PosX + (Doodle->Width / 2) - Bullet->Width / 2;
			Bullet->PosY = Doodle->PosY - Bullet->Height;
		}
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
		if (Score_Enemies_killed > Best_Score_Enemies_killed)
			Best_Score_Enemies_killed = Score_Enemies_killed;
		Score_Platforms = 0, Score_Distance = 0; Score_Enemies_killed = 0;
		Doodle->PosX = Background->Width / 2 - (31 * Width_Scalar);
		Doodle->PosY = Background->Height / 2 - (30 * Height_Scalar);
		CloneXpos = Doodle->PosX;
		for (int i = 0; i < Number_Of_Platforms; i++)
		{
			Platform[i].PosX = distr2(gen);
			Platform[i].PosY = distr1(gen);
			std::uniform_int_distribution<> EnemyRndmXpos(Platform[i].PosX, Platform[i].PosX + Platform[i].Width - Enemy[i].Width);
			Enemy[i].PosX = EnemyRndmXpos(gen);
			Enemy[i].PosY = Platform[i].PosY - Enemy[i].Height;
			PlatformExists[i] = true;
			if ((rand() % FindingEnemyProbability) == 0)
				EnemyExist[i] = true;
			else EnemyExist[i] = false;
			if ((rand() % OneUseProbability) == 0)
				Oneuse[i] = true;
			else Oneuse[i] = false;
		}
	}
	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		MouseXpos = x;
		MouseYpos = y;
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
				std::uniform_int_distribution<> EnemyRndmXpos(Platform[i].PosX, Platform[i].PosX + Platform[i].Width - Enemy[i].Width);
				Enemy[i].PosX = EnemyRndmXpos(gen);
				Enemy[i].PosY = Platform[i].PosY - Enemy[i].Height;
				PlatformExists[i] = true;
				if ((rand() % FindingEnemyProbability) == 0)
					EnemyExist[i] = true;
				else EnemyExist[i] = false;
				if ((rand() % OneUseProbability) == 0)
					Oneuse[i] = true;
				else Oneuse[i] = false;
			}
		}
		if (button == FRMouseButton::LEFT)
		{
			if (!BulletIsMoving)
			{
				DoneDistanceX = 0.0;
				DoneDistanceY = 0.0;
				ShootingStartTime = getTickCount();
				BulletIsMoving = true;
				TargetPlayerDistance = sqrt(((MouseXpos - Bullet->PosX) * (MouseXpos - Bullet->PosX)) + ((MouseYpos - Bullet->PosY) * (MouseYpos - Bullet->PosY)));
				//Normalization
				BulletXMovement =  (MouseXpos - Bullet->PosX) / TargetPlayerDistance;
				BulletYMovement =  (MouseYpos - Bullet->PosY) / TargetPlayerDistance;
				BulletXMovement *= BulletSpeed;
				BulletYMovement *= BulletSpeed;
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
