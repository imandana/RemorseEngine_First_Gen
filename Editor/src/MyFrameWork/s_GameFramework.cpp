
GameObject::GameObject(){}
GameObject::~GameObject(){}
  

Entity::Entity(){}
Entity::~Entity(){}


void Entity::SetTextureFromPath(std::string path)
{
    m_texture.loadFromFile(path);
    m_sprite.setTexture(m_texture);
}
void Entity::Draw(sf::RenderWindow* window)
{
    /* window->draw(m_sprite, rndrStates); */
}

