#ifndef __GAME_FRAMEWORK_HPP__
#define __GAME_FRAMEWORK_HPP__

class GameObject
{
  public:
      GameObject();
      virtual ~GameObject();
  
      int id;
      std::string name;
};

class Entity : public GameObject
{
    public:
        Entity();
        ~Entity();

        void SetTextureFromPath(std::string );
        void Draw(sf::RenderWindow* );

        sf::Sprite m_sprite;
        sf::Texture m_texture;
        sf::Vector2i size;
};

#endif /* __GAME_FRAMEWORK_HPP__ */
