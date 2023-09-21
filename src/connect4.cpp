#include<iostream>
#include <Windows.h>
#include<vector>
#include<map>
#include<algorithm>
#include<fstream>
using namespace std;
const string SavedGamesfilePath = "SavedGames.txt" ;
const string TopScoresfilePath = "TopScores.txt" ;

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
            {"white" , 15} ,
            {"green" , 2}
        };
    }

    void colourize(const string &colour) {
        SetConsoleTextAttribute(hConsole,ColoursNum[colour] );
    }

};
bool topScoresCompareFunctions(pair<string,int> &p1 ,pair<string,int> &p2){
    return p1.second > p2.second ;
}
struct TopScoresFileManager
{
    ofstream fout ;
    vector<pair<string ,int>> top10Scores;

    vector<pair<string ,int>> returnTopPlayers(){
        ifstream fin ;
        fin.open(TopScoresfilePath) ;
        string playerName;
        string playerScore ;
        while(true) {
            getline(fin,playerName) ;
            if (playerName.empty() || top10Scores.size() > 10) break;
            getline(fin,playerScore) ;

            top10Scores.push_back({playerName,stoi(playerScore)}) ;
        }
        fin.close() ;
        sort(top10Scores.begin(),top10Scores.end(),topScoresCompareFunctions) ;
        return top10Scores ;
    }

    void writePlayer(const string &playername,const int &playerScore) {
        returnTopPlayers() ;

        int playersTopScoreCnt = top10Scores.size() ;
        ofstream fout(TopScoresfilePath) ;

        if (playersTopScoreCnt < 10 ) 
            top10Scores.push_back({playername,playerScore}) ;
        else {
            for (int i = playersTopScoreCnt-1; i >=0 ; i--)
                {
                    if (playerScore > top10Scores[i].second)
                        {
                            top10Scores[i] = {playername,playerScore} ;
                            break;
                        }
                }
            }
        playersTopScoreCnt = top10Scores.size() ;
        for (int i = 0; i < playersTopScoreCnt; i++)
        {
            fout << top10Scores[i].first << '\n';
            fout << top10Scores[i].second << '\n'; 
        }
        fout.close();
    }
};


struct Game
{
    int rows,colums ;
    vector<vector<char>> grid;

    vector<int> colums_height;
    int colums_height_sum;

    bool isPlayerOneTurn , isTwoPlayersGame  ;
    
    int player1Score , player2Score;
    bool isNewGamePub ;

    ColoursTerminal tr ;
    Game(bool isNewGame=true,string FEN=""){
        isNewGamePub = isNewGame ; // i will use it later
        if (isNewGame)
        {
            setSize() ;
            initializeGrid();
            isPlayerOneTurn = 1;
        }
        else
            loadGame(FEN) ;
        
        colums_height.resize(colums,0);
        colums_height_sum = 0 ;

        player1Score = 0,player2Score=0 ;

        if (!isNewGame)
                matchColumsHeightWirhGrid() ;


        srand(time(0));
    }
    void setSize() {
        cout << "dimentions of Grid >> (colums,rows)\n" ;
        cin >> colums >>rows  ;
    }
    void initializeGrid() {
        grid.resize(rows,vector<char>(colums,'.')) ;
    }
    void matchColumsHeightWirhGrid(){
        for (int c = 0; c < colums; c++)
        {
            for (int r = rows-1; r >= 0; r--)
            {
                if (grid[r][c] != '.')
                        {
                            colums_height[c] = (rows) -r ;
                            colums_height_sum++ ;
                        }
                else 
                        break;
            }
        }
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
    string gridToFEN(){ 
        //rows/colums/grid/isplayeroneturn + istwoplayerGame
        string fen = "";
        fen += (rows + '0') ;
        fen += '/' ;
        fen += (colums + '0') ;
        fen += '/' ;

        for (int r = 0; r < rows; r++)
        {
            int cnt=0; 
            for (int c = 0; c < colums; c++)
            {
                char &ch = grid[r][c] ;
                if (ch == '.') {
                    cnt++ ;
                    if (c == colums-1 ||grid[r][c+1] != '.' ){
                        fen += (cnt+'0') ;
                        cnt = 0;
                    }

                }
                else 
                    fen += ch ;
            }
            fen += '/' ;
        }

        fen += (isPlayerOneTurn + '0') ;
        fen += (isTwoPlayersGame + '0') ; 

        return fen;
        
    }
    int extractRowsColsFEN(const string &FEN ,int &fenCnt){
        string tmpNum = "" ;
        while(true)
        {
            if (FEN[fenCnt] == '/') 
            {
                fenCnt++ ;
                break;
            }
            else tmpNum+= FEN[fenCnt] ;

            fenCnt++ ;
        }
        return stoi(tmpNum) ; // str to num
    }
    void loadGame(const string &FEN){
        int fenCnt = 0;

        rows = extractRowsColsFEN(FEN,fenCnt) ;
        colums = extractRowsColsFEN(FEN,fenCnt) ;

        initializeGrid();

        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < colums; c++)
            {
                char ch = FEN[fenCnt++] ;
                if (isdigit(ch)) c += (ch - 1 - '0') ;
                else 
                    grid[r][c] = ch ;
            }
            fenCnt++;
        }
        isPlayerOneTurn = (FEN[fenCnt++] == '1'?1:0) ;
        isTwoPlayersGame = (FEN[fenCnt++] == '1'?1:0) ;

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
    int validRandomCol(){
        int col = -1;
        while (col == -1) {
            col = rand()%colums ;

            int &colum_height = colums_height[col] ;
            if ( colum_height == rows)
                col = -1;
        }
        return col ;
    }
    void updatePlayersScore(){
        if (!isNewGamePub || !isPlayerOneTurn) 
            player1Score = userConnectedFoursCnt('X') ;
        if (!isNewGamePub || isPlayerOneTurn ) 
            player2Score = userConnectedFoursCnt('O') ;
    }

    void runGame(const bool &isTwoPlayers = 0) {
        if (rows < 4 || colums < 4) {
            cout << "minumum grid is 4x4\n" ;
            system("pause") ;
            return;
        }

        Timer timer ;
        time_t my_time = time(NULL);
        
        if (isNewGamePub)
            isTwoPlayersGame = isTwoPlayers ;

        tr.colourize("green") ;
        cout << "(-1) if you want menu or (-2) if you want to save current version of the game\n" ;
        system("pause") ;

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

            int col;
            if (isTwoPlayersGame||isPlayerOneTurn)
            {
                cout << "player "<< (isPlayerOneTurn?1:2) << " turn: \n" ;
                cin >> col ;
                if (col == -1) return ;
                else if (col == -2) {
                    ofstream fout(SavedGamesfilePath,ios_base::app) ;
                    fout << ctime(&my_time)  ;
                    fout << gridToFEN() << '\n' ;

                    cout << "Done Saved\n" ;
                    fout.close();

                    system("pause");
                    continue;
                }
                col-- ;
            }
            else 
                col = validRandomCol() ;

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

        cout << "player " << (ifFirstplayerWin?"one":"two") << " win the match \n"  ;

        tr.colourize("white") ;
        if (!ifFirstplayerWin && !isTwoPlayersGame ) return ;

        TopScoresFileManager topscores ;
        cout << "player " << (ifFirstplayerWin?1:2)<< " enter your name : \n" ;
        string playerName;

        cin.ignore();
        getline(cin , playerName) ;

        topscores.writePlayer(playerName, (ifFirstplayerWin?player1Score:player2Score))  ;
    }

};

struct connect4GameManager
{
    int menu(){
        int choice=-1 ;
        while (choice == -1 ) {
            cout << "enter a num between 1 and 5 : \n" ;

            cout << "1) New two players game\n" ;
            cout << "2) New one players game\n" ;
            cout << "3) Load Game\n" ;
            cout << "4) Print Top Players\n" ;
            cout << "5) Exit\n" ;

            cin >> choice ;

            if (choice < 1 || choice > 5 )

                {
                    cout << "invalid choice\n" ;
                    choice = -1 ;
                }
        }
        return choice ;
    }

    void run(){
        int choice ;
        while(true) {
            choice = menu() ;

            if (choice == 1 || choice == 2) {
                Game twoPlayersGame ;
                twoPlayersGame.runGame(choice == 1) ;
            }
            else if (choice == 3)
            {
                vector<pair<string,string>> savedGames ;
                string date , FEN ;
                ifstream fin(SavedGamesfilePath) ; 
                while(true) {
                    getline(fin,date) ;
                    if (date.empty()) break;
                    getline(fin,FEN) ;

                    savedGames.push_back({date,FEN}) ;
                }

                int savedGamesLen = savedGames.size() ;

                if (savedGamesLen)
                {
                    for (int i = 0; i < savedGamesLen; i++){
                        cout << i+1 << " ) saved at " << savedGames[i].first << '\n' ;
                    }

                    int choice ;
                    cin >> choice ;
                    if (choice < 1 || choice > savedGamesLen) 
                        cout << "invalid number\n" ;
                    
                    FEN = savedGames[choice-1].second ;
                    Game twoPlayersGame(false,FEN) ;
                    twoPlayersGame.runGame() ;

                }
                else 
                    cout << "no saved Games found\n" ;
                
            }
            else if (choice == 4) 
                {
                    TopScoresFileManager topscores ;
                    vector<pair<string,int>> topPlayers = topscores.returnTopPlayers() ;

                    if (topPlayers.empty()) 
                        cout << "No such top players, play games to store top players\n";
                    else 
                        for (auto playerInfo :topPlayers) 
                            cout << "name: "<< playerInfo.first << " | score: " << playerInfo.second << '\n' ;
                }
            else if (choice == 5) 
                exit(0) ;
                

        }
    }
};


int main()
{
    connect4GameManager connect4 ;
    connect4.run();
    
    return 0;
}