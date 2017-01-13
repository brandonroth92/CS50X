/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        } 
        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // Starting tile number (highest number) is one less than total grid dimension due to empty space
    int tileNumber = (d*d) - 1;
  
    // Row
    for (int i = 0; i < d; i++)
    {
        // Column
        for (int j = 0; j < d; j++)
        {
            board[i][j] = tileNumber;
            tileNumber--;
        }
    }
    
    /* Check if size of grid row is even
     * If even, swap "2" and "1" tiles to make it possible to win game
     */
    if (d % 2 == 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Row
    for (int i = 0; i < d; i++)
    {
        // Column
        for (int j = 0; j < d; j++)
        {
            // Print an underscore instead of 0 to indicate blank tile
            if (board[i][j] == 0)
            {
                printf("  _");
            
            // Otherwise, print value of tileNumber in each grid space
            } else
            {
                printf(" %2i", board[i][j]);
            }
        }
    // Print newline at the end of each row
    printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // Declare tracking variables to keep track of selected tile and blank tile locations
    int tileRow, tileCol, blankRow, blankCol;
    
    // Row
    for (int i = 0; i < d; i++)
    {
        //Column
        for (int j = 0; j < d; j++)
        {
            // Put tileNumber location into row and col variables
            if (board[i][j] == tile)
            {
                tileRow = i;
                tileCol = j;
            }
          
            // Put blank tile location into row and col variables
            if (board[i][j] == 0)
            {
                blankRow = i;
                blankCol = j;
            }
        }
    }
    
    // Check if valid tile move based on rules of game and swap tiles if appropriate
    if ((tileRow == blankRow && (tileCol == blankCol - 1 || tileCol == blankCol + 1)) || (tileCol == blankCol && (tileRow == blankRow - 1 || tileRow == blankRow + 1)))
    {
        board[blankRow][blankCol] = board[tileRow][tileCol];
        board[tileRow][tileCol] = 0;
        return true;
    }
  
    // If tile move is not valid, return false for main function to print error
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    /*
     * The game is won when numbers are in ascending order, starting with 1
     * Initialize a count variable set to 1
     * As we iterate over each grid square, we add 1 to the counter variable
     * The counter should equal the tileNumber in the grid square, since the tiles should be in ascending order
     */
    int counter = 1;
    
    //row
    for (int i = 0; i < d; i++)
    {
        //column
        for (int j = 0; j < d; j++)
        {
            /*
             * Set counter to 0 if iterating over blank tile
             * On win condition, blank tile is final square and counter must equal the value of the grid square (0)
             */
            if (board[i][j] == 0)
            {
                counter = 0;
            }
          
            // Check if tileNumber in grid location is equal to counter, otherwise return false
            if (board[i][j] != counter)
            {
                return false;
            }
        
        // Add 1 to counter variable as each grid square is iterated over
        counter++;
        }
    }
    return true;
}