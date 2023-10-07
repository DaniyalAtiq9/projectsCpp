#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int MaxRows = 14;
const int MaxCols = 14;

struct Player
{
    string Name;
    char Mark;

    Player(char mark = 'o', string PlayerName = "Player Name")
    {
        Mark = mark;
        Name = PlayerName;
    }

    void setPlayerName(string PlayerName)
    {
        Name = PlayerName;
        return;
    }

    void setPlayerMark(char PlayerMark)
    {
        Mark = PlayerMark;
        return;
    }
};

struct GameBoard
{
    char Board[MaxRows][MaxCols];
    GameBoard(char FillChar = '.')
    {
        for (int r = 0; r < MaxRows; r++)
            for (int c = 0; c < MaxCols; c++)
                Board[r][c] = FillChar;
        return;
    }
};

struct Connect_N
{

    GameBoard Board;   // Game board to hold move data
    int Rows, Columns; // Game board size
    int ConnectLimit;  // How many Marks to connect to win

    Player Players[2]; // list of players
    int TurningPlayer; // index of turning player

    int GameEndState;
    // GameEndState 0 means game is not over
    // GameEndState 1 means first player has won it
    // GameEndState 2 means second player has won it

    int Moves_Made[MaxCols];
    // An array to hold the count of moves made in each
    // column. It will help us determine the row for
    // each move in an efficient way.

    Connect_N(int R = 6, int C = 7, int Limit = 4)
    {
        if (R < 0 || R > MaxRows)
            R = 6;

        if (C < 0 || C > MaxCols)
            C = 7;

        if (Limit < 0)
            Limit = 4;

        Rows = R;
        Columns = C;
        ConnectLimit = Limit;
        TurningPlayer = 0;
        GameEndState = 0;

        for (int i = 0; i < MaxCols; i++)
            Moves_Made[i] = 0;
    }
    bool validate(int R, int C)
    {
        if (C < 0 || C >= Columns)
            return false;
        if (R == Rows)
            return false;
        return true;
    }
    bool VerticalCheck(int R, int Col, char p)
    {
        int count = 0;
        int i = R;
        while (i < Rows)
        {
            if (Board.Board[i][Col] == p)
            {
                count++;
                if (count == ConnectLimit)
                {
                    return true;
                }
            }
            else
                return false;
            i++;
        }
        return false;
    }
    int HorizontalCheck(int R, int Col, char p, bool goLeft)
    {
        int count = 0;
        while (true)
        {
            if (goLeft)
            {
                if (Col == 0)
                    break;
                Col--;
            }
            else
            {
                if (Col == Columns - 1)
                    break;
                Col++;
            }
            if (Board.Board[R][Col] == p)
                count++;
        }
        return count;
    }
    bool HorizontalCheck(int R, int Col, char p)
    {
        return (HorizontalCheck(R, Col, p, true) + HorizontalCheck(R, Col, p, false) + 1 == ConnectLimit);
    }
    int DiagonalCheck(int R, int Col, char p, bool goLeft, bool goTop)
    {
        int count = 0;
        while (1)
        {
            if (goLeft)
            {
                if (Col == 0)
                    break;
                Col--;
            }
            else
            {
                if (Col == Columns - 1)
                    break;
                Col++;
            }
            if (goTop)
            {
                if (R == 0)
                    break;
                R--;
            }
            else
            {
                if (R == Rows - 1)
                    break;
                R++;
            }
            if (p == Board.Board[R][Col])
                count++;
            else
                break;
        }
        return count;
    }
    bool DiagonalCheck(int R, int Col, char p)
    {
        return (DiagonalCheck(R, Col, p, true, true) + DiagonalCheck(R, Col, p, false, false) + 1 == ConnectLimit) || (DiagonalCheck(R, Col, p, true, false) + DiagonalCheck(R, Col, p, false, true) + 1 == ConnectLimit);
    }
    bool MakeMove(int Col)
    {
        int R = Moves_Made[Col];
        if (!validate(R, Col))
            return false;
        int row = Rows - 1 - R;
        Board.Board[row][Col] = Players[TurningPlayer].Mark;
        if (VerticalCheck(row, Col, Players[TurningPlayer].Mark) || HorizontalCheck(row, Col, Players[TurningPlayer].Mark) || DiagonalCheck(row, Col, Players[TurningPlayer].Mark))
        {
            GameEndState = TurningPlayer + 1;
            cout << "player " << TurningPlayer + 1 << " wins \n";
        }
        TurningPlayer = (TurningPlayer + 1) % 2;
        Moves_Made[Col]++;
        return true;
    }

    bool GameisOn()
    {
        return GameEndState == 0;
    }

    bool SaveGame(string FileName)
    {

        ofstream outputFile;
        outputFile.open(FileName);
        if (outputFile)
        {
            outputFile << Rows << " " << Columns << endl;
            outputFile << ConnectLimit << endl;
            outputFile << Players[0].Name << " " << Players[0].Mark << endl;
            outputFile << Players[1].Name << " " << Players[1].Mark << endl;
            outputFile << TurningPlayer << endl;
            outputFile << GameEndState << endl;
            for (int i = 0; i < MaxCols; i++)
            {
                outputFile << Moves_Made[i] << endl;
            }
            outputFile << endl;
            for (int r = 0; r < MaxRows; r++)
            {
                for (int c = 0; c < MaxCols; c++)
                {
                    outputFile << Board.Board[r][c] << " ";
                }
                outputFile << endl;
            }
            return true;
        }
        return false;
    }

    bool LoadSavedGame(string FileName)
    {

        ifstream inputFile;
        inputFile.open(FileName);
        if (inputFile)
        {
            inputFile >> Rows;
            inputFile >> Columns;
            inputFile >> ConnectLimit;
            inputFile >> Players[0].Name;
            inputFile >> Players[0].Mark;
            inputFile >> Players[1].Name;
            inputFile >> Players[1].Mark;
            inputFile >> TurningPlayer;
            inputFile >> GameEndState;
            for (int i = 0; i < MaxCols; i++)
            {
                inputFile >> Moves_Made[i];
            }
            for (int r = 0; r < Rows; r++)
            {
                for (int c = 0; c < Columns; c++)
                {
                    inputFile >> Board.Board[r][c];
                }
            }
            return true;
        }
        return false;
    }

    void ShowGame()
    {
        /**
         This function must show the game on screen and
         if the game is not over take the next move from
         the next player and return that move.

         A very simple implementation is already provided
         to get you started but you better replace it with
         your code to produce a pleasing output
         */

         // Your better replace the following code

        for (int r = 0; r < Rows; r++)
        {
            cout << setw(Columns * 2);
            for (int c = 0; c < Columns; c++)
                cout
                << Board.Board[r][c] << " ";
            cout << endl;
        }
        if (GameisOn())
        {
            cout << endl
                << Players[TurningPlayer].Name
                << "("
                << Players[TurningPlayer].Mark << ")"
                << "Moving " << endl;
        }
    }

    void Play()
    {
        int Move;
        do
        {
            ShowGame();
            cout << "ENTER NEGATIVE NUMBER FOR THE MAIN MENU" << endl;
            cout << "Please Enter Your Move ? (0 - " << Columns - 1 << ") ";
            cin >> Move;
            if (Move < 0)
                break;
            MakeMove(Move);
        } while (GameisOn());
        if (Move >= 0)
            ShowGame();
    }
};

int Menu()
{
    int Choice;
    do
    {
        cout << "1. Play" << endl
            << "2. Save Game" << endl
            << "3. Load Game" << endl
            << "4. Exit" << endl
            << "Enter You Choice (1 - 4)";
        cin >> Choice;
    } while (Choice < 1 || Choice > 4);
    return Choice;
}

void initNewGame(Connect_N& C)
{
    int Rows, Cols, Limit;
    cout << "How Many Rows ?";
    cin >> Rows;
    cout << "How Many Cols ?";
    cin >> Cols;
    cout << "Connectivity Limit ?";
    cin >> Limit;
    C.Rows = Rows;
    C.Columns = Cols;
    C.ConnectLimit = Limit;
    cout << "First Player Name? ";
    string N;
    cin >> N;
    C.Players[0].setPlayerName(N);
    cout << "Second Player Name? ";
    cin >> N;
    C.Players[1].setPlayerMark('x');
    C.Players[1].setPlayerName(N);
}

int main()
{
    int Choice;
    bool GameNotLoaded = true;
    Connect_N Connect_Game(MaxRows, MaxCols, MaxRows);
    do
    {
        Choice = Menu();
        switch (Choice)
        {
        case 1:
            if (GameNotLoaded)
            {
                initNewGame(Connect_Game);
                GameNotLoaded = false;
            }
            Connect_Game.Play();
            break;
        case 2:
            if (!GameNotLoaded)
                Connect_Game.SaveGame("Mysave.txt");
            break;

        case 3:
            GameNotLoaded != Connect_Game.LoadSavedGame("Mysave.txt");
            break;
        }

    } while (Choice != 4);
    return 0;
}
