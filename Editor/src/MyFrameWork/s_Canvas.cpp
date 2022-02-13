
LineRenderer::LineRenderer()
{
        line[0] = &v1;
        line[1] = &v2;
}
LineRenderer::LineRenderer(sf::Vertex pv1, sf::Vertex pv2)
        : v1(pv1), v2(pv2)
{
    line[0] = &v1;
    line[1] = &v2;
}
LineRenderer::~LineRenderer()
{
    
}

GridLines::GridLines()
{
    
}
GridLines::~GridLines()
{
    
}

void GridLines::Init(sf::Vector2u sz)
{   
    int sideSize = 10;
    boxXCount = sz.y / sideSize;;
    boxYCount = sz.x / sideSize;
    
    boxXCount += 15; /* test */
    boxYCount += 15;
    
    xMainLine.v1.position = sf::Vector2f(0, sideSize * 38 );
    xMainLine.v2.position = sf::Vector2f(sz.x,  sideSize * 38 );
    xMainLine.v1.color = xMainLine.v2.color = sf::Color(255,0,0);
    
    xMainLine.basePoint = (sf::Vector2i)xMainLine.v1.position;
    
    
    yMainLine.v1.position = sf::Vector2f( sideSize * 68, 0);
    yMainLine.v2.position = sf::Vector2f( sideSize * 68, sz.y );
    yMainLine.v1.color = yMainLine.v2.color = sf::Color(0,0,255);
    
    yMainLine.basePoint = (sf::Vector2i)yMainLine.v1.position;

    xAllLines.reserve( boxXCount );
    for(int i=0; i< boxXCount; i++)
    {
        xAllLines.emplace_back();
    }
    for(int i=0; i< boxXCount; i++)
    {
        xAllLines[i].v1.position = sf::Vector2f(0, sideSize - 50 ); /* This 50 is for minus position line of y */
        xAllLines[i].v2.position = sf::Vector2f(sz.x, sideSize - 50);
        
        xAllLines[i].basePoint = (sf::Vector2i)xAllLines[i].v1.position;
        
        if(i % gapBold == 0){
            xAllLines[i].v1.color = xAllLines[i].v2.color = sf::Color(255, 255, 255, 50);
        }
        else{
            xAllLines[i].v1.color = xAllLines[i].v2.color = sf::Color(255, 255, 255, 20);
        }
        sideSize += 10;
    }
    
    sideSize = 10;
    yAllLines.reserve( boxYCount );
    for(int i=0; i< boxYCount; i++)
    {
        yAllLines.emplace_back();
    }
    for(int i=0; i< boxYCount; i++)
    {
        yAllLines[i].v1.position = sf::Vector2f(sideSize - 50, 0 ); /* same here */
        yAllLines[i].v2.position = sf::Vector2f(sideSize - 50, sz.y );
        
        yAllLines[i].basePoint = (sf::Vector2i)yAllLines[i].v1.position;
        
        if(i % gapBold == 0){
            yAllLines[i].v1.color = yAllLines[i].v2.color = sf::Color(255, 255, 255, 75);
        }
        else{
            yAllLines[i].v1.color = yAllLines[i].v2.color = sf::Color(255, 255, 255, 50);
        }
        
        sideSize += 10;
    }
 }
    
void GridLines::UpdateLines(int point)
{
    
}
void GridLines::Draw(sf::RenderWindow& window)
{   
     for(int i=0; i< boxXCount; i++)
     {  
         window.draw(*xAllLines[i].line, 2, sf::Lines);
     }
     
     for(int i=0; i< boxYCount; i++)
     {
         window.draw(*yAllLines[i].line, 2, sf::Lines);
     }
     
    window.draw(*xMainLine.line, 2, sf::Lines);
    window.draw(*yMainLine.line, 2, sf::Lines);
}
 
Canvas::Canvas(sf::RenderWindow* window)
{
    sf::Vector2u sz = window->getSize();
    
    grid.Init(sz);

}
Canvas::~Canvas()
{
    
}

void Canvas::RightClick(sf::Vector2i moved)
{
    /* Warning IsAnyWindowFocused() gonna obsolete */
    if(!ImGui::IsAnyWindowFocused())
    {
        
        /* grid.yMainLine.v1.color = grid.yMainLine.v2.color = sf::Color(0,255,0); */
        grid.xMainLine.v1.position = sf::Vector2f( 
                            0, grid.xMainLine.basePoint.y + moved.y );
        grid.xMainLine.v2.position = sf::Vector2f(
                            grid.xMainLine.v2.position.x, grid.xMainLine.basePoint.y + moved.y );

        
        grid.yMainLine.v1.position = sf::Vector2f( 
                            grid.yMainLine.basePoint.x + moved.x, 0 );
        grid.yMainLine.v2.position = sf::Vector2f(
                            grid.yMainLine.basePoint.x + moved.x, grid.yMainLine.v2.position.y );
        
        /* grid.allLinesInfiniteLoop = moved - grid.lastInfiniteLoop; */
         for(int i=0; i< grid.boxXCount; i++)
         { 
/*                                                // this 10 value is SideSize, for now Hardcoded
            if( grid.allLinesInfiniteLoop.y % 60 == 0 )
            {
                grid.allLinesInfiniteLoop.y = 0;
                grid.lastInfiniteLoop.y = moved.y;
            } */
            grid.xAllLines[i].v1.position = sf::Vector2f( 
                                0, grid.xAllLines[i].basePoint.y + moved.y );
            grid.xAllLines[i].v2.position = sf::Vector2f(
                           grid.xAllLines[i].v2.position.x, grid.xAllLines[i].basePoint.y + moved.y );
         }
         
         for(int i=0; i< grid.boxYCount; i++)
         {
/*                                                 // this 10 value is SideSize, for now Hardcoded
            if( grid.allLinesInfiniteLoop.x % 60 == 0 )
            {
                grid.allLinesInfiniteLoop.x = 0;
                grid.lastInfiniteLoop.x  = moved.x;
            } */
            grid.yAllLines[i].v1.position = sf::Vector2f( 
                                grid.yAllLines[i].basePoint.x + moved.x, 0 );
            grid.yAllLines[i].v2.position = sf::Vector2f(
                                grid.yAllLines[i].basePoint.x + moved.x, grid.yAllLines[i].v2.position.y );
         }
    } 
}
void Canvas::RightClickRelease()
{
    grid.xMainLine.basePoint = (sf::Vector2i)grid.xMainLine.v1.position;
    grid.yMainLine.basePoint = (sf::Vector2i)grid.yMainLine.v1.position;
    
    for(int i=0; i< grid.boxXCount; i++)
    {
        grid.xAllLines[i].basePoint = (sf::Vector2i)grid.xAllLines[i].v1.position;
    }
    for(int i=0; i< grid.boxYCount; i++)
    {
        grid.yAllLines[i].basePoint = (sf::Vector2i)grid.yAllLines[i].v1.position;
    }

}
    
void Canvas::UpdateCanvas()
{

}
    
void Canvas::Draw(sf::RenderWindow& window)
{

    grid.Draw(window);
}

