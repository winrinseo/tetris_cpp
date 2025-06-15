/*
    다른 사람과 함께 플레이하는 것에 특화된 객체.
    스레드를 활용해 병렬적으로 각 게임을 수행.
    키보드 입력은 각 스레드가 동시에 받지 못하기 때문에 외부에서 키 입력을 받아 이 객체로 통보. (인큐)
    큐가 비어있는 동안 해당 스레드를 블록해 cpu 부담 완화.
*/

#pragma once
#include "block.h"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

class GameWithThread {
private:
    int * status;
    int player;

    //게임판
    Block ** tetris;

    //키 입력
    std::queue<DIRECTION> * pressed_key;


    //스레드 실행 상태
    bool _thread;

    //상태변수와 뮤텍스(뮤텍스는 락을 위한 것은 아니고 상태변수를 사용하기 위함)
    std::mutex * dummy;
    std::condition_variable * cv;

    //실행중인 스레드
    std::vector<std::thread> threads;

    //실행할 스레드 함수
    void worker(int p);

public:
    GameWithThread(Block ** tetris , char *** board, int _max_size , int player , int * status); // player : 플레이어 수
    ~GameWithThread();

    void pushKey(DIRECTION key , int p);
    
    //스레드 실행
    void run();

    //스레드 종료
    void stop();

    //존재하는 블록을 모두 내림
    void moveAll();

    //틱마다 수행할 작업
    void tick();
};