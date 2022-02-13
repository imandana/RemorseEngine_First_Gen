
/* class Player : public Entity */
Player::Player()
{
    name = "Player";
    objName = "Player";
    thisClassSize = sizeof(this);
}
Player::~Player()
{
    
}
Entity* Player::CreateInstance()
{
    //return new Player();
}
void Player::Start()
{
    ADDCOMPONENT(PlayerInput)
}
void Player::Update( float* time )
{
    Entity::Update( time );
    
    /* Camera Follow Player */
    globalGame.gameView.setCenter( m_position );
    globalGame.pWindow->setView( globalGame.gameView );
}

/* class MarcoUp : public Entity */
MarcoUp::MarcoUp()
{
    INITCLASS(MarcoUp)
}
MarcoUp::~MarcoUp()
{
    
}
Entity* MarcoUp::CreateInstance()
{
    std::cout << "Mem Marco up 1st" << std::endl;
    void* ins = new(globalGame.memPRuntimeObj.RequestMemory( sizeof(MarcoUp) )) MarcoUp();
    Entity* kore = (Entity*)ins;
    
    std::cout << "Mem Marco Up" << std::endl;
    return kore;
    //return new MarcoUp();
}
void MarcoUp::Start()
{
}
void MarcoUp::Update( float* time )
{
    Entity::Update( time );
    
}

/* class MarcoDown : public Entity */
MarcoDown::MarcoDown()
{
    INITCLASS(MarcoDown)
}
MarcoDown::~MarcoDown()
{
    
}
Entity* MarcoDown::CreateInstance()
{
    std::cout << "Mem Marco down " << sizeof(MarcoUp) + sizeof(MarcoDown) <<std::endl;
    Entity* ins = new(globalGame.memPRuntimeObj.RequestMemory( sizeof(MarcoDown) )) MarcoDown();
    Entity* kore = (Entity*)ins;
    
    std::cout << "Mem down 2nd" << std::endl;
    return kore;
    //return new MarcoDown();
}
void MarcoDown::Start()
{
}
void MarcoDown::Update( float* time )
{
    Entity::Update( time );
    
}

/* class Enemy : public Entity */
Enemy::Enemy()
{
    name = "Enemy";
    objName = "Enemy";
}
Enemy::~Enemy()
{
    
}
Entity* Enemy::CreateInstance()
{
    //return new Enemy();
}
void Enemy::Start()
{
    ADDCOMPONENT(AIAutoMove)
}
void Enemy::Update( float* time )
{
   Entity::Update( time );
}
    
/* class Tent_A : public Entity */
Tent_A::Tent_A()
{
    name = "Tent_A1";
    objName = "Tent_A1";
}
Tent_A::~Tent_A()
{
    
}
Entity* Tent_A::CreateInstance()
{
    //return new Tent_A();
}
void Tent_A::Start()
{
    tentBoxCollision = (const BoxCollision*)Entity::GetComponentByName("BoxCollision");
    for(int i =0; i < globalGame.getEntity.size(); i++)
    {
        if(globalGame.getEntity[i]->name == "Player")
        {
            player = (Player*)globalGame.getEntity[i];
            playerBoxCollision = (const BoxCollision*)player->GetComponentByName("BoxCollision");
        }
    }
    
    solidColl = (SolidCollide*)Entity::GetComponentByName("SolidCollide");
    solidColl->SetUp(this, player, tentBoxCollision, playerBoxCollision);
}
void Tent_A::Update( float* time )
{
    Entity::Update( time );
}

/* class Tree_A : public Entity */
Tree_A::Tree_A()
{
    name = "Tree_A1";
    objName = "Tree_A1";
}
Tree_A::~Tree_A()
{
    
}
Entity* Tree_A::CreateInstance()
{
    //return new Tree_A();
}
void Tree_A::Start()
{
    treeBoxCollision = (const BoxCollision*)Entity::GetComponentByName("BoxCollision");
    for(int i =0; i < globalGame.getEntity.size(); i++)
    {
        if(globalGame.getEntity[i]->name == "Player")
        {
            player = (Player*)globalGame.getEntity[i];
            playerBoxCollision = (const BoxCollision*)player->GetComponentByName("BoxCollision");
        }
    }
    
    solidColl = (SolidCollide*)Entity::GetComponentByName("SolidCollide");
    solidColl->SetUp(this, player, treeBoxCollision, playerBoxCollision);
}
void Tree_A::Update( float* time )
{
    Entity::Update( time );
}

/* class Fence : public Entity */
Fence::Fence()
{
    name = "Fence";
    objName = "Fence";
}
Fence::~Fence()
{
    
}
Entity* Fence::CreateInstance()
{
    //return new Fence();
}
void Fence::Start()
{
    fenceBoxCollision = (const BoxCollision*)Entity::GetComponentByName("BoxCollision");
    for(int i =0; i < globalGame.getEntity.size(); i++)
    {
        if(globalGame.getEntity[i]->name == "Player")
        {
            player = (Player*)globalGame.getEntity[i];
            playerBoxCollision = (const BoxCollision*)player->GetComponentByName("BoxCollision");
        }
    }
    
    solidColl = (SolidCollide*)Entity::GetComponentByName("SolidCollide");
    solidColl->SetUp(this, player, fenceBoxCollision, playerBoxCollision);
}
void Fence::Update( float* time )
{
    Entity::Update( time );
}

/* class Well : public Entity */
Well::Well()
{
    name = "Well";
    objName = "Well";
}
Well::~Well()
{
    
}
Entity* Well::CreateInstance()
{
    //return new Well();
}
void Well::Start()
{
        wellBoxCollision = (const BoxCollision*)Entity::GetComponentByName("BoxCollision");
    for(int i =0; i < globalGame.getEntity.size(); i++)
    {
        if(globalGame.getEntity[i]->name == "Player")
        {
            player = (Player*)globalGame.getEntity[i];
            playerBoxCollision = (const BoxCollision*)player->GetComponentByName("BoxCollision");
        }
    }
    
    solidColl = (SolidCollide*)Entity::GetComponentByName("SolidCollide");
    solidColl->SetUp(this, player, wellBoxCollision, playerBoxCollision);
}
void Well::Update( float* time )
{
    Entity::Update( time );
}

/* class MovingObject : public Entity */
MovingObject::MovingObject()
{
    name = "MovingObject";
    objName = "MovingObject";
}
MovingObject::~MovingObject()
{
}
Entity* MovingObject::CreateInstance()
{
    //return new MovingObject();
}
void MovingObject::Start()
{
    
}
void MovingObject::Update( float* time )
{
    Entity::Update( time );
}

/* class Grass : public Entity */
Grass::Grass()
{
    name = "Grass";
    objName = "Grass";
}
Grass::~Grass()
{
}
Entity* Grass::CreateInstance()
{
    //return new Grass();
}
void Grass::Start()
{
    
}
void Grass::Update( float* time )
{
    Entity::Update( time );
}

/* class Flower : public Entity */
Flower::Flower()
{
    name = "Flower";
    objName = "Flower";
}
Flower::~Flower()
{
}
Entity* Flower::CreateInstance()
{
    //return new Flower();
}
void Flower::Start()
{
    
}
void Flower::Update( float* time )
{
    Entity::Update( time );
}

/* Extra Component */
/* class AIAutoMove : public Component */
AIAutoMove::AIAutoMove()
{

}
AIAutoMove::~AIAutoMove()
{
    
}
void AIAutoMove::Initialize(Entity* entity)
{
    thisBoxCollision = (const BoxCollision*)entity->GetComponentByName("BoxCollision");
    for(int i =0; i < globalGame.getEntity.size(); i++)
    {
        if(globalGame.getEntity[i]->name == "Player")
        {
            player = (Player*)globalGame.getEntity[i];
            playerBoxCollision = (const BoxCollision*)player->GetComponentByName("BoxCollision");
        }
    }
    
    solidColl = (SolidCollide*)entity->GetComponentByName("SolidCollide");
    solidColl->SetUp(entity, player, thisBoxCollision, playerBoxCollision);
    
    entity->aState = AnimationState::HORIZONTALRIGHT;
    rightOffset.x = entity->GetPosition()->x + 40;
    leftOffset.x  = entity->GetPosition()->x - 40;
    move = 0.02f;
}
void AIAutoMove::Execute(Entity* entity)
{
   if( entity->GetPosition()->x >= rightOffset.x )
       entity->aState = AnimationState::HORIZONTALLEFT;
   else if ( entity->GetPosition()->x <= leftOffset.x)
       entity->aState = AnimationState::HORIZONTALRIGHT;
   
   
   if( entity->GetPosition()->x >= rightOffset.x || entity->GetPosition()->x <= leftOffset.x )
        move *= -1;
   
   entity->curAnimSpeed  = entity->GetPosition()->x;
   entity->GetPosition()->x += move * *entity->myTime;
   /* std::cout <<  GetPosition()->x << std::endl; */
   entity->GetSprite()->setPosition( *entity->GetPosition() );
}

/* class PlayerInput : public Component */
PlayerInput::PlayerInput()
{

}
PlayerInput::~PlayerInput()
{
    
}
void PlayerInput::Initialize(Entity* entity)
{
    speed = 0.05f;
}
void PlayerInput::Execute(Entity* entity)
{
    if ( Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D) ) 
        { accel.x += 0.001f * *entity->myTime;  if(accel.x > 1.0f) accel.x = 1.0f; }
    else
        { accel.x -= 0.001f * *entity->myTime; if(accel.x < 0.0f) accel.x = 0.0f; }
    
    if ( Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::S) ) 
        { accel.y += 0.001f * *entity->myTime;  if(accel.y > 1.0f) accel.y = 1.0f; }
    else
        { accel.y -= 0.001f * *entity->myTime; if(accel.y < 0.0f) accel.y = 0.0f; }
    
    if ( Keyboard::isKeyPressed(Keyboard::A) ) { direction.x = -1; entity->aState = AnimationState::HORIZONTALLEFT; }
    if ( Keyboard::isKeyPressed(Keyboard::D) ) { direction.x = 1; entity->aState = AnimationState::HORIZONTALRIGHT; }
    if ( Keyboard::isKeyPressed(Keyboard::W) ) { direction.y = -1; entity->aState = AnimationState::VERTICALUP; }
    if ( Keyboard::isKeyPressed(Keyboard::S) ) { direction.y = 1; entity->aState = AnimationState::VERTICALDOWN; }
    
    entity->GetPosition()->x += *entity->myTime * (speed*accel.x)* direction.x; 
    entity->GetPosition()->y += *entity->myTime * (speed*accel.y)* direction.y; 
    
    /* Animation Accel */
    if ( Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::W) ||
         Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::S)  ) 
    {    animAccel += (*entity->myTime *0.001f) ;  if(animAccel > 1.0f)animAccel = 1.0f;}
    else
    {   animAccel -= (*entity->myTime *0.001f) ; if(animAccel < 0.0f) animAccel = 0.0f; }
     
    entity->curAnimSpeed += *entity->myTime * (speed * animAccel * direction.x);
    /* Animation Accel */
    
    entity->GetSprite()->setPosition( *entity->GetPosition() );
}

/* MUST REGISTER YOUR CLASS OBJECT */
void ObjectRegister()
{
    unsigned int size = sizeof(MarcoUp);
    size += sizeof(MarcoDown); 
    globalGame.memPInitialObj.InitMemory(size);
    void* instantiate = globalGame.memPInitialObj.RequestMemory( sizeof(MarcoUp) );
    globalGame.definedEntity.push_back( new(instantiate) MarcoUp() );
    
    instantiate = globalGame.memPInitialObj.RequestMemory( sizeof(MarcoDown) );
    globalGame.definedEntity.push_back( new(instantiate) MarcoDown() );
    
/*     globalGame.definedEntity.push_back( new Player() );
    globalGame.definedEntity.push_back( new Enemy() );   
    globalGame.definedEntity.push_back( new Tree_A() );
    globalGame.definedEntity.push_back( new Tent_A() );
    globalGame.definedEntity.push_back( new Fence() );
    globalGame.definedEntity.push_back( new Well() );
    globalGame.definedEntity.push_back( new MovingObject() );
    globalGame.definedEntity.push_back( new Grass() );
    globalGame.definedEntity.push_back( new Flower() ); */
}

