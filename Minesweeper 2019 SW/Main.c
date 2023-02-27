#include <stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

void main()
{
	// VARIABLES
	// Game
	int bombNum = 9, totalBombs = 10;
	int bombX = 0, bombY = 0, guessX = 0, guessY = 0;
	// Control
	int choice = 0, saveChoice = 0;
	bool loadingGame = false, bombHit = false;
	// Player Management
	int playerChoice = 1;
	int currentPlayer = 1, uncurrentPlayer = 2;
	int player1Score = 0, player2Score = 0;
	int tilesRevealed = 0;
	// Arrays
	int hidden[8][8]; // Hidden board, holds all the numbers of current game
	char visible[8][8]; // Visible board. Holds default chars until updated with nums
	// File Assign
	FILE* gameFile;

	// Fix for rand method issue
	srand(time(0));

	// Intro & choices
	printf("		 Welcome to Minesweeper! \n");
	printf("Choose 1 to start a new game, choose 2 to reload a previous\n");
	scanf("%d", &choice);

	printf("		   1 Player or 2 player? \n");
	scanf("%d", &playerChoice);

	// File Loading
	if (choice == 2)
	{
		// Load previous hidden array
		gameFile = fopen("gamefile.txt", "r");

		if (gameFile == NULL)
		{
			printf("		Sorry, no previous games saved \n");
		}
		else
		{
			printf("		Loading previous board \n");
			// Read in file
				for (int row = 0; row < 8; row++)
				{
					for (int col = 0; col < 8; col++)
					{
						fscanf(gameFile, " %d", &hidden[row][col]);
					}
				}

			// Closing file and setting control vars
			fclose(gameFile);
			choice = 1;
			loadingGame = true;
		}
	}
	
	// Board Creation
	while (choice == 1)
	{
		choice = 0;
		bombHit = false;
		if (loadingGame == false)
		{
			printf("		    Creating New Board! \n");

			// Sets all cells to 0 of hidden gameboard
			for (int row = 0; row < 8; row++)
			{
				for (int col = 0; col < 8; col++)
				{
					hidden[row][col] = 0;
				}
			}

			// Generate bombs within random cells of hidden
			for (int bomb = 0; bomb < totalBombs; bomb++)
			{
				bombX = rand();
				bombX = (bombX % 8);
				bombY = rand();
				bombY = (bombY % 8);
				while (hidden[bombX][bombY] == bombNum) // Checks for dupes and if so, regenerates
				{
					bombX = rand();
					bombX = (bombX % 8);
					bombY = rand();
					bombY = (bombY % 8);
				}
				hidden[bombX][bombY] = bombNum; // Assigns Bombs

				// Increase numbers around bombs, filling the board
				// Extra conditions present for error handling
				if(hidden[bombX][bombY-1] != bombNum && (bombY-1) != -1) // left
					hidden[bombX][bombY-1]++;
				if (hidden[bombX][bombY+1] != bombNum && (bombY+1) != 8) // right
					hidden[bombX][bombY+1]++;
				if (hidden[bombX-1][bombY] != bombNum && (bombX-1) != -1) // up
					hidden[bombX-1][bombY]++;
				if(hidden[bombX+1][bombY] != bombNum && (bombX+1) != 8) // down
					hidden[bombX+1][bombY]++;
				if (hidden[bombX-1][bombY-1] != bombNum && (bombX-1) != -1 && (bombY-1) != -1) // top left
					hidden[bombX-1][bombY-1]++;
				if (hidden[bombX-1][bombY+1] != bombNum && (bombX-1) != -1 && (bombY+1) != 8) // top right
					hidden[bombX-1][bombY+1]++;
				if (hidden[bombX+1][bombY-1] != bombNum && (bombX+1) != 8 && (bombY-1) != -1) // bottom left
					hidden[bombX+1][bombY-1]++;
				if (hidden[bombX+1][bombY+1] != bombNum && (bombX+1) != 8 && (bombY+1) != 8) // bottom right
					hidden[bombX+1][bombY+1]++;
			}
		}

		// sets up visible, the board the players see
		for (int row = 0; row < 8; row++)
		{
			for (int col = 0; col < 8; col++)
			{
				visible[row][col] = '*'; // fills with stars
			}
		}

		// Gameplay Loop
		do
		{
			if (playerChoice == 1)
			{
				// Singleplayer Guess
				printf("\n\n		Your turn! \nWhat square would you wish to reveal, type the coodinates (1-8, 1-8) \n");
				scanf("		%d %d", &guessX, &guessY);
			}
			else
			{
				// 2 Player Guess
				printf("\n\n		Player %d's turn \nWhat square would you wish to reveal, type the coordinates (1-8, 1-8) \n", currentPlayer);
				scanf("		%d %d", &guessX, &guessY);
			}

			// Insert Player Guess Position
			guessX = guessX - 1;
			guessY = guessY - 1;
			int guess = hidden[guessX][guessY];
			visible[guessX][guessY] = guess;

			// If Bomb is Hit
			if (visible[guessX][guessY] == bombNum)
			{
				if (playerChoice == 1)
				{
					printf("\nYou Hit a Bomb! You lose!\n");
				}
				printf("\n\nPlayer %d HIT A BOMB, Player %d Wins!\n", currentPlayer, uncurrentPlayer);
				bombHit = true;
			}

			// If 0 is Hit, tag touching squares with a #
			else if (visible[guessX][guessY] == 0)
			{
				printf("\nYou Hit a Zero, Touching Squares Opened\n");
				visible[guessX][guessY] = ' ';
				if((guessY-1) != -1)
					visible[guessX][guessY - 1] = '#'; // left
				if((guessY+1) != 8)
					visible[guessX][guessY + 1] = '#'; // right
				if((guessX-1) != -1)
					visible[guessX - 1][guessY] = '#'; // above
				if((guessX+1) != 8)
					visible[guessX + 1][guessY] = '#'; // below
			}

			// Displays the Current Board
			printf("\n		Current Board: \n");
			printf("  1    2    3    4    5    6    7    8\n");
			for (int row = 0; row < 8; row++)
			{
				printf("+---++---++---++---++---++---++---++---+\n");
				for (int col = 0; col < 8; col++)
				{
					// This display system prints the visible board out differently depending on what int equivalent to a char it reads
					if (visible[row][col] == 42) // For example: This if detects the int equivalent for star (42), so prints out the star 
					{
						printf("| %c |", '*');
						if (col == 7) {
							printf(" %d\n", row+1);
						}
					}
					else if (visible[row][col] == 32) // Changes 0 to blank space
					{
						printf("| %c |", ' ');
						if (col == 7) {
							printf(" %d\n", row+1);
						}
					}
					else if (visible[row][col] == 35) // Changes nums next to 0 (tagged with #) to a blank space
					{
						if (hidden[row][col] == 0)
						{
							printf("| %c |", ' ');
							if (col == 7) {
								printf(" %d\n", row+1);
							}
						}
						else {
							printf("| %d |", hidden[row][col]); // Changes nums next to 0 (tagged with #) to a number
							if (col == 7) {
								printf(" %d\n", row+1);
							}
						}
					}
					else
					{
						printf("| %d |", visible[row][col]); // If nothing else, gives num
						if (col == 7) {
							printf(" %d\n", row+1);
						}
					}
				}
			}
			printf("+---++---++---++---++---++---++---++---+\n");
			
			// Score Calculation
			if (visible[guessX][guessY] == bombNum)
			{
				guess -= 9;
			}
			else if (currentPlayer == 1)
			{
				player1Score += guess;
			}
			else if (currentPlayer == 2)
			{
				player2Score += guess;
			}
			if (playerChoice == 1)
			{
				// Score Display for Singleplayer
				printf("\nCurrent Score: %d\n", player1Score);
			}
			else
			{ // Score Display for 2 Players
				printf("\nCurrent Scores: \n");
				printf("Player 1: %d\n", player1Score);
				printf("Player 2: %d\n", player2Score);
			}
			// Player Swap
			if (currentPlayer == 1)
			{
				currentPlayer = 2;
				uncurrentPlayer = 1;
			}
			else if (currentPlayer == 2)
			{
				currentPlayer = 1;
				uncurrentPlayer = 2;
			}
			tilesRevealed++;

			// file saving
			printf("Choose 1 to save this game or 2 to keep playing \n");
			scanf("%d", &saveChoice);

			gameFile = fopen("gamefile.txt", "w");

			if (saveChoice == 1 && gameFile != NULL)
			{
				for (int row = 0; row < 8; row++)
				{
					for (int col = 0; col < 8; col++)
					{
						fprintf(gameFile, " %d", hidden[row][col]); // Writes hidden array to file
					}
				}
				printf("Game successfully saved!");
			}
			else if (gameFile != NULL)
			{
				fclose(gameFile); // close gameFile
			}
			else {
				printf("Next Round! \n");
			}

			// System to allow players to win by all tiles revealed
			if (tilesRevealed == 54)
			{
				if (playerChoice == 1)
				{
					printf("All squares revealed! You won with a score of %d!\n", player1Score);
				}
				else {
					printf("All squares revealed! Lets see who scored more!\n");
					if (player1Score > player2Score)
					{
						printf("Player 1 wins with a score of %d over player 2's score of %d\n", player1Score, player2Score);
					}
					else if (player2Score > player1Score)
					{
						printf("Player 2 wins with a score of %d over player 1's score of %d\n", player2Score, player1Score);
					}
				}
			}
		} while (bombHit == false);

		//End of game board revealed
		printf("\nFinal Board: \n");
		for (int row = 0; row < 8; row++)
		{
			printf("+---++---++---++---++---++---++---++---+\n");
			for (int col = 0; col < 8; col++)
			{
				printf("| %d |", hidden[row][col]);
				if (col == 7) {
					printf("\n");
				}
			}
		}
		printf("+---++---++---++---++---++---++---++---+\n");
		
		// Play again functionality
		printf("Type 1 to play again or 0 to exit\n");
		scanf("%d", &choice);
	}

	// Exit
	if (choice == 0)
	{
		printf("Thank you for playing! \n");
	}


}