// c++ standart
#include <iostream>

// SFML Library
#include <SFML/Graphics.hpp>

// AngelScript
//#include <angelscript.h>

// SimdJson
#include "simdjson.h"

//////////////////////////////////////////////////////////////////////////
////                                                                  ////
////                        NAMESPACE DECLARATION                     ////          
////                                                                  ////
//////////////////////////////////////////////////////////////////////////
using namespace sf;

//////////////////////////////////////////////////////////////////////////
////                                                                  ////
////                        DECLARATION                               ////          
////                                                                  ////
//////////////////////////////////////////////////////////////////////////
class GameObject
{
  public:
      GameObject(){}
      virtual ~GameObject(){}
  
      int id;
      std::string name;
};

class TextureCollection
{
    public:
        TextureCollection(){}
        TextureCollection(int data);
        ~TextureCollection(){ dataTexture.clear(); pathTexture.clear(); }
    
        Texture* AddTextureData(std::string path);
        Texture* GetTextureData(std::string path);
        std::vector<std::string> pathTexture;
        std::vector<Texture> dataTexture;
};

class ShaderCollection
{
    public:
    ShaderCollection(){}
    ShaderCollection(int data);
    ~ShaderCollection(){ dataShader.clear(); pathShader.clear(); }

    Shader* AddShaderData(std::string path);
    Shader* GetShaderData(std::string path);
    
    Shader* AddShaderData(std::string path, std::string path2);
    Shader* GetShaderData(std::string path, std::string path2);
    std::vector<std::string> pathShader;
    std::vector<Shader*> dataShader;
};

class SoundCollection
{
    public:
    SoundCollection(){}
    SoundCollection(int data);
    ~SoundCollection(){ dataShader.clear(); pathShader.clear(); }

    Shader* AddShaderData(std::string path);
    Shader* GetShaderData(std::string path);
    std::vector<std::string> pathShader;
    std::vector<Shader> dataShader;
};

class AssetsCollectionManager
{
  public:
    AssetsCollectionManager(){}
    ~AssetsCollectionManager(){ if(textureColl) delete textureColl;
                                if(shaderColl) delete shaderColl;
                               }
  
    TextureCollection* textureColl;
    ShaderCollection*  shaderColl;
};

class MemoryPoolCollection
{
    public: 
    MemoryPoolCollection(){}
    ~MemoryPoolCollection(){}
    
    char* allMemoryPool;
    
    int textureDataAddr;
    int scriptDataAddr;
    int soundDataAddr;
    int shaderDataAddr;
    
};

class Entity;

// static class for global data pointer
static class GlobalGamePointer
{
    public:
    GlobalGamePointer(){}
    ~GlobalGamePointer(){}
    
    // Global Variable
    std::vector<Entity*> getEntity;
    
    RenderWindow* pWindow;
    Vector2i screenSize;
    
    AssetsCollectionManager assetsColl;
    Mutex mutex;
    
    MemoryPoolCollection memPool;
    //
    
} globalGame;
enum AnimationState { HORIZONTALLEFT=0, HORIZONTALRIGHT, VERTICALUP, VERTICALDOWN }; 

class Component : public GameObject
{
    public:
        Component(){}
        virtual ~Component(){}
        virtual void Execute(Entity* entity) = 0;
};

class AnimationKind
{
    public:
    AnimationKind(){}
    ~AnimationKind(){}
    
    int frameSpeed;
    int mode = 0; /* 0 =  backwards, 1 = forwards */
    std::vector<int> backWardList;
    std::vector<IntRect> frameRec;
    
    std::string name;
};

class FrameAnimation : public Component
{
    public:
        FrameAnimation(){}
        ~FrameAnimation(){}
    
    std::vector<AnimationKind> animationList;
    void Execute(Entity* entity) override;
};

class ShaderScript : public Component
{
    public:
        ShaderScript(){}
        ~ShaderScript(){}
    
    Shader* pShader;
    
    enum TypeShader {VERTEX=0, FRAGMENT, GEOMETRY};
    void Execute(Entity* entity) override;
};

class Shadow2D : public Component
{
    public:
        Shadow2D(){}
        ~Shadow2D(){}
    
    RenderStates state;
    Shader* pShader;
    
    void Execute(Entity* entity) override;
};

class BoxCollision : public Component
{
    public:
        BoxCollision(){}
        ~BoxCollision(){ std::cout << "BoxCollision DESSSS" << std::endl; }
        
        IntRect boundingBox;
        IntRect staticBoundingBox;
        
        void Execute(Entity* entity) override;
};

class Script : public Component
{
    public:
        Script(){  }
        ~Script(){ std::cout << "Script DESSSS" << std::endl; }
        
        float speed;
        float animAccel =0.0f;
        Vector2f accel;
        Vector2i direction= {1,1};
        std::string scriptName;
        void Execute(Entity* entity) override;
};

class SolidCollide : public Component
{
    public:
        SolidCollide(){}
        ~SolidCollide(){ std::cout << "SolidCollide DESSSS" << std::endl; }
        
        Entity* srcEntity;
        Entity* trgEntity;;
        
        const BoxCollision* srcBoxColl;
        const BoxCollision* trgBoxColl;
        
        void SetUp(Entity* sEntity, Entity* tEntity, const BoxCollision* sCollision , const BoxCollision* tCollision);
        void Execute(Entity* entity) override;
};

class Entity : public GameObject
{
    public:
        Entity(){}
        virtual ~Entity();
    
        void SetUp(simdjson::dom::element jsonData);
        
        void SetTextureFromPath(std::string path);
        const Component* GetComponentByName(std::string name);
        
        virtual void Start();
        virtual void Update( float* time );
        void Draw(RenderWindow* window);
        
        Vector2f* GetPosition();
        const IntRect* GetIntRect() const;
        Sprite* GetSprite();
        
        float* myTime;
        float curAnimSpeed = 1.0f;
        int depthLevel;
        
        AnimationState aState;
        RenderStates rndrStates;
        
        bool othersDraw = false;
        std::vector<RenderStates> othersRenderStates;
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

/* Planning */
class Camera2D : public GameObject
{
    public:
    Camera2D() { }
    ~Camera2D(){ }
    
    IntRect cameraRect;
};
/* Planning */

class DrawOrders
{
    public:
    DrawOrders() { }
    ~DrawOrders(){ }
     
    std::vector<Entity*>* entitiesArray;
    
    void SetEntity(std::vector<Entity*>* pEntity);
    void Update( float* time ); 
    void Draw(RenderWindow* window);
};

/* Planning */
class CullingScreen2D
{
    public:
    CullingScreen2D(){}
    ~CullingScreen2D(){}
    
    Camera2D* pCamera;
    std::vector<Entity*>* readyEntity;
    
    void Init(Camera2D* camera);
    void CullingScreen( std::vector<Entity*>* pEntity );
    void Update( float* time ); 
};

class ScreenDrawManager
{
    public:
    ScreenDrawManager(){}
    ~ScreenDrawManager(){}
    
    DrawOrders drawOrder;
    CullingScreen2D culling2D;
    
    void Init( std::vector<Entity*>* pEntity, Camera2D* camera);
    void Update( float* time ); 
    void Draw(RenderWindow* window);
};
/* Planning */

//////////////////////////////////////////////////////////////////////////
////                                                                  ////
////                        DECLARATION  -  END                       ////          
////                                                                  ////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
////                                                                  ////
////                        IMPLEMENTATION                            ////          
////                                                                  ////
//////////////////////////////////////////////////////////////////////
////  TextureCollection
TextureCollection::TextureCollection(int data){ 
            dataTexture.reserve(data); 
            pathTexture.reserve(data); 
}
Texture* TextureCollection::AddTextureData(std::string path)
{
    std::cout << "Adding Texture" << path << ", Count : " << pathTexture.size()+1 << std::endl;
    Texture temp;
    temp.loadFromFile(path);
    
    pathTexture.push_back(path);    
    dataTexture.push_back(temp);    // Copy texture
    
    return &dataTexture[dataTexture.size()-1];
}
Texture* TextureCollection::GetTextureData(std::string path)
{    
    for(int i = 0; i < pathTexture.capacity(); i++)
    {
        if( pathTexture[i] == path){
            return &dataTexture[i];
        }
    }
    
    return AddTextureData(path);
}

////  ShaderCollection
ShaderCollection::ShaderCollection(int data){ 
            dataShader.reserve(data); 
            pathShader.reserve(data); 
}

Shader* ShaderCollection::AddShaderData(std::string path)
{
    std::cout << "Adding Shader" << path << ", Count : " << pathShader.size()+1 << std::endl;
     
    pathShader.push_back(path);    
    dataShader.push_back( new Shader() );  /* sementara */
    
    dataShader[ dataShader.size() -1 ]->loadFromFile(path, sf::Shader::Vertex);
    
    return dataShader[dataShader.size()-1];
}
Shader* ShaderCollection::GetShaderData(std::string path)
{    
    for(int i = 0; i < pathShader.capacity(); i++)
    {
        if( pathShader[i] == path){
            return dataShader[i];
        }
    }
    
    return AddShaderData(path);
}

Shader* ShaderCollection::AddShaderData(std::string path, std::string path2)
{
    std::cout << "Adding Shader" << path << ", Count : " << pathShader.size()+1 << std::endl;
     
    pathShader.push_back(path);    
    dataShader.push_back( new Shader() );  /* sementara */
    
    dataShader[ dataShader.size() -1 ]->loadFromFile(path, path2);
    
    return dataShader[dataShader.size()-1];
}
Shader* ShaderCollection::GetShaderData(std::string path, std::string path2)
{    
    for(int i = 0; i < pathShader.capacity(); i++)
    {
        if( pathShader[i] == path){
            return dataShader[i];
        }
    }
    
    return AddShaderData(path, path2);
}
////  FrameAnimation : public Component
void FrameAnimation::Execute(Entity* entity)
{
    int num;
    
    switch(animationList[entity->aState].mode)
    {
        case 0:
            /* Backward Animation */
            num = ( (int)entity->curAnimSpeed ) % 
                ( ( animationList[entity->aState].frameRec.size() + (animationList[entity->aState].frameRec.size()) -2 ) * animationList[entity->aState].frameSpeed);
            num /= animationList[entity->aState].frameSpeed;
         
            entity->GetSprite()->setTextureRect( 
                animationList[entity->aState].frameRec[ animationList[entity->aState].backWardList[ num ]]
                );
            break;
        case 1:
            /* Forward Animation */
            num = ( (int)entity->curAnimSpeed ) % 
                ( (animationList[entity->aState].frameRec.size()-1) * animationList[entity->aState].frameSpeed);   
            num /= animationList[entity->aState].frameSpeed;
            entity->GetSprite()->setTextureRect( animationList[entity->aState].frameRec[num] );
            break;
        
    }
}
////  ShaderScript : public Component
void ShaderScript::Execute(Entity* entity)
{
    float test = static_cast<float>( Mouse::getPosition( *globalGame.pWindow ).x ) ;
    pShader->setUniform("wave_phase", test) ;
    pShader->setUniform("wave_amplitude", Vector2f( 40, 40));
    
    std::cout << test << std::endl;
    entity->rndrStates.shader = pShader;
}

////  Shadow2D : public Component
void Shadow2D::Execute(Entity* entity)
{
    float test = static_cast<float>( Mouse::getPosition( *globalGame.pWindow ).x ) ;
    pShader->setUniform("wave_phase", test*2.0f) ;
    pShader->setUniform("wave_amplitude", Vector2f( 40, 40));
    pShader->setUniform("sprite_size", entity->GetIntRect() );
    
    std::cout << "SHADOW " <<test << std::endl;
    entity->othersRenderStates[0].shader = pShader;
}

////  BoxCollision : public Component
void BoxCollision::Execute(Entity* entity)
{
    boundingBox.left = staticBoundingBox.left + entity->GetPosition()->x ;
    boundingBox.top = staticBoundingBox.top + entity->GetPosition()->y;
      
    boundingBox.width = staticBoundingBox.width;
    boundingBox.height = staticBoundingBox.height;
}

////  Script : public Component
void Script::Execute(Entity* entity)
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

////  SolidCollide
void SolidCollide::SetUp(Entity* sEntity, Entity* tEntity, const BoxCollision* sCollision , const BoxCollision* tCollision)
{
    srcEntity = sEntity;
    trgEntity = tEntity;
    
    srcBoxColl = sCollision;
    trgBoxColl =  tCollision;
}
void SolidCollide::Execute(Entity* entity) 
{
  if(  srcBoxColl->boundingBox.intersects( trgBoxColl->boundingBox )  ) {
        std::cout << "I am " << srcEntity->name << " Colliding With " << trgEntity->name <<std::endl;
        
        // Kanan
        if( srcBoxColl->boundingBox.left + srcBoxColl->boundingBox.width  < trgBoxColl->boundingBox.left + trgBoxColl->boundingBox.width &&
        
            trgBoxColl->boundingBox.top + trgBoxColl->boundingBox.height > srcBoxColl->boundingBox.top + 3                                  &&
            trgBoxColl->boundingBox.top < srcBoxColl->boundingBox.top + srcBoxColl->boundingBox.height - 3
          ){
                trgEntity->GetPosition()->x = srcBoxColl->boundingBox.left + srcBoxColl->boundingBox.width;
            }// Kiri
        else if(  trgBoxColl->boundingBox.left < srcBoxColl->boundingBox.left  &&
        
                  trgBoxColl->boundingBox.top + trgBoxColl->boundingBox.height > srcBoxColl->boundingBox.top + 3  &&
                  trgBoxColl->boundingBox.top < srcBoxColl->boundingBox.top + srcBoxColl->boundingBox.height - 3
               ){
                trgEntity->GetPosition()->x =  srcBoxColl->boundingBox.left - trgBoxColl->boundingBox.width;
            }
            
        // Bawah
        if( srcBoxColl->boundingBox.top + srcBoxColl->boundingBox.height  > trgBoxColl->boundingBox.top &&
             srcBoxColl->boundingBox.top + srcBoxColl->boundingBox.height < trgBoxColl->boundingBox.top + trgBoxColl->boundingBox.height &&
             
            trgBoxColl->boundingBox.left + trgBoxColl->boundingBox.width > srcBoxColl->boundingBox.left + 3                                  &&
            trgBoxColl->boundingBox.left < srcBoxColl->boundingBox.left + srcBoxColl->boundingBox.width - 3
             ){
                trgEntity->GetPosition()->y = 2 + (srcBoxColl->boundingBox.top + srcBoxColl->boundingBox.height) - trgBoxColl->boundingBox.height;
            } // Atas
        else if(  trgBoxColl->boundingBox.top + trgBoxColl->boundingBox.height >= srcBoxColl->boundingBox.top &&
                  trgBoxColl->boundingBox.top < srcBoxColl->boundingBox.top                                        &&
                  
                trgBoxColl->boundingBox.left + trgBoxColl->boundingBox.width > srcBoxColl->boundingBox.left + 3                                  &&
                trgBoxColl->boundingBox.left < srcBoxColl->boundingBox.left + srcBoxColl->boundingBox.width - 3
                  ){
                trgEntity->GetPosition()->y = srcBoxColl->boundingBox.top - (trgEntity->GetIntRect()->height + 2);
            }
   }
}

////  Entity : public GameObject
void Entity::SetUp(simdjson::dom::element jsonData)
{
    aState = HORIZONTALLEFT;
    /* m_shader.loadFromFile("Assets/Shader/wave.vert", Shader::Vertex); */
    /* m_shader.setUniform("texture", Shader::CurrentTexture); */
    
    
    std::string temp( jsonData["Image"].get_c_str() );
    SetTextureFromPath(temp);
    
    temp = jsonData["Name"].get_c_str();
    name = temp;
    
    m_transform.left = jsonData["Transform"].at(0).get_int64();
    m_transform.top = jsonData["Transform"].at(1).get_int64();
    m_transform.width = jsonData["Transform"].at(2).get_int64();
    m_transform.height = jsonData["Transform"].at(3).get_int64();
    
    m_sprite.setTextureRect(m_transform);
    
    m_position.x = jsonData["Position"].at(0).get_int64();
    m_position.y = jsonData["Position"].at(1).get_int64();
    
    m_sprite.setPosition( m_position );
    
    depthLevel = jsonData["DepthLevel"].get_int64();
    std::cout << " DEPTH ENTIRY" << depthLevel << std::endl;
    // Component check
    for(int i =0 ; i < jsonData["Component"].get_array().size(); i++){
        std::string tempName( jsonData["Component"].at(i)["Type"].get_c_str() );
        
        std::cout << tempName << std::endl;
        if(tempName == "Script"){
            
            Component* tempComponent =  new Script();
            Script* tempScript = (Script*)tempComponent;
            
            tempScript->speed = jsonData["Component"].at(i)["MovementSpeed"].get_double();
            tempScript->name  = tempName;
            
            components.push_back( tempComponent );
        } 
        else if(tempName == "SolidCollide"){
            
            Component* tempComponent =  new SolidCollide();
            SolidCollide* tempSolidCollide = (SolidCollide*)tempComponent;

            tempSolidCollide->name  = tempName;
            
            components.push_back( tempComponent );
        }
        else if(tempName == "BoxCollision"){
            
            Component* tempComponent =  new BoxCollision();
            BoxCollision* tempBoxCollision = (BoxCollision*)tempComponent;
            
            tempBoxCollision->staticBoundingBox.left = jsonData["Component"].at(i)["BoundingBox"].at(0).get_int64();
            tempBoxCollision->staticBoundingBox.top = jsonData["Component"].at(i)["BoundingBox"].at(1).get_int64();
            tempBoxCollision->staticBoundingBox.width = jsonData["Component"].at(i)["BoundingBox"].at(2).get_int64();
            tempBoxCollision->staticBoundingBox.height = jsonData["Component"].at(i)["BoundingBox"].at(3).get_int64();
            
            tempBoxCollision->name   = tempName;
            
            components.push_back( tempComponent );
        }
        else if(tempName == "ShaderScript"){
            
            Component* tempComponent =  new ShaderScript();
            ShaderScript* tempShaderScript = (ShaderScript*)tempComponent;

            tempShaderScript->name  = tempName;
            
            int shadCount = jsonData["Component"].at(i)["ShaderFile"].get_array().size();
            
            std::string vertsShad;
            std::string fragShad;
            switch(shadCount)
            {
                case 1:
                    vertsShad = jsonData["Component"].at(i)["ShaderFile"].at(0).get_c_str() ;
                    tempShaderScript->pShader = globalGame.assetsColl.shaderColl->GetShaderData( vertsShad );
                break;
                case 2:
                    vertsShad = jsonData["Component"].at(i)["ShaderFile"].at(0).get_c_str();
                    fragShad = jsonData["Component"].at(i)["ShaderFile"].at(1).get_c_str();
                    
                    tempShaderScript->pShader = globalGame.assetsColl.shaderColl->GetShaderData( vertsShad, fragShad);
                break;
                case 3:
                
                break;
                default:
                
                break;
            }

            tempShaderScript->pShader->setUniform("texture", Shader::CurrentTexture);
            tempShaderScript->pShader->setUniform("resolution", Vector2f(globalGame.screenSize.x , globalGame.screenSize.y ));
            components.push_back( tempComponent );
        }
        else if(tempName == "Shadow2D"){
            Component* tempComponent =  new Shadow2D();
            Shadow2D* tempShadow2D = (Shadow2D*)tempComponent;

            tempShadow2D->name  = tempName;
            
            int shadCount = jsonData["Component"].at(i)["ShaderFile"].get_array().size();
            
            std::string vertsShad;
            std::string fragShad;
            switch(shadCount)
            {
                case 1:
                    vertsShad = jsonData["Component"].at(i)["ShaderFile"].at(0).get_c_str() ;
                    tempShadow2D->pShader = globalGame.assetsColl.shaderColl->GetShaderData( vertsShad );
                break;
                case 2:
                    vertsShad = jsonData["Component"].at(i)["ShaderFile"].at(0).get_c_str();
                    fragShad = jsonData["Component"].at(i)["ShaderFile"].at(1).get_c_str();
                    
                    tempShadow2D->pShader = globalGame.assetsColl.shaderColl->GetShaderData( vertsShad, fragShad);
                break;
                case 3:
                
                break;
                default:
                
                break;
            }

            if(!othersDraw) othersDraw = true;
            
            othersRenderStates.emplace_back();
            
            tempShadow2D->pShader->setUniform("texture", Shader::CurrentTexture);
            tempShadow2D->pShader->setUniform("resolution", Vector2f(globalGame.screenSize.x , globalGame.screenSize.y ));
            components.push_back( tempComponent );
        }
         else if(tempName == "FrameAnimation"){
            
            Component* tempComponent =  new FrameAnimation();
            FrameAnimation* tempFrameAnimation = (FrameAnimation*)tempComponent;
            
            /* std::cout << jsonData["Component"].at(i)["AnimationsKind"] << std::endl; */
            int kindAnimationSize = jsonData["Component"].at(i)["AnimationsKind"].get_array().size();
            
            for(int j =0 ; j < kindAnimationSize; j++)
            {
                tempFrameAnimation->animationList.emplace_back();
                
                std::string animName( jsonData["Component"].at(i)["AnimationsKind"].at(j)["Name"].get_c_str() );
                tempFrameAnimation->animationList[j].name = animName;
                
                int frmRcSz = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].get_array().size();
                
                tempFrameAnimation->animationList[j].backWardList.reserve(frmRcSz);
                
                for(int k =0 ; k < frmRcSz; k++)
                {
                    tempFrameAnimation->animationList[j].backWardList.push_back( (frmRcSz-1) - k);
                    
                    tempFrameAnimation->animationList[j].frameRec.emplace_back();

                    tempFrameAnimation->animationList[j].frameRec[k].left = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].at(k).at(0).get_int64();
                    tempFrameAnimation->animationList[j].frameRec[k].top = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].at(k).at(1).get_int64();
                    tempFrameAnimation->animationList[j].frameRec[k].width = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].at(k).at(2).get_int64();
                    tempFrameAnimation->animationList[j].frameRec[k].height = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].at(k).at(3).get_int64();
                }
                
                /* Add the middle value of list data */
                for(int k =frmRcSz - 2 ; k > 0 ; k--)
                    tempFrameAnimation->animationList[j].backWardList.push_back( k );
                
                tempFrameAnimation->animationList[j].mode = jsonData["Component"].at(i)["AnimationsKind"].at(j)["ModeDirection"].get_int64();
                tempFrameAnimation->animationList[j].frameSpeed = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameSpeed"].get_int64();
            }
    
            tempFrameAnimation->name   = tempName;
            
            components.push_back( tempComponent );
        }
        // next will phisics, frameanimation
    }
}
Entity::~Entity(){ 
    std::cout << "Entity DESSSS" << std::endl;
    for(int i=0; i < components.size(); i++){
        if(components[i])
            delete components[i];
        
        components[i] = 0;
    }
    components.clear();
}

void Entity::SetTextureFromPath(std::string path )
{
    std::cout << "SetTextureFromPath" << std::endl;
    m_texture = globalGame.assetsColl.textureColl->GetTextureData(path);
    m_sprite.setTexture(*m_texture);
}

const Component* Entity::GetComponentByName(std::string name)
{
    for(int i=0; i < components.size(); i++)
    {
        if(components[i]->name == name)
            return components[i];
    }
    return 0;
}

void Entity::Start()
{

}

void Entity::Update( float* time )
{
    if(components.size() == 0) return;

    myTime =  time;
    for(int i=0; i < components.size(); i++)
    {
        components[i]->Execute(this);
    }
    
     /* m_shader.setUniform("pixel_threshold", 10 / 30); */
   /*  m_shader.setUniform("wave_phase", m_position.x);
    m_shader.setUniform("wave_amplitude", Vector2f(10, 10)); */

}
void Entity::Draw(RenderWindow* window)
{
    window->draw(m_sprite, rndrStates);
    if(othersDraw)
        for(int i = 0; i < othersRenderStates.size(); i++)
            window->draw(m_sprite, othersRenderStates[i]);
    //std::cout << depthLevel << std::endl;
}

Vector2f* Entity::GetPosition()
{
    return &m_position;
}

const IntRect* Entity::GetIntRect() const
{
    return &m_transform;
}
Sprite* Entity::GetSprite()
{
    return &m_sprite;
}

//  class DrawOrders
void DrawOrders::SetEntity(std::vector<Entity*>* pEntity)
{
   entitiesArray = pEntity;
}
void DrawOrders::Update( float* time )
{
    Entity* temp = entitiesArray->at(0);
    for(int i=1; i< entitiesArray->size() ; i++)
    { 
        entitiesArray->at(i)->Update( time );

        if((temp->GetPosition()->y + temp->GetIntRect()->height) > (entitiesArray->at(i)->GetPosition()->y + entitiesArray->at(i)->GetIntRect()->height) && 
                                    temp->GetPosition()->y != 0  &&
                                    temp->depthLevel >=  entitiesArray->at(i)->depthLevel )
        {
            entitiesArray->at(i-1) = entitiesArray->at(i);
            entitiesArray->at(i) = temp;
        }
        temp = entitiesArray->at(i);
    }
}

void DrawOrders::Draw(RenderWindow* window)
{
    for(int i=0; i< entitiesArray->size(); i++)
    { 
        /* window->draw( *entitiesArray->at(i)->GetSprite() ); */
        entitiesArray->at(i)->Draw( window );
    }
}

//////////////////////////////////////////////////////////////
///////////     This is Game Logic Area     //////////////////
//////////////////////////////////////////////////////////////
class Player : public Entity
{
    public : 
        Player(){}
        ~Player(){ }

    const BoxCollision* playerBoxCollision;
    Vector2f currentPos;
    
    void Start() override;
    void Update( float* time ) override;
    
};
class Enemy : public Entity
{
    public : 
        Enemy(){}
        ~Enemy(){ }

    Player* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* enemyBoxCollision;
    
    SolidCollide* solidColl;
    
       /* add some movement */
    float move;
    
    void Start() override;
    void Update( float* time ) override;
    
};
class Tent_A : public Entity
{
    public : 
        Tent_A(){}
        ~Tent_A(){ }
    
    Player* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* tentBoxCollision;
    
    SolidCollide* solidColl;
    
    void Start() override;
    void Update( float* time ) override;
};
class Tree_A : public Entity
{
    public : 
        Tree_A(){}
        ~Tree_A(){ }
    
    Entity* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* treeBoxCollision;
    
    SolidCollide* solidColl;
    
    void Start() override;
    void Update( float* time ) override;
};
class Fence : public Entity
{
    public : 
        Fence(){}
        ~Fence(){ }
    
    Entity* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* fenceBoxCollision;
    
    SolidCollide* solidColl;
    
    void Start() override;
    void Update( float* time ) override;
};
class Well : public Entity
{
    public : 
        Well(){}
        ~Well(){ }
    
    Entity* player;
    const BoxCollision* playerBoxCollision;
    const BoxCollision* wellBoxCollision;
    
    SolidCollide* solidColl;
    
    void Start() override;
    void Update( float* time ) override;
};
class Foo : public Entity
{
    public : 
        Foo(){}
        ~Foo(){ }
    
    Entity* tent;
    void Start() override;
    void Update( float* time ) override;
};

//  class Player : public Entity
void Player::Start()
{

}
void Player::Update( float* time )
{
   Entity::Update( time );
   
}
//  class Enemy : public Entity
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

//  class Tent_A : public Entity
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
//  class Tree_A : public Entity
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
//  class Fence : public Entity
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
//  class Well : public Entity
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

//  class Foo : public Entity
void Foo::Start()
{

}
void Foo::Update( float* time )
{
   Entity::Update( time );
   //std::cout << "FOO" << std::endl;
}

//////////////////////////////////////////////////////////////////////////
////                                                                  ////
////                        IMPLEMENTATION  -  END                    ////          
////                                                                  ////
//////////////////////////////////////////////////////////////////////////

int main()
{  
  simdjson::dom::parser parser;
  simdjson::dom::element dataJson = parser.load("RemorseAdventurer.json");

  /// Initialization Loading Data...
/*   int width = dataJson["MainScene"].at(1)["Camera2D"]["Transform"].at(2).get_int64();
  int height = dataJson["MainScene"].at(1)["Camera2D"]["Transform"].at(3).get_int64(); */
  globalGame.screenSize.x = dataJson["MainScene"].at(1)["Camera2D"]["Transform"].at(2).get_int64();
  globalGame.screenSize.y = dataJson["MainScene"].at(1)["Camera2D"]["Transform"].at(3).get_int64();
  
  const char* title = dataJson["MainScene"].at(0)["Title"].get_c_str();
  const char* tilePath = dataJson["MainScene"].at(2)["Entity"].at(0)["Image"].get_c_str();

  // Prepare the data, temporary using hardcoded
  //TextureCollection textureData(4);
  globalGame.assetsColl.shaderColl  =  new ShaderCollection( dataJson["MainScene"].at(0)["ShaderDataCount"].get_int64() );
  globalGame.assetsColl.textureColl = new TextureCollection( dataJson["MainScene"].at(0)["TextureDataCount"].get_int64() );
  //TextureCollection* textureData = new TextureCollection( dataJson["MainScene"].at(0)["TextureDataCount"].get_int64() );
  /* tempShader.loadFromFile("Assets/Shader/pixelate.frag", Shader::Fragment); */
  
  std::vector<Entity*> pEntity;
  std::vector<Entity*> entity;
  entity.reserve( dataJson["MainScene"].at(2)["Entity"].get_array().size() );
  pEntity.reserve( dataJson["MainScene"].at(2)["Entity"].get_array().size() );
  
  globalGame.getEntity.reserve( dataJson["MainScene"].at(2)["Entity"].get_array().size() );
  
  //Entity* ent;
  // Texture Pointing and selection of Entity name
  for(int i=0; i < entity.capacity(); i++ ){ 
      // Hardcoded because native development
      std::string name( dataJson["MainScene"].at(2)["Entity"].at(i)["Name"].get_c_str() );
      if( name == "Player" ){
          entity.push_back( new Player() );
          entity[ entity.size() -1 ]->SetUp( dataJson["MainScene"].at(2)["Entity"].at(i) );
          std::cout << "Masuk Player" << std::endl;
      }
      else if( name == "Enemy" ){
          entity.push_back( new Enemy() );
          entity[ entity.size() -1 ]->SetUp( dataJson["MainScene"].at(2)["Entity"].at(i) );
          std::cout << "Masuk Enemy" << std::endl;
      }
      else if( name == "Tent_A1") {
          entity.push_back( new Tent_A() );
          entity[ entity.size() -1 ]->SetUp( dataJson["MainScene"].at(2)["Entity"].at(i) );
      }
      else if( name == "Tree_A1") {
          entity.push_back( new Tree_A() );
          entity[ entity.size() -1 ]->SetUp( dataJson["MainScene"].at(2)["Entity"].at(i) );
      }
      else if( name == "Fence") {
          entity.push_back( new Fence() );
          entity[ entity.size() -1 ]->SetUp( dataJson["MainScene"].at(2)["Entity"].at(i) );
      }
      else if( name == "Well") {
          entity.push_back( new Well() );
          entity[ entity.size() -1 ]->SetUp( dataJson["MainScene"].at(2)["Entity"].at(i) );
      }
      else{
          entity.push_back( new Foo() );
          entity[ entity.size() -1 ]->SetUp( dataJson["MainScene"].at(2)["Entity"].at(i) );
      }
  }
  
  std::cout << entity.capacity() << std::endl;
  std::cout << entity.size() << std::endl;
  
  //
  // DepthLevel Arrangement,
      std::vector<std::vector<int>> depthPos;
      int minDepth = dataJson["MainScene"].at(0)["DepthMin"].get_int64();
      int maxDepth = dataJson["MainScene"].at(0)["DepthMax"].get_int64();
      
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

    RenderWindow window(VideoMode(globalGame.screenSize.x , globalGame.screenSize.y ), title);
    
    globalGame.pWindow = &window;
    globalGame.getEntity = pEntity;
    // StartData
    for(int i=0; i < pEntity.size(); i++ ){
            pEntity[i]->Start();
    };
    std::cout << "Start sdfs" << entity.size() << std::endl;
    
    DrawOrders drawOrder;
    drawOrder.SetEntity( &pEntity );
    
    Clock clock;
    
    while (window.isOpen())
    { 
        float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time/500;

		if (time > 40) time = 40; 

        window.clear(Color(104, 176, 53));

        Event event;
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
        delete entity[i];
    }
    entity.clear();
    pEntity.clear();
    globalGame.getEntity.clear();
    
    /* delete textureData; */
    std::cout << "LLLL" << std::endl;
    return 0;
}



