#pragma once


#define MUNCHIECOUNT 50
// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

constexpr auto GHOSTCOUNT = 4;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

//Structure Definition
struct Player
{
	bool dead;
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	
};

struct Enemy
{
	int currentFrameTime;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	int Direction;
	float speed;
};


struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
	int frame;
};




class Pacman : public Game
{
private:

	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state);

	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();

	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchies(Enemy*, int elapsedTime);

	void Input(int elapsedTime, Input::KeyboardState* keyboardState, Input::MouseState* mouseState);

	//Collision check
	void CheckGhostCollisions();

	void UpdateGhost(MovingEnemy*, int elapsedTime);



	// Data to represent Pacman
	Player* _pacman;
	//Enemy* _munchies[MUNCHIECOUNT];
	Enemy* _munchies[MUNCHIECOUNT];
	//data to represent Ghosts
	MovingEnemy* _ghosts;

	// Data to represent Munchie
	int _munchieFrameCount = 0;
	Rect* _munchieRect;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieInvertedTexture;
	int _munchieFrame;
	int _munchieCurrentFrameTime;

	//Data to represent Cherry
	int _cherryFrameCount = 0;
	Rect* _cherryRect;
	Texture2D* _cherryTexture;
	Texture2D* _cherryInvertedTexture;
	int _cherryFrame;
	int _cherryCurrentFrameTime;

	


	// Position for String
	Vector2* _stringPosition;

	//Constant data for Game Variables
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	const int _cCherryFrameTime;
	const int _cMunchieFrameTime;

	// Data for Menu
	Texture2D* _menuBackground = new Texture2D();
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;

	//Pause menu key
	bool _pKeyDown;


	//Data for sound effect
	SoundEffect* _pop;


public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);


};