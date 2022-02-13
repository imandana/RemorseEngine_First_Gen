// c++ standart
#include <iostream>

// SFML Library
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// AngelScript
#include <angelscript.h>

// SimdJson
#include <simdjson.h>

//////////////////////////////////////////////////////////////////////////
////                                                                  ////
////                        NAMESPACE DECLARATION                     ////          
////                                                                  ////
//////////////////////////////////////////////////////////////////////////
using namespace sf;

/* Our Remorse Framework */
#include "RemorseGameFramework/RemorseGameFramework.h"


int main()
{  
    #ifndef REMORSE_DEBUG
        std::cout << "GameManager Init" << std::endl;
    #endif
    GameManager* remorse = new GameManager();
    
    globalGame.gameMgr = remorse;
    
    remorse->Initialization();
    remorse->Run();
    
    delete remorse;
    return 0;
}



