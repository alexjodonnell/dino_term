#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../util/termlib.h"
#include "../util/logger.h"

#include "Dino.h"

int ground;

Dino * newDino(){
    Dino * dino = calloc(1, sizeof(Dino));
    return dino;
};

// todo handle filenotfound error (need to add some sort of error handling mechanism)
// it would be cool to load the dino from a file once than store it in a resource header
Dino * newDinoFromFile(const char * fname, int ground){
    Dino * dino = calloc(1, sizeof(Dino));

    FILE * fd = fopen(fname, "r");

    if(!fd){
        logbroke("FILE NOT FOUND\n");
        exit(EXIT_FAILURE);
    }

    logger("opened: %s\n", fname);

    // parse the dimensions of the dino

    // XXX: it would be nicer to calculate it. It would make it way easier to make resources.
    // I guess ideally we wouldn't have to load in from files anyway
    // we'd have all of the resources in a header or somedino.
    fscanf(fd, "%d %d", &dino->size.x, &dino->size.y);
    
    // get rid of the newline after the size
    getc(fd);

    // initialize the lines

    dino->lines = calloc(dino->size.x, sizeof(char *));

    for(int i = 0; i < dino->size.y; i++){
        dino->lines[i] = calloc(dino->size.x, sizeof(char));

        fgets(dino->lines[i], termSize.x, fd);

        // remove trailing newline from fgets
        strtok(dino->lines[i], "\n");
    }

    // init dummy physics
    dino->physics.s.x = 20;
    dino->physics.s.y = ground + dino->size.y - 1;
    dino->physics.ds.x = 0;
    dino->physics.ds.y = 0;    
    dino->physics.d2s.x = 0;
    dino->physics.d2s.y = 0;

    fclose(fd);
    return dino;
}

void destroyDino(Dino * dino){
    if(dino){
        if(dino->lines){
            for(int i=0; i<dino->size.y; i++){
                if(dino->lines[i]){
                    free(dino->lines[i]);
                }else{
                    break;
                }
            }
        }
        free(dino->lines);
    }
    free(dino);
}

void drawDinoHere(const Dino * dino, const Vec2i * here){
    // in our dwg library we can have max_y and max_x as protected global variables or somedino
    Vec2i adjust = vec2i((int)dino->physics.s.x, (int)dino->physics.s.y); 
    for(int i = 0; i < dino->size.y; i++){
        drawLine(dino->lines[i], &adjust);
        adjust.y--;
    }
}

void drawDino(const Dino * dino){
    // in our dwg library we can have max_y and max_x as protected global variables or somedino
    Vec2i adjust = vec2i((int)dino->physics.s.x, (int)dino->physics.s.y); 
    for(int i = 0; i < dino->size.y; i++){
        drawLine(dino->lines[i], &adjust);
        adjust.y--;
    }
}

void tickDino(Dino * dino){

    // stop if you hit the ground
    if(dino->physics.s.y == ground + dino->size.y){
        dino->physics.ds.y = 0;
        dino->canJump = 1;
    }
    tickPhysicsf(&dino->physics);
}

void set_ground(int grnd){
    ground = grnd;
}