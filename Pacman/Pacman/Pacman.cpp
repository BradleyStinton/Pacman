#include "Pacman.h"

#include <sstream>

//void Pacman::Input(int elaseTime, Input::KeyboardState* state)
//{
//}

//void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
//{
//}

//void Pacman::CheckViewportCollision()
//{
//}

//void Pacman::UpdatePacman(int elapsedTime)
//{
//}

//void Pacman::UpdateMunchie(int elapsedTime)
//{
//}




Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500)
{

	_munchieFrameCount = 0;
	_paused = false;
	_pKeyDown = false;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	_munchieCurrentFrameTime = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

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
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	//Count frames
	_pacmanCurrentFrameTime += elapsedTime;

	if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
	{
		_pacmanFrame++;

		if (_pacmanFrame >= 2)
			_pacmanFrame = 0;

		_pacmanCurrentFrameTime = 0;
	}

	//Change Pacman direction facing
	_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;
	
	
	if (!_paused)
	{//Cause Pacman's mouth to move
		_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;
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

		// Checks if D key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
			_pacmanDirection = 0; //Sets Pacman Facing
		}

			// Checks if A key is pressed
		else if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			_pacmanPosition->X += -_cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
			_pacmanDirection = 2;
		}
			// Checks if S key is pressed
		else if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman across Y axis
			_pacmanDirection = 1;
		}
			// Checks if W key is pressed
		else if (keyboardState->IsKeyDown(Input::Keys::W))
		{
			_pacmanPosition->Y += -_cPacmanSpeed * elapsedTime; //Moves Pacman across Y axis
			_pacmanDirection = 3;
		}




		// Checks if Pacman is trying to disappear
		if (_pacmanPosition->X + _pacmanSourceRect->Width > 1024) //1024 is game width
		{
			//Pacman hit right wall - reset his position
			_pacmanPosition->X = 1024 - _pacmanSourceRect->Width;
			//Pacman will appear on left side after hitting right wall
			_pacmanPosition->X = 0 - _pacmanSourceRect->Width;

		}
		// Checks if Pacman is trying to disappear
		if (_pacmanPosition->X + _pacmanSourceRect->Width < 0) //30 is left side size
		{
			//Pacman hit left wall - reset his position
			_pacmanPosition->X = 0 - _pacmanSourceRect->Width;
			//Pacman will appear on right side after hitting left wall
			_pacmanPosition->X = 1024 - _pacmanSourceRect->Width;
		}
		// Checks if Pacman is trying to disappear
		if (_pacmanPosition->Y + _pacmanSourceRect->Height < 0) //30 is left side size
		{
			//Pacman hit top wall - reset his position
			_pacmanPosition->Y = 0 - _pacmanSourceRect->Height;
			//Pacman will appear on bottom side after hitting top wall
			_pacmanPosition->Y = 768 - _pacmanSourceRect->Height;

		}
		// Checks if Pacman is trying to disappear
		if (_pacmanPosition->Y + _pacmanSourceRect->Height > 768) //1024 is game width
		{
			//Pacman hit bottom wall - reset his position
			_pacmanPosition->Y = 768 - _pacmanSourceRect->Height;
			//Pacman will appear on top side after hitting bottom wall
			_pacmanPosition->Y = 0 - _pacmanSourceRect->Height;
		}

		

		_munchieCurrentFrameTime += elapsedTime;

		if (_munchieCurrentFrameTime > _cMunchieFrameTime)
		{
			_munchieFrameCount++;

			if (_munchieFrameCount >= 2)
				_munchieFrameCount = 0;

			_munchieCurrentFrameTime = 0;

		}
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

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
	// When paused create pause screen
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}




	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}