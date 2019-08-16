#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <conio.h>//kbhit()使用

int score = 0 ;
int rank  = 0 ;

#define BLOCK_COUNT   5
#define BLOCK_WIDTH   5
#define BLOCK_HEIGHT  5
#define UNIT_SIZE     20

#define START_X       130
#define START_Y       30

#define KEY_UP        72
#define KEY_RIGHT     77
#define KEY_LEFT      75
#define KEY_SPACE     32
#define KEY_DOWN      80

int speed = 500;
int minX  = 30;
int minY  = 30;

typedef enum{
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
}move_dir_t;

typedef enum{
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWN,
	BLOCK_LEFT
}block_dir_t;

int NextIndex = -1;
int BlockIndex = -1;

int color[BLOCK_COUNT] = {
	GREEN,CYAN,MAGENTA,BROWN,YELLOW
};

int visit[30][15];//访问数组
int markColor[30][15];//表示对应位置的颜色

int block[BLOCK_COUNT*4][BLOCK_HEIGHT][BLOCK_WIDTH] = {
//|形状
   {
	0,0,0,0,0,
    0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
  	0,0,0,0,0
     },
 	{	0,0,0,0,0,
		0,0,0,0,0,
		0,1,1,1,0,
		0,0,0,0,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,0,0,0,0,
		0,1,1,1,0,
		0,0,0,0,0,
		0,0,0,0,0,
	},

	//L型方块 4

	{	0,0,0,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},

	{	0,0,0,0,0,
		0,0,0,0,0,
		0,1,1,1,0,
		0,1,0,0,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,1,1,0,0,
		0,0,1,0,0,
		0,0,1,0,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,0,0,1,0,
		0,1,1,1,0,
		0,0,0,0,0,
		0,0,0,0,0,
	},
	//田型方块 8

	{	0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,1,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},

	{	0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,1,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,1,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,1,0,
		0,0,1,1,0,
		0,0,0,0,0,
	},
	//Z型方块 12

	{	0,0,0,0,0,
		0,1,1,0,0,
		0,0,1,1,0,
		0,0,0,0,0,
		0,0,0,0,0,
	},

	{	0,0,0,0,0,
		0,0,1,0,0,
		0,1,1,0,0,
		0,1,0,0,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,1,1,0,0,
		0,0,1,1,0,
		0,0,0,0,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,0,1,0,0,
		0,1,1,0,0,
		0,1,0,0,0,
		0,0,0,0,0,
	},
	//T型方块 16

	{	0,0,0,0,0,
		0,1,1,1,0,
		0,0,1,0,0,
		0,0,0,0,0,
		0,0,0,0,0,
	},

	{	0,0,0,0,0,
		0,0,0,1,0,
		0,0,1,1,0,
		0,0,0,1,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,0,0,0,0,
		0,0,1,0,0,
		0,1,1,1,0,
		0,0,0,0,0,
	},


	{	0,0,0,0,0,
		0,1,0,0,0,
		0,1,1,0,0,
		0,1,0,0,0,
		0,0,0,0,0,
	}

};

//实现欢迎界面
void welcome(void){
	initgraph(550,660);
	
	//设置窗口标题
	HWND hwnd = GetHWnd();//获取窗口
	SetWindowText(hwnd,_T("俄罗斯方块            kinghero"));
	
	//游戏标题设置  
	setfont(40,0,_T("微软雅黑"));//字体式样
	setcolor(WHITE);
	outtextxy(205,200,_T("俄罗斯方块"));
    
	//小标题
	setfont(22,0,_T("楷体"));
	outtextxy(175,300,_T("编程，从俄罗斯方块开始？？？"));

	Sleep(3000);

}

void initGameScene(void){
	char str[16];

	//清除屏幕
	cleardevice();

	rectangle(27,27,336,635);
	rectangle(29,29,334,633);
	rectangle(370,50,515,195);

	setfont(24,0,_T("楷体"));
	setcolor(LIGHTGRAY);
	outtextxy(405,215,_T("下一个"));
	
	setcolor(RED);
	outtextxy(405,280,_T("分数："));
	sprintf(str,"%d",score);//写入分数score中的分写到str，并在下面一行代码中打印
	outtextxy(415,310,str);

	outtextxy(405,375,"等级：");
	sprintf(str,"%d",rank);
	outtextxy(425,405,str);

	//打印操作说明
	setcolor(LIGHTBLUE);
	outtextxy(390,475,"操作说明");
	outtextxy(390,500,"↑：旋转");
	outtextxy(390,525,"↓：下降");
	outtextxy(390,550,"←：左移");
	outtextxy(390,575,"→：右移");
	outtextxy(390,600,"空格：暂停");
}

void clearBlock(void){
	setcolor(BLACK);
	setfont(23,0,"楷体");

	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			//■
			int x = 391 + j * UNIT_SIZE;
			int y = 71 + i * UNIT_SIZE;
			outtextxy(x,y,"■");
        }
	}
}

//绘制方块
void drawBlock(int x,int y){
	setcolor(color[NextIndex]);
	setfont(23,0,"楷体");

	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			//■
			if (block[NextIndex*4][i][j]){
			int x2 = x + j * UNIT_SIZE;
			int y2 = y + i * UNIT_SIZE;
			outtextxy(x2,y2,"■");
			}
        }
	}
}

//绘制方块：在指定位置绘制指定方块的指定方向
void drawBlock(int x,int y,int blockIndex,block_dir_t dir){
	setcolor(color[blockIndex]);
	setfont(23,0,"楷体");
	int id = blockIndex * 4 + dir;
	

	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			//■
			if (block[id][i][j]){
			int x2 = x + j * UNIT_SIZE;
			int y2 = y + i * UNIT_SIZE;
			outtextxy(x2,y2,"■");
			}
        }
	}
}

//清除指定位置指定方向的方块
//参数x：方块的左上角的x坐标
//参数y：方块左上角在游戏区域中的坐标
void clearBlock(int x,int y,block_dir_t dir){
	setcolor(BLACK);
	int id = BlockIndex * 4 + dir;
	y +=START_Y;

	for (int i=0;i<5;i++){
		for (int j=0;j<5;j++){
			if (block[id][i][j]){
				//擦除该方块的第i行第j列
				outtextxy(x+20*j,y+i*20,"■");
			
			}
		}
	}
}

void nextblock(void){
	clearBlock();//清除右上角区域

	//随机选择一种方块
	srand(time(NULL));//使用时间函数返回值，作为随机种子
	NextIndex = rand()%BLOCK_COUNT;
	
	drawBlock(391,71);
}

//如果在指定位置能向指定方向移动，返回1，否则返回0
int moveable(int x0,int y0,move_dir_t moveDir,block_dir_t blockDir){
	//计算当前方块的左上角在30*15的游戏区的位置（多少行，多少列）
	int x = (y0 - minY)/UNIT_SIZE;
	int y = (x0 - minX)/UNIT_SIZE;
	int id = BlockIndex * 4 + blockDir;
	int ret = 1;


	if (MOVE_DOWN == moveDir){
		for (int i=0;i<5;i++){
			for(int j=0;j<5;j++){
				if (block[id][i][j] == 1&&
					(x + i >= 29 || visit[x+i+1][y+j] == 1)){
						ret = 0;
				}
			}		
		}
	}else if (MOVE_LEFT == moveDir){
		for (int i=0;i<5;i++){
			for(int j=0;j<5;j++){
				if (block[id][i][j] == 1&&
					(y + j == 0 ||  visit[x+i][y+j-1] == 1)){
						ret = 0;
				}
			}		
		}
	}else if (MOVE_RIGHT == moveDir){
		for (int i=0;i<5;i++){
			for(int j=0;j<5;j++){
				if (block[id][i][j] == 1&&
					(y + j >= 14 || visit[x+i][y+j+1] == 1)){
						ret = 0;
				}
			}		
		}
	}
	return ret;
}

//检查游戏是否结束
void failCheck(){
	if (!moveable(START_X,START_Y,MOVE_DOWN,BLOCK_UP)){
		setcolor(WHITE);
		setfont(45,0,"微软雅黑");
		outtextxy(75,300,"GAME OVER!");
		Sleep(500);
	    system("pause");
		closegraph();
		exit(0);
	}
}

void wait(int interval){//500
	int count = interval/10;
	for (int i =0;i<count;i++){
		Sleep(10);
		if (kbhit()){			
				return;						
		}
	}
}

//判断当前方块假设旋转后的是否合法
int rotatable(int x,int y,block_dir_t dir){
	int id = BlockIndex * 4 +dir;
	int xIndex = (y - minY)/20;
	int yIndex = (x - minX)/20;


	if (!moveable(x,y,MOVE_DOWN,dir)){
		return 0;
	}
	for (int i=0;i<5;i++){
		for (int j=0;j<5;j++){
			if (block[id][i][j] && 
				(yIndex+j<0 || yIndex+j>15 || visit[xIndex+i][yIndex+j])){
					return 0;
			}
		}
	}

	return 1;
}

void mark(int x,int y,int blockIndex,block_dir_t dir){
	int id = blockIndex * 4 + dir;
	int x2 = (y - minY)/20;
	int y2 = (x - minX)/20;

	for (int i=0;i<5;i++){
		for (int j=0;j<5;j++){
			if (block[id][i][j]){
				visit[x2+i][y2+j] = 1;
				markColor[x2+i][y2+j] = color[blockIndex];
			}		
		}
	}	
}

void move(void){
	int x = START_X;
	int y = START_Y;
	int k = 0;
	block_dir_t blockDir = BLOCK_UP;
	int curSpeed = speed;

	//检测游戏是否结束
	failCheck();
	
	//持续下降
	while(1){
		if(kbhit()){
			int key = getch();
			if (KEY_SPACE == key){
				getch();
			
			}
		}

		//清除当前方块
		clearBlock(x,k,blockDir);

		if (kbhit()){
			int key = getch();

			if(KEY_UP == key){
				block_dir_t nextDir = (block_dir_t)((blockDir + 1)%4);
				if (rotatable(x,y+k,nextDir)){
					blockDir = nextDir;
				}
			}else if(KEY_DOWN == key){
				curSpeed = 50;
			}else if(KEY_LEFT == key){
				if (moveable(x,y+k+20,MOVE_LEFT,blockDir)){
					x -=20;
				}
			}else if(KEY_RIGHT == key){
				if (moveable(x,y+k+20,MOVE_RIGHT,blockDir)){
					x +=20;
				}
			}
		}

		k+=20;
		//绘制当前方块
		drawBlock(x,y+k,BlockIndex,blockDir);

		wait(curSpeed);

		//方块的固化处理
		if (!moveable(x,y+k,MOVE_DOWN,blockDir)){
			mark(x,y+k,BlockIndex,blockDir);
			break;
		}
	}
}

void newblock(void){
	//确定即将使用的方块类型
	BlockIndex = NextIndex;

	//绘制刚从顶部下降的方块
	drawBlock(START_X,START_Y);

	//让新出现的方块暂停一会，让用户识别
	Sleep(100);//0.1秒

	//在右上角绘制下一个的方块
	nextblock();

	//方块降落
	move();
}

//消除第i行，并把上面的行都下移
void down(int x){
	for (int i=x;i>0;i--){
		//准备消除第i行第j列的方块
		for (int j=0;j<15;j++){
			if (visit[i-1][j]){
				visit[i][j]=1;
				markColor[i][j] = markColor[i-1][j];
				setcolor(markColor[i][j]);
				outtextxy(j*20+30,i*20+30,"■");
			}else{
				visit[i][j] = 0;
				setcolor(BLACK);
				outtextxy(j*20+30,i*20+30,"■");			
			}		
		}
	}
	//清除行标为0的那一行
	setcolor(BLACK);
	for (int j=0;j<15;j++){
		visit[0][j] = 0;
		outtextxy(j*20+30,30,"■");
	}
}

//更新参数，参数lines表示消除行数
void addScore(int lines){
	char str[32];

	setcolor(RED);
	score += lines * 10;
	sprintf(str,"%d",score);
	outtextxy(415,310,str);

}

void updateGrade(){
	//更新等级的提示
	//假设50分一级
	rank = score/50 + 1;
	char str[16];
	sprintf(str,"%d",rank);
	outtextxy(425,405,str);

	//更新速度，等级越高，速度越快，speed越小
	speed = 500 - rank * 100;
	if (speed <= 100){
		speed = 100;
	}
}

void check(void){
	int i,j;
	int clearLines = 0;
	for (i=29;i>=0;i--){
		//检查第i行是否满
		for (j=0;j<15 && visit[i][j];j++);

		//执行到此处时，有两种情况
		//1.第i行没有满，j<15
		//2.第i行已经满了j>=15
		if (j >= 15){
			//此时第i行已经满了，需要消除第i行
			down(i);//消除第i行，并把上面的函数下移
			i++;//使得下次循环时再把第i行检查一下
			clearLines++;
		}
	}
	//更新分数
	addScore(clearLines);

	//更新等级(等级和速度)
	updateGrade();
}

int main(void){
	welcome();
	initGameScene();

	//产生新方块
	nextblock();
	Sleep(500);
	
	//初始化访问数组
	memset(visit,0,sizeof(visit));

	while (1){
		newblock();
		
		//消除满行，并更新分数和速度
		check();
	}
	

	system("pause");
	closegraph();
	return 0;
}