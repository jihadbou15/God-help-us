// 1DAE06 - Bossuyt Niels

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
const float g_WindowWidth{ 600.0f };
const float g_WindowHeight{ 800.0f };
const std::string g_WindowTitle{ "TheImpossibleGameDontEvenTryUnlessYouWantToGetFucked - Bossuyt, Niels- 1DAE06" };
bool g_IsVSyncOn{ true };
#pragma endregion windowInformation

#pragma region textureDeclarations
struct Texture
{
	GLuint id;
	float width;
	float height;
};

bool TextureFromFile( const std::string& path, Texture & texture );
bool TextureFromString( const std::string & text, TTF_Font *pFont, const Color4f & textColor, Texture & texture );
bool TextureFromString( const std::string & text, const std::string& fontPath, int ptSize, const Color4f & textColor, Texture & texture );
void TextureFromSurface( const SDL_Surface *pSurface, Texture & textureData );
void DrawTexture( const Texture & texture, const Point2f& bottomLeftVertex, const Rectf & textureRect = {} );
void DrawTexture( const Texture & texture, const Rectf & vertexRect, const Rectf & textureRect = {} );
void DeleteTexture( Texture & texture );
#pragma endregion textureDeclarations

#pragma region coreDeclarations
// Functions
void Initialize( );
void Run( );
void Cleanup( );
void QuitOnSDLError( );
void QuitOnOpenGlError( );
void QuitOnImageError( );
void QuitOnTtfError( );

// Variables
SDL_Window* g_pWindow{ nullptr }; // The window we'll be rendering to
SDL_GLContext g_pContext; // OpenGL context
Uint32 g_MilliSeconds{};
const Uint32 g_MaxElapsedTime{ 100 };
#pragma endregion coreDeclarations

#pragma region gameDeclarations
// Functions
void InitGameResources( );
void FreeGameResources( );

void ProcessKeyDownEvent( const SDL_KeyboardEvent  & e );
void ProcessKeyUpEvent( const SDL_KeyboardEvent  & e );
void ProcessMouseMotionEvent( const SDL_MouseMotionEvent & e );
void ProcessMouseDownEvent( const SDL_MouseButtonEvent & e );
void ProcessMouseUpEvent( const SDL_MouseButtonEvent & e );

void Update( float elapsedSec );
void UpdateBall(float elapsedSec);
void KeepBallInScreen();
void BounceOffBat(Point2f PrevBallPos);
void UpdateBat(float elapsedSec);

void Draw( );
void DrawBat();
void DrawBall();
void ClearBackground( );



float CalculateAngle(float Point1X, float Point1Y, float Point2X, float Point2Y);

// Variables
// init values
const Point2f g_BatDimens{100.0f,50.0f};
const Point2f g_BatPos{g_WindowWidth/2 - g_BatDimens.x / 2 ,60.0f};
float g_VelBatValue{500.0f};
float g_ColorR{0.0f};
float g_ColorG{1.0f};
float g_ColorB{0.0f};
float g_ColorA{0.5f};

//Bat var
Rectf g_BatRect{ g_BatPos.x, g_BatPos.y,g_BatDimens.x,g_BatDimens.y};
float g_BatVel{};
bool g_MoveLeft{};
bool g_MoveRight{};

//Ball var
Color4f g_ColorBall{ g_ColorR ,g_ColorG ,g_ColorB ,g_ColorA };
Point2f g_Radius{ 20.0f,20.0f };
Point2f g_Center{ g_WindowWidth/2, g_WindowHeight / 2 };
Point2f g_PrevBallPos{};
float g_VelBallYValue{ -300.0f };
float g_VelBallXValue{300.0f};

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
#pragma endregion gameDeclarations



int main( int argc, char* args[] )
{
	// Initialize SDL and OpenGL
	Initialize( );

	// Event loop
	Run( );

	// Clean up SDL and OpenGL
	Cleanup( );

	return 0;
}

#pragma region gameImplementations
void InitGameResources( )
{
	TextureFromFile("Resources/ball.png",g_BallTex);
	TextureFromFile("Resources/bat base.png", g_BatTex);
	TextureFromFile("Resources/bomb.png", g_BombTex);
	TextureFromFile("Resources/boss.png", g_BossTex);
	TextureFromFile("Resources/boss laser .png", g_BossWithLaserTex);
	TextureFromFile("Resources/danger.png", g_dangerTex);
	TextureFromFile("Resources/bat dead.png", g_deadBatTex);
	TextureFromFile("Resources/laser piece.png", g_LaserTex);
	TextureFromFile("Resources/laser piece boss.png", g_LaserBossTex);
	TextureFromFile("Resources/left canon.png", g_LeftCanonTex);
	TextureFromFile("Resources/left canon laser.png", g_LeftCanonLaserTex);
	TextureFromFile("Resources/right canon.png", g_RightCanonTex);
	TextureFromFile("Resources/right canon laser .png", g_RightCanonLaserTex);
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
void ProcessKeyDownEvent( const SDL_KeyboardEvent  & e )
{
	switch (e.keysym.sym)
	{
	case SDLK_LEFT:
		g_MoveLeft = true;
		break;
	case SDLK_RIGHT:
		g_MoveRight = true;
		break;
	}
}
void ProcessKeyUpEvent(const SDL_KeyboardEvent  & e)
{
	switch (e.keysym.sym)
	{
	case SDLK_LEFT:
		g_BatVel = 0.0f;
		break;
	case SDLK_RIGHT:
		g_BatVel = 0.0f;
		break;
	}
}
void ProcessMouseMotionEvent( const SDL_MouseMotionEvent & e )
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	//Point2f mousePos{ float( e.x ), float( g_WindowHeight - e.y ) };
}
void ProcessMouseDownEvent( const SDL_MouseButtonEvent & e )
{

}
void ProcessMouseUpEvent( const SDL_MouseButtonEvent & e )
{
	//std::cout << "  [" << e.x << ", " << e.y << "]\n";
	switch ( e.button )
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


void ClearBackground( )
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
void Update( float elapsedSec )
{
	UpdateBat(elapsedSec);
	UpdateBall(elapsedSec);
}

void Draw( )
{
	ClearBackground( );
	DrawBat();
	DrawBall();
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
		g_MoveLeft = false;
	}
	else if (g_MoveRight)
	{
		g_BatVel = g_VelBatValue;
		g_MoveRight = false;
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
	float scale{g_BallTex.width/(2*g_Radius.x)};
	Rectf ballRect{};
	ballRect.left = g_Center.x - g_Radius.x;
	ballRect.bottom = g_Center.y - g_Radius.y;
	ballRect.width = g_BallTex.width / scale;
	ballRect.height = g_BallTex.height / scale;;
	DrawTexture(g_BallTex,ballRect);

}
void UpdateBall(float elapsedSec)
{
	//old position
	g_PrevBallPos.x = g_Center.x;
	g_PrevBallPos.y = g_Center.y;

	//new position
	g_Center.y += g_VelBallYValue*elapsedSec;
	g_Center.x += g_VelBallXValue*elapsedSec;

	BounceOffBat(g_PrevBallPos);

	KeepBallInScreen();
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
void BounceOffBat(Point2f PrevBallPos)
{
	bool IsBallXInRect
	{ 
		dae::IsXBetween(g_BatRect.left + g_BatRect.width, g_BatRect.left, g_Center.x + g_Radius.x)
		|| dae::IsXBetween(g_BatRect.left + g_BatRect.width, g_BatRect.left, g_Center.x - g_Radius.x) 
	};
	bool IsBallYInRect
	{ 
		dae::IsYBetween(g_BatRect.bottom + g_BatRect.height, g_BatRect.bottom, g_Center.y + g_Radius.y)
		|| dae::IsYBetween(g_BatRect.bottom + g_BatRect.height, g_BatRect.bottom, g_Center.y - g_Radius.y) 
	};
	if (IsBallXInRect && IsBallYInRect)// if the ball hits the rect
	{
		//if it hits on the left or right
		if (PrevBallPos.y < g_BatRect.bottom + g_BatRect.height && PrevBallPos.y > g_BatRect.bottom)
		{
			g_VelBallXValue = -g_VelBallXValue;
			//if it hits on the right
			if (PrevBallPos.x > g_BatRect.left && PrevBallPos.x > g_BatRect.left + g_BatRect.width)
			{
				g_Center.x = g_BatRect.left + g_BatRect.width + g_Radius.x;
			}
			//if it hits on the right
			else
			{
				g_Center.x = g_BatRect.left - g_Radius.x;
			}
		}
		//if it hits on the top or bottom
		if (PrevBallPos.x > g_BatRect.left  && PrevBallPos.x < g_BatRect.left+ g_BatRect.width)
		{
			g_VelBallYValue = -g_VelBallYValue;
			//if it hits on the top
			if (PrevBallPos.y > g_BatRect.bottom && PrevBallPos.y > g_BatRect.bottom + g_BatRect.height)
			{
				g_Center.y = g_BatRect.bottom + g_BatRect.height + g_Radius.y;
			}
			//if it hits on the bottom
			else
			{
				g_Center.y = g_BatRect.bottom - g_Radius.y;
			}
		}

	}

}
void KeepBallInScreen()
{
	if (g_Center.y+g_Radius.y > g_WindowHeight)
	{
		g_VelBallYValue = -g_VelBallYValue;
		g_Center.y = g_WindowHeight - g_Radius.y;
	}
	if (g_Center.y - g_Radius.y < 0.0f)
	{
		g_VelBallYValue = -g_VelBallYValue;
		g_Center.y = g_Radius.y;
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
#pragma endregion gameImplementations

#pragma region coreImplementations
void Initialize( )
{
	//Initialize SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		QuitOnSDLError( );
	}

	//Use OpenGL 2.1
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

	//Create window
	g_pWindow = SDL_CreateWindow(
		g_WindowTitle.c_str( ),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		int( g_WindowWidth ),
		int( g_WindowHeight ),
		SDL_WINDOW_OPENGL );

	if ( g_pWindow == nullptr )
	{
		QuitOnSDLError( );
	}

	// Create an opengl context and attach it to the window 
	g_pContext = SDL_GL_CreateContext( g_pWindow );
	if ( g_pContext == nullptr )
	{
		QuitOnSDLError( );
	}

	if ( g_IsVSyncOn )
	{
		// Synchronize buffer swap with the monitor's vertical refresh
		if ( SDL_GL_SetSwapInterval( 1 ) < 0 )
		{
			QuitOnSDLError( );
		}
	}


	// Initialize Projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	// Set the clipping (viewing) area's left, right, bottom and top
	gluOrtho2D( 0, g_WindowWidth, 0, g_WindowHeight );

	// The viewport is the rectangular region of the window where the image is drawn.
	// Set it to the entire client area of the created window
	glViewport( 0, 0, int( g_WindowWidth ), int( g_WindowHeight ) );

	//Initialize Modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// Enable color blending and use alpha blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if ( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		QuitOnImageError( );
	}

	//Initialize SDL_ttf
	if ( TTF_Init( ) == -1 )
	{
		QuitOnTtfError( );
	}
}
void Run( )
{
	//Main loop flag
	bool quit{ false };

	// Set start time
	g_MilliSeconds = SDL_GetTicks( );

	InitGameResources( );

	//The event loop
	SDL_Event e{};
	while ( !quit )
	{
		// Poll next event from queue
		while ( SDL_PollEvent( &e ) != 0 )
		{
			// Handle the polled event
			switch ( e.type )
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				ProcessKeyDownEvent( e.key );
				break;
			case SDL_KEYUP:
				ProcessKeyUpEvent( e.key );
				break;
			case SDL_MOUSEMOTION:
				ProcessMouseMotionEvent( e.motion );
				break;
			case SDL_MOUSEBUTTONDOWN:
				ProcessMouseDownEvent( e.button );
				break;
			case SDL_MOUSEBUTTONUP:
				ProcessMouseUpEvent( e.button );
				break;
			}
		}

		if ( !quit )
		{

			// Calculate elapsed time
			// Get the number of milliseconds since the SDL library initialization
			// Note that this value wraps if the program runs for more than ~49 days.
			Uint32 currentMilliSeconds = SDL_GetTicks( );

			// Calculate elapsed time
			Uint32 elapsedTime = currentMilliSeconds - g_MilliSeconds;

			// Update current time
			g_MilliSeconds = currentMilliSeconds;

			// Prevent jumps in time caused by break points
			if ( elapsedTime > g_MaxElapsedTime )
			{
				elapsedTime = g_MaxElapsedTime;
			}

			// Call update function, using time in seconds (!)
			Update( elapsedTime / 1000.0f );

			// Draw in the back buffer
			Draw( );

			// Update screen: swap back and front buffer
			SDL_GL_SwapWindow( g_pWindow );
		}
	}

	FreeGameResources( );
}

void Cleanup( )
{
	SDL_GL_DeleteContext( g_pContext );

	SDL_DestroyWindow( g_pWindow );
	g_pWindow = nullptr;

	SDL_Quit( );
}


void QuitOnSDLError( )
{
	std::cout << "Problem during SDL initialization: ";
	std::cout << SDL_GetError( );
	std::cout << std::endl;
	Cleanup( );
	exit( -1 );
}
void QuitOnOpenGlError( )
{
	std::cout << "Problem during OpenGL initialization: ";
	std::cout << SDL_GetError( );
	std::cout << std::endl;
	Cleanup( );
	exit( -1 );
}
void QuitOnImageError( )
{
	std::cout << "Problem during SDL_image initialization: ";
	std::cout << IMG_GetError( );
	std::cout << std::endl;
	Cleanup( );
	exit( -1 );
}
void QuitOnTtfError( )
{
	std::cout << "Problem during SDL_ttf initialization: ";
	std::cout << TTF_GetError( );
	std::cout << std::endl;
	Cleanup( );
	exit( -1 );
}
#pragma endregion coreImplementations

#pragma region textureImplementations

bool TextureFromFile( const std::string& path, Texture & texture )
{
	//Load file for use as an image in a new surface.
	SDL_Surface* pLoadedSurface = IMG_Load( path.c_str( ) );
	if ( pLoadedSurface == nullptr )
	{
		std::cout << "TextureFromFile: SDL Error when calling IMG_Load: " << SDL_GetError( ) << std::endl;
		return false;
	}

	TextureFromSurface( pLoadedSurface, texture );

	//Get rid of the loaded surface
	SDL_FreeSurface( pLoadedSurface );

	return true;
}

bool TextureFromString( const std::string & text, const std::string& fontPath, int ptSize, const Color4f & textColor, Texture & texture )
{
	// Create font
	TTF_Font *pFont{};
	pFont = TTF_OpenFont( fontPath.c_str( ), ptSize );
	if ( pFont == nullptr )
	{
		std::cout << "TextureFromString: Failed to load font! SDL_ttf Error: " << TTF_GetError( );
		std::cin.get( );
		return false;
	}

	// Create texture using this fontand close font afterwards
	bool textureOk = TextureFromString( text, pFont, textColor, texture );
	TTF_CloseFont( pFont );

	return textureOk;
}

bool TextureFromString( const std::string & text, TTF_Font *pFont, const Color4f & color, Texture & texture )
{
	//Render text surface
	SDL_Color textColor{};
	textColor.r = Uint8( color.r * 255 );
	textColor.g = Uint8( color.g * 255 );
	textColor.b = Uint8( color.b * 255 );
	textColor.a = Uint8( color.a * 255 );

	SDL_Surface* pLoadedSurface = TTF_RenderText_Blended( pFont, text.c_str( ), textColor );
	//SDL_Surface* pLoadedSurface = TTF_RenderText_Solid(pFont, textureText.c_str(), textColor);
	if ( pLoadedSurface == nullptr )
	{
		std::cout << "TextureFromString: Unable to render text surface! SDL_ttf Error: " << TTF_GetError( );
		return false;
	}

	// copy to video memory
	TextureFromSurface( pLoadedSurface, texture );

	//Get rid of loaded surface
	SDL_FreeSurface( pLoadedSurface );

	return true;
}

void TextureFromSurface( const SDL_Surface *pSurface, Texture & texture )
{
	//Get image dimensions
	texture.width = float( pSurface->w );
	texture.height = float( pSurface->h );

	// Get pixel format information and translate to OpenGl format
	GLenum pixelFormat{ GL_RGB };
	switch ( pSurface->format->BytesPerPixel )
	{
	case 1:
		pixelFormat = GL_RED;
		break;
	case 2:
		pixelFormat = GL_RG;
		break;
	case 3:
		if ( pSurface->format->Rmask == 0x000000ff )
		{
			pixelFormat = GL_RGB;
		}
		else
		{
			pixelFormat = GL_BGR;
		}
		break;
	case 4:
		if ( pSurface->format->Rmask == 0x000000ff )
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
	glGenTextures( 1, &texture.id );

	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	glBindTexture( GL_TEXTURE_2D, texture.id );

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
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels );

	//Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	//*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	//each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	//further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	//them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void DeleteTexture( Texture & texture )
{
	glDeleteTextures( 1, &texture.id );
}

void DrawTexture( const Texture & texture, const Point2f& bottomLeftVertex, const Rectf & textureRect )
{
	Rectf vertexRect{ bottomLeftVertex.x, bottomLeftVertex.y, textureRect.width, textureRect.height };
	DrawTexture( texture, vertexRect, textureRect );
}

void DrawTexture( const Texture & texture, const Rectf & vertexRect, const Rectf & textureRect )
{
	// Determine texture coordinates
	float textLeft{};
	float textRight{};
	float textTop{};
	float textBottom{};
	if ( !( textureRect.width > 0.0f && textureRect.height > 0.0f ) ) // No rect specified, use complete texture
	{
		textLeft = 0.0f;
		textRight = 1.0f;
		textTop = 0.0f;
		textBottom = 1.0f;
	}
	else // Clip specified, convert them to the range [0.0, 1.0]
	{
		textLeft = textureRect.left / texture.width;
		textRight = ( textureRect.left + textureRect.width ) / texture.width;
		textTop = ( textureRect.bottom - textureRect.height ) / texture.height;
		textBottom = textureRect.bottom / texture.height;
	}

	// Determine vertex coordinates
	float vertexLeft{ vertexRect.left };
	float vertexBottom{ vertexRect.bottom };
	float vertexRight{};
	float vertexTop{};
	if ( !( vertexRect.width > 0.0f && vertexRect.height > 0.0f ) ) // If no size specified use size of texture
	{
		vertexRight = vertexLeft + texture.width;
		vertexTop = vertexBottom + texture.height;
	}
	else
	{
		vertexRight = vertexLeft + vertexRect.width;
		vertexTop = vertexBottom + vertexRect.height;

	}

	// Tell opengl which texture we will use
	glBindTexture( GL_TEXTURE_2D, texture.id );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	// Draw
	glEnable( GL_TEXTURE_2D );
	{
		glBegin( GL_QUADS );
		{
			glTexCoord2f( textLeft, textBottom );
			glVertex2f( vertexLeft, vertexBottom );

			glTexCoord2f( textLeft, textTop );
			glVertex2f( vertexLeft, vertexTop );

			glTexCoord2f( textRight, textTop );
			glVertex2f( vertexRight, vertexTop );

			glTexCoord2f( textRight, textBottom );
			glVertex2f( vertexRight, vertexBottom );
		}
		glEnd( );
	}
	glDisable( GL_TEXTURE_2D );

}
#pragma endregion textureImplementations
