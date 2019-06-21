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

#include "States.h"

State::State() {}

State::compl State() {}

void State::Render() {
	SDL_RenderPresent(Game::Instance()->GetRenderer());
}

// TITLE STATE
TitleState::TitleState() {
	
	SDL_Rect tempSrc, tempDest;
		tempSrc.x = 0x00;
		tempSrc.y = 0x00;
		tempSrc.w = 0x80;
		tempSrc.h = 0x80;
		
		tempDest.x = 0x1C1;
		tempDest.y = 0x1A5;
		tempDest.w = 0x70;
		tempDest.h = 0x80;
		 
	pButton = new PlayButton(tempSrc, tempDest);
}

TitleState::compl TitleState() {
	delete pButton;
	pButton = nullptr;
}

void TitleState::Update() {
	Game::Instance()->GetMouse(pButton->GetDst());
}

void TitleState::Render() {

	m_rSrc = { 0, 0, 1024, 728 };
	m_rDest = { 0, 0, 1024, 728 };
	
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(Game::Instance()->GetRenderer());
	//further implementation goes here
	SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetImage(), &m_rSrc, &m_rDest);
	
	SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetButton(), pButton->GetSrcP(), pButton->GetDstP());
	State::Render();
}

void TitleState::Enter() {}

void TitleState::Exit() {}

void TitleState::Resume() {}

// GAME STATE
GameState::GameState() {}

GameState::compl GameState() {}

void GameState::Update() {

	//Pause Game

	if (Game::Instance()->KeyDown(SDL_SCANCODE_P)) {
		Game::Instance()->RequestStateChange();
	}

	// Move the player.
	if (Game::Instance()->KeyDown(SDL_SCANCODE_W) && 
		Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Map[Game::Instance()->GetPlayer()->GetY() - 1][Game::Instance()->GetPlayer()->GetX()].isObstacle() == false)
	{
		Game::Instance()->GetPlayer()->SetY(Game::Instance()->GetPlayer()->GetY() - 1);
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_S) && Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Map[Game::Instance()->GetPlayer()->GetY() + 1][Game::Instance()->GetPlayer()->GetX()].isObstacle() == false)
	{
		Game::Instance()->GetPlayer()->SetY(Game::Instance()->GetPlayer()->GetY() + 1);
	}
	if (Game::Instance()->KeyDown(SDL_SCANCODE_A) && Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Map[Game::Instance()->GetPlayer()->GetY()][Game::Instance()->GetPlayer()->GetX() - 1].isObstacle() == false)
	{
		Game::Instance()->GetPlayer()->SetX(Game::Instance()->GetPlayer()->GetX() - 1);
	}
	else if (Game::Instance()->KeyDown(SDL_SCANCODE_D) && Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Map[Game::Instance()->GetPlayer()->GetY()][Game::Instance()->GetPlayer()->GetX() + 1].isObstacle() == false)
	{
		Game::Instance()->GetPlayer()->SetX(Game::Instance()->GetPlayer()->GetX() + 1);
	}
	// Hazard check.
	if (Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Map[Game::Instance()->GetPlayer()->GetY()][Game::Instance()->GetPlayer()->GetX()].isHazard())
	{
		Game::Instance()->GetPlayer()->GetSrcP()->x = 160; // Set tombstone sprite.
		Render(); // Invoke a render before we delay.
		Game::Instance()->GetPlayer()->Die();
		Game::Instance()->RequestStateChange();
		//SDL_Delay(2000);
		//m_bRunning = false;//REQUIRES A DIFFERENT APPROACH, CAUSE IT'LL HAVE TO TRIGGER A STATE CHANGE TO LOSESTATE
	}
	// Door check.
	for (int i = 0; i < Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_iMaxDoors; i++)
	{
		if (Game::Instance()->GetPlayer()->GetY() == Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Doors[i].GetY() && Game::Instance()->GetPlayer()->GetX() == Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Doors[i].GetX())
		{
			Game::Instance()->GetPlayer()->SetX(Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Doors[i].GetDestX());
			Game::Instance()->GetPlayer()->SetY(Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Doors[i].GetDestY());
			Game::Instance()->SetLevel(Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Doors[i].GetToLevel());
			break;
		}
	}
}

void GameState::Render() {

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(Game::Instance()->GetRenderer());
	// Render the map.
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLS; col++)
		{
			SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetTile(), 
						   Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Map[row][col].GetSrcP(), 
						   Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Map[row][col].GetDstP());
		}
	}
	
	// Render the doors. Note, if we didn't, we'd just see a black square.
	for (int i = 0; i < Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_iMaxDoors; i++)
		SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetTile(), 
					   Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Doors[i].GetSrcP(), 
					   Game::Instance()->GetLevel()[Game::Instance()->GetLevelIndex()].m_Doors[i].GetDstP());
	
	// Render the player.
	SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetPlayerText(), Game::Instance()->GetPlayer()->GetSrcP(), Game::Instance()->GetPlayer()->GetDstP());
	State::Render();
}

void GameState::Enter() {}

void GameState::Exit() {}

void GameState::Resume() {}


//PAUSE STATE
PauseState::PauseState() {
	SDL_Rect tempSrc, tempDest;
	tempSrc.x = 0x00;
	tempSrc.y = 0x00;
	tempSrc.w = 0x80;
	tempSrc.h = 0x80;

	tempDest.x = 0x1C1;
	tempDest.y = 0x1A5;
	tempDest.w = 0x70;
	tempDest.h = 0x80;

	pButton = new PlayButton(tempSrc, tempDest);
	
	/*
	m_overlay.x = 0; m_overlay.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &m_overlay.w, &m_overlay.h);
	//*/
}

PauseState::compl PauseState() {
	delete pButton;
	pButton = nullptr;
}

void PauseState::Update() {
	Game::Instance()->GetMouse(pButton->GetDst());
}

void PauseState::Render() {
	//the two lines below need to be used together for the alpha channel to be properly blended
	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0x00, 0x00, 0x64, 0x32);
	SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	//this line renders to the whole renderer, since the second argument passed is a nullptr
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), nullptr);

	SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetButton(), pButton->GetSrcP(), pButton->GetDstP());
	State::Render();
}

void PauseState::Enter() {}

void PauseState::Exit() {}

void PauseState::Resume() {}

//WIN STATE
WinState::WinState() {
	SDL_Rect tempSrc, tempDest;
	tempSrc.x = 0x00;
	tempSrc.y = 0x00;
	tempSrc.w = 0x80;
	tempSrc.h = 0x80;

	tempDest.x = 0x1C1;
	tempDest.y = 0x1A5;
	tempDest.w = 0x70;
	tempDest.h = 0x80;

	pButton = new PlayButton(tempSrc, tempDest);
	/*
	m_overlay.x = 0; m_overlay.y = 0;
	SDL_GetWindowSize(Game::Instance()->GetWindow(), &m_overlay.w, &m_overlay.h);
	//*/
}

WinState::compl WinState() {
	delete pButton;
	pButton = nullptr;
}

void WinState::Update() {
	Game::Instance()->GetMouse(pButton->GetDst());
}

void WinState::Render() {//reminder: Win/Loss DO NOT render overlays (as of now)

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0x00, 0x00, 0xFF, 0xA0);
	//SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), nullptr);

	SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetButton(), pButton->GetSrcP(), pButton->GetDstP());
	State::Render();
}

void WinState::Enter() {}

void WinState::Exit() {}

void WinState::Resume() {}


//LOSE STATE
LoseState::LoseState() {
	
	SDL_Rect tempSrc, tempDest;
	tempSrc.x = 0x00;
	tempSrc.y = 0x00;
	tempSrc.w = 0x80;
	tempSrc.h = 0x80;

	tempDest.x = 0x1C1;
	tempDest.y = 0x1A5;
	tempDest.w = 0x70;
	tempDest.h = 0x80;

	pButton = new PlayButton(tempSrc, tempDest);
}

LoseState::compl LoseState() {
	delete pButton;
	pButton = nullptr;
}

void LoseState::Update() {
	Game::Instance()->GetMouse(pButton->GetDst());
}

void LoseState::Render() {//reminder: Win/Loss DO NOT render overlays (as of now)

	SDL_SetRenderDrawColor(Game::Instance()->GetRenderer(), 0xA0, 0x00, 0x00, 0xFF);
	//SDL_SetRenderDrawBlendMode(Game::Instance()->GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(Game::Instance()->GetRenderer(), nullptr);

	SDL_RenderCopy(Game::Instance()->GetRenderer(), Game::Instance()->GetButton(), pButton->GetSrcP(), pButton->GetDstP());
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
		m_vStates.shrink_to_fit();
		if (typeid(*(m_vStates.back())) == typeid(PauseState)) { //if the last state is pause
			// Assuming Pause was overlapped on Game
			//		=> Alternate implementation had type comparisons in a for loop, 
			//			but this approach seems faster
			for (int i = 0; i < m_vStates.size(); ++i) {
				m_vStates[i]->Render();
			}
		}
		else {
			m_vStates.back()->Render();
		}
		
	}
}

void SMachine::PushState(State* pState) {
	pState->Enter();
	m_vStates.push_back(pState);
}

void SMachine::ChangeState(State* pState) { //verify again later for accuracy
	if (!m_vStates.empty()) { //emptyness check
		//Game state cannot be popped on pause
		if (typeid(*(m_vStates.back())) == typeid(GameState)
			&& typeid(*(pState)) == typeid(PauseState)) { //type comparisons	
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
		m_vStates.shrink_to_fit();
	}
}