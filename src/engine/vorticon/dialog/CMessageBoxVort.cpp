/*
 * CMessageBoxVort.cpp
 *
 *  Created on: 30.03.2011
 *      Author: gerstrong
 */

#include "CMessageBoxVort.h"
#include "sdl/CVideoDriver.h"
#include "sdl/input/CInput.h"
#include "graphics/CGfxEngine.h"
#include "sdl/extensions.h"
#include "gui/CGUIBitmap.h"

CMessageBoxVort::CMessageBoxVort(const std::string& Text, bool lower, bool keymsg, bool leftbound) :
CMessageBox(Text, lower, keymsg, leftbound)
{}

// This function is used in your Ship need those parts.
// This special Messagebox can hold up to 4 images
void CMessageBoxVort::addTileAt(Uint16 tile, Uint16 x, Uint16 y)
{
	CTilemap &tilemap = g_pGfxEngine->getTileMap(1);

	const int tileDim = tilemap.getDimension();

	// now we'll create a new surface on which the Tile will be printed.
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = rect.h = tileDim;
	SmartPointer<SDL_Surface> bmpSfc = CG_CreateRGBSurface(rect);

	tilemap.drawTile(bmpSfc.get(), 0, 0, tile);

	SDL_FillRect(bmpSfc.get(), NULL, 0x0000FF00);

	rect.x = x;	rect.y = y;

	/*CRect<float> fRect( x, y, 16.0f, 16.0f);

	CRect<float> gameRect = g_pVideoDriver->getGameResolution();
	fRect.transformInverse(gameRect);
	fRect.transform(mRect);*/

	CRect<float> fRect( 0.0f, 0.0f, 1.0f, 1.0f);

	SmartPointer<CBitmap> pBitmap = new CBitmap(bmpSfc);

	addControl( new CGUIBitmap(pBitmap), fRect );
}
