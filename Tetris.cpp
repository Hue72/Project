#include<stdio.h>
#include<windows.h> //원하는 좌표에 출력 및 화면을 깨끗이 지움 
#include<conio.h> // 키 관련 
#include<time.h>
#include<stdlib.h>
 
 
#define LEFT 75 //키보드 값들 
#define RIGHT 77 
#define UP 72 
#define DOWN 80 
#define SPACE 32 
#define ESC 27 
 
#define false 0 //판단용 
#define true 1
 
#define ACTIVE_BLOCK -2 // 필드 배열에 저장될 블록의 상태들 
#define CANMOVE -1     // 블록이 이동할 수 있는 공간
#define EMPTY 0 // 블록이 이동할 수 있는 공간       
#define WALL 1 // 블록이 이동할 수 없는 공간
#define MOVED_BLOCK 2 // 이동이 완료된 블록
 
#define GAME_X 11 //게임판들 
#define GAME_Y 23 
#define GAME_X_POS 3 //위치 조절 
#define GAME_Y_POS 1 
 
#define STATUS_X_POS GAME_X_POS+GAME_X+1 //게임정보
 
int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장 
int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장
 
int blocks[7][4][4][4]={
{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
 {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
}; //블록모양 4*4공간에 블록을 표현 

//0000
//0100
//1110
//0000
 
int block_type; //블록 종류를 저장 
int block_spin; //블록 회전값 저장 
int block_type_next; //다음 블록값 저장 
 
int main1[GAME_Y][GAME_X]; //게임판의 정보를 저장하는 배열 모니터에 표시후에 main2로 복사됨 
int main2[GAME_Y][GAME_X]; //즉 main2는 게임판이 모니터에 표시되기 전의 정보를 가지고 있음 
                              //main의 전체를 계속 모니터에 표시하지 않고(이렇게 하면 모니터가 깜빡거림) 
                              //main2와 배열을 비교해서 값이 달라진 곳만 모니터에 고침 
int bx,by; //이동중인 블록의 게임판상의 x,y좌표를 저장 
 
int key; //키보드로 입력받은 키값을 저장 
 
int speed; //게임진행속도 
int level; //현재 level 
int level_goal; //다음레벨로 넘어가기 위한 목표점수 
int delete_line; //현재 레벨에서 제거한 줄 수를 저장 
int score; //현재 점수 
 
int new_block_on=0; //새로운 블럭이 필요함을 알리는 flag 
int contact=0; //현재 이동중인 블록이 충돌상태인지 알려주는 flag 
int level_up_on=0; //다음레벨로 진행(현재 레벨목표가 완료되었음을) 알리는 flag 
int space_bar_on=0; //hard drop상태임을 알려주는 flag 
 
void main_screen(void); //게임시작화면 
void reset(void); //게임판 초기화 
void reset_main(void); //메인 게임판(main1[][]를 초기화)
void reset_main2(void); //copy 게임판(main2[][]를 초기화)
void draw_map(void); //게임 전체 인터페이스를 표시 
void draw_main(void); //게임판을 그림 
void new_block(void); //새로운 블록을 하나 만듦 
void check_key(void); //키보드로 키를 입력받음 
void down_block(void); //블록을 아래로 떨어트림 
int check_contact(int bx, int by, int rotation); //bx, by위치에 rotation회전값을 같는 경우 충돌 판단 
void move_block(int dir); //dir방향으로 블록을 움직임 
void check_line(void); //줄이 가득찼는지를 판단하고 지움 
void check_level_up(void); //레벨목표가 달성되었는지를 판단하고 levelup시킴 
void check_game_over(void); //게임오버인지 판단하고 게임오버를 진행 
void pause(void);//게임을 일시정지시킴 
 
void gotoxy(int x,int y) { 
    COORD pos={2*x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형 
void setcursortype(CURSOR_TYPE c){ //커서숨기는 함수 
     CONSOLE_CURSOR_INFO CurInfo;
 
     switch (c) {
     case NOCURSOR:
          CurInfo.dwSize=1;
          CurInfo.bVisible=FALSE;
          break;
     case SOLIDCURSOR:
          CurInfo.dwSize=100;
          CurInfo.bVisible=TRUE;
          break;
     case NORMALCURSOR:
          CurInfo.dwSize=20;
          CurInfo.bVisible=TRUE;
          break;
     }
     SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}

enum {
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_SKYBLUE,
    DARK_RED,
    DARK_VOILET,
    DARK_YELLOW,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    SKYBLUE,
    RED,
    VIOLET,
    YELLOW,
    WHITE,
};

void setColor(unsigned short text) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}
 

 
int main(){
    int i;
    
    srand((unsigned)time(NULL)); //난수표생성 
    setcursortype(NOCURSOR); //커서 없앰
    main_screen(); //메인타이틀 호출 
    reset(); //게임판 리셋 
    
    while(1){
        for(i=0;i<5;i++){ //블록이 한칸떨어지는동안 5번 키입력받을 수 있음 
            check_key(); //키입력확인 
            draw_main(); //화면을 그림 
            Sleep(speed); //게임속도조절 
            if(contact&&check_contact(bx,by+1, block_spin)==false) Sleep(100);
                                                 //블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음 
            if(space_bar_on==1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
                space_bar_on=0;
                break;
            }
        }
        down_block(); // 블록을 한칸 내림 
        check_level_up(); // 레벨업을 체크 
        check_game_over(); //게임오버를 체크 
        if(new_block_on==1) new_block(); // 뉴 블럭 flag가 있는 경우 새로운 블럭 생성 
    }
}
 
void main_screen(void){
int x=5; //타이틀화면이 표시되는 x좌표 
int y=4; //타이틀화면이 표시되는 y좌표 
int count; //타이틀 프레임을 세는 변수  

    setColor(YELLOW);
    gotoxy(x,y+0);printf("    ▣▣▣▣▣  ▣▣▣▣▣  ▣▣▣▣▣  ▣▣▣▣    ▣▣▣▣▣  ▣▣▣▣▣            ");
    gotoxy(x,y+1);printf("        ▣      ▣              ▣      ▣      ▣      ▣      ▣                    ");
    gotoxy(x,y+2);printf("        ▣      ▣              ▣      ▣      ▣      ▣      ▣                    ");
    gotoxy(x,y+3);printf("        ▣      ▣▣▣▣▣      ▣      ▣▣▣▣        ▣      ▣▣▣▣▣            ");
    gotoxy(x,y+4);printf("        ▣      ▣              ▣      ▣      ▣      ▣              ▣            ");
    gotoxy(x,y+5);printf("        ▣      ▣▣▣▣▣      ▣      ▣      ▣  ▣▣▣▣▣  ▣▣▣▣▣            ");
    gotoxy(x,y+6);printf("                                                                                    ");
    setColor(GRAY);
    gotoxy(x,y+9);  printf("   +--------------+                                        ↑   : Shift             ");     
    gotoxy(x,y+10); printf("   -              -                                      ←  → : Left / Right      ");     
    gotoxy(x,y+11); printf("   -  21113683    -                                        ↓   : Soft Drop         ");
    gotoxy(x,y+12); printf("   -              -                                      SPACE : Hard Drop          "); 
    gotoxy(x,y+14); printf("   -      이강호  -                                       ESC  : Quit               ");  
    gotoxy(x,y+16); printf("   +--------------+                                    BONUS FOR HARD DROPS / COMBOS"); 
    Sleep(5000);
        
    while (kbhit()) getch(); 
 
}
 
void reset(void){
 
    
   
        
    level=1; //각종변수 초기화 
    score=0;
    level_goal=1000;
    key=0;
    contact=0;
    delete_line=0;
    speed=100;
    
    system("cls"); //화면지움 
    reset_main(); // main1를 초기화 
    draw_map(); // 게임화면을 그림
    draw_main(); // 게임판을 그림 
 
    block_type_next=rand()%7; //다음번에 나올 블록 종류를 랜덤하게 생성 
    new_block(); //새로운 블록을 하나 만듦  
    
}
 
void reset_main(void){ //필드 
    int i,j;
    
    for(i=0;i<GAME_Y;i++){ // 필드초기화  
        for(j=0;j<GAME_X;j++){
            main1[i][j]=0;
            main2[i][j]=100;
        }
    }    
    for(j=1;j<GAME_X;j++){ //천장
        main1[3][j]=CANMOVE;
    }
    for(i=1;i<GAME_Y-1;i++){ //좌우 생성 
        main1[i][0]=WALL;
        main1[i][GAME_X-1]=WALL;
    }
    for(j=0;j<GAME_X;j++){ //밑바닥 생성 
        main1[GAME_Y-1][j]=WALL;
    }
}
 
void reset_main2(void){ //main2를 초기화 
    int i, j;
    
    for(i=0;i<GAME_Y;i++){         //게임판에 게임에 사용되지 않는 숫자를 넣음 
        for(j=0;j<GAME_X;j++){  //이는 main1와 같은 숫자가 없게 하기 위함 
            main2[i][j]=100;
        }
    }    
}
 
void draw_map(void){ //게임 진행 상태
int y=3;             // level, goal, score는 게임중 상태가 바뀌기 때문에 y값 저장 
    setColor(YELLOW);                
    gotoxy(STATUS_X_POS, STATUS_Y_LEVEL=y); printf(" LEVEL : %5d", level); 
    gotoxy(STATUS_X_POS, STATUS_Y_GOAL=y+1); printf(" GOAL  : %5d", 10-delete_line);
    gotoxy(STATUS_X_POS, y+2); printf("+-NEXT  BLOCK-+ ");
    gotoxy(STATUS_X_POS, y+3); printf("|             | ");
    gotoxy(STATUS_X_POS, y+4); printf("|             | ");
    gotoxy(STATUS_X_POS, y+5); printf("|             | ");
    gotoxy(STATUS_X_POS, y+6); printf("|             | ");
    gotoxy(STATUS_X_POS, y+7); printf("+-- -  -  - --+ "); 
    gotoxy(STATUS_X_POS, y+8); printf(" YOUR SCORE :");     
    gotoxy(STATUS_X_POS, STATUS_Y_SCORE=y+9); printf("        %6d", score);   
    gotoxy(STATUS_X_POS, y+15); printf("  ↑   : Shift        SPACE : Hard Drop");     
    gotoxy(STATUS_X_POS, y+16); printf("←  → : Left / Right");     
    gotoxy(STATUS_X_POS, y+17); printf("  ↓   : Soft Drop     ESC  : Quit");
    gotoxy(STATUS_X_POS, y+20);printf("");
}
 
void draw_main(void){ //게임판 그리는 함수 
    int i, j;    
    
    for(j=1;j<GAME_X-1;j++){ //천장은 계속 새로운블럭이 지나가서 지워지면 새로 그려줌 
        if(main1[3][j]==EMPTY) main1[3][j]=CANMOVE;
    }
    for(i=0;i<GAME_Y;i++){
        for(j=0;j<GAME_X;j++){
            if(main2[i][j]!=main1[i][j]){ //2랑 비교해서 값이 달라진 부분만 새로 그려줌.
                                                //이게 없으면 게임판전체를 계속 그려서 느려지고 반짝거림
            gotoxy(GAME_X_POS+j,GAME_Y_POS+i); 
                switch(main1[i][j]){
                    case EMPTY: //빈칸모양 
                        printf("  ");
                        break;
                    case CANMOVE: //천장모양 
                        printf(". ");
                        break;
                    case WALL: //벽모양 
                    	setColor(VIOLET);
                        printf("▩"); 
                        break;
                    case MOVED_BLOCK: //굳은 블럭 모양  
                    	setColor(SKYBLUE);
                        printf("□");
                        break;
                    case ACTIVE_BLOCK: //움직이고있는 블럭 모양  
                    	setColor(DARK_YELLOW);
                        printf("■");
                        break;    
                }    
            }
        }
    }
    for(i=0;i<GAME_Y;i++){ //게임판을 그린 후 main2에 복사  
        for(j=0;j<GAME_X;j++){
            main2[i][j]=main1[i][j];
        }
    }
}
 
void new_block(void){ //테트리미노 만들기  
    int i, j;    
    
    bx=(GAME_X/2)-1;
    by=0;
    block_type=block_type_next; //다음 테트리미노 값 
    block_type_next=rand()%7; //다음 테트리미노 생성 
    block_spin=0;  
    
    new_block_on=0;   
    
    for(i=0;i<4;i++){ 
        for(j=0;j<4;j++){
            if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=ACTIVE_BLOCK;
        }
    }
    for(i=1;i<3;i++){ //다음 테트리미노 미리보기 
        for(j=0;j<4;j++){
            if(blocks[block_type_next][0][i][j]==1) {
                gotoxy(STATUS_X_POS+2+j,i+6);
                printf("■");
            }
            else{
                gotoxy(STATUS_X_POS+2+j,i+6);
                printf("  ");
            }
        }
    }
}
 
void check_key(void){
    key=0; //키값 초기화  
    
    if(kbhit()){ //키입력이 있는 경우  
        key=getch(); //키값을 받음
        if(key==224){ //방향키인경우 
            do{key=getch();} while(key==224);//방향키지시값을 버림 
            switch(key){
                case LEFT: //왼쪽키 눌렀을때  
                    if(check_contact(bx-1,by,block_spin)==true) move_block(LEFT); 
                    break;                            //왼쪽으로 갈 수 있는지 체크 후 가능하면 이동
                case RIGHT: //오른쪽 방향키 눌렀을때- 위와 동일하게 처리됨 
                    if(check_contact(bx+1,by,block_spin)==true) move_block(RIGHT);
                    break;
                case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨 
                    if(check_contact(bx,by+1,block_spin)==true) move_block(DOWN);
                    break;
                case UP: //위쪽 방향키 눌렀을때 
                    if(check_contact(bx,by,(block_spin+1)%4)==true) move_block(UP); 
                                                        //회전할 수 있는지 체크 후 가능하면 회전
                    else if(contact==1&&check_contact(bx,by-1,(block_spin+1)%4)==true) move_block(100);     
            }                    //바닥에 닿은 경우 위쪽으로 한칸띄워서 회전이 가능하면 그렇게 함(특수동작)
        }
        else{ //방향키가 아닌 입력 
            switch(key){
                case SPACE:
                    space_bar_on=1; 
                    while(contact==0){ //바닥까지 이동
                        down_block();
                        score+=level; // hard drop 보너스 점수
                        gotoxy(STATUS_X_POS, STATUS_Y_SCORE); printf("        %6d", score);  
                    }
                    break;
                case ESC:
                    system("cls");
                    exit(0); //게임종료 
            }
        }
    }
    while (kbhit()) getch(); //키버퍼를 비움 
}
 
void down_block(void){
    int i,j;
    
    if(contact&&check_contact(bx,by+1, block_spin)==true) contact=0; //밑이 비어있으면 crush flag 끔 
    if(contact&&check_contact(bx,by+1, block_spin)==false){ //밑이 비어있지않고 crush flag가 켜저있으면 
        for(i=0;i<GAME_Y;i++){ //현재 조작중인 블럭을 굳힘 
            for(j=0;j<GAME_X;j++){
                if(main1[i][j]==ACTIVE_BLOCK) main1[i][j]=MOVED_BLOCK;
            }
        }
        contact=0; //flag를 끔 
        check_line(); //라인체크를 함 
        new_block_on=1; //새로운 블럭생성 flag를 켬    
        return; //함수 종료 
    }
    if(check_contact(bx,by+1, block_spin)==true) move_block(DOWN); //밑이 비어있으면 밑으로 한칸 이동 
    if(check_contact(bx,by+1, block_spin)==false) contact++; //밑으로 이동이 안되면  crush flag를 켬
}
 
int check_contact(int bx, int by, int block_spin){ //지정된 좌표와 회전값으로 충돌이 있는지 검사 
    int i,j;
    
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){ //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴 
            if(blocks[block_type][block_spin][i][j]==1&&main1[by+i][bx+j]>0) return false;
        }
    }    
    return true; //하나도 안겹치면 true리턴 
};
 
void move_block(int dir){ //블록을 이동시킴 
    int i,j;
 
    switch(dir){
        case LEFT: //왼쪽
            for(i=0;i<4;i++){ //현재좌표의 테트리미노를 지움 
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){ //왼쪽으로 한칸간 뒤 테트리미노 생성 
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j-1]=ACTIVE_BLOCK;
                }
            }
            bx--; //좌표값 이동 
            break;
        
        case RIGHT:    //오른쪽 
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j+1]=ACTIVE_BLOCK;
                }
            }
            bx++;        
            break;
        
        case DOWN:    //아래쪽
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i+1][bx+j]=ACTIVE_BLOCK;
                }
            }
            by++;        
            break;
        
        case UP:
            for(i=0;i<4;i++){ //현재 테트리미노 지움  
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=EMPTY;
                }
            }
            block_spin=(block_spin+1)%4; //회전값을 증가, 계속 회전하면 원래 모양으로 다시 돌아옴 (%4)
            for(i=0;i<4;i++){ //회전한 테트리미노 출력 
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=ACTIVE_BLOCK;
                }
            }
            break;
            
        case 100: //테트리미노가 바닥,다른 테트리미노와 닿은 상태에서 한칸위로 올려 회전이 가능한 경우 
                
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=EMPTY;
                }
            }
            block_spin=(block_spin+1)%4;
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i-1][bx+j]=ACTIVE_BLOCK;
                }
            }
            by--;
            break;
    }
}
 
void check_line(void){
    int i, j, k, l;
    
    int    block_count; //한줄의 블록갯수를 저장하는 변수 
    int combo=0; //콤보갯수 저장하는 변수 지정및 초기화 
    
    for(i=GAME_Y-2;i>3;){ //i=MAIN_Y-2 : 밑쪽벽의 윗칸부터,  i>3 : 천장(3)아래까지 검사 
        block_count=0; //블록갯수 저장 변수 초기화 
        for(j=1;j<GAME_X-1;j++){ //벽과 벽사이의 블록갯루를 셈 
            if(main1[i][j]>0) block_count++;
        }
        if(block_count==GAME_X-2){ //블록이 가득 찬 경우 
            if(level_up_on==0){ //레벨업상태가 아닌 경우에(레벨업이 되면 자동 줄삭제가 있음) 
                score+=100*level; //점수추가 
                delete_line++; //지운 줄 갯수 카운트 증가 
                combo++; //콤보수 증가  
            }
            for(k=i;k>1;k--){ //윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만) 
                for(l=1;l<GAME_X-1;l++){
                    if(main1[k-1][l]!=CANMOVE) main1[k][l]=main1[k-1][l];
                    if(main1[k-1][l]==CANMOVE) main1[k][l]=EMPTY; 
//윗줄이 천장인 경우에는 천장을 한칸 내리면 안되니까 빈칸을 넣음 
                }
            }
        }
        else i--;
    }
    if(combo){ 
        if(combo>1){ //2줄 이상 지웠을 경우 
            gotoxy(GAME_X_POS+(GAME_X/2)-1,GAME_Y_POS+by-2);printf("%d COMBO!",combo);
            Sleep(500);
            score+=(combo*level*100);
            reset_main2(); //텍스트를 삭제하려면  main2을 초기화해야함 필드를 전부 다시 그리지 않기 위해 
        }
        gotoxy(STATUS_X_POS, STATUS_Y_GOAL); printf(" GOAL  : %5d", (delete_line<=10)?10-delete_line:0);
        gotoxy(STATUS_X_POS, STATUS_Y_SCORE); printf("        %6d", score);    
    }
}
 
void check_level_up(void){
    int i, j;
    
    if(delete_line>=10){ //10줄을 없앨 때마다 레벨 업!! 
        draw_main();
        level_up_on=1; 
        level+=1; 
        delete_line=0; //다시 10줄 지우기 시작 
        for(i=0;i<4;i++){
            gotoxy(GAME_X_POS+(GAME_X/2)-3,GAME_Y_POS+4); //레벨업 스피드업 위치 조절 
            printf("             ");
            gotoxy(GAME_X_POS+(GAME_X/2)-2,GAME_Y_POS+6);
            printf("             ");
            Sleep(200);
            
            gotoxy(GAME_X_POS+(GAME_X/2)-3,GAME_Y_POS+4);
            printf("LEVEL UP!");
            gotoxy(GAME_X_POS+(GAME_X/2)-2,GAME_Y_POS+6);
            printf("SPEED UP!");
            Sleep(200);
        }
        reset_main2(); //콤보와 같은 이유로 초기화 

        
        for(i=GAME_Y-2;i>GAME_Y-2-(level-1);i--){ //레벨업하면 아래 줄 지워줌 
           
            }
        }
         
        check_line(); //블록으로 채운 줄 지우기 
      
        switch(level){ //레벨업하면 속도도 증가 
            case 2:
                speed=50;
                break;
            case 3:
                speed=45;
                break;
            case 4:
                speed=40;
                break;
            case 5:
                speed=35;
                break;
            case 6:
                speed=30;
                break;
            case 7:
                speed=25;
                break;
            case 8:
                speed=20;
                break;
            case 9:
                speed=15;
                break;
            case 10:
                speed=10;
                break;
        }    
        level_up_on=0; //레벨업 flag꺼줌
        
        gotoxy(STATUS_X_POS, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //레벨표시 
        gotoxy(STATUS_X_POS, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10-delete_line); // 레벨목표 표시 
    
    }

 
void check_game_over(void){ 
    int i;
    
    int x=5;
    int y=5;
    
    for(i=1;i<GAME_X-2;i++){
        if(main1[3][i]>0){ //천장에 블럭 생기면 게임 오버 
        	setColor(YELLOW);
            gotoxy(x+15,y+0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
            gotoxy(x+15,y+1); printf("▤                              ▤");
            gotoxy(x+15,y+2); printf("▤                              ▤");
            gotoxy(x+15,y+3); printf("▤     G A M E     O V E R..    ▤");
            gotoxy(x+15,y+4); printf("▤                              ▤");
            gotoxy(x+15,y+5); printf("▤                              ▤");
            gotoxy(x+15,y+6); printf("▤                              ▤");
            gotoxy(x+15,y+7); printf("▤                              ▤");
            gotoxy(x+15,y+8); printf("▤                              ▤");
            gotoxy(x+15,y+9); printf("▤                              ▤");
            gotoxy(x+15,y+10); printf("▤                              ▤");
            gotoxy(x+15,y+11); printf("▤                              ▤");
            gotoxy(x+15,y+12); printf("▤        YOUR SCORE: %6d    ▤",score);
            gotoxy(x+15,y+13); printf("▤                              ▤");
            gotoxy(x+15,y+14); printf("▤                              ▤");
            gotoxy(x+15,y+15); printf("▤  Press any key to restart!   ▤");
            gotoxy(x+15,y+16); printf("▤                              ▤");
            gotoxy(x+15,y+17); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

                  
            Sleep(1000);
            while (kbhit()) getch();
            key=getch();
            reset();
        }
    }
}
 
