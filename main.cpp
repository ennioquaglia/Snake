//Snake
//Author: Ennio Quaglia


#include <iostream>
#include <conio.h>
#include <list>
#include <algorithm>
#include <windows.h>
#include <time.h>
#include <fstream>

using namespace std;

#define W 30
#define H 30
#define TIME 10
#define TELEPORT 1
#define HEAD_PROTECTION 1


/*

	186		||
	187		-T
	188		-j
	200		|-
	201		T-
	205 	=


*/

class Point
{
public:
	Point() {
		x = 0;
		y = 0;
	}
	Point(int X, int Y) {
		x = X;
		y = Y;
	}
	bool operator==(const Point& p) {
		if(p.x == x)if(p.y == y)return 1;
		return 0;
	}
	bool operator!=(const Point& p) {
		if(p.x == x)if(p.y == y)return 0;
		return 1;
	}
	Point& operator+(const Point& p) {
		Point np(x + p.x, y + p.y);
		return np;
	}
	Point& operator+=(const Point& p) {
		x += p.x;
		y += p.y;
		return *this;
	}
	int x, y;

};



class Game
{
	struct Matrix {
		char data[W][H];
	};
	Matrix screen;
	list<Point> snake;
	Point head;
	Point direction,preDirection;
	bool gameover, teleport, head_protection;
	int size, score;
	int speed;
public:
	Game() {
		Matrix m = {0};
		screen = m;
		gameover = 0;
		direction = Point(1, 0);
		head.x = W / 2;
		head.y = H / 2;
		size = 25;
		score = 0;
		teleport = TELEPORT;
		head_protection = HEAD_PROTECTION;
	}
	void setSpeed(int Speed) {
		speed = Speed;
	}
	char input() {
		char cmd = 0;
		if(kbhit()) {
			cmd = getch();
		}
		if(cmd == 'p') {
			pause();
		}
		return cmd;
	}
	void pause() {
		system("cls");
		setCursorPos(W, H / 2);
		cout << " PAUSE";
		char in;
		do {
			in=getch();
			if(in=='d'){
				system("cls");
				debugRender();
			}
			if(in==' '){
				system("cls");
				render();
			}
		} while(in != 'p');
		system("cls");
		render();
	}
	void debugRender(){
		cout << "\n\n";
		for(char y = 0; y < H; y++) {
			if(y == 0) {
				cout << "  " << (char)197;
				for(char x = 0; x <  W; x++)
					cout << "-";
				cout << (char)197 << endl;
			}
			cout << "  |";
			for(char x = 0; x < W; x++) {
				if(isFree(Point(x,y)))cout << " ";
				else cout<<"o";
			}
			cout << "|\n";
			if(y == H - 1) {
				cout << "  " << (char)197;
				for(char x = 0; x <  W; x++)
					cout << "-";
				cout << (char)197 << endl;
			}
		}
	}
	Point getDirection() {
		char cmd = input();
		if(!cmd)return direction;

		switch(cmd) {
		case 'w':
			if(head_protection)
				if(direction != Point(0, 1))
					direction = Point(0, -1);
			break;
		case 'a':
			if(head_protection)
				if(direction != Point(1, 0))
					direction = Point(-1, 0);
			break;
		case 's':
			if(head_protection)
				if(direction != Point(0, -1))
					direction = Point(0, 1);
			break;
		case 'd':
			if(head_protection)
				if(direction != Point(-1, 0))
					direction = Point(1, 0);
			break;
		}
		
		return direction;
	}
	bool isOut(Point p) {
		if(p.x < 0 || p.x >= W)return 1;
		if(p.y < 0 || p.y >= H)return 1;
		return 0;
	}
	bool isFree(Point p) {
		if(!teleport)
			if(isOut(p))return 0;

		char corpo[8]={186,201,188,200,187,205,'o',1};
		for(int i=0;i<8;i++)if(screen.data[p.x][p.y] == corpo[i])return 0; 
		return 1;
		//return screen.data[p.x][p.y] != 'o' && screen.data[p.x][p.y] != 1 && screen.data[p.x][p.y] != 187 && screen.data[p.x][p.y] != 188 && screen.data[p.x][p.y] != 200 && screen.data[p.x][p.y] != 201&& screen.data[p.x][p.y] != 205 && screen.data[p.x][p.y] != 186;//screen.data[p.x][p.y]==0 || screen.data[p.x][p.y]==' ' || screen.data[p.x][p.y]==3;//screen.data[p.x][p.y] != 'o' && screen.data[p.x][p.y] != 1;
		//return !(std::find(snake.begin(), snake.end(), p) != snake.end());

	}
	void updateSnake(Point p) {
		char c='o';
		if(direction==preDirection){
			if(direction.y)c=186;
			else c=205;
		}else{
			if(direction.y)
				if(direction.y>0)
					if(preDirection.x>0)c=187;
					else c=201;
				else
					if(preDirection.x>0)c=188;
					else c=200;
			else
				if(direction.x>0)
					if(preDirection.y>0)c=200;
					else c=201;
				else
					if(preDirection.y>0)c=188;
					else c=187;
		}
		
		
		screen.data[head.x][head.y] = c;
		updateCharachter(head.x, head.y, c);
		snake.push_back(head);
		if(teleport) {
			if(isOut(head + p)) {
				Point np = head + p; //newPos
				if(np.x == -1)head.x = W - 1;
				else if(np.x == W)head.x = 0;
				if(np.y == -1)head.y = H - 1;
				else if(np.y == H)head.y = 0;
			} else head += p;
		} else head += p;

		screen.data[head.x][head.y] = 1;
		updateCharachter(head.x, head.y, 1);
		if(snake.size() >= size) {
			Point toEliminate = *(snake.begin());
			screen.data[toEliminate.x][toEliminate.y] = ' ';
			updateCharachter(toEliminate.x, toEliminate.y, ' ');
			snake.pop_front();
		}
		bool isFruit = (head.x == fruit.x && head.y == fruit.y); //screen.data[head.x][head.y] == 3;
		if(isFruit) {
			score++;
			size++;
			generateFruit();
		}
		
		updateCharachter(fruit.x, fruit.y, 3);

	}
	void setCursorPos(int x, int y) {
		COORD newPosition = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newPosition);
	}
	void cleanScreen() {
		setCursorPos(0, 0);
	}
	void updateCharachter(int x, int y, char value) {
		setCursorPos(x + 3, y + 3);
		cout << (char)value;
		//setCursorPos(0,0);
	}
	void render() {
		cout << "\n\n";
		for(char y = 0; y < H; y++) {
			if(y == 0) {
				cout << "  " << (char)197;
				for(char x = 0; x <  W; x++)
					cout << "-";
				cout << (char)197 << endl;
			}
			cout << "  |";
			for(char x = 0; x < W; x++) {
				cout << screen.data[x][y] << "";
			}
			cout << "|\n";
			if(y == H - 1) {
				cout << "  " << (char)197;
				for(char x = 0; x <  W; x++)
					cout << "-";
				cout << (char)197 << endl;
			}
		}
	}
	Point fruit;
	void generateFruit() {
		srand(time(NULL));

		do {
			fruit.x = rand() % W;
			fruit.y = rand() % H;
		} while(!isFree(fruit));
		screen.data[fruit.x][fruit.y] = 3;
		updateCharachter(fruit.x, fruit.y, 3);
		//setCursorPos(0, 1);
		//cout << fruit.x << " - " << fruit.y;
	}

	void exec() {

		int i = 0;

		generateFruit();
		cleanScreen();
		render();
		preDirection=direction;
		while(!gameover) {
			Point p = getDirection();

			if(isFree(head + p)) {
				updateSnake(p);
			} else gameover = 1;


			//render();

			cleanScreen();
			setCursorPos(W/2. - 6, 0);
			cout << "   SCORE : " << snake.size();
			float ratio=1;	
			//if(direction.x)ratio=8/12.;
			Sleep(speed*ratio);
			i++;
			preDirection=direction;
		}
		system("cls");
		setCursorPos(W/2. - 4, H / 2);
		cout << " GAME OVER";
		setCursorPos(W/2. - 4, H / 2 + 2);
		cout << "SCORE : " << snake.size();
		setCursorPos(W/2. - 6, H /2 + 4);
		getch();
		

	}


};


static void ResizeConsole(SHORT xSize, SHORT ySize)
{
	if(xSize<80)xSize=80;
	//if(ySize<40)ySize=40;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi; // Hold Current Console Buffer Info
	BOOL bSuccess;
	SMALL_RECT srWindowRect;         // Hold the New Console Size
	COORD coordScreen;

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);

	// Get the Largest Size we can size the Console Window to
	coordScreen = GetLargestConsoleWindowSize(hConsole);

	// Define the New Console Window Size and Scroll Position
	srWindowRect.Right  = (SHORT)(min(xSize, coordScreen.X) - 1);
	srWindowRect.Bottom = (SHORT)(min(ySize, coordScreen.Y) - 1);
	srWindowRect.Left   = srWindowRect.Top = (SHORT)0;

	// Define the New Console Buffer Size
	coordScreen.X = xSize;
	coordScreen.Y = ySize;

	
	
	bSuccess = SetConsoleScreenBufferSize(hConsole, coordScreen);
		bSuccess = SetConsoleWindowInfo(hConsole, TRUE, &srWindowRect);
	// If the Current Buffer is Larger than what we want, Resize the
	// Console Window First, then the Buffer
/*	if((DWORD)csbi.dwSize.X * csbi.dwSize.Y > (DWORD) xSize * ySize) {
		bSuccess = SetConsoleWindowInfo(hConsole, TRUE, &srWindowRect);
		bSuccess = SetConsoleScreenBufferSize(hConsole, coordScreen);
	}

	// If the Current Buffer is Smaller than what we want, Resize the
	// Buffer First, then the Console Window
	if((DWORD)csbi.dwSize.X * csbi.dwSize.Y < (DWORD) xSize * ySize) {
		bSuccess = SetConsoleScreenBufferSize(hConsole, coordScreen);
		bSuccess = SetConsoleWindowInfo(hConsole, TRUE, &srWindowRect);
	}*/
	SetConsoleOutputCP(CP_UTF8);
	// If the Current Buffer *is* the Size we want, Don't do anything!
	return;
}

int main()
{

	do{
		system("cls");
		Game game;
	
		//    SetWindow(W,H);
		ResizeConsole(80,30);
	
		cout << "\n\n";
		ifstream fin("asciiArt.txt");
		while(!fin.eof())cout << (char)fin.get();
	
		
		int speed;
		cout << "\n\n";
		do {
			cout << "\nSelect speed(1%10): ";
			cin >> speed;
		} while(speed < 1 || speed > 10);
	
		speed = 500 / (2 * speed);
		game.setSpeed(speed);
	
	
		ResizeConsole(W + 6, H + 6);
		system("cls");
		game.exec();
		
		cout<<"countinue(y/n)?  ";
	}while(getch()=='y');
	
	return 0;
}
