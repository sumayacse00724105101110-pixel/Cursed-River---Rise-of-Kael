#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<windows.h>
#include<mmsystem.h>
using namespace std;
#include "iGraphics.h"

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::Idraw Here::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
int page = 0;
int pos_x = 0;
int pos_y = 0;
int speed = 10;
int score = 0;
int highScore = 0;
bool musicOn = true;
int loading;
int riverImg[2], badha[2];
int currentRiver = 0;
bool showText = true;
int boat, riverAutoMoveTimer;
int back_button, back_button1;
int main_bg, button_start, Credits, Options, Exit, loadimage;
double loading_bar_X = 0;
double start_button_X = 830, start_button_Y = 370, start_button_Width = 250, start_button_Height = 72;
double Options_X = 830, Options_Y = 278, Options_Width = 250, Options_Height = 72;
double Credits_X = 830, Credits_Y = 186, Credits_Width = 250, Credits_Height = 72;
double Exit_X = 830, Exit_Y = 94, Exit_Width = 250, Exit_Height = 72;
double back_button_X = 20, back_button_Y = 580, back_button_Width = 100, back_button_Height = 100;
double bgX1 = 0, bgX2 = 1200, badhaX[2] = { 1400, 1400 }, badhaY[2];
int boatX = 150;
int boatY = 195;
int badhaSwitcher = -1;
int riverSpeed = 3;
bool gameOverHandled = false;
int coinScore = 0;
bool isPaused = false;

int coinImg;
const int maxCoins = 10;
int coinX[maxCoins], coinY[maxCoins];
bool coinVisible[maxCoins];
int totalCoins = 0;

bool powerActive = false;       // Is the power-up currently active
int powerDuration = 10000;       // Duration of power in milliseconds (10 seconds)
int coinsForPower = 25;        // Minimum coins required to activate
int powerTimer = 0;             // Timer ID for power-up duration

int creditImg;
int back_button2;
int optionImg;


//double back_button2_X = 1050, back_button2_Y = 600;   // Right side
//double back_button2_Width = 100, back_button2_Height = 100;




void deactivatePower() {
	powerActive = false;
	iPauseTimer(powerTimer); // stop the power timer
}




void loadHighScore() {
	ifstream inFile("highscore.txt");
	if (inFile.is_open()) {
		inFile >> highScore;
		inFile.close();
	}
	else {
		highScore = 0;
	}
}

void saveHighScore() {
	if (score > highScore) {
		highScore = score;
		ofstream outFile("highscore.txt");
		if (outFile.is_open()) {
			outFile << highScore;
			outFile.close();
		}
	}
}

void resetGame() {
	score = 0;
	coinScore = 0;
	boatX = 150;
	boatY = 195;
	badhaX[0] = 1400;
	badhaX[1] = 1400;
	badhaSwitcher = -1;
	showText = true;           // Tap to Play text show
	isPaused = false;
	totalCoins = 0;
	for (int i = 0; i < maxCoins; i++)
		coinVisible[i] = false;

	iPauseTimer(riverAutoMoveTimer);  // river movement pause

	
}

void loadImages(){
	riverImg[0] = iLoadImage("notunbg1.jpeg");
	riverImg[1] = iLoadImage("notunbg2.jpeg");
	badha[0] = iLoadImage("badha1.png");
	badha[1] = iLoadImage("badha2.png");
	coinImg = iLoadImage("coin.png");
	



}

void changeRiver(){
	if (!showText){
		currentRiver = (currentRiver + 1) % 2;
	}
}

void increaseSpeed() {
	if (riverSpeed < 15) riverSpeed++;
}

void spawnCoins() {
	int groupSizes[4] = { 3, 5, 7, 10 };
	totalCoins = groupSizes[rand() % 4];
	int startX = 1400;
	int randY;
	do {
		randY = rand() % 300 + 100;
	} while (abs(randY - badhaY[badhaSwitcher]) < 120);
	for (int i = 0; i < totalCoins; i++) {
		coinX[i] = startX + i * 50;
		coinY[i] = randY;
		coinVisible[i] = true;
	}
}

void riverAutoMove()
{
	if (badhaSwitcher == -1) // kono badha active na thakle
	{
		badhaSwitcher = rand() % 2; // randomly 0 or 1 select
	}

	if (badhaX[badhaSwitcher] == 1400) // jokhon new badha start hobe
	{
		badhaY[badhaSwitcher] = rand() % 289; // vertical position randomly
	}

	// Background and current badha move
	bgX1 -= riverSpeed;
	bgX2 -= riverSpeed;
	badhaX[badhaSwitcher] -= riverSpeed;

	score += 1; // distance score

	// Background looping
	if (bgX1 <= -1200)
		bgX1 = bgX2 + 1200;
	if (bgX2 <= -1200)
		bgX2 = bgX1 + 1200;

	// Badha screen er left side e giye gele
	if (badhaX[badhaSwitcher] <= -200)
	{
		badhaX[badhaSwitcher] = 1400; // abar right side theke start
		badhaSwitcher = -1;           // next badha randomly select hobe
		score += 5;                   // safely pass bonus
	}




	// Coins movement (original tumar code e ase)
	for (int i = 0; i < totalCoins; i++) {
		if (coinVisible[i]) {
			coinX[i] -= riverSpeed;
			if (coinX[i] < -50)
				coinVisible[i] = false;
		}
	}
}

void iDraw()
{
	iClear();
	if (page == 0){
		if (loading_bar_X < 590){
			iShowImage(0, 0, 1200, 700, loadimage);
			iSetColor(255, 255, 255);
			iRectangle(300, 150, 600, 30);
			iFilledRectangle(305, 155, loading_bar_X, 20);
		}
		else {
			iPauseTimer(loading);
			iShowBMP(0, 0, "entergame1.bmp");
		}
	}
	else if (page == 1){
		iShowImage(0, 0, 1200, 700, main_bg);
		iShowImage(start_button_X, start_button_Y, start_button_Width, start_button_Height, button_start);
		iShowImage(Options_X, Options_Y, Options_Width, Options_Height, Options);
		iShowImage(Credits_X, Credits_Y, Credits_Width, Credits_Height, Credits);
		iShowImage(Exit_X, Exit_Y, Exit_Width, Exit_Height, Exit);
		iShowImage(back_button_X, back_button_Y, back_button_Width, back_button_Height, back_button);
	}
	else if (page == 2){
		iShowImage(bgX1, 0, 1200, 700, riverImg[1]);
		iShowImage(bgX2, 0, 1200, 700, riverImg[1]);

		if (showText){
			iSetColor(97, 54, 19);
			iFilledRectangle(520, 330, 200, 50);
			iSetColor(255, 255, 255);
			iRectangle(520, 330, 200, 50);
			iSetColor(255, 255, 255);
			iText(550, 350, "Tap to Play", GLUT_BITMAP_TIMES_ROMAN_24);
		}

	/*	switch (badhaSwitcher){
		case 0:
			iShowImage(badhaX[badhaSwitcher], badhaY[badhaSwitcher], 150, 83.333, badha[badhaSwitcher]);
			if (((boatY >= badhaY[badhaSwitcher]) && (boatY <= badhaY[badhaSwitcher] + 83.333) &&
				((badhaX[badhaSwitcher] <= boatX + 200) && (badhaX[badhaSwitcher] + 150 >= boatX)))) page = 5;
			if (((boatY + 100 >= badhaY[badhaSwitcher]) && (boatY + 100 <= badhaY[badhaSwitcher] + 83.333) &&
				((badhaX[badhaSwitcher] <= boatX + 200) && (badhaX[badhaSwitcher] + 150 >= boatX)))) page = 5;
			break;
		case 1:
			iShowImage(badhaX[badhaSwitcher], badhaY[badhaSwitcher], 100, 100, badha[badhaSwitcher]);
			if (((boatY >= badhaY[badhaSwitcher]) && (boatY <= badhaY[badhaSwitcher] + 100) &&
				((badhaX[badhaSwitcher] <= boatX + 200) && (badhaX[badhaSwitcher] + 100 >= boatX)))) page = 5;
			if (((boatY + 100 >= badhaY[badhaSwitcher]) && (boatY + 100 <= badhaY[badhaSwitcher] + 100) &&
				((badhaX[badhaSwitcher] <= boatX + 200) && (badhaX[badhaSwitcher] + 100 >= boatX)))) page = 5;
			break;
		}*/


		switch (badhaSwitcher){
		case 0:
			iShowImage(badhaX[badhaSwitcher], badhaY[badhaSwitcher], 150, 83.333, badha[badhaSwitcher]);
			if (!powerActive && (
				((boatY >= badhaY[badhaSwitcher]) && (boatY <= badhaY[badhaSwitcher] + 83.333) &&
				((badhaX[badhaSwitcher] <= boatX + 200) && (badhaX[badhaSwitcher] + 150 >= boatX))) ||
				((boatY + 100 >= badhaY[badhaSwitcher]) && (boatY + 100 <= badhaY[badhaSwitcher] + 83.333) &&
				((badhaX[badhaSwitcher] <= boatX + 200) && (badhaX[badhaSwitcher] + 150 >= boatX)))
				)) page = 5; // Game over
			break;

		case 1:
			iShowImage(badhaX[badhaSwitcher], badhaY[badhaSwitcher], 100, 100, badha[badhaSwitcher]);
			if (!powerActive && (
				((boatY >= badhaY[badhaSwitcher]) && (boatY <= badhaY[badhaSwitcher] + 100) &&
				((badhaX[badhaSwitcher] <= boatX + 200) && (badhaX[badhaSwitcher] + 100 >= boatX))) ||
				((boatY + 100 >= badhaY[badhaSwitcher]) && (boatY + 100 <= badhaY[badhaSwitcher] + 100) &&
				((badhaX[badhaSwitcher] <= boatX + 200) && (badhaX[badhaSwitcher] + 100 >= boatX)))
				)) page = 5; // Game over
			break;
		}


		// Score rectangle
		iSetColor(193, 209, 31);
		iFilledRectangle(1000, 630, 150, 50);
		iSetColor(0, 0, 0);
		char scoreText[50]; sprintf_s(scoreText, "Score: %d", score);
		iText(1020, 650, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);

		// Coin score
		iSetColor(255, 215, 0);
		iFilledRectangle(1000, 570, 150, 50);
		iSetColor(0, 0, 0);
		char coinText[50]; sprintf_s(coinText, "Coins: %d", coinScore);
		iText(1020, 590, coinText, GLUT_BITMAP_TIMES_ROMAN_24);


		//Coins draw & collision check
		for (int i = 0; i<totalCoins; i++){
			if (coinVisible[i]){
				iShowImage(coinX[i], coinY[i], 40, 40, coinImg);
				if (boatX < coinX[i] + 40 && boatX + 150 > coinX[i] && boatY < coinY[i] + 40 && boatY + 150 > coinY[i]){
					coinVisible[i] = false;
					coinScore++;


					// Check for power activation
					if (!powerActive && coinScore >= coinsForPower) {
						powerActive = true;
						powerTimer = iSetTimer(powerDuration, deactivatePower); // Set power duration
						coinScore -= coinsForPower; // inside the power activation block in Step 3

					}
				}
			}
		}

		
		


		iShowImage(boatX, boatY, 220, 120, boat);
		iShowImage(10, 640, 50, 50, back_button1);
		


		if (powerActive){
			iSetColor(0, 255, 0);
			iFilledRectangle(500, 650, 200, 30);
			iSetColor(0, 0, 0);
			iText(520, 655, "POWER ACTIVE!", GLUT_BITMAP_HELVETICA_18);
		}


		


		if (isPaused){
			iSetColor(0, 0, 0); iFilledRectangle(350, 200, 500, 300);
			iSetColor(255, 255, 255); iRectangle(350, 200, 500, 300);
			iSetColor(0, 200, 0); iFilledRectangle(400, 350, 180, 60); iSetColor(255, 255, 255);
			iText(450, 370, "RESUME", GLUT_BITMAP_TIMES_ROMAN_24);
			iSetColor(200, 0, 0); iFilledRectangle(650, 350, 180, 60); iSetColor(255, 255, 255);
			iText(710, 370, "EXIT", GLUT_BITMAP_TIMES_ROMAN_24);
		}
	}

	
	else if (page == 4){
		iShowImage(0, 0, 1200, 700, creditImg);
		//iShowImage(10, 640, 50, 50, back_button1); // optional back button
		iShowImage(10, 640, 50, 50, back_button2);
		//iShowImage(back_button2_X, back_button2_Y, back_button2_Width, back_button2_Height, back_button2);
	}

	else if (page == 3){
		iShowImage(0, 0, 1200, 700, optionImg); // full screen option image
		iShowImage(10, 640, 50, 50, back_button2); // back button upper right
	}

	

	else if (page == 5){
		iPauseTimer(riverAutoMoveTimer);
		

		if (!gameOverHandled){ saveHighScore(); gameOverHandled = true; }
		iSetColor(255, 0, 0); iText(550, 350, "GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
		char showScore[50]; sprintf_s(showScore, "Your Score: %d", score);
		iText(520, 300, showScore, GLUT_BITMAP_TIMES_ROMAN_24);
		char showHigh[50]; sprintf_s(showHigh, "High Score: %d", highScore);
		iText(520, 260, showHigh, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

void iMouseMove(int mx, int my){}
void iPassiveMouseMove(int mx, int my){
	// Optimized hover logic

	//start button hover
	if (mx >= 830 && mx <= 1080 && my >= 370 && my <= 442){ start_button_X = 825; start_button_Y = 365; start_button_Width = 260; start_button_Height = 82; }
	else { start_button_X = 830; start_button_Y = 370; start_button_Width = 250; start_button_Height = 72; }

	// Options button hover
	if (mx >= 830 && mx <= 1080)
	{
		if (my >= 278 && my <= 350)
		{
			Options_X = 825;
			Options_Y = 273;
			Options_Width = 260;
			Options_Height = 82;
		}
		else
		{
			Options_X = 830;
			Options_Y = 278;
			Options_Width = 250;
			Options_Height = 72;
		}
	}

	// Credits button hover
	if (mx >= 830 && mx <= 1080)
	{
		if (my >= 186 && my <= 258)
		{
			Credits_X = 825;
			Credits_Y = 181;
			Credits_Width = 260;
			Credits_Height = 82;
		}
		else
		{
			Credits_X = 830;
			Credits_Y = 186;
			Credits_Width = 250;
			Credits_Height = 72;
		}
	}
	
	//exit button hover
	if (mx >= 830 && mx <= 1080 && my >= 85 && my <= 172){ Exit_X = 825; Exit_Y = 95; Exit_Width = 260; Exit_Height = 82; }
	else { Exit_X = 830; Exit_Y = 100; Exit_Width = 250; Exit_Height = 72; }

	//Credits_X = 830; Credits_Y = 186; Credits_Width = 250; Credits_Height = 72;
}

void iMouse(int button, int state, int mx, int my){
	if (isPaused && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		if (mx >= 400 && mx <= 580 && my >= 350 && my <= 410)
		{ isPaused = false; iResumeTimer(riverAutoMoveTimer); }
		

		else if (mx >= 650 && mx <= 830 && my >= 350 && my <= 410){ isPaused = false; page = 1;  }

	}
    
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		if (page == 1){
			if (mx > 830 && mx<1145 && my>355 && my<420){
				page = 2;
				resetGame();
			}
			else if (mx>830 && mx<1145 && my>265 && my<330) page = 3;
			else if (mx>830 && mx<1145 && my>175 && my<240) page = 4;
			else if (mx>830 && mx<1145 && my>85 && my<150) exit(0);
			else if (mx>20 && mx<100 && my>580 && my<680) page = 0;
		}
		else if (page == 2){ if (mx>10 && mx<125 && my>640 && my<695) page = 1; }
	}


	else if (page == 3) {
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			if (mx > 10 && mx < 125 && my > 640 && my < 695) {
				page = 1; // back to menu
			}
		}
	}

	else if (page == 4){
		if (mx > 10 && mx < 125 && my > 640 && my < 695) {
			page = 1;
		}
	}




	

}

void iKeyboard(unsigned char key){
	if (key == 'r' || key == 'R'){ iResumeTimer(riverAutoMoveTimer); showText = false; score = 0; gameOverHandled = false; }
	
	else if (key == 'b' || key == 'B'){ page = 1; }
	else if (key == 'p' || key == 'P'){ if (!isPaused && page == 2){ isPaused = true; iPauseTimer(riverAutoMoveTimer); } }
	

}

void iSpecialKeyboard(unsigned char  key){
	if (key == GLUT_KEY_HOME) page = 1;
	if (key == GLUT_KEY_INSERT) page = 0;
	if (key == GLUT_KEY_UP){ boatY = min(345, boatY + 15); }
	if (key == GLUT_KEY_DOWN){ boatY = max(0, boatY - 15); }
	if (key == GLUT_KEY_F1){
		if (musicOn){ musicOn = false; PlaySound(0, 0, 0); }
		else { musicOn = true; PlaySound("BgMusic.wav", NULL, SND_LOOP | SND_ASYNC); }
	}
}

void change(){ pos_x += speed; if (pos_x >= 1200) pos_x = 0; }
void loading_Animation(){ loading_bar_X += 3; }

int main()
{
	srand((unsigned)time(NULL));
	loadHighScore();

	iSetTimer(5, change);
	if (musicOn) PlaySound("BgMusic.wav", NULL, SND_LOOP | SND_ASYNC);

	iInitialize(1200, 700, "Cursed River");

	loading = iSetTimer(2, loading_Animation);
	riverAutoMoveTimer = iSetTimer(16, riverAutoMove);

	iPauseTimer(riverAutoMoveTimer);

	main_bg = iLoadImage("game_bg.jpg");
	loadimage = iLoadImage("loadimage.jpeg");
	button_start = iLoadImage("start_button.jpg");
	Options = iLoadImage("Options.jpeg");
	Credits = iLoadImage("Credits.jpg");
	Exit = iLoadImage("Exit.jpeg");
	boat = iLoadImage("boat.png");
	coinImg = iLoadImage("coin.png");
	back_button = iLoadImage("back_button.jpeg");
	back_button1 = iLoadImage("back_button1.png");
	creditImg = iLoadImage("credit.jpg");
	back_button2 = iLoadImage("back_button2.jpg");
	optionImg = iLoadImage("option_page.jpg"); // tumi option er picture jpg dile name boshao


	loadImages();


	iSetTimer(200, changeRiver);
	iSetTimer(5000, increaseSpeed);
	iSetTimer(7000, spawnCoins);

	


	iStart();
	return 0;
}
