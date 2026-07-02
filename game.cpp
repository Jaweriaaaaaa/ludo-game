#include<iostream>
#include<cstdlib>
#include<ctime>
#include<windows.h>
#include<fstream>
using namespace std;
void gotoxy(int x, int y);
void setcolor(int color);

char player_name[2][30];   // 2 players + each can have 29 char in name
int token_position[2][4];   // 2 players + 4 token each
int score[2];      // how many tokens are finsihed
int current_player = 0;  //( 0=player 1),(1=player 2)
int dice_value = 0;  // store result of last dice roll
int gameover = 0;    // 0 if running ,1 if one player won
int extra_turn = 0;  // if a player got 6 ,then he will get extra turn

// this function means move the cursor on ths screen to position x,y




#define TEXT_COL 35
#define TEXT_ROW 2
int text_line = TEXT_ROW;  // tracks current text line on right side

void printgame(const char* msg, int color = 7) {
	gotoxy(TEXT_COL, text_line++);
	setcolor(color);
	cout << msg;
	setcolor(7);
}

void cleargametext() {
	// Clear the right side text area
	for (int r = TEXT_ROW; r < 30; r++) {
		gotoxy(TEXT_COL, r);
		cout << "                                        "; // 40 spaces
	}
	text_line = TEXT_ROW;
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;       
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);   

}

// this function means to change color 

void setcolor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


// initialize set all token to home base and reset score to zero

void initgame() {
	for (int p = 0; p < 2; p++) {
		score[p] = 0;
		for (int t = 0; t < 4; t++) {
			token_position[p][t] = -1;
		}
	}
	current_player = 0;
	gameover = 0;
	extra_turn = 0;
}

// get names of player and set colours

void getplayer_name() {
	setcolor(15);
	cout << "enter name of player 1 ";
	cin >> player_name[0];
	cout << "enter name of player 2  ";
	cin >> player_name[1];
}

// roll dice with animation
int roll_dice() {
	int result;
	int frames[] = { 1,2,3,4,5,6,1,2,3,4 }; 

	setcolor(14);
	cout << "\n";

	
	for (int i = 0; i < 10; i++) {
		result = rand() % 6 + 1;
		cout << "Dice: [ " << result << " ]   " << "\n";
		Beep(400, 50);
		Sleep(100);
		cout << "\033[A"; // go up one line
		cout << "\033[2K"; // clear that line completely
	}

	
	result = rand() % 6 + 1;
	cout << "Dice: [ " << result << " ]   " << "\n\n";

	if (result == 6) {
		setcolor(10);

		
		Beep(600, 100);   
		Beep(800, 100);   
		Beep(1000, 100);   
		Beep(1200, 300);   

		cout << "*** Rolled a 6! Extra turn! ***\n\n";
		extra_turn = 1;
	}
	else {
		Beep(800, 100);
		extra_turn = 0;
	}

	setcolor(7);
	return result;
}
char board[15][15];

void initboard() {
	for (int r = 0; r < 15; r++) {
		for (int c = 0; c < 15; c++) {
			board[r][c] = '.';
		}
	}
	//draw borders with # it is filling left and right border
	for (int r = 0; r < 15; r++) {
		board[r][0] = '#'; 
		board[r][14] = '#';
	}
	//it is filling top and bottom border
	for (int c = 0; c < 15; c++) {
		board[0][c] = '#';
		board[14][c] = '#';
}

	
	board[7][7] = '*';
	board[2][7] = '*';
	board[7][2] = '*';
	board[7][12] = '*';
	board[12][7] = '*';

	//making home with letters r and y

	for (int r = 1; r <= 5; r++) {
		for (int c = 1; c <= 5; c++) {
			board[r][c] = 'R';
		}
}
	for (int r = 1; r < 5; r++) {
		for (int c = 8; c <= 13; c++) {
			board[r][c] = 'Y';
		}
	}

}

// draawing board and setting the coor according to characters to make difference 

void drawboard() {
	gotoxy(0, 0);   // CHANGE from gotoxy(0,5) to gotoxy(0,0)
	for (int r = 0; r < 15; r++) {
		for (int c = 0; c < 15; c++) {
			if (board[r][c] == 'R')       setcolor(12);
			else if (board[r][c] == 'Y')  setcolor(14);
			else if (board[r][c] == '*')  setcolor(10);
			else if (board[r][c] == '1')  setcolor(12);  // player 1 token
			else if (board[r][c] == '2')  setcolor(14);  // player 2 token
			else                          setcolor(7);
			cout << board[r][c] << " ";
		}
		cout << endl;
	}
	setcolor(7);
}





void getboardcoord(int player, int pos, int& row, int& col) {
	// 52 squares going around the board clockwise
	int track[52][2] = {
		// Left column going up (col 1, rows 12 to 8)
		{12,1},{11,1},{10,1},{9,1},{8,1},        // 0-4
		// Top-left corner and top row
		{7,1},{6,1},{7,2},{7,3},{7,4},{7,5},{7,6}, // 5-11
		// Top middle going right
		{6,7},{5,7},{4,7},{3,7},{2,7},            // 12-16  (removed, player 2 home)
		// Adjust: top path
		{1,6},{1,7},{1,8},                        // 17-19
		// Right side going down
		{1,9},{2,9},{3,9},{4,9},{5,9},            // 20-24
		{6,9},{7,9},{7,10},{7,11},{7,12},{7,13},  // 25-30
		// Bottom right going down
		{8,13},{9,13},{10,13},{11,13},{12,13},    // 31-35
		// Bottom row going left
		{13,12},{13,11},{13,10},{13,9},{13,8},    // 36-40
		{13,7},{13,6},{13,5},{13,4},{13,3},       // 41-45
		// Left side going up
		{12,3},{11,3},{10,3},{9,3},{8,3},         // 46-50
		{7,3}                                     // 51
	};

	int homepath[2][6][2] = {
		// Player 1 home path (going right toward center)
		{{8,7},{9,7},{10,7},{11,7},{12,7},{7,7}},
		// Player 2 home path (going left toward center)  
		{{6,7},{5,7},{4,7},{3,7},{2,7},{7,7}}
	};

	row = -1; col = -1;  // default: invalid

	if (pos >= 0 && pos <= 51) {
		row = track[pos][0];
		col = track[pos][1];
	}
	else if (pos >= 52 && pos <= 56) {
		int idx = pos - 52;
		row = homepath[player][idx][0];
		col = homepath[player][idx][1];
	}
}




void updateboard() {
	// Reset non-home, non-border cells
	for (int r = 1; r < 14; r++) {
		for (int c = 1; c < 14; c++) {
			if (board[r][c] != 'R' && board[r][c] != 'Y')
				board[r][c] = '.';
		}
	}
	// Reset path/center cells
	board[7][7] = '*';
	board[2][7] = '*';
	board[7][2] = '*';
	board[7][12] = '*';
	board[12][7] = '*';

	char symbols[2] = { '1', '2' };  // player 1 uses '1', player 2 uses '2'

	for (int p = 0; p < 2; p++) {
		for (int t = 0; t < 4; t++) {
			int pos = token_position[p][t];
			if (pos == -1 || pos == 57) continue;

			int r, c;
			getboardcoord(p, pos, r, c);
			if (r != -1)
				board[r][c] = symbols[p];
		}
	}
}




// players pick which token to move

int selecttoken() {
	int choice;

	// first check — can the player move ANY token at all?
	int canmove = 0;
	for (int t = 0; t < 4; t++) {
		if (token_position[current_player][t] == -1 && dice_value == 6) {
			canmove = 1;  
		}
		if (token_position[current_player][t] >= 0 &&
			token_position[current_player][t] <= 56) {
			canmove = 1;  
		}
	}

	
	if (canmove == 0) {
		setcolor(12);
		cout << "No token can move! Turn skipped.\n";
		setcolor(7);
		return -1;  // -1 means skip
	}

	
	setcolor(15);
	cout << "\n" << player_name[current_player] << " choose a token:\n";

	for (int t = 0; t < 4; t++) {
		cout << "Token " << t + 1 << ": ";
		if (token_position[current_player][t] == -1) {
			setcolor(12);
			cout << "Home Base";
		}
		else if (token_position[current_player][t] == 57) {
			setcolor(10);
			cout << "Finished";
		}
		else {
			setcolor(14);
			cout << "Position " << token_position[current_player][t];
		}
		setcolor(15);
		cout << "\n";
	}

	
	do {
		cout << "\nEnter token number (1-4): ";
		cin >> choice;

		// check if input is in range first
		if (choice < 1 || choice > 4) {
			setcolor(12);
			cout << "Enter 1 to 4 only!\n";
			setcolor(15);
			choice = -1;
			continue;
		}

		int t = choice - 1;  // convert to 0-3

		// token already finished
		if (token_position[current_player][t] == 57) {
			setcolor(12);
			cout << "That token already finished! Pick another.\n";
			setcolor(15);
			choice = -1;
		}
		
		else if (token_position[current_player][t] == -1
			&& dice_value != 6) {
			setcolor(12);
			cout << "Need a 6 to leave home! Pick another.\n";
			setcolor(15);
			choice = -1;
		}

	} while (choice < 1 || choice > 4);

	return choice - 1;
}
void movetoken(int token) {
	int p = current_player;
	int pos = token_position[p][token];

	// CASE 1: token is at home base 
	if (pos == -1) {
		if (dice_value == 6) {
			// player 1 starts at 0, player 2 starts at 13
			if (p == 0) token_position[p][token] = 0;
			if (p == 1) token_position[p][token] = 13;
			setcolor(10);
			cout << "Token " << token + 1 << " leaves home!" << endl;
			setcolor(7);
		}
		return;  // if not 6
	}

	// CASE 2: token is on main track 
	if (pos >= 0 && pos <= 51) {
		int newpos = pos + dice_value;

		// check if can enter home ,player 1(50) and player 2(11)
		if (p == 0 && pos <= 50 && newpos > 50) {
			newpos = 52 + (newpos - 51);
		}
		else if (p == 1 && pos <= 11 && newpos > 11) {
			newpos = 52 + (newpos - 12);
		}
		else if (newpos > 51) {
			newpos = newpos - 52;  // wrap around track
		}

		token_position[p][token] = newpos;
		setcolor(14);
		cout << "Token " << token + 1
			<< " moved to position " << newpos << "\n";
		setcolor(7);
	}

	//  CASE 3: token is on home path 
	else if (pos >= 52 && pos <= 56) {
		int newpos = pos + dice_value;

		if (newpos == 57) {
			token_position[p][token] = 57;
			score[p]++;
			setcolor(10);
			cout << "Token " << token + 1 << " FINISHED!\n";
			setcolor(7);
		}
		else if (newpos < 57) {
			token_position[p][token] = newpos;
			setcolor(14);
			cout << "Token " << token + 1
				<< " on home path: " << newpos << "\n";
			setcolor(7);
		}
		else {
			setcolor(12);
			cout << "Can't move — exact number needed!\n";
			setcolor(7);
		}
	}
}

void checkcapture() {
	int p = current_player;
	int opponent = 1 - p;

	int safesquares[] = { 0, 8, 13, 21, 26, 34, 39, 47 }; // position which are safe the oponent cant send you home

	for (int t = 0; t < 4; t++) {
		int mypos = token_position[p][t];

		if (mypos < 0 || mypos > 51) continue;

		
		int issafe = 0;    // flag
		for (int s = 0; s < 8; s++) {
			if (mypos == safesquares[s]) {
				issafe = 1;   
				break;
			}
		}
		if (issafe == 1) continue;  
		// checking oponents token
		for (int ot = 0; ot < 4; ot++) {  
			if (token_position[opponent][ot] == mypos) {
				token_position[opponent][ot] = -1;  //snd opponent home
				setcolor(12);
				Beep(200, 400);
				cout << player_name[opponent]
					<< "'s token " << ot + 1
					<< " was sent home!\n";
				setcolor(7);
			}
		}
	}
}


int checkwin() {
	
	
	if (score[current_player] == 4) {
		return 1;  
	}
	return 0; 
}


void showwinner() {
	
	cout << "\n\n";

	if (current_player == 0) {
		setcolor(12);  // red for player 1
	}
	else {
		setcolor(14);  // yellow for player 2
	}

	
	cout << "================================\n";
	cout << "        GAME OVER!              \n";
	cout << "================================\n";
	cout << "  WINNER: "
		<< player_name[current_player] << "!\n";
	cout << "================================\n\n";

	
	Beep(523, 300);  
	Beep(659, 300);  
	Beep(784, 300);  
	Beep(1047, 500);  

	setcolor(7);
}






void savegame() {
	
	ofstream file("savegame.dat");

	
	if (!file) {
		setcolor(12);
		cout << "Error! Could not save game.\n";
		setcolor(7);
		return;
	}

	
	file << current_player << "\n";

	
	file << player_name[0] << "\n";
	file << player_name[1] << "\n";

	
	file << score[0] << "\n";
	file << score[1] << "\n";

	
	for (int t = 0; t < 4; t++) {
		file << token_position[0][t] << "\n";
	}
	
	for (int t = 0; t < 4; t++) {
		file << token_position[1][t] << "\n";
	}

	
	file.close();

	setcolor(10);
	cout << "\nGame saved successfully!\n\n";
	setcolor(7);
}



void loadgame() {
	
	ifstream file("savegame.dat");


	if (!file) {
		setcolor(12);
		cout << "No saved game found!\n";
		setcolor(7);
		return;
	}

	
	file >> current_player;


	file >> player_name[0];
	file >> player_name[1];

	
	file >> score[0];
	file >> score[1];

	
	for (int t = 0; t < 4; t++) {
		file >> token_position[0][t];
	}

	for (int t = 0; t < 4; t++) {
		file >> token_position[1][t];
	}

	
	file.close();

	setcolor(10);
	cout << "\nGame loaded successfully!\n\n";
	setcolor(7);
}

// check if file exits

int savexists() {
	ifstream file("savegame.dat");
	if (file) {
		file.close();
		return 1;  
	}
	return 0;  
}



int showmenu() {
	int choice;

	
	system("cls");

	
	setcolor(14); // yellow
	cout << "================================\n";
	cout << "         LUDO GAME              \n";
	cout << "      2 Player Edition          \n";
	cout << "================================\n\n";

	setcolor(10); // green
	cout << "  1. New Game\n\n";

	setcolor(12); // red
	cout << "  2. Quit\n\n";

	setcolor(15); // white
	cout << "================================\n";
	cout << "Enter your choice (1 or 2): ";

	
	do {
		cin >> choice;
		if (choice != 1 && choice != 2) {
			setcolor(12);
			cout << "Invalid! Enter 1 or 2: ";
			setcolor(15);
		}
	} while (choice != 1 && choice != 2);

	setcolor(7);
	return choice;
}  


void cheatwin() {
	// Move all tokens of current player to position 56 (one step from finish)
	for (int t = 0; t < 4; t++) {
		token_position[current_player][t] = 56;
	}
	setcolor(13);
	cout << "\n[CHEAT] All tokens moved to position 56!\n";
	cout << "Now roll a 1 to win!\n\n";
	setcolor(7);
}



int main() {
	srand(time(0));

	int choice = showmenu();

	// this will end the game
	if (choice == 3 ||
		(choice == 2 && savexists() == 0)) {
		setcolor(12);
		cout << "\nThanks for playing! Goodbye!\n";
		setcolor(7);


	}

	// if we enter two the previous game will ressume
	if (choice == 2 && savexists() == 1) {
		loadgame();
		initboard();
		drawboard();
		setcolor(10);
		cout << "Resuming game...\n\n";
		cout << player_name[0] << " vs "
			<< player_name[1] << "\n\n";
		setcolor(7);
	}

	// start the new game
	if (choice == 1) {
		initgame();
		getplayer_name();
		initboard();
		drawboard();
		setcolor(10);
		cout << "\nWelcome " << player_name[0]
			<< " and " << player_name[1]
			<< "!\n\n";
		setcolor(7);
	}


	while (gameover == 0) {
		updateboard();
		drawboard();
		cleargametext();   // ? wipe right side each turn

		// print turn header on right side
		gotoxy(TEXT_COL, TEXT_ROW);
		setcolor(15);
		cout << "--------------------------------";
		gotoxy(TEXT_COL, TEXT_ROW + 1);
		cout << player_name[current_player] << "'s turn!";
		gotoxy(TEXT_COL, TEXT_ROW + 2);
		cout << "--------------------------------";
		text_line = TEXT_ROW + 4;
		setcolor(7);

		// input prompt on right side
		gotoxy(TEXT_COL, text_line++);
		setcolor(14);
		cout << "ENTER=roll  S=save  Q=quit  C=cheat";
		setcolor(7);
		// ... rest of loop

		char key = 'x';
		while (key != '\n') {
			if (key == 'c' || key == 'C') {
				cheatwin();
				updateboard();
				drawboard();
				break;
			}
			key = cin.get();


			if (key == 's' || key == 'S') {
				savegame();
				cout << "Press ENTER to continue...";
				while (cin.get() != '\n');
				break;
			}


			if (key == 'q' || key == 'Q') {
				setcolor(12);
				cout << "\nAre you sure you want to quit?\n";
				cout << "Your game will NOT be saved!\n";
				cout << "Type Y to quit or N to continue: ";
				char confirm;
				cin >> confirm;
				if (confirm == 'y' || confirm == 'Y') {
					setcolor(14);
					cout << "\nThanks for playing!\n";
					setcolor(7);

					exit(0);
				}
				break;
			}
		}

		// if saved then it will continuee to next turn
		if (key == 's' || key == 'S') {
			continue;
		}

		dice_value = roll_dice();

		setcolor(15);
		cout << "You rolled: " << dice_value << "\n\n";

		int canmove = 0;
		for (int t = 0; t < 4; t++) {
			if (token_position[current_player][t] == -1
				&& dice_value == 6) {
				canmove = 1;
			}
			if (token_position[current_player][t] >= 0
				&& token_position[current_player][t] <= 56) {
				canmove = 1;
			}
		}

		if (canmove == 0) {
			setcolor(12);
			cout << "No token can move! Turn skipped.\n\n";
			setcolor(7);
		}
		else {
			int token = selecttoken();
			// AFTER:
			if (token != -1) {
				movetoken(token);
				checkcapture();
				updateboard();   // ? add this
				drawboard();     // ? add this

			}

			if (checkwin() == 1) {
				gameover = 1;
				showwinner();

				// delete save file when game ends
				remove("savegame.dat");
				break;
			}
		}
	}

	cout << "\n";

	if (gameover == 0) {
		if (extra_turn == 1) {
			setcolor(10);
			cout << player_name[current_player]
				<< " gets an extra turn!\n\n";
			setcolor(7);
			extra_turn = 0;
		}
		else {
			current_player = 1 - current_player;
		}
	}
}
	
