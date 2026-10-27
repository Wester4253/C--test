#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>
#include <functional>
using namespace std;

// functions to clear screen and restore screen
void enaltscreen() {
	std::cout << "\033[?1049h";
	std::cout.flush();
}
void exaltscreen() {
	std::cout << "\033[?1049l";
	std::cout.flush();
}

// define stuff for moving stuff
static int x = 1;
static int y = 4;
static bool initialized = false;
string playerchar = "O";
void setcurpos(int targetX, int targetY) {
	std::cout << "\033[" << targetY << ";" << targetX << "H" << std::flush;
}
void initsprite() {
	if (!initialized) {
		setcurpos(x, y);
		std::cout << playerchar << std::flush;
		initialized = true;
	}
}
void showcurpos() {
	setcurpos(1, 2);
	std::cout << "\033[2K";
	std::cout << "X: " << x << ",Y: " << y << std::flush;
	setcurpos(x, y);
}

void topmsg() {
	setcurpos(1, 1);
	std::cout << "\033[2K";
	std::cout << "Press WASD to move the cursor. E will exit." << std::flush;
	setcurpos(x, y);
}

// functions to move char
void mvup() {
//	cout << "mvup called\n" << flush;
	initsprite();
	setcurpos(x, y);
	std::cout << " " << std::flush;
	if (y > 1) {
		y--;
	}
	setcurpos(x, y);
	std::cout << playerchar << std::flush;
	showcurpos();
	topmsg();
}
void mvlft() {
//	cout << "mvlft called\n" << flush;
	initsprite();
	setcurpos(x, y);
	std::cout << " " << std::flush;
	if (x > 1) {
		x--;
	}
	setcurpos(x, y);
	std::cout << playerchar << std::flush;
	showcurpos();
	topmsg();
}
void mvdwn() {
//	cout << "mvdwn called\n" << flush;
	initsprite();
	setcurpos(x, y);
	std::cout << " " << std::flush;
	if (y < 80){
		y++;
	}
	setcurpos(x, y);
	std::cout << playerchar << std::flush;
	showcurpos();
	topmsg();
}
void mvrht() {
//	cout << "mvrht called\n" << flush;
	initsprite();
	setcurpos(x, y);
	std::cout << " " << std::flush;
	x++;
	setcurpos(x, y);
	std::cout << playerchar << std::flush;
	showcurpos();
	topmsg();
}

struct termios orig_termios;

void init_term() {
	struct termios raw;
	tcgetattr(STDIN_FILENO, &orig_termios);
	raw = orig_termios;
	raw.c_lflag &= ~(ICANON | ECHO);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}
void reset_term() {
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}
char getch_linux_patched() {
	char buf = 0;
	if (read(STDIN_FILENO, &buf, 1) < 0) {
		perror("read()");
	}
	return buf;
}

// OLD CODE - NOT USED
// char getch_linux() {
//	char buf = 0;
//	struct termios old = {0};
//
//	if (tcgetattr(0, &old) < 0) perror("tcgetattr()");
//
//	old.c_lflag &= ~ICANON;
//	old.c_lflag &= ~ECHO;
//	old.c_cc[VMIN] = 1;
//	old.c_cc[VTIME] = 0;
//
//	if(tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ~ICANON");
//
//	if(read(0, &buf, 1) < 0) perror("read()");
//
//	old.c_lflag |= ICANON;
//	old.c_lflag |= ECHO;
//	if(tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ICANON");
//
//	return buf;	
//}

void devshowinput() {
	std::function<char()> getch = getch_linux_patched;
	char ctlvar;
	ctlvar = getch();
	setcurpos(1, 3);
	std::cout << ctlvar << std::flush;
	setcurpos(x, y);
}

int main() {
	init_term();
	enaltscreen();
	topmsg();
	initsprite();
	function<char()> getch = getch_linux_patched;
	showcurpos();	
	char ctlvar;
//	devshowinput();
	
	while(true) {
		ctlvar = getch();
		if (ctlvar == 'w') {
			mvup();
		} else if (ctlvar == 'a') {
			mvlft();
		} else if (ctlvar == 's'){
			mvdwn();
		} else if (ctlvar == 'd'){
			mvrht();
		} else if (ctlvar == 'e'){
			reset_term();
			exaltscreen();
		} else if (ctlvar == 'q'){
			reset_term();
			exaltscreen();
		}
	}
	reset_term();
	return 0;
}
