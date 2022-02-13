#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include <vector>

#include <SFML/Graphics.hpp>

class LineRenderer
{
  public:
    LineRenderer();
    LineRenderer(sf::Vertex , sf::Vertex );
    ~LineRenderer();
    
    sf::Vertex v1;
    sf::Vertex v2;
    
    sf::Vector2i basePoint;
    
    sf::Vertex* line[2];
};

 class GridLines
 {
     public:
     GridLines();
     ~GridLines();
     
    LineRenderer xMainLine;
    LineRenderer yMainLine;
    
    std::vector<LineRenderer> xAllLines;
    std::vector<LineRenderer> yAllLines;
    
    int gapBold = 6;
    
    sf::Vector2i allLinesInfiniteLoop = { 0 , 0 };
    sf::Vector2i lastInfiniteLoop = { 0 , 0 };
    
    sf::Vector2i basePoint = {0,0};
    sf::Vector2i normalPoint= {0,0};
    sf::Vector2i firstGap ;
    
    int boxXCount;
    int boxYCount;
    
    void Init(sf::Vector2u );
    
    void UpdateLines(int );
    void Draw(sf::RenderWindow& );
    
 };
 
class Canvas
{
  public:
    Canvas(sf::RenderWindow* );
    ~Canvas();

    GridLines grid;
    
    void RightClick(sf::Vector2i );
    void RightClickRelease();
    
    void UpdateCanvas();
    
    void Draw(sf::RenderWindow& );
};

#endif /* __CANVAS_HPP__ */
