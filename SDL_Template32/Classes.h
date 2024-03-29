#pragma once
#include <fstream>
#include "SDL.h"
#include "States.h"
#define ROWS 24
#define COLS 32
#define OTILES 8
#define HTILES 1
using namespace std;

class State;
class SMachine;

/* ',', ';', 'M', 'm', '.', '-', '~', 'X',
   '#', 'O', '=', 'H', 'B', '_', 'Q', '%' */
constexpr char g_cOTiles[OTILES] = { 'M', 'm', '#', 'O', '=', 'H', 'B', '%' };
constexpr char g_cHTiles[HTILES] = { 'X' };

class Sprite
{
protected:
	SDL_Rect m_rSrc, m_rDst;

public:
	Sprite() {}
	Sprite(SDL_Rect s, SDL_Rect d) :m_rSrc(s), m_rDst(d) {}
	SDL_Rect GetSrc() { return m_rSrc; }
	SDL_Rect GetDst() { return m_rDst; }
	SDL_Rect* GetSrcP() { return &m_rSrc; }
	SDL_Rect* GetDstP() { return &m_rDst; }

	void SetSrc(char c)
	{
		if (c == ',') // Grass.
			m_rSrc = { 0,0,32,32 };
		else if (c == ';') // Tall grass.
			m_rSrc = { 32,0,32,32 };
		else if (c == 'M') // Mountain.
			m_rSrc = { 64,0,32,32 };
		else if (c == 'm') // Foothill.
			m_rSrc = { 96,0,32,32 };
		else if (c == '.') // Dirt.
			m_rSrc = { 128,0,32,32 };
		else if (c == '-') // Sand.
			m_rSrc = { 160,0,32,32 };
		else if (c == '~') // Water.
			m_rSrc = { 192,0,32,32 };
		else if (c == 'X') // Lava.
			m_rSrc = { 224,0,32,32 };
		else if (c == '#') // Tree.
			m_rSrc = { 0,32,32,32 };
		else if (c == 'O') // Cut trunk.
			m_rSrc = { 32,32,32,32 };
		else if (c == '=') // Wood wall.
			m_rSrc = { 64,32,32,32 };
		else if (c == 'H') // Wood roof.
			m_rSrc = { 96,32,32,32 };
		else if (c == 'B') // Stone wall.
			m_rSrc = { 128,32,32,32 };
		else if (c == '_') // Stone floor.
			m_rSrc = { 160,32,32,32 };
		else if (c == 'D') // Note there is no Q.
			m_rSrc = { 192,32,32,32 };
		else if (c == '%') // Skull pile.
			m_rSrc = { 224,32,32,32 };
		else
			m_rSrc = { 0,0,0,0 };
	}
	void SetDst(SDL_Rect d) { m_rDst = d; }
};

class Item : public Sprite {
	private:
	public:
};

class PlayButton : public Sprite {
	private:
		//int m_X, m_Y, m_W, m_H;
		
	public:
		PlayButton(SDL_Rect s, SDL_Rect d) :Sprite(s, d) {}
		//Design-wise, the callback is here. It's just not being used...
		void Click(void (*inFunction)(State*), State* targetDest) {
			(*inFunction)(targetDest);
		}
};

class Player : public Sprite
{
private:
	int m_iX, m_iY, m_speed, m_iSprite, m_iMaxSprites, m_iFrame, m_iMaxFrame;
	bool m_alive;
	bool m_hasKey;
	bool m_hasItem;

public:
	Player(SDL_Rect s, SDL_Rect d) :Sprite(s, d) {
		m_speed = 4;
		m_iSprite = 0;
		m_iMaxSprites = 5;
		m_iFrame = 0;
		m_iMaxFrame = 5;
		m_alive = true;
		m_hasKey = false;
		m_hasItem = false;
	}
	void Reset(int x, int y) {
		//resets position to whatever the engine determines
		SetX(x);
		SetY(y);
		//resets sprite
		this->m_rSrc.x = 0;
		//resets control variables (life, key, item)
		m_alive = true;
		m_hasKey = false;
		m_hasItem = false;
	}
	int GetX() { return m_iX; }
	int GetY() { return m_iY; }
	int GetSpeed()const { return m_speed; }
	bool IsAlive()const { return m_alive; }
	bool HasKey()const { return m_hasKey; }
	bool HasItem()const { return m_hasItem; }
	void Die() {
		m_rSrc.x = 160;
		m_alive = false;
	}
	void GetKey() { m_hasKey = true; }
	void GetItem() { m_hasItem = true; }
	void SetX(int x) 
	{ 
		m_iX = x; 
		m_rDst.x = x * 32;
	}
	void SetY(int y)
	{ 
		m_iY = y; 
		m_rDst.y = y * 32;
	}
	void MoveX(int x) { 
		if ((m_rDst.x += x) % 32 == 0) m_iX = m_rDst.x / 32;
		cout << (int)(m_rDst.x + (0.5 * m_rDst.w)) << ", " << m_iX << endl;
	}
	void MoveY(int y) {
		if((m_rDst.y += y)%32==0) m_iY = m_rDst.y/32;
		cout << (int)(m_rDst.y + (0.5 * m_rDst.h)) << ", " << m_iY << endl;
	}
	void Animate() { //taken straight from the asteroids example, with adapted names.
		m_iFrame++;
		if (m_iFrame == m_iMaxFrame) {
			m_iFrame = 0;
			m_iSprite++;
			if (m_iSprite == m_iMaxSprites)
				m_iSprite = 0;
		}
	}
	void Update() {
		m_rSrc.x = m_iFrame * 32;
	}
};

class Tile : public Sprite
{
private:
	bool m_bIsObstacle, m_bIsHazard;

public:
	Tile(): m_bIsObstacle(false), m_bIsHazard(false) {}
	bool isObstacle() { return m_bIsObstacle; }
	bool isHazard() { return m_bIsHazard; }
	void SetObstacle(bool b) { m_bIsObstacle = b; }
	void SetHazard(bool b) { m_bIsHazard = b; }
	
};

class Door : public Sprite
{
private:
	int m_iX, m_iY, m_iToLevel, m_iDestX, m_iDestY;
	bool m_IsLocked;

public:
	Door()
	{
		SetSrc('D');
		SetDst({ 0,0,32,32 });
	}
	int GetX() { return m_iX; }
	int GetY() { return m_iY; }
	int GetToLevel() { return m_iToLevel; }
	int GetDestX() { return m_iDestX; }
	int GetDestY() { return m_iDestY; }
	void SetToLevel(int l) { m_iToLevel = l; }
	void SetDestX(int x) { m_iDestX = x; }
	void SetDestY(int y) { m_iDestY = y; }
	void SetDoor(const int x, const int y, const int toLevel, const int destX, const int destY)
	{
		m_IsLocked = false; //hard-coding unlocked doors. Tests will come later
		m_iX = x;
		m_rDst.x = x * 32;
		m_iY = y;
		m_rDst.y = y * 32;
		m_iToLevel = toLevel;
		m_iDestX = destX;
		m_iDestY = destY;
		cout << "Created door at " << x << "," << y << " to " << toLevel << endl;
	}
	bool IsLocked() const {return m_IsLocked;}
	bool SetLock(bool lockState) { m_IsLocked = lockState; }
};

class Level
{
private:
	void SetLevel(int d)
	{
		m_iNumDoors = 0;
		m_iMaxDoors = d;
		m_Doors = new Door[m_iMaxDoors];
		cout << "Created level with " << m_iMaxDoors << " doors!" << endl;
	}
public: // Properties.
	int m_iNumDoors, m_iMaxDoors;
	Door* m_Doors;
	Tile m_Map[24][32];

public: // Methods.
	Level()	{ SetLevel(0); } // Default constructor. Not used.
	Level(int d, const char* n) // Non-default constructor.
	{ 
		SetLevel(d);
		ifstream inFile(n); // We are building the map here!
		for (int row = 0; row < ROWS; row++) {
			for (int col = 0; col < COLS; col++) {
				char temp;
				inFile >> temp;
				m_Map[row][col].SetSrc(temp);
				m_Map[row][col].SetDst({ col*32, row*32, 32, 32 });
				for (int i = 0; i < OTILES; i++)
				{
					if (g_cOTiles[i] == temp)
						m_Map[row][col].SetObstacle(true);
				}
				for (int i = 0; i < HTILES; i++)
				{
					if (g_cHTiles[i] == temp)
						m_Map[row][col].SetHazard(true);
				}
				if (temp == 'Q')
				{
					int tl, px, py;
					inFile >> tl >> px >> py;
					AddDoor(col, row, tl, px, py);
				}
			}
		}
		inFile.close();
	} 
	~Level() { delete[] m_Doors; }
	void AddDoor(const int x, const int y, const int toLevel, const int destX, const int destY)
	{
		if (m_iNumDoors < m_iMaxDoors)
			m_Doors[m_iNumDoors++].SetDoor(x, y, toLevel, destX, destY);
	}
};