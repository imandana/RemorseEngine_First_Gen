#ifndef __GAME_MANAGER_HPP__
#define __GAME_MANAGER_HPP__

class GameManager
{
  public:
    GameManager();
    ~GameManager();
  
  const char* title;
  RenderWindow window;
  DrawOrders drawOrder;
  
  Clock clock;
 
  std::vector<Entity*> pEntity;
  std::vector<Entity*> entity;
 
  GameObject* GetGameObjectByName(const std::string &name) const;
  GameObject* GetComponentByName(const std::string &name) const;
  
  void Initialization();  
  void Run();
    
};

#endif /* __GAME_MANAGER_HPP__ */

