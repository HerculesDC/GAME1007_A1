#include <typeinfo>
#include "Engine.h"
#define FPS 30 // A '.0' will make it a double.
#define MIX_STEREO_SOUND 2 //for the audio mix
#define MIX_AUDIO_SIZE 4096
#define MIX_AUDIO_CHANNELS 16

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
					SDL_Surface* pKeySurf = IMG_Load("keys.png");
					SDL_Surface* pCrystalSurf = IMG_Load("crystals.png");

					m_oScreensText = SDL_CreateTextureFromSurface(m_pRenderer, pScreensSurf);
					m_pPlayButtonText = SDL_CreateTextureFromSurface(m_pRenderer, pPlayButtonSurf);
					m_pPlayerText = SDL_CreateTextureFromSurface(m_pRenderer, pPlayerSurf);
					m_pTileText = SDL_CreateTextureFromSurface(m_pRenderer, pTileSurf);
					m_pKeyText = SDL_CreateTextureFromSurface(m_pRenderer, pKeySurf);
					m_pGemText = SDL_CreateTextureFromSurface(m_pRenderer, pCrystalSurf);
					
					SDL_FreeSurface(pScreensSurf);
					SDL_FreeSurface(pPlayButtonSurf);
					SDL_FreeSurface(pPlayerSurf);
					SDL_FreeSurface(pTileSurf);
					SDL_FreeSurface(pKeySurf);
					SDL_FreeSurface(pCrystalSurf);
					cout << "Image creation success!" << endl;
				}
				else return false; // Img(Png) init fail. The JPG library is not available in this build
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	
	if (Mix_Init(MIX_INIT_MP3) != 0) {
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_STEREO_SOUND, MIX_AUDIO_SIZE);
		Mix_AllocateChannels(MIX_AUDIO_CHANNELS);
		m_pMusic = Mix_LoadMUS("Summer.mp3");
		m_pStep = Mix_LoadWAV("asphalt.wav");
	}
	else return false; //audio init fail
	
	// Continue here a, AUDIO_S16SYS, 2, fter successful init.
	TTF_Init(); //Font initialization. Returns an int, that won't be used right now
	m_pFont = TTF_OpenFont("caveman.ttf", 0x32);
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
	m_sMachin->Render();
}

void Engine::Clean()
{
	cout << "Cleaning game." << endl;
	//pointer deallocating
	delete m_pPlayer;
	delete [] m_pLevels;
	//font cleanup
	TTF_CloseFont(m_pFont);
	//audio cleanup
	Mix_FreeChunk(m_pStep);
	Mix_FreeMusic(m_pMusic);
	Mix_CloseAudio();
	//cleaning textures
	SDL_DestroyTexture(m_pPlayerText);
	SDL_DestroyTexture(m_pTileText);
	SDL_DestroyTexture(m_pPlayButtonText);
	SDL_DestroyTexture(m_oScreensText);
	SDL_DestroyTexture(m_pKeyText);
	SDL_DestroyTexture(m_pGemText);
	//deallocating video resources
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	//Quitting routines
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}