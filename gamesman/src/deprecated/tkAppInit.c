/************************************************************************
**
** NAME:        tkAppInit.c
**
** DESCRIPTION: Code for creating .so files so that the wish will know
**              the C commands in Gamesman.
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 1999. All rights reserved.
**
** DATE:        1999-04-02
**
**************************************************************************/

#include "tk.h"
#include "gsolve.h"

extern STRING gValueString[];        /* The GAMESMAN Value strings */
extern BOOLEAN gStandardGame;
extern BOOLEAN kPartizan;
extern BOOLEAN kLoopy;

/*
 * The following variable is a special hack that is needed in order for
 * Sun shared libraries to be used for Tcl.
 */

extern int matherr();
int *tclDummyMathPtr = (int *) matherr;

/*************************************************************************
**
** Begin prototype for commands invoked from tcl command requests
**
**************************************************************************/

static int		FooCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		InitializeCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		InitializeDatabasesCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		GetComputersMoveCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		SetGameSpecificOptionsCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		DetermineValueCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		GetValueOfPositionCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		RemotenessCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		MexCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		DoMoveCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		PrimitiveCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		GetValueMovesCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
static int		RandomCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
 #ifdef COMPUTEC
static int		ComputeCCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));
 #endif
static int		GoAgainCmd _ANSI_ARGS_((ClientData clientData,
			    Tcl_Interp *interp, int argc, char **argv));

/************************************************************************
**
** NAME:        Gamesman_Init
**
** DESCRIPTION: This is the 
** 
** INPUTS:      Tcl_Interp *interp : Interpreter for application
**
** OUTPUTS:     TCL_OK : Confirmation that we exited successfully
**
** CALLS:       Tcl_CreateCommand
**
************************************************************************/

int
Gamesman_Init(interp)
    Tcl_Interp *interp;		/* Interpreter for application. */
{
    Tk_Window mainWindow;

    mainWindow = Tk_MainWindow(interp);
    Tcl_CreateCommand(interp, "foo", FooCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_Initialize", InitializeCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_InitializeDatabases", InitializeDatabasesCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_GetComputersMove", GetComputersMoveCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_SetGameSpecificOptions", SetGameSpecificOptionsCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_DetermineValue", DetermineValueCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_GetValueOfPosition", GetValueOfPositionCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_Remoteness", RemotenessCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_Mex", MexCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_DoMove", DoMoveCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_Primitive", PrimitiveCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_GetValueMoves", GetValueMovesCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
    Tcl_CreateCommand(interp, "C_Random", RandomCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
  #ifdef COMPUTEC
    Tcl_CreateCommand(interp, "C_ComputeC", ComputeCCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);
 #endif
    Tcl_CreateCommand(interp, "C_GoAgain", GoAgainCmd, (ClientData) mainWindow,
	    (void (*)()) NULL);

    {
#pragma weak GameSpecificTclInit
    int GameSpecificTclInit(Tcl_Interp *interp,Tk_Window);

    if (GameSpecificTclInit)
      GameSpecificTclInit(interp,mainWindow);
    }

    return TCL_OK;
}

/*************************************************************************
**
** Begin commands invoked from the tcl command requests coming in
**
**************************************************************************/

static int
FooCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
    if (argc != 4) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" x y z\"", (char *) NULL);
	return TCL_ERROR;
    }
    else {
	interp->result = "Dan Garcia";
	return TCL_OK;
    }
}

static int
InitializeCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  if (argc != 1) {
    interp->result = "wrong # args: shouldn't have any args";
    return TCL_ERROR;
  }
  else {
    Initialize();
    interp->result = "System Initialized";
    return TCL_OK;
  }
}

static int
InitializeDatabasesCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  if (argc != 1) {
    interp->result = "wrong # args: shouldn't have any args";
    return TCL_ERROR;
  }
  else {
    InitializeDatabases();
    interp->result = "Databases Initialized";
    return TCL_OK;
  }
}

static int
GetComputersMoveCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  MOVE GetComputersMove();
  POSITION position;

  if (argc != 2) {
    interp->result = "wrong # args: GetComputerMove (int)Position";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &position) != TCL_OK)
      return TCL_ERROR;

    sprintf(interp->result,"%d",(int)GetComputersMove(position));
    return TCL_OK;
  }
}

static int
SetGameSpecificOptionsCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int i, theOptions[100]; /* 100 other options should be enough! */
  
  if (argc < 2) {
    interp->result = "wrong # args: SetGameSpecificOptions (boolean)Standard-Game-p (optional)Other-Game-Specific-Options";
    return TCL_ERROR;
  }

  if(Tcl_GetInt(interp, argv[1], &gStandardGame) != TCL_OK)
    return TCL_ERROR;

  if (argc > 2)
    for(i=2;i<argc;i++)
      if(Tcl_GetInt(interp, argv[i], &theOptions[i-2]) != TCL_OK)
	return TCL_ERROR;

  SetTclCGameSpecificOptions(theOptions);

  return TCL_OK;
}

static int
DetermineValueCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int position;
  VALUE DetermineValue(), DetermineLoopyValue();

  if (argc != 2) {
    interp->result = "wrong # args: DetermineValue (int)Position";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &position) != TCL_OK)
      return TCL_ERROR;

    if(kLoopy)
      interp->result = gValueString[(int)DetermineLoopyValue(position)];
    else
      interp->result = gValueString[(int)DetermineValue(position)];

    return TCL_OK;
  }
}

static int
GetValueOfPositionCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int position;
  VALUE GetValueOfPosition();

  if (argc != 2) {
    interp->result = "wrong # args: GetValueOfPosition (int)Position";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &position) != TCL_OK)
      return TCL_ERROR;

    interp->result = gValueString[(int)GetValueOfPosition(position)];
    return TCL_OK;
  }
}

static int
RemotenessCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int position;
  REMOTENESS Remoteness();

  if (argc != 2) {
    interp->result = "wrong # args: Remoteness (int)Position";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &position) != TCL_OK)
      return TCL_ERROR;

    sprintf(interp->result,"%d",(int)Remoteness(position));
    return TCL_OK;
  }
}

static int
MexCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int position;
  REMOTENESS Remoteness();

  if (argc != 2) {
    interp->result = "wrong # args: Mex (int)Position";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &position) != TCL_OK)
      return TCL_ERROR;

    if(!kPartizan)
      MexFormat(position,interp->result);
    else
      sprintf(interp->result,"");
    return TCL_OK;
  }
}

static int
DoMoveCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int position, move;
  POSITION DoMove();

  if (argc != 3) {
    interp->result = "wrong # args: DoMove (int)Position (int)Move";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &position) != TCL_OK)
      return TCL_ERROR;
    if(Tcl_GetInt(interp, argv[2], &move) != TCL_OK)
      return TCL_ERROR;

    sprintf(interp->result,"%d",(int)DoMove(position,move));
    return TCL_OK;
  }
}

static int
GoAgainCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int position, move;
  extern BOOLEAN (*gGoAgain)(POSITION,MOVE);

  if (argc != 3) {
    interp->result = "wrong # args: GoAgain (int)Position (int)Move";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &position) != TCL_OK)
      return TCL_ERROR;
    if(Tcl_GetInt(interp, argv[2], &move) != TCL_OK)
      return TCL_ERROR;

    sprintf(interp->result,"%d",(int)gGoAgain(position,move));
    return TCL_OK;
  }
}

static int
PrimitiveCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int position;
  VALUE Primitive();

  if (argc != 2) {
    interp->result = "wrong # args: Primitive (int)Position";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &position) != TCL_OK)
      return TCL_ERROR;

    interp->result = gValueString[(int)Primitive(position)];
    return TCL_OK;
  }
}

static int
GetValueMovesCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int position;
  MOVE DoMove();
  VALUE GetValueOfPosition();
  MOVELIST *ptr, *head, *GenerateMoves();
  char theAnswer[10000], tmp[1000];

  if (argc != 2) {
    interp->result = "wrong # args: GetValueMoves (int)Position";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &position) != TCL_OK)
      return TCL_ERROR;

    head = ptr = GenerateMoves(position);
    theAnswer[0] = '\0';
    while (ptr != NULL) {
      sprintf(tmp,"{ %d %s } ",
	      (ptr->move), 
	      gValueString[GetValueOfPosition(DoMove(position,ptr->move))]);
      /* If this barfs, change 'char' to 'const char' */
      strcpy(theAnswer,(char *)strcat(theAnswer,tmp));
      ptr = ptr->next;
    }

    Tcl_SetResult(interp,theAnswer,TCL_VOLATILE);

    FreeMoveList(head);
    return TCL_OK;
  }
}

static int
RandomCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int GetRandomNumber();
  int n;

  if (argc != 2) {
    interp->result = "wrong # args: Random (int)n";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &n) != TCL_OK)
      return TCL_ERROR;

    sprintf(interp->result,"%d",GetRandomNumber(n));
    return TCL_OK;
  }
}

 #ifdef COMPUTEC
static int
ComputeCCmd(dummy, interp, argc, argv)
    ClientData dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    char **argv;			/* Argument strings. */
{
  int ComputeC();
  int n,k;

  if (argc != 3) {
    interp->result = "wrong # args: Random (int)n";
    return TCL_ERROR;
  }
  else {
    if(Tcl_GetInt(interp, argv[1], &n) != TCL_OK)
      return TCL_ERROR;
    if(Tcl_GetInt(interp, argv[2], &k) != TCL_OK)
      return TCL_ERROR;

    sprintf(interp->result,"%d",ComputeC(n,k));
    return TCL_OK;
  }
}
 #endif



