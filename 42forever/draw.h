#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define numMenu 4
#define numCtrl 11
struct Console {
	int width;
	int height;
	HANDLE hOutput;
	HANDLE hInput;
	COORD coord;
	CONSOLE_SCREEN_BUFFER_INFO defaultBufferSize;
	int ch;
};

struct Controls {
	char Xpos[3];
	char Xspd[3];
	char Xacc[3];
	char Xbrk[3];
	char Ypos[3];
	char Yspd[3];
	char Yacc[3];
	char Ybrk[3];
};

struct Item {
	char str[30];
	COORD coord;
};

_Bool console_init(struct Console **c) {
	COORD bufferSize = { 80,25 };
	SMALL_RECT sWin = { 0,0,80 - 1,25 - 1 };
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (c == 0) return FALSE;
	(*c) = calloc(1, sizeof(struct Console));
	(*c)->hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	(*c)->hInput = GetStdHandle(STD_INPUT_HANDLE);
	(*c)->ch = -1;
	GetConsoleScreenBufferInfo((*c)->hOutput, &((*c)->defaultBufferSize));
	SetConsoleWindowInfo((*c)->hOutput, TRUE, &sWin);
	SetConsoleScreenBufferSize((*c)->hOutput, bufferSize);
	GetConsoleScreenBufferInfo((*c)->hOutput, &csbi);
	(*c)->width = csbi.dwSize.X;
	(*c)->height = csbi.dwSize.Y;
	return TRUE;
}

void menu_page(struct Item **m, struct Console *c) {
	char str[][10] = {

		"**MENU**",

		"SETTINGS",

		"CONTROL",

		"EXIT"

	};
	if (((*m) = calloc(numMenu, sizeof(struct Item))) == NULL) printf("\nERROR_1");
	int j;
	for (j = 0; j < numMenu; j++) {
		strcpy((*m + j)->str, str[j]);
		(*m + j)->coord.X = c->width / 2 - strlen(str[j]) / 2;
		(*m + j)->coord.Y = c->height / 2 - (numMenu * 2 - 1) / 2 + (2 * j);
		SetConsoleCursorPosition(c->hOutput, (*m + j)->coord);
		printf("%s", (*m + j)->str);
	}
	c->coord.X = (*m + 1)->coord.X - 1;
	c->coord.Y = (*m + 1)->coord.Y;
	SetConsoleCursorPosition(c->hOutput, c->coord);
	printf(">");
}

void menu_ctrl(struct Item **ctrl, struct Console **c, struct Item *m) {
	char str[][31] = {

		" **CONTROLS** ",

		"Input position on X axis",

		"Input speed on X axis",

		"Input acceleretaion on X axis",

		"Input brake on X axis",

		"Input position on Y axis",

		"Input speed on Y axis",

		"Input acceleretaion on Y axis",

		"Input brake on Y axis",

		"Back",

		"GO"

	};
	if (((*ctrl) = calloc(numCtrl, sizeof(struct Item))) == NULL) printf("\nERROR_1");

	int j;
	for (j = 0; j < numCtrl; j++) {
		strcpy((*ctrl + j)->str, str[j]);
		(*ctrl + j)->coord.X = (*c)->width / 14;
		(*ctrl + j)->coord.Y = (*c)->height / 2 - (numCtrl * 2 - 1) / 2 + (2 * j);
		SetConsoleCursorPosition((*c)->hOutput, (*ctrl + j)->coord);
		printf("%s", (*ctrl + j)->str);
	}
	(*c)->coord.X = (*ctrl + 1)->coord.X - 1;
	(*c)->coord.Y = (*ctrl + 1)->coord.Y;
	SetConsoleCursorPosition((*c)->hOutput, (*c)->coord);
	printf(">");
}

void console_clear(struct Console *c) {
	system("cls");
}

void settings_page(struct Item **set, struct Console **c) {

}

void cursor_down(struct Item *m, struct Console *c, const char num) {
	switch (num) {
	case numMenu: {
		if ((c->coord.Y < (m + num - 1)->coord.Y) && (c->coord.Y >= (m + (num - 3))->coord.Y)) {
			SetConsoleCursorPosition(c->hOutput, c->coord);
			printf(" ");
			c->coord.Y = c->coord.Y + 2;
			int i;
			for (i = 1; i < num; i++) {
				if (c->coord.Y == (m + i)->coord.Y) {
					c->coord.X = (m + i)->coord.X - 1;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}
			}
		}break;
	}
	case numCtrl: {
		if ((c->coord.Y < (m + num - 1)->coord.Y)) {
			SetConsoleCursorPosition(c->hOutput, c->coord);
			printf(" ");
			c->coord.Y = c->coord.Y + 2;
			int i;
			for (i = 1; i < num; i++) {
				if (c->coord.Y == (m + i)->coord.Y) {
					c->coord.X = (m + i)->coord.X - 1;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}
			}
		}
	}break;
	}
}

void cursor_up(struct Item *m, struct Console *c, const char num) {
	switch (num) {
	case numMenu: {
		if ((c->coord.Y <= (m + num - 1)->coord.Y) && (c->coord.Y > (m + (num - 3))->coord.Y)) {
			SetConsoleCursorPosition(c->hOutput, c->coord);
			printf(" ");
			c->coord.Y = c->coord.Y - 2;
			int i;
			for (i = 1; i < num; i++) {
				if (c->coord.Y == (m + i)->coord.Y) {
					c->coord.X = (m + i)->coord.X - 1;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}
			}
		}break; }

	case numCtrl: {
		if ((c->coord.Y <= (m + num - 1)->coord.Y) && (c->coord.Y >(m + (num - 10))->coord.Y)) {
			SetConsoleCursorPosition(c->hOutput, c->coord);
			printf(" ");
			c->coord.Y = c->coord.Y - 2;
			int i;
			for (i = 1; i < num; i++) {
				if (c->coord.Y == (m + i)->coord.Y) {
					c->coord.X = (m + i)->coord.X - 1;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}
			}
		}break;
	}
	}
}

_Bool menu_console_event(struct Console **c) {
	INPUT_RECORD eventBuffer;
	DWORD numEventsRead = 0;
	if (ReadConsoleInput((*c)->hInput, &eventBuffer, 1, &numEventsRead))
		if (numEventsRead == 1)
			if (eventBuffer.EventType == KEY_EVENT)
				if (eventBuffer.Event.KeyEvent.bKeyDown) {
					(*c)->ch = eventBuffer.Event.KeyEvent.wVirtualKeyCode;
					return TRUE;
				}
				else return FALSE;
}

void vvod_proverka(char **str) {
	char pos[3] = { '0','0','0' };
	int i = 0;
	for (i; i <3; i++) {
		char c = '0';
		scanf("%c", &c);
		//printf("%c", c);
		if ((c >= '0') && (c <= '9')) {
			pos[i] = c;
			//printf("%c", pos[i]);
		}
		/*else {
		printf("Wrong input."); break;
		}*/
		//printf("%s", pos);
	}
	//printf("\n");
	//printf("%s", pos);
	//printf("%c", pos);
	if (VK_RETURN) strcpy(&str, pos);
}

void draw(struct Item *ctrl, struct Console *c) {
	c->coord.X = (ctrl + 1)->coord.X - 1;
	SetConsoleCursorPosition(c->hOutput, c->coord);
	printf(" ");
	c->coord.X = c->coord.X + 30;
	SetConsoleCursorPosition(c->hOutput, c->coord);
	printf(">");
}

void make_event_ctrl(struct Console *c, struct Item *ctrl, struct Item **m, struct Controls *st) {
	_Bool isRun = TRUE;
	while (isRun) {
		if (menu_console_event(&c))
			switch (c->ch) {
			case VK_ESCAPE:
				isRun = FALSE;
				system("cls");
				menu_page(&m, c);
				break;
			case VK_DOWN:
				cursor_down(ctrl, c, numCtrl);
				break;
			case VK_UP:
				cursor_up(ctrl, c, numCtrl);
				break;
			case VK_RETURN: {
				if (c->coord.Y == ctrl[1].coord.Y) {
					draw(ctrl, c);
					vvod_proverka(st->Xpos);
					c->coord.X = c->coord.X - 30;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}
				if (c->coord.Y == ctrl[2].coord.Y) {
					draw(ctrl, c);
					vvod_proverka(&st->Xspd);
					c->coord.X = c->coord.X - 30;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}

				if (c->coord.Y == ctrl[3].coord.Y) {
					draw(ctrl, c);
					vvod_proverka(&st->Xacc);
					c->coord.X = c->coord.X - 30;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}

				if (c->coord.Y == ctrl[4].coord.Y) {
					draw(ctrl, c);
					vvod_proverka(&st->Xbrk);
					c->coord.X = c->coord.X - 30;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}

				if (c->coord.Y == ctrl[5].coord.Y) {
					draw(ctrl, c);
					vvod_proverka(&st->Ypos);
					c->coord.X = c->coord.X - 30;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}

				if (c->coord.Y == ctrl[6].coord.Y) {
					draw(ctrl, c);
					vvod_proverka(&st->Yspd);
					c->coord.X = c->coord.X - 30;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}

				if (c->coord.Y == ctrl[7].coord.Y) {
					draw(ctrl, c);
					vvod_proverka(&st->Yacc);
					c->coord.X = c->coord.X - 30;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}

				if (c->coord.Y == ctrl[8].coord.Y) {
					draw(ctrl, c);
					vvod_proverka(&st->Ybrk);
					c->coord.X = c->coord.X - 30;
					SetConsoleCursorPosition(c->hOutput, c->coord);
					printf(">");
				}
				if (c->coord.Y == ctrl[9].coord.Y) {
					printf("%d", c->coord.Y);
					isRun = FALSE;
					system("cls");
					menu_page(&m, c);
				}
				if (c->coord.Y == ctrl[10].coord.Y) {
							//передать дальше по модулю значение сруктуры и прочей херни
				}
			} break;
			}
	}
}

void make_event_menu(struct Console *c, struct Item *m, struct Item **ctrl, struct  Controls *st) {
	_Bool isRun = TRUE;
	while (isRun) {
		if (menu_console_event(&c))
			switch (c->ch) {
			case VK_ESCAPE:
				isRun = FALSE;
				break;
			case VK_DOWN:
				cursor_down(m, c, numMenu);
				break;
			case VK_UP:
				cursor_up(m, c, numMenu);
				break;
			case VK_RETURN: {
				if (c->coord.Y == m[1].coord.Y) {
					//настройки
				}
				if (c->coord.Y == m[2].coord.Y) {
					console_clear(c);
					menu_ctrl(&ctrl, &c,m);
					make_event_ctrl(c, ctrl, m, &st);
				}
				if (c->coord.Y == m[3].coord.Y)	isRun = FALSE;
			}		break;
			}
	}
	return 0;
} 