#include "gameInstance.h"


GameInstance::GameInstance(){
    this->is_run = false;
}

GameInstance::~GameInstance(){}


void GameInstance::run(){
    
    while(this->is_run){
        this->gameLoop();
    }
}


void GameInstance::start(){
    this->is_run = true;
    this->run();
}

void GameInstance::stop(){
    this->is_run = false;
}

