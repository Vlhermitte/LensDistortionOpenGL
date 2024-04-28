
#ifndef LENSDISTORTIONOPENGL_GAMESTATE_H
#define LENSDISTORTIONOPENGL_GAMESTATE_H

// File for the game state

typedef struct gameState {
    bool preProcessingDistortion = true; // Set to TRUE to enable preprocessing distortion
    bool postProcessingDistortion = false; // Set to TRUE to enable postprocessing distortion
} GameState;

GameState gameState;

#endif //LENSDISTORTIONOPENGL_GAMESTATE_H
