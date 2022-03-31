#ifndef __GAME_FRAMEWORK_HPP__
#define __GAME_FRAMEWORK_HPP__

class Component;
class GameObject
{
  public:
      GameObject();
      virtual ~GameObject();
  
      int id;
      unsigned int thisClassSize;
      std::string name;
      std::string objName;
      
      Transformable* transform;
      
      int refCount;
      
/*       virtual void TransformMove(float x, float y);
      virtual void TransformMove(const Vector2f& vec2f); */
      
      int AddRef();
      int Release();
      
      virtual const Component* GetComponentByName(std::string &name);   
      virtual const Component* GetComponentByIndex(int idx);
      
      /* virtual Transformable* GetTransformable(); */
      virtual Vector2f* GetPosition();
      virtual const IntRect* GetIntRect() const;
      virtual Sprite* GetSprite();
};

class Entity;
class Component : public GameObject
{
    public:
        Component();
        virtual ~Component();
        
        virtual void Initialize(Entity* entity);
        virtual void Execute(Entity* entity) = 0;
};

enum AnimationState { HORIZONTALLEFT=0, HORIZONTALRIGHT, VERTICALUP, VERTICALDOWN,
                      DIAGONALLEFTUP, DIAGONALRIGHTUP, DIAGONALLEFTDOWN, DIAGONALRIGHTDOWN,
                      JUMP, SHOOT, IDLE, IDLE2, IDLE3, SIT, CRAWL, COUNT}; 
static std::vector<const char*> animNames = 
    {"HorizontalLeft", "HorizontalRight", "VerticalUp", "VerticalDown",
      "DiagonalLeftUp", "DiagonalRightUp", "DiagonalLeftDown", "DiagonalRightDown",
    "Jump", "Shoot", "Idle", "Idle2", "Idle3", "Sit", "Crawl"};
class AnimationKind
{
    public:
    AnimationKind();
    ~AnimationKind();
    
    int frameSpeed;
    int mode = 0; /* 0 =  backwards, 1 = forwards */
    
    std::vector<int> backWardList;
    std::vector<IntRect> frameRec;
    
    std::string name;
};

class Entity : public GameObject
{
    public:
        Entity();
        
        /* virtual ~Entity() = 0; */
        /* untuk scripting kita tak gunakan pure virtual */
        ~Entity();
    
        void SetUp(simdjson::dom::element jsonData);
        void SetTextureFromPath(std::string path);
        
        const Component* GetComponentByName(std::string &name) override;
        const Component* GetComponentByIndex(int idx) override;
        
/*         void TransformMove(float x, float y) override;
        void TransformMove(const Vector2f& vec2f) override; */
        
        /* virtual Entity* CreateInstance() = 0; */
        virtual void Start();
        virtual void Update( float* time );
        void Draw(RenderWindow* window);
       
        /* Transformable* GetTransformable() override; */
        Vector2f* GetPosition() override;
        const IntRect* GetIntRect() const override;
        Sprite* GetSprite() override;
        
        float* myTime;
        float curAnimSpeed = 1.0f;
        int depthLevel;
        
        AnimationState aState;
        RenderStates rndrStates;
        
        bool othersDraw = false;
        std::vector<RenderStates> othersRenderStates;
        std::vector<Entity*> subEntities;
        EntityStates theEntityStates;
        
    protected:
        Sprite m_sprite;
        IntRect m_transform;
        Vector2f m_position;
        
        Texture* m_texture;
        IntRect* mp_boundingBox;
        
        /* Shader m_shader; */
        
        // Component
        std::vector<Component*> components;
};

class DataCollection
{
    public:
        DataCollection();
        DataCollection(int data);
        virtual ~DataCollection();

        std::vector<std::string> pathData;
};

class TextureCollection : public DataCollection
{
    public:
        TextureCollection();
        TextureCollection(int data);
        ~TextureCollection();
    
        Texture* AddTextureData(std::string path);
        Texture* GetTextureData(std::string path);
        /* std::vector<std::string> pathTexture; */
        std::vector<Texture> dataTexture;
};

class ShaderCollection : public DataCollection
{
    public:
    ShaderCollection();
    ShaderCollection(int data);
    ~ShaderCollection();

    Shader* AddShaderData(std::string path);
    Shader* GetShaderData(std::string path);
    
    Shader* AddShaderData(std::string path, std::string path2);
    Shader* GetShaderData(std::string path, std::string path2);
    /* std::vector<std::string> pathShader; */
    
/*     Error kalau bukan pointer
    error: use of deleted function 'sf::Shader::Shader(const sf::Shader&)' */
    std::vector<Shader*> dataShader;
};

class SoundCollection : public DataCollection
{
    public:
    SoundCollection();
    SoundCollection(int data);
    ~SoundCollection();

    Shader* AddShaderData(std::string path);
    Shader* GetShaderData(std::string path);
    std::vector<Shader> dataShader;
};

class AssetsCollectionManager
{
  public:
    AssetsCollectionManager();
    ~AssetsCollectionManager();
  
    TextureCollection* textureColl;
    ShaderCollection*  shaderColl;
};

class FrameAnimation : public Component
{
    public:
        FrameAnimation();
        ~FrameAnimation();
    
    std::vector<AnimationKind> animationList;
    void Execute(Entity* entity) override;
};

class ShaderScript : public Component
{
    public:
        ShaderScript();
        ~ShaderScript();
    
    Shader* pShader;
    
    enum TypeShader {VERTEX=0, FRAGMENT, GEOMETRY};
    void Execute(Entity* entity) override;
};

class Shadow2D : public Component
{
    public:
        Shadow2D();
        ~Shadow2D();
    
    RenderStates state;
    Shader* pShader;
    
    void Execute(Entity* entity) override;
};

class BoxCollision : public Component
{
    public:
        BoxCollision();
        ~BoxCollision();
        
        FloatRect boundingBox;
        
        FloatRect staticBoundingBox;
        
        void Execute(Entity* entity) override;
};

class SubEntities : public Component
{
    public:
        SubEntities();
        ~SubEntities();
        
        std::vector<Entity*>* listSubEntities;
        
        // Component Derived
        void Execute(Entity* entity) override;
};
class AudioSource : public Component
{
    public:
        AudioSource();
        ~AudioSource();

        void Play(std::string &name, bool isLoop);
        void PlayStream(std::string &name, bool isLoop);
        
        void Stop(std::string &name);
        void StopAll();
        
        std::vector< Music > theMusics;
        
        void Execute(Entity* entity) override;
};
class EntityStates : public Component
{
    public:
        EntityStates();
        ~EntityStates();

        std::vector<int> states;
        int curState;
        
        void Execute(Entity* entity) override;
};

class Script : public Component
{
    public:
        Script();
        ~Script();
        
		std::string        module;
		asITypeInfo       *type;
		asIScriptFunction *factoryFunc;
		asIScriptFunction *startMethod;
		asIScriptFunction *updateMethod;
        
        /* for now use 1 script  */
        std::vector<asIScriptObject*> controller;
        
        asILockableSharedBool *weakRefFlag;
        
        void Initialize(Entity* entity) override;
        
        void Update();
        void Execute(Entity* entity) override;
};

class SolidCollide : public Component
{
    public:
        SolidCollide();
        ~SolidCollide();
        
        Entity* srcEntity;
        Entity* trgEntity;;

        Vector2f trg;
        
        const BoxCollision* srcBoxColl;
        const BoxCollision* trgBoxColl;
        
        void SetUp(Entity* sEntity, Entity* tEntity, const BoxCollision* sCollision , const BoxCollision* tCollision);
        void Execute(Entity* entity) override;
};

class Camera2D : public GameObject
{
    public:
    Camera2D();
    ~Camera2D();
    
    View gameView;
    IntRect cameraRect;
};
/* Planning */

class DrawOrders
{
    public:
    DrawOrders();
    ~DrawOrders();
     
    std::vector<Entity*>* entitiesArray;
    
    void SetEntity(std::vector<Entity*>* pEntity);
    void Update( float* time ); 
    void Draw(RenderWindow* window);
};

/* Planning */
class CullingScreen2D
{
    public:
    CullingScreen2D();
    ~CullingScreen2D();
    
    Camera2D* pCamera;
    std::vector<Entity*>* readyEntity;
    
    void Init(Camera2D* camera);
    void CullingScreen( std::vector<Entity*>* pEntity );
    void Update( float* time ); 
};

class ScreenDrawManager
{
    public:
    ScreenDrawManager();
    ~ScreenDrawManager();
    
    DrawOrders drawOrder;
    CullingScreen2D culling2D;
    
    void Init( std::vector<Entity*>* pEntity, Camera2D* camera);
    void Update( float* time ); 
    void Draw(RenderWindow* window);
};

/* GLOBAL */
class ScriptManager;
class GameManager;
static class GlobalGamePointer
{
    public:
    GlobalGamePointer(){}
    ~GlobalGamePointer(){}
    
    View gameView;
    
    // Global Variable
    std::vector<Entity*> getEntity;
    
    RenderWindow* pWindow;
    Vector2i screenSize;
    
    MemoryPool memPInitialObj;
    MemoryPool memPRuntimeObj;
    
    MemoryPool memPResource;
    
    AssetsCollectionManager assetsColl;
    Mutex mutex;
    
    ScriptManager* scriptMngr;
    GameManager*     gameMgr;

    //
    
} globalGame;

#define ADDCOMPONENT(A) \
            A* comp = new A(); \
            comp->Initialize( this ); \
            components.push_back( comp );
            
#define INITCLASS(A) \
            name = #A; \
            objName = #A; \
            thisClassSize = sizeof(A);

void ObjectRegister();

#endif /* __GAME_FRAMEWORK_HPP__ */
