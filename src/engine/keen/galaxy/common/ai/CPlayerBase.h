/*
 * CPlayerBase.h
 *
 *  Created on: 19.06.2011
 *      Author: gerstrong
 *
 *  This is the base class for the Player. This class is very new and I thought of it for
 *  the following reason. We have Keen on the Map in level and when diving.
 *  Those AIs are different of course, but still many things in common like dying
 *  (See the puzzle Keen 5 mod versions by Mink) and collecting items.
 *  It will not only reduce the code of some repeating routines, but also
 *  provide better overview over the whole thing.
 */

#ifndef CPLAYERBASE_H_
#define CPLAYERBASE_H_

#include "../CGalaxySpriteObject.h"
#include "../CInventory.h"

#include "engine/core/Playerdefines.h"
#include <base/GsEvent.h>
#include "engine/core/Cheat.h"
#include "engine/core/CCamera.h"


struct EventEnterLevel : CEvent {
    uint16_t data;
    EventEnterLevel(const uint16_t d) : data(d) {}
};




struct EventExitLevel : CEvent {
    const uint16_t levelObject;
    const bool sucess;
    const bool teleport;
    const int who;
    EventExitLevel(const uint16_t l, const bool s, const bool t, const bool lWho) :
        levelObject(l), sucess(s), teleport(t), who(lWho) {}
};

struct EventExitLevelWithFoot : CEvent {
    const uint16_t levelObject;
    const int who;
    EventExitLevelWithFoot(const uint16_t l, const int lWho) : levelObject(l), who(lWho) {}
};


struct EventDieKeenPlayer  : CEvent
{
    const int playerID;
    const bool gameOver;
    const int levelObj;
    const std::string levelName;

    EventDieKeenPlayer(const int id, const bool go, const int lo, const std::string &ln) :
        playerID(id), gameOver(go),
        levelObj(lo), levelName(ln){}
};


namespace galaxy
{


enum PLAYER_ACTIONS
{
A_KEEN_STAND = 0,
A_KEEN_ON_PLAT,
A_KEEN_BORED,
A_KEEN_QUESTION,
A_KEEN_MOON,
A_KEEN_BOOK_OPEN,
A_KEEN_BOOK_READ,
A_KEEN_BOOK_CLOSE,
A_KEEN_LOOKUP,
A_KEEN_LOOKDOWN,
A_KEEN_ACTION_1,
A_KEEN_ACTION_2,
A_KEEN_ACTION_3,
A_KEEN_DIE,
A_KEEN_SHOOT,
A_KEEN_SHOOT_UP,
A_KEEN_ACTION_4,
A_KEEN_SLIDE,
A_KEEN_ENTER_DOOR,
A_KEEN_POLE,
A_KEEN_POLE_CLIMB,
A_KEEN_POLE_SLIDE,
A_KEEN_POLE_SHOOT,
A_KEEN_POLE_SHOOTUP,
A_KEEN_POLE_SHOOTDOWN,
A_KEEN_RUN,
A_KEEN_POGO_START,
A_KEEN_POGO_UP,
A_KEEN_POGO_HIGH,
A_KEEN_JUMP,
A_KEEN_JUMP_DOWN,
A_KEEN_FALL,
A_KEEN_JUMP_SHOOT,
A_KEEN_JUMP_SHOOTUP,
A_KEEN_JUMP_SHOOTDOWN,
A_KEEN_HANG,
A_KEEN_CLIMB,
A_KEEN_RIDING_ON_FOOT,
A_KEEN_STUNNED,
NUM_KEEN_ACTIONS
};


class CPlayerBase : public CGalaxySpriteObject
{
public:
	/**
	 * \description Basic Constructor. An Object always need the pointer to the Map of the level,
	 * 				Coordinates as also the type of the Object
	 */
    CPlayerBase(CMap *pmap,
                const Uint16 foeID,
                Uint32 x,
                Uint32 y,
                direction_t facedir,
                CInventory &l_Inventory,
                stCheat &Cheatmode,
                int playerID);


    /**
	 * Pushes the players by the given object Reference
	 */
	void push( CGalaxySpriteObject &theObject );

	/**
	 * Overloaded getActionStatus which uses the episodes' map
	 */
	bool getActionStatus(int16_t ActionNumber);

	/**
	 * returns the Sprite ID from the given action
	 */
	int getSpriteIDFromAction(const int16_t ActionNumber);

	/**
	 * Overloaded setActionForce which uses the action for the player
	 */
	void setActionForce(const size_t ActionNumber);

	/**
	 * \brief The Player will get 1 UP when that function is launched
	 */
	void getAnotherLife(const int lc_x, const int lc_y, const bool display, const bool alt);

    virtual void pumpEvent(const CEvent *evPtr);

    /**
     * \description Camera of the player gets performed here...
     */
    void processCamera();

    /**
     * @brief Player can always be visible and active, even if he is not seen.
     * @return
     */
    bool calcVisibility();

	/**
	 * \description Read the Input of the Player and sets the variables accordingly
	 */
	virtual void processInput();


	void processLevelMiscFlagsCheck();

	/**
	 * Produces the walking sound. To produce that sound correctly, it uses a special timer.
	 * That's the reason we need that aiding function
	 */
	void playWalkSound();


	/**
	 * Make keen move with x/y speed move towards a target which is held in the variable "mTarget"
	 * it only moves towards, this routines is exec'd several times in order achieve a smooth transition.
	 */
    void guideToTarget(const Vector2D<int> &speed);


	/**
	 * Same here but with swimming
	 */
	void playSwimSound();

    void respawnImportantItem(const int itemId);

    bool touchedBottomOfMap();

	void processDead();
	virtual void processDying();
	virtual void kill(const bool force = false);

	/**
	 * Happens when usually the Dopefish eats Keen
	 */
	void getEaten();

	/**
	 * Process the cycle when keen is getting eaten
	 */
	void processGetEaten();

	CInventory &m_Inventory;

	CCamera m_camera;
	
	bool m_dying;

protected:

	bool checkMapBoundaryL(const int x1);
	bool checkMapBoundaryR(const int x2);
	bool checkMapBoundaryU(const int y1);

	unsigned short mPlayerNum;	

	Sint16 m_playcontrol[PA_MAX_ACTIONS];
	int m_timer;
	

	stCheat& m_Cheatmode;


	void (CPlayerBase::*mp_processState)();
	unsigned char m_walktimer;
	std::map< size_t, void (CPlayerBase::*)() > mActionMap;

	// Assignment for the same player for all the episodes
	static const int mEpisodeActionNumMap[3][NUM_KEEN_ACTIONS];

    Vector2D<int> mTarget;
};

};

#endif /* CPLAYERBASE_H_ */