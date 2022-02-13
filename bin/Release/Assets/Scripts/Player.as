// This script implements the logic for the Player 
class Player : IRemorseBehaviour
{
	// The constructor must take a GameObject handle as input,
	// this is the object that the script will control
	Player(GameObject @obj)
	{
		// Keep the owner attached GameObject for later reference
		@gameObject = obj;
	}

	GameObject @gameObject;
    Vector2f enemyVec;
    GameObject @enemy;
    
    Music myMusic;

    /* Called Once when Game or new Scene Started  */
    void Start()
    {
        // for now we set the File Manually(with Path)
        myMusic.OpenFromFile("Assets/Audio/canary.wav");
        myMusic.SetLoop(true);
        myMusic.Play();
        
        @enemy = game.GetGameObjectByName("Enemy");
        enemyVec.x = 0.008f;

    }
    /* Called every frame */
	void Update()
	{
		if(Keyboard::IsKeyPressed(Keyboard::Key::A) )
        {
            gameObject.transform.Move( -0.4f, 0 );
        }
        if(Keyboard::IsKeyPressed(Keyboard::Key::D) )
        {
            gameObject.transform.Move( 0.4f, 0 );
        }
        if(Keyboard::IsKeyPressed(Keyboard::Key::W) )
        {
            gameObject.transform.Move( 0, -0.4f );
        }
        if(Keyboard::IsKeyPressed(Keyboard::Key::S) )
        {
            gameObject.transform.Move( 0, 0.4f );
        }
        enemy.transform.Move( enemyVec );
        enemy.transform.Rotate( 0.09f );
	}
}
