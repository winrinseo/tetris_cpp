#include <iostream>
#include <cstdlib>
#include <queue>
/*
 블록에 대한 연산을 제공
 여긴 블록에 대한 연산만 제공하고 키 입력은 외부에서 판별
 블록은 회전 축으로부터 상대좌표 형태로 저장
*/

// 게임판 색깔 지정
typedef enum __BOARD {
    BLACK,
    DARKBLUE,
    DARKGREEN,
    DARKCYAN,
    DARKRED,
    DARKMAGENTA,
    DARKYELLOW,
    GRAY,
    DARKGRAY,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    YELLOW,
    WHITE,
    WALL = 99 , EMPTY = 100, CUR_BLOCK , SHADOW , PREV_BLOCK , } _BOARD;

extern const int BLOCK[7][4][2]; //회전 축은 무조건 idx 0
extern const _BOARD BLOCK_COLOR[7]; //회전 축은 무조건 idx 0

enum SHAPE {ORIGINAL , ROTATE , MOV};
enum DIRECTION {UP , DOWN , LEFT , RIGHT};
const int FULLED_LINE = (1 << 11) - 2; //라인이 가득 찼을 때의 비트마스킹



class Block{
private:
    
    char ** board; //게임판
    int BOARD_HEIGHT;

    char * next_queue; //다음 블록을 저장하는 큐
    int front , back , _max_size;

    int cur_block; //현재 생성된 블록
    int cur_shape[4][2]; //현재 생성된 블록의 회전

    int speed; // 블록이 떨어지는 속도
    bool _key_pressed; // 눌렸는지
    bool space_key_pressed; // 스페이스가 눌렸는지
    int curY , curX;

    bool on_board; // 블록이 지금 게임판 위인지 판별

    int * full_line_check; // 블록이 한줄을 만들었는지 확인할 비트마스킹 배열

    //현재 블록의 좌표를 구함
    std::pair<int,int> _rotate(std::pair<int,int> coord);


    
public:
    Block(char ** board, int _max_size);
    ~Block();

    void inQueueBlock(); // 랜덤한 블록을 큐에 넣는 함수
    char popQueueBlock(); // 큐에 있는 블록을 꺼내는 함수
    char peekQueueBlock();

    int getCurrentBlock(); // 현재 블록 번호 반환

    bool onBoardBlock(); // 큐에서 블록을 하나 꺼내서 게임판에 올려놓는 함수. 성공적으로 생성 시 true 반환

    void rotateBlock(); // 블록의 축을 기준으로 90도 회전하는 함수

    bool moveBlock(DIRECTION dir);// 블록 이동

    void spaceKeyPress(); // 스페이스바 눌렀을 때 

    bool isCollision(SHAPE shape , int y = 0, int x = 0); // 현재 생성된 블록의 충돌 여부를 판별하는 함수

    void deleteShadow(); // 기존에 생성되어 있던 그림자 제거
    void makeShadow(); // 현재 생성된 블록의 예상 착지 지점

    int clearLine(); // 지울 수 있는 라인이 존재할 경우 라인을 지운다.

    void recodeLine(int y , int x); // 떨어진 블록을 비트마스크로 기록
    
};