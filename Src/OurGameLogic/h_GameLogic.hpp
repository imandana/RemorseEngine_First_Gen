#ifndef __GAME_LOGIC_HPP__
#define __GAME_LOGIC_HPP__

class Player : public Entity
{
    public : 
        Player();
        ~Player();

    const BoxCollision* playerBoxCollision;
    Vector2f currentPos;
    
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
    
};
class MarcoUp : public Entity
{
    public:
        MarcoUp();
        ~MarcoUp();
        
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
};
class MarcoDown : public Entity
{
    public:
        MarcoDown();
        ~MarcoDown();
        
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
};
class Enemy : public Entity
{
    public : 
        Enemy();
        ~Enemy();

    Player* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* enemyBoxCollision;
    
    SolidCollide* solidColl;
    
       /* add some movement */
    float move;
    
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
    
};
class Tent_A : public Entity
{
    public : 
        Tent_A();
        ~Tent_A();
    
    Player* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* tentBoxCollision;
    
    SolidCollide* solidColl;
    
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
};
class Tree_A : public Entity
{
    public : 
        Tree_A();
        ~Tree_A();
    
    Entity* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* treeBoxCollision;
    
    SolidCollide* solidColl;
    
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
};
class Fence : public Entity
{
    public : 
        Fence();
        ~Fence();
    
    Entity* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* fenceBoxCollision;
    
    SolidCollide* solidColl;
    
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
};
class Well : public Entity
{
    public : 
        Well();
        ~Well();
    
    Entity* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* wellBoxCollision;
    
    SolidCollide* solidColl;
    
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
};

class MovingObject : public Entity
{
    public : 
        MovingObject();
        ~MovingObject();
    
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
};

class Grass : public Entity
{
    public : 
        Grass();
        ~Grass();
    
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
};

class Flower : public Entity
{
    public : 
        Flower();
        ~Flower();
    
    Entity* CreateInstance() override;
    void Start() override;
    void Update( float* time ) override;
};

/* Extra Component */
class AIAutoMove : public Component
{
    public:   
        AIAutoMove();
        ~AIAutoMove();

        Player* player;
        const BoxCollision* playerBoxCollision;
        const BoxCollision* thisBoxCollision;
        
        SolidCollide* solidColl;
        
           /* add some movement */
        float move;
        Vector2i rightOffset;
        Vector2i leftOffset;
        
        void Initialize(Entity* entity) override;
        void Execute(Entity* entity) override;
};

class PlayerInput : public Component
{
    public:   
        PlayerInput();
        ~PlayerInput();

        float speed;
        float animAccel =0.0f;
        Vector2f accel;
        Vector2i direction= {1,1};
        std::string scriptName;
        
        void Initialize(Entity* entity) override;
        void Execute(Entity* entity) override;
};

#endif /* __GAME_LOGIC_HPP__ */
