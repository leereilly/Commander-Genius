/*
 * CMenu.h
 *
 *  Created on: 22.04.2010
 *      Author: gerstrong
 */

#ifndef CMENU_H_
#define CMENU_H_

#include <string>
#include "options.h"
#include "CObject.h"
#include "Menu/CBaseMenu.h"

#include "../sdl/CInput.h"
#include "../fileio/CSavedGame.h"

class CMenu : public CBaseMenu
{
public:
	CMenu(char menu_mode, std::string &GamePath,
			 char &Episode, CSavedGame &SavedGame,
			 stOption *pOption, Uint8 DlgTheme );

	bool getChooseGame() { return m_choosegame; }
	bool getExitEvent() { return m_quit; }
	bool restartVideo() { return m_RestartVideo; }

	void videoRestarted() { m_RestartVideo = false; }

	// initialization functions
	void init( char menu_type );
	void initMainMenu();
	void initNumPlayersMenu();
	void initF1Menu();
	void initConfirmMenu(std::string confirmtext = "  Are you certain?  ");
	void initNumControlMenu();
	void initDifficultyMenu();
	void initConfigureMenu();
	void initSaveMenu();
	void initControlMenu();
	void initOptionsMenu();
	void initAudioMenu();
	void initDebugMenu();
	void initModMenu();

	void (CMenu::*processPtr)();

	void process();

	// Process functions
	void processMainMenu();
	void processNumPlayersMenu();
	void processDifficultyMenu();
	void processNumControlMenu();
	void processDebugMenu();
	void processModMenu();
	void processSaveMenu();
	void processLoadMenu();
	void processOverwriteMenu();
	void processQuitMenu();
	void processEndGameMenu();

	void cleanup();

	virtual ~CMenu();

	bool m_demoback;
	bool m_hideobjects;

	CDialog *mp_Dialog;

protected:
	char &m_Episode;
	std::string &m_GamePath;
	CSavedGame &m_SavedGame;
	bool m_RestartVideo;
	stOption *mp_option;
	char m_menu_mode;

	bool m_choosegame;
	bool m_overwrite;
	bool m_goback;
	bool m_goback2;
	bool m_Endgame;
	bool m_quit;
	int m_selection;
	char m_lastselect;
	char m_menu_type;
	char m_NumPlayers;
	char m_Difficulty;
	int  m_saveslot;

	CBaseMenu *mp_Menu;
	std::vector<int> m_menuback;
	std::vector<int> m_menumap;

private:
	std::vector<std::string> m_StateFileList;
};


#endif /* CMENU_H_ */
