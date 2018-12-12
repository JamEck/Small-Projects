#include <iostream>

using namespace std;

const int size = 15;
const int mineIcon = 88;
const int uncovTile = 43;
const int blank = 32;
const int flag = 70;

int mineCount = 0;
bool lose = false;

int getRandNum ();

void createArray (int[][size]);

void getMineInfo(int[][size]);

void createBoardSol (int[][size], char[][size]);

void createBoard (char [][size]);

void updateBoard (char [][size], char [][size], int, int, string);

void printArray (char[][size]);

string promptUpdate (int*, int*, string);

void loseMessage (char [][size]);

int main()
{
    int boardData[size][size];
    char boardSol[size][size];
    char board[size][size];
    
    srand(time(0));
    
    createArray(boardData);
    
    getMineInfo(boardData);
    
    createBoardSol(boardData, boardSol);

    createBoard(board);

    while (!lose)
    {
        int row, col;
        string action;
        
        //printArray(boardSol);
        printArray(board);
        
        action = promptUpdate(&row, &col, action);
       
        updateBoard (board, boardSol, row - 1, col - 1, action);
    }
    
    loseMessage(boardSol);
    
}

int getRandNum ()
{
    int num;
    num = rand() % 100;
    
    return num;
}

void createArray (int a[][size])
{
    mineCount = 0;
    
    int minePerc;
    cout << "Mine Percentage: ";
    cin >> minePerc;
    
    if (minePerc <= 0 || minePerc > 100)
    {
        cout << " Not a valid Mine Percentage; Setting to default of 10%";
        minePerc = 10;
    }

    for (int n = 0; n < size; n++)
    {
        for (int i = 0; i < size; i++)
        {
            if (getRandNum() < minePerc)
            {
                a[n][i] = 9;
                mineCount++;
            }
            else
            {
                a[n][i] = 0;
            }
        }
    }
}

void getMineInfo(int a[][size])
{
    for (int n = 0; n < size; n++)
    {
        for (int i = 0; i < size; i++)
        {
            if (a[n][i] == 9)
            {
                for(int nRel = -1; nRel <= 1; nRel++)
                {
                    for (int iRel = -1; iRel <= 1; iRel++)
                    {
                        if
                        (
                            n + nRel >= 0 &&
                            i + iRel >= 0 &&
                            n + nRel < size &&
                            i + iRel < size &&
                            a[n + nRel][i + iRel] != 9
                        )
                            a[n + nRel][i + iRel] += 1;
                    }
                }
            }
        }
    }
}

void createBoardSol (int a[][size], char b[][size])
{
    for (int n = 0; n < size; n++)
    {
        for (int i = 0; i < size; i++)
        {
            if(a[n][i] == 9)
                b[n][i] = mineIcon;
            else if(a[n][i] == 0)
                b[n][i] = blank;
            else
                b[n][i] = a[n][i] + 48;
        }
    }
}

void printArray (char a[][size])
{
    cout << endl;
    
    for (int n = -1; n < size; n++)
    {
        if (n > -1 && n < 9)                     // display vertical coordinates
            cout << n + 1 << "     ";
        else if (n >= 9)
            cout << n + 1 << "    ";
        
        for (int i = -1; i < size; i++)
        {
            if (n == -1)
            {
                if (i >= -1 && i < 8)           // display horizontal coordinates
                {
                    if (i == -1)
                        cout << "      ";
                    else
                        cout << i + 1 << "   ";
                }
                else if (i >= 8)
                    cout << i + 1 << "  ";
                
                if (i == size - 1)              // space between horizontal coordinates and first row
                    cout << endl;
            }
            
            else if (i > -1)                    // print table elements
                cout << a[n][i] << "   ";
        }
        
        if (n == 0)                             // display number of bombs
            cout << mineCount << " mines";
        
        cout << "\n\n";                         // vertical spacing
    }
}

void createBoard (char a[][size])
{
    for (int n = 0; n < size; n++)
    {
        for (int i = 0; i < size; i++)
        {
            a[n][i] = uncovTile;
        }
    }
}

void updateBoard (char a[][size], char b[][size], int row, int col, string action) // Coolest function of all time. It's true.
{
    if (action == "D" || action == "d") // If you're digging
    {
        if (a[row][col] != flag)        // If the tile is unflagged
        {
            bool uncovered = false;
            
            if (a[row][col] == b[row][col]) // If tile is uncovered
                uncovered = true;           // Make note
            else
                a[row][col] = b[row][col];  // Uncover the tile

            if (b[row][col] != mineIcon)    // If a mine is not under the tile
            {
                int flagCount = 0;
                
                for(int nRel = -1; nRel <= 1; nRel++) // loops for checking all eight spaces near a tile
                {
                    for (int iRel = -1; iRel <= 1; iRel++)
                    {
                        
                        if                           // If the nearby tile is inside the board
                        (
                            row + nRel >= 0 &&
                            col + iRel >= 0 &&
                            row + nRel < size &&
                            col + iRel < size
                        )
                        {
                            if (b[row][col] == blank)                                       // If underneath the center tile there is nothing
                            {
                                if (a[row + nRel][col + iRel] != b[row + nRel][col + iRel]) // If the nearby tile has not been uncovered
                                {
                                    if (b[row + nRel][col + iRel] == blank)                 // If underneath the nearby tile there is nothing
                                    {
                                        updateBoard (a, b, row + nRel, col + iRel, action); // Pass function with nearby tile as center tile
                                    }
                                    
                                    a[row + nRel][col + iRel] = b[row + nRel][col + iRel];  // Uncover nearby tile
                                }
                            }
                            else if (a[row + nRel][col + iRel] == flag)                     // If nearby tile is flagged
                            {
                                if (b[row + nRel][col + iRel] != mineIcon)                  // If underneath same nearby tile there is not a mine
                                    lose = true;
                                
                                flagCount++;
                            }
                        }
                    }
                }

                if (flagCount == b[row][col] - 48 && uncovered)          // If the flag count matches the value of the uncovered tile
                {
                    for(int nRel = -1; nRel <= 1; nRel++)
                    {
                        for (int iRel = -1; iRel <= 1; iRel++)
                        {
                            if (b[row + nRel][col + iRel] != mineIcon)                      // If underneath the nearby tile there is not a mine
                                a[row + nRel][col + iRel] = b[row + nRel][col + iRel];      // Uncover nearby tile
                            
                            if (b[row + nRel][col + iRel] == blank)                         // If underneath the nearby tile there is nothing
                            {
                                updateBoard (a, b, row + nRel, col + iRel, action);         // Pass function with nearby tile as center tile
                            }
                        }
                    }
                }
                else
                    lose = false;         // If the flag count does not match the value of the tile dug into, undo the loss
            }
            else
                lose = true;
        }
        else
            cout << "\nTile is Flagged: Unflag by flagging again.\n";
    }
    else if (a[row][col] != b[row][col])
    {
        if (a[row][col] == flag)
            a[row][col] = uncovTile;
        else
            a[row][col] = flag;
    }
    else
        cout << "\nUncovered tiles may not be flagged.\n";
}

string promptUpdate (int* row, int* col, string action)
{
    while (1)   // loop for inputting coordinates
    {
        cout << "              Row: ";
        cin >> *row;

        cout << "           Column: ";
        cin >> *col;

        if(*row < 1 || *row > size || *col < 1 || *col > size)
            cout << "\nNot a valid spot on the board.\nEnter again.\n";
        else
            break;
    }

    while (1)   // loop for inputting action (dig or flag)
    {
        cout << "Dig or Flag (D/F): ";

        cin >> action;
        
        if(action != "D" && action != "F" && action != "d" && action != "f")
            cout << "\nNot a valid input.(D/F)\n";
        else
            break;
    }
    
    return action;
}

void loseMessage (char b[][size])
{
    cout << "\t\t\nYOU LOSE\n";
    printArray(b);
    cout << "\t\t\nYOU LOSE\n";
}