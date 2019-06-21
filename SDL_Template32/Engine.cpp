#include <typeinfo>
#include "Engine.h"
#define FPS 10 // A '.0' will make it a double.

Engine::Engine():m_bStarted(false), m_bRunning(false)
{
	cout << "Constructing engine." << endl;
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
}

Engine::~Engine()
{
	cout << "Destructing engine." << endl;
}

void Engine::Run(const char * title, int xPos, int yPos, int width, int height, int flags)
{
	if (m_bStarted == true)
		return;
	cout << "Starting game." << endl;
	m_bStarted = true;
	if (Init(title, xPos, yPos, width, height, flags) == false)
	{
		cout << "Cannot initialize game." << endl;
		return;
	}
	// Main engine loop here.
	while (Running())
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (Running())
			Sleep();
	}
	// End main engine loop.
	Clean();
}

Engine* Engine::Instance() // This method creates the static instance that can be accessed 'globally'
{
	static Engine* instance = new Engine(); // C++11 will prevent this line from running more than once.
	return instance;
}

bool Engine::Init(const char * title, int xPos, int yPos, int width, int height, int flags)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0) // SDL init success.
	{
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != 0) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				if (IMG_Init(IMG_INIT_PNG))
				{
					SDL_Surface* pScreensSurf = IMG_Load("Title.png");
					SDL_Surface* pPlayButtonSurf = IMG_Load("playButton.png");
					SDL_Surface* pPlayerSurf = IMG_Load("Player.png");
					SDL_Surface* pTileSurf = IMG_Load("Tiles.png");

					m_oScreensText = SDL_CreateTextureFromSurface(m_pRenderer, pScreensSurf);
					m_pPlayButtonText = SDL_CreateTextureFromSurface(m_pRenderer, pPlayButtonSurf);
					m_pPlayerText = SDL_CreateTextureFromSurface(m_pRenderer, pPlayerSurf);
					m_pTileText = SDL_CreateTextureFromSurface(m_pRenderer, pTileSurf);
					
					SDL_FreeSurface(pScreensSurf);
					SDL_FreeSurface(pPlayButtonSurf);
					SDL_FreeSurface(pPlayerSurf);
					SDL_FreeSurface(pTileSurf);
					cout << "Image creation success!" << endl;
				}
				else return false; // Img(Png) init fail. The JPG library is not available in this build
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	// Continue here after successful init.
	m_sMachin = new SMachine();
	m_sMachin->PushState(new TitleState());
	m_pPlayer = new Player({0, 0, 32, 32}, {COLS/2*32, ROWS/2*32, 32, 32});
	m_pPlayer->Reset(COLS/2, ROWS/2); //created a Reset function to reset player's condition when player opts to play again
	m_pLevels = new Level[5] { Level(3, "Level1.txt"), Level(1, "Level2.txt"), Level(1, "Level3.txt"), 
		                       Level(2, "Level4.txt"), Level(1, "Level5.txt") };
	m_iCurrLevel = 0;
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	m_mouse = nullptr;
	m_bRunning = true; // Everything is okay, start the engine.
	return true;
}

void Engine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_mouse = new SDL_MouseButtonEvent(event.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_mouse = nullptr;
			break;
		}
	}
}

bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}
void Engine::GetMouse(SDL_Rect r) {
	if (m_mouse) {
		
		if (m_mouse->x >= r.x && m_mouse->x <= (r.x + r.w) &&
			m_mouse->y >= r.y && m_mouse->y <= (r.y + r.h)) {
				RequestStateChange();
		}
	}
}

void Engine::SetLevel(int index) {
	m_iCurrLevel = index;
}

void Engine::Update()
{
	m_sMachin->Update();

	/*

	// Move the player.
	if (KeyDown(SDL_SCANCODE_W) && m_pLevels[m_iCurrLevel].m_Map[m_pPlayer->GetY() - 1][m_pPlayer->GetX()].isObstacle() == false)
	{
		m_pPlayer->SetY(m_pPlayer->GetY() - 1);
	}
	else if (KeyDown(SDL_SCANCODE_S) && m_pLevels[m_iCurrLevel].m_Map[m_pPlayer->GetY() + 1][m_pPlayer->GetX()].isObstacle() == false)
	{
		m_pPlayer->SetY(m_pPlayer->GetY() + 1);
	}
	if (KeyDown(SDL_SCANCODE_A) && m_pLevels[m_iCurrLevel].m_Map[m_pPlayer->GetY()][m_pPlayer->GetX() - 1].isObstacle() == false)
	{
		m_pPlayer->SetX(m_pPlayer->GetX() - 1);
	}
	else if (KeyDown(SDL_SCANCODE_D) && m_pLevels[m_iCurrLevel].m_Map[m_pPlayer->GetY()][m_pPlayer->GetX() + 1].isObstacle() == false)
	{
		m_pPlayer->SetX(m_pPlayer->GetX() + 1);
	}
	// Hazard check.
	if (m_pLevels[m_iCurrLevel].m_Map[m_pPlayer->GetY()][m_pPlayer->GetX()].isHazard())
	{
		m_pPlayer->GetSrcP()->x = 160; // Set tombstone sprite.
		Render(); // Invoke a render before we delay.
		SDL_Delay(2000);
		m_bRunning = false;
	}
	// Door check.
	for (int i = 0; i < m_pLevels[m_iCurrLevel].m_iMaxDoors; i++)
	{
		if (m_pPlayer->GetY() == m_pLevels[m_iCurrLevel].m_Doors[i].GetY() && m_pPlayer->GetX() == m_pLevels[m_iCurrLevel].m_Doors[i].GetX())
		{
			m_pPlayer->SetX(m_pLevels[m_iCurrLevel].m_Doors[i].GetDestX());
			m_pPlayer->SetY(m_pLevels[m_iCurrLevel].m_Doors[i].GetDestY());
			m_iCurrLevel = m_pLevels[m_iCurrLevel].m_Doors[i].GetToLevel();
			break;
		}
	}
	//*/
}

void Engine::RequestStateChange() {
	
	//std::cout << typeid(*(m_sMachin->GetStates().back())).name() << endl;
	if (typeid(*(m_sMachin->GetStates().back())) == typeid(TitleState)) {
		m_pPlayer->Reset(COLS/2, ROWS/2);
		m_sMachin->ChangeState(new GameState());
	}
	else if (typeid(*(m_sMachin->GetStates().back())) == typeid(GameState)) {
		
		if (!m_pPlayer->IsAlive()) { //check for death
			m_sMachin->ChangeState(new LoseState());
		}
		else{
			if (m_pPlayer->HasItem()) {
				m_sMachin->ChangeState(new WinState());
			}
			else { 
				m_sMachin->ChangeState(new PauseState());
			}
		}
	}
	else if (typeid(*(m_sMachin->GetStates().back())) == typeid(PauseState)) {
		m_sMachin->PopState();
	}
	else if (typeid(*(m_sMachin->GetStates().back())) == typeid(WinState) || 
			 typeid(*(m_sMachin->GetStates().back())) == typeid(LoseState)) {
		m_sMachin->Clean();
		m_sMachin->PushState(new TitleState());
	}
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::Render()
{
	/*
	SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	//*/
	m_sMachin->Render();
}

void Engine::Clean()
{
	cout << "Cleaning game." << endl;
	delete m_pPlayer;
	delete [] m_pLevels;
	SDL_DestroyTexture(m_pPlayerText);
	SDL_DestroyTexture(m_pTileText);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	IMG_Quit();
	SDL_Quit();
}