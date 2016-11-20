// 1DAE06 - Bossuyt Niels , Jihad Bouhaiji

#pragma region generalDirectives
// SDL libs
#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "SDL2main.lib")

// OpenGL libs
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"Glu32.lib")

// SDL extension libs 
#pragma comment(lib, "SDL2_image.lib") // Library to load image files
#pragma comment(lib, "SDL2_ttf.lib") // Library to use fonts

// SDL and OpenGL Includes
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>

#include <SDL_image.h> // png loading
#include <SDL_ttf.h> // Font
#pragma endregion generalDirectives


#include <iostream>
#include <string>
#include <cmath>
#include "structs.h"
#include "utils.h"

#pragma region windowInformation
const float g_WindowWidth{ 1000.0f };
const float g_WindowHeight{ 800.0f };
const std::string g_WindowTitle{ "TheImpossibleGameDontEvenTryUnlessYouWantToGetFucked - Bossuyt, Niels ; Jihad, Bouhaiji- 1DAE06" };
bool g_IsVSyncOn{ true };
#pragma endregion windowInformation

#pragma region textureDeclarations
struct Texture
{
	GLuint id;
	float width;
	float height;
};

bool TextureFromFile(const std::string& path, Texture & texture);
bool TextureFromString(const std::string & text, TTF_Font *pFont, const Color4f & textColor, Texture & texture);
bool TextureFromString(const std::string & text, const std::string& fontPath, int ptSize, const Color4f & textColor, Texture & texture);
void TextureFromSurface(const SDL_Surface *pSurface, Texture & textureData);
void DrawTexture(const Texture & texture, const Point2f& bottomLeftVertex, const Rectf & textureRect = {});
void DrawTexture(const Texture & texture, const Rectf & vertexRect, const Rectf & textureRect = {});
void DeleteTexture(Texture & texture);
#pragma endregion textureDeclarations

#pragma region coreDeclarations
// Functions
void Initialize();
void Run();
void Cleanup();
void QuitOnSDLError();
void QuitOnOpenGlError();
void QuitOnImageError();
void QuitOnTtfError();

// Variables
SDL_Window* g_pWindow{ nullptr }; // The window we'll be rendering to
SDL_GLContext g_pContext; // OpenGL context
Uint32 g_MilliSeconds{};
const Uint32 g_MaxElapsedTime{ 100 };
#pragma endregion coreDeclarations;

#pragma region gameDeclarations
//Enum class

enum class ObjState
{
	Destroyed, Running, Destroying
};

enum class Diff
{
	Easy, Medium, Hard, Daddy, DOA
};


// Functions
void InitGameResources();
void InitBricks(Rectf *pArray, ObjState *pState, int columns, int rows);
void FreeGameResources();

void ProcessKeyDownEvent(const SDL_KeyboardEvent  & e);
void ProcessKeyUpEvent(const SDL_KeyboardEvent  & e);
void ProcessMouseMotionEvent(const SDL_MouseMotionEvent & e);
void ProcessMouseDownEvent(const SDL_MouseButtonEvent & e);
void ProcessMouseUpEvent(const SDL_MouseButtonEvent & e);

void Update(float elapsedSec);
void UpdateBall(float elapsedSec, Rectf *pArray, ObjState *pState);
void KeepBallInScreen();
bool CollisionDetect(Point2f PrevBallPos, Rectf rectangle);
void UpdateBat(float elapsedSec);

void Draw();
void DrawBat();
void DrawBall();
void DrawBricks(Rectf *pArray, ObjState *pState, int rows, int columns);
void DrawLaser();
void DrawBoss();
void ClearBackground();

float CalculateAngle(float Point1X, float Point1Y, float Point2X, float Point2Y);


// Variables

int g_FrameCounter{};
Circlef g_Explosion{};
int g_DeathCounter{};

// init values
const Point2f g_BatDimens{ 100.0f,50.0f };
const Point2f g_BatPos{ g_WindowWidth / 2 - g_BatDimens.x / 2 ,60.0f };
float g_VelBatValue{ 500.0f };
float g_ColorR{ 0.0f };
float g_ColorG{ 1.0f };
float g_ColorB{ 0.0f };
float g_ColorA{ 0.5f };
Diff g_GameDiff{};

const Point2f g_Ballpos{ g_WindowWidth / 2,120.0f };
//brick var 
float g_BrickWidth{ 40.0f };
float g_BrickHeight{ 20.0f };

int g_Rows{};
int g_Columns{ int((g_WindowWidth - g_BrickWidth) / g_BrickWidth) };

Rectf *bricks{};
ObjState *bricksState{};

//Bat var
Rectf g_BatRect{ g_BatPos.x, g_BatPos.y,g_BatDimens.x,g_BatDimens.y };
float g_BatVel{};
bool g_MoveLeft{};
bool g_MoveRight{};

//Ball var
Color4f g_ColorBall{ g_ColorR ,g_ColorG ,g_ColorB ,g_ColorA };
Point2f g_Radius{ 10.0f,10.0f };
Point2f g_Center{ g_Ballpos.x,g_Ballpos.y };
Point2f g_PrevBallPos{};
float g_VelBallYValue{ 300.0f };
float g_VelBallXValue{ 300.0f };
ObjState g_BallState{ObjState::Running};

//Boss Var
float g_BossWidth{256.0f};
float g_BossHeight{213.0f};
Rectf g_BossRect{(g_WindowWidth/2)-(g_BossWidth/2),g_WindowHeight-g_BossHeight, g_BossWidth,g_BossHeight};
ObjState g_BossState{ObjState::Running};

//Texture var
Texture g_BallTex{};
Texture g_BatTex{};
Texture g_BombTex{};
Texture g_BossTex{};
Texture g_BossWithLaserTex{};
Texture g_dangerTex{};
Texture g_deadBatTex{};
Texture g_LaserTex{};
Texture g_LaserBossTex{};
Texture g_LeftCanonTex{};
Texture g_RightCanonTex{};
Texture g_LeftCanonLaserTex{};
Texture g_RightCanonLaserTex{};

//laser var
bool g_IsShooting{ false };
float g_Angle{ 0.0f };

#pragma endregion gameDeclarations




int main(int argc, char* args[])
{
	int choice{};

	std::cout << "Choose difficulty level:  0 = easy , 1  = medium , 2 = hard , 3 = Daddy, 4 = Dead on arrival: ";
	std::cin >> choice;
	
	while (choice < 0 && choice > 4)
	{
		std::cout << "not within range: Try again" << std::endl;
		std::cin >> choice;

	};

	switch (choice)
	{
	case 0: g_Rows = 3;
		g_GameDiff = Diff(choice);
		break;
	case 1: g_Rows = 6;
		g_GameDiff = Diff(choice);
		break;
	case 2: g_Rows = 9;
		g_GameDiff = Diff(choice);
		break;
	case 3: g_Rows = 12;
		g_GameDiff = Diff(choice);
		break;
	case 4: g_Rows = 18;
		g_GameDiff = Diff(choice);
		break;
	}

	bricks = new Rectf[g_Rows * g_Columns]{};
	bricksState = new ObjState[g_Rows * g_Columns]{};

	// Initialize SDL and OpenGL
	Initialize();

	// Event loop
	Run();

	// Clean up SDL and OpenGL
	Cleanup();

	delete[] bricks;
	delete[] bricksState;
	return 0;
}

#pragma region gameImplementations
void InitGameResources()
{
	
	bool result = TextureFromFile("Resources/ball.png", g_BallTex);
	if (!result)
	{
		std::cout << "ball.png failed to load." << std::endl;
	}
	result = TextureFromFile("Resources/bat base.png", g_BatTex);
	if (!result)
	{
		std::cout << "bat base.png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/bat dead.png", g_deadBatTex);
	if (!result)
	{
		std::cout << "bat dead.png failed to load." << std::endl;
	}
	result = TextureFromFile("Resources/danger.png", g_dangerTex);
	if (!result)
	{
		std::cout << "danger.png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/bomb.png", g_BombTex);
	if (!result)
	{
		std::cout << "bomb.png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/boss.png", g_BossTex);
	if (!result)
	{
		std::cout << "boss.png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/boss laser .png", g_BossWithLaserTex);
	if (!result)
	{
		std::cout << "boss laser .png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/laser piece.png", g_LaserTex);
	if (!result)
	{
		std::cout << "laser piece.png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/laser piece boss.png", g_LaserBossTex);
	if (!result)
	{
		std::cout << "laser piece boss.png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/left canon.png", g_LeftCanonTex);
	if (!result)
	{
		std::cout << "left canon.png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/left canon laser.png", g_LeftCanonLaserTex);
	if (!result)
	{
		std::cout << "left canon laser.png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/right canon.png", g_RightCanonTex);
	if (!result)
	{
		std::cout << "right canon.png failed to load." << std::endl;
	}

	result = TextureFromFile("Resources/right canon laser .png", g_RightCanonLaserTex);
	if (!result)
	{
		std::cout << "right canon laser.png failed to load." << std::endl;
	}


	InitBricks(bricks, bricksState, g_Columns, g_Rows);
}

void InitBricks(Rectf * pArray, ObjState *pState, int columns, int rows)
{
	int counter{};

	for (int i = 0; i < rows; i++)
	{
		counter++;
		for (int j = 0; j < columns; j++)
		{
			
			pArray[dae::GetArrayIndex(i, j, g_Columns)].width = g_BrickWidth;
			pArray[dae::GetArrayIndex(i, j, g_Columns)].height = g_BrickHeight;
			pArray[dae::GetArrayIndex(i, j, g_Columns)].left = (g_BrickWidth/counter) + (j*g_BrickWidth);
			pArray[dae::GetArrayIndex(i, j, g_Columns)].bottom = (g_WindowHeight - 200.0f) - (i*g_BrickHeight);
			pState[dae::GetArrayIndex(i, j, g_Columns)] = ObjState::Running;
			
		}
		if (counter == 2)
		{
			counter = 0;
	}
}
}
void FreeGameResources( )
{
	DeleteTexture(g_BallTex);
	DeleteTexture(g_BatTex);
	DeleteTexture(g_BombTex);
	DeleteTexture(g_BossTex);
	DeleteTexture(g_BossWithLaserTex);
	DeleteTexture(g_dangerTex);
	DeleteTexture(g_deadBatTex);
	DeleteTexture(g_LaserTex);
	DeleteTexture(g_LaserBossTex);
	DeleteTexture(g_LeftCanonTex);
	DeleteTexture(g_LeftCanonLaserTex);
	DeleteTexture(g_RightCanonTex);
	DeleteTexture(g_RightCanonLaserTex);

	
}
void ProcessKeyDownEvent(const SDL_KeyboardEvent  & e)
{
	switch (e.keysym.sym)
	{
	case SDLK_LEFT:
		g_MoveLeft = true;
		break;
	case SDLK_RIGHT:
		g_MoveRight = true;
		break;
	case SDLK_l:
		g_IsShooting = true;
		break;
	case SDLK_k:
		g_IsShooting =false;
		break;
	
	}
}
void ProcessKeyUpEvent(const SDL_KeyboardEvent  & e)
{
	switch (e.keysym.sym)
	{
	case SDLK_LEFT:
		g_MoveLeft = false;
		break;
	case SDLK_RIGHT:
		g_MoveRight = false;
		break;
	case SDLK_1:
		g_Angle += 30.0f;
		break;
	case SDLK_2:
		g_Angle += 60.0f;
		break;
	case SDLK_3:
		g_Angle += 90.0f;
		break;


	}
}
void ProcessMouseMotionEvent(const SDL_MouseMotionEvent & e)
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
}
void ProcessMouseDownEvent(const SDL_MouseButtonEvent & e)
{

}
void ProcessMouseUpEvent(const SDL_MouseButtonEvent & e)
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
	{
		//std::cout << "Left mouse button released\n";
		//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
		break;
	}
	case SDL_BUTTON_RIGHT:
		//std::cout << "Right mouse button released\n";
		break;
	case SDL_BUTTON_MIDDLE:
		//std::cout << "Middle mouse button released\n";
		break;
	}
}


void ClearBackground()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
void Update(float elapsedSec)
{
	UpdateBat(elapsedSec);
	UpdateBall(elapsedSec, bricks, bricksState);
}

void Draw()
{
	ClearBackground();
	DrawBat();
	DrawBall();
	DrawBricks(bricks, bricksState, g_Rows, g_Columns);
	DrawLaser();
	DrawBoss();
}
void DrawBat()
{
	//dae::DrawRect(g_BatRect);
	DrawTexture(g_BatTex, g_BatRect);
}
void UpdateBat(float elapsedSec)
{
	//change bat velocity
	g_BatRect.left += elapsedSec* g_BatVel;

	if (g_MoveLeft)
	{
		g_BatVel = -g_VelBatValue;
	}
	else if (g_MoveRight)
	{
		g_BatVel = g_VelBatValue;
	}
	else if (!g_MoveLeft && !g_MoveRight)
	{
		g_BatVel = 0;
	}

	//keep bat in screen
	if (g_BatRect.left + g_BatRect.width> g_WindowWidth)
	{
		g_BatRect.left = g_WindowWidth - g_BatRect.width;
	}
	if (g_BatRect.left < 0.0f)
	{
		g_BatRect.left = 0.0f;
	}
	
}

void DrawBall()
{
	//dae::DrawEllipse(g_ColorBall, g_Center, g_Radius);
	float scale{ g_BallTex.width / (2 * g_Radius.x) };
	Rectf ballRect{};
	ballRect.left = g_Center.x - g_Radius.x;
	ballRect.bottom = g_Center.y - g_Radius.y;
	ballRect.width = g_BallTex.width / scale;
	ballRect.height = g_BallTex.height / scale;;
	DrawTexture(g_BallTex, ballRect);

}

void DrawBricks(Rectf *pArray, ObjState *pState, int rows, int columns)
{
	Color4f red{ 1.0f,0.0f,0.0f,1.0f };
	Color4f white{ 1.0f,1.0f,1.0f,1.0f };
	Color4f green{ 0.0f,1.0f,0.0f,1.0f };
	Color4f outline{ 0.0f,0.0f,0.0f,1.0f };

	int columnsPerColor{ g_Columns / 3 };

	int ColorCounter{};
	int ColumnsCounter{};

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			switch (ColorCounter)
				{
				case 0:
					if (pState[dae::GetArrayIndex(i, j, columns)] == ObjState::Running)
					{
						dae::DrawFillRect(pArray[dae::GetArrayIndex(i, j, columns)], green);
						dae::DrawRect(pArray[dae::GetArrayIndex(i, j, columns)], outline);
					}
					else if (pState[dae::GetArrayIndex(i, j, columns)] == ObjState::Destroying)
					{
						pArray[dae::GetArrayIndex(i, j, columns)].bottom--;
						dae::DrawFillRect(pArray[dae::GetArrayIndex(i, j, columns)], green);
						dae::DrawRect(pArray[dae::GetArrayIndex(i, j, columns)], outline);
						if (pArray[dae::GetArrayIndex(i, j, columns)].bottom < 0.0f - pArray[dae::GetArrayIndex(i, j, columns)].height)
						{
							pState[dae::GetArrayIndex(i, j, columns)] = ObjState::Destroyed;
						}
					}
					break;
				case 1:
					if (pState[dae::GetArrayIndex(i, j, columns)] == ObjState::Running)
					{
						dae::DrawFillRect(pArray[dae::GetArrayIndex(i, j, columns)], white);
						dae::DrawRect(pArray[dae::GetArrayIndex(i, j, columns)], outline);
					}
					else if (pState[dae::GetArrayIndex(i, j, columns)] == ObjState::Destroying)
					{
						pArray[dae::GetArrayIndex(i, j, columns)].bottom--;
						dae::DrawFillRect(pArray[dae::GetArrayIndex(i, j, columns)], white);
						dae::DrawRect(pArray[dae::GetArrayIndex(i, j, columns)], outline);
						if (pArray[dae::GetArrayIndex(i, j, columns)].bottom < 0.0f - pArray[dae::GetArrayIndex(i, j, columns)].height)
						{
							pState[dae::GetArrayIndex(i, j, columns)] = ObjState::Destroyed;
						}
					}
					break;
				case 2: 
					if (pState[dae::GetArrayIndex(i, j, columns)] == ObjState::Running)
					{
						dae::DrawFillRect(pArray[dae::GetArrayIndex(i, j, columns)], red);
						dae::DrawRect(pArray[dae::GetArrayIndex(i, j, columns)], outline);
					}
					else if (pState[dae::GetArrayIndex(i, j, columns)] == ObjState::Destroying)
					{
						pArray[dae::GetArrayIndex(i, j, columns)].bottom--;
						dae::DrawFillRect(pArray[dae::GetArrayIndex(i, j, columns)], red);
						dae::DrawRect(pArray[dae::GetArrayIndex(i, j, columns)], outline);
						if (pArray[dae::GetArrayIndex(i, j, columns)].bottom < 0.0f - pArray[dae::GetArrayIndex(i, j, columns)].height)
						{
							pState[dae::GetArrayIndex(i, j, columns)] = ObjState::Destroyed;
						}
					}
				default:
					break;
				}
				
				ColumnsCounter++;
				if (ColumnsCounter >= columnsPerColor)
				{
					ColumnsCounter = 0;
					ColorCounter++;
				}	
		}
		ColorCounter = 0;
	}
}

void UpdateBall(float elapsedSec, Rectf *pArray, ObjState *pState)
{
	if (g_BallState == ObjState::Running && !(g_BossState == ObjState::Destroying || g_BossState == ObjState::Destroyed))
	{
		//old position
		g_PrevBallPos.x = g_Center.x;
		g_PrevBallPos.y = g_Center.y;

		//new position
		g_Center.y += g_VelBallYValue*elapsedSec;
		g_Center.x += g_VelBallXValue*elapsedSec;

		CollisionDetect(g_PrevBallPos, g_BatRect);

		bool hit{ false };

		for (int i = 0; i < g_Rows; i++)
		{
			for (int j = 0; j < g_Columns; j++)
			{
				if (pState[dae::GetArrayIndex(i, j, g_Columns)] == ObjState::Running)
				{
					hit = CollisionDetect(g_PrevBallPos, pArray[dae::GetArrayIndex(i, j, g_Columns)]);

					if (hit)
					{
						pState[dae::GetArrayIndex(i, j, g_Columns)] = ObjState::Destroying;
					}
				}
			}
		}

		hit = false;

		hit = CollisionDetect(g_PrevBallPos, g_BossRect);

		if (hit)
		{
			g_BossState = ObjState::Destroying;
		}
		KeepBallInScreen();

		if (g_Center.y + g_Radius.y < 0.0f)
		{
			g_FrameCounter = 0;
			g_DeathCounter++;
			g_VelBallYValue = -g_VelBallYValue;
			g_BallState = ObjState::Destroyed;
		}
	}
	if (g_BallState == ObjState::Destroyed)
	{
		
		g_FrameCounter++;
		if (g_FrameCounter == 60)
		{
			g_BallState = ObjState::Running;
			g_Center.x = g_BatPos.x+g_BatDimens.x;
			g_Center.y = g_BatPos.y+g_BatDimens.y;

		}
	}
}

	

float CalculateAngle(float Point1X, float Point1Y, float Point2X, float Point2Y)
{
	//caculate angle
	float deltaY{ Point2Y - Point1Y };
	float deltaX{ Point2X - Point1X };
	float angle{ atan(deltaY / deltaX)*float(180 / M_PI) };

	//fix atan values
	if (deltaX < 0 && deltaY <0)
	{
		angle += 180;
	}
	if (deltaX < 0 && deltaY >0)
	{
		angle += 180;
	}
	if (deltaX > 0 && deltaY < 0)
	{
		angle += 360;
	}
	
	return angle;
}

bool CollisionDetect(Point2f PrevBallPos, Rectf rectangle)
{
	bool IsBallXInRect
	{ 
		dae::IsXBetween(rectangle.left + rectangle.width, rectangle.left, g_Center.x + g_Radius.x)
		|| dae::IsXBetween(rectangle.left + rectangle.width, rectangle.left, g_Center.x - g_Radius.x) 
	};
	bool IsBallYInRect
	{ 
		dae::IsYBetween(rectangle.bottom + rectangle.height, rectangle.bottom, g_Center.y + g_Radius.y)
		|| dae::IsYBetween(rectangle.bottom + rectangle.height, rectangle.bottom, g_Center.y - g_Radius.y) 
	};
	if (IsBallXInRect && IsBallYInRect)// if the ball hits the rect
	{
		//if it hits on the left or right
		if (PrevBallPos.y < rectangle.bottom + rectangle.height && PrevBallPos.y > rectangle.bottom)
		{
			g_VelBallXValue = -g_VelBallXValue;
			//if it hits on the right
			if (PrevBallPos.x > rectangle.left && PrevBallPos.x > rectangle.left + rectangle.width)
			{
				g_Center.x = rectangle.left + rectangle.width + g_Radius.x;
			}
			//if it hits on the right
			else
			{
				g_Center.x = rectangle.left - g_Radius.x;
			}
		}
		//if it hits on the top or bottom
		else if (PrevBallPos.x > rectangle.left  && PrevBallPos.x < rectangle.left + rectangle.width)
		{
			g_VelBallYValue = -g_VelBallYValue;
			//if it hits on the top
			if (PrevBallPos.y > rectangle.bottom && PrevBallPos.y > rectangle.bottom + rectangle.height)
			{
				g_Center.y = rectangle.bottom + rectangle.height + g_Radius.y;
			}
			//if it hits on the bottom
			else
			{
				g_Center.y = rectangle.bottom - g_Radius.y;
			}
		}
		else if ((PrevBallPos.y < rectangle.bottom + rectangle.height && PrevBallPos.y > rectangle.bottom) || (PrevBallPos.x > rectangle.left  && PrevBallPos.x < rectangle.left + rectangle.width))
		{
			g_VelBallYValue = -g_VelBallYValue;
			g_VelBallXValue = -g_VelBallXValue;
		}
		

		return true;
	}

	return false;

}

void KeepBallInScreen()
{
	if (g_Center.y + g_Radius.y > g_WindowHeight)
	{
		g_VelBallYValue = -g_VelBallYValue;
		g_Center.y = g_WindowHeight - g_Radius.y;
	}
	if (g_Center.x + g_Radius.x > g_WindowWidth)
	{
		g_VelBallXValue = -g_VelBallXValue;
		g_Center.x = g_WindowWidth - g_Radius.x;
	}
	if (g_Center.x - g_Radius.x < 0.0f)
	{
		g_VelBallXValue = -g_VelBallXValue;
		g_Center.x = g_Radius.x;
	}
}

void DrawLaser()
{
	float scale{ 0.75f };
	float leftLaserX{ 0.0f };
	float leftLaserY{ g_WindowHeight / 3 };
	float scaleLaserPiece{ 0.5f };
	float xCorrection{ 100.0f };
	int numberPieces{ 30 };

	Rectf leftCanonPos{ leftLaserX,leftLaserY,g_LeftCanonTex.width*scale,g_LeftCanonTex.height*scale };
	DrawTexture(g_LeftCanonTex, leftCanonPos);

	Rectf leftLaserPos{ g_WindowWidth/2- g_LeftCanonLaserTex.width/2, g_WindowHeight/2- g_LeftCanonLaserTex.height/2 ,g_LeftCanonLaserTex.width,g_LeftCanonLaserTex.height };
	if (g_IsShooting)
	{

		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, g_LeftCanonLaserTex.id);


		glPushMatrix();

		glTranslatef(+0.0f, +0.0f, 0.0f);
		glRotatef(g_Angle, 0.0f, 0.0f, 1.0f);
		glTranslatef(-0.0f, -0.0f, 0.0f);
		
		glBegin(GL_QUADS);
		
		glTexCoord2i(0, 1);
		glVertex2f(leftLaserPos.left, leftLaserPos.bottom);

		glTexCoord2i(1, 1);
		glVertex2f(leftLaserPos.left + leftLaserPos.width, leftLaserPos.bottom);

		glTexCoord2i(1, 0);
		glVertex2f(leftLaserPos.left + leftLaserPos.width, leftLaserPos.bottom + leftLaserPos.height);

		glTexCoord2i(0, 0);
		glVertex2f(leftLaserPos.left, leftLaserPos.bottom + leftLaserPos.height);
	
		glEnd();

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);	

	}
	


}

void DrawBoss()
{
	Color4f color{ 1.0f,1.0f,1.0f,0.5f };
	int nrSides{ 20 };

	switch (g_BossState)
	{
	case ObjState::Running:
		DrawTexture(g_BossTex, g_BossRect);
		break;

	case ObjState::Destroying:
		
		g_Explosion.center.x = g_BossRect.left + (g_BossRect.width / 2);
		g_Explosion.center.y = g_BossRect.bottom + (g_BossRect.height / 2);
		g_Explosion.radius++;
		if (g_Explosion.radius >= ((g_WindowWidth / 2) + 100.f))
		{
		g_BossState = ObjState::Destroyed;
		}
		dae::DrawEllipse(color, g_Explosion.center, g_Explosion.radius,nrSides);
		break;

	default:
		break;
	}
	
}

#pragma endregion gameImplementations

#pragma region coreImplementations
void Initialize()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		QuitOnSDLError();
	}

	//Use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//Create window
	g_pWindow = SDL_CreateWindow(
		g_WindowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		int(g_WindowWidth),
		int(g_WindowHeight),
		SDL_WINDOW_OPENGL);

	if (g_pWindow == nullptr)
	{
		QuitOnSDLError();
	}

	// Create an opengl context and attach it to the window 
	g_pContext = SDL_GL_CreateContext(g_pWindow);
	if (g_pContext == nullptr)
	{
		QuitOnSDLError();
	}

	if (g_IsVSyncOn)
	{
		// Synchronize buffer swap with the monitor's vertical refresh
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			QuitOnSDLError();
		}
	}


	// Initialize Projection matrix
	
	glLoadIdentity();
	// Set the clipping (viewing) area's left, right, bottom and top
	gluOrtho2D(0, g_WindowWidth, 0, g_WindowHeight);

	// The viewport is the rectangular region of the window where the image is drawn.
	// Set it to the entire client area of the created window
	glViewport(0, 0, int(g_WindowWidth), int(g_WindowHeight));

	//Initialize Modelview matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Enable color blending and use alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		QuitOnImageError();
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		QuitOnTtfError();
	}
	
}
void Run()
{
	//Main loop flag
	bool quit{ false };

	// Set start time
	g_MilliSeconds = SDL_GetTicks();

	InitGameResources();

	//The event loop
	SDL_Event e{};
	while (!quit)
	{
		// Poll next event from queue
		while (SDL_PollEvent(&e) != 0)
		{
			// Handle the polled event
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				ProcessKeyDownEvent(e.key);
				break;
			case SDL_KEYUP:
				ProcessKeyUpEvent(e.key);
				break;
			case SDL_MOUSEMOTION:
				ProcessMouseMotionEvent(e.motion);
				break;
			case SDL_MOUSEBUTTONDOWN:
				ProcessMouseDownEvent(e.button);
				break;
			case SDL_MOUSEBUTTONUP:
				ProcessMouseUpEvent(e.button);
				break;
			}
		}

		if (!quit)
		{

			// Calculate elapsed time
			// Get the number of milliseconds since the SDL library initialization
			// Note that this value wraps if the program runs for more than ~49 days.
			Uint32 currentMilliSeconds = SDL_GetTicks();

			// Calculate elapsed time
			Uint32 elapsedTime = currentMilliSeconds - g_MilliSeconds;

			// Update current time
			g_MilliSeconds = currentMilliSeconds;

			// Prevent jumps in time caused by break points
			if (elapsedTime > g_MaxElapsedTime)
			{
				elapsedTime = g_MaxElapsedTime;
			}

			// Call update function, using time in seconds (!)
			Update(elapsedTime / 1000.0f);

			// Draw in the back buffer
			Draw();

			// Update screen: swap back and front buffer
			SDL_GL_SwapWindow(g_pWindow);
		}
	}

	FreeGameResources();
}

void Cleanup()
{
	SDL_GL_DeleteContext(g_pContext);

	SDL_DestroyWindow(g_pWindow);
	g_pWindow = nullptr;

	SDL_Quit();
}


void QuitOnSDLError()
{
	std::cout << "Problem during SDL initialization: ";
	std::cout << SDL_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}
void QuitOnOpenGlError()
{
	std::cout << "Problem during OpenGL initialization: ";
	std::cout << SDL_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}
void QuitOnImageError()
{
	std::cout << "Problem during SDL_image initialization: ";
	std::cout << IMG_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}
void QuitOnTtfError()
{
	std::cout << "Problem during SDL_ttf initialization: ";
	std::cout << TTF_GetError();
	std::cout << std::endl;
	Cleanup();
	exit(-1);
}
#pragma endregion coreImplementations

#pragma region textureImplementations

bool TextureFromFile(const std::string& path, Texture & texture)
{
	//Load file for use as an image in a new surface.
	SDL_Surface* pLoadedSurface = IMG_Load(path.c_str());
	if (pLoadedSurface == nullptr)
	{
		std::cout << "TextureFromFile: SDL Error when calling IMG_Load: " << SDL_GetError() << std::endl;
		return false;
	}

	TextureFromSurface(pLoadedSurface, texture);

	//Get rid of the loaded surface
	SDL_FreeSurface(pLoadedSurface);

	return true;
}

bool TextureFromString(const std::string & text, const std::string& fontPath, int ptSize, const Color4f & textColor, Texture & texture)
{
	// Create font
	TTF_Font *pFont{};
	pFont = TTF_OpenFont(fontPath.c_str(), ptSize);
	if (pFont == nullptr)
	{
		std::cout << "TextureFromString: Failed to load font! SDL_ttf Error: " << TTF_GetError();
		std::cin.get();
		return false;
	}

	// Create texture using this fontand close font afterwards
	bool textureOk = TextureFromString(text, pFont, textColor, texture);
	TTF_CloseFont(pFont);

	return textureOk;
}

bool TextureFromString(const std::string & text, TTF_Font *pFont, const Color4f & color, Texture & texture)
{
	//Render text surface
	SDL_Color textColor{};
	textColor.r = Uint8(color.r * 255);
	textColor.g = Uint8(color.g * 255);
	textColor.b = Uint8(color.b * 255);
	textColor.a = Uint8(color.a * 255);

	SDL_Surface* pLoadedSurface = TTF_RenderText_Blended(pFont, text.c_str(), textColor);
	//SDL_Surface* pLoadedSurface = TTF_RenderText_Solid(pFont, textureText.c_str(), textColor);
	if (pLoadedSurface == nullptr)
	{
		std::cout << "TextureFromString: Unable to render text surface! SDL_ttf Error: " << TTF_GetError();
		return false;
	}

	// copy to video memory
	TextureFromSurface(pLoadedSurface, texture);

	//Get rid of loaded surface
	SDL_FreeSurface(pLoadedSurface);

	return true;
}

void TextureFromSurface(const SDL_Surface *pSurface, Texture & texture)
{
	//Get image dimensions
	texture.width = float(pSurface->w);
	texture.height = float(pSurface->h);

	// Get pixel format information and translate to OpenGl format
	GLenum pixelFormat{ GL_RGB };
	switch (pSurface->format->BytesPerPixel)
	{
	case 1:
		pixelFormat = GL_RED;
		break;
	case 2:
		pixelFormat = GL_RG;
		break;
	case 3:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGB;
		}
		else
		{
			pixelFormat = GL_BGR;
		}
		break;
	case 4:
		if (pSurface->format->Rmask == 0x000000ff)
		{
			pixelFormat = GL_RGBA;
		}
		else
		{
			pixelFormat = GL_BGRA;
		}
		break;
	default:
		std::cout << "IMG error: Unknow pixel format, BytesPerPixel: " << pSurface->format->BytesPerPixel;
	}

	//Generate an array of textures.  We only want one texture (one element array), so trick
	//it by treating "texture" as array of length one.
	glGenTextures(1, &texture.id);

	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	glBindTexture(GL_TEXTURE_2D, texture.id);

	//Specify the texture's data.  This function is a bit tricky, and it's hard to find helpful documentation.  A summary:
	//   GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
	//               0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
	//                         not cached smaller copies)
	//         GL_RGBA:    Specifies the number of color components in the texture.
	//                     This is how OpenGL will store the texture internally (kinda)--
	//                     It's essentially the texture's type.
	//      surface->w:    The width of the texture
	//      surface->h:    The height of the texture
	//               0:    The border.  Don't worry about this if you're just starting.
	//     pixelFormat:    The format that the *data* is in--NOT the texture! 
	//GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
	//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
	//                         any value that fits in one byte (so 0 through 255).  These values are to be interpreted as
	//                         *unsigned* values (since 0x00 should be dark and 0xFF should be bright).
	// surface->pixels:    The actual data.  As above, SDL's array of bytes.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels);

	//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void DeleteTexture(Texture & texture)
{
	glDeleteTextures(1, &texture.id);
}

void DrawTexture(const Texture & texture, const Point2f& bottomLeftVertex, const Rectf & textureRect)
{
	Rectf vertexRect{ bottomLeftVertex.x, bottomLeftVertex.y, textureRect.width, textureRect.height };
	DrawTexture(texture, vertexRect, textureRect);
}

void DrawTexture(const Texture & texture, const Rectf & vertexRect, const Rectf & textureRect)
{
	// Determine texture coordinates
	float textLeft{};
	float textRight{};
	float textTop{};
	float textBottom{};
	if (!(textureRect.width > 0.0f && textureRect.height > 0.0f)) // No rect specified, use complete texture
	{
		textLeft = 0.0f;
		textRight = 1.0f;
		textTop = 0.0f;
		textBottom = 1.0f;
	}
	else // Clip specified, convert them to the range [0.0, 1.0]
	{
		textLeft = textureRect.left / texture.width;
		textRight = (textureRect.left + textureRect.width) / texture.width;
		textTop = (textureRect.bottom - textureRect.height) / texture.height;
		textBottom = textureRect.bottom / texture.height;
	}

	// Determine vertex coordinates
	float vertexLeft{ vertexRect.left };
	float vertexBottom{ vertexRect.bottom };
	float vertexRight{};
	float vertexTop{};
	if (!(vertexRect.width > 0.0f && vertexRect.height > 0.0f)) // If no size specified use size of texture
	{
		
		vertexRight = vertexLeft + (texture.width);
		vertexTop = vertexBottom + (texture.height);
	}
	else
	{
		vertexRight = vertexLeft + (vertexRect.width);
		vertexTop = vertexBottom + (vertexRect.height);

	}

	// Tell opengl which texture we will use
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// Draw
	glEnable(GL_TEXTURE_2D);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(textLeft, textBottom);
			glVertex2f(vertexLeft, vertexBottom);

			glTexCoord2f(textLeft, textTop);
			glVertex2f(vertexLeft, vertexTop);

			glTexCoord2f(textRight, textTop);
			glVertex2f(vertexRight, vertexTop);

			glTexCoord2f(textRight, textBottom);
			glVertex2f(vertexRight, vertexBottom);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

}
#pragma endregion textureImplementations
