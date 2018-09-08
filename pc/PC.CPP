// PC - The Players Connection
//Copyright 1996 Aaron Brinton

#define DARKSIDE

#define cTitle "The Players Connection"
#define cCopyright1 "Copyright 1996 Aaron Brinton and Kevin Bentley"
#define cCopyright2 "Copyright 1996 Kevin Bentley and Aaron Brinton"
#define cVersion "0.70"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <dos.h>
#include <ctype.h>
#include <assert.h>

#pragma codeseg PC_DPACKAPI
#include "..\dpack\dpackapi.hpp"
#pragma codeseg PC_DCHAT
#include "..\dchat\dchat.hpp"
#pragma codeseg PC_TCMDLINE
#include "..\..\clib\tcmdline.hpp"
#pragma codeseg PC_TOUTWIN
#include "..\..\clib\toutwin.hpp"
#pragma codeseg PC_TLISTWIN
#include "..\..\clib\tlistwin.hpp"
#pragma codeseg PC_TCONFIG
#include "tconfig.hpp"
#pragma codeseg PC_TSCREEN
#include "tscreen.hpp"
#pragma codeseg PC_TAPP
#include "tapp.hpp"

#pragma codeseg PC_MAIN

void main(void) {
  randomize();
  int iReturn = 0;
  int iErrCode = 0;
  textmode(C4350);
  clrscr();
  TApp *App;
  App = new TApp; assert(App != NULL);
  iErrCode = App->Startup();
  if(iErrCode >= 0) {
    while(iReturn == 0) iReturn = App->Run();
    App->Shutdown();
    }
  delete App;
  textmode(C80);
  clrscr();
  printf("PC %s - The Players Connection\n", cVersion);
  if(random(100) < 50) printf(cCopyright1);
    else printf(cCopyright2);
  printf("\nThis is pre-release software, so don't expect much support.\n");
  if(iErrCode == -1) printf("\nCould not find PC.CFG!\a\n");
  if(iErrCode == -2) printf("\nCould not initialize IPX!\a\n");
  if(iReturn < 0) iReturn = 0;
  if(iReturn > 0) printf("\nExiting with errorlevel %d.\n", iReturn);
  exit(iReturn);
  }


