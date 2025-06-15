#pragma once


class GameInstance{
private:
    //게임 루프를 무한히 반복 (stop 호출 전까지)
    bool is_run;
    void run();

public:

    GameInstance();
    ~GameInstance();

    //게임 시작부터 반복할 로직
    virtual void gameLoop() = 0;
    //루프 시작
    void start();
    //루프 중지
    void stop();

};