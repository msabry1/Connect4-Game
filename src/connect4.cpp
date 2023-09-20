#include<iostream>
#include <Windows.h>
#include<vector>
#include<map>
using namespace std;

struct Timer
{
    time_t initialTime ;

    Timer(){
        initialTime = time(NULL) ;
    }
    
    void printTimePassed()
    {

        int timePassed = time(NULL) - initialTime ;
        int hours = timePassed/3600 ;
        int minutes = (timePassed-hours*3600)/60 ;
        int seconds = timePassed - hours*3600 - minutes*60 ;

        cout << hours << ':' 
            << minutes << ':' 
            << seconds << '\n' ;

    }
    

};
#define RESET   "\033[0m"
#define RED     "\033[31;1m"
#define YELLOW  "\033[33;1m"
#define BLUE    "\033[34m"
struct ColoursTerminal
{
    HANDLE hConsole ;
    map<string,int> ColoursNum ;
    ColoursTerminal(){
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        ColoursNum = {
            {"red" , 12 },
            {"yellow" , 14} ,
            {"blue" , 9} ,
            {"white" , 15} 
        };
    }

    void colourize(const string &colour) {
        SetConsoleTextAttribute(hConsole,ColoursNum[colour] );
    }

};
struct Game
{
    int rows,colums ;
    vector<vector<char>> grid;

    vector<int> colums_height;
    int colums_height_sum;

    bool isPlayerOneTurn = 1 ;
    
    int player1Score , player2Score;

    ColoursTerminal tr ;
    Game(){
        setSize();
        initializeGrid();
        
        colums_height.resize(colums,0);
        colums_height_sum = 0 ;

        player1Score = 0 ; player2Score=0 ;
    }
    void setSize() {
        cout << "dimentions of Grid >> (colums,rows)\n" ;
        cin >> colums >>rows  ;
    }
    void initializeGrid() {
        grid.resize(rows,vector<char>(colums,'.')) ;
    }
    

    void printGrid()
    {

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < colums; c++)
            {
                char ch = grid[r][c] ;

                if (ch == 'X') 
                    tr.colourize("red") ;
                if (ch == 'O') 
                    tr.colourize("yellow") ;

                cout << ch << "   ";
                tr.colourize("white") ;
            }
            
        cout << "\n\n" ;
        }
        tr.colourize("blue") ;
        for (int i = 1; i <= colums; i++)
            cout << i << "   " ;
        
        cout << "\n\n";
        tr.colourize("white") ;
    }
    void checkGridAndUpdateCnts(char &user,int &r,int &c,int &cnt,int &fours_cnt){
        if(grid[r][c]==user)
                cnt++;
        else 
            cnt = 0;
                
        if (cnt == 4) 
            fours_cnt++ , cnt = 0 ;
    }
    int userConnectedFoursCnt(char user) {
        int foursCnt = 0 ;

        for (int r = 0; r < rows; r++) //check for rows
        {
            int cnt = 0;
            for (int c = 0; c < colums; c++)
                checkGridAndUpdateCnts(user,r,c,cnt,foursCnt) ;
            
        }
        for (int c = 0; c < colums; c++) //check for colums
        {
            int cnt = 0;
            for (int r = 0; r < rows; r++)
                checkGridAndUpdateCnts(user,r,c,cnt,foursCnt) ;
        }


        for (int r = 0; r < rows-3; r++) //check for diag from left (down-right)
        {
            int cnt = 0;
            for (int c = 0,temp_r=r; c < colums && temp_r <rows; c++ , temp_r++)
                checkGridAndUpdateCnts(user,temp_r,c,cnt,foursCnt) ;
            
        }
        for (int c = 1; c < colums-3; c++) //check for diag from top (down-right)
        {
            int cnt = 0;
            for (int r = 0,temp_c=c; r < rows && temp_c <colums; r++ , temp_c++)
                checkGridAndUpdateCnts(user,r,temp_c,cnt,foursCnt) ;
            
        }

        for (int r = 3; r < rows; r++) //check for diag from left (up-right)
        {
            int cnt = 0;
            for (int c = 0,temp_r=r; c < colums && temp_r >=0 ; c++ , temp_r--)
                checkGridAndUpdateCnts(user,temp_r,c,cnt,foursCnt) ;
            
        }
        for (int c = 1; c < colums-3; c++) //check for diag from bottom(up-right)
        {
            int cnt = 0;
            for (int r = rows-1,temp_c=c; r >= 0 && temp_c <colums; r-- , temp_c++)
                checkGridAndUpdateCnts(user,r,temp_c,cnt,foursCnt) ;
            
        }

        return foursCnt;
    }

    void updatePlayersScore(){
        if (!isPlayerOneTurn) 
            player1Score = userConnectedFoursCnt('X') ;
        else 
            player2Score = userConnectedFoursCnt('O') ;
    }

    void runGame() {
        Timer timer ;
        while (true) {
            system("cls") ;
            timer.printTimePassed() ;

            updatePlayersScore() ;

            tr.colourize("red") ;
            cout << "Player one Score is " << player1Score << '\n' ;
            tr.colourize("yellow") ;
            cout << "Player two Score is " << player2Score << "\n\n" ;
            tr.colourize("white") ;
            

            printGrid() ;

            if (colums_height_sum == rows*colums) break;

            cout << "player "<< (isPlayerOneTurn?1:2) << " turn: \n" ;

            int col;
            cin >> col ;
            col-- ;

            if (col < 0 || col >= colums) {
                cout << "invalid input choose a number from 1 to " << colums << '\n' ;
                system("pause") ;
                continue;
            }
            
            int &colum_height = colums_height[col] ;
            if ( colum_height == rows){
                cout << "this colum is complete choose another colum " << '\n' ;
                system("pause") ;
                continue;
            }

            grid[rows-colum_height-1][col] = (isPlayerOneTurn ? 'X' : 'O') ;
            colum_height++ ;
            colums_height_sum++ ;
            isPlayerOneTurn = !isPlayerOneTurn ;

        }

        if (player1Score ==  player2Score) {
            cout << "Draw\n" ; 
            return;
        }

        bool ifFirstplayerWin = 1;
        if (player1Score < player2Score) ifFirstplayerWin = 0 ;

        tr.colourize((ifFirstplayerWin?"red":"yellow")) ;
        cout << "player " << (ifFirstplayerWin?"one":"two") << " win the match " ;
        tr.colourize("white") ;

    }

};

int main()
{
    Game connect4 ;
    connect4.runGame();
    
    return 0;
}
