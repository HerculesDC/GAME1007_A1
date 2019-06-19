#pragma once

#include "SDL.h"

/* This classs will operate on a set of assumptions:
 *		=> First of all, that the transforms will be inherently part of them
 *		=> Second, that all of them will also have images associated with them
 */
class GameObject {
	public:
		GameObject() {/*empty default constructor*/}
		GameObject(SDL_Rect spriteSource, SDL_Rect screenDest);
		compl GameObject();
		SDL_Rect GetSrc();
		SDL_Rect GetDst();
		SDL_Rect* GetSrcP();
		SDL_Rect* GetDstP();
		void SetSrc(char src);
		void SetDest(SDL_Rect dest);

	protected:
		SDL_Rect m_rSrc, m_rDst;
};