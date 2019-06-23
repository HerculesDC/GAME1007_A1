#pragma once
#include <iostream>
#include "Classes.h"
#include "States.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
using namespace std;

//forward declarations
class States;
class SMachine;

typedef class Engine
{
// Attributes.
public:

private:
	bool m_bStarted, m_bRunning;
	const Uint8* m_iKeystates;
	const SDL_MouseButtonEvent* m_mouse;
	Uint32 m_start, m_end, m_delta, m_fps;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	// Game specific attributes.
	SMachine* m_sMachin;
	int m_iCurrLevel;
	Level* m_pLevels; // Pointer to dynamic level array.
	Player* m_pPlayer; // Pointer to dynamic player.
	SDL_Texture* m_oScreensText; //other Screens texture: using a single image...
	SDL_Texture* m_pPlayButtonText;
	SDL_Texture* m_pPlayerText;
	SDL_Texture* m_pTileText;
	Mix_Music* m_pMusic;
	Mix_Chunk* m_pStep;
	TTF_Font* m_pFont;

// Methods.
public:
	void Run(const char*, int, int, int, int, int);
	static Engine* Instance();
	SDL_Texture* GetImage() const { return m_oScreensText; }
	SDL_Texture* GetButton() const { return m_pPlayButtonText; }
	SDL_Texture* GetPlayerText() const { return m_pPlayerText; }
	SDL_Texture* GetTile() const { return m_pTileText; }
	TTF_Font* GetFont() const { return m_pFont; }
	Mix_Music* GetMusic() const { return m_pMusic; }
	Mix_Chunk* GetChunk() const { return m_pStep; }
	Level* GetLevel() const { return m_pLevels; }
	void SetLevel(int index);
	int GetLevelIndex() const { return m_iCurrLevel; }
	Player* GetPlayer() { return m_pPlayer; }
	bool KeyDown(SDL_Scancode c);
	void GetMouse(SDL_Rect r);
	void RequestStateChange();
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	SDL_Window* GetWindow() const { return m_pWindow; }

private:
	Engine();
	~Engine();
	bool Init(const char*, int, int, int, int, int);
	bool Running() { return m_bRunning; };
	void Update();
	void HandleEvents();
	void Wake();
	void Sleep();
	void Render();
	void Clean();
} Game;