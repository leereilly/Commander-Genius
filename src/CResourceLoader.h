/*
 * CResourceLoader.h
 *
 *  Created on: 23.10.2010
 *      Author: gerstrong
 *
 * This class will handle the Loadscreen when something is loaded and the user
 * has to wait. It's only duty is to manage another thread where the stuff is loaded, while the
 * user sees a loading screen.
 */

#ifndef CRESOURCELOADER_H_
#define CRESOURCELOADER_H_

#include "CSingleton.h"
#include <SDL_thread.h>
#define g_pResourceLoader CResourceLoader::Get()

enum ProgressStyle
{
	TEXT,
	BITMAP
};

class CResourceLoader : public CSingleton<CResourceLoader> {
public:
	CResourceLoader();

	void setStyle(ProgressStyle style);

	void startLoadingSequence();
	void processThread();
    void finishLoadingSequence();

	void setPermilage(int permil);

	virtual ~CResourceLoader();

private:
	void renderLoadingGraphic();

	bool m_threadrunning;
	int m_permil;
	SDL_Thread *mp_thread;
	ProgressStyle m_style;
};

#endif /* CRESOURCELOADER_H_ */
