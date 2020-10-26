#include "Src.h"
#include "Frac.h"

static inline void Manual(){
	printf("\033[4mManual\033[24m\n\n");
	printf("> Press ESC to return to Main Menu\n\n");
	printf("> Valid inputs:\n");
	printf("	b/c\n");
	printf("	-b/c\n");
	printf("	b/-c\n");
	printf("	-b/-c\n");
	printf("	a b/c\n");
	printf("	a -b/c\n");
	printf("	a b/-c\n");
	printf("	a -b/-c\n");
	printf("	-a b/c\n");
	printf("	-a -b/c\n");
	printf("	-a b/-c\n");
	printf("	-a -b/-c\n");
	printf("  where...\n");
	printf("\ta represents the whole number\n");
	printf("\tb represents the numerator\n");
	printf("\tc represents the denominator\n");
	printf("\t  represents the separation between a and b/c\n");
	printf("\ta, b and c are non-negative integers");
	while(_getch() != 27){}
}

namespace Assn{
	void BinaryOperation(const short& choice){
		const str operations[]{"Addition", "Subtraction", "Multiplication", "Division", "", "Comparison"};
		Frac LHS, RHS;
		std::cout << "\n<" + operations[choice] + ">\n";
		printf("Enter 1st fraction: ");
		if(std::cin >> LHS){
			return;
		}
		printf("Enter 2nd fraction: ");
		if(std::cin >> RHS){
			return;
		}

		if(choice == 5){
			const int&& result = Compare(LHS, RHS);
			std::cout << "1st fraction" + (result ? " is " + str(result < 0 ? "smaller" : "larger") + " than " : " and ") + "2nd fraction" + (result ? ".\n" : " are equal.\n");
		} else{
			Frac outcome;
			switch(choice){
				case 0: outcome = LHS + RHS; break;
				case 1: outcome = LHS - RHS; break;
				case 2: outcome = LHS * RHS; break;
				case 3: outcome = LHS / RHS;
			}
			std::cout << "Result: " << outcome << std::endl;
		}
	}

	void Operation(const short& choice){
		if(choice == 7){
			printf("\n<Toggle form>\n");
			std::cout << "Result: " << (Frac::ToggleForm() ? "mixed" : "improper") << std::endl;
			return;
		}
		if(choice == 5){
			printf("\n<Simplification>\n");
			printf("Enter fraction: ");
			Frac input;
			if(std::cin >> input){
				return;
			}
			std::cout << "Result: " << input << std::endl;
			return;
		}
		BinaryOperation(choice - 1);
	}

	void Control(){
		printf("***********************\n");
		printf("* Fraction Calculator *\n");
		printf("***********************\n");
		printf("1: Add\n");
		printf("2: Subtract\n");
		printf("3: Multiply\n");
		printf("4: Divide\n");
		printf("5: Simplify\n");
		printf("6: Compare\n");
		printf("7: Toggle\n");
		short choice = _getch() - 48;
		if(choice == -21){
			return;
		}
		Operation(choice);
		printf("\n<<<Press any key to refresh>>>");
		_getch();
		system("cls");
		Control();
	}
}

namespace Game{
	bool endLoop;
	float secLeft;
	int score;

	static inline const bool Key(const char& key){
		return GetAsyncKeyState((unsigned short)key) & 0x8000;
	}

	void Update(){
		wchar_t* buffer = new wchar_t[50];
		HANDLE handle = GetStdHandle(DWORD(-11)); //Handle to console screen buffer
		DWORD dwBytesWritten = 0;
		float dt;
		system_clock::time_point timePt = system_clock::now();

		while(!endLoop){
			system_clock::time_point timePtNow = system_clock::now();
			duration<float> frameDur = timePtNow - timePt;
			timePt = timePtNow;
			dt = frameDur.count();

			if(Key(char(27))){
				endLoop = 1;
				break;
			}

			swprintf_s(buffer, size_t(50), L"Score: %i\t\t\t Time left: %.5f seconds", score, secLeft);
			secLeft -= dt;
			if(secLeft <= 0.f){
				endLoop = 1;
				break;
			}
			for(size_t i = 0; i < 50; ++i){
				if(int(buffer[i]) < 32 || int(buffer[i]) > 127){
					buffer[i] = '\0';
				}
			}
			WriteConsoleOutputCharacter(handle, buffer, DWORD(50), {0, 10}, &dwBytesWritten);
		}
	}

	void TrueOrFalse(short const& myGameMode){
		endLoop = 0;
		secLeft = 60.f;
		score = 0;
		float keyBT = secLeft;
		std::thread updater(Update);
		short gameMode = myGameMode;
		Frac ans, frac, otherFrac;
		const str operators[]{" + ", " - ", " * ", " / "};

		while(!endLoop){
			const int& finalGameMode = gameMode == 5 ? rand() % 5 : gameMode;
			frac.SetNum(rand() % 11);
			frac.SetDenom((rand() % 10) + 1);
			otherFrac.SetNum(rand() % 11);
			otherFrac.SetDenom((rand() % 10) + 1);
			switch(finalGameMode){
				case 0: ans = frac + otherFrac; break;
				case 1: ans = frac - otherFrac; break;
				case 2: ans = frac * otherFrac; break;
				case 3: ans = frac / otherFrac; break;
				case 4: ans = frac;
			}
			bool showCorrect = rand() & 1;
			std::cout << (finalGameMode == 4 ? std::to_string(frac.GetNum()) + '/' + std::to_string(frac.GetDenom()) + " = " + std::to_string(showCorrect ? ans.GetNum() : rand() % 11) + '/' + std::to_string(showCorrect ? ans.GetDenom() : (rand() % 10) + 1) :
				std::to_string(frac.GetNum()) + '/' + std::to_string(frac.GetDenom()) + operators[finalGameMode] + std::to_string(otherFrac.GetNum()) + '/' + std::to_string(otherFrac.GetDenom()) + " = " + std::to_string(showCorrect ? ans.GetNum() : rand() % 11) + '/' + std::to_string(showCorrect ? ans.GetDenom() : (rand() % 10) + 1));
			while(!endLoop && !((Key('Q') || Key('E')) && keyBT >= secLeft)){}
			if((Key('Q') && showCorrect) || (Key('E') && !showCorrect)){
				++score;
			}
			if((Key('Q') && !showCorrect) || (Key('E') && showCorrect)){
				score -= 2;
			}
			keyBT = secLeft - .5f;
			system("cls");
		}

		updater.join();
		system("cls");
		std::cout << "Game Over!" << std::endl << "Final Score: " << score;
		std::this_thread::sleep_for(3s);
		std::cout << "\n\nPress any key to return to Main Menu...";
		system("pause > nul");
	}

	void Control(){
		printf("////////////////////////\n");
		printf("////// Frac Blitz //////\n");
		printf("////////////////////////\n");
		printf("- The Educational Game -");
		printf("\n\nControls:\n");
		printf("   Press Q - Indicate true\n");
		printf("   Press E - Indicate false\n");
		printf("\nRules:\n");
		printf("   +1 pt for correct ans\n");
		printf("   -2 pts for wrong ans\n");
		printf("   60-seconds time limit\n");
		printf("\n\n*** Press any key to start ***");
		_getch();
		system("cls");

		printf("Choose your game mode:\n");
		printf("0: Addition (+)\n");
		printf("1: Subtraction (-)\n");
		printf("2: Multiplication (*)\n");
		printf("3: Division (/)\n");
		printf("4: Simplification (ga/gb = a/b)\n");
		printf("5: ALL OF EM\n");
		short gameMode = _getch() - 48;
		if(gameMode == -21){
			return;
		}
		system("cls");

		for(short i = 4; i >= 0; --i){
			if(i % 4){
				std::cout << i;
			} else{
				printf(i ? "Ready?" : "GO!!!");
			}
			Sleep(1000);
			system("cls");
		}
		TrueOrFalse(gameMode);
	}
}

int main(){
	SetConsoleTitleA("193541T");
	srand(uint(time(NULL)));
	system("color 06");
	HANDLE StdHandle = GetStdHandle(DWORD(-11));
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(StdHandle, &cursorInfo);
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(StdHandle, &cursorInfo);
MainMenu:
	std::cout << "\033[4mMain Menu\033[24m\n" << "0: Read Manual\n" << "1: Test Assn\n" << "2: Play Game\n\n" << "Press ESC to end prog";
	short choice = _getch() - 48;
	system("cls");
	if(choice == -21){
		goto EndProg;
	}
	choice ? (choice & 1 ? Assn::Control() : Game::Control()) : Manual();
	system("cls");
	goto MainMenu;
EndProg:
	_CrtDumpMemoryLeaks();
}