#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int last_frame_time = 0;

struct player {
    float x;
    float y;
    float xVel;
    float yVel;
    float width;
    float height;
} player;

int initialize_window(void){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }
    window = SDL_CreateWindow(
            NULL, 
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_BORDERLESS
        );
    if (!window){
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer){ 
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }

    return TRUE;
}

void process_input(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type){
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){ 
                case SDLK_ESCAPE:
                    game_is_running = FALSE;
                    break;
                case SDLK_LEFT:
                    player.xVel = -MOVE_SPEED;
                    break;
                case SDLK_RIGHT:
                    player.xVel = MOVE_SPEED;
                    break;
                case SDLK_UP:
                    player.yVel = -MOVE_SPEED;
                    break;
                case SDLK_DOWN:
                   player.yVel = MOVE_SPEED; 
                    break;
                default:
                    break;
            }
            break;

        case SDL_KEYUP:
            switch(event.key.keysym.sym){ 
                case SDLK_LEFT:
                    if (player.xVel < 0) player.xVel = 0;
                    break;
                case SDLK_RIGHT: 
                    if (player.xVel > 0) player.xVel = 0;
                    break;
                case SDLK_UP:
                    if (player.yVel < 0) player.yVel = 0; 
                    break;
                case SDLK_DOWN:
                    if (player.yVel > 0) player.yVel = 0;
                    break;
                default:
                    break;
            }
            break;

        default: 
            break;
    }     
}

void setup (){
    player.x = 20;
    player.y = 20;
    player.xVel = 0;
    player.yVel = 0;
    player.width = 15;
    player.height = 15;    
}

void update(){
    //Sleep until we reach target frame time
    //Below while loop locks execution until the requirement is satisfied
    //Requirement: Current ticks (GetTicks) = the time the last frame occurred at + time length of a single frame (1000ms / frames per second)
    //Basically: hold execution until the correct amount of time has passed, then continue
    //However, this approach uses 100% of the CPU while its locked in this loop. So, uh, don't do that. Just for knowledge. Use SDL_Delay() instead
    // while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    
    // The following lines are only necessary if you want to cap the frame rate at the FPS variable
    // Otherwise, the process will just go as a function of delta time and move the correct amount of pixels per second at an uncapped framerate
    // int time_to_wait = (FRAME_TARGET_TIME - (SDL_GetTicks()- last_frame_time) / 1000.0);
    //if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) SDL_Delay(time_to_wait);

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();
    
    player.x += player.xVel * delta_time;
    player.y += player.yVel * delta_time;    
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Draw a rectangle
    SDL_Rect player_rect = {
        (int)player.x, 
        (int)player.y, 
        (int)player.width, 
        (int)player.height 
    };
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    SDL_RenderPresent(renderer);     
}

void destroy_window(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    game_is_running =  initialize_window();
  
    setup();
    SDL_GetError();
    while (game_is_running){
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}
