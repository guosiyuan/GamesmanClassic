#include<stdio.h>
#include<ctype.h>
#include "gamesman.h"

extern STRING gValueString[];

int rows = 5 ;

int      gNumberOfPositions ;
POSITION gInitialPosition ;
POSITION gMinimalPosition ;
//POSITION gMinimalPosition = (1<<(ROWS*3)) - 1 ;

BOOLEAN  kPartizan           = FALSE ;
BOOLEAN  kDebugMenu          = FALSE;
BOOLEAN  kGameSpecificMenu   = TRUE;
BOOLEAN  kTieIsPossible      = FALSE;
BOOLEAN  kLoopy               = FALSE;
BOOLEAN  kDebugDetermineValue = FALSE;
STRING   kGameName           = "2-D Nim";

STRING   kHelpGraphicInterface = "Help Graphic Interface: Fill in later" ;
STRING   kHelpTextInterface    = "Help Text Interface: Fill in later" ;
STRING   kHelpOnYourTurn = "Help on your turn" ;
STRING   kHelpStandardObjective = "help standard objective" ;
STRING   kHelpReverseObjective = "reverse objective" ;
STRING   kHelpTieOccursWhen = "A tie is not possible in this game" ;
STRING   kHelpExample = "some really long thing<-Actually play a game,then copy/paste" ;

InitializeGame()
{
	// HERE, YOU SHOULD ASSIGN gNumberOfPositions and gInitialPosition
	gNumberOfPositions =(1<<(rows*3)) * 2 ;
	gInitialPosition = ((1<<(rows*3)) - 1) * 2;
	gMinimalPosition = gInitialPosition ;
}

// SUNIL: NOT WRITING
FreeGame()
{
}

// SUNIL: NOT WRITING
DebugMenu()
{
}

// SUNIL: NOT WRITING
GameSpecificMenu()
{
	char option ;
	int newrows ;

	do
	{
		printf("Currently you have %d rows. Rows can range between 1 and 9\n", rows) ;
		printf("Would you like to change it? ") ;
		fflush(stdin) ;
		scanf("%c", &option) ;
		fflush(stdin) ;
		option = toupper(option) ;
		if(option == 'Y')
		{
			do
			{
				printf("Enter new number of rows [1-9] : ") ;
				scanf("%d", &newrows) ;
			}while(newrows > 9 || newrows < 1) ;
			rows = newrows ;
		}
		else if(option != 'N')
		{
			printf("Invalid option. Please try again.\n") ;
		}
	}while(option != 'N') ;
}

// SUNIL: NOT WRITING
SetTclCGameSpecificOptions(int theOptions [])
{
}

#define max(a,b) (((a)>(b))?(a):(b))

// SUNIL: DONE
POSITION DoMove(POSITION thePosition, MOVE theMove)
{
	int * array ;
	int i ;
	int row ;	// the Row the move refers to (between 1 and ROWS)
	int num ;	// the number to remove from row specified
	int turnbit = thePosition % 2 ;

	thePosition /= 2 ;

	array = (int *)malloc(sizeof(int)*rows) ;

	for(i = 0 ; i < rows ; i++)
	{
		array[i] = thePosition % 8 ;
		thePosition = thePosition >> 3 ;
	}
	row = (theMove / 10) ;
	num = (theMove % 10) ;

	array[row-1] = array[row-1]-num ;

	thePosition = 0 ;

	for(i = rows - 1 ; i >= 0 ; i--)
	{
		thePosition = thePosition << 3 ;
		thePosition += array[i] ;
	}

	free(array) ;

	return thePosition*2+(1-turnbit) ;
}

GetInitialPosition()
{
}

// SUNIL: DONE
PrintComputersMove(MOVE computersMove, STRING computersName)
{
	printf("%s's move: %d\n", computersName, computersMove) ;
}

// SUNIL: DONE
VALUE Primitive(POSITION position)
{
	if(position == 0 || position == 1) return (gStandardGame?lose:win) ;
	return undecided ;
}

PrintPosition(POSITION position, STRING playerName, BOOLEAN usersTurn)
{
	int i, j ;
	int positionBak = position ;
	position /=2 ;
	printf("\n") ;
	for(i = 0 ; i < rows ; i++)
	{
		printf("%d: ", i+1) ;
		for(j = 0 ; j < position % 8 ; j++)
			printf("O ") ;
		printf("\n") ;
		position = position >> 3 ;
	}
	printf("\n%s\n\n", GetPrediction(positionBak, playerName, usersTurn)) ;
}

MOVELIST *GenerateMoves(POSITION position)
{
	// Here, use head = CreateMovelistNode(move,head) ;
	// then return head when done
	MOVELIST *CreateMovelistNode(), *head = NULL;
	int i, j ;

	if(Primitive(position) == undecided)
	{
		position /= 2 ;
		for(i = 0 ; i < rows ; i++)
		{
			int piecesinrow = position % 8 ;
			position = position >> 3 ;
			for(j = 1 ; j <= piecesinrow ; j++)
				head = CreateMovelistNode( (MOVE)((i+1)*10 + j), head) ;
		}
	}

	return head ;
}

USERINPUT GetAndPrintPlayersMove(POSITION thePosition, MOVE * theMove, STRING playerName)
{
	USERINPUT ret ;
	do
	{
		printf("%8s's move [[1-%d][1-7]] :  ", playerName, rows);
		ret = HandleDefaultTextInput(thePosition, theMove, playerName); 
		if(ret != Continue)
			return(ret);
	}while(TRUE);
	return(Continue);
}

BOOLEAN ValidTextInput(STRING input)
{
	int i ;
	for(i = 0 ; i < strlen(input) ; i++)
		if(!isdigit(input[i]))
			return FALSE ;
	return TRUE ;
}

MOVE ConvertTextInputToMove(STRING input)
{
	int ret ;
	sscanf(input, "%d", &ret) ;
	return ret ;
}

PrintMove(MOVE theMove)
{
	printf("%d", theMove) ;
}

STRING kDBName = "nim" ;

int NumberOfOptions()
{    
	return 2 ;
} 

int getOption()
{
	if(gStandardGame)
	{
		return 1 ;
	}
	return 2 ;
} 

void setOption(int option)
{
	if(option == 2)
	{
		gStandardGame = FALSE ;
	}
	else
	{
		gStandardGame = TRUE ;
	}
}

int GameSpecificTclInit(Tcl_Interp* interp,Tk_Window mainWindow) {}
