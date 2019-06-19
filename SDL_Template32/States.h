#pragma once

#include <vector>

#include "Engine.h"
#include "Classes.h"
#include "GameObject.h"

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
		SDL_Rect m_rSrc, m_rDest;
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