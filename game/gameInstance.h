


class GameInstance{
private:
    bool is_run;
    void run();

public:

    GameInstance();
    ~GameInstance();

    virtual void gameLoop() = 0;
    void start();
    void stop();

};