
/* class GameOject */
GameObject::GameObject()
{
    refCount = 1;
}
GameObject::~GameObject()
{
    
}
/* void GameObject::TransformMove(float x, float y)
{
}
void GameObject::TransformMove(const Vector2f& vec2f)
{ 
} */
int GameObject::AddRef()
{
    return ++refCount;
}
int GameObject::Release()
{
	if( --refCount == 0 )
	{
		delete this;
		return 0;
	}
	return refCount;
}
const GameObject* GameObject::GetComponentByName(std::string &name)
{
    return 0;
}
const GameObject* GameObject::GetComponentByIndex(int idx)
{
    return 0;
}
void GameObject::SetActive( bool active )
{
    if( active == isActive)
        return;
    
    isActive = active;
    if( active )
    {
        globalGame->activeEntities.push_back( drawId );
        return;
    }
    
    // if set active false
    int tempId = drawId;
    
    globalGame->activeEntities[ drawId ] = globalGame->activeEntities.back();
    /// Sabar liat dikertas nanti ya...
    
    
    
    globalGame->activeEntities.pop_back();
}
/* Transformable* GameObject::GetTransformable()
{
    return 0;
} */
Vector2f* GameObject::GetPosition()
{
    return 0;
}
const IntRect* GameObject::GetIntRect() const
{
    return 0;
}
Sprite* GameObject::GetSprite()
{
    
    return 0;
}

/* class Entity : GameObject */
Entity::Entity()
{
}
Entity::~Entity()
{
    for(int i=0; i < components.size(); i++){
        if(components[i])
            delete components[i];
        
        components[i] = 0;
    }
    components.clear();
}
void Entity::SetUp(simdjson::dom::element jsonData)
{
        aState = HORIZONTALLEFT;
    /* m_shader.loadFromFile("Assets/Shader/wave.vert", Shader::Vertex); */
    /* m_shader.setUniform("texture", Shader::CurrentTexture); */
    
    
    std::string temp( jsonData["Image"].get_c_str() );
    SetTextureFromPath(temp);
    
    temp = jsonData["Name"].get_c_str();
    name = temp;
    
    isActive = jsonData["IsActive"].get_int64();
    
    std::cout<< name << std::endl;
    
    m_transform.left = jsonData["Transform"].at(0).get_int64();
    m_transform.top = jsonData["Transform"].at(1).get_int64();
    m_transform.width = jsonData["Transform"].at(2).get_int64();
    m_transform.height = jsonData["Transform"].at(3).get_int64();
    
    m_sprite.setTextureRect(m_transform);
    
    m_position.x = jsonData["Position"].at(0).get_int64();
    m_position.y = jsonData["Position"].at(1).get_int64();
    
    m_sprite.setPosition( m_position );
    transform = &m_sprite;
    
    depthLevel = jsonData["DepthLevel"].get_int64();
    std::cout << " DEPTH ENTIRY" << depthLevel << std::endl;
    
    // Component check
    for(int i =0 ; i < jsonData["Component"].get_array().size(); i++){
        std::string tempName( jsonData["Component"].at(i)["Type"].get_c_str() );
        
        std::cout << tempName << std::endl;
        if(tempName == "Script"){
            
/*             Component* tempComponent =  new Script();
            Script* tempScript = (Script*)tempComponent; */
            
/*             tempScript->Init();
            tempScript->Start(); */

            /* using 1 script for now */
            std::string scriptName( jsonData["Component"].at(i)["Scripts"].at(0).get_c_str() );
            
            std::cout << "SCRIPTTTT 1 \n";
            Script* tempScript = globalGame.scriptMngr->GetControllerScript(scriptName, this);
            
            std::cout << "SCRIPTTTT 2 \n";
            
            tempScript->name  = tempName;
            
            Component* tempComponent = (Component*)tempScript;
            components.push_back( tempComponent );
            
            std::cout << "SCRIPTTTT 3 \n";
            globalGame.scriptMngr->CreateController( tempScript, this );
            std::cout << "SCRIPTTTT 4 \n";
        } 
        else if(tempName == "SolidCollide"){
            
            Component* tempComponent =  new SolidCollide();
            SolidCollide* tempSolidCollide = (SolidCollide*)tempComponent;

            tempSolidCollide->name  = tempName;
            
            std::cout << "SOL" << std::endl;
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
            std::cout << "K" << std::endl;
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

            std::cout << "O" << std::endl;
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
            
            /* Sekarang sementara statis data untuk FrameAnimation */
            int kindAnimationSize = jsonData["Component"].at(i)["AnimationsKind"].get_array().size();
            
            /* Init The Static Frame Animation */
            tempFrameAnimation->animationList.reserve( AnimationState::COUNT );
            for(int ii = 0; ii < AnimationState::COUNT; ii++)
            {
                tempFrameAnimation->animationList.emplace_back();
            }
            
            for(int j =0 ; j < kindAnimationSize; j++)
            {
                /* tempFrameAnimation->animationList.emplace_back(); */
                
                std::string animName( jsonData["Component"].at(i)["AnimationsKind"].at(j)["Name"].get_c_str() );
                
                for(int jj=0; jj < AnimationState::COUNT; jj++)
                {
                    if( animName == animNames[jj] )
                    {
                        tempFrameAnimation->animationList[jj].name = animName;
                        
                        int frmRcSz = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].get_array().size();
                        
                        tempFrameAnimation->animationList[jj].backWardList.reserve(frmRcSz);
                        
                        for(int k =0 ; k < frmRcSz; k++)
                        {
                            tempFrameAnimation->animationList[jj].backWardList.push_back( (frmRcSz-1) - k);
                            
                            tempFrameAnimation->animationList[jj].frameRec.emplace_back();

                            tempFrameAnimation->animationList[jj].frameRec[k].left = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].at(k).at(0).get_int64();
                            tempFrameAnimation->animationList[jj].frameRec[k].top = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].at(k).at(1).get_int64();
                            tempFrameAnimation->animationList[jj].frameRec[k].width = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].at(k).at(2).get_int64();
                            tempFrameAnimation->animationList[jj].frameRec[k].height = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameRec"].at(k).at(3).get_int64();
                        }
                        
                        /* Add the middle value of list data */
                        for(int k =frmRcSz - 2 ; k > 0 ; k--)
                            tempFrameAnimation->animationList[jj].backWardList.push_back( k );
                        
                        tempFrameAnimation->animationList[jj].mode = jsonData["Component"].at(i)["AnimationsKind"].at(j)["ModeDirection"].get_int64();
                        tempFrameAnimation->animationList[jj].frameSpeed = jsonData["Component"].at(i)["AnimationsKind"].at(j)["FrameSpeed"].get_int64();
                   
                        break;
                    }
                }
            }
    
            tempFrameAnimation->name   = tempName;
            
            components.push_back( tempComponent );
        }
        else if(tempName == "SubEntities"){
            
            Component* tempComponent =  new SubEntities();
            SubEntities* tempSubEntities = (SubEntities*)tempComponent;

            tempSubEntities->name  = tempName;
            
            tempSubEntities->listSubEntities = &subEntities;
            
            for (int j=0;j<jsonData["Component"].at(i)["SubEntities"].get_array().size(); j++)
            {
                subEntities.push_back(  new Entity() );
                subEntities[ subEntities.size() -1 ]->SetUp( jsonData["Component"].at(i)["SubEntities"].at(j) );
            }
        } 
        else if(tempName == "AudioSource"){
            
            Component* tempComponent =  new AudioSource();
            AudioSource* tempAudioSource = (AudioSource*)tempComponent;

            tempAudioSource->name  = tempName;


        }
        else if(tempName == "EntityStates"){
            
            Component* tempComponent =  new EntityStates();
            EntityStates* tempEntityStates = (EntityStates*)tempComponent;

            tempEntityStates->name  = tempName;
            
            for (int j=0;j<jsonData["Component"].at(i)["States"].get_array().size(); j++)
            {
                theEntityStates.states.push_back(  j  );
                theEntityStates.states[ theEntityStates.states.size() -1 ]->name = jsonData["Component"].at(i)["States"].at(j).get_c_str();
            }
        }
        // next will phisics, frameanimation
    }
}
void Entity::SetTextureFromPath(std::string path)
{
    m_texture = globalGame.assetsColl.textureColl->GetTextureData(path);
    m_sprite.setTexture(*m_texture);
}
const Component* Entity::GetComponentByName(std::string &name)
{
    for(int i=0; i < components.size(); i++)
    {
        if(components[i]->name.compare( name ) == 0)
            return components[i];
    }
    return 0;
}

const Component* Entity::GetComponentByIndex(int idx)
{
    return components[idx];
}

void Entity::Start()
{
    if(components.size() == 0) return;

    for(int i=0; i < components.size(); i++)
    {
        components[i]->Initialize(this);
    }
}
void Entity::Update( float* time )
{
    if(components.size() == 0) return;

    myTime =  time;
    for(int i=0; i < components.size(); i++)
    {
        components[i]->Execute(this);
    }
}
void Entity::Draw(RenderWindow* window)
{
    window->draw(m_sprite, rndrStates);
    if(othersDraw)
        for(int i = 0; i < othersRenderStates.size(); i++)
            window->draw(m_sprite, othersRenderStates[i]);
}
/* Transformable* Entity::GetTransformable()
{
    return &m_sprite;
} */
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
        

/* class Component : public GameObject */
Component::Component()
{}
Component::~Component()
{}
void Component::Initialize(Entity* entity)
{
    
}

/* class DataCollection */
DataCollection::DataCollection()
{
    pathData.reserve(2000); 
}
DataCollection::DataCollection(int data)
{
    pathData.reserve(data); 
}
DataCollection::~DataCollection()
{
    pathData.clear();
}


/* class TextureCollection : public DataCollection */
TextureCollection::TextureCollection() : DataCollection()
{
    dataTexture.reserve(2000); 
}
TextureCollection::TextureCollection(int data) : DataCollection(data)
{
    dataTexture.reserve(data); 
}
TextureCollection::~TextureCollection()
{
    dataTexture.clear();
}
Texture* TextureCollection::AddTextureData(std::string path)
{
    /* Untuk memorypoolnya masih diteliti sementara standart SFML */
    dataTexture.emplace_back();
    dataTexture[dataTexture.size()-1].loadFromFile(path);
    
    pathData.push_back(path);    
    /* dataTexture.push_back(temp);    // Copy texture */
    
    return &dataTexture[dataTexture.size()-1];
}
Texture* TextureCollection::GetTextureData(std::string path)
{
    for(int i = 0; i < pathData.capacity(); i++)
    {
        if( pathData[i] == path){
            return &dataTexture[i];
        }
    }
    
    return AddTextureData(path);
}

/* class ShaderCollection : public DataCollection */
ShaderCollection::ShaderCollection() : DataCollection()
{
    dataShader.reserve(2000); 
}
ShaderCollection::ShaderCollection(int data) : DataCollection(data)
{
    dataShader.reserve(data); 
}
ShaderCollection::~ShaderCollection()
{
    dataShader.clear();
}
Shader* ShaderCollection::AddShaderData(std::string path)
{
    dataShader.push_back( new Shader() );
    dataShader[ dataShader.size() -1 ]->loadFromFile(path, sf::Shader::Vertex);
    
    pathData.push_back(path);    
    
    return dataShader[dataShader.size()-1];
}
Shader* ShaderCollection::GetShaderData(std::string path)
{
    for(int i = 0; i < pathData.capacity(); i++)
    {
        if( pathData[i] == path){
            return dataShader[i];
        }
    }
    
    return AddShaderData(path);
}
Shader* ShaderCollection::AddShaderData(std::string path, std::string path2)
{
    dataShader.push_back( new Shader() );
    dataShader[ dataShader.size() -1 ]->loadFromFile(path, path2);
    
    pathData.push_back(path);    
    
    return dataShader[dataShader.size()-1];
}
Shader* ShaderCollection::GetShaderData(std::string path, std::string path2)
{
    for(int i = 0; i < pathData.capacity(); i++)
    {
        if( pathData[i] == path){
            return dataShader[i];
        }
    }
    
    return AddShaderData(path, path2);
}

/* class SoundCollection : public DataCollection */
SoundCollection::SoundCollection() : DataCollection()
{
    
}
SoundCollection::SoundCollection(int data) : DataCollection(data)
{
    
}
SoundCollection::~SoundCollection()
{
    
}
Shader* SoundCollection::AddShaderData(std::string path)
{
    return 0;
}
Shader* SoundCollection::GetShaderData(std::string path)
{
    return 0;
}


/* class AssetsCollectionManager */
AssetsCollectionManager::AssetsCollectionManager()
{
    
}
AssetsCollectionManager::~AssetsCollectionManager()
{
    if(textureColl) delete textureColl;
    if(shaderColl) delete shaderColl;
}

/* class AnimationKind */
AnimationKind::AnimationKind()
{
    
}
AnimationKind::~AnimationKind()
{
    
}

/* class FrameAnimation : public Component */
FrameAnimation::FrameAnimation()
{
    
}
FrameAnimation::~FrameAnimation()
{
    
}
void FrameAnimation::Execute(Entity* entity) 
{
    int num;
    
    /* std::cout << " FFFFRAE " << animationList[entity->aState].mode << std::endl; */
    switch(animationList[entity->aState].mode)
    {
        case 0:
            /* Backward Animation */
            num = ( (int)entity->curAnimSpeed ) % 
                ( ( animationList[entity->aState].frameRec.size() + (animationList[entity->aState].frameRec.size()) -2 ) * animationList[entity->aState].frameSpeed);
            num /= animationList[entity->aState].frameSpeed;
         
         /* std::cout << " FFFFRAE " << entity->aState << std::endl; */
            entity->GetSprite()->setTextureRect( 
                animationList[entity->aState].frameRec[ animationList[entity->aState].backWardList[ num ]]
                );
         /* std::cout << " FFFFRAE " << entity->aState << std::endl; */
            break;
        case 1:
            /* Forward Animation */
            num = ( (int)entity->curAnimSpeed ) % 
                ( (animationList[entity->aState].frameRec.size()-1) * animationList[entity->aState].frameSpeed);   
            num /= animationList[entity->aState].frameSpeed;
            entity->GetSprite()->setTextureRect( animationList[entity->aState].frameRec[num] );
            break;
        
    }
    /* std::cout << " FFFFRAE " << entity->aState << std::endl; */
}

// class ShaderScript : public Component
ShaderScript::ShaderScript()
{
    
}
ShaderScript::~ShaderScript()
{
    
}
void ShaderScript::Execute(Entity* entity)
{
    float test = static_cast<float>( Mouse::getPosition( *globalGame.pWindow ).x ) ;
    pShader->setUniform("wave_phase", test) ;
    pShader->setUniform("wave_amplitude", Vector2f( 40, 40));

    entity->rndrStates.shader = pShader;
}

/* class Shadow2D : public Component */
Shadow2D::Shadow2D()
{
    
}
Shadow2D::~Shadow2D()
{
    
}
void Shadow2D::Execute(Entity* entity)
{
    float test = static_cast<float>( Mouse::getPosition( *globalGame.pWindow ).x ) ;
    pShader->setUniform("wave_phase", test*2.0f) ;
    pShader->setUniform("wave_amplitude", Vector2f( 40, 40));
    pShader->setUniform("sprite_size", entity->GetIntRect() );
    
    std::cout << "SHADOW " <<test << std::endl;
    entity->othersRenderStates[0].shader = pShader;
}

/* class BoxCollision : public Component */
BoxCollision::BoxCollision()
{
    
}
BoxCollision::~BoxCollision()
{
    
}
void BoxCollision::Execute(Entity* entity)
{
    boundingBox.left = staticBoundingBox.left + entity->GetPosition()->x ;
    boundingBox.top = staticBoundingBox.top + entity->GetPosition()->y;
      
    boundingBox.width = staticBoundingBox.width;
    boundingBox.height = staticBoundingBox.height;
    
}

/* class SubEntities : public Component */
SubEntities::SubEntities()
{
    
}
SubEntities::~SubEntities()
{
    
}
void SubEntities::Execute(Entity* entity)
{
    for(int i = 0; i < listSubEntities.size(); i++)
    {
        ////// WAIIITTT
    }
    
}

/* class AudioSource : public Component */
AudioSource::AudioSource()
{
    
}
AudioSource::~AudioSource()
{
    
}
void AudioSource::Execute(Entity* entity)
{
    
}
/* class EntityStates : public Component */
EntityStates::EntityStates()
{
    
}
EntityStates::~EntityStates()
{
    
}
void EntityStates::Execute(Entity* entity)
{

    
}

/* class Script : public Component */
Script::Script() : type(0), factoryFunc(0), startMethod(0), updateMethod(0)
{
}
Script::~Script()
{
    
}
void Script::Initialize(Entity* entity)
{
    std::cout << "SCRIPTTTT INITTTT \n";
    globalGame.scriptMngr->CallStart(controller[0]);
}
void Script::Update()
{
    //std::cout << "SCRIPTTTT UpDTATEE \n";
    globalGame.scriptMngr->CallUpdate(controller[0]);
}
void Script::Execute(Entity* entity)
{
    Update();
}

/* class SolidCollide : public Component */
SolidCollide::SolidCollide()
{
    
}
SolidCollide::~SolidCollide()
{
    
}
void SolidCollide::SetUp(Entity* sEntity, Entity* tEntity, const BoxCollision* sCollision , const BoxCollision* tCollision)
{
    srcEntity = sEntity;
    trgEntity = tEntity;
    
    srcBoxColl = sCollision;
    trgBoxColl =  tCollision;
}
#define SPACE 0.5f
void SolidCollide::Execute(Entity* entity)
{    
      if(  srcBoxColl->boundingBox.intersects( trgBoxColl->boundingBox )  ) {
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

/* class Camera2D : public GameObject */
Camera2D::Camera2D()
{
    
}
Camera2D::~Camera2D()
{
    
}
    
/* class DrawOrders */
DrawOrders::DrawOrders()
{
    
}
DrawOrders::~DrawOrders()
{
    
}
void DrawOrders::SwapInActiveEntity()
{
    
}
void DrawOrders::SetEntity(std::vector<Entity*>* pEntity)
{
    entitiesArray = pEntity;
    
    activeEntities.reserve( entitiesArray.size() );
    
    for(int i=0; i< entitiesArray->size(); i++)
    { 
        activeEntities.push_back( i );
    }
    
    globalGame.activeEntities = &activeEntities;
}
void DrawOrders::Update( float* time )
{
    Entity* temp = entitiesArray->at(0);
    int drawIdTemp = 0;
    
    temp->Update( time );
    
    // Gonna replace with this, test
    for(int i=1; i< activeEntities->size() ; i++)
    { 
        entitiesArray->at(i)->Update( time );

        if((temp->GetPosition()->y + temp->GetIntRect()->height) > (entitiesArray->at(i)->GetPosition()->y + entitiesArray->at(i)->GetIntRect()->height) && 
                                    temp->GetPosition()->y != 0  &&
                                    temp->depthLevel >=  entitiesArray->at(i)->depthLevel )
        {
            entitiesArray->at(i-1) = entitiesArray->at(i);
            entitiesArray->at(i) = temp;
            
            /////
            entitiesArray->at(i-1).drawId = i;
            entitiesArray->at(i).drawId = drawIdTemp;
        }
        temp = entitiesArray->at(i);
        
        //////
        drawIdTemp = i ;
    }
    //
    
    ////////////////////////////////////////////////////
    /*
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
    
    */
}
void DrawOrders::Draw(RenderWindow* window)
{
    for(int i=0; i< entitiesArray->size(); i++)
    { 
        /* window->draw( *entitiesArray->at(i)->GetSprite() ); */
        entitiesArray->at(i)->Draw( window );
    }
    
    for(int i=0; i< entitiesArray->size(); i++)
    { 
        /* window->draw( *entitiesArray->at(i)->GetSprite() ); */
        entitiesArray->at(i)->Draw( window );
    }
}

/* class CullingScreen2D */
CullingScreen2D::CullingScreen2D()
{
    
}
CullingScreen2D::~CullingScreen2D()
{
    
}
void CullingScreen2D::Init(Camera2D* camera)
{
    
}
void CullingScreen2D::CullingScreen( std::vector<Entity*>* pEntity )
{
    
}
void CullingScreen2D::Update( float* time )
{
    
}

/* class ScreenDrawManager */
ScreenDrawManager::ScreenDrawManager()
{
    
}
ScreenDrawManager::~ScreenDrawManager()
{
    
}
void ScreenDrawManager::Init( std::vector<Entity*>* pEntity, Camera2D* camera)
{
    
}
void ScreenDrawManager::Update( float* time )
{
    
}
void ScreenDrawManager::Draw(RenderWindow* window)
{
    
}

