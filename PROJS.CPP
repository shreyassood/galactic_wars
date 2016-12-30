#include<iostream.h>//Cout
#include<conio.h>//clrscr, gotoxy, cursor, COLOR constants, kbhit, getch, _setcursortype,, _nocursor constant, textbackground, textcolor
#include<stdlib.h>//randomize, random, i to a (integer to string), exit
#include<string.h>//Strcpy, strcat, strlen
#include<graphics.h> //Various Graphics Mode functions such as outtextxy,fillellipse,etc.
#include<dos.h> //Delay
#include<math.h> //sqrt, pow

//-------------
//--Constants--
//-------------
const int FALSE = 0;
const int TRUE = !FALSE;

const int STATUS_BACK = 0;
const int STATUS_OVER = 1;
const int STATUS_WON = 2;


const int MINWIDTH = 1 + 1;
const int MAXWIDTH = 80 - 1;
const int MINHEIGHT = 1 + 1;
const int MAXHEIGHT =  25 - 2;
const int MAXENEMSHIPTYPES = 2;
const int MAXSHIPSIZE = 10;
const int USER = 1;
const int ENEMY = 2;
const int MAX_NUM_BULLETS = 255;
const int MAX_NUM_ENEM = 15;
const int MAX_COLLISIONS = MAX_NUM_ENEM;
const int BASETIME = 30; //Base Redraw
const int MAXTIMESLOWERTHANBASETIME = 100; //For count
const int MAXTIME = MAXTIMESLOWERTHANBASETIME * BASETIME;
const int STAGE_ENEMS = 3;
const int MAXSTAGES = 4;
const int BGCOLORS[MAXSTAGES] = {BLACK,BLUE,BROWN,GREEN};
const int TEXTCOLORS[MAXSTAGES] = {YELLOW,WHITE,BLACK,BLUE};
const int BGCOLORCHANGINGSTAGES = 5; //Should be odd
const int HPPERLIFE = 8;
const int LIVES = 2;
const int INVINCIBLEFLASHRATE = 6;
const int MAXINVINCIBLE = INVINCIBLEFLASHRATE * 12;
const int MAXSTAGECHANGINGTEXT = 100;
const char BAR[2] = {254,'\0'};
const char HEART[2] = {3,'\0'};
const char GRAPH_ADD[20] = "A:/BGI";
const int BOSSX = 5;
const int BOSSMAXHEALTH = 15;

//--------------
//--Structures--
//--------------

struct Collision{
	int xo, //X Coordinate for centre
	yo, //Y Coordinate for centre
	stage, //Current Stage/Size
	maxstage; //MaxStage/Size
};

struct ship{
	char
	shape[MAXSHIPSIZE][MAXSHIPSIZE]; //Contains the shape of the ship
	int
	height, //Height of the Ship
	width, //Width of the Ship
	x, //Current X Position
	y, //Current Y Position
	disp, //Currently displayed boolean
	bulletx, //X Position where bullets are shot
	bullety; //Y Position where bullets are shot
};

struct Bullet{
	int x, //X Position
		y, //Y Position
		disp, //If it is being used/displayed
		src; //Source (User/Enemies)
};

//--------------
//--Functions---
//--------------

int min(int a,int b);
//Returns the smaller of the two integer parameters

void gamewon(int score);
//Shows the graphics screen when game is won

void gameover(int score);
//Shows the graphics screen when game is lost

void ascii(int x);
//Takes the ascii code and prints the corresponding character

void putShip(ship &s,char c[MAXSHIPSIZE*MAXSHIPSIZE],int width,int height,int x,int y,int bulletx, int bullety);
//Intitializes the ship structure by putting in details

void getShip(int choice,char c[MAXSHIPSIZE*MAXSHIPSIZE],int& width,int& height,int& bulletx, int& bullety);
//Gets details for various ships from a list of choices

void draw(ship s);
//Draws the ship

short inscreen(int x,int y);
//Check if the x,y coordinates are within the screen (So it doen't print them causing overlapping)
void spawnbullet(Bullet bullets[], int& n, ship s,int src);
//Creates a new bullet from a ship

int bulletShipIntersect(Bullet b,ship s);
//Checks if a bullet and ship are coinciding (for collision)

int game(int& totalscore);
//The main game functions which return various statuses i.e. Game Won, Game Over and Back

void draw_title();
//Draws the title on the landing screen

void gameloading();
//Shows the loading screen before the game starts

void pressenter(int x);
//Executes various choices on the main screen i.e. Start Game, credits and Exit

void main();
//Starts the program by showing the landing page

int min(int a,int b)
{
	return (a<b?a:b);
}
void gamewon(int score)
{
	int gd = DETECT,gm;
	initgraph(&gd,&gm,GRAPH_ADD);
	int maxx = getmaxx();
	int maxy = getmaxy();
	settextstyle(DEFAULT_FONT,0,6);
	setbkcolor(WHITE);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,GREEN);
	int circleradius = (sqrt(pow(maxx,2) + pow(maxy,2))/2) + 60;
	settextstyle(DEFAULT_FONT,0,5);
	for(int i = 0; i<= circleradius; i+=30)
	{
		delay(60);
		fillellipse(maxx/2,maxy/2,i,i);
		outtextxy(23,215,"Congratulations");
	}
	char ch;
	do{
		while(!kbhit()){
			fillellipse(maxx/2,maxy/2,i,i);
			delay(500);
			settextstyle(DEFAULT_FONT,0,6);
			outtextxy(93,215,"GAME WON!");
			settextstyle(DEFAULT_FONT,0,2);
			outtextxy(193,280,"Your Score:");
			char sctext[10];
			itoa(score,sctext,10);
			outtextxy(385,280,sctext);
			outtextxy(170,400,"Press 'E' to return to menu");

			delay(500);
		};
		ch = getch();
	}while(ch!='e');

	closegraph();
}
void gameover(int score)
{
	int gd = DETECT,gm;
	initgraph(&gd,&gm,GRAPH_ADD);
	int maxx = getmaxx();
	int maxy = getmaxy();
	settextstyle(DEFAULT_FONT,0,6);
	setbkcolor(WHITE);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,RED);
	int circleradius = (sqrt(pow(maxx,2) + pow(maxy,2))/2) + 60;
	settextstyle(DEFAULT_FONT,0,5);
	for(int i = 0; i<= circleradius; i+=30)
	{
		delay(60);
		fillellipse(maxx/2,maxy/2,i,i);
		outtextxy(150,225,"You lose");
	}
	char ch;
	do{
		while(!kbhit()){
			fillellipse(maxx/2,maxy/2,i,i);
			delay(500);
			settextstyle(DEFAULT_FONT,0,6);
			outtextxy(93,215,"GAME OVER!");
			//settextstyle(DEFAULT_FONT,0,1);
			settextstyle(DEFAULT_FONT,0,2);
			outtextxy(193,280,"Your Score:");
			char sctext[10];
			itoa(score,sctext,10);
			outtextxy(385,280,sctext);
			outtextxy(170,400,"Press 'E' to return to menu");
			delay(500);
		};
		ch = getch();
	}while(ch!='e');
	closegraph();
}

void ascii(int x)
{
	cout<<char(x);
}

void putShip(ship &s,char c[MAXSHIPSIZE*MAXSHIPSIZE],int width,int height,int x,int y,int bulletx, int bullety)
{
	int o = 0;
	s.width = width;
	s.height = height;
	s.x = x;
	s.y = y;
	s.bulletx = bulletx;
	s.bullety = bullety;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			s.shape[i][j] = c[o];
			o++;
		}
	}
}
void getShip(int choice,char c[MAXSHIPSIZE*MAXSHIPSIZE],int &width,int &height,int &bulletx, int &bullety){
	switch(choice)
	{
		//USER SHIP
	case 0:
		strcpy(c,
		"�� "
		"���"
		"��");
		width = 3;
		height = 3;
		bulletx = 4;
		bullety = 2;
		break;
	case 1:
		strcpy(c,
		" � "
		"�=="
		" � ");
		width = 3;
		height = 3;
		bulletx = 0;
		bullety = 2;
		break;
	case 2:
		strcpy(c,
		" �       "
		"��======�"
		" �       ");
		width = 9;
		height = 3;
		bulletx = 0;
		bullety = 2;
		break;
		//Boss
	case 3:
		strcpy(c,
		"   /////--<"
		"<<<||||||| "
		"   \\\\\\\\\\--<");
		width = 11;
		height = 3;
		bulletx = 0;
		bullety = 2;
		break;
	}
}
void draw(ship s)
{
	//Drawing Rowwise
	for(int i=0;i<s.height;i++)
	{
		for(int j=0;j<s.width;j++)
		//Don't draw part of the ship outside the screen
		if(s.x + j >= MINWIDTH && s.x + j <= MAXWIDTH)
		{
			gotoxy(s.x+j,s.y+i);
			cout<<s.shape[i][j];
		}
	}
}
short inscreen(int x,int y){
	if(		x>=MINWIDTH
			&& 	x<=MAXWIDTH
			&&	y>=MINHEIGHT
			&& 	y<=MAXHEIGHT)
	return TRUE;
	else
	return FALSE;
}
void spawnbullet(Bullet bullets[], int &n, ship s,int src)
{
	int inx;
	if(n<MAX_NUM_BULLETS)
	{
		inx = n;
		n++;
	}
	else
	{
		inx=n=0;
		n++;
	}
	bullets[inx].y = s.y + s.bullety - 1;
	bullets[inx].x = s.x + s.bulletx - 1;
	bullets[inx].src = src;
	bullets[inx].disp = TRUE;
}
int bulletShipIntersect(Bullet b,ship s)
{
	if(b.x >=  s.x && b.x <= s.x + s.width - 1)
		if(b.y >= s.y && b.y <= s.y + s.height - 1)
			return TRUE;
	return FALSE;
}
int game(int &totalscore)
{

	_setcursortype(_NOCURSOR);
	clrscr();
	randomize();


	//Initialize User Ship
	ship imp;
	int width,height,bulletx,bullety;
	char c[MAXSHIPSIZE*MAXSHIPSIZE];
	getShip(0,c,width,height,bulletx,bullety);
	putShip(imp,c,width,height,MINWIDTH + 1,(MINHEIGHT + MAXHEIGHT/2) - 1,bulletx,bullety);

	//Initialize Boss Ship
	ship boss;
	getShip(MAXENEMSHIPTYPES + 1,c,width,height,bulletx,bullety);
	putShip(boss,c,width,height,MAXWIDTH,(MINHEIGHT + MAXHEIGHT/2) - 1,bulletx,bullety);

	//Initializing bullets, enemy ships, collisions and setting their displays to false
	Bullet bullets[MAX_NUM_BULLETS];
	ship enemies[MAX_NUM_ENEM];
	Collision collisions[MAX_COLLISIONS];

	for(int i=0;i<MAX_NUM_BULLETS;i++)
	bullets[i].disp =FALSE;

	for(int m=0;m<MAX_NUM_ENEM;m++)
	enemies[m].disp = FALSE;

	for(m=0;m<MAX_COLLISIONS;m++)
	{
		collisions[m].stage = 0;
	}

	char ch;//For choice
	int count = 1; //For timing
	int b = 4;	//For Border Timing
	int n=0; //For current bullet count
	totalscore = 0;
	int stagescore = 0;
	int stage = 1;
	int stagechanging = FALSE;
	int stagechangingcount = 1;
	int stagechangingtext = TRUE;
	int stagechangingtextcount = 1;
	int color = 0;
	int hp = HPPERLIFE;
	int lives = LIVES;
	int invincible = TRUE;
	int invincibledisp = 1;
	int invinciblecount = 1;

	int BossMode = FALSE;
	int BossHealth = BOSSMAXHEALTH;

	do{

		while(!kbhit()){
			delay(BASETIME);
			if(stagechanging==TRUE)
			{
				if(count%6==0)
				{
					if(stagechangingcount%2 != 0)
					color++;
					else
					color--;
					stagechangingcount++;
				}
				if(stagechangingcount == BGCOLORCHANGINGSTAGES + 1)
				{
					stagechanging = FALSE;
					stagechangingcount = 1;
				}
			}

			if(BossMode == TRUE || BossHealth == 0)
			{
				textbackground(BLACK);
				textcolor(color);
				if(count%10==0){
					color = 2 + random(6);
				}
			}
			else
			{
				textbackground(BGCOLORS[color]);
				textcolor(TEXTCOLORS[color]);
			}
			clrscr();

			//1. DRAWING

			//Draw Border (Moving and changes every %4 times)
			if(count%4==0)
			b--;
			if(b==0)
			b=4;
			//Top and Bottom Borders
			int m = b;
			while(m<=MAXWIDTH){
				gotoxy(m,1);
				ascii(205);ascii(205);
				gotoxy(m,MAXHEIGHT + 1);
				ascii(205);ascii(205);
				m+=4;
			}

			//SideBorder
			for(m=MINHEIGHT;m<=MAXHEIGHT;m+=2)
			{
				gotoxy(MINWIDTH-1,m);
				ascii(186);
				gotoxy(MAXWIDTH+1,m);
				ascii(186);

			}

			//Corners
			//Top
			//Left
			gotoxy(MINWIDTH - 1,1);
			ascii(201);
			//Right
			gotoxy(MAXWIDTH + 1,1);
			ascii(187);
			//Bottom
			//Left
			gotoxy(MINWIDTH - 1,MAXHEIGHT+1);
			ascii(200);
			//Right
			gotoxy(MAXWIDTH + 1,MAXHEIGHT+1);
			ascii(188);

			//Print Bottom Bar (Score, Life etc.)
			char bottom[MAXWIDTH - 1] = "";

			//HP
			strcat(bottom,"HP: ");
			for(int u=1;u<=hp;u++)
			strcat(bottom,BAR);
			for(u=1;u<=HPPERLIFE-hp;u++)
			strcat(bottom," ");

			//Lives with invincible flashing
			strcat(bottom,"  LIVES: ");
			if(invincible == FALSE || (invincible==TRUE && invincibledisp == 1))
			{
				for(u=1;u<=lives;u++)
				strcat(bottom,HEART);
				for(u=1;u<=LIVES-lives;u++)
				strcat(bottom," ");
			}
			else
			{
				for(u=1;u<=LIVES;u++)
				strcat(bottom," ");
			}


			//Score
			strcat(bottom,"  SCORE: ");
			char score[4];
			itoa(totalscore,score,10);
			strcat(bottom,score);
			int length = strlen(bottom);
			strcat(bottom,"  'e': Exit");
			gotoxy((MINWIDTH + MAXWIDTH/2)-(length/2),MAXHEIGHT+2);
			cout<<bottom;

			//Draw Explosions/ Draw Collisions (Or Condition for a faster collision when Boss dead)
			if(count%2==0 || BossHealth == 0)
			{
				for(m=0;m<MAX_COLLISIONS;m++)
				{
					if(collisions[m].stage > 0)
					{
						int x = collisions[m].xo-collisions[m].stage + 1,
						y = collisions[m].yo-collisions[m].stage + 1,
						squareside = (2*collisions[m].stage) - 1;
						for(int i=0;i<squareside;i++)
						{
							for(int j=0;j<squareside;j++)
							{
								//Fancy Random Explosions
								if(random(10)>3)
								if(inscreen(x+i,y+j))
								{
									gotoxy(x+i,y+j);
									ascii(177);
								}
							}
						}
						collisions[m].stage++;
						if(collisions[m].stage==collisions[m].maxstage+1)
						{
							collisions[m].stage = 0;
							if(BossHealth==0)
							{
								return STATUS_WON;
							}
						}
					}
				}
			}



			//Draw Enemy Ships
			for(i = 0;i<MAX_NUM_ENEM;i++){
				if(enemies[i].disp == TRUE){
					draw(enemies[i]);
				}
			}

			//Draw Boss
			if(BossMode == TRUE)
			{
				draw(boss);
			}

			//Draw user ship
			if(invincible == TRUE)
			{
				if(invincibledisp==1)
				{
					draw(imp);
				}
			}
			else
			{
				draw(imp);
			}

			//Invincible flashing
			if(invincible == TRUE)
			{
				invinciblecount++;
				if(invinciblecount%INVINCIBLEFLASHRATE==0)
				invincibledisp *= -1;
				if(invinciblecount == MAXINVINCIBLE)
				{
					invinciblecount = 1;
					invincible = FALSE;
				}
			}

			//Draw and Move Bullets
			for(int i=0;i<MAX_NUM_BULLETS;i++)
			{
				if(bullets[i].disp == TRUE)
				{
					if(bullets[i].src == USER)
					{
						gotoxy(bullets[i].x++,bullets[i].y);
						ascii(240);
						if(bullets[i].x>MAXWIDTH)
						bullets[i].disp = FALSE;
					}
					//Slower Bullets (Enemies)
					else
					{
						gotoxy(bullets[i].x,bullets[i].y);
						ascii(240);
						if(count%2 == 0)
						bullets[i].x--;
						if(bullets[i].x<MINWIDTH)
						bullets[i].disp = FALSE;
					}

				}
			}

			//Print New Stage Changed Text
			if(stagechangingtext == TRUE)
			{
				char st[25] = "Level ";
				if(BossMode == FALSE)
				{
					char stagetext[2];
					itoa(stage,stagetext,10);
					strcat(st,stagetext);
				}
				else
				strcpy(st,"PREPARE FOR BOSS!");
				int len = strlen(st);
				gotoxy((MINWIDTH + MAXWIDTH)/2 - len/2,(MINHEIGHT + MAXHEIGHT)/3.5);
				cout<<st;
				stagechangingtextcount++;
				if(stagechangingtextcount==MAXSTAGECHANGINGTEXT+1)
				{
					stagechangingtext = FALSE;
					stagechangingtextcount = 1;
				}
			}




			//2. Moving and Spawing
			//Move Enemy Ships
			if(count%4==0)
			{
				for(i = 0;i<MAX_NUM_ENEM;i++){
					if(enemies[i].disp == TRUE){
						//gotoxy(1,MAXHEIGHT);
						//cout<<enemies[i].x<<" "<<enemies[i].y;
						enemies[i].x--;
						if(enemies[i].x==-enemies[i].width)
						enemies[i].disp = FALSE;
					}
				}
			}
			//Spawn Enemy Ship
			if(count%(100/stage)==0 && BossMode==FALSE && BossHealth!=0){
				for(i = 0;i<MAX_NUM_ENEM;i++){
					if(enemies[i].disp == FALSE){
						int width,height,bulletx,bullety;
						char c[MAXSHIPSIZE*MAXSHIPSIZE];
						getShip(1 + random(MAXENEMSHIPTYPES),c,width,height,bulletx,bullety);
						putShip(enemies[i],c,width,height,MAXWIDTH,MINHEIGHT + random(MAXHEIGHT - height - (MINHEIGHT - 1)),bulletx,bullety);
						enemies[i].disp = TRUE;
						break;
					}
				}
			}
			//Spawn Enemy Bullets //TODO
			for(i = 0;i<MAX_NUM_ENEM;i++){
				if(enemies[i].disp == TRUE){
					if(random(100)<stage*1.5) //1.5% Randomness at stage 1 (Every 25milliseconds)
					{
						spawnbullet(bullets,n,enemies[i],ENEMY);
					}
				}
			}

			//Spawn Boss, Boss Bullets & Move Boss
			if(BossMode == TRUE)
			{
				//Get Into Frame
				if(count%4==0)
				if(boss.x>MAXWIDTH - BOSSX - boss.width)
				boss.x--;
				//Move Up & Down
				if(count%12==0)
				{
					if(random(2)==0)
					{
						if(boss.y>MINHEIGHT)
						boss.y--;
					}
					else if(boss.y + boss.height<MAXHEIGHT)
					boss.y++;
				}
				//Spawn Boss Bullets
				if(random(100)<30)
				spawnbullet(bullets,n,boss,ENEMY);
			}




			//3. Checking

			//COLLISIONS
			for(int a=0;a<MAX_NUM_BULLETS;a++)
			{
				if(bullets[a].disp == TRUE && BossHealth!=0)
				{
					if(bullets[a].src == USER)
					{
						//Check enemy
						for(int b=0;b<MAX_NUM_ENEM;b++)
						{
							if(enemies[b].disp == TRUE)
							{
								//CHECK X SAME as first of SHIP
								if(bulletShipIntersect(bullets[a],enemies[b]))
								{
									enemies[b].disp = FALSE;
									bullets[a].disp = FALSE;
									//Scorings
									totalscore+=10;
									stagescore++;
									//EXPLOSION
									for(int m=0;m<MAX_COLLISIONS;m++)
									{
										if(collisions[m].stage == 0)
										{
											collisions[m].xo = bullets[a].x;
											collisions[m].yo = bullets[a].y;
											collisions[m].stage = 1;
											//Average of minimum b/w width and height and average of width and height
											collisions[m].maxstage = (min(enemies[b].width,enemies[b].height) + ((enemies[b].width + enemies[b].height) / 2))/2;
											break; //Don't look for more empty Collisions
										}
									}
								}
							}
						}
						//Check Boss
						if(BossMode==TRUE)
						{
							if(bulletShipIntersect(bullets[a],boss))
							{
								//GAME OVER or Reduce Health
								BossHealth--;
								bullets[a].disp = FALSE;
								//Scorings
								totalscore+=50;
								if(BossHealth==0)
								{
									BossMode = FALSE;
									//End all other explosions
									for(int m=0;m<MAX_COLLISIONS;m++)
									{
										collisions[m].stage = 0;
									}
								}
								//EXPLOSION
								for(int m=0;m<MAX_COLLISIONS;m++)
								{
									if(collisions[m].stage == 0)
									{
										collisions[m].xo = bullets[a].x;
										collisions[m].yo = bullets[a].y;
										collisions[m].stage = 1;
										//BigCollision if dead otherwise average of boss width & height
										collisions[m].maxstage = (BossHealth!=0?(boss.width+boss.height)/2:boss.x);
										break; //Don't look for more empty Collisions
									}
								}
							}
						}
					}
					else
					{
						if(invincible==FALSE)
						{
							if(bulletShipIntersect(bullets[a],imp))
							{
								bullets[a].disp = FALSE;

								//EXPLOSION for user ship
								for(int m=0;m<MAX_COLLISIONS;m++)
								{
									if(collisions[m].stage == 0)
									{
										collisions[m].xo = (imp.x + imp.x + imp.width)/2;
										collisions[m].yo = (imp.y + imp.y + imp.height)/2;
										collisions[m].stage = 1;
										collisions[m].maxstage = 3;
										break;
									}
								}
								//Reducing HP and Lives
								hp--;
								if(hp==0)
								{
									hp = HPPERLIFE;
									lives--;
									invincible = TRUE;
								}
								if(lives==0)
								{
									return STATUS_OVER;
								}
							}
						}
					}
				}
			}

			//Stage Change
			if(stagescore>=stage*STAGE_ENEMS && BossMode == FALSE)
			{
				stagescore = 0;
				stage++;
				stagechanging = TRUE;
				stagechangingtext = TRUE;
				if(stage == MAXSTAGES+1)
				{
					BossMode = TRUE;
				}
			}

			if(count>=(MAXTIME/BASETIME)*2)
			count = 1;
			else
			count++;
		}
		ch=getch();
		switch(ch)
		{
			//UP
		case 72:
			if(imp.y>=MINHEIGHT + 1)
			imp.y--;
			break;
			//DOWN
		case 80:
			if(imp.y<=MAXHEIGHT-(imp.height))
			imp.y++;
			break;
			//SPACE
		case 32:
			spawnbullet(bullets,n,imp,USER);
			break;
		}
	}while(ch!='e');
	return STATUS_BACK;
}
void draw_title()
{
	setcolor(WHITE);
	settextstyle(DEFAULT_FONT,0,10);
	outtextxy(80,70,"G");
	settextstyle(DEFAULT_FONT,0,9);
	outtextxy(155,70,"A");
	settextstyle(DEFAULT_FONT,0,8);
	outtextxy(220,70,"L");
	settextstyle(DEFAULT_FONT,0,7);
	outtextxy(280,70,"A");
	settextstyle(DEFAULT_FONT,0,6);
	outtextxy(335,70,"C");
	settextstyle(DEFAULT_FONT,0,5);
	outtextxy(380,70,"T");
	settextstyle(DEFAULT_FONT,0,4);
	outtextxy(410,70,"I");
	settextstyle(DEFAULT_FONT,0,4);
	outtextxy(435,70,"C");
	setcolor(WHITE);
	settextstyle(DEFAULT_FONT,0,10);
	outtextxy(290,120,"W");
	settextstyle(DEFAULT_FONT,0,9);
	outtextxy(365,120,"A");
	settextstyle(DEFAULT_FONT,0,8);
	outtextxy(430,120,"R");
	settextstyle(DEFAULT_FONT,0,7);
	outtextxy(490,120,"S");
	settextstyle(DEFAULT_FONT,0,10);
	outtextxy(520,120,"!");
	setcolor(YELLOW);
}
void gameloading()
{
	clrscr();
	int i,gd=DETECT ,gm;
	initgraph(&gd,&gm,GRAPH_ADD);
	settextstyle(SANS_SERIF_FONT,0,4);

	setcolor(CYAN);
	outtextxy(140,158,"Prepare for battle!");
	setcolor(9);
	rectangle(140,205,440,225);
	for(i=0;i<101;i++)
	{
		rectangle(140,205,140+3*i,225);
		gotoxy(57,14);
		cout<<i<<"%";
		delay(50);

		if(i==1)
		{
			gotoxy(20,16);
			cout<<"Initiating primary engines...";
		}
		if(i==26)
		{
			setcolor(BLACK);
			rectangle(140,230,440,260);
			setfillstyle(SOLID_FILL,BLACK);
			gotoxy(20,16);
			cout<<"Engaging Hyper drive.......";
			setcolor(9);
		}
		if(i==51)
		{
			setcolor(BLACK);
			rectangle(140,240,440,260);
			setfillstyle(SOLID_FILL,BLACK);
			gotoxy(20,16);
			cout<<"Preparing defense shields....";
			setcolor(9);
		}
		if(i==76)
		{
			setcolor(BLACK);
			rectangle(140,240,440,260);
			setfillstyle(SOLID_FILL,BLACK);
			gotoxy(20,16);
			cout<<"Crashing into asteroids....";
			setcolor(9);
		}
	}
	gotoxy(45,25);
	cout<<"Press any key to continue.";
	getch();
	setbkcolor(DARKGRAY);
	cleardevice();
}
void pressenter(int x)
{
	switch(x)
	{
	case 1://GAME
		gameloading();
		closegraph();
		int score;
		int x = game(score);
		switch(x)
		{
		case STATUS_WON:
			gamewon(score);
			break;
		case STATUS_OVER:
			gameover(score);
			break;
		}
		int gd= DETECT,gm;
		initgraph(&gd,&gm,GRAPH_ADD);
		setbkcolor(DARKGRAY);
		setcolor(WHITE);
		break;

	case 2://Credits
		cleardevice();
		int t;
		setbkcolor(0);
		setcolor(WHITE);
		for(x=500;x>-500;x-=5)
		{ outtextxy(270,x,"CREDITS");
			outtextxy(220,x+100, "DESIGNING  S & M");
			outtextxy(220,x+140, "DEBUGGING S & M");
			outtextxy(200,x+200, "SPECIAL THANKS TO");
			outtextxy(170,x+240,"TANMAY - GAME TESTER");
			outtextxy(140,x+280,"MRS NAGPAL - IMP. ADVICE");
			outtextxy(60,x+320,"SCS COMPUTER LAB - HIGH TECH COMPUTING");
			outtextxy(140,x+380, "Registered trademark of S&M");
			outtextxy(220,x+420, "All rights reserved");
			if(kbhit())
			{
				getch();
				break;
			}
			delay(60);
			cleardevice();
		}
		setbkcolor(DARKGRAY);
		setcolor(RED);
		cleardevice();
		break;

	case 3://Exit
		cleardevice();
		outtextxy(20,20,"Exiting....");
		delay(1000);
		cleardevice();
		closegraph();
		exit(0);
		break;
	}
}
void main()
{
	int i,j,k,x,y,gd= DETECT,gm,left,right,top;
	initgraph(&gd,&gm,GRAPH_ADD);
	setbkcolor(DARKGRAY);

	j=1;

	do
	{

		int xa = getmaxx();
		int ya = getmaxy();
		int dist = ya*0.04;
		draw_title();
		setcolor(WHITE);
		setlinestyle(DOTTED_LINE,10000000000,3);;
		rectangle(dist,dist,xa-dist,ya-dist);
		setcolor(CYAN);
		settextstyle(SANS_SERIF_FONT,0,4);
		outtextxy(225,250,"START GAME");
		outtextxy(260,290,"CREDITS");
		outtextxy(290,330,"QUIT");
		settextstyle(SANS_SERIF_FONT,0,3);
		setcolor(YELLOW);
		outtextxy(190,380,"Use arrow keys to move");
		outtextxy(210,405,"Press enter to select");
		setcolor(CYAN);

		setcolor(WHITE);
		randomize();
		int NO_USER_PATTERN = 1;
		setlinestyle(DASHED_LINE,NO_USER_PATTERN,NORM_WIDTH);
		left = xa*0.3;
		right = xa - left;
		top = 250 + 40*(j-1);
		rectangle(left,top,right,top+40);

		k=getch();
		switch(k)
		{
		case 72:
			j--;
			if(j==0)
			j=3;
			break;
		case 80:
			j++;
			if(j==4)
			j=1;
			break;
		case 13:
			pressenter(j);
			break;
		}

		setcolor(DARKGRAY);
		setlinestyle(SOLID_LINE,NO_USER_PATTERN,NORM_WIDTH);
		for(int m=0;m<3;m++)
			rectangle(left,250+40*m,right,250+40+40*m);
	}while(1);
}
