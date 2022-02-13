
/* class Player : public Entity */
Player::Player()
{
    name = "Player";
    objName = "Player";
}
Player::~Player()
{
    
}
Entity* Player::CreateInstance()
{
    return new Player();
}
void Player::Start()
{
    
}
void Player::Update( float* time )
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
    return new Enemy();
}
void Enemy::Start()
{
    enemyBoxCollision = (const BoxCollision*)Entity::GetComponentByName("BoxCollision");
    for(int i =0; i < globalGame.getEntity.size(); i++)
    {
        if(globalGame.getEntity[i]->name == "Player")
        {
            player = (Player*)globalGame.getEntity[i];
            playerBoxCollision = (const BoxCollision*)player->GetComponentByName("BoxCollision");
        }
    }
    
    solidColl = (SolidCollide*)Entity::GetComponentByName("SolidCollide");
    solidColl->SetUp(this, player, enemyBoxCollision, playerBoxCollision);
    
     move = 0.02f;
}
void Enemy::Update( float* time )
{
   Entity::Update( time );
   
   if( m_position.x >= 100 )
       aState = AnimationState::HORIZONTALLEFT;
   else if ( m_position.x <= 0)
       aState = AnimationState::HORIZONTALRIGHT;
   
   
   if( m_position.x >= 100 || m_position.x <= 0 )
        move *= -1;
   
   curAnimSpeed  = m_position.x;
   m_position.x += move * *time;
   /* std::cout <<  m_position.x << std::endl; */
   m_sprite.setPosition( m_position );
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
    return new Tent_A();
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
    return new Tree_A();
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
    return new Fence();
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
    return new Well();
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
    return new MovingObject();
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
    return new Grass();
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
    return new Flower();
}
void Flower::Start()
{
    
}
void Flower::Update( float* time )
{
    Entity::Update( time );
}


/* MUST REGISTER YOUR CLASS OBJECT */
void ObjectRegister()
{
    globalGame.definedEntity.push_back( new Player() );
    globalGame.definedEntity.push_back( new Enemy() );   
    globalGame.definedEntity.push_back( new Tree_A() );
    globalGame.definedEntity.push_back( new Tent_A() );
    globalGame.definedEntity.push_back( new Fence() );
    globalGame.definedEntity.push_back( new Well() );
    globalGame.definedEntity.push_back( new MovingObject() );
    globalGame.definedEntity.push_back( new Grass() );
    globalGame.definedEntity.push_back( new Flower() );
}

