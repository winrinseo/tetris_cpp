#include "block.h"


Block::Block(char ** board, int _max_size, int speed) : 
    board(board) , _max_size(_max_size) , speed(speed){

        next_queue = new char[_max_size];
        front = -1 , back = 0;

        srand(clock());

}

Block::~Block(){
    delete next_queue;
}


std::pair<int,int> rotate(std::pair<int,int> coord){
    return {-coord.second , coord.first};
}

void Block::inQueueBlock(){
    char gen = rand() % 7;

    next_queue[back] = gen;
    front = ((front + 1) % _max_size);
    back = ((back + 1) % _max_size);
}

char Block::popQueueBlock(){
    return next_queue[front++];
}

bool Block::onBoardBlock(){
    // 축을 (1 , 5)에 위치하도록 생성
    
    int y = 1 , x = 5;

    curY = y , curX = x;

    char b = popQueueBlock();
    //꺼낸만큼 다시 생성해 인큐
    inQueueBlock();

    //충돌하지 않는다면 생성
    if(!isCollision(0))
        for(int i = 0;i<4;i++){
            board[curY + block[b][i][0]][curX + block[b][i][1]] = 1;
        }

    on_board = true;
    cur_block = b;
    cur_shape = 0;
}

void Block::rotateBlock(){
    if(isCollision(cur_shape + 1)){
        for(int i = 1;i<4;i++){
            std::pair<int,int> s , t;
            t = {block[cur_block][i][0] ,block[cur_block][i][1] };
            for(int j = 0;j<cur_shape;j++)
                s = rotate(s);
            
            board[curY - t.first][curX - t.second] = 0;
            board[curY - s.first][curX - s.second] = 1;
        }
    }
}

bool Block::isCollision(int shape){

    for(int i = 1;i<4;i++){
        std::pair<int,int> s;
        s = {block[cur_block][i][0] ,block[cur_block][i][1] };
        for(int j = 0;j<shape;j++)
            s = rotate(s);
        if(board[curY + s.first][curX + s.second] >= 2)
            return false;
    }
    return true;
}
