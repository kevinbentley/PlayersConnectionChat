// PC - Players Connection
// Aaron Brinton

#define cTitle "Player's Connection"
#define cCopyright1 "Copyright 1996 Aaron Brinton and Kevin Bentley"
#define cCopyright2 "Copyright 1996 Kevin Bentley and Aaron Brinton"
#define cVersion "0.70"

#include <assert.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>

#include "tapp.hpp"

void main(void)
{
  randomize();
  int iReturn = 0;
  int iErrCode = 0;
  textmode(C4350);
  clrscr();
  TApp *App;
  App = new TApp;
  assert(App != NULL);
  iErrCode = App->Startup();
  if (iErrCode >= 0)
  {
    while (iReturn == 0)
      iReturn = App->Run();
    App->Shutdown();
  }
  delete App;
  textmode(C80);
  clrscr();
  printf("GMON %s Network Game Monitor\n", cVersion);
  if (random(100) < 50)
    printf(cCopyright1);
  else
    printf(cCopyright2);
  printf("\nThis is pre-release software, so don't expect much support.\n");
  if (iErrCode == -1)
    printf("\nCould not find GMON.CFG!\a\n");
  if (iErrCode == -2)
    printf("\nCould not initialize IPX!\a\n");
  if (iReturn < 0)
    iReturn = 0;
  if (iReturn > 0)
    printf("\nExiting with errorlevel %d.\n", iReturn);
  exit(iReturn);
}
