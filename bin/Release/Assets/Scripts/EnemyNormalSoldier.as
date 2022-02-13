#include 'RemorseBehaviour.as'

// This script implements the logic for the player Soldier
class EnemyNormalSoldier : RemorseBehaviour
{
	// The constructor must take a GameObject handle as input,
	// this is the object that the script will control
	EnemyNormalSoldier(GameObject @obj)
	{
		// Keep the owner for later reference
		@gameObject = obj;
	}

    /* Called Once when Game or new Scene Started  */
    void Start()
    {
    
    }
    
	void Update()
	{
		// What do the player want to do?
		int dx = 0, dy = 0;
		if( game.actionState[UP] )
			dy--;
		if( game.actionState[DOWN] )
			dy++;
		if( game.actionState[LEFT] )
			dx--;
		if( game.actionState[RIGHT] )
			dx++;
		if( !gameObject.Move(dx,dy) )
		{
			// It wasn't possible to move there.
			// Is there a zombie in front of us?
			// TODO:
		}
	}
	
	void OnMessage(ref @m, const GameObject @sender)
	{
		CMessage @msg = cast<CMessage>(m);
		if( msg !is null && msg.txt == 'Attack' )
		{
			// The zombie got us
			gameObject.Kill();
			game.EndGame(false);
		}
	}
	
	GameObject @gameObject;
}


// These are the actions that the user can do
enum EAction
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3  
}
