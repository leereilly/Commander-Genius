#include "CNospike.h"
#include <misc.h>

#include <engine/galaxy/common/ai/CPlayerLevel.h>


/*
$2460W  #Nospike sit 0
$247EW  #Nospike walks 1
$249CW  #Nospike walks
$24BAW  #Nospike walks
$24D8W  #Nospike walks
$24F6W  #Nospike charges 5
$2514W  #Nospike charges
$2532W  #Nospike charges
$2550W  #Nospike charges
$256EW  #Nospike standing (Pause before ?) 9
$258CW  #Nospike start ? 
$25AAW  #Nospike ?
$25C8W  #Nospike fall [Will not hit ground unless it has Nospike property] 12
$25E6W  #Stunned Nospike  13
*/
namespace galaxy {
  

enum NOSPIKEACTIONS
{
A_NOSPIKE_SIT = 0,
A_NOSPIKE_WALK = 1,	// Ordinary walk action 
A_NOSPIKE_CHARGE = 5,
A_NOSPIKE_STAND = 9,
A_NOSPIKE_FALL = 12,
A_NOSPIKE_STUNNED = 13
};


const int WALK_SPEED = 30;
const int CHARGE_SPEED = 60;

const int TIME_UNTIL_WALK = 120;
const int TIME_UNTIL_SIT = 120;

const int CSF_DISTANCE_TO_CHARGE = 6<<CSF;


CNospike::CNospike(CMap* pmap, const Uint16 foeID, const Uint32 x, const Uint32 y) : 
CStunnable(pmap, foeID, x, y),
mHealth(4),
mTimer(0),
mKeenAlignment(LEFT),
mGoodChargeChance(false)
{
    mActionMap[A_NOSPIKE_SIT] = (void (CStunnable::*)()) &CNospike::processSitting;
    mActionMap[A_NOSPIKE_WALK] = (void (CStunnable::*)()) &CNospike::processWalking;
    mActionMap[A_NOSPIKE_CHARGE] = (void (CStunnable::*)()) &CNospike::processCharging;
    mActionMap[A_NOSPIKE_STAND] = (void (CStunnable::*)()) &CNospike::processStanding;
    //mActionMap[A_NOSPIKE_FALL] = (void (CStunnable::*)()) &CNospike::processWalking;
    mActionMap[A_NOSPIKE_STUNNED] = &CStunnable::processGettingStunned;
    
    setupGalaxyObjectOnMap(0x2460, A_NOSPIKE_SIT);
    
    xDirection = LEFT;
}



void CNospike::processSitting()
{
    if(getActionStatus(A_NOSPIKE_WALK))
    {
	setAction(A_NOSPIKE_WALK);
    }
    
    mTimer++;
    if( mTimer < TIME_UNTIL_WALK )  
	return;
    
    mTimer = 0;
    
    setAction(A_NOSPIKE_WALK);    
}


void CNospike::processWalking()
{
    // Move normally in the direction
    if( xDirection == RIGHT )
    {
	moveRight( WALK_SPEED );
    }
    else
    {
	moveLeft( WALK_SPEED );
    }
    
    if(mGoodChargeChance)      
    {
      xDirection = mKeenAlignment;
      setAction(A_NOSPIKE_CHARGE);
    }
    
    mTimer++;
    if( mTimer < TIME_UNTIL_SIT )  
	return;
    
    mTimer = 0;
    
    setAction(A_NOSPIKE_SIT);     
}

void CNospike::processCharging()
{
    // Move normally in the direction
    if( xDirection == RIGHT )
    {
	moveRight( CHARGE_SPEED );
    }
    else
    {
	moveLeft( CHARGE_SPEED );
    }
    
    if(!mGoodChargeChance)      
    {
      xDirection = mKeenAlignment;
      setAction(A_NOSPIKE_WALK);
    }
    
    mTimer++;
    if( mTimer < TIME_UNTIL_WALK )  
	return;
    
    mTimer = 0;
    
    setAction(A_NOSPIKE_WALK); 
}

void CNospike::processStanding()
{

}



bool CNospike::isNearby(CSpriteObject& theObject)
{
	if( !getProbability(10) )
		return false;

	if( CPlayerLevel *player = dynamic_cast<CPlayerLevel*>(&theObject) )
	{
		if( player->getXMidPos() < getXMidPos() )
			mKeenAlignment = LEFT;
		else
			mKeenAlignment = RIGHT;
		
		
		const int objX = theObject.getXMidPos();
		const int objY = theObject.getYMidPos();
		const int nospikeX = getXMidPos();
		const int nospikeY = getYMidPos();
		
		mGoodChargeChance = false;
		
		if( objX < nospikeX - CSF_DISTANCE_TO_CHARGE ||
			objX > nospikeX + CSF_DISTANCE_TO_CHARGE )
			return false;

		if( objY < nospikeY - CSF_DISTANCE_TO_CHARGE ||
			objY > nospikeY + CSF_DISTANCE_TO_CHARGE )
			return false;
		
		mGoodChargeChance = true;
	}

	return true;
}



void CNospike::getTouchedBy(CSpriteObject& theObject)
{
    if(dead || theObject.dead)
	return;
    
    CStunnable::getTouchedBy(theObject);
    
    // Was it a bullet? Then loose health.
    if( dynamic_cast<CBullet*>(&theObject) )
    {
	mHealth--;
	theObject.dead = true;
	
	if(mHealth == 0)
	{
	    setAction(A_NOSPIKE_STUNNED);
	    dead = true;
	}
	else
	{
	    blink(10);
	}
    }
    
    if( CPlayerBase *player = dynamic_cast<CPlayerBase*>(&theObject) )
    {
	player->kill();
    }      
}



int CNospike::checkSolidD(int x1, int x2, int y2, const bool push_mode)
{
    turnAroundOnCliff( x1, x2, y2 );
    
    return CGalaxySpriteObject::checkSolidD(x1, x2, y2, push_mode);
}



void CNospike::process()
{
    performCollisions();
    
    performGravityHigh();
    
    if( blockedl )
	xDirection = RIGHT;
    else if(blockedr)
	xDirection = LEFT;
    
    if(!processActionRoutine())
	exists = false;
    
    (this->*mp_processState)();        
}



};