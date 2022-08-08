
GameManager::GameManager()
{
    
}
GameManager::~GameManager()
{
    
}
GameObject* GameManager::GetGameObjectByName(const std::string &name) const
{
    for(int i =0; i < pEntity.size(); i++)
    {
        if( pEntity[i]->name.compare( name ) == 0 )
        {
            return pEntity[i];
        }
    }
    return 0;
}
GameObject* GameManager::GetComponentByName(const std::string &name) const
{
    return 0;
}

void GameManager::Initialization()
{
    
  simdjson::dom::parser parser;
  simdjson::dom::element dataJson = parser.load("RemorseAdventurer.json");

  /// Initialization Loading Data...
  globalGame.screenSize.x = dataJson["EntryScene"].at(1)["Camera2D"]["Transform"].at(2).get_int64();
  globalGame.screenSize.y = dataJson["EntryScene"].at(1)["Camera2D"]["Transform"].at(3).get_int64();
  
  /* custom view */
  globalGame.gameView = View( FloatRect(0, 0, globalGame.screenSize.x, globalGame.screenSize.y) );
  
  
  title = dataJson["EntryScene"].at(0)["Title"].get_c_str();
  /* const char* tilePath = dataJson["MainScene"].at(2)["Entity"].at(0)["Image"].get_c_str(); */

  // Prepare the data, temporary using hardcoded
  globalGame.assetsColl.shaderColl  =  new ShaderCollection( dataJson["EntryScene"].at(0)["ShaderDataCount"].get_int64() );
  globalGame.assetsColl.textureColl = new TextureCollection( dataJson["EntryScene"].at(0)["TextureDataCount"].get_int64() );

  window.create(VideoMode(globalGame.screenSize.x , globalGame.screenSize.y ), title);  
  
  window.SetFramerateLimit(40);
  
  globalGame.pWindow = &window;
  
  /* this would use the highest number of entity every SCENE */
  entity.reserve( dataJson["MainScene"].at(2)["Entity"].get_array().size() );
  pEntity.reserve( dataJson["MainScene"].at(2)["Entity"].get_array().size() );
  
  globalGame.getEntity.reserve( dataJson["MainScene"].at(2)["Entity"].get_array().size() );
  
  /* Ini untuk native scripting */
  /* Insert Object */
  /* ObjectRegister(); */
  
  //Entity* ent;
  
  /* Init Memory size before using it */
  /* Gonna use predefined size on json data for fast load */
  /* for now dynamic data at runtime */
/*   unsigned int memSize =0;
  std::cout << "Mem Allocation" << std::endl;
  for(int i=0; i < entity.capacity(); i++ )
  { 
    std::string name( dataJson["MainScene"].at(2)["Entity"].at(i)["Name"].get_c_str() );
    
    for( int j =0; j< globalGame.definedEntity.size(); j++)
    {
        if(globalGame.definedEntity.at(j)->name == name )
        {
            memSize += globalGame.definedEntity.at(j)->thisClassSize;
          break;
        }
    }  
  }
  std::cout << "Mem Init " << memSize << std::endl;
  globalGame.memPRuntimeObj.InitMemory( memSize );
  
  std::cout << "Mem Assign" << std::endl;
  std::cout << globalGame.memPRuntimeObj.size << std::endl;
  // Texture Pointing and selection of Entity name
  for(int i=0; i < entity.capacity(); i++ )
  { 
    std::string name( dataJson["MainScene"].at(2)["Entity"].at(i)["Name"].get_c_str() );
    
    for( int j =0; j< globalGame.definedEntity.size(); j++)
    {
        if(globalGame.definedEntity.at(j)->name == name )
        {
          entity.push_back( globalGame.definedEntity.at(j)->CreateInstance() );
          entity[ entity.size() -1 ]->SetUp( dataJson["MainScene"].at(2)["Entity"].at(i) );
          std::cout << "Mem Loop defi" << std::endl;
          break;
        }
    }  
  } */ /* Ini untuk native scripting */
  
  std::cout << "Script Init" << std::endl;
  /* ScriptManager placed on global */
  globalGame.scriptMngr = new ScriptManager();
  globalGame.scriptMngr->Initialization();
  
  std::cout << "Script End Init" << std::endl;
  
  /* this should be use SceneManager if script works on single scene */
  for(int i=0; i < entity.capacity(); i++ )
  { 
        std::cout << "Entuti Init" << std::endl;
    /* std::string name( dataJson["MainScene"].at(2)["Entity"].at(i)["Name"].get_c_str() ); */   
      entity.push_back( new Entity() );
      entity[ entity.size() -1 ]->SetUp( dataJson["MainScene"].at(2)["Entity"].at(i) );
      std::cout << "Entuty End Init" << std::endl;
  }
    #ifndef REMORSE_DEBUG
      std::cout << entity.capacity() << std::endl;
      std::cout << entity.size() << std::endl;
    #endif
  //
  // DepthLevel Arrangement,
      std::vector<std::vector<int>> depthPos;
      int minDepth = dataJson["EntryScene"].at(0)["DepthMin"].get_int64();
      int maxDepth = dataJson["EntryScene"].at(0)["DepthMax"].get_int64();
      
      depthPos.reserve( maxDepth );
      
      for(int i = 0; i< maxDepth ; i++)
          depthPos.emplace_back();

      // Add depth minDepth first
      for(int i=0; i < entity.size(); i++ )
              for(int j= 0; j < maxDepth ; j++ )
                  if( entity[i]->depthLevel == j + minDepth )
                      depthPos[j].emplace_back(i);

      for(int i=0; i < depthPos.size(); i++ )
          for(int j=0; j < depthPos[i].size(); j++ )
                pEntity.push_back( entity[ depthPos[i][j] ] );
    // DepthLevel Arrangement,

    globalGame.getEntity = pEntity;
    
    // StartData , and Initialize component
    for(int i=0; i < pEntity.size(); i++ ){
            pEntity[i]->Start();
            
            // dipake buat draworder activeEntities
            pEntity[i]->drawId = i;
    };
    std::cout << "Start Initialize" << entity.size() << std::endl; 
    drawOrder.SetEntity( &pEntity );

}

void GameManager::Run()
{
    float time;
    Event event;
    while (window.isOpen())
    { 
        time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time/500;

		if (time > 40) time = 40; 

        window.clear(Color(104, 176, 53));

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)      
                window.close();
        }
        
            drawOrder.Update( &time );
            drawOrder.Draw(&window);

        window.display();
    }
    
    // Delete Entity
    std::cout << "DFFD" << std::endl;                                                                  
    for(int i=0; i < entity.size(); i++ ){
        std::cout <<  entity[i]->name << std::endl;
        
        /* No More delete, just call Destructor */
        /* delete entity[i]; */
        entity[i]->~Entity();
    }
    entity.clear();
    pEntity.clear();
    globalGame.getEntity.clear();
    
    /* for now manually delete */
    delete globalGame.scriptMngr;
    
    /* delete textureData; */
    std::cout << "LLLL" << std::endl;
}

    