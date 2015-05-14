#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <iomanip>
#include <cmath>
#include <windows.h>
#include <fstream>

using namespace std;
#define wall char(42)
#define mine char(4)
#define lu   char(218)
#define pu char(191)
#define ld char(192)
#define pd  char(217)
#define hor char(196)
#define vert char(179)
#define avatar char(1)
#define flag char(20)

int N=18,M=18,B=40,CB,c;
double duration;
HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );
WORD StandardColors;
int background = ( BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED );
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
char interface1[9][15] =
{
 lu,  hor,hor,hor,hor,hor,hor,hor,hor,hor,hor,hor,hor,hor,pu,
 vert,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',vert,
 vert,' ','L','E','V','E','L',':',' ','I',' ',' ',' ',' ',vert,
 vert,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',vert,
 vert,' ','M','I','N','E','S',':',' ',' ',' ',' ',' ',' ',vert,
 vert,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',vert,
 vert,' ','T','I','M','E',':',' ',' ',' ',' ',' ',' ',' ',vert,
 vert,' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',vert,
 ld,  hor,hor,hor,hor,hor,hor,hor,hor,hor,hor,hor,hor,hor,pd
};
struct point
{
    int x,y;
} player,bomb,copy_player;
struct tiles
{
    char hidden = wall, bomb;
}**board;
enum type {Beginner, Intermediate, Expert, Custom};
struct score
{
    char name [10];
    double time;
    type mode = Intermediate;
}current;
void outro();
void menu();
void end_game();
void if_free();
void gotoxy(int y, int x)       // WinApi method to change coordinates of cursor
 {
  COORD c;
  c.X = x;
  c.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void HideCursor()               // WinAPI method to hide position of cursor
{
    ::HANDLE hConsoleOut =::GetStdHandle( STD_OUTPUT_HANDLE );
    ::CONSOLE_CURSOR_INFO hCCI;
    ::GetConsoleCursorInfo( hConsoleOut, & hCCI );
    hCCI.bVisible = FALSE;
    ::SetConsoleCursorInfo( hConsoleOut, & hCCI );
}
void cls()                      //WinApi method to clear screen
{
    HANDLE hStdout;
    hStdout = GetStdHandle( STD_OUTPUT_HANDLE );
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    if( !GetConsoleScreenBufferInfo( hStdout, & csbi ) )
         return;

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    if( !FillConsoleOutputCharacter( hStdout,( TCHAR ) ' ',
    dwConSize, coordScreen, & cCharsWritten ) )
         return;

    if( !GetConsoleScreenBufferInfo( hStdout, & csbi ) )
         return;

    if( !FillConsoleOutputAttribute( hStdout, csbi.wAttributes,
    dwConSize, coordScreen, & cCharsWritten ) )
         return;

    SetConsoleCursorPosition( hStdout, coordScreen );
}
void beep()
{
    _beep( 500, 150);
}
void alert ()
{
    _beep(700,300);
    _beep(700,300);
}
void gbc()              // Getch Beep Cls
{
   getch();
    beep();
    cls();
}
void about()
{
    cls();
    cout<<"\n\n\n\t\tSaCpper - Console Minesweeper in Cpp"
        <<"\n\n\t\tAuthor: Marcin Jozwik"
        <<"\n\t\tVersion: 1.1.0"
        <<"\n\t\tYear: 2015";
    gbc();
    menu();
}
void title()
{
    cout<<"Sa";
    SetConsoleTextAttribute( hOut, FOREGROUND_RED );
    cout<<"Cpp";
    SetConsoleTextAttribute( hOut,StandardColors);
    cout<<"er:\n"<<endl;
}
void title_screen ()
{
    cout<<"\n\n\n\n\n\t\t";
    title();
    cout<<"\tConsole Minesweeper in Cpp\n"<<endl;
    cout<<"\n\t\t-: Marcin Jozwik 2015 :-";
    getch();
    cls();
}
void highscores()
{
    cls();
    const int Y = 100;
    score data [Y];
    cout<<"\n\tWelcome to Hall of Fame.\n\n\tWhich High Scores would you like to see?\n"
        <<"\n\tB - Beginner\n"
        <<"\tI - Intermediate\n"
        <<"\tE - Expert\n"
        <<"\tC - Custom\n";
    ifstream file;
    char ans = getch();
    beep();
    switch (ans)
    {
    case char(27):  cls(); menu(); break;
    case 'B': case 'b': file.open ("highscores_beginner.txt", ios_base :: in); break;
    case 'I': case 'i': file.open ("highscores_intermediate.txt", ios_base :: in); break;
    case 'E': case 'e': file.open ("highscores_expert.txt", ios_base :: in); break;
    case 'C': case 'c': file.open ("highscores_custom.txt", ios_base :: in); break;
    default: cls(); highscores();
    }
    if(file.good() == false) cout<<"\n\n\tFile doesn't exist!\n\tYou don't have any High Scores on this level"<<endl;
    else
    {
    int lines=0;
    char bin [2];
    file.getline(bin,1);
    while ( !file.eof())
    {
        file.getline(data[lines].name,10);
        file >> data[lines].time;
        file.get();
        lines++;
    }
    file.close();
    cls();
    score guard;
    for (int i =0; i < lines; i++)
    {
        for(int j = 0 ; j< lines - 1; j++)
        {
            if(data[j+1].time < data[j].time)
            {
                guard = data[j];
                data [j] = data[j+1];
                data [j+1] = guard;
            }
        }
    }
    cout<<"\t10 Top Scores:\n\n"
        <<"\t"<<setw(4)<<left<<"No."<<setw(14)<<left<<"Name:"<<setw(14)<<left<<"Time: \n";
    for(int i = 0; i<lines; i++)
    {cout<<"\n\t"<<setw(4)<<left<<i+1<<setw(14)<<left<<data[i].name<<setw(14)<<left<<data[i].time; if (i ==9) break;}
    }
    gbc();
    menu();

}
void level_menu()
{
    cout<<"\n\tChoose difficulty level:                 ESC - Back to Menu\n"
        <<"\n\tB - Beginner\n"
        <<"\tI - Intermediate\n"
        <<"\tE - Expert\n"
        <<"\tC - Custom\n"
        <<"\n\tM - Change number of mines"<< endl;
}
void choose_level ()                   //of difficulty
{
    char answer;
    level_menu();
    answer = getch();
    beep();
    switch (answer)
    {
    case char(27):
    case char(13):
            cls();
            menu();
            break;
    case 'B':
    case 'b':
            cout<<"\n\tGame will start in Beginner Mode";
            N=8;
            M=12;
            B=10;                                       //setting size of board
            interface1[2][9]= 'B';                      //putting first letter of levels name i interface array
            current.mode = Beginner;                    //info highscores
            break;
    case 'I':
    case 'i':
            cout<<"\n\tGame will start in Intermediate Mode";
            N=18;
            M=18;
            B=40;
            interface1[2][9] = 'I';
            current.mode = Intermediate;
            break;
    case 'e':
    case 'E':
            cout<<"\n\tGame will start in Expert Mode";
            N=18;
            M=32;
            B=99;
            interface1[2][9] = 'E';
            current.mode = Expert;
            break;
    case 'c':
    case 'C':
            do
            {
            cout<<"\n\tChoose width of board: ";
            cin>>M;
            while (cin == false)
            {
                cin.clear();
                cin.sync();
                cout<<"\tWidth must be a number: ";
                alert();
                cin>>M;
            }
            beep();
            cout<<"\tChoose length of board: ";
            cin>>N;
            while (cin == false)
            {
                cin.clear();
                cin.sync();
                cout<<"\tLength must be a number: ";
                alert();
                cin>>N;
            }
            beep();
            if(M<9 || N<9 || M>51 || N>51)                  //smaller boards will crush interface (because of gotoxy method)
            {
                cout<<"\tWrong data - choose size from range <9,50>";
                alert();
                getch();
                cls();
                level_menu();
            }
            }
            while (M<9 || N<9 || M>51 || N>51 );
            interface1[2][9] = 'C';
            current.mode = Custom;
            cout<<"\tGame will start in Custom Mode";
            break;
    case 'M':
    case 'm':
            cout<<"\n\tHow many mines you want? ";
            cin>>CB;
            while (cin == false)
            {
                cin.clear();
                cin.sync();
                cout<<"\tGive me a number: ";
                alert();
                cin>>CB;
            }
            if(CB<1)
            {
                cout<<"\tWrong data - you need at least 1 bomb!";
                alert();
                getch();
                cls();
                level_menu();
            }
            else if(B>((M-1)*(N-1)))
            {
                cout<<"\tWrong data - amount of mines can't be greater than number of fields!";
                alert();
                getch();
                cls();
                level_menu();
            }
            else
            {
            B = CB;
            cout<<"\tYour board will have "<<B<<" bombs";
            interface1[2][9] = 'C';
            current.mode = Custom;
            break;
            }
    default:
            cls();
            choose_level();
    }
    gbc();
    choose_level();
}
void how_to_play ()              //how to play
{
 cout<<"\n\tSaCpper is port of classic built-in Windows game - Minesweeper.\n\n\tYour objective is to mark every single mine placed on a board \n\twith hidden fields.\n\tTo help you do that on board you can find some clues - numbers, \n\twhich describe how many bombs are set around them. \n\n\tSimple? Maybe. But hard to master.  "
     <<"\n\n\t"<<setw(20)<<left<<"Keys:"<<setw(20)<<left<<"Standard:"<<"Alternative:"
     <<"\n\n\t"<<setw(20)<<left<<"Move player:"<<setw(20)<<left<<"Arrow keys"<<"WSAD"
     <<"\n\t"<<setw(20)<<left<<"Reveal square:"<<setw(20)<<left<<"X"<<"ENTER"
     <<"\n\t"<<setw(20)<<left<<"Set flag:"<<setw(20)<<left<<"SPACE"<<"'"
     <<"\n\n\tGood luck!";
 gbc();
 menu();
}
point draw_point ()        // draw coordinates on game's board
{
    point object;
    object.x = (rand() % (M-2)) +1;
    object.y = (rand() % (N-2)) +1;
    return object;
}
void clear_line()                  //clear line for gotoxy
{
    cout<<"                         ";
    cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
}
void print_board () //game board printing
{
    for (int i =0; i < N; i++)
    {
        for (int j=0; j < M ;j++)
        {
            if (board[i][j].hidden == wall){
            SetConsoleTextAttribute ( hOut, background);
            cout<<board[i][j].hidden;
            SetConsoleTextAttribute ( hOut, StandardColors);
            }
            else cout<<board[i][j].hidden;
        }
        cout<<endl;
    }
}
void cout_field_bomb(point &field)      //print fields with colour of foreground and background
{
  switch(board[field.y][field.x].bomb)
            {
            case mine:   SetConsoleTextAttribute( hOut, 0 | background); break;
            case ' ':    SetConsoleTextAttribute( hOut,     background); break;
            case '1':    SetConsoleTextAttribute( hOut, 9 | background); break;
            case '2':    SetConsoleTextAttribute( hOut, 2 | background); break;
            case '3':    SetConsoleTextAttribute( hOut, 12 | background); break;
            case '4':    SetConsoleTextAttribute( hOut, 1 | background); break;
            case '5':    SetConsoleTextAttribute( hOut, 4 | background); break;
            case '6':    SetConsoleTextAttribute( hOut, 3 | background); break;
            case '7':    SetConsoleTextAttribute( hOut, 0 | background); break;
            case '8':    SetConsoleTextAttribute( hOut, 0 | background); break;
            }
            cout<<board[field.y][field.x].bomb;
            SetConsoleTextAttribute ( hOut, StandardColors);
}
void cout_field_hidden(point &field)
{
  switch(board[field.y][field.x].hidden)
            {
            case flag:   SetConsoleTextAttribute( hOut, 1 | background); break;
            case mine:   SetConsoleTextAttribute( hOut, 0 | background); break;
            case wall:   SetConsoleTextAttribute( hOut,     background); break;
            case ' ':    SetConsoleTextAttribute( hOut,     background); break;
            case '1':    SetConsoleTextAttribute( hOut, 9 | background); break;
            case '2':    SetConsoleTextAttribute( hOut, 2 | background); break;
            case '3':    SetConsoleTextAttribute( hOut, 12 | background); break;
            case '4':    SetConsoleTextAttribute( hOut, 1 | background); break;
            case '5':    SetConsoleTextAttribute( hOut, 4 | background); break;
            case '6':    SetConsoleTextAttribute( hOut, 3 | background); break;
            case '7':    SetConsoleTextAttribute( hOut, 0 | background); break;
            case '8':    SetConsoleTextAttribute( hOut, 0 | background); break;
            }
            cout<<board[field.y][field.x].hidden;
            SetConsoleTextAttribute ( hOut, StandardColors);
}
void print_board_bombs () //game board printing
{
    point object;
    for (object.y =0; object.y < N; object.y++)
    {
        for (object.x=0; object.x < M ;object.x++)
        {
        cout_field_bomb(object);
        }
        cout<<endl;
    }
}
void draw_bomb_place()
{
    while (CB>0)
    {
        bomb = draw_point();
        if(board[bomb.y][bomb.x].bomb != mine)
        {
            board[bomb.y][bomb.x].bomb = mine;
            CB--;
        }
        else draw_bomb_place();
    }
}
void create_board ()      //create empty board with frame
{
    for (int i =0; i < N; i++)
    {
        board[i] = new tiles [M];
        for (int j=1; j < M-1 ;j++)
        {
            if (i == 0 || i == N-1)
                {
                    board [i][j].hidden = hor;
                }
            else
                {
                    board [i][j].bomb = '0';
                }
        }
        board [i][0].hidden = vert;
        board [i][M-1].hidden = vert;
    }
    board [0][0].hidden  = lu;
    board [0][M-1].hidden =  pu ;
    board [N-1][0].hidden =  ld;
    board [N-1][M-1].hidden = pd;

}
void set_bombs ()           //creating right numbers around bombs
{
    for (int i = 1 ; i < N-1 ; i++)
    {
        for(int j = 1; j < M-1; j++)
         {
            if (board [i][j].bomb == mine)
            {
               if(board [i+1][j].bomb != mine) board [i+1][j].bomb ++;
               if(board [i-1][j].bomb != mine) board [i-1][j].bomb ++;
               if(board [i][j-1].bomb != mine) board [i][j-1].bomb ++;
               if(board [i][j+1].bomb != mine) board [i][j+1].bomb ++;
               if(board [i+1][j+1].bomb != mine) board [i+1][j+1].bomb ++;
               if(board [i-1][j-1].bomb != mine) board [i-1][j-1].bomb ++;
               if(board [i+1][j-1].bomb != mine) board [i+1][j-1].bomb ++;
               if(board [i-1][j+1].bomb != mine) board [i-1][j+1].bomb ++;
            }
        }
    }
}
void rearrange_board()              //remove 0 number and corrections of frame
{
     for (int i = 0 ; i < N ; i++)
    {
        for(int j = 0 ; j < M ; j++)
        {
             if (i == 0 || i == N-1)
                {
                    board [i][j].bomb = hor;
                }
            else if(board[i][j].bomb == '0')  board[i][j].bomb = ' ';
        }
     board[i][0].bomb = vert;
     board [i][M-1].bomb = vert;
    }
    board [0][0].bomb  = lu;
    board [0][M-1].bomb =  pu ;
    board [N-1][0].bomb =  ld;
    board [N-1][M-1].bomb = pd;
}
void print_interface1()                 // interface on right with background
{
    for (int i =0; i < 9; i++)
    {
        gotoxy(i,M+2);
        cout<<interface1[i][0];
        for (int j=1; j <14 ;j++)
        {
            if (i ==0 || i == 8) cout<<interface1[i][j];
            else
            {
            SetConsoleTextAttribute( hOut, 0 | background);
            cout<<interface1[i][j];
            SetConsoleTextAttribute( hOut, StandardColors);
            }
        }
        cout<<interface1[i][14];
    }
    gotoxy(2,M+11);
    cout<<interface1[2][9];
}
void coordinates()                      //show coordinates of player
{
    gotoxy(12,M+2);
    clear_line();
    cout<<avatar<<": ["<<player.x<<","<<player.y<<"]"<<endl;
}
bool timer (clock_t start)               //timer
{
    duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;  //time from start
    gotoxy(6,M+10);
    cout<<fixed<<setprecision(1)<<duration;
}
void if_mine()
{
    if (board[player.y][player.x].bomb == mine) end_game();
}
void place_flag()
{
    if(board[player.y][player.x].hidden == wall || board[player.y][player.x].hidden == flag)
    {
    if (board[player.y][player.x].hidden == flag)
    {
    board[player.y][player.x].hidden = wall;
    CB++;                   //mine counter
    }
    else
    {
      board[player.y][player.x].hidden = flag;
      CB--;
    }
    gotoxy(player.y,player.x);
    cout_field_hidden(player);
    }
}
void change_field()         //shows whats behind wall
{
    board[player.y][player.x].hidden = board[player.y][player.x].bomb;
    gotoxy(player.y,player.x);
    cout_field_bomb(player);
}
void check_field()
{
    if (board[player.y][player.x].hidden != flag)
    {
     change_field();
     copy_player = player;
     if_free();
     if_mine();
     int sound = (board[player.y][player.x].bomb == ' ' ? 700 : 500);
    _beep(sound,150);
    }
}
void change_around()            //reveal fields around player if empty
{
        ++player.x;             change_field(); --player.x;
        --player.x;             change_field(); ++player.x;
        ++player.y;             change_field(); --player.y;
        --player.y;             change_field(); ++player.y;
        ++player.y; ++player.x; change_field(); --player.y; --player.x;
        --player.y; --player.x; change_field(); ++player.y; ++player.x;
        ++player.x; --player.y; change_field(); --player.x; ++player.y;
        --player.x; ++player.y; change_field(); ++player.x; --player.y;

}
void go_forward(int x1, int y1)         //reveal squares while empty
{
    point player_temp = player;
    while (board[player.y][player.x].bomb == ' ')
    {
        change_around();
        player.y = player.y + y1;
        player.x = player.x + x1;
    }
    player = player_temp;
}
void big_forward(int x, int y, int x1, int y1) //reveal squares and check right/left
{
  while (board[player.y][player.x].bomb == ' ')
    {
        change_around();
        go_forward(x1,y1);
        go_forward(-x1,-y1);
        player.y = player.y + y;
        player.x = player.x + x;
    }
    player = copy_player;
}
void if_free()
{
    big_forward(0,-1,1,0);
    big_forward(0,1,1,0);
    big_forward(1,0,0,1);
    big_forward(-1,0,0,1);
}
void show_avatar()
{
        gotoxy(player.y,player.x);
        SetConsoleTextAttribute( hOut, 0 | background);
        cout<<avatar;
        SetConsoleTextAttribute( hOut, StandardColors);
}
void move_right()
{
    if (board[player.y][player.x + 1].hidden != vert && board[player.y][player.x + 1].hidden != hor)
        {
        cout_field_hidden(player);
        player.x++;
        show_avatar();
        }
}
void move_left()
{
   if (board[player.y][player.x - 1].hidden != vert && board[player.y][player.x - 1].hidden != hor)
        {
        cout_field_hidden(player);
        player.x--;
        show_avatar();
        }
}
void move_down()
{
    if (board[player.y + 1][player.x ].hidden != vert && board[player.y + 1][player.x].hidden != hor)
        {
        cout_field_hidden(player);
        player.y++;
        show_avatar();
        }
}
void move_up()
{
   if (board[player.y - 1][player.x ].hidden != vert && board[player.y - 1][player.x].hidden != hor)
        {
        cout_field_hidden(player);
        player.y--;
        show_avatar();
        }
}
void print_all()          //print: board, interface, player and coordinates
{
    print_board();
    gotoxy(0,M+2);
    print_interface1();
    gotoxy(10,M+2);
    cout<<"ESC - Escape";
    gotoxy(11,M+2);
    cout<<"M  - Menu";
    show_avatar();
    coordinates();
}
void update_info(clock_t start)         //after every move (getch)
{
    timer(start);
    coordinates();
    gotoxy(4,M+11);
    if (CB<10 && CB >=0) cout<<"0";
    cout<<CB;
}
void move_player ()
{
 switch (c)
    {
    case 77:
    case 'd': move_right();
              break;
    case 75:
    case 'a': move_left();
              break;
    case 72:
    case 'w': move_up();
              break;
    case 80:
    case 's': move_down();
              break;
    case 13:
    case 'x': check_field();
              break;
    case '\'':
    case 32:  place_flag();
              _beep(300,200);
              break;
    case 109:                 // M
              cls();
              menu();
              break;
    case 27:  outro();      // ESC

    }
}
bool check_win()
{
    if(CB==0)
    {
        gotoxy(0,0);
        for(int i = 1; i<N-1; i++)
        {
            for(int j = 1; j <M-1; j++)
            {
                if(board[i][j].hidden == wall) return false;
            }
        }
        return true;
    }
    return false;
}
void retry()
{
   cout <<"\n\nDo you want to start again? [y/n] [ ]\b\b";
 while (kbhit()) c = getch();
 char ans;
 ans = getch();
 beep();
 if (ans == 'y' || ans == 'Y' || ans == 't' || ans == 'T')
 {
     cls();
     menu();
 }
 else outro();
}
void clear_info()           //making space for sad/happy face :)
{
  gotoxy(10,M+2);
  clear_line();
  gotoxy(11,M+2);
  clear_line();
  gotoxy(12,M+2);
  clear_line();
}
void save_score()
{
    cout<<"\tYour score is superior!\n\tType your name: ";
    cin>>current.name;
    ofstream file;
    switch(current.mode)
    {
    case 0: file.open("highscores_beginner.txt", ios_base :: app); break;
    case 1: file.open("highscores_intermediate.txt", ios_base :: app); break;
    case 2: file.open("highscores_expert.txt", ios_base :: app); break;
    case 3: file.open("highscores_custom.txt", ios_base :: app); break;
    }
    file<<endl<<current.name<<endl<<current.time;
    file.close();
    cout<<"\tYour time: "<<current.time;
}
void win_game()
{
    current.time = duration;
    SetConsoleTextAttribute( hOut, 0 | background);
    gotoxy(1,1);
    for(int i = 1; i<N-1 ;i++)
    {
        for(int j = 1; j<M-1; j++)
        {
            Sleep(1);
            board[i][j].bomb = avatar;
            cout<<board[i][j].bomb;
        }
        gotoxy(1 + i ,1);
    }
    _beep( 523.2 , 600); // C
    _beep( 784.8 , 900); // C
    SetConsoleTextAttribute( hOut, StandardColors);
    clear_info();
    gotoxy(10,M+5); cout<<" #  #";
    gotoxy(12,M+5); cout<<ld<<hor<<hor<<hor<<hor<<pd;
    int n = (N < 9 ? 6 : 1);
    gotoxy(N+n,0);
    cout<<"\tYOU WIN!!!!"<<endl;
    save_score();
    retry();

}
void create_final_board()
{
    CB = B;
    create_board();
    draw_bomb_place();
    set_bombs();
    rearrange_board();
}
void game()               //list of orders
{
    board = new tiles * [N];
    create_final_board();
    while(board[player.y][player.x].bomb != ' ') player = draw_point();
    clock_t start = clock();
    //print_board_bombs();
    //getch();
    //cls();
    print_all();
    CB = B;
    do
    {
    update_info(start);
    if(check_win()) win_game();
    //Sleep(35);                              // player slow-motion
    c = getch(); while (kbhit()) c = getch();
    gotoxy(player.y,player.x);
    move_player();

    }
    while (1);
}
void menu()
{
    cout<<"\n\t\t   ";
    title();
    cout<<"\n\t\t1 - PLAY!\n"
        <<"\t\t2 - How to play?\n"
        <<"\t\t3 - Settings\n"
        <<"\t\t4 - Highscores\n"
        <<"\t\t5 - About\n"
        <<"\t\t6 - Exit\n";
    char answer;
    answer = getch();
    beep();
    switch (answer)
    {
       case '1':
       case char(13):
                cls();
               game();
               break;
       case '2': cls();
               how_to_play();
               menu();
       case '3': cls();
               choose_level();
               menu();
               break;
       case '4':
               highscores();
               break;
       case '5':
                about();
                break;
       case '6':
                 outro();
                 break;
       default: cls();
                menu();

    }
}
void end_game()
{
  gotoxy(0,0);
  print_board_bombs();
  clear_info();
  _beep( 555.3 , 600); // D                 SAD SONG OF FAILURE
  _beep( 539.3 , 600); // Cis
  _beep( 523.2 , 700); // C
  _beep( 493.9 , 900); // H
  gotoxy(10,M+3); cout<<" X  X";
  gotoxy(12,M+3); cout<<hor<<hor<<hor<<hor<<hor<<hor;
  int n = (N < 9 ? 6 : 1);
  gotoxy(N+n,0);
  cout<<"\tGAME OVER!!!!"<<endl;
  retry();
}
void outro()
{
    cls();
    cout<<"\n\n\n\n";
    title();
    cout<<"Console Minesweeper in Cpp \n\n"<<char(174)<<" All rights reserved - Marcin Jozwik 2015\n\n\n\n\n\n\n\n\n\n"<<endl;
    exit(0);
}
int main()
{

    GetConsoleScreenBufferInfo(hOut, &csbiInfo);
    StandardColors = csbiInfo.wAttributes;
    srand(time(NULL));
    HideCursor();
    title_screen();
    menu();
    outro();
    return 0;
}
