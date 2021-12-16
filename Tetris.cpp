#include<stdio.h>
#include<windows.h> //���ϴ� ��ǥ�� ��� �� ȭ���� ������ ���� 
#include<conio.h> // Ű ���� 
#include<time.h>
#include<stdlib.h>
 
 
#define LEFT 75 //Ű���� ���� 
#define RIGHT 77 
#define UP 72 
#define DOWN 80 
#define SPACE 32 
#define ESC 27 
 
#define false 0 //�Ǵܿ� 
#define true 1
 
#define ACTIVE_BLOCK -2 // �ʵ� �迭�� ����� ����� ���µ� 
#define CANMOVE -1     // ����� �̵��� �� �ִ� ����
#define EMPTY 0 // ����� �̵��� �� �ִ� ����       
#define WALL 1 // ����� �̵��� �� ���� ����
#define MOVED_BLOCK 2 // �̵��� �Ϸ�� ���
 
#define GAME_X 11 //�����ǵ� 
#define GAME_Y 23 
#define GAME_X_POS 3 //��ġ ���� 
#define GAME_Y_POS 1 
 
#define STATUS_X_POS GAME_X_POS+GAME_X+1 //��������
 
int STATUS_Y_GOAL; //GOAL ����ǥ����ġY ��ǥ ���� 
int STATUS_Y_LEVEL; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE; //SCORE ����ǥ����ġY ��ǥ ����
 
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
}; //��ϸ�� 4*4������ ����� ǥ�� 

//0000
//0100
//1110
//0000
 
int block_type; //��� ������ ���� 
int block_spin; //��� ȸ���� ���� 
int block_type_next; //���� ��ϰ� ���� 
 
int main1[GAME_Y][GAME_X]; //�������� ������ �����ϴ� �迭 ����Ϳ� ǥ���Ŀ� main2�� ����� 
int main2[GAME_Y][GAME_X]; //�� main2�� �������� ����Ϳ� ǥ�õǱ� ���� ������ ������ ���� 
                              //main�� ��ü�� ��� ����Ϳ� ǥ������ �ʰ�(�̷��� �ϸ� ����Ͱ� �����Ÿ�) 
                              //main2�� �迭�� ���ؼ� ���� �޶��� ���� ����Ϳ� ��ħ 
int bx,by; //�̵����� ����� �����ǻ��� x,y��ǥ�� ���� 
 
int key; //Ű����� �Է¹��� Ű���� ���� 
 
int speed; //��������ӵ� 
int level; //���� level 
int level_goal; //���������� �Ѿ�� ���� ��ǥ���� 
int delete_line; //���� �������� ������ �� ���� ���� 
int score; //���� ���� 
 
int new_block_on=0; //���ο� ���� �ʿ����� �˸��� flag 
int contact=0; //���� �̵����� ����� �浹�������� �˷��ִ� flag 
int level_up_on=0; //���������� ����(���� ������ǥ�� �Ϸ�Ǿ�����) �˸��� flag 
int space_bar_on=0; //hard drop�������� �˷��ִ� flag 
 
void main_screen(void); //���ӽ���ȭ�� 
void reset(void); //������ �ʱ�ȭ 
void reset_main(void); //���� ������(main1[][]�� �ʱ�ȭ)
void reset_main2(void); //copy ������(main2[][]�� �ʱ�ȭ)
void draw_map(void); //���� ��ü �������̽��� ǥ�� 
void draw_main(void); //�������� �׸� 
void new_block(void); //���ο� ����� �ϳ� ���� 
void check_key(void); //Ű����� Ű�� �Է¹��� 
void down_block(void); //����� �Ʒ��� ����Ʈ�� 
int check_contact(int bx, int by, int rotation); //bx, by��ġ�� rotationȸ������ ���� ��� �浹 �Ǵ� 
void move_block(int dir); //dir�������� ����� ������ 
void check_line(void); //���� ����á������ �Ǵ��ϰ� ���� 
void check_level_up(void); //������ǥ�� �޼��Ǿ������� �Ǵ��ϰ� levelup��Ŵ 
void check_game_over(void); //���ӿ������� �Ǵ��ϰ� ���ӿ����� ���� 
void pause(void);//������ �Ͻ�������Ŵ 
 
void gotoxy(int x,int y) { 
    COORD pos={2*x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ������� �Լ��� ���Ǵ� ������ 
void setcursortype(CURSOR_TYPE c){ //Ŀ������� �Լ� 
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
    
    srand((unsigned)time(NULL)); //����ǥ���� 
    setcursortype(NOCURSOR); //Ŀ�� ����
    main_screen(); //����Ÿ��Ʋ ȣ�� 
    reset(); //������ ���� 
    
    while(1){
        for(i=0;i<5;i++){ //����� ��ĭ�������µ��� 5�� Ű�Է¹��� �� ���� 
            check_key(); //Ű�Է�Ȯ�� 
            draw_main(); //ȭ���� �׸� 
            Sleep(speed); //���Ӽӵ����� 
            if(contact&&check_contact(bx,by+1, block_spin)==false) Sleep(100);
                                                 //����� �浹���ΰ�� �߰��� �̵��� ȸ���� �ð��� ���� 
            if(space_bar_on==1) { //�����̽��ٸ� �������(hard drop) �߰��� �̵��� ȸ���Ҽ� ���� break; 
                space_bar_on=0;
                break;
            }
        }
        down_block(); // ����� ��ĭ ���� 
        check_level_up(); // �������� üũ 
        check_game_over(); //���ӿ����� üũ 
        if(new_block_on==1) new_block(); // �� �� flag�� �ִ� ��� ���ο� �� ���� 
    }
}
 
void main_screen(void){
int x=5; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ 
int y=4; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ 
int count; //Ÿ��Ʋ �������� ���� ����  

    setColor(YELLOW);
    gotoxy(x,y+0);printf("    �ââââ�  �ââââ�  �ââââ�  �âââ�    �ââââ�  �ââââ�            ");
    gotoxy(x,y+1);printf("        ��      ��              ��      ��      ��      ��      ��                    ");
    gotoxy(x,y+2);printf("        ��      ��              ��      ��      ��      ��      ��                    ");
    gotoxy(x,y+3);printf("        ��      �ââââ�      ��      �âââ�        ��      �ââââ�            ");
    gotoxy(x,y+4);printf("        ��      ��              ��      ��      ��      ��              ��            ");
    gotoxy(x,y+5);printf("        ��      �ââââ�      ��      ��      ��  �ââââ�  �ââââ�            ");
    gotoxy(x,y+6);printf("                                                                                    ");
    setColor(GRAY);
    gotoxy(x,y+9);  printf("   +--------------+                                        ��   : Shift             ");     
    gotoxy(x,y+10); printf("   -              -                                      ��  �� : Left / Right      ");     
    gotoxy(x,y+11); printf("   -  21113683    -                                        ��   : Soft Drop         ");
    gotoxy(x,y+12); printf("   -              -                                      SPACE : Hard Drop          "); 
    gotoxy(x,y+14); printf("   -      �̰�ȣ  -                                       ESC  : Quit               ");  
    gotoxy(x,y+16); printf("   +--------------+                                    BONUS FOR HARD DROPS / COMBOS"); 
    Sleep(5000);
        
    while (kbhit()) getch(); 
 
}
 
void reset(void){
 
    
   
        
    level=1; //�������� �ʱ�ȭ 
    score=0;
    level_goal=1000;
    key=0;
    contact=0;
    delete_line=0;
    speed=100;
    
    system("cls"); //ȭ������ 
    reset_main(); // main1�� �ʱ�ȭ 
    draw_map(); // ����ȭ���� �׸�
    draw_main(); // �������� �׸� 
 
    block_type_next=rand()%7; //�������� ���� ��� ������ �����ϰ� ���� 
    new_block(); //���ο� ����� �ϳ� ����  
    
}
 
void reset_main(void){ //�ʵ� 
    int i,j;
    
    for(i=0;i<GAME_Y;i++){ // �ʵ��ʱ�ȭ  
        for(j=0;j<GAME_X;j++){
            main1[i][j]=0;
            main2[i][j]=100;
        }
    }    
    for(j=1;j<GAME_X;j++){ //õ��
        main1[3][j]=CANMOVE;
    }
    for(i=1;i<GAME_Y-1;i++){ //�¿� ���� 
        main1[i][0]=WALL;
        main1[i][GAME_X-1]=WALL;
    }
    for(j=0;j<GAME_X;j++){ //�عٴ� ���� 
        main1[GAME_Y-1][j]=WALL;
    }
}
 
void reset_main2(void){ //main2�� �ʱ�ȭ 
    int i, j;
    
    for(i=0;i<GAME_Y;i++){         //�����ǿ� ���ӿ� ������ �ʴ� ���ڸ� ���� 
        for(j=0;j<GAME_X;j++){  //�̴� main1�� ���� ���ڰ� ���� �ϱ� ���� 
            main2[i][j]=100;
        }
    }    
}
 
void draw_map(void){ //���� ���� ����
int y=3;             // level, goal, score�� ������ ���°� �ٲ�� ������ y�� ���� 
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
    gotoxy(STATUS_X_POS, y+15); printf("  ��   : Shift        SPACE : Hard Drop");     
    gotoxy(STATUS_X_POS, y+16); printf("��  �� : Left / Right");     
    gotoxy(STATUS_X_POS, y+17); printf("  ��   : Soft Drop     ESC  : Quit");
    gotoxy(STATUS_X_POS, y+20);printf("");
}
 
void draw_main(void){ //������ �׸��� �Լ� 
    int i, j;    
    
    for(j=1;j<GAME_X-1;j++){ //õ���� ��� ���ο���� �������� �������� ���� �׷��� 
        if(main1[3][j]==EMPTY) main1[3][j]=CANMOVE;
    }
    for(i=0;i<GAME_Y;i++){
        for(j=0;j<GAME_X;j++){
            if(main2[i][j]!=main1[i][j]){ //2�� ���ؼ� ���� �޶��� �κи� ���� �׷���.
                                                //�̰� ������ ��������ü�� ��� �׷��� �������� ��¦�Ÿ�
            gotoxy(GAME_X_POS+j,GAME_Y_POS+i); 
                switch(main1[i][j]){
                    case EMPTY: //��ĭ��� 
                        printf("  ");
                        break;
                    case CANMOVE: //õ���� 
                        printf(". ");
                        break;
                    case WALL: //����� 
                    	setColor(VIOLET);
                        printf("��"); 
                        break;
                    case MOVED_BLOCK: //���� �� ���  
                    	setColor(SKYBLUE);
                        printf("��");
                        break;
                    case ACTIVE_BLOCK: //�����̰��ִ� �� ���  
                    	setColor(DARK_YELLOW);
                        printf("��");
                        break;    
                }    
            }
        }
    }
    for(i=0;i<GAME_Y;i++){ //�������� �׸� �� main2�� ����  
        for(j=0;j<GAME_X;j++){
            main2[i][j]=main1[i][j];
        }
    }
}
 
void new_block(void){ //��Ʈ���̳� �����  
    int i, j;    
    
    bx=(GAME_X/2)-1;
    by=0;
    block_type=block_type_next; //���� ��Ʈ���̳� �� 
    block_type_next=rand()%7; //���� ��Ʈ���̳� ���� 
    block_spin=0;  
    
    new_block_on=0;   
    
    for(i=0;i<4;i++){ 
        for(j=0;j<4;j++){
            if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=ACTIVE_BLOCK;
        }
    }
    for(i=1;i<3;i++){ //���� ��Ʈ���̳� �̸����� 
        for(j=0;j<4;j++){
            if(blocks[block_type_next][0][i][j]==1) {
                gotoxy(STATUS_X_POS+2+j,i+6);
                printf("��");
            }
            else{
                gotoxy(STATUS_X_POS+2+j,i+6);
                printf("  ");
            }
        }
    }
}
 
void check_key(void){
    key=0; //Ű�� �ʱ�ȭ  
    
    if(kbhit()){ //Ű�Է��� �ִ� ���  
        key=getch(); //Ű���� ����
        if(key==224){ //����Ű�ΰ�� 
            do{key=getch();} while(key==224);//����Ű���ð��� ���� 
            switch(key){
                case LEFT: //����Ű ��������  
                    if(check_contact(bx-1,by,block_spin)==true) move_block(LEFT); 
                    break;                            //�������� �� �� �ִ��� üũ �� �����ϸ� �̵�
                case RIGHT: //������ ����Ű ��������- ���� �����ϰ� ó���� 
                    if(check_contact(bx+1,by,block_spin)==true) move_block(RIGHT);
                    break;
                case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó���� 
                    if(check_contact(bx,by+1,block_spin)==true) move_block(DOWN);
                    break;
                case UP: //���� ����Ű �������� 
                    if(check_contact(bx,by,(block_spin+1)%4)==true) move_block(UP); 
                                                        //ȸ���� �� �ִ��� üũ �� �����ϸ� ȸ��
                    else if(contact==1&&check_contact(bx,by-1,(block_spin+1)%4)==true) move_block(100);     
            }                    //�ٴڿ� ���� ��� �������� ��ĭ����� ȸ���� �����ϸ� �׷��� ��(Ư������)
        }
        else{ //����Ű�� �ƴ� �Է� 
            switch(key){
                case SPACE:
                    space_bar_on=1; 
                    while(contact==0){ //�ٴڱ��� �̵�
                        down_block();
                        score+=level; // hard drop ���ʽ� ����
                        gotoxy(STATUS_X_POS, STATUS_Y_SCORE); printf("        %6d", score);  
                    }
                    break;
                case ESC:
                    system("cls");
                    exit(0); //�������� 
            }
        }
    }
    while (kbhit()) getch(); //Ű���۸� ��� 
}
 
void down_block(void){
    int i,j;
    
    if(contact&&check_contact(bx,by+1, block_spin)==true) contact=0; //���� ��������� crush flag �� 
    if(contact&&check_contact(bx,by+1, block_spin)==false){ //���� ��������ʰ� crush flag�� ���������� 
        for(i=0;i<GAME_Y;i++){ //���� �������� ���� ���� 
            for(j=0;j<GAME_X;j++){
                if(main1[i][j]==ACTIVE_BLOCK) main1[i][j]=MOVED_BLOCK;
            }
        }
        contact=0; //flag�� �� 
        check_line(); //����üũ�� �� 
        new_block_on=1; //���ο� ������ flag�� ��    
        return; //�Լ� ���� 
    }
    if(check_contact(bx,by+1, block_spin)==true) move_block(DOWN); //���� ��������� ������ ��ĭ �̵� 
    if(check_contact(bx,by+1, block_spin)==false) contact++; //������ �̵��� �ȵǸ�  crush flag�� ��
}
 
int check_contact(int bx, int by, int block_spin){ //������ ��ǥ�� ȸ�������� �浹�� �ִ��� �˻� 
    int i,j;
    
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){ //������ ��ġ�� �����ǰ� ������� ���ؼ� ��ġ�� false�� ���� 
            if(blocks[block_type][block_spin][i][j]==1&&main1[by+i][bx+j]>0) return false;
        }
    }    
    return true; //�ϳ��� �Ȱ�ġ�� true���� 
};
 
void move_block(int dir){ //����� �̵���Ŵ 
    int i,j;
 
    switch(dir){
        case LEFT: //����
            for(i=0;i<4;i++){ //������ǥ�� ��Ʈ���̳븦 ���� 
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){ //�������� ��ĭ�� �� ��Ʈ���̳� ���� 
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j-1]=ACTIVE_BLOCK;
                }
            }
            bx--; //��ǥ�� �̵� 
            break;
        
        case RIGHT:    //������ 
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
        
        case DOWN:    //�Ʒ���
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
            for(i=0;i<4;i++){ //���� ��Ʈ���̳� ����  
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=EMPTY;
                }
            }
            block_spin=(block_spin+1)%4; //ȸ������ ����, ��� ȸ���ϸ� ���� ������� �ٽ� ���ƿ� (%4)
            for(i=0;i<4;i++){ //ȸ���� ��Ʈ���̳� ��� 
                for(j=0;j<4;j++){
                    if(blocks[block_type][block_spin][i][j]==1) main1[by+i][bx+j]=ACTIVE_BLOCK;
                }
            }
            break;
            
        case 100: //��Ʈ���̳밡 �ٴ�,�ٸ� ��Ʈ���̳�� ���� ���¿��� ��ĭ���� �÷� ȸ���� ������ ��� 
                
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
    
    int    block_count; //������ ��ϰ����� �����ϴ� ���� 
    int combo=0; //�޺����� �����ϴ� ���� ������ �ʱ�ȭ 
    
    for(i=GAME_Y-2;i>3;){ //i=MAIN_Y-2 : ���ʺ��� ��ĭ����,  i>3 : õ��(3)�Ʒ����� �˻� 
        block_count=0; //��ϰ��� ���� ���� �ʱ�ȭ 
        for(j=1;j<GAME_X-1;j++){ //���� �������� ��ϰ��縦 �� 
            if(main1[i][j]>0) block_count++;
        }
        if(block_count==GAME_X-2){ //����� ���� �� ��� 
            if(level_up_on==0){ //���������°� �ƴ� ��쿡(�������� �Ǹ� �ڵ� �ٻ����� ����) 
                score+=100*level; //�����߰� 
                delete_line++; //���� �� ���� ī��Ʈ ���� 
                combo++; //�޺��� ����  
            }
            for(k=i;k>1;k--){ //������ ��ĭ�� ��� ����(������ õ���� �ƴ� ��쿡��) 
                for(l=1;l<GAME_X-1;l++){
                    if(main1[k-1][l]!=CANMOVE) main1[k][l]=main1[k-1][l];
                    if(main1[k-1][l]==CANMOVE) main1[k][l]=EMPTY; 
//������ õ���� ��쿡�� õ���� ��ĭ ������ �ȵǴϱ� ��ĭ�� ���� 
                }
            }
        }
        else i--;
    }
    if(combo){ 
        if(combo>1){ //2�� �̻� ������ ��� 
            gotoxy(GAME_X_POS+(GAME_X/2)-1,GAME_Y_POS+by-2);printf("%d COMBO!",combo);
            Sleep(500);
            score+=(combo*level*100);
            reset_main2(); //�ؽ�Ʈ�� �����Ϸ���  main2�� �ʱ�ȭ�ؾ��� �ʵ带 ���� �ٽ� �׸��� �ʱ� ���� 
        }
        gotoxy(STATUS_X_POS, STATUS_Y_GOAL); printf(" GOAL  : %5d", (delete_line<=10)?10-delete_line:0);
        gotoxy(STATUS_X_POS, STATUS_Y_SCORE); printf("        %6d", score);    
    }
}
 
void check_level_up(void){
    int i, j;
    
    if(delete_line>=10){ //10���� ���� ������ ���� ��!! 
        draw_main();
        level_up_on=1; 
        level+=1; 
        delete_line=0; //�ٽ� 10�� ����� ���� 
        for(i=0;i<4;i++){
            gotoxy(GAME_X_POS+(GAME_X/2)-3,GAME_Y_POS+4); //������ ���ǵ�� ��ġ ���� 
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
        reset_main2(); //�޺��� ���� ������ �ʱ�ȭ 

        
        for(i=GAME_Y-2;i>GAME_Y-2-(level-1);i--){ //�������ϸ� �Ʒ� �� ������ 
           
            }
        }
         
        check_line(); //������� ä�� �� ����� 
      
        switch(level){ //�������ϸ� �ӵ��� ���� 
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
        level_up_on=0; //������ flag����
        
        gotoxy(STATUS_X_POS, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //����ǥ�� 
        gotoxy(STATUS_X_POS, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10-delete_line); // ������ǥ ǥ�� 
    
    }

 
void check_game_over(void){ 
    int i;
    
    int x=5;
    int y=5;
    
    for(i=1;i<GAME_X-2;i++){
        if(main1[3][i]>0){ //õ�忡 �� ����� ���� ���� 
        	setColor(YELLOW);
            gotoxy(x+15,y+0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
            gotoxy(x+15,y+1); printf("��                              ��");
            gotoxy(x+15,y+2); printf("��                              ��");
            gotoxy(x+15,y+3); printf("��     G A M E     O V E R..    ��");
            gotoxy(x+15,y+4); printf("��                              ��");
            gotoxy(x+15,y+5); printf("��                              ��");
            gotoxy(x+15,y+6); printf("��                              ��");
            gotoxy(x+15,y+7); printf("��                              ��");
            gotoxy(x+15,y+8); printf("��                              ��");
            gotoxy(x+15,y+9); printf("��                              ��");
            gotoxy(x+15,y+10); printf("��                              ��");
            gotoxy(x+15,y+11); printf("��                              ��");
            gotoxy(x+15,y+12); printf("��        YOUR SCORE: %6d    ��",score);
            gotoxy(x+15,y+13); printf("��                              ��");
            gotoxy(x+15,y+14); printf("��                              ��");
            gotoxy(x+15,y+15); printf("��  Press any key to restart!   ��");
            gotoxy(x+15,y+16); printf("��                              ��");
            gotoxy(x+15,y+17); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

                  
            Sleep(1000);
            while (kbhit()) getch();
            key=getch();
            reset();
        }
    }
}
 
