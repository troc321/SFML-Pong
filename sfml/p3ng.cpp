#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <sstream>
 
using namespace sf;

//checks collision with FloatRect type's "intersects" built-in method
bool collides (const RectangleShape & rect1,const RectangleShape & rect2)
{
    FloatRect r1=rect1.getGlobalBounds();
    FloatRect r2=rect2.getGlobalBounds();
    return r1.intersects (r2);
}

//returns bounds of window
int bounds (const int x, const int a, const int b)
{
    return std::min(std::max(a,x),b);
}
 
int main()
{
    const int width = 640;
    const int height= 480;
    const int borderSize= 12;
    const int margin = 50;
    const int moveSpeedP1 = 3;
    const int moveSpeedP2 = 1;
 
    VideoMode videoMode(width, height);
    RenderWindow window(videoMode,"P3ng");
 
    Font font;
    if (!font.loadFromFile("SourceSansPro-Black.ttf"))
        return EXIT_FAILURE;     
    
    //Court "Object"
    RectangleShape top;
    RectangleShape left;
    RectangleShape right;
    RectangleShape bottom;    
 
    //make "sprites" and court lines
    top.setPosition(0, 0);
    top.setSize(Vector2f(width, borderSize));
 
    left.setPosition(-borderSize, 0);
    left.setSize(Vector2f(borderSize, height));
 
    right.setPosition(width, 0);
    right.setSize(Vector2f(borderSize, height));
 
    bottom.setPosition(0, height-borderSize);
    bottom.setSize(Vector2f(width, borderSize));
             
    top.setFillColor(Color::Yellow);
    left.setFillColor(Color::Yellow);
    right.setFillColor(Color::Yellow);
    bottom.setFillColor(Color::Yellow);
 
    RectangleShape ball;
    ball.setPosition(width/2, height/2);
    ball.setSize(Vector2f(20, 20));
    ball.setFillColor(Color::White);
 
    Vector2<float> ballSpeed(0.02,0.02);
 
    RectangleShape p1;    
    p1.setSize(Vector2f(borderSize, 90));
    p1.setPosition(margin-borderSize, height/2-25);
    p1.setFillColor(Color::Yellow);
 
    RectangleShape p2;    
    p2.setSize(Vector2f(borderSize, 90));
    p2.setPosition(width-margin, height/2-25);
    p2.setFillColor(Color::Yellow);
    
    //Setup text to be displayed at different states
    Text title("P3ng",font,50);
    title.setPosition(width/2-title.getGlobalBounds().width/2,100);
    title.setColor(Color::Yellow); 
     
    Text start("Press G to start or ESC to quit",font,30);
    start.setPosition(width/2-start.getGlobalBounds().width/2,400);
    start.setColor(Color::White);

    Text scoring("First to score 5 wins, must win by 2!",font,30);
    start.setPosition(width/2-start.getGlobalBounds().width/2,250);
    start.setColor(Color::White);
 
    Text won("\t\t\t\t\t   Human won! \n Press G to play again or ESC to quit!",font,30);
    won.setPosition(80,150);
    won.setColor(Color::Green);
 
    Text lost("\t\t\t\t\t  Computer won! \n Press G to play again or ESC to quit!",font,30);
    lost.setPosition(80,150);
    lost.setColor(Color::Red);
 
    Text score("0           0",font,50);
    score.setPosition(width/2-score.getGlobalBounds().width/2,40);
    score.setColor(Color::Yellow);
 
    int p1Score=0;
    int p2Score=0;

    //SFXs found @ https://mixkit.co/free-sound-effects/game/
    //Score sound (On web as: "Arcade video game bonus", Downloads as: mixkit-arcade-video-game-bonus-2044.wav)
    SoundBuffer buffer1;
    buffer1.loadFromFile("score.wav");
    Sound soundScore;
    soundScore.setBuffer(buffer1);

    //paddle volley sound (On web as: "Electronic retro block hit", Downloads as: mixkit-electronic-retro-block-hit-2185.wav)
    SoundBuffer buffer2;
    buffer2.loadFromFile("hit.wav");
    Sound soundHit;
    soundHit.setBuffer(buffer2);
 
    enum states {INTRO, GAME, HWON, CWON};
     
    int gameState=INTRO;
 
    while (window.isOpen())
    {
        window.clear(Color::Black);
        switch(gameState)
        {
            case INTRO:
                window.draw(title);
                window.draw(start);
                window.draw(scoring);
                break;
            case GAME:
                window.draw(left);
                window.draw(right);            
                window.draw(p1);    
                window.draw(p2);    
                window.draw(ball);    
                window.draw(score);    
                window.draw(top);
                window.draw(bottom);        
                break;
            case HWON:
                p1Score=0;
                p2Score=0;
                score.setString("0           0");
                window.draw(won);
                break;
            case CWON:
                p1Score=0;
                p2Score=0;
                score.setString("0           0");
                window.draw(lost);
                break;
        }
        window.display();
         
        Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == Event::Closed) || ((event.type == Event::KeyPressed) && (event.key.code==Keyboard::Escape)))
                window.close();    
            else
            if (((gameState == INTRO) || (gameState == HWON) || (gameState == CWON)) && (event.type == Event::KeyPressed) && (event.key.code==Keyboard::G))
                gameState=GAME;
        }

        //catch to make sure state is GAME before proceeding
        if (gameState!=GAME)
            continue;
 
        //Auto move p2
        if (ball.getPosition().y < p2.getPosition().y)
            p2.move(0, -moveSpeedP2/50.0);
        else if(ball.getPosition().y+ball.getSize().y > p2.getPosition().y+p2.getSize().y)
            p2.move(0, moveSpeedP2/25.0);

        
        //Move p1
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            p1.move(0,-moveSpeedP1/50.0);
        } else
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            p1.move(0,moveSpeedP2/25.0);
        }

        //Keep players in area
        if (collides(p1,bottom) || collides(p1,top) )
        {
            FloatRect t=top.getGlobalBounds();
            FloatRect b=bottom.getGlobalBounds();
            Vector2f p=p1.getPosition();
            p.y=bounds(p.y,t.top+t.height+5,b.top-p1.getSize().y-5);
            p1.setPosition(p);
        }
        if (collides(p2,bottom) || collides(p2,top) )
        {
            FloatRect t=top.getGlobalBounds();
            FloatRect b=bottom.getGlobalBounds();
            Vector2f p=p2.getPosition();
            p.y=bounds(p.y,t.top+t.height+5,b.top-p2.getSize().y-5);
            p2.setPosition(p);
        }

        //If ball collides with walls
        if (collides(ball,top))
        {
            FloatRect t=top.getGlobalBounds();
            FloatRect b=ball.getGlobalBounds();
            ballSpeed.y=-ballSpeed.y;
            int u = t.top + t.height - b.top;
            ball.move(0,2*u);            
        }
        if (collides(ball,bottom) )
        {
            FloatRect bot= bottom.getGlobalBounds();
            FloatRect b= ball.getGlobalBounds();
            ballSpeed.y=-ballSpeed.y;
            int u = bot.top - b.height - b.top;
            ball.move(0,2*u);   
        }

        //If ball collides with p1 or p2
        if (collides(ball,p1))
        {
            soundHit.play();
            FloatRect p= p1.getGlobalBounds();
            FloatRect b= ball.getGlobalBounds();
            ballSpeed.x= -ballSpeed.x;
            ballSpeed.y= (b.top+b.height/2 - p.top - p.height/2) / 1000;
            int u = p.left +  p.width - b.left;
            b.left = p.left +  p.width + u;
            ball.setPosition(b.left,b.top);
            ballSpeed.x=ballSpeed.x*1.05;
            ballSpeed.y=ballSpeed.y*1.05;
        }
        if (collides(ball,p2) )
        {
            soundHit.play();
            FloatRect p=p2.getGlobalBounds();
            FloatRect b=ball.getGlobalBounds();
            ballSpeed.x=-ballSpeed.x;
            ballSpeed.y= (b.top+b.height/2 - p.top - p.height/2) / 1000;
            int u = b.left + b.width - p.left;
            b.left = p.left -  b.width - u;
            ball.setPosition(b.left,b.top);
            ballSpeed.x=ballSpeed.x*1.05;
            ballSpeed.y=ballSpeed.y*1.05;
        }                        
         
        //Scoring
        if (collides(ball,left))
        {
            soundScore.play();
            p2Score++;
            std::stringstream str;
            str << p1Score << "           " << p2Score;
            score.setString(str.str());
            score.setPosition(width/2-score.getGlobalBounds().width/2,40);

            //Reset ball
            ball.setPosition(width/2, height/2);
            ballSpeed.x = -0.03;
            ballSpeed.y = 0.03;
 
        }
        if (collides(ball,right))
        {
            soundScore.play();
            p1Score++;
            std::stringstream str;
            str << p1Score << "           " << p2Score;
            score.setString(str.str());
            score.setPosition(width/2-score.getGlobalBounds().width/2,40);

            //Reset ball
            ball.setPosition(width/2, height/2);
            ballSpeed.x = 0.03;
            ballSpeed.y = 0.03;
        }

        if (p1Score >=5 && p1Score >= p2Score +2)
            gameState=HWON;
        if (p2Score >=5 && p2Score >= p1Score +2)
            gameState=CWON;
 
        ball.move(ballSpeed.x,ballSpeed.y);
 
         
    }
    return EXIT_SUCCESS;
}