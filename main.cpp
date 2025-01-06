#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <ctime>
#include <time.h>
#include <fstream>

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 320;
const int SCREEN_BPP = 32;

const int FRAMES_PER_SECOND = 20;

SDL_Joystick *stick = NULL;
SDL_Joystick *stick2 = NULL;

const int mario_WIDTH = 24;
const int mario_HEIGHT = 32;

const int luigi_WIDTH = 24;
const int luigi_HEIGHT = 32;

int num=0;
bool via=true;
int x_=0;
int y_=0;

int gravita_mario=0;
int gravita_luigi=0;

bool sinistra=false;

const int LEVEL_WIDTH = 480;
const int LEVEL_HEIGHT = 320;

const int Mario_RIGHT = 0;
const int Mario_LEFT = 1;

const int Luigi_RIGHT = 0;
const int Luigi_LEFT = 1;

SDL_Surface *mario = NULL;
SDL_Surface *mario_move0 = NULL;
SDL_Surface *mario_move1 = NULL;
SDL_Surface *mario_move2 = NULL;
SDL_Surface *mario_moveLeft0 = NULL;
SDL_Surface *mario_moveLeft1 = NULL;
SDL_Surface *mario_moveLeft2 = NULL;

SDL_Surface *luigi = NULL;
SDL_Surface *luigi_move0 = NULL;
SDL_Surface *luigi_move1 = NULL;
SDL_Surface *luigi_move2 = NULL;
SDL_Surface *luigi_moveLeft0 = NULL;
SDL_Surface *luigi_moveLeft1 = NULL;
SDL_Surface *luigi_moveLeft2 = NULL;

SDL_Surface *block = NULL;
SDL_Surface *blocco = NULL;
SDL_Surface *pavimento = NULL;
SDL_Surface *screen = NULL;

SDL_Event event;

SDL_Rect wall[23];
SDL_Rect blocchi_vincita[1];

class Mario
{
    private:
    SDL_Rect mario_;

    int offSet;
    
    int x_mario_Vel,y_mario_Vel;
    
    int mario_y;

    int mario_velocity;
    
     int frame;

     int status;
     
    public:
           
    Mario();

    void handle_input_mario();

    void move_mario();

    void show_mario();
};

class Luigi
{
    private:
    
    SDL_Rect luigi_;
    
    int offSet;
    
    int xVel,yVel;
    
    int y;

    int velocity;
    
     int frame;

     int status;
     
    public:
           
    Luigi();

    void handle_input_luigi();

    void move_luigi();

    void show_luigi();
};

class Timer
{
    private:
    int startTicks;

    int pausedTicks;

    bool paused;
    bool started;

    public:

    Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    int get_ticks();

    bool is_started();
    bool is_paused();
};

SDL_Surface *load_image( std::string filename )
{
    SDL_Surface* loadedImage = NULL;

    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );

        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0xFF, 0, 0xFF ) );
        }
    }

    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, clip, destination, &offset );
}

bool check_collision( SDL_Rect A, SDL_Rect B )
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    if( bottomA <= topB )
    {
        return false;
    }
    if( topA >= bottomB )
    {
        return false;
    }
    if( rightA <= leftB )
    {
        return false;
    }
    if( leftA >= rightB )
    {
        return false;
    }

    return true;
}

bool init()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    if( screen == NULL )
    {
        return false;
    }
    

    stick = SDL_JoystickOpen( 0 );

    

    SDL_WM_SetCaption( "Super mario", NULL );

    return true;
}

bool load_files()
{
    mario = load_image( "mario.bmp" );

    if( mario == NULL )
    {
        return false;
    }
    
    luigi = load_image( "Mario\\luigi.bmp" );

    if( luigi == NULL )
    {
        return false;
    }
    
    block = load_image( "IMmagini\\gnd_red2.bmp" );

    if( block == NULL )
    {
        return false;
    }
    
    blocco = load_image( "IMmagini\\blockq_0.bmp" );

    if( blocco == NULL )
    {
        return false;
    }
    
    pavimento = load_image( "IMmagini\\gnd_red_1.bmp" );

    if( pavimento == NULL )
    {
        return false;
    }
    
    mario_move0 = load_image( "Mario\\mario_move0.bmp" );

    if( mario_move0 == NULL )
    {
        return false;
    }
    
    mario_move1 = load_image( "Mario\\mario_move1.bmp" );

    if( mario_move1 == NULL )
    {
        return false;
    }
    
    mario_move2 = load_image( "Mario\\mario_move2.bmp" );

    if( mario_move2 == NULL )
    {
        return false;
    }
    
    mario_moveLeft0 = load_image( "Mario\\mario_moveLeft0.bmp" );

    if( mario_moveLeft0 == NULL )
    {
        return false;
    }
    
    mario_moveLeft1 = load_image( "Mario\\mario_moveLeft1.bmp" );

    if( mario_moveLeft1 == NULL )
    {
        return false;
    }
    
    mario_moveLeft2 = load_image( "Mario\\mario_moveLeft2.bmp" );

    if( mario_moveLeft2 == NULL )
    {
        return false;
    }
    
    luigi_move0 = load_image( "Mario\\luigi_move0.bmp" );

    if( luigi_move0 == NULL )
    {
        return false;
    }
    
    luigi_move1 = load_image( "Mario\\luigi_move1.bmp" );

    if( luigi_move1 == NULL )
    {
        return false;
    }
    
    luigi_move2 = load_image( "Mario\\luigi_move2.bmp" );

    if( luigi_move2 == NULL )
    {
        return false;
    }
    
    luigi_moveLeft0 = load_image( "Mario\\luigi_moveLeft0.bmp" );

    if( luigi_moveLeft0 == NULL )
    {
        return false;
    }
    
    luigi_moveLeft1 = load_image( "Mario\\luigi_moveLeft1.bmp" );

    if( luigi_moveLeft1 == NULL )
    {
        return false;
    }
    
    luigi_moveLeft2 = load_image( "Mario\\luigi_moveLeft2.bmp" );

    if( luigi_moveLeft2 == NULL )
    {
        return false;
    }

    return true;
}

void clean_up()
{
     
     
    SDL_FreeSurface( mario );
    SDL_FreeSurface( luigi );
    SDL_FreeSurface( block );
    SDL_FreeSurface( blocco );
    SDL_FreeSurface( pavimento );
    SDL_FreeSurface( mario_move0 );
    SDL_FreeSurface( mario_move1 );
    SDL_FreeSurface( mario_move2 );
    SDL_FreeSurface( mario_moveLeft0 );
    SDL_FreeSurface( mario_moveLeft1 );
    SDL_FreeSurface( mario_moveLeft2 );
    SDL_FreeSurface( luigi_move0 );
    SDL_FreeSurface( luigi_move1 );
    SDL_FreeSurface( luigi_move2 );
    SDL_FreeSurface( luigi_moveLeft0 );
    SDL_FreeSurface( luigi_moveLeft1 );
    SDL_FreeSurface( luigi_moveLeft2 );
    
    SDL_Quit();
}

Mario::Mario()
{
    mario_.x = 64;
    mario_.y = 384;
    
    offSet = 0;
    mario_velocity = 0;

    frame = 0;
    
    status=Mario_RIGHT;
    
    mario_.w = mario_WIDTH;
    mario_.h = mario_HEIGHT;

    x_mario_Vel = 0;
    y_mario_Vel = 0;
}

Luigi::Luigi()
{
    luigi_.x = 416;
    luigi_.y = 384;
    
    offSet = 0;
    velocity = 0;

    frame = 0;
    
    status=Luigi_RIGHT;
    
    luigi_.w = luigi_WIDTH;
    luigi_.h = luigi_HEIGHT;

    xVel = 0;
    yVel = 0;
}

void Mario::handle_input_mario()
{
    if( event.type == SDL_JOYAXISMOTION )
    
        if( event.jaxis.which == 0 )
        {
            if( event.jaxis.axis == 0 )
            {
                if( ( event.jaxis.value > -8000 ) && ( event.jaxis.value < 8000 ) )
                {
                    x_mario_Vel = 0;
                    mario_velocity=0;
                }
                else
                {
                    if( event.jaxis.value < 0 )
                    {
                        x_mario_Vel = -mario_WIDTH / 2;
                        mario_velocity  = -mario_WIDTH / 2;
                    }
                    else
                    {
                        x_mario_Vel = mario_WIDTH / 2;
                        mario_velocity  = mario_WIDTH / 2;
                    }
                }
            }
            }
    int dst;
    
    if(SDL_JoystickGetButton(stick, 2)){ 
                   if(((mario_.y+mario_HEIGHT)>=SCREEN_HEIGHT-64))
                   {
                   gravita_mario=-16;
                   } 
                   if(mario_.x>=0&&(mario_.y+mario_HEIGHT)<=32&&mario_.x<=96)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=128&&(mario_.y+mario_HEIGHT)<=32&&mario_.x<=160)
                   {
                   gravita_mario=-16;
                   } 
                   if(mario_.x>=256&&(mario_.y+mario_HEIGHT)<=32&&mario_.x<=352)
                   {
                   gravita_mario=-16;
                   } 
                   if(mario_.x>=384&&(mario_.y+mario_HEIGHT)<=32&&mario_.x<=480)
                   {
                   gravita_mario=-16;
                   }
                   
                   if(mario_.x>=128&&(mario_.y+mario_HEIGHT)<=64&&mario_.x<=192)
                   {
                   gravita_mario=-16;
                   }
  
                   if(mario_.x>=320&&(mario_.y+mario_HEIGHT)<=64&&mario_.x<=352)
                   {
                   gravita_mario=-16;
                   }    
                   
                   if(mario_.x>=0&&(mario_.y+mario_HEIGHT)<=96&&mario_.x<=32)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=192&&(mario_.y+mario_HEIGHT)<=96&&mario_.x<=224)
                   {
                   gravita_mario=-16;
                   }
                   
                   if(mario_.x>=32&&(mario_.y+mario_HEIGHT)<=128&&mario_.x<=64)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=224&&(mario_.y+mario_HEIGHT)<=128&&mario_.x<=256)
                   {
                   gravita_mario=-16;
                   }        
                   if(mario_.x>=416&&(mario_.y+mario_HEIGHT)<=128&&mario_.x<=480)
                   {
                   gravita_mario=-16;
                   } 
                   
                   if(mario_.x>=32&&(mario_.y+mario_HEIGHT)<=160&&mario_.x<=128)
                   {
                   gravita_mario=-16;
                   } 
                   if(mario_.x>=160&&(mario_.y+mario_HEIGHT)<=160&&mario_.x<=192)
                   {
                   gravita_mario=-16;
                   }   
                   if(mario_.x>=352&&(mario_.y+mario_HEIGHT)<=160&&mario_.x<=384)
                   {
                   gravita_mario=-16;
                   }
                   
                   if(mario_.x>=32&&(mario_.y+mario_HEIGHT)<=192&&mario_.x<=64)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=192&&(mario_.y+mario_HEIGHT)<=192&&mario_.x<=224)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=256&&(mario_.y+mario_HEIGHT)<=192&&mario_.x<=320)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=352&&(mario_.y+mario_HEIGHT)<=192&&mario_.x<=480)
                   {
                   gravita_mario=-16;
                   } 
                   }
                   
  if( event.type == SDL_KEYDOWN )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_a: x_mario_Vel -= mario_WIDTH / 2; 
                            mario_velocity-=mario_WIDTH/2; break;
            case SDLK_d: x_mario_Vel += mario_WIDTH / 2;
                             mario_velocity+=mario_WIDTH/2; break;
        }
    }
    else if( event.type == SDL_KEYUP )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_a: x_mario_Vel += mario_WIDTH / 2;
                             mario_velocity+=mario_WIDTH/2; break;
            case SDLK_d: x_mario_Vel -= mario_WIDTH / 2; 
                             mario_velocity-=mario_WIDTH/2; break; break;
        }
    }

    if( event.type == SDL_KEYDOWN )
    {
    switch( event.key.keysym.sym )
    {
    case SDLK_w:if(((mario_.y+mario_HEIGHT)>=SCREEN_HEIGHT-64))
                   {
                   gravita_mario=-16;
                   } 
                   if(mario_.x>=0&&(mario_.y+mario_HEIGHT)<=32&&mario_.x<=96)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=128&&(mario_.y+mario_HEIGHT)<=32&&mario_.x<=160)
                   {
                   gravita_mario=-16;
                   } 
                   if(mario_.x>=256&&(mario_.y+mario_HEIGHT)<=32&&mario_.x<=352)
                   {
                   gravita_mario=-16;
                   } 
                   if(mario_.x>=384&&(mario_.y+mario_HEIGHT)<=32&&mario_.x<=480)
                   {
                   gravita_mario=-16;
                   }
                   
                   if(mario_.x>=128&&(mario_.y+mario_HEIGHT)<=64&&mario_.x<=192)
                   {
                   gravita_mario=-16;
                   }
  
                   if(mario_.x>=320&&(mario_.y+mario_HEIGHT)<=64&&mario_.x<=352)
                   {
                   gravita_mario=-16;
                   }    
                   
                   if(mario_.x>=0&&(mario_.y+mario_HEIGHT)<=96&&mario_.x<=32)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=192&&(mario_.y+mario_HEIGHT)<=96&&mario_.x<=224)
                   {
                   gravita_mario=-16;
                   }
                   
                   if(mario_.x>=32&&(mario_.y+mario_HEIGHT)<=128&&mario_.x<=64)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=224&&(mario_.y+mario_HEIGHT)<=128&&mario_.x<=256)
                   {
                   gravita_mario=-16;
                   }        
                   if(mario_.x>=416&&(mario_.y+mario_HEIGHT)<=128&&mario_.x<=480)
                   {
                   gravita_mario=-16;
                   } 
                   
                   if(mario_.x>=32&&(mario_.y+mario_HEIGHT)<=160&&mario_.x<=128)
                   {
                   gravita_mario=-16;
                   } 
                   if(mario_.x>=160&&(mario_.y+mario_HEIGHT)<=160&&mario_.x<=192)
                   {
                   gravita_mario=-16;
                   }   
                   if(mario_.x>=352&&(mario_.y+mario_HEIGHT)<=160&&mario_.x<=384)
                   {
                   gravita_mario=-16;
                   }
                   
                   if(mario_.x>=32&&(mario_.y+mario_HEIGHT)<=192&&mario_.x<=64)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=192&&(mario_.y+mario_HEIGHT)<=192&&mario_.x<=224)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=256&&(mario_.y+mario_HEIGHT)<=192&&mario_.x<=320)
                   {
                   gravita_mario=-16;
                   }
                   if(mario_.x>=352&&(mario_.y+mario_HEIGHT)<=192&&mario_.x<=480)
                   {
                   gravita_mario=-16;
                   } 
                   break;
                   }
                   }                     
}

void Luigi::handle_input_luigi()
{
    if( event.type == SDL_KEYDOWN )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_LEFT: xVel -= luigi_WIDTH / 2; 
                            velocity-=luigi_WIDTH/2; break;
            case SDLK_RIGHT: xVel += luigi_WIDTH / 2;
                             velocity+=luigi_WIDTH/2; break;
        }
    }
    else if( event.type == SDL_KEYUP )
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_LEFT: xVel += luigi_WIDTH / 2;
                             velocity+=luigi_WIDTH/2; break;
            case SDLK_RIGHT: xVel -= luigi_WIDTH / 2; 
                             velocity-=luigi_WIDTH/2; break; break;
        }
    }

    int dst;
    if( event.type == SDL_KEYDOWN )
    {
    switch( event.key.keysym.sym )
    {
    case SDLK_UP:if(((luigi_.y+luigi_HEIGHT)>=SCREEN_HEIGHT-64))
                   {
                   gravita_luigi=-16;
                   }
                   if(luigi_.x>=0&&(luigi_.y+luigi_HEIGHT)<=32&&luigi_.x<=96)
                   {
                   gravita_luigi=-16;
                   }
                   if(luigi_.x>=128&&(luigi_.y+luigi_HEIGHT)<=32&&luigi_.x<=160)
                   {
                   gravita_luigi=-16;
                   } 
                   if(luigi_.x>=256&&(luigi_.y+luigi_HEIGHT)<=32&&luigi_.x<=352)
                   {
                   gravita_luigi=-16;
                   } 
                   if(luigi_.x>=384&&(luigi_.y+luigi_HEIGHT)<=32&&luigi_.x<=480)
                   {
                   gravita_luigi=-16;
                   }
                   
                   if(luigi_.x>=128&&(luigi_.y+luigi_HEIGHT)<=64&&luigi_.x<=192)
                   {
                   gravita_luigi=-16;
                   }
  
                   if(luigi_.x>=320&&(luigi_.y+luigi_HEIGHT)<=64&&luigi_.x<=352)
                   {
                   gravita_luigi=-16;
                   }    
                   
                   if(luigi_.x>=0&&(luigi_.y+luigi_HEIGHT)<=96&&luigi_.x<=32)
                   {
                   gravita_luigi=-16;
                   }
                   if(luigi_.x>=192&&(luigi_.y+luigi_HEIGHT)<=96&&luigi_.x<=224)
                   {
                   gravita_luigi=-16;
                   }
                   
                   if(luigi_.x>=32&&(luigi_.y+luigi_HEIGHT)<=128&&luigi_.x<=64)
                   {
                   gravita_luigi=-16;
                   }
                   if(luigi_.x>=224&&(luigi_.y+luigi_HEIGHT)<=128&&luigi_.x<=256)
                   {
                   gravita_luigi=-16;
                   }        
                   if(luigi_.x>=416&&(luigi_.y+luigi_HEIGHT)<=128&&luigi_.x<=480)
                   {
                   gravita_luigi=-16;
                   } 
                   
                   if(luigi_.x>=32&&(luigi_.y+luigi_HEIGHT)<=160&&luigi_.x<=128)
                   {
                   gravita_luigi=-16;
                   } 
                   if(luigi_.x>=160&&(luigi_.y+luigi_HEIGHT)<=160&&luigi_.x<=192)
                   {
                   gravita_luigi=-16;
                   }   
                   if(luigi_.x>=352&&(luigi_.y+luigi_HEIGHT)<=160&&luigi_.x<=384)
                   {
                   gravita_luigi=-16;
                   }
                   
                   if(luigi_.x>=32&&(luigi_.y+luigi_HEIGHT)<=192&&luigi_.x<=64)
                   {
                   gravita_luigi=-16;
                   }
                   if(luigi_.x>=192&&(luigi_.y+luigi_HEIGHT)<=192&&luigi_.x<=224)
                   {
                   gravita_luigi=-16;
                   }
                   if(luigi_.x>=256&&(luigi_.y+luigi_HEIGHT)<=192&&luigi_.x<=320)
                   {
                   gravita_luigi=-16;
                   }
                   if(luigi_.x>=352&&(luigi_.y+luigi_HEIGHT)<=192&&luigi_.x<=480)
                   {
                   gravita_luigi=-16;
                   }               
                   break;
                   }
                   }
}

void Mario::move_mario()
{
    mario_.x += x_mario_Vel;
    offSet += mario_velocity;
    if(( mario_.x < 0 ) || ( mario_.x + mario_WIDTH > LEVEL_WIDTH ) || ( check_collision(  mario_, wall[0] ) )==true|| ( check_collision(  mario_, wall[1] ) )|| ( check_collision(  mario_, wall[2] ) )|| ( check_collision(  mario_, wall[3] ) )|| ( check_collision(  mario_, wall[4] ) )|| ( check_collision(  mario_, wall[5] ) )|| ( check_collision(  mario_, wall[6] ) )|| ( check_collision(  mario_, wall[7] ) )|| ( check_collision(  mario_, wall[8] ) )|| ( check_collision(  mario_, wall[9] ) )|| ( check_collision(  mario_, wall[10] ) )|| ( check_collision(  mario_, wall[11] ) )|| ( check_collision(  mario_, wall[12] ) )|| ( check_collision(  mario_, wall[13] ) )|| ( check_collision(  mario_, wall[14] ) )|| ( check_collision(  mario_, wall[15] ) )|| ( check_collision(  mario_, wall[16] ) )|| ( check_collision(  mario_, wall[17] ) )|| ( check_collision(  mario_, wall[18] ) )|| ( check_collision(  mario_, wall[19] ) )|| ( check_collision(  mario_, wall[20] ) )|| ( check_collision(  mario_, wall[21] ) )|| ( check_collision(  mario_, wall[22] ) ) )
    {
        mario_.x -= x_mario_Vel;
        offSet -= mario_velocity;
    }
    
    if((check_collision(mario_, blocchi_vincita[0]))==false)
    {                                  
                                       via=false;
    }
            
    
    mario_.y += y_mario_Vel;

    if(( mario_.y < 0 ) || ( mario_.y + mario_HEIGHT > LEVEL_HEIGHT )   || ( check_collision(  mario_, wall[0] ) )||( check_collision(  mario_, wall[1] ) )|| ( check_collision(  mario_, wall[2] ) )|| ( check_collision(  mario_, wall[3] ) )|| ( check_collision(  mario_, wall[4] ) )|| ( check_collision(  mario_, wall[5] ) )|| ( check_collision(  mario_, wall[6] ) )|| ( check_collision(  mario_, wall[7] ) )|| ( check_collision(  mario_, wall[8] ) )|| ( check_collision(  mario_, wall[9] ) )|| ( check_collision(  mario_, wall[10] ) )|| ( check_collision(  mario_, wall[11] ) )|| ( check_collision(  mario_, wall[12] ) )|| ( check_collision(  mario_, wall[13] ) )|| ( check_collision(  mario_, wall[14] ) )|| ( check_collision(  mario_, wall[15] ) )|| ( check_collision(  mario_, wall[16] ) )|| ( check_collision(  mario_, wall[17] ) )|| ( check_collision(  mario_, wall[18] ) )|| ( check_collision(  mario_, wall[19] ) )|| ( check_collision(  mario_, wall[20] ) )|| ( check_collision(  mario_, wall[21] ) )|| ( check_collision(  mario_, wall[22] ) ) )
    {
        mario_.y -= y_mario_Vel;
        gravita_mario=10;
    }
}

void Luigi::move_luigi()
{

    luigi_.x += xVel;
    offSet += velocity;
    if(( luigi_.x < 0 ) || ( luigi_.x + luigi_WIDTH > LEVEL_WIDTH ) || ( check_collision(  luigi_, wall[0] ) )==true|| ( check_collision(  luigi_, wall[1] ) )==true|| ( check_collision(  luigi_, wall[2] ) )|| ( check_collision(  luigi_, wall[3] ) )|| ( check_collision(  luigi_, wall[4] ) )|| ( check_collision(  luigi_, wall[5] ) )|| ( check_collision(  luigi_, wall[6] ) )|| ( check_collision(  luigi_, wall[7] ) )|| ( check_collision(  luigi_, wall[8] ) )|| ( check_collision( luigi_, wall[9] ) )|| ( check_collision(  luigi_, wall[10] ) )|| ( check_collision(  luigi_, wall[11] ) )|| ( check_collision(  luigi_, wall[12] ) )|| ( check_collision(  luigi_, wall[13] ) )|| ( check_collision(  luigi_, wall[14] ) )|| ( check_collision(  luigi_, wall[15] ) )|| ( check_collision(  luigi_, wall[16] ) )|| ( check_collision(  luigi_, wall[17] ) )|| ( check_collision(  luigi_, wall[18] ) )|| ( check_collision(  luigi_, wall[19] ) )|| ( check_collision(  luigi_, wall[20] ) )|| ( check_collision(  luigi_, wall[21] ) )|| ( check_collision(  luigi_, wall[22] ) ) )
    {
        luigi_.x -= xVel;
        offSet -= velocity;

    }

    luigi_.y += yVel;

    if(( luigi_.y < 0 ) || ( luigi_.y + luigi_HEIGHT > LEVEL_HEIGHT )  || ( check_collision(  luigi_, wall[0] ) )||( check_collision(  luigi_, wall[1] ) )|| ( check_collision(  luigi_, wall[2] ) )|| ( check_collision(  luigi_, wall[3] ) )|| ( check_collision(  luigi_, wall[4] ) )|| ( check_collision(  luigi_, wall[5] ) )|| ( check_collision(  luigi_, wall[6] ) )|| ( check_collision(  luigi_, wall[7] ) )|| ( check_collision(  luigi_, wall[8] ) )|| ( check_collision(  luigi_, wall[9] ) )|| ( check_collision(  luigi_, wall[10] ) )|| ( check_collision(  luigi_, wall[11] ) )|| ( check_collision(  luigi_, wall[12] ) )|| ( check_collision(  luigi_, wall[13] ) )|| ( check_collision(  luigi_, wall[14] ) )|| ( check_collision(  luigi_, wall[15] ) )|| ( check_collision(  luigi_, wall[16] ) )|| ( check_collision(  luigi_, wall[17] ) )|| ( check_collision(  luigi_, wall[18] ) )|| ( check_collision(  luigi_, wall[19] ) )|| ( check_collision(  luigi_, wall[20] ) )|| ( check_collision(  luigi_, wall[21] ) )|| ( check_collision(  luigi_, wall[22] ) ) )
    {
        luigi_.y -= yVel;
        gravita_luigi=10;
    }
}

void Mario::show_mario()
{
    mario_.y+=gravita_mario++;

    if ((mario_.y+mario_HEIGHT)>=SCREEN_HEIGHT-64)

    {

        gravita_mario=0;
        mario_.y=SCREEN_HEIGHT-64-mario_HEIGHT;

        }

                   if(mario_.x>=0&&(mario_.y+mario_HEIGHT)>=32&&mario_.x<=96-8&&(mario_.y+mario_HEIGHT)<=32+32)
                   {
                   gravita_mario=0;
                   mario_.y=32-mario_HEIGHT;
                   }
                   if(mario_.x>=128-16&&(mario_.y+mario_HEIGHT)>=32&&mario_.x<=160-16&&(mario_.y+mario_HEIGHT)<=32+32)
                   {
                   gravita_mario=0;
                   mario_.y=32-mario_HEIGHT;
                   } 
                   if(mario_.x>=256-16&&(mario_.y+mario_HEIGHT)>=32&&mario_.x<=352-8&&(mario_.y+mario_HEIGHT)<=32+32)
                   {
                   gravita_mario=0;
                   mario_.y=32-mario_HEIGHT;
                   } 
                   if(mario_.x>=384-16&&(mario_.y+mario_HEIGHT)>=32&&mario_.x<=480-8&&(mario_.y+mario_HEIGHT)<=32+32)
                   {
                   gravita_mario=0;
                   mario_.y=32-mario_HEIGHT;
                   }
                   
                   if(mario_.x>=160-16&&(mario_.y+mario_HEIGHT)>=64&&mario_.x<=192-8&&(mario_.y+mario_HEIGHT)<=64+32)
                   {
                   gravita_mario=0;
                   mario_.y=64-mario_HEIGHT;
                   }
                  
                   
                   if(mario_.x>=0&&(mario_.y+mario_HEIGHT)>=96&&mario_.x<=32-8&&(mario_.y+mario_HEIGHT)<=96+32)
                   {
                   gravita_mario=0;
                   mario_.y=96-mario_HEIGHT;
                   }
                   if(mario_.x>=192-16&&(mario_.y+mario_HEIGHT)>=96&&mario_.x<=224-8&&(mario_.y+mario_HEIGHT)<=96+32)
                   {
                   gravita_mario=0;
                   mario_.y=96-mario_HEIGHT;
                   }
                   
                   if(mario_.x>=32-16&&(mario_.y+mario_HEIGHT)>=128&&mario_.x<=64-8&&(mario_.y+mario_HEIGHT)<=128+32)
                   {
                   gravita_mario=0;
                   mario_.y=128-mario_HEIGHT;
                   }
                   if(mario_.x>=224-16&&(mario_.y+mario_HEIGHT)>=128&&mario_.x<=256-8&&(mario_.y+mario_HEIGHT)<=128+32)
                   {
                   gravita_mario=0;
                   mario_.y=128-mario_HEIGHT;
                   }    
 
                   if(mario_.x>=416-16&&(mario_.y+mario_HEIGHT)>=128&&mario_.x<=480-8&&(mario_.y+mario_HEIGHT)<=128+32)
                   {
                   gravita_mario=0;
                   mario_.y=128-mario_HEIGHT;
                   } 
                   
                   if(mario_.x>=32-16&&(mario_.y+mario_HEIGHT)>=160&&mario_.x<=128-8&&(mario_.y+mario_HEIGHT)<=160+32)
                   {
                   gravita_mario=0;
                   mario_.y=160-mario_HEIGHT;
                   } 
                   if(mario_.x>=160-16&&(mario_.y+mario_HEIGHT)>=160&&mario_.x<=192-8&&(mario_.y+mario_HEIGHT)<=160+32)
                   {
                   gravita_mario=0;
                   mario_.y=160-mario_HEIGHT;
                   }   
                   if(mario_.x>=352-16&&(mario_.y+mario_HEIGHT)>=160&&mario_.x<=384-8&&(mario_.y+mario_HEIGHT)<=160+32)
                   {
                   gravita_mario=0;
                   mario_.y=160-mario_HEIGHT;
                   }
                   
                   if(mario_.x>=192-16&&(mario_.y+mario_HEIGHT)>=192&&mario_.x<=224-8&&(mario_.y+mario_HEIGHT)<=192+32)
                   {
                   gravita_mario=0;
                   mario_.y=192-mario_HEIGHT;
                   }
                   if(mario_.x>=256-16&&(mario_.y+mario_HEIGHT)>=192&&mario_.x<=320-8&&(mario_.y+mario_HEIGHT)<=192+32)
                   {
                   gravita_mario=0;
                   mario_.y=192-mario_HEIGHT;
                   }
                   if(mario_.x>=352-16&&(mario_.y+mario_HEIGHT)>=192&&mario_.x<=480-8&&(mario_.y+mario_HEIGHT)<=192+32)
                   {
                   gravita_mario=0;
                   mario_.y=192-mario_HEIGHT;
                   }    

     if( mario_velocity < 0 )
    {
        status = Mario_LEFT;

        frame++;
    }
    else if( mario_velocity > 0 )
    {
         status = Mario_RIGHT;
        frame++;
    }
    else
    {
        frame = -1;
    }
    if( frame >= 3 )
    {
        frame = 0;
    }
    if( frame==0&&status==Mario_RIGHT )
    {
        apply_surface( mario_.x , mario_.y, mario_move0, screen );
    }
    if( frame==1&&status==Mario_RIGHT )
    {
        apply_surface( mario_.x , mario_.y, mario_move1, screen );
    }
    if( frame==2&&status==Mario_RIGHT )
    {
        apply_surface( mario_.x, mario_.y, mario_move2, screen );
    }
    if( frame==0&&status==Mario_LEFT )
    {
        apply_surface( mario_.x, mario_.y, mario_moveLeft0, screen );
    }
    if( frame==1&&status==Mario_LEFT )
    {
        apply_surface( mario_.x, mario_.y, mario_moveLeft1, screen );
    }
    if( frame==2&&status==Mario_LEFT )
    {
        apply_surface( mario_.x, mario_.y, mario_moveLeft2, screen );
    }
    if(frame==-1)
    apply_surface(  mario_.x, mario_.y, mario, screen );
}

void Luigi::show_luigi()
{
    luigi_.y+=gravita_luigi++;

    if ((luigi_.y+luigi_HEIGHT)>=SCREEN_HEIGHT-64)

    {

        gravita_luigi=0;
        luigi_.y=SCREEN_HEIGHT-64-luigi_HEIGHT;

        }
                   if(luigi_.x>=0&&(luigi_.y+luigi_HEIGHT)>=32&&luigi_.x<=96-8&&(luigi_.y+luigi_HEIGHT)<=32+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=32-luigi_HEIGHT;
                   }
                   if(luigi_.x>=128-16&&(luigi_.y+luigi_HEIGHT)>=32&&luigi_.x<=160-8&&(luigi_.y+luigi_HEIGHT)<=32+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=32-luigi_HEIGHT;
                   } 
                   if(luigi_.x>=256-16&&(luigi_.y+luigi_HEIGHT)>=32&&luigi_.x<=352-8&&(luigi_.y+luigi_HEIGHT)<=32+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=32-luigi_HEIGHT;
                   } 
                   if(luigi_.x>=384-16&&(luigi_.y+luigi_HEIGHT)>=32&&luigi_.x<=480-8&&(luigi_.y+luigi_HEIGHT)<=32+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=32-luigi_HEIGHT;
                   }
                   
                   if(luigi_.x>=160-16&&(luigi_.y+luigi_HEIGHT)>=64&&luigi_.x<=192-8&&(luigi_.y+luigi_HEIGHT)<=64+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=64-luigi_HEIGHT;
                   }
                
                   if(luigi_.x>=320-16&&(luigi_.y+luigi_HEIGHT)>=64&&luigi_.x<=352-8&&(luigi_.y+luigi_HEIGHT)<=64+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=64-luigi_HEIGHT;
                   }    
                   
                   if(luigi_.x>=0&&(luigi_.y+luigi_HEIGHT)>=96&&luigi_.x<=32-8&&(luigi_.y+luigi_HEIGHT)<=96+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=96-luigi_HEIGHT;
                   }
                   if(luigi_.x>=192-16&&(luigi_.y+luigi_HEIGHT)>=96&&luigi_.x<=224-8&&(luigi_.y+luigi_HEIGHT)<=96+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=96-luigi_HEIGHT;
                   }
                   
                   if(luigi_.x>=32-16&&(luigi_.y+luigi_HEIGHT)>=128&&luigi_.x<=64-8&&(luigi_.y+luigi_HEIGHT)<=128+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=128-luigi_HEIGHT;
                   }
                   if(luigi_.x>=224-16&&(luigi_.y+luigi_HEIGHT)>=128&&luigi_.x<=256-8&&(luigi_.y+luigi_HEIGHT)<=128+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=128-luigi_HEIGHT;
                   }    
 
                   if(luigi_.x>=416-16&&(luigi_.y+luigi_HEIGHT)>=128&&luigi_.x<=480-8&&(luigi_.y+luigi_HEIGHT)<=128+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=128-luigi_HEIGHT;
                   } 
                   
                   if(luigi_.x>=32-16&&(luigi_.y+luigi_HEIGHT)>=160&&luigi_.x<=128-8&&(luigi_.y+luigi_HEIGHT)<=160+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=160-luigi_HEIGHT;
                   } 
                   if(luigi_.x>=160-16&&(luigi_.y+luigi_HEIGHT)>=160&&luigi_.x<=192-8&&(luigi_.y+luigi_HEIGHT)<=160+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=160-luigi_HEIGHT;
                   }   
                   if(luigi_.x>=352-16&&(luigi_.y+luigi_HEIGHT)>=160&&luigi_.x<=384-8&&(luigi_.y+luigi_HEIGHT)<=160+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=160-luigi_HEIGHT;
                   }
                   
                   if(luigi_.x>=192-16&&(luigi_.y+luigi_HEIGHT)>=192&&luigi_.x<=224-16&&(luigi_.y+luigi_HEIGHT)<=192+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=192-luigi_HEIGHT;
                   }
                   if(luigi_.x>=256-16&&(luigi_.y+luigi_HEIGHT)>=192&&luigi_.x<=320-16&&(luigi_.y+luigi_HEIGHT)<=192+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=192-luigi_HEIGHT;
                   }
                   if(luigi_.x>=352-16&&(luigi_.y+luigi_HEIGHT)>=192&&luigi_.x<=480-8&&(luigi_.y+luigi_HEIGHT)<=192+32)
                   {
                   gravita_luigi=0;
                   luigi_.y=192-luigi_HEIGHT;
                   }    
     if( velocity < 0 )
    {
        status = Luigi_LEFT;

        frame++;
    }
    else if( velocity > 0 )
    {
         status = Luigi_RIGHT;
        frame++;
    }
    else
    {
        frame = -1;
    }
    if( frame >= 3 )
    {
        frame = 0;
    }
    if( frame==0&&status==Luigi_RIGHT )
    {
        apply_surface( luigi_.x , luigi_.y, luigi_move0, screen );
    }
    if( frame==1&&status==Luigi_RIGHT )
    {
        apply_surface( luigi_.x , luigi_.y, luigi_move1, screen );
    }
    if( frame==2&&status==Luigi_RIGHT )
    {
        apply_surface( luigi_.x, luigi_.y, luigi_move2, screen );
    }
    if( frame==0&&status==Luigi_LEFT )
    {
        apply_surface( luigi_.x, luigi_.y, luigi_moveLeft0, screen );
    }
    if( frame==1&&status==Luigi_LEFT )
    {
        apply_surface( luigi_.x, luigi_.y, luigi_moveLeft1, screen );
    }
    if( frame==2&&status==Luigi_LEFT )
    {
        apply_surface( luigi_.x, luigi_.y, luigi_moveLeft2, screen );
    }
    if(frame==-1)
    apply_surface(  luigi_.x, luigi_.y, luigi, screen );
}

Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    started = true;

    paused = false;

    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    started = false;

    paused = false;
}

void Timer::pause()
{
    if( ( started == true ) && ( paused == false ) )
    {
        paused = true;

        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    if( paused == true )
    {
        paused = false;

        startTicks = SDL_GetTicks() - pausedTicks;

        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    if( started == true )
    {
        if( paused == true )
        {
            return pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - startTicks;
        }
    }

    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

int main( int argc, char* args[] )
{
    
    srand(time(NULL));
    num=rand()%27+1;
    bool quit = false;

    Mario mariopiccolo;
    
    Luigi luigipiccolo;

    Timer fps;
    Timer myTimer;

    if( init() == false )
    {
        return 1;
    }

    if( load_files() == false )
    {
        return 1;
    }

    wall[0].x = 256;
    wall[0].y = 0;
    wall[0].w = 32;
    wall[0].h = 32;
    
    wall[1].x = 0;
    wall[1].y = 32;
    wall[1].w = 32*3;
    wall[1].h = 32;
    
    wall[2].x = 128;
    wall[2].y = 32;
    wall[2].w = 32;
    wall[2].h = 32;
    
    wall[3].x = 256;
    wall[3].y = 32;
    wall[3].w = 32*3;
    wall[3].h = 32;
    
    wall[4].x = 384;
    wall[4].y = 32;
    wall[4].w = 32*3;
    wall[4].h = 32;
    
    wall[5].x = 128;
    wall[5].y = 64;
    wall[5].w = 32*2;
    wall[5].h = 32;
    
    /*wall[6].x = 256;
    wall[6].y = 64;
    wall[6].w = 32;
    wall[6].h = 32;*/
    
    wall[7].x = 320;
    wall[7].y = 64;
    wall[7].w = 32;
    wall[7].h = 32;
    
    wall[8].x = 0;
    wall[8].y = 96;
    wall[8].w = 32;
    wall[8].h = 32;
    
    /*wall[9].x = 96;
    wall[9].y = 96;
    wall[9].w = 32;
    wall[9].h = 32;*/
    
    wall[10].x = 192;
    wall[10].y = 96;
    wall[10].w = 32;
    wall[10].h = 32;
    
    /*wall[11].x = 320;
    wall[11].y = 96;
    wall[11].w = 32*2;
    wall[11].h = 32;*/
    
    wall[12].x = 32;
    wall[12].y = 128;
    wall[12].w = 32;
    wall[12].h = 32;
    
    wall[13].x = 224;
    wall[13].y = 128;
    wall[13].w = 32;
    wall[13].h = 32;
    
    /*wall[14].x = 288;
    wall[14].y = 128;
    wall[14].w = 32;
    wall[14].h = 32;*/
    
    wall[15].x = 416;
    wall[15].y = 128;
    wall[15].w = 32*2;
    wall[15].h = 32;
    
    wall[16].x = 32;
    wall[16].y = 160;
    wall[16].w = 32*3;
    wall[16].h = 32;
    
    
    wall[17].x = 160;
    wall[17].y = 160;
    wall[17].w = 32;
    wall[17].h = 32;
    
    wall[18].x = 352;
    wall[18].y = 160;
    wall[18].w = 32;
    wall[18].h = 32;
    
    wall[19].x = 32;
    wall[19].y = 192;
    wall[19].w = 32;
    wall[19].h = 32;
    
    wall[20].x = 192;
    wall[20].y = 192;
    wall[20].w = 32;
    wall[20].h = 32;
    
    wall[21].x = 256;
    wall[21].y = 192;
    wall[21].w = 32*2;
    wall[21].h = 32;
    
    wall[22].x = 352;
    wall[22].y = 192;
    wall[22].w = 32*4;
    wall[22].h = 32;
    
    if(num==1/*&&via==true*/)
    {
     blocchi_vincita[0].x = 0;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 32;
    blocchi_vincita[0].h = 32;
    }
    if(num==2/*&&via==true*/)
    {
    blocchi_vincita[0].x = 32;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 32;
    blocchi_vincita[0].h = 32;
    }
    if(num==3/*&&via==true*/)
    {
    blocchi_vincita[0].x = 64;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 32;
    blocchi_vincita[0].h = 32;
    }
    if(num==4/*&&via==true*/)
    {
    blocchi_vincita[0].x = 256;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 32;
    blocchi_vincita[0].h = 32;
    }
    if(num==5/*&&via==true*/)
    {
    blocchi_vincita[0].x = 288;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 32;
    blocchi_vincita[0].h = 32;
    }
    if(num==6/*&&via==true*/)
    {
    blocchi_vincita[0].x = 384;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 32;
    blocchi_vincita[0].h = 32;
    }
    if(num==7/*&&via==true*/)
    {
    blocchi_vincita[0].x= 416;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 32;
    blocchi_vincita[0].h = 32;
    }
    if(num==8/*&&via==true*/)
    {
    blocchi_vincita[0].x = 448;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 32;
    blocchi_vincita[0].h = 32;
    }
    
    if(num==9/*&&via==true*/)
    {
    blocchi_vincita[0].x = 128;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 64;
    blocchi_vincita[0].h = 32;
    }
    if(num==10/*&&via==true*/)
    {
    blocchi_vincita[0].x = 160;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 64;
    blocchi_vincita[0].h = 32;
    }
    if(num==11/*&&via==true*/)
    {
    blocchi_vincita[0].x = 320;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 64;
    blocchi_vincita[0].h = 32;
    }
    
    if(num==12/*&&via==true*/)
    {
    blocchi_vincita[0].x = 192;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 96;
    blocchi_vincita[0].h = 32;
    }
    if(num==13/*&&via==true*/)
    {
    blocchi_vincita[0].x= 0;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 96;
    blocchi_vincita[0].h = 32;
    }
    
    if(num==14/*&&via==true*/)
    {
    blocchi_vincita[0].x= 224;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 128;
    blocchi_vincita[0].h = 32;
    }
    if(num==15/*&&via==true*/)
    {
    blocchi_vincita[0].x= 416;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 128;
    blocchi_vincita[0].h = 32;
    }
    if(num==16/*&&via==true*/)
    {
    blocchi_vincita[0].x= 448;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 128;
    blocchi_vincita[0].h = 32;
    }
    
    if(num==17/*&&via==true*/)
    {
    blocchi_vincita[0].x= 64;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 160;
    blocchi_vincita[0].h = 32;
    }
    if(num==18/*&&via==true*/)
    {
    blocchi_vincita[0].x= 96;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 160;
    blocchi_vincita[0].h = 32;
    }
    if(num==19/*&&via==true*/)
    {
    blocchi_vincita[0].x= 160;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 160;
    blocchi_vincita[0].h = 32;
    }
    
    if(num==20/*&&via==true*/)
    {
    blocchi_vincita[0].x= 32;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 192;
    blocchi_vincita[0].h = 32;
    }
    if(num==21/*&&via==true*/)
    {
    blocchi_vincita[0].x= 192;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 192;
    blocchi_vincita[0].h = 32;
    }
    if(num==22/*&&via==true*/)
    {
    blocchi_vincita[0].x= 256;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 192;
    blocchi_vincita[0].h = 32;
    }
    if(num==23/*&&via==true*/)
    {
    blocchi_vincita[0].x = 288;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 192;
    blocchi_vincita[0].h = 32;
    }
    if(num==24/*&&via==true*/)
    {
    blocchi_vincita[0].x = 352;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 192;
    blocchi_vincita[0].h = 32;
    }
    if(num==25/*&&via==true*/)
    {
    blocchi_vincita[0].x = 384;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 192;
    blocchi_vincita[0].h = 32;
    }
    if(num==26/*&&via==true*/)
    {
    blocchi_vincita[0].x = 416;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 192;
    blocchi_vincita[0].h = 32;
    }
    if(num==27/*&&via==true*/)
    {
    blocchi_vincita[0].x = 448;
    blocchi_vincita[0].w = 32;
    blocchi_vincita[0].y = 192;
    blocchi_vincita[0].h = 32;
    }
            myTimer.start();
    while( quit == false )
    {
        fps.start();
        
        while( SDL_PollEvent( &event ) )
        {
            mariopiccolo.handle_input_mario();
            luigipiccolo.handle_input_luigi();

            if( event.type == SDL_QUIT )
            {
                quit = true;
            }
        }
        mariopiccolo.move_mario();
        luigipiccolo.move_luigi();

        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 146, 144, 255 ) );
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==1)
        {
        myTimer.pause();
        via=true;
        apply_surface(0,32, blocco, screen);
        }
        else
        {
        apply_surface(0,32, block, screen);
        }
        apply_surface(256,0, block, screen);
        
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==2)
        {
        myTimer.pause();
        via=true;
        apply_surface(32,32, blocco, screen);
        }
        else
        {
        apply_surface(32,32, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==3)
        {
        myTimer.pause();
        via=true;
        apply_surface(64,32, blocco, screen);
        }
        else
        {
        apply_surface(64,32, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==4)
        {
        myTimer.pause();
        via=true;
        apply_surface(256,32, blocco, screen);
        }
        else
        {
        apply_surface(256,32, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==5)
        {
        myTimer.pause();
        via=true;
        apply_surface(288,32, blocco, screen);
        }
        else
        {
        apply_surface(288,32, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==6)
        {
        myTimer.pause();
        via=true;
        apply_surface(384,32, blocco, screen);
        }
        else
        {
        apply_surface(384,32, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==7)
        {
        myTimer.pause();
        via=true;
        apply_surface(416,32, blocco, screen);
        }
        else
        {
        apply_surface(416,32, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==8)
        {
        myTimer.pause();
        via=true;
        apply_surface(448,32, blocco, screen);
        }
        else
        {
        apply_surface(448,32, block, screen);
        }
        apply_surface(128,32, block, screen);
        apply_surface(320,32, block, screen);
        
        
        
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==9)
        {
        myTimer.pause();
        via=true;
        apply_surface(128,64, blocco, screen);
        }
        else
        {
        apply_surface(128,64, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==10)
        {
        myTimer.pause();
        via=true;
        apply_surface(160,64, blocco, screen);
        }
        else
        {
        apply_surface(160,64, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==11)
        {
        myTimer.pause();
        via=true;
        apply_surface(320,64, blocco, screen);
        }
        else
        {
        apply_surface(320,64, block, screen);
        }


        
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==12)
        {
        myTimer.pause();
        via=true;
        apply_surface(0,96, blocco, screen);
        }
        else
        {
        apply_surface(0,96, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==13)
        {
        myTimer.pause();
        via=true;
        apply_surface(192,96, blocco, screen);
        }
        else
        {
        apply_surface(192,96, block, screen);
        }
        
        
        
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==14)
        {
        myTimer.pause();
        via=true;
        apply_surface(224,128, blocco, screen);
        }
        else
        {
        apply_surface(224,128, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==15)
        {
        myTimer.pause();
        via=true;
        apply_surface(416,128, blocco, screen);
        }
        else
        {
        apply_surface(416,128, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==16)
        {
        myTimer.pause();
        via=true;
        apply_surface(448,128, blocco, screen);
        }
        else
        {
        apply_surface(448,128, block, screen);
        }
        apply_surface(32,128, block, screen);
        
       
        
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==17)
        {
        myTimer.pause();
        via=true;
        apply_surface(64,160, blocco, screen);
        }
        else
        {
        apply_surface(64,160, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==18)
        {
        myTimer.pause();
        via=true;
        apply_surface(96,160, blocco, screen);
        }
        else
        {
        apply_surface(96,160, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==19)
        {
        myTimer.pause();
        via=true;
        apply_surface(160,160, blocco, screen);
        }
        else
        {
        apply_surface(160,160, block, screen);
        }
        apply_surface(32,160, block, screen);
        apply_surface(352,160, block, screen);
        
        
        
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==20)
        {
        myTimer.pause();
        via=true;
        apply_surface(32,192, blocco, screen);
        }
        else
        {
        apply_surface(32,192, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==21)
        {
        myTimer.pause();
        via=true;
        apply_surface(192,192, blocco, screen);
        }
        else
        {
        apply_surface(192,192, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==22)
        {
        myTimer.pause();
        via=true;
        apply_surface(256,192, blocco, screen);
        }
        else
        {
        apply_surface(256,192, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==23)
        {
        myTimer.pause();
        via=true;
        apply_surface(288,192, blocco, screen);
        }
        else
        {
        apply_surface(288,192, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==24)
        {
        myTimer.pause();
        via=true;
        apply_surface(352,192, blocco, screen);
        }
        else
        {
        apply_surface(352,192, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==25)
        {
        myTimer.pause();
        via=true;
        apply_surface(384,192, blocco, screen);
        }
        else
        {
        apply_surface(384,192, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==26)
        {
        myTimer.pause();
        via=true;
        apply_surface(416,192, blocco, screen);
        }
        else
        {
        apply_surface(416,192, block, screen);
        }
        if(myTimer.get_ticks()<=31000&&myTimer.get_ticks()>=29000&&num==27)
        {
        myTimer.pause();
        via=true;
        apply_surface(448,192, blocco, screen);
        }
        else
        {
        apply_surface(448,192, block, screen);
        }
        
        for(int i=0;i<SCREEN_WIDTH;i+=32)
        {
                apply_surface(i,SCREEN_HEIGHT-32, pavimento, screen);
        }
        for(int i=0;i<SCREEN_WIDTH;i+=32)
        {
                apply_surface(i,SCREEN_HEIGHT-32-32, pavimento, screen);
        }
        luigipiccolo.show_luigi();
        mariopiccolo.show_mario();
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }
    clean_up();

    return 0;
}
