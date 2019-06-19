/* STATES AND STATE MACHINE CLASSES
 * These are listed in the following order:
 *		=> State class (abstract)
 *		=> Title State
 *		=> Game State
 *		=> Pause State
 *		=> Win State
 *		=> Lose State
 *		=> State Machine Class
 */

#include <typeinfo> //to include type information

//#include "Classes.h"
#include "States.h"

State::State() {}

State::compl State() {}

void State::Render() {
	SDL_RenderPresent(Game::Instance()->GetRenderer());
}

// TITLE STATE
TitleState::TitleState() {}

TitleState::compl TitleState() {}

void TitleState::Update() {}

void TitleState::Render() {

	m_rSrc = { 0, 0, 1024, 728 };
	m_rDest = { 0, 0, 1024, 728 };
	
	//further implementation goes here
	SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetImage(), &m_rSrc, &m_rDest);
	m_rSrc = { 0, 0, 0x7F, 0x7F };
	m_rDest = { 0x1C1, 0x1A5, 0x7F, 0x7F };
	SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetButton(), &m_rSrc, &m_rDest);
	State::Render();
}

void TitleState::Enter() {}

void TitleState::Exit() {}

void TitleState::Resume() {}


// GAME STATE
GameState::GameState() {}

GameState::compl GameState() {}

void GameState::Update() {}

void GameState::Render() {
	State::Render();
}

void GameState::Enter() {}

void GameState::Exit() {}

void GameState::Resume() {}


//PAUSE STATE
PauseState::PauseState() {}

PauseState::compl PauseState() {}

void PauseState::Update() {}

void PauseState::Render() {
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0x00, 0x00, 0x64, 0x63);
	State::Render();
}

void PauseState::Enter() {}

void PauseState::Exit() {}

void PauseState::Resume() {}

//WIN STATE
WinState::WinState() {}

WinState::compl WinState() {}

void WinState::Update() {}

void WinState::Render() {
	State::Render();
}

void WinState::Enter() {}

void WinState::Exit() {}

void WinState::Resume() {}


//LOSE STATE
LoseState::LoseState() {}

LoseState::compl LoseState() {}

void LoseState::Update() {}

void LoseState::Render() {
	State::Render();
}

void LoseState::Enter() {}

void LoseState::Exit() {}

void LoseState::Resume() {}

//STATE MACHINE
SMachine::SMachine() {
	m_vStates.reserve(1);
}

SMachine::compl SMachine() {}

void SMachine::Update() { if(!m_vStates.empty()) m_vStates.back()->Update(); }

void SMachine::Render() { // Requires refinement for display of the game state
	if (!m_vStates.empty()) { //checks for emptiness
		if (typeid(m_vStates.back()) == typeid(PauseState)) { //if the last state is pause
			// Assuming Pause was overlapped on Game
			//		=> Alternate implementation had type comparisons in a for loop, 
			//			but this approach seems faster
			m_vStates[m_vStates.size() - 2]->Render();
		}
		m_vStates.back()->Render();
	}
}

void SMachine::PushState(State* pState) {
	pState->Enter();
	m_vStates.push_back(pState);
}

void SMachine::ChangeState(State* pState) { //verify again later for accuracy
	if (!m_vStates.empty()) { //emptyness check
		//Game state cannot be popped on pause
		if (typeid(m_vStates.back()) == typeid(GameState)
			&& typeid(*pState) == typeid(PauseState)) { //type comparisons
			PushState(pState);
		}
		else {
			DestroyLastState();
			PushState(pState);
		}
	}
	else {
		PushState(pState);
	}
	
}

void SMachine::PopState() {//check implementation
	if (!m_vStates.empty()) { //emptiness check
		DestroyLastState();
	}
	if (!m_vStates.empty()) { //emptyness check after removal
		m_vStates.back()->Resume();
	}
}

void SMachine::Clean() {
	while (!m_vStates.empty()) {
		DestroyLastState();
	}
}

std::vector<State*>& SMachine::GetStates() { return m_vStates; }

void SMachine::DestroyLastState() {
	if (!m_vStates.empty()) {
		m_vStates.back()->Exit();
		delete m_vStates.back();
		m_vStates.back() = nullptr;
		m_vStates.pop_back();
	}
}