#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "MyFrameWork/RemorseWindowFrameWork.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(), "Remorse Engine Editor", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    /* Prevent Saved to imgui.ini */
    ImGui::GetIO().IniFilename = NULL;
    
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    ControllManager ctrlMgr;
    MemoryPool* memPool = new MemoryPool(2000);
    
    void* mem = memPool->RequestMemory( sizeof(MenuBarControll) );
    ControllBase* menuBar = new (mem) MenuBarControll("MenuBar");
    
    mem = memPool->RequestMemory( sizeof(ObjectWindowView) );
    ControllBase* windowObjList = new (mem) ObjectWindowView( ImVec2(300 , 500), 
                                        ImVec2( window.getSize().x - 300 , 20), "Object Hierarcy List");
    mem = memPool->RequestMemory( sizeof(ObjectWindowCanvas) );
/*     ControllBase* windowObjEdit = new (mem) WindowControll( ImVec2( window.getSize().x - 40 , 150), 
                                        ImVec2( 20 , window.getSize().y - 200) , "Object Editor"); */
    ControllBase* windowObjEdit = new (mem) ObjectWindowCanvas(ImVec2( 500 , 500), 
                                        ImVec2( 20 , window.getSize().y - 400) , "Object Editor");

    MenuBarControll* menuTemp = (MenuBarControll*)menuBar;
    menuTemp->AddControll( windowObjList );
    menuTemp->AddControll( windowObjEdit );
    
    ctrlMgr.RegisterControll( menuBar );
    ctrlMgr.RegisterControll( windowObjList );
    ctrlMgr.RegisterControll( windowObjEdit );
    
    Canvas cvs(&window);
    
    sf::Clock deltaClock;
    sf::Time deltaTime;
    
    bool rightClickHold = false;
    sf::Vector2i firstClick;
    sf::Vector2i mouseElapsed;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed || !menuBar->isOpen ) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed ) {
               if(event.mouseButton.button == sf::Mouse::Left)
               {
                   if(!rightClickHold)
                   {
                       rightClickHold = !rightClickHold;
                       firstClick = sf::Mouse::getPosition();
                       /* cvs.grid.firstGap = sf::Mouse::getPosition() - cvs.grid.basePoint; */
                   }
               }
            }
            if (event.type == sf::Event::MouseButtonReleased ) {
               if(event.mouseButton.button == sf::Mouse::Left)
               {
                    rightClickHold = !rightClickHold;
                    cvs.RightClickRelease();
                    /* firstClick = sf::Mouse::getPosition(); */
               }
            }
            
            if (event.type == sf::Event::MouseMoved ) {
                if(rightClickHold)
                {
                    cvs.RightClick( sf::Mouse::getPosition() - firstClick );
                }
            }
        }

        deltaTime = deltaClock.restart();
        ImGui::SFML::Update(window, deltaTime);
      
        window.clear(sf::Color(141,141,141));
        
        cvs.Draw( window );
        ctrlMgr.DrawControll();
        
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    
    return 0;
}
