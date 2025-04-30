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
#define PLAYER_SPAWN_X 100
#define PLAYER_SPAWN_Y 100

/* Player */
typedef struct {
    int x;
    int y;
    int vel_y;
} Player;

/* App State (holding variables between frames) */
typedef struct {
    Player player;
} AppState;

/* Initializes the player on startup or restart */
void initialize_player(Player* player) {
    player->x = PLAYER_SPAWN_X;
    player->y = PLAYER_SPAWN_Y;
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

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Initialize player
    initialize_player(&as->player);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void move_player(Player* player, SDL_Scancode key_code) {
    // TODO: Implement moving
}

static SDL_AppResult handle_key_event_(Player* player, SDL_Scancode key_code) {
    switch (key_code) {
        // Pressing Q quits the game
    case SDL_SCANCODE_Q:
        return SDL_APP_SUCCESS;

        // Movement keys for the player
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_LEFT:
        move_player(player, key_code);
        break;
    default:
        break;
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
    case SDL_EVENT_KEY_DOWN:
        return handle_key_event_(player, event->key.scancode);
    default:
        break;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    // Get pointers to app state
    AppState* as = (AppState*)appstate;
    Player* player = &as->player;

    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    const float red = (float)(0.5 + 0.5 * SDL_sin(now));
    const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}



