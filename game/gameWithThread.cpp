#include "gameWithThread.h"


GameWithThread::GameWithThread(Block ** tetris , char *** board, int _max_size , int player , int * status) : 
    tetris(tetris),player(player) , status(status){

        //스레드 상태
        _thread = false;
        pressed_key = new std::queue<DIRECTION>[player];

        //스레드 블로킹을 위한 뮤텍스와 상태 변수
        dummy = new std::mutex[player];
        cv = new std::condition_variable[player];
        
        //각 플레이어에게 게임판 할당
        for(int i = 0;i < player; i++){
            tetris[i] = new Block(board[i] , _max_size);
        }
        
    
}

GameWithThread::~GameWithThread(){
    //각 메모리 해제
    for(int i = 0;i < player; i++){
        delete tetris[i];
    }

    delete[] pressed_key;
}

void GameWithThread::pushKey(DIRECTION dir , int p){
    pressed_key[p].push(dir);
    //상태 변수에 신호를 줌
    cv[p].notify_one();
}


void GameWithThread::moveAll(){
    for(int i = 0;i<player;i++){ //모든 게임판의 블록을 아래로 내린다.
        tetris[i]->moveBlock(DIRECTION::DOWN);
    }
}



void GameWithThread::tick(){
    for(int i = 0;i<player;i++){ //틱마다 수행할 작업
        bool is_gen = this->tetris[i]->onBoardBlock();
        int s = this->tetris[i]->clearLine();

        if(s == -1 || !is_gen) { //반환된 점수가 -1이거나 블록이 생성되지 않았다면 게임 오버
            *status = i; //게임에서 패배한 플레이어
            break;
        }
    }
}


void GameWithThread::worker(int p){
    
    

    while(_thread){
        std::unique_lock<std::mutex> lock(dummy[p]);

        //큐가 비어있는 동안 블로킹
        cv[p].wait(lock , [this , p]{ return !pressed_key[p].empty();}); // 큐가 차있지 않으면 signal 이 들어와도 무시

        //큐가 빌때까지 각 명령을 수행
        while(!pressed_key[p].empty()){
            DIRECTION next = pressed_key[p].front();
            pressed_key[p].pop();
            
            if(next == UP){
                tetris[p]->rotateBlock();
            }else if(next == SPACE){
                tetris[p]->spaceKeyPress();
            }else{
                tetris[p]->moveBlock(next);
            }
        }
        // usleep(100000);
    }
}


void GameWithThread::run(){
    _thread = true;
    threads.clear();
    for(int i = 0;i<player;i++){
        threads.emplace_back(&GameWithThread::worker,this,i); //각 스레드 실행
    }

    for(int i = 0;i<player;i++){
        threads[i].detach(); //메인스레드 대기없이 스레드 실행
    }
}

void GameWithThread::stop(){
    _thread = false;
}