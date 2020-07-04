#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>

void clean(char* s_resume_game, char* s_quit, char* s_new_game, int row, int col);
void init_new_game_win(int row, int col, int* ok_resume, int argc, char* argv[], int v[], int A[][10]);
void init_game_win(int row, int col, int argc, char* argv[], int v[], int* ok_fisier, char**** config, int A[][10], int B[][10]);
void show_arguments(WINDOW* win, int argc, char* argv[], int row, int col);
int dim_max_arg(int argc, char* argv[]);
void clean_new_game_win(WINDOW* win, int argc, char* argv[], int row, int col, int max);
void init_win_player(WINDOW* win, char**** config, int argc, char** argv, int v[], int B[][10]);
void compose_play_grid(char s[], int B[][10], int x);
void joc(WINDOW* computer, WINDOW* player, WINDOW* game, int A[][10], int* ok_resume2, int row, int col, int B[][10]);

// Functie care genereaza numere aleatoare
int random1()
{
	return rand() % 10;
}
// Functie care verifica daca pozitionarea unui punct 
//ce reprezinta o parte de nava este corecta pe grid
int verify_position(int x, int y, int A[][10], int v)
{
	int i;
	if(A[x][y] == 1)
		return 1;
	if(x > 0 && y > 0 && x < 9 && y < 9)
	{
		{
			for(i = -1; i < 2; i++)
				{
				if(A[x + i][y - 1] == 1)
					return 1;
				if(A[x + i][y + 1] == 1)
					return 1;
				}
				if(A[x - 1][y] == 1)
					return 1;
				if(A[x + 1][y] == 1)
					return 1;
		}
	}
	if(x == 0 && y == 0)
	{
		if(A[x + 1][y] == 1)
			return 1;
		if(A[x + 1][y + 1] == 1)
			return 1;
		if(A[x][y + 1] == 1)
			return 1;
	}
	if(x == 0 && y == 9)
	{
		if(A[x][y - 1] == 1)
			return 1;
		if(A[x - 1][y - 1] == 1)
			return 1;
		if(A[x - 1][y] == 1)
			return 1;		
	}
	if(x == 9 && y == 0)
	{
		if(A[x + 1][y] == 1)
			return 1;
		if(A[x + 1][y + 1] == 1)
			return 1;
		if(A[x][y + 1] == 1)
			return 1;		
	}
	if(x == 9 && y == 9)
	{
		if(A[x - 1][y] == 1)
			return 1;
		if(A[x - 1][y - 1] == 1)
			return 1;
		if(A[x][y - 1] == 1)
			return 1;		
	}
	if(y == 0 && x > 0 && x < 9)
	{
		if(A[x - 1][y] == 1)
			return 1;
		if(A[x + 1][y] == 1)
			return 1;
		if(A[x - 1][y + 1] == 1)
			return 1;
		if(A[x + 1][y + 1] == 1)
			return 1;
		if(A[x][y + 1] == 1)
			return 1;
	}
	if(y == 9 && x > 0 && x < 9)
	{
		if(A[x - 1][y] == 1)
			return 1;
		if(A[x + 1][y] == 1)
			return 1;
		if(A[x - 1][y - 1] == 1)
			return 1;
		if(A[x + 1][y - 1] == 1)
			return 1;
		if(A[x][y - 1] == 1)
			return 1;
	}
	if(x == 0 && y > 0 && y < 9)
	{
		if(A[x][y - 1] == 1)
			return 1;
		if(A[x][y + 1] == 1)
			return 1;
		if(A[x + 1][y + 1] == 1)
			return 1;
		if(A[x + 1][y - 1] == 1)
			return 1;
		if(A[x + 1][y] == 1)
			return 1;
	}
	if(x == 0 && y > 0 && y < 9)
	{
		if(A[x - 1][y] == 1)
			return 1;
		if(A[x + 1][y] == 1)
			return 1;
		if(A[x - 1][y + 1] == 1)
			return 1;
		if(A[x + 1][y + 1] == 1)
			return 1;
		if(A[x][y + 1] == 1)
			return 1;
	}
	return 0;
}
//Functie care verifica daca o nava poate fi asezata pe verticala
int verify_config0(int x, int y, int A[][10], int v)
{
	if(x < v - 1)
		return 0;
	{
		int i;
		for(i = 0; i < v; i++)
			if(verify_position(x - i, y, A, v) == 1)
				return 0;
	}
	return 1;
}
//Functie care verifica daca o nava poate fi asezata pe orizontala
int verify_config1(int x, int y, int A[][10], int v)
{
	if(9 - y + 1 < v)
		return 0;
	
	{
		int i;
		for(i = 0; i < v; i++)
			if(verify_position(x , y + i, A, v) == 1)
				return 0;
	}
	return 1;	
}
//Functie care pe baza functiilor de mai sus genereaza
//o matrice de numere intregi in care elementele cu 1
//reprezinta partile unei nave, iar cele care sunt 0
//reprezinta casute goale de pe grid
void matrice_computer(int A[][10])
{
	int v[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
	int ok = 0, i = 0, x, y, z, j;
	while(i < 10)
	{
	do{	
	ok = 0;
	x = random1();
	y = random1();
	z = random1() % 2;
	if(verify_position(x, y, A, v[i]) == 0)
	{
		if(z == 0)
		{
			if(verify_config0(x, y, A, v[i]) == 1)
				{
				ok = 1;
				for(j = 0; j < v[i]; j++)
					A[x - j][y] = 1;
				}
		}
		if(z == 1)
		{
			if(verify_config1(x, y, A, v[i]) == 1)
				{
				ok = 1;
				for(j = 0; j < v[i]; j++)
					A[x][y + j] = 1;
				}
		}
	}

	}while(ok == 0);
	
	i++;
	
	}
}
//Functie care sterge ultimul caracter dintr-un sir
void strdel(char *s, int n)
{
	char aux[100];
	strcpy(aux, s + n);
	strcpy(s, aux);
}
//Functie care afiseaza grid-ul computerului intr-un fisier(pentru verificare)
int afisare_grid_comp(int A[][10])
{
	FILE* file;
	int i, j;
	file = fopen("computer_grid.txt","w");
	if(file == NULL)
	{
		printf("[Eroare]: Nu s-a putut deshide fisierul care contine grid-ul computerului");
		return 1;
	}
	else
	{
		for(i = 0; i < 10; i++)
		{
			fprintf(file, "|");
			for(j = 0; j < 10; j++)
				if(A[i][j] == 0)
					fprintf(file, " |");
				else
					fprintf(file, "x|");
			fprintf(file, "\n");
		}
	}
	fclose(file);
	return 0;
}
//Functie care afiseaza meniul principal
void init_scr_menu(char* s_resume_game, char* s_quit, char* s_new_game, int row, int col)
{
	char s1[]="	   ____  ___  ______________    ___________ __  __________ "; 
	char s2[]="	  / _  )/   |/_  __/_  __/ /   / ____/ ___// / / /  _/ __ \\";
	char s3[]="	 / __  / /| | / /   / / / /   / __/  \\__ \\/ /_/ // // /_/ /";
	char s4[]="	/ /_/ / ___ |/ /   / / / /___/ /___ ___/ / __  // // ____/ ";
 	char s5[]="     /_____/_/  |_/_/   /_/ /_____/_____//____/_/ /_/___/_/      ";
	getmaxyx(stdscr, row, col);
	attron(A_BLINK);
	move(1, 2);
	printw("%s", s1);
	move(2, 2);
	printw("%s", s2);
	move(3, 2);
	printw("%s", s3);
	move(4, 2);
	printw("%s", s4);
	move(5, 2);
	printw("%s", s5);
	attroff(A_BLINK);
	move(0,0);
	mvprintw(row / 3,(col - strlen(s_new_game)) / 2, s_new_game);
	mvprintw(row / 3 + 1, (col - strlen(s_resume_game)) / 2, s_resume_game);
	mvprintw(row / 3 + 2,(col - strlen(s_quit)) / 2, s_quit);
	move(0,0);
	refresh();
}
//Functie care se ocupa cu navigarea in meniul principal si cu initierea
//urmatoarelor ferestre
void first_window(char* s_resume_game, char* s_quit, char* s_new_game, int row, int col,
 int argc, char* argv[], int* ok_fisier, char**** config, int A[][10], int B[][10])
{
	init_scr_menu(s_resume_game, s_quit, s_new_game, row, col);
	refresh();
	noecho();
	cbreak();
	int v[argc - 1];
	int ch, ok1 = 0, ok2 = 0, ok3 = 0, ok_resume=0, n = 0;
	keypad(stdscr, TRUE);
	while(1)
	{
		refresh();	
		ch = wgetch(stdscr);
		switch(ch)
		{	
			case KEY_UP:
				clean(s_resume_game, s_quit, s_new_game, row, col);
				if(n == row / 3 + 1 || n == row / 3 + 2)
				{
					n--;
					move(n, (col - strlen(s_new_game)) / 2 - strlen(s_resume_game) + 3);
					printw("-->");
					move(n,(col - strlen(s_new_game)) / 2 + 2 + strlen(s_resume_game));
					printw("<--");
				}
				else
					if(n == row / 3)
					{
						move(row / 3 + 2, (col - strlen(s_new_game)) / 2 - strlen(s_resume_game) + 3);
						printw("-->");
						move(row / 3 + 2,(col - strlen(s_new_game)) / 2 + 2 + strlen(s_resume_game));
						printw("<--");
						n = row / 3 + 2;
					}
					else
					{
						move(row / 3, (col - strlen(s_new_game)) / 2 - strlen(s_resume_game) + 3);
						printw("-->");
						move(row / 3,(col - strlen(s_new_game)) / 2 + 2 + strlen(s_resume_game));
						printw("<--");
						n =row / 3;	
					}	
				if(n == row / 3) 
					{
					ok1 = 1;
					ok2 = 0;
					ok3 = 0;
					}
					else
						if(n == row / 3 + 1)
						{
							ok1 = 0;
							ok2 = 1;
							ok3 = 0;
						}
						else
						{
							ok1 = 0;
							ok2 = 0;
							ok3 = 1;
						}
				break;
			case KEY_DOWN:
				clean(s_resume_game, s_quit, s_new_game, row, col);
				if(n == row / 3 || n == row / 3 + 1)
				{
				n++;
				move(n,(col - strlen(s_new_game)) / 2 - strlen(s_resume_game) + 3);
				printw("-->");
				move(n,(col - strlen(s_new_game)) / 2 + 2 + strlen(s_resume_game));
				printw("<--");
				}
				else
					if(n == row / 3 + 2)
					{
						move(row / 3,(col - strlen(s_new_game)) / 2 - strlen(s_resume_game) + 3);
						printw("-->");
						move(row / 3,(col - strlen(s_new_game)) / 2 + 2 + strlen(s_resume_game));
						printw("<--");
						n = row / 3;
					}
					else
					{
						move(row / 3, (col - strlen(s_new_game)) / 2 - strlen(s_resume_game) + 3);
						printw("-->");
						move(row / 3,(col - strlen(s_new_game)) / 2 + 2 + strlen(s_resume_game));
						printw("<--");
						n = row / 3;
					}		
				if(n == row / 3) 
					{
					ok1 = 1;
					ok2 = 0;
					ok3 = 0;
					}
					else
						if(n == row / 3 + 1)
						{
							ok1 = 0;
							ok2 = 1;
							ok3 = 0;
						}
						else
						{
							ok1 = 0;
							ok2 = 0;
							ok3 = 1;
						}
				break;
			case 10:
				if(ok1 == 1 && ok2 == 0 && ok3 == 0)
					{
						init_new_game_win(row, col, &ok_resume, argc, argv, v, A);
						if(ok_resume == 1)
						{
							init_game_win(row, col, argc, argv, v, ok_fisier, config, A, B);
							if(*ok_fisier == 1)
								goto iesire;	
						}
						init_scr_menu(s_resume_game, s_quit, s_new_game, row, col);
						refresh();
					}
				else
					if(ok1 == 0 && ok2 == 1 && ok3 == 0 && ok_resume == 1)
					{
						init_game_win(row, col, argc, argv, v, ok_fisier, config, A, B);
						if(*ok_fisier == 1)
								goto iesire;
						init_scr_menu(s_resume_game, s_quit, s_new_game, row, col);
						refresh();	
					}
					else
						if(ok1 == 0 && ok2 == 0 && ok3 == 1)
						{
							goto iesire;
						}
						else
							if(ok1 == 0 && ok2 == 1 && ok3 == 0 && ok_resume == 0)
								{
								char s_error_resume[] = "There is no game to be resumed";
								mvprintw(row / 3 + 5, (col - strlen(s_error_resume)) / 2, s_error_resume);
								}
				break;
			default:
				break;
		}
	}
	refresh();
	while(1);
	iesire: ;
}
//Functie care afiseaza meniul din care jucatorul isi alege configuratia
//de nave pentru joc
void init_new_game_win(int row, int col, int* ok_resume,
 int argc, char* argv[], int v[], int A[][10])
{
	WINDOW* new_game;
	new_game = newwin(row, col, 0, 0);
	wrefresh(new_game);
	wmove(new_game, 0, col - 28);
	wprintw(new_game, "Press Q to quit to Main Menu");
	wmove(new_game, 3, 5);
	wprintw(new_game, "Choose a map for the ships:");
	show_arguments(new_game, argc, argv, row, col);
	wmove(new_game, 0, 0);
	wrefresh(new_game);
	noecho();
	cbreak();
	keypad(new_game, TRUE);
	int ch, ok, max = dim_max_arg(argc, argv);
	int n = 0, i, j;
	*ok_resume = 0;
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			A[i][j] = 0;
	while(1)
	{
	ok = 0;
	ch = wgetch(new_game);
	switch(ch)
	{
		case 113:
			goto iesire_new_win;
			break; 
		case 10:
			if(n != 0)
			{
			*ok_resume = 1;
			matrice_computer(A);
			afisare_grid_comp(A);
			goto iesire_new_win;
			}
			break;
		case KEY_UP:
			clean_new_game_win(new_game, argc, argv, row, col, max);
			for(i = 1; i < argc - 1; i++)
				if(n == row / 4 + i * 2) ok = 1;
			if(ok == 1)
				{
					n = n - 2	;
					wmove(new_game, n, (col - max) / 2 - max + 3);
					wprintw(new_game, "-->");
					wmove(new_game, n,(col - max) / 2 + 2 + max);
					wprintw(new_game, "<--");
					wrefresh(new_game);
					for(i = 0; i < argc - 1; i++)
					{
						if(i + 1 == (n - row / 4 + 2) / 2)
							v[i] = 1;
						else v[i] = 0;
					}		
				}
				else
					if(n == row / 4)	
					{
						wmove(new_game, row / 4 + 2 * argc - 4, (col - max) / 2 - max + 3);
						wprintw(new_game, "-->");
						wmove(new_game, row / 4 + 2 * argc - 4,(col - max) / 2 + 2 + max);
						wprintw(new_game, "<--");
						n = row / 4 + 2 * argc - 4;
						wrefresh(new_game);
						for(i = 0; i < argc - 1; i++)
						{
							if(i + 1 == argc - 1)
								v[i] = 1;
							else v[i] = 0;
						}			
					}
					else
					{
						wmove(new_game, row / 4, (col - max) / 2 - max + 3);
						wprintw(new_game, "-->");
						wmove(new_game, row / 4,(col - max) / 2 + 2 + max);
						wprintw(new_game, "<--");
						n = row / 4;	
						wrefresh(new_game);
						for(i = 0; i < argc - 1; i++)
						{
							if(i + 1 == 1)
								v[i] = 1;
							else v[i] = 0;
						}
					}
			break;
		case KEY_DOWN:
			clean_new_game_win(new_game, argc, argv, row, col, max);
			for(i = 0; i < argc - 2; i++)
				if(n == row / 4 + i * 2) ok = 1;
			if(ok == 1)
				{
					n = n + 2	;
					wmove(new_game, n, (col - max) / 2 - max + 3);
					wprintw(new_game, "-->");
					wmove(new_game, n,(col - max) / 2 + 2 + max);
					wprintw(new_game, "<--");
					wrefresh(new_game);		
					for(i = 0; i < argc - 1; i++)
					{
						if(i + 1 == (n - row / 4 + 2) / 2)
							v[i] = 1;
						else v[i] = 0;
					}
				}
				else
					if(n == row / 4 + 2 * (argc - 1))
					{
						wmove(new_game, row / 4, (col - max) / 2 - max + 3);
						wprintw(new_game, "-->");
						wmove(new_game, row / 4,(col - max) / 2 + 2 + max);
						wprintw(new_game, "<--");
						n = row / 4;
						wrefresh(new_game);
						for(i = 0; i < argc - 1; i++)
						{
							if(i + 1 == argc - 1)
								v[i] = 1;
							else v[i] = 0;
						}
					}
					else
					{
						wmove(new_game, row / 4, (col - max) / 2 - max + 3);
						wprintw(new_game, "-->");
						wmove(new_game, row / 4,(col - max) / 2 + 2 + max);
						wprintw(new_game, "<--");
						n = row / 4;	
						wrefresh(new_game);
						for(i = 0; i < argc - 1; i++)
						{
							if(i + 1 == 1)
								v[i] = 1;
							else v[i] = 0;
						}
					}
			break;
		default:
			break;
	}
	}
	wrefresh(new_game);
	while(1);
	iesire_new_win: ;
	wclear(new_game);
	wrefresh(new_game);
	delwin(new_game);
}
//Functie care afiseaza un grid gol
//O folosesc cand afisez grid-ul computerului
void init_grid(WINDOW* win)
{
	int row, col, n, i;
	getmaxyx(win, row, col);
	n = row / 5;
	wmove(win, n - 2, col / 5);
	wprintw(win, "    A B C D E F G H I J ");
	for(i = 0; i < 10; i++)
	{
	wmove(win, n, col / 5);
	wprintw(win, "%d  | | | | | | | | | | |", i);	
	n++;
	}
}
//Functie care initiaza fereastra de joc odata ce este aleasa
//o configuratie de grid de catre utilizator
void init_game_win(int row, int col, int argc, char* argv[], int v[],
 int* ok_fisier, char**** config, int A[][10], int B[][10])
{
	int ok_resume2 = 0;
	WINDOW* game;
	WINDOW *player, *computer;
	game = newwin(row, col, 0, 0);
	wrefresh(game);
	wmove(game, 0, 0);
	wmove(game, row / 5 - 1, col / 5);
	wprintw(game, "COMPUTER");
	wmove(game, row / 5 - 1, 4 * col / 5 - 7);
	wprintw(game, "PLAYER");
	wmove(game, 0, col - 28);
	wprintw(game, "Press Q to quit to Main Menu");
	wmove(game, 0, 0);
	wrefresh(game);
	computer = newwin(row - row / 5, col / 2, row / 5, 0);
	wborder(computer, '|', '|', '-', '-', '$', '$', '$', '$');
	wmove(computer, 0, 0);
	init_grid(computer);
	wrefresh(computer);
	player = newwin(row - row / 5, col / 2, row / 5, col / 2);
	wborder(player, '|', '|', '-', '-', '$', '$', '$', '$');
	wmove(player, 0, 0);
	init_win_player(player, config, argc, argv, v, B);	
	wrefresh(player);
	joc(computer, player, game, A, &ok_resume2, row, col, B);	
	if(ok_resume2 == 1)//
		keypad(game, TRUE);//
	if(*ok_fisier == 1)
		goto iesire_win;
	wrefresh(game);
	int ch;
	while(1)
	{
	ch = getchar();
	switch(ch)
	{
		case 113:
			goto iesire_win;
			break; 
		default:
			break;
	}
	}
	wrefresh(game);
	while(1);
	iesire_win: ;
	wclear(game);
	wrefresh(game);
	delwin(game);	
}
//Functie care afiseaza in meniul de configuratii
//argumentele pe care programul le-a primit
void show_arguments(WINDOW* win, int argc, char* argv[], int row, int col)
{
	int i, n = row / 4;
	for(i = 1; i < argc; i++)
	{
		wmove(win, n, (col - (strlen(argv[i]))) / 2);
		wprintw(win, argv[i]);
		n = n + 2;
	}
}
//Functie care calculeaza lungimea maxima a numelui unui argument
//(fisier care contine configuratie)
int dim_max_arg(int argc, char* argv[])
{
	int maxim = 0, i;
	for(i = 1; i < argc; i++)
		if(strlen(argv[i]) > maxim)
		{
			maxim = strlen(argv[i]);
		}
	return maxim;
}
//Functie care citeste fiecare fisier dat ca argument si
//memoreaza configuratia respectiva
int opening_file(char**** config, int argc, char** argv)
{
	FILE *file;
	int i, j;
	char aux[100];
	for(i = 1; i < argc; i++)
	{
		file = fopen(argv[i], "r");
		if(file == NULL)
		{
			printf("[Eroare]: Fisierul %s nu poate fi deschis", argv[i]);
			return 1;
		}
		else
		{
			j = 0;
			while(j < 10)
			{
				fgets(aux, 100, file);
				strdel(aux + strlen(aux) - 1, 1);
				(*config)[i][j] = malloc((strlen(aux) + 1) * sizeof(char));
				strcpy((*config)[i][j], aux);
				j++;
			}
		}
		fclose(file);
	}
	return 0;
}
//Functie care initiaza o fereastra noua cu grid-ul jucatorului
void init_win_player(WINDOW* win, char**** config,
 int argc, char** argv, int v[], int B[][10])
{
	int row, col, n, i, j;
	getmaxyx(win, row, col);
	n = row / 5;
	wmove(win, n - 2, col / 5);
	wprintw(win, "    A B C D E F G H I J ");
	for(i = 0; i < argc - 1; i++)
	{
		if(v[i] == 1)
			for(j = 0; j < 10; j++)
			{
				wmove(win, n, col / 5);
				wprintw(win, "%d  %s", j, (*config)[i + 1][j]);
				compose_play_grid((*config)[i + 1][j], B, j);
				n++;
			}
	}
}
//Functie care realizeaza o matrice de numere
//intregi pe baza configuratiei grid-ului jucatorului
void compose_play_grid(char s[], int B[][10], int x)
{
	int i, nr = 0;
	for(i = 1; i < strlen(s) - 1; i++)
		{
		if(s[i] == ' ')
		{
			B[x][nr] = 0;
			nr++;
		}
		else
			if(s[i] == 'x')
			{
				B[x][nr] = 1;
				nr++;
			}
		}
}
//Functie care verifica daca inca mai sunt
//nave pe grid-ul calculatorului
int ok_calc(int A[][10])
{
	int i, j;
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			if(A[i][j] == 1)return 1;
	return 0;
}
/*
//Functie care verifica daca inca mai sunt
//nave pe grid-ul jucatorului
int ok_play(int B[][10])
{
	int i, j;
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			if(B[i][j] == 1)return 1;
	return 0;
}
void turn_computer(int B[][10])
{
	int a, b;
	a = random1();
	b = random1();
}
*/
//Functia care contine o parte din implementarea propriu zisa a jocului
//Da voie jucatorului sa parcurga grid-ul calculatorului si sa marcheze casute
void play_winner(int row, int col)
{
	WINDOW *play_winner;
	play_winner = newwin(row, col, 0, 0);
	wmove(play_winner, 5, 5);
	wprintw(play_winner, "YOU WIN!");
	wrefresh(play_winner);
}
void joc(WINDOW* computer, WINDOW* player, WINDOW* game, int A[][10], 
	int* ok_resume2, int row, int col, int B[][10])
{
	int row1, col1;
	getmaxyx(computer, row1, col1);
	int i, ch, n = row1 / 5, m = col1 / 5 + 4;
	int ok1 = 0;
	keypad(game, TRUE);
	wmove(game, 0, col - 32);
	wprintw(game, "                             ");
	wmove(game, 0, col - 32);
	wprintw(game, "Press Q twice to go to Main Menu");
	wrefresh(game);
	wmove(computer, n, m);
	wrefresh(computer);
	int x = 0, y = 0;
	do
	{
		ok1 = 0;
		ch = wgetch(game);
		x = n - row1 / 5;
		y = (m - col1 / 5 - 4) / 2;
		switch(ch)
			{
				case 113:
					*ok_resume2 = 1;
					goto iesire_joc;
				case KEY_DOWN:
					for(i = 0; i < 9; i++)
						{
						if(n == row1 / 5 + i)
							ok1 = 1;
						}
					if(ok1 == 1)
						{
							n++;
							wmove(computer, n, m);
							wrefresh(computer);
						}
						else
							if(n == row1 / 5 + 9)
								{
									wmove(computer, row1 / 5, m);
									wrefresh(computer);
									n = row1 / 5;
								}
					break;
				case KEY_UP:
					for(i = 1; i <= 9; i++)
						{
						if(n == row1 / 5 + i)
							ok1 = 1;
						}
					if(ok1 == 1)
						{
							n--;
							wmove(computer, n, m);
							wrefresh(computer);
						}
						else
							if(n == row / 5)
							{
								wmove(computer, row1 / 5 + 9, m);
								wrefresh(computer);
								n = row1 / 5 + 9;
							}
					break;
				case KEY_RIGHT:
					for(i = 0; i < 18; i = i + 2)
						{
						if(m == col1 / 5 + 4 + i)
							ok1 = 1;
						}
					if(ok1 == 1)
						{
							m = m + 2;
							wmove(computer, n, m);
							wrefresh(computer);
						}
						else
							if(m == col1 / 5 + 22)
							{
								wmove(computer, n, col1 / 5 + 4);
								wrefresh(computer);
								m = col1 / 5 + 4;
							}
					break;
				case KEY_LEFT:
					for(i = 2; i <= 18; i = i + 2)
						{
						if(m == col1 / 5 + 4 + i)
							ok1 = 1;
						}
					if(ok1 == 1)
						{
							m = m - 2;
							wmove(computer, n, m);
							wrefresh(computer);
						}
						else
							if(m == col1 / 5 + 4)
							{
								wmove(computer, n, col1 / 5 + 22);
								wrefresh(computer);
								m = col1 / 5 + 22;
							}
					break;
				case 10:
					if(A[x][y] == 1)
					{
						wmove(computer, n, m);
						wprintw(computer, "x");
						A[x][y] = 2;
					}
					else
						if(A[x][y] == 0)
						{
							wmove(computer, n, m);
							wprintw(computer, "~");
							//turn_computer(B);
						}
					break;
			}
			if(ok_calc(A) == 0)
				play_winner(row, col);
	}while(1);
	sleep(10);
	iesire_joc: ;
}
//Functie care sterge de pe ecranul meniului principal sagetile
//odata ce sunt apasate alte taste
void clean(char* s_resume_game, char* s_quit, char* s_new_game, int row, int col)
{
	move(row / 3,(col - strlen(s_new_game)) / 2 - strlen(s_resume_game) + 3);
	printw("   ");
	move(row / 3,(col - strlen(s_new_game)) / 2 + 2 + strlen(s_resume_game));
	printw("   ");
	move(row / 3 + 1,(col - strlen(s_new_game)) / 2 - strlen(s_resume_game) + 3);
	printw("   ");
	move(row / 3 + 1,(col - strlen(s_new_game)) / 2 + 2 + strlen(s_resume_game));
	printw("   ");
	move(row / 3 + 2,(col - strlen(s_new_game)) / 2 - strlen(s_resume_game) + 3);
	printw("   ");
	move(row / 3 + 2,(col - strlen(	s_new_game)) / 2 + 2 + strlen(s_resume_game));
	printw("   ");
	move(row / 3 + 5, (col - 28) / 2);	
	printw("                              ");
}
//Functie care sterge de pe ecranul meniului de configuratii sagetile
//odata ce sunt apasate alte taste
void clean_new_game_win(WINDOW* win, int argc, char* argv[], int row, int col, int max)
{
	int i;
	for(i = 0; i <= argc * 2 - 2; i++)
	{
		wmove(win, row / 4 + i * 2, (col - max) / 2 - max + 3);
		wprintw(win, "   ");
		wmove(win, row / 4 + i * 2, (col - max) / 2 + 2 + max);
		wprintw(win, "   ");
	}
}
//Functia main
int main(int argc, char* argv[])
{
	time_t seed;
	seed = time(NULL);
	srand(seed);
	if(argc == 1)
	{
		printf("[Eroare]: Nu s-au dat argumente de comanda.\n");
		return 1;
	}
	char ***config;
	config = malloc((argc + 1) * sizeof(char**));
	int i;
	for(i = 1; i <= argc; i++)
		config[i] = malloc(10 * sizeof(char*));
	if(opening_file(&config, argc, argv) == 1)
		return 1;
	int A[10][10] = {0};
	int B[10][10] = {0};
	int ok_fisier = 0;
	char s_new_game[] = "New Game";
	char s_resume_game[] = "Resume Game";
	char s_quit[] = "Quit";
	int row, col;
	initscr();
	getmaxyx(stdscr, row, col);
	first_window(s_resume_game, s_quit, s_new_game, row, col, argc, argv, &ok_fisier, &config, A, B);
	endwin();
	return 0;
}
