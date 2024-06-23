
#ifndef LENSDISTORTIONOPENGL_GAMESTATE_H
#define LENSDISTORTIONOPENGL_GAMESTATE_H

// File for the game state

typedef struct gameState {
    bool preProcessingDistortion = true; // TRUE to enable preprocessing distortion FALSE to enable postprocessing distortion
    bool datasetGenProcedure = false;
} GameState;

GameState gameState;

#endif //LENSDISTORTIONOPENGL_GAMESTATE_H
