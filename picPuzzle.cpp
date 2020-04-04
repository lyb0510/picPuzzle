#include <Bangtal.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>

#pragma comment(lib,"Bangtal.lib")

SceneID scene1;
ObjectID startButton, restartButton, endButton, base;
ObjectID p[16];

int X[4] = {300, 450, 600, 750};
int Y[4] = {470, 320, 170, 20};
char filename[20];

int p_location[16];
int blank = 15;
int err = 0;

char timerBuffer[50];

void secToHHMMSS(int secs, char* s, size_t size) {
	int hour, min, sec;

	sec = secs % 60;
	min = secs / 60 % 60;
	hour = secs / 3600;

	sprintf_s(s, size, "경과 시간은 [%02d:%02d:%02d] 입니다.", hour, min, sec);
}

void stopwatch(int onOff) {
	static int oldTime;
	if (onOff == 1) {
		oldTime = (int)time(NULL);
	}
	if (onOff == 0) {
		secToHHMMSS( (int)time(NULL) - oldTime, timerBuffer, sizeof(timerBuffer)	);
	}

}

void suffle() {
	srand(time(NULL));
	err = 0;
	for (int i = 0; i < 30; i++) {
		int randNum1 = rand() % 14;
		int randNum2 = rand() % 14;
		int temp;
		temp = p_location[randNum1];
		p_location[randNum1] = p_location[randNum2];
		p_location[randNum2] = temp;
	}
	for (int i = 0; i < 14; i++) {
		for (int j = i + 1; j < 15; j++) {
			if (p_location[i] > p_location[j]) {
				err++;
			}
		}
	}
	if (err % 2 == 1) {
		suffle();
	}
	return;
}

void startgame() {
	for (int i = 0; i < 16; i++) {
		p_location[i] = i + 1;
	}
	blank = 15;
	suffle();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			sprintf_s(filename, "Images\\%d.png", p_location[4 * i + j]);
			p[4 * i + j] = createObject("p", filename);
			locateObject(p[4 * i + j], scene1, X[j], Y[i]);
			showObject(p[4 * i + j]);
		}
	}
	hideObject(p[15]);
	hideObject(startButton);
	showObject(base);
	stopwatch(1);
	return;
}

void clear() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			hideObject(p[4*i+j]);
		}
	}
	hideObject(restartButton);
	hideObject(base);
	showObject(startButton);
}

void fin() {
	err = 0;
	for (int i = 0; i < 16; i++) {
		if (p_location[i] != i + 1) {
			err++;
		}
	}
	if (err == 0) {
		showObject(p[15]);
		stopwatch(0);
		showMessage(timerBuffer);
		clear();
	}
	return;
}

void click_p(int i) {
	int temp;
	int x, y;
	x = i % 4;
	y = i / 4;
	if ((blank - i == 4) || (blank - i == -4) || (((blank - i == 1) && (i % 4 != 3))) || ((blank - i == -1) && (i % 4 != 0))) {
		temp = p_location[blank];
		p_location[blank] = p_location[i];
		p_location[i] = temp;


		sprintf_s(filename, "Images\\%d.png", p_location[blank]);
		setObjectImage(p[blank], filename);
		showObject(p[blank]);

		sprintf_s(filename, "Images\\%d.png", p_location[i]);
		setObjectImage(p[i], filename);
		hideObject(p[i]);	
		blank = i;
	}
	return;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == startButton) {
		hideObject(startButton);
		showObject(base);
		showObject(restartButton);
		startgame();
	}
	else if (object == restartButton) {
		stopwatch(0);
		clear();
	}
	else if (object == endButton) {
		endGame();
	}
	for (int i = 0; i < 16; i++) {
		if (object == p[i]) {
			click_p(i);
			fin();
		}
	}
}

int main(){
	setMouseCallback(mouseCallback);

	scene1 = createScene("우주", "Images\\원본.png");
	base = createObject("우주", "Images\\배경.png");
	locateObject(base, scene1, 0, 0);

	startButton = createObject("시작버튼", "Images\\start.png");
	locateObject(startButton, scene1, 590, 460);
	scaleObject(startButton, 1.4f);
	showObject(startButton);

	endButton = createObject("종료버튼", "Images\\end.png");
	locateObject(endButton, scene1, 150, 50);
	scaleObject(endButton, 1.4f);
	showObject(endButton);

	restartButton = createObject("재시작버튼", "Images\\restart.png");
	locateObject(restartButton, scene1, 150,100);
	scaleObject(restartButton, 1.4f);

	startGame(scene1);
}