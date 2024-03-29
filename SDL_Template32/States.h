#pragma once

#include <vector>
#include "Engine.h"
#include "Classes.h"
#include "GameObject.h"

class PlayButton;
class Sprite;

class State {//abstract class
	public:
		State();
		compl State();
		virtual void Update() = 0;
		virtual void Render();
		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual void Resume() = 0;

	private:
};

//technically, each state needs to be a singleton.
class TitleState : public State {
	public:
		TitleState();
		compl TitleState();
		void Update() override;
		void Render() override;
		void Enter() override;
		void Exit() override;
		void Resume() override;

	private:
		//text variables
		SDL_Texture* m_tText; //texture to render text.
		const char* m_text; //message
		SDL_Rect m_rText; //text location. 
						  //Defining 0 for width and height causes the whole text to be rendered
		SDL_Color m_tColor; //text color
				
		SDL_Rect m_rSrc, m_rDest;
		PlayButton*  pButton;
};

class GameState : public State {
	public:
		GameState();
		compl GameState();
		void Update() override;
		void Render() override;
		void Enter() override;
		void Exit() override;
		void Resume() override;

	private:
		bool CheckCollision(Sprite* bb1, Sprite * bb2); //opting for sprites
};

class PauseState : public State {
	public:
		PauseState();
		compl PauseState();
		void Update() override;
		void Render() override;
		void Enter() override;
		void Exit() override;
		void Resume() override;

	private:
		SDL_Texture* m_tText;
		const char* m_text;
		SDL_Rect m_rText;
		SDL_Color m_tColor;

		SDL_Rect m_rSrc, m_rDest; 
		/* originally, there was an SDL_Rect m_overlay as well
			that was dropped when I discovered you can pass null
			to SDL_FillRect to fill the whole rendering area
		//*/
		PlayButton* pButton;
};

class WinState : public State {
	public:
		WinState();
		compl WinState();
		void Update() override;
		void Render() override;
		void Enter() override;
		void Exit() override;
		void Resume() override;

	private:
		SDL_Texture* m_tText;
		const char* m_text;
		SDL_Rect m_rText;
		SDL_Color m_tColor;

		SDL_Rect m_rSrc, m_rDest;
		PlayButton* pButton;
};

class LoseState : public State {
	public:
		LoseState();
		compl LoseState();
		void Update() override;
		void Render() override;
		void Enter() override;
		void Exit() override;
		void Resume() override;

	private:		
		SDL_Texture* m_tText;
		const char* m_text;
		SDL_Rect m_rText; 
		SDL_Color m_tColor;

		SDL_Rect m_rSrc, m_rDest;
		PlayButton* pButton;
};

class SMachine {
	public:
		SMachine();
		compl SMachine();
		void Update();
		void Render();
		void PushState(State* pState);
		void ChangeState(State* pState);
		void PopState();
		void Clean();
		std::vector<State*>& GetStates();

	private:
		void DestroyLastState();

	private:
		std::vector<State*> m_vStates;
};