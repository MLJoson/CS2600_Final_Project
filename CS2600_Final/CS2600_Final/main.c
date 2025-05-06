/*
 * Marco Joson
 * Chelsea Kathleen Ocampo
 * Lucia Maturino Iniguez
 * Ethan Nhan
 * Branden Romero
 */

 /* clear.c ... * /

 /*
  * This example code creates an SDL window and renderer, and then clears the
  * window to a different color every frame, so you'll effectively get a window
  * that's smoothly fading between colors.
  *
  * This code is public domain. Feel free to use it for any purpose!
  */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

  /* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

/* Constants */
#define FRAMES_PER_SECOND 60
#define STEP_RATE_IN_MILLISECONDS (1.0/ FRAMES_PER_SECOND) * 1000
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 720

#define PLAYER_SPAWN_X SCREEN_WIDTH / 2
#define PLAYER_SPAWN_Y SCREEN_HEIGHT / 2
#define PLAYER_JUMP_HEIGHT 50.0
#define PLAYER_SIZE 50
#define GRAVITY 3

#define MAX_PLATFORMS 10
#define PLATFORM_HEIGHT 10
#define PLATFORM_WIDTH 80

/* Player */
typedef struct {
    float x;
    float y;
    float vel_y;
    float vel_x;

    // For key handling
    bool holdRight;
    bool holdLeft;
} Player;

/* Game State (in menu, game, or end screen) */
typedef enum {
    main_menu,
    game,
    retry_menu
} GameState;
/* App State (holding variables between frames) */
typedef struct {
    Player player;
    int last_step;
    GameState game_state;
} AppState;

/*Platforms*/
typedef struct {
    float x, y;
    float width, height;
} Platform;



/*Play Button*/
typedef struct {
    float x, y;
    float len, width;

    char text[8];
} Play_Button;

/*Quit Button*/
typedef struct {
    float x, y;
    float len, width;

    char text[8];
} Quit_Button;

/*Options Button*/

/* Initializes the player on startup or restart */
void initialize_player(Player* player) {
    player->x = PLAYER_SPAWN_X - (PLAYER_SIZE / 2);
    player->y = PLAYER_SPAWN_Y;
    player->vel_y = 0.0;
    player->vel_x = 0.0;
}

/* Initialize the platforms on startup or restart */
Platform platforms[MAX_PLATFORMS];
void initialize_platforms(Platform platforms[]) {
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        platforms[i].x = rand() % (SCREEN_WIDTH - 60);
        platforms[i].y = i * 70; //to space them out
        platforms[i].width = PLATFORM_WIDTH;
        platforms[i].height = PLATFORM_HEIGHT;
    }

    // Make sure there is a platform to catch the player
    platforms[MAX_PLATFORMS - 1].x = (SCREEN_WIDTH / 2) - (PLATFORM_WIDTH / 2);
}

Play_Button start_button;
Play_Button retry_button;
void initialize_play_button(Play_Button *button, float x, float y, float len, float width, char *text) {
    button->x = x;
    button->y = y;
    button->len = len;
    button->width = width;
    strcpy_s(button->text, sizeof(button->text), text);
}

Quit_Button quit_button;
void initialize_quit_button(Play_Button* button, float x, float y, float len, float width, char* text) {
    button->x = x;
    button->y = y;
    button->len = len;
    button->width = width;
    strcpy_s(button->text, sizeof(button->text), text);
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Initialize AppState
    AppState* as = (AppState*)SDL_calloc(1, sizeof(AppState));
    if (!as) {
        return SDL_APP_FAILURE;
    }

    *appstate = as;

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->game_state = main_menu;

    // Initialize player
    initialize_player(&as->player);

    // Initialize platform
    initialize_platforms(platforms);

    //Initialize buttons
    initialize_play_button(&start_button, 220, 450, 200, 80, "Play!");
    initialize_play_button(&retry_button, 220, 300, 200, 80, "Retry");

    initialize_quit_button(&quit_button, 220, 570, 200, 80, "Quit");
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* Key handling */
static SDL_AppResult handle_key_event_(Player* player, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            // Pressing Q quits the game
        case SDL_SCANCODE_Q:
            return SDL_APP_SUCCESS;

            // Movement keys for the player
        case SDL_SCANCODE_RIGHT:
            player->holdRight = true;
            break;
        case SDL_SCANCODE_LEFT:
            player->holdLeft = true;
            break;
        default:
            break;
        }
    }
    else if (event->type == SDL_EVENT_KEY_UP) {
        switch (event->key.scancode) {
            // Movement keys for the player
        case SDL_SCANCODE_RIGHT:
            player->holdRight = false;
            break;
        case SDL_SCANCODE_LEFT:
            player->holdLeft = false;
            break;
        default:
            break;
        }
    }

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    Player* player = &((AppState*)appstate)->player;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    case SDL_EVENT_KEY_UP:
    case SDL_EVENT_KEY_DOWN:
        return handle_key_event_(player, event);
    default:
        break;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* Updates player's x-velocity with input */
void move_player(Player* player) {
    // Max velocities
    int LEFT_MAX = -10;
    int RIGHT_MAX = 10;

    // If player is holding left and right
    if (player->holdRight && player->holdLeft)
        return;

    // If player is holding only right
    else if (player->holdRight) {
        if (player->vel_x <= RIGHT_MAX) {
            // Ramps up velocity to the right
            player->vel_x += 2;
        }
    }

    // If player is holding only left
    else if (player->holdLeft) {
        if (player->vel_x >= LEFT_MAX) {
            // Ramps up velocity to the left
            player->vel_x -= 2;
        }
    }
}

/* Handles resetting the game */
void reset_game(Player* player) {
    initialize_player(player);
    initialize_platforms(platforms);
}

/* Updates players position and velocity*/
void update_player(Player* player) {
    // Adds gravity to y-velocity
    player->vel_y += GRAVITY;

    // Apply movement input to x-velocity
    move_player(player);

    // Applies y-velocity to y-position
    player->y += player->vel_y * 0.1;

    // Applies x-velocity to x-position
    player->x += player->vel_x;

    // Wrap the player position around the borders
    int leftBorder = -40;
    int rightBorder = SCREEN_WIDTH + 20;
    if (player->x < leftBorder)
        player->x = rightBorder + (player->x - leftBorder);
    else if (player->x > rightBorder)
        player->x = leftBorder + (player->x - rightBorder);

    // Dampens x-velocity
    player->vel_x = player->vel_x * 0.9;

    /* platform interaction code
    Explaination: the player must bounce off the platfrom from ABOVE
    (so the player can enter from under.)*/
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        Platform p = platforms[i];
        if (player->vel_y > 0 &&
            player->x + PLAYER_SIZE > p.x &&
            player->x < p.x + p.width &&
            player->y + PLAYER_SIZE >= p.y &&
            player->y + PLAYER_SIZE <= p.y + p.height) {

            player->y = p.y - PLAYER_SIZE; // Fixes player clipping into platform
            player->vel_y = -PLAYER_JUMP_HEIGHT * GRAVITY;
        }
    }

    /* Use the player reaching the halfway point to move the platforms down
    to give the illusion of bouncing upwards*/
    if (player->y < SCREEN_HEIGHT / 2) {
        float move_down = (SCREEN_HEIGHT / 2) - player->y;
        player->y = (SCREEN_HEIGHT / 2);

        //move the platforms down
        for (int i = 0; i < MAX_PLATFORMS; i++) {
            platforms[i].y += move_down;
        }
    }

    //platform reset code
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        if (platforms[i].y > SCREEN_HEIGHT + 10)
        {
            platforms[i].y = -10;
            platforms[i].x = (float)(rand() % (SCREEN_WIDTH - (int)platforms[i].width));
        }
    }

    // Check for player death
    if (player->y > SCREEN_HEIGHT)
        reset_game(player);
    
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    // Get pointers to app state
    AppState* as = (AppState*)appstate;
    Player* player = &as->player;

    SDL_FRect r;

    // Update player position / velocity
    while ((SDL_GetTicks() - as->last_step) >= STEP_RATE_IN_MILLISECONDS) {
        update_player(player);
        as->last_step += STEP_RATE_IN_MILLISECONDS;
    }

    if (as->game_state == main_menu) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_FRect play_rect = { start_button.x, start_button.y, start_button.len, start_button.width };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &play_rect);

        SDL_FRect quit_rect = { quit_button.x, quit_button.y, quit_button.len, quit_button.width };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &quit_rect);

        SDL_RenderPresent(renderer);
        return SDL_APP_CONTINUE;
    }

    // Background color stuff
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    const float red = (float)(0.5 + 0.5 * SDL_sin(now));
    const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);



    //Draw the platforms
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white

    for (int i = 0; i < MAX_PLATFORMS; ++i) {
        SDL_FRect plat_rect = {
            platforms[i].x, platforms[i].y,
            platforms[i].width, platforms[i].height
        };
        SDL_RenderFillRect(renderer, &plat_rect);
    }

    // Draw the player
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // black
    r.x = player->x;
    r.y = player->y;
    r.w = r.h = PLAYER_SIZE;

    SDL_RenderFillRect(renderer, &r);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */

    // Free appstate
    if (appstate != NULL) {
        AppState* as = (AppState*)appstate;
        SDL_free(as);
    }
}



