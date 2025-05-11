#include "block.h"

const int BLOCK[7][4][2] = {
    {{0,0},{0,-1},{0,1},{0,2}}, // 일자모양
    {{0,0},{0,-1},{-1,-1},{0,1}}, // ㄴ자 모양
    {{0,0},{0,-1},{0,1},{-1,1}}, // ㄴ자 모양 반대
    {{0,0},{-1,0},{-1,1},{0,1}}, // ㅁ자 모양
    {{0,0},{0,-1},{-1,0},{-1,1}}, // 번개모양
    {{0,0},{0,1},{-1,0},{-1,-1}}, // 번개모양 반대
    {{0,0},{-1,0},{0,1},{0,-1}}, // ㅗ자 모양
}; //회전 축은 무조건 idx 0

Block::Block(char ** board, int _max_size, int speed) : 
    board(board) , _max_size(_max_size) , speed(speed){
        
        on_board = false;

        next_queue = new char[_max_size];
        front = -1 , back = 0;

        srand(clock());

        for(int i = 0;i<_max_size;i++) inQueueBlock();
}

Block::~Block(){
    delete next_queue;
}


std::pair<int,int> Block::_rotate(std::pair<int,int> coord){
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
    // 생성 위치에 블록이 가득차서 생성하지 못하는 경우를 제외하곤 true 반환
    if(on_board) return true;
    // 축을 (2 , 5)에 위치하도록 생성
    int y = 2 , x = 5;

    curY = y , curX = x;
    
    for(int i = 0;i<4;i++)
        for(int j = 0;j<2;j++)
            cur_shape[i][j] = BLOCK[next_queue[front]][i][j];

    //충돌하지 않는다면 생성
    if(!isCollision(SHAPE::ORIGINAL)){
        cur_block = (char)popQueueBlock();
        //꺼낸만큼 다시 생성해 인큐
        inQueueBlock();
        for(int i = 0;i<4;i++){
            board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 1;
        }
    }
    else return false;

    on_board = true;

    makeShadow();

    return true;
}

void Block::rotateBlock(){
    if(!on_board || cur_block == 3) return; //현재 블록이 없거나 ㅁ자 모양일 경우 즉시 종료

    if(!isCollision(SHAPE::ROTATE)){

        
        int temp[4][2] = {0};
        deleteShadow(); //그림자 제거
        
        for(int i = 0;i<4;i++){
            std::pair<int,int> r = _rotate({cur_shape[i][0] , cur_shape[i][1]});
            temp[i][0] = r.first;
            temp[i][1] = r.second;
            board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 0; // 기존 블록은 지워준다
        }
        for(int i = 0;i<4;i++){
            cur_shape[i][0] = temp[i][0];
            cur_shape[i][1] = temp[i][1];
            board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 1;
        }
        makeShadow(); //그림자 생성
    }

}

bool Block::isCollision(SHAPE shape , int y , int x){ // ORIGINAL : 원본 , ROTATE : 90도 회전

    if(shape == ORIGINAL){
        for(int i = 0;i<4;i++){
           if( board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] > 2)
            return true;
        }
    }else if(shape == ROTATE){
        for(int i = 0;i<4;i++){
            std::pair<int,int> r = _rotate({cur_shape[i][0] , cur_shape[i][1]});
            if( board[curY + r.first][curX + r.second] > 2)
             return true;
         }
    }else if(shape == MOV){//움직이는 방향에 장애물이 있는지 확인
        for(int i = 0;i<4;i++){
            if( board[curY + cur_shape[i][0] + y][curX + cur_shape[i][1] + x] > 2)
             return true;
         }
    }

    return false;
}


//블록의 이동
const int d[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
bool Block::moveBlock(DIRECTION dir){
    if(!on_board) return false; // 블록이 없다면

    if(!isCollision(SHAPE::MOV , d[dir][0] , d[dir][1])){
        deleteShadow(); // 그림자 제거

        for(int i = 0;i<4;i++)
            board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 0;
        
        curY += d[dir][0]; curX += d[dir][1];
        makeShadow(); // 실제 블록이 그림자에 묻히지 않도록, 블록의 위치만 정해졌을 때 그림자를 그린다.
                        // 실제 블록이 그림자를 덧씌울 수 있도록

        for(int i = 0;i<4;i++)
            board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 1;
        
    }else {
        //바닥에 닿았을 때 블록을 새로 생성하기 위해 on_board를 끈다
        //컨트롤 불가능한 블록은 board에 3로 교체한다.
        if(dir == DOWN) {
            on_board = false;
            for(int i = 0;i<4;i++)
                board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 3;
        }
        return false;
    }

    return true;
}


void Block::spaceKeyPress(){
    if(!on_board) return;
    on_board = false;
    for(int i = 0 ; i<4;i++)
        board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 0;

    while(!isCollision(SHAPE::MOV , d[DIRECTION::DOWN][0] , d[DIRECTION::DOWN][1]))
        ++curY;
    
    for(int i = 0 ; i<4;i++)
        board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 3;
}


void Block::deleteShadow(){
    int tempY = curY;

    while(!isCollision(SHAPE::MOV , d[DIRECTION::DOWN][0] , d[DIRECTION::DOWN][1]))
      ++curY;
    
    for(int i = 0; i<4;i++){
        board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 0;
    }
    curY = tempY;
}

void Block::makeShadow(){
    int tempY = curY;

    while(!isCollision(SHAPE::MOV , d[DIRECTION::DOWN][0] , d[DIRECTION::DOWN][1]))
        ++curY;

    for(int i = 0; i<4;i++){
        if(board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] == 1)
            continue;
        board[curY + cur_shape[i][0]][curX + cur_shape[i][1]] = 2;
    }
    curY = tempY;
}

//10칸 모두 찬 라인을 지우고 지워진 라인 수를 리턴
int Block::clearLine(){
    //아래에서부터 
}