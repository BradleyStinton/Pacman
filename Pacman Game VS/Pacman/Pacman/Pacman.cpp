#include "Pacman.h"

#include <sstream>

#include <time.h>



#define MUNCHIECOUNT 50

void Pacman::Input(int elaseTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
}

void Pacman::CheckViewportCollision()
{
}

void Pacman::UpdatePacman(int elapsedTime)
{
}

void Pacman::UpdateMunchies(Enemy*, int elapsedTime)
{
}











Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500), _cCherryFrameTime(500)
{

	//Initialise member variables
	_pacman = new Player();
	_pacman->dead = false;
	

	_munchieFrameCount = 0;
	_cherryFrameCount = 0;
	_paused = false;
	_pKeyDown = false;
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;
	_munchieCurrentFrameTime = 0;
	_cherryCurrentFrameTime = 0;
	_pacman->speedMultiplier = 1.0f;
	srand(time(NULL));


	//Munchie Count
	int i;

	for (i = 0; i < MUNCHIECOUNT; i++)
		{
		_munchies[i] = new Enemy();
		_munchies[i]->frame = rand() % 1;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->currentFrameTime = rand() % 500 + 50;
		}



	//Enemy Variables
	_ghosts = new MovingEnemy();
	_ghosts->direction = 0;
	_ghosts->speed = 0.2f;
	
	Audio::Initialise();
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	//Sound
	_pop = new SoundEffect();


	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
	delete _cherryTexture;
	delete _cherryInvertedTexture;
	delete _ghosts->texture;
	delete _pop;

	//Clean up the texture
	delete _munchies[0]->texture;

	int nCount = 0;
	for (nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->position;
		delete _munchies[nCount]->sourceRect;
		delete _munchies[nCount];
	}
	//Order of operations is important, array deleted last
	delete[] _munchies;

}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", false);


	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}
	

	//Load Cherry
	_cherryTexture = new Texture2D();
	_cherryTexture->Load("Textures/Cherry.png", true);
	_cherryInvertedTexture = new Texture2D();
	_cherryInvertedTexture->Load("Textures/CherryInverted.png", true);
	_cherryRect = new Rect(200.0f, 300.0f, 25, 25);

	//Load Ghost Character
	_ghosts->texture = new Texture2D();
	_ghosts->texture->Load("Textures/GhostBlue.png", false);
	_ghosts->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_ghosts->sourceRect = new Rect(0.0f, 0.0f, 20, 20);

	//Load sound effects
	//_pop->Load("Sounds/pop.wav");



	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight() / 2.0f);
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);


}



void Pacman::Update(int elapsedTime)
{

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		UpdateMunchies(_munchies[i], elapsedTime);
	}

	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	//Count frames
	_pacman->currentFrameTime += elapsedTime;

	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;

		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}

	//Change Pacman direction facing
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	
	
	if (!_paused)
	{//Cause Pacman's mouth to move
		_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
	}

	if (!_paused)
	{
		Input(elapsedTime, keyboardState, mouseState);
		UpdatePacman(elapsedTime);
		UpdateGhost(_ghosts, elapsedTime);
	}



	// Check if the P Key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;

	if (!_paused)
	{

		if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
		{
			_pacman->speedMultiplier = 2.0f;
		}
		else
		{
			_pacman->speedMultiplier = 1.0f;
		}






		// Checks if D key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			_pacman->position->X += _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier; //Moves Pacman across X axis
			_pacman->direction = 0; //Sets Pacman Facing
		}

			// Checks if A key is pressed
		else if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			_pacman->position->X += -_cPacmanSpeed * elapsedTime * _pacman->speedMultiplier; //Moves Pacman across X axis
			_pacman->direction = 2;
		}
			// Checks if S key is pressed
		else if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			_pacman->position->Y += _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier; //Moves Pacman across Y axis
			_pacman->direction = 1;
		}
			// Checks if W key is pressed
		else if (keyboardState->IsKeyDown(Input::Keys::W))
		{
			_pacman->position->Y += -_cPacmanSpeed * elapsedTime * _pacman->speedMultiplier; //Moves Pacman across Y axis
			_pacman->direction = 3;
		}




		// Checks if Pacman is trying to disappear
		if (_pacman->position->X + _pacman->sourceRect->Width > 1024) //1024 is game width
		{
			//Pacman hit right wall - reset his position
			_pacman->position->X = 1024 - _pacman->sourceRect->Width;
			//Pacman will appear on left side after hitting right wall
			_pacman->position->X = 0 - _pacman->sourceRect->Width;

		}
		// Checks if Pacman is trying to disappear
		if (_pacman->position->X + _pacman->sourceRect->Width < 0) //30 is left side size
		{
			//Pacman hit left wall - reset his position
			_pacman->position->X = 0 - _pacman->sourceRect->Width;
			//Pacman will appear on right side after hitting left wall
			_pacman->position->X = 1024 - _pacman->sourceRect->Width;
		}
		// Checks if Pacman is trying to disappear
		if (_pacman->position->Y + _pacman->sourceRect->Height < 0) //30 is left side size
		{
			//Pacman hit top wall - reset his position
			_pacman->position->Y = 0 - _pacman->sourceRect->Height;
			//Pacman will appear on bottom side after hitting top wall
			_pacman->position->Y = 768 - _pacman->sourceRect->Height;

		}
		// Checks if Pacman is trying to disappear
		if (_pacman->position->Y + _pacman->sourceRect->Height > 768) //1024 is game width
		{
			//Pacman hit bottom wall - reset his position
			_pacman->position->Y = 768 - _pacman->sourceRect->Height;
			//Pacman will appear on top side after hitting bottom wall
			_pacman->position->Y = 0 - _pacman->sourceRect->Height;
		}

		

		_munchieCurrentFrameTime += elapsedTime;

		if (_munchieCurrentFrameTime > _cMunchieFrameTime)
		{
			_munchieFrameCount++;

			if (_munchieFrameCount >= 2)
				_munchieFrameCount = 0;

			_munchieCurrentFrameTime = 0;

		}

		_cherryCurrentFrameTime += elapsedTime;

		if (_cherryCurrentFrameTime > _cCherryFrameTime)
		{
			_cherryFrameCount++;

			if (_cherryFrameCount >= 2)
				_cherryFrameCount = 0;

			_cherryCurrentFrameTime = 0;
		}

	}
}

void Pacman::UpdateGhost(MovingEnemy* ghosts, int elapsedTime)
{
	if (ghosts->direction == 0) //Moves Right
	{
		ghosts ->position->X += ghosts->speed * elapsedTime;
	}
	else if (ghosts->direction == 1) //Moves Left
	{
		ghosts->position->X -= ghosts->speed * elapsedTime;
	}
	if (ghosts->position->X + ghosts->sourceRect->Width >= Graphics::GetViewportWidth()) //Hits right edge
	{
		ghosts->direction = 1; //Change direction
	}
	else if (ghosts->position->X <= 0) //Hits left edge
	{
		ghosts->direction = 0; //Change direction
	}

}

void Pacman::CheckGhostCollisions()
{
	//Local Variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0;i < GHOSTCOUNT; i++)
	{
		//Populate variables with Ghost data
		bottom2 = _ghosts->position->Y + _ghosts->sourceRect->Height;
		left2 = _ghosts->position->X;
		right2 = _ghosts->position->X + _ghosts->sourceRect->Width;
		top2 = _ghosts->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing

	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	}
	SpriteBatch::Draw(_ghosts->texture, _ghosts->position, _ghosts->sourceRect);

	if (_munchieFrameCount == 0)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_munchieFrameCount++;

		if (_munchieFrameCount >= 60)
			_munchieFrameCount = 0;
	}


	//Draw Cherry
	if (_cherryFrameCount == 0)
	{
		SpriteBatch::Draw(_cherryInvertedTexture, _cherryRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}
	else
	{
		SpriteBatch::Draw(_cherryTexture, _cherryRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_cherryFrameCount++;

		if (_cherryFrameCount >= 60)
			_cherryFrameCount = 0;
	}




	// When paused create pause screen
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

	//Draw Ghosts
	SpriteBatch::Draw(_ghosts->texture, _ghosts->position, _ghosts->sourceRect);


	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}