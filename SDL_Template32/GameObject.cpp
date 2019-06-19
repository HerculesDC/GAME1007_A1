#include "GameObject.h"

GameObject::GameObject(SDL_Rect source, SDL_Rect dest): m_rSrc(source), m_rDst(dest) {}

GameObject::compl GameObject() {}

SDL_Rect  GameObject::GetSrc()  { return m_rSrc; }
SDL_Rect  GameObject::GetDst()  { return m_rDst; }
SDL_Rect* GameObject::GetSrcP() { return &m_rSrc; }
SDL_Rect* GameObject::GetDstP() { return &m_rDst; }

void GameObject::SetSrc(char src) {
	if (src == ',') // Grass.
		m_rSrc = { 0,0,32,32 };
	else if (src == ';') // Tall grass.
		m_rSrc = { 32,0,32,32 };
	else if (src == 'M') // Mountain.
		m_rSrc = { 64,0,32,32 };
	else if (src == 'm') // Foothill.
		m_rSrc = { 96,0,32,32 };
	else if (src == '.') // Dirt.
		m_rSrc = { 128,0,32,32 };
	else if (src == '-') // Sand.
		m_rSrc = { 160,0,32,32 };
	else if (src == '~') // Water.
		m_rSrc = { 192,0,32,32 };
	else if (src == 'X') // Lava.
		m_rSrc = { 224,0,32,32 };
	else if (src == '#') // Tree.
		m_rSrc = { 0,32,32,32 };
	else if (src == 'O') // Cut trunk.
		m_rSrc = { 32,32,32,32 };
	else if (src == '=') // Wood wall.
		m_rSrc = { 64,32,32,32 };
	else if (src == 'H') // Wood roof.
		m_rSrc = { 96,32,32,32 };
	else if (src == 'B') // Stone wall.
		m_rSrc = { 128,32,32,32 };
	else if (src == '_') // Stone floor.
		m_rSrc = { 160,32,32,32 };
	else if (src == 'D') // Note there is no Q.
		m_rSrc = { 192,32,32,32 };
	else if (src == '%') // Skull pile.
		m_rSrc = { 224,32,32,32 };
	else
		m_rSrc = { 0,0,0,0 };
}

void GameObject::SetDest(SDL_Rect dest) { m_rDst = dest; }