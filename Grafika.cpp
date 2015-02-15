#include <unistd.h>
#include <fcntl.h>		/* for fcntl */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>		/* for mmap */
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <vector>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

// Variabel Global
long int screensize = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
int fbfd;					/* frame buffer file descriptor */
char* fbp;					/* pointer to framebuffer */
int location;					/* iterate to location */
int x, y;					/* x and y location */

class Color{
	public:
	int Red;
	int Green;
	int Blue;
	
	Color(){
		Red=255;
		Green=255;
		Blue=255;
	}
	
	Color(int red, int green, int blue){
		if(red<=255 && red>=0 && green<=255 && green>=0 && blue>=0&& blue<=255){
			
			Red=red;
			Green=green;
			Blue=blue;
		}
		else{
			Red=0;
			Green=0;
			Blue=0;
		}
	}
	
	void setRed(int r){
		if(r<=0&&r>=255)Red=r;
	}
	void setGreen(int g){
		if(g<=0&&g>=255)Green=g;
	}
	void setBlue(int b){
		if(b<=0&&b>=255)Blue=b;
	}
	bool isBlack(){
		return(Red==1&&Green==1&&Blue==1);
	}
};

class Pattern
{
public:
	virtual Color getColor(int x, int y) = 0;
};

class BirdPattern : public Pattern
{
private:
	Color pattern[8][12];

public:
	static const int WIDTH = 12;
	static const int HEIGHT = 8;
	BirdPattern()
	{
		int i, j;
		for (i=0; i<HEIGHT; i++)
		{
			for (j=0; j< WIDTH; j++)
			{
				pattern[i][j] = Color(222, 222, 222);
			}
		}
		
		pattern[2][2] = Color(24,7,108);

		pattern[3][2] = Color(24,7,108);
		pattern[3][3] = Color(24,7,108);
		pattern[3][6] = Color(24,7,108);
		pattern[3][7] = Color(24,7,108);

		pattern[4][3] = Color(24,7,108);
		pattern[4][4] = Color(24,7,108);
		pattern[4][5] = Color(24,7,108);
		pattern[4][6] = Color(24,7,108);
		pattern[4][7] = Color(0,0,0);
		pattern[4][8] = Color(24,7,108);

		pattern[5][3] = Color(24,7,108);
		pattern[5][4] = Color(24,7,108);
		pattern[5][5] = Color(24,7,108);
		pattern[5][6] = Color(24,7,108);
		pattern[5][7] = Color(24,7,108);
		pattern[5][8] = Color(24,7,108);
		pattern[5][9] = Color(250,237,0);

		pattern[6][1] = Color(24,7,108);
		pattern[6][2] = Color(24,7,108);
		pattern[6][3] = Color(24,7,108);
		pattern[6][4] = Color(24,7,108);
		pattern[6][5] = Color(24,7,108);

	}
	virtual Color getColor(int x, int y)
	{
		int xPattern = x % WIDTH;
		int yPattern = y % HEIGHT;

		return pattern[yPattern][xPattern];
	}
};

class Point{
public:
	int x;
	int y;

	Point(){
		x = 0;
		y = 0;
	}
	Point(int x1, int y1){
		x = x1;
		y = y1;
	}
};

void setPixelCol(int x, int y,Color C);
Color getColor(Point P);
void setPixel(int x, int y);
void drawLine(int x1, int y1, int x2, int y2);
void drawBullet(int x1, int y1, int x2, int y2);
void deleteBullet(int x1, int y1, int x2, int y2);
void deletePixel (int x, int y);

void setPixelCol(int x, int y,Color C){
    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
    *(fbp + location) = C.Blue; // Some blue 
    *(fbp + location + 1) = C.Green; // A little green 
    *(fbp + location + 2) = C.Red; // A lot of red
    *(fbp + location + 3) = 0; // No transparency 
}

Color getColor(Point P){
	int red,blue,green;
	location = (P.x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (P.y+vinfo.yoffset) * finfo.line_length;
    blue = *(fbp + location);// Some blue 
    green = *(fbp + location + 1); // A little green 
    red = *(fbp + location + 2) ; // A lot of red
	Color C(red,green,blue);
	return C; 
}

void setPixel(int x, int y){
    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
    *(fbp + location) = 100; // Some blue 
    *(fbp + location + 1) = 15+(x-100)/2; // A little green 
    *(fbp + location + 2) = 200-(y-100)/5; // A lot of red
    *(fbp + location + 3) = 0; // No transparency 
}

void deletePixel(int x, int y){
	location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
    *(fbp + location) = 255; // Some blue 
    *(fbp + location + 1) = 255; // A little green 
    *(fbp + location + 2) = 255; // A lot of red
    *(fbp + location + 3) = 0; // No transparency 
}

void drawBullet(int x0, int y0, int x1, int y1){
	int dx = abs (x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = - abs (y1-y0), sy = y0<y1 ? 1 : -1;
	int error = dx + dy, e2;
	int count=50;
	int n=0;

	for (;;){
		if(n<=0){
			n=count;
			count = count - 2;
		}
		else if(n<=20){
			n--;
		}
		else{
			n--;
			setPixel(x0,y0);
		}
		if (x0==x1 && y0==y1){
			sleep(0.5);
			deleteBullet(x0,y0,x1,y1);
			break;
		} 
		e2 = 2 * error;
		if (e2 >= dy) {error += dy; x0 += sx;}
		if (e2 <= dx) {error += dx; y0 += sy;}
	}
}

void deleteBullet(int x0, int y0, int x1, int y1){
	int dx = abs (x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = - abs (y1-y0), sy = y0<y1 ? 1 : -1;
	int error = dx + dy, e2;
	int count=50;
	int n=0;

	for (;;){
		if(n<=0){
			n=count;
			count = count - 2;
		}
		else if(n<=20){
			n--;
		}
		else{
			n--;
			deletePixel(x0,y0);
		}
		if (x0==x1 && y0==y1) break;
		e2 = 2 * error;
		if (e2 >= dy) {error += dy; x0 += sx;}
		if (e2 <= dx) {error += dx; y0 += sy;}
	}	
}

void drawLine(int x0, int y0, int x1, int y1){
	int dx = abs (x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = - abs (y1-y0), sy = y0<y1 ? 1 : -1;
	int error = dx + dy, e2;

	for (;;){
		setPixel(x0,y0);
		if (x0==x1 && y0==y1) break;
		e2 = 2 * error;
		if (e2 >= dy) {error += dy; x0 += sx;}
		if (e2 <= dx) {error += dx; y0 += sy;}
	}
}

class Plane{
public:	
	Point center;
	std::vector<Point> arrPoint;
	std::vector<Point> arrPointOriginal;
	BirdPattern birdPattern;

	Plane(Point x){
		center=x;
		arrPoint.push_back(Point(center.x-50,center.y));
		arrPoint.push_back(Point(center.x+50,center.y-30));
		arrPoint.push_back(Point(center.x+5,center.y));
		arrPoint.push_back(Point(center.x+50,center.y+30));
		arrPointOriginal=arrPoint;
	}
	
	void setColorFloodPlane(Point P){
		Color C = birdPattern.getColor(P.x, P.y);

		setPixelCol(P.x,P.y,C);
		if(getColor(Point(P.x+1,P.y)).isBlack()){
			setColorFloodPlane(Point(P.x+1,P.y));
		}
		if(getColor(Point(P.x,P.y+1)).isBlack()){
			setColorFloodPlane(Point(P.x,P.y+1));
		}
		if(getColor(Point(P.x-1,P.y)).isBlack()){
			setColorFloodPlane(Point(P.x-1,P.y));
		}
		if(getColor(Point(P.x,P.y-1)).isBlack()){
			setColorFloodPlane(Point(P.x,P.y-1));
		}
	}

	void Update(){
		arrPoint[0] = Point(center.x-50,center.y);
		arrPoint[1] = Point(center.x+50,center.y-30);
		arrPoint[2] = Point(center.x+5,center.y);
		arrPoint[3] = Point(center.x+50,center.y+30);
		arrPointOriginal=arrPoint;
	}

	void Draw(){
		Update();
		drawLine(arrPoint[0].x,arrPoint[0].y,arrPoint[1].x,arrPoint[1].y);
		drawLine(arrPoint[0].x,arrPoint[0].y,arrPoint[3].x,arrPoint[3].y);
		drawLine(arrPoint[2].x,arrPoint[2].y,arrPoint[1].x,arrPoint[1].y);
		drawLine(arrPoint[2].x,arrPoint[2].y,arrPoint[3].x,arrPoint[3].y);
		setColorFloodPlane(center);		
	}

	void Swap(){
		if(!(arrPointOriginal[0].x == arrPoint[0].x) && (arrPointOriginal[0].y==arrPoint[0].y)){
			arrPoint[0].x=center.x+50;
			arrPoint[1].x=center.x-50;
			arrPoint[2].x=center.x-5;
			arrPoint[3].x=center.x-50;
		}else{
			arrPoint=arrPointOriginal;
		}
	}

	void MoveDown(){
		center.y += 10;
		Update();
		Draw(); 
	}

	void MoveUp(){
		center.y -= 10;
		Update();
		Draw(); 
	}

	void MoveRight(){
		center.x += 10;
		Update();
		Draw();
	}

	void MoveLeft(){
		center.x -= 10;
		Update();
		Draw(); 
	}
};	
	
class Ship{
public:
	Point center;
	std::vector<Point> arrPoint;
	std::vector<Point> arrPointOriginal;
	
	Ship(Point x){
		center=x;
		arrPoint.push_back(Point(center.x-100,center.y));
		arrPoint.push_back(Point(center.x+100,center.y));
		arrPoint.push_back(Point(center.x-50,center.y+50));
		arrPoint.push_back(Point(center.x+50,center.y+50));
		arrPoint.push_back(Point(center.x,center.y-50));
		arrPoint.push_back(Point(center.x+30,center.y-30));
		arrPoint.push_back(Point(center.x,center.y-10));
		arrPoint.push_back(Point(center.x,center.y));
		arrPointOriginal=arrPoint;	
	}
	
	void Draw(){
		drawLine(arrPoint[0].x,arrPoint[0].y, arrPoint[1].x,arrPoint[1].y);
		drawLine(arrPoint[0].x,arrPoint[0].y, arrPoint[2].x,arrPoint[2].y);
		drawLine(arrPoint[1].x,arrPoint[1].y, arrPoint[3].x,arrPoint[3].y);
		drawLine(arrPoint[2].x,arrPoint[2].y, arrPoint[3].x,arrPoint[3].y);	
		drawLine(arrPoint[7].x,arrPoint[7].y, arrPoint[4].x,arrPoint[4].y);
		drawLine(arrPoint[4].x,arrPoint[4].y, arrPoint[5].x,arrPoint[5].y);
		drawLine(arrPoint[5].x,arrPoint[5].y, arrPoint[6].x,arrPoint[6].y);	
	}

	void Update(){
		arrPoint[0] = Point(center.x-100,center.y);
		arrPoint[1] = Point(center.x+100,center.y);
		arrPoint[2] = Point(center.x-50,center.y+50);
		arrPoint[3] = Point(center.x+50,center.y+50);
		arrPoint[4] = Point(center.x,center.y-50);
		arrPoint[5] = Point(center.x+30,center.y-30);
		arrPoint[6] = Point(center.x,center.y-10);
		arrPoint[7] = Point(center.x,center.y);
		arrPointOriginal=arrPoint;
	}

	void MoveDown(){
		center.y += 10;
		Update(); 
		Draw();
	}

	void MoveUp(){
		center.y -= 10;
		Update(); 
		Draw();
	}

	void MoveRight(){
		center.x += 10;
		Update();
		Draw();
	}

	void MoveLeft(){
		center.x -= 10;
		Update(); 
		Draw();
	}
};

class Bomb{
public:
	Point asalDuar;
	vector<Point> arrPoint;
	vector<Point> arrPoint2;
	
	Bomb(int x, int y){
		asalDuar.x = x;
		asalDuar.y = y;
		arrPoint.push_back(Point(x-30, y-26));
		arrPoint.push_back(Point(x-83, y-87));
		arrPoint.push_back(Point(x-16, y-51));
		arrPoint.push_back(Point(x+38, y-100));
		arrPoint.push_back(Point(x+24, y-57));
		arrPoint.push_back(Point(x+70, y-36));
		arrPoint.push_back(Point(x+44, y-27));
		arrPoint.push_back(Point(x+105, y+38));
		arrPoint.push_back(Point(x+29, y+20));
		arrPoint.push_back(Point(x-1, y+101));
		arrPoint.push_back(Point(x-9, y+45));
		arrPoint.push_back(Point(x-93, y+75));
		arrPoint.push_back(Point(x-42, y+27));
		arrPoint.push_back(Point(x-88, y+21));
		// for(int i = 0; i<arrPoint.size()-1; i++){
			arrPoint2 = arrPoint;

		// }
	}
	void drawBomb(){
		for(int i = 0; i < arrPoint.size()-1; i++){
			drawLine(arrPoint[i].x, arrPoint[i].y, arrPoint[i+1].x, arrPoint[i+1].y);
		}
		drawLine(arrPoint[arrPoint.size()-1].x, arrPoint[arrPoint.size()-1].y, arrPoint[0].x, arrPoint[0].y);
	}
	void reset(){
		arrPoint = arrPoint2;
	}
	void zoomIn(){
		for(int i = 0; i < arrPoint.size(); i++){
			arrPoint[i].x = ((arrPoint[i].x - asalDuar.x) * 8 / 5)+asalDuar.x;
			arrPoint[i].y = ((arrPoint[i].y - asalDuar.y)* 8 / 5)+asalDuar.y;
		}
	}
};



void scanLine();


static struct termios old, New;

//input without enter
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  New = old; /* make new settings same as old settings */
  New.c_lflag &= ~ICANON; /* disable buffered i/o */
  New.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &New); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

Plane plane(Point(200,100));
Ship ship(Point(60,400));
bool shoot=false;
bool endGame;
bool hit = false;
char temp;
bool moveleft=true;

void movePlaneMainThread(){
	std::chrono::milliseconds dura(100);
	while(!endGame)
	{
		if (!hit){
			printf("movePlaneMainThread: %d\n", plane.center.x);
			if(plane.center.x>=50 && moveleft){
				plane.MoveLeft();
			}
			if(plane.center.x==50){
				plane.Swap();
				plane.Draw();
				moveleft=false;
			}
			if(plane.center.x<=530&&!moveleft){
				plane.MoveRight();
			}
			if(plane.center.x==530){
				plane.Swap();
				plane.Draw();
				moveleft=true;
			}
			std::this_thread::sleep_for(dura);
		}
	}
}

void drawThread(){
	std::chrono::milliseconds dura(20);
	while(temp != 'q' && !endGame)
	{
		system("clear");
		plane.Draw();
		ship.Draw();
		
		if(shoot)
		{
			drawBullet(ship.center.x, ship.center.y,ship.center.x,100); // Gambar peluru
			if (ship.center.x < plane.center.x + 50 && ship.center.x > plane.center.x - 50){ // Got hit	
				Bomb bomb(plane.center.x,plane.center.y+100);
				for(int i=0;i<100;i++){
					system("clear");
					//drawBullet(ship.center.x, ship.center.y,plane.center.x,plane.center.y);
					if(i%2==0){
						bomb.drawBomb();
						bomb.reset();
					}else{
						bomb.zoomIn();
						bomb.drawBomb();
						bomb.reset();
					}
					ship.Draw();
				}
				hit = true;
				endGame= true;
				shoot = false;
			}
			else{
				deleteBullet(ship.center.x, ship.center.y,ship.center.x,100);
				hit = false;
				shoot = false;
			}
		}

		munmap(fbp, screensize);
		std::this_thread::sleep_for(dura);
	}
}




// Main program start
int main() {
	/* open the file for reading and writing */
	fbfd = open("/dev/fb0",O_RDWR);
	if (!fbfd) {
		printf("Error: cannot open framebuffer device.\n");
		exit(1);
	}
	printf ("The framebuffer device was opened successfully.\n");

	/* get the fixed screen information */
	if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		printf("Error reading fixed information.\n");
		exit(2);
	}

	/* get variable screen information */
	if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		printf("Error reading variable information.\n");
		exit(3);
	}

	/* map the device to memory */
	fbp = (char*)mmap(0, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if ((long long)fbp == -1) {
		printf ("Error: failed to map framebuffer device to memory.\n");
		exit(4);
	}
	printf ("Framebuffer device was mapped to memory successfully.\n");

	temp= 'a';
	endGame=false;
	std::thread planeMover(movePlaneMainThread);
	std::thread drawerThread(drawThread);
	for(;temp!='q' && !endGame;)
	{
		temp=getche();
		switch(temp){
			case 'a' : ship.MoveLeft(); break;
			case 'd' : ship.MoveRight(); break;
			case 'b' : shoot = true; break;
			case 'q' : endGame = true;
		}
	}
	
	planeMover.join();
	drawerThread.join();
	munmap(fbp, screensize);
	
	close(fbfd);
	system("clear");
	return 0;
}
// Main Program end
