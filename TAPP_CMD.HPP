//GMON TApp Command Handling
//Copyright 1996 Aaron Brinton

void TApp::HandleCommand(char *szCommand) {
  if(szCommand[0] == '/') {
    int iValid = 0;
    char *szTemp = new char[300]; assert(szTemp != NULL);
    char *lpszVerb;
    char *lpszParms;
    strcpy(szTemp, szCommand);
    lpszVerb = strtok(szTemp, " ");
    lpszParms = strchr(lpszVerb, 0);
    if(strcmp(lpszVerb, szCommand) != 0) lpszParms++;
    lpszVerb++;
    delete(szTemp);
    //general commands
    if(stricmp(lpszVerb, Config->Command[ 0]) == 0) {CMD_Quit(-1, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[ 1]) == 0) {CMD_Quit(1, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[ 2]) == 0) {CMD_Quit(2, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[ 3]) == 0) {CMD_Quit(3, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[ 4]) == 0) {CMD_Quit(4, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[ 5]) == 0) {CMD_Quit(5, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[ 6]) == 0) {CMD_Quit(6, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[ 7]) == 0) {CMD_Quit(7, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[ 8]) == 0) {CMD_Quit(8, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[ 9]) == 0) {CMD_Quit(9, lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[10]) == 0) {CMD_Help(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[11]) == 0) {CMD_Chat(); goto SKIP;}
    //kali commands
    if(stricmp(lpszVerb, Config->Command[12]) == 0) {CMD_Users(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[13]) == 0) {CMD_Email(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[14]) == 0) {CMD_Whois(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[15]) == 0) {CMD_Servers(); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[16]) == 0) {CMD_Motd(); goto SKIP;}
    //chat commands
    if(stricmp(lpszVerb, Config->Command[17]) == 0) {CMD_Away(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[18]) == 0) {CMD_Beep(); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[19]) == 0) {CMD_Clear(); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[20]) == 0) {CMD_Ignore(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[21]) == 0) {CMD_Join(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[22]) == 0) {CMD_Me(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[23]) == 0) {CMD_Msg(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[24]) == 0) {CMD_Ping(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[25]) == 0) {CMD_Who(lpszParms); goto SKIP;}
    //file commands
    if(stricmp(lpszVerb, Config->Command[26]) == 0) {CMD_Offer(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[27]) == 0) {CMD_Deoffer(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[28]) == 0) {CMD_Get(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[29]) == 0) {CMD_Get(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[30]) == 0) {CMD_Progress(); goto SKIP;}
    //game commands
//    if(stricmp(lpszVerb, "d1") == 0) {CMD_D1(); goto SKIP;}
//    if(stricmp(lpszVerb, "d2") == 0) {CMD_D2(); goto SKIP;}
//    if(stricmp(lpszVerb, "list") == 0) {CMD_List(lpszParms); goto SKIP;}
//    if(stricmp(lpszVerb, "game") == 0) {CMD_Game(lpszParms); goto SKIP;}
    //dos commands
    if(stricmp(lpszVerb, Config->Command[31]) == 0) {sprintf(szTemp, "cd %s", lpszParms); CMD_Dos(szTemp); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[32]) == 0) {sprintf(szTemp, "dir %s", lpszParms); CMD_Dos(szTemp); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[33]) == 0) {sprintf(szTemp, "type %s", lpszParms); CMD_Dos(szTemp); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[34]) == 0) {CMD_Shell(lpszParms); goto SKIP;}
    //undocumented
    if(stricmp(lpszVerb, Config->Command[35]) == 0) {CMD_NoPrivate(lpszParms); goto SKIP;}
    if(stricmp(lpszVerb, Config->Command[36]) == 0) {CMD_Dos(lpszParms); goto SKIP;}
#ifdef DARKSIDE
    //debug commands
    if(stricmp(lpszVerb, "debug"   ) == 0) {CMD_Debug(); goto SKIP;}
    if(stricmp(lpszVerb, "darkside") == 0) {CMD_DarkSide(); goto SKIP;}
    if(stricmp(lpszVerb, "allmsgs" ) == 0) {CMD_AllMsgs(); goto SKIP;}
#endif
    if(iValid == 0) OutWin->AddWrap(Config->String[cMaxStrings - 2], Config->Color[cMaxColors - 1]);
    SKIP:
    } else if(strcmp(szCommand, "") != 0) SendText(szCommand);
  }

void TApp::HandleHotKey(int iHotKey) {
  char *szTemp = new char[50]; assert(szTemp != NULL);
  switch(iHotKey) {
    case  -9: sprintf(szTemp, "/%s %s ", Config->Command[23], szLastMsgNick); CmdLine->SetText(szTemp); break; //backspace
    case -27: CmdLine->Clear(); break; //escape
    case  16: HandleCommand(Config->Macro[26]); break; //alt-q
    case  17: HandleCommand(Config->Macro[32]); break; //alt-w
    case  18: HandleCommand(Config->Macro[14]); break; //alt-e
    case  19: HandleCommand(Config->Macro[27]); break; //alt-r
    case  20: HandleCommand(Config->Macro[29]); break; //alt-t
    case  21: HandleCommand(Config->Macro[34]); break; //alt-y
    case  22: HandleCommand(Config->Macro[30]); break; //alt-u
    case  23: HandleCommand(Config->Macro[18]); break; //alt-i
    case  24: HandleCommand(Config->Macro[24]); break; //alt-o
    case  25: HandleCommand(Config->Macro[25]); break; //alt-p
    case  30: HandleCommand(Config->Macro[10]); break; //alt-a
    case  31: HandleCommand(Config->Macro[28]); break; //alt-s
    case  32: HandleCommand(Config->Macro[13]); break; //alt-d
    case  33: HandleCommand(Config->Macro[15]); break; //alt-f
    case  34: HandleCommand(Config->Macro[16]); break; //alt-g
    case  35: HandleCommand(Config->Macro[17]); break; //alt-h
    case  36: HandleCommand(Config->Macro[19]); break; //alt-j
    case  37: HandleCommand(Config->Macro[20]); break; //alt-k
    case  38: HandleCommand(Config->Macro[21]); break; //alt-l
    case  44: HandleCommand(Config->Macro[35]); break; //alt-z
    case  45: HandleCommand(Config->Macro[33]); break; //alt-x
    case  46: HandleCommand(Config->Macro[12]); break; //alt-c
    case  47: HandleCommand(Config->Macro[31]); break; //alt-v
    case  48: HandleCommand(Config->Macro[11]); break; //alt-b
    case  49: HandleCommand(Config->Macro[23]); break; //alt-n
    case  50: HandleCommand(Config->Macro[22]); break; //alt-m
    case  73: OutWin->PageUp(20); break; //pgup
    case  81: OutWin->PageDown(20); break; //pgdn
    case 118: ListWin->PageDown(1); break; //ctrl-pgdn
    case 120: case 121: case 122: case 123: case 124: case 125: case 126: case 127: case 128: case 129:
              HandleCommand(Config->Macro[iHotKey - 120]); break; //alt-1 to alt-0
    case 132: ListWin->PageUp(1); break; //ctrl-pgup
    }
  delete(szTemp);
  }

//***************************************************************************

void TApp::CMD_AllMsgs(void) {
#ifdef DARKSIDE
  Config->AllMsgs = 1 - Config->AllMsgs;
  if(Config->AllMsgs == 0) OutWin->AddWrap("You are no longer viewing all messages!", Config->DarkSideColor);
    else OutWin->AddWrap("You are now viewing all messages!", Config->DarkSideColor);
#endif
  }

void TApp::CMD_Chat(void) {
#ifdef GAMEMON
  if(iMode == 1) delete(D1Game);
  if(iMode == 2) delete(D2Game);
#endif
  iMode = 0;
  DrawScreen(0, 4);
  }

void TApp::CMD_Clear(void) {
  OutWin->Clear();
  }

void TApp::CMD_DarkSide(void) {
#ifdef DARKSIDE
  Config->DarkSide = 1 - Config->DarkSide;
  if(Config->DarkSide == 0) OutWin->AddWrap("You have abandoned the dark side of the force!", Config->DarkSideColor);
    else OutWin->AddWrap("You have turned to the dark side of the force!", Config->DarkSideColor);
#endif
  }

void TApp::CMD_Debug(void) {
#ifdef DARKSIDE
  Config->Debug = 1 - Config->Debug;
  if(Config->Debug == 0) OutWin->AddWrap("You have left debug mode!", Config->DarkSideColor);
    else OutWin->AddWrap("You have entered debug mode!", Config->DarkSideColor);
#endif
  }

void TApp::CMD_Help(char *szTopic) {
  int iTopic = 0;
  if(stricmp(szTopic, "edit") == 0) iTopic = 1;
  if(stricmp(szTopic, "kali") == 0) iTopic = 2;
  if(stricmp(szTopic, "chat") == 0) iTopic = 3;
  if(stricmp(szTopic, "file") == 0) iTopic = 4;
  if(stricmp(szTopic, "game") == 0) iTopic = 5;
  if(stricmp(szTopic, "dos")  == 0) iTopic = 6;

#ifdef GAMEMON
  if(iMode == 1) delete(D1Game);
  if(iMode == 2) delete(D2Game);
#endif

  iMode = 0;
  switch(iTopic) {
    case  1: DrawScreen(2, 10); break;
    case  2: DrawScreen(3, 10); break;
    case  3: DrawScreen(4, 14); break;
    case  4: DrawScreen(5, 10); break;
    case  5: DrawScreen(6,  4); break;
    case  6: DrawScreen(7,  9); break;
    default: DrawScreen(1, 14); break;
    }
  }

void TApp::CMD_Ignore(char *szBadNick) {
  char *szMessage;
  char *szList[20];
  char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
  int iStatus;
  for(int i = 0; i < 20; i++) szList[i] = new char[20]; assert(szList[i] != NULL);

  int iCount = ParseRecipients(szBadNick, &szMessage, &szList[0]);
  for(i = 0; i < iCount; i++) {
    if(szList[i][0] == 49) {
      iStatus = KChat->IgnoreUser(&szList[i][1]);
      if(iStatus == 0) sprintf(szLine2Add, Config->String[9], "accepting", &szList[i][1]);
      if(iStatus > 0) sprintf(szLine2Add, Config->String[9], "rejecting", &szList[i][1]);
      OutWin->AddWrap(szLine2Add, Config->Color[9]);
      } else {
        sprintf(szLine2Add, Config->String[cMaxStrings - 1], &szList[i][1]);
        OutWin->AddWrap(szLine2Add, Config->Color[cMaxColors - 1]);
        }
    delete(szList[i]);
    }
  if(iCount == 0) OutWin->AddWrap(Config->String[cMaxStrings - 2], Config->Color[cMaxColors - 1]);
  delete(szLine2Add);
  }

void TApp::CMD_Join(char *szNewChannel) {
  char *szMsg = new char[300]; assert(szMsg != NULL);
  if(szNewChannel[0] == 35) {
    if(strlen(szNewChannel) < 16) {
      if(strcmp(Config->Channel, szNewChannel) == 0) {
        sprintf(szMsg, Config->String[cMaxStrings - 3], szNewChannel);
        OutWin->AddWrap(szMsg, Config->Color[cMaxColors - 1]);
        }
        else {
          if(Config->DarkSide) {
#ifdef DARKSIDE
            KChat->SetChannel(szNewChannel);
            sprintf(szMsg, "You have moved silently to channel %s.", szNewChannel);
            OutWin->AddWrap(szMsg, Config->DarkSideColor);
#endif
            } else {
              KChat->Part();
              KChat->AnnounceMe(Config->Nickname, szNewChannel);
              sprintf(szMsg, Config->String[1], Config->Nickname, szNewChannel);
              OutWin->AddWrap(szMsg, Config->Color[1]);
              }
          }
      strcpy(Config->Channel, szNewChannel);
      } else {
        sprintf(szMsg, Config->String[cMaxStrings - 4], szNewChannel);
        OutWin->AddWrap(szMsg, Config->Color[cMaxColors - 1]);
        }
    } else {
      sprintf(szMsg, Config->String[cMaxStrings - 4], szNewChannel);
      OutWin->AddWrap(szMsg, Config->Color[cMaxColors - 1]);
      }
  delete(szMsg);
  }

void TApp::CMD_Me(char *szMessage) {
  char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
  sprintf(szLine2Add, Config->String[5], Config->Nickname, szMessage);
  OutWin->AddWrap(szLine2Add, Config->Color[5]);
  KChat->SendPostureMessage(szMessage);
  delete(szLine2Add);
  }

void TApp::CMD_Msg(char *szParms) {
  char *szMessage;
  char *szList[20];
  char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
  for(int i = 0; i < 20; i++) szList[i] = new char[20]; assert(szList[i] != NULL);
  int iCount = ParseRecipients(szParms, &szMessage, &szList[0]);
  for(i = 0; i < iCount; i++) {
    if(szList[i][0] == 49) {
      KChat->SendPrivateMessage(szMessage, &szList[i][1]);
      sprintf(szLine2Add, Config->String[10], &szList[i][1], szMessage);
      OutWin->AddWrap(szLine2Add, Config->Color[10]);
      strcpy(szLastMsgNick, &szList[i][1]);
      } else {
        sprintf(szLine2Add, Config->String[cMaxStrings - 1], &szList[i][1]);
        OutWin->AddWrap(szLine2Add, Config->Color[cMaxColors - 1]);
        }
    delete(szList[i]);
    }
  if(iCount == 0) OutWin->AddWrap(Config->String[cMaxStrings - 2], Config->Color[cMaxColors - 1]);
  delete(szLine2Add);
  }

void TApp::CMD_NoPrivate(char *szMessage) {
  char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
  if(strlen(szMessage) > 0) {
    char *szTemp = new char[300]; assert(szTemp != NULL);
    sprintf(szTemp, " (%s)", szMessage);
    sprintf(szLine2Add, Config->String[11], "rejecting", szTemp);
    OutWin->AddWrap(szLine2Add, Config->Color[11]);
    sprintf(szLine2Add, Config->String[cMaxStrings - 5], szMessage);
    KChat->NoPrivate(szLine2Add);
    delete(szTemp);
    } else {
      sprintf(szLine2Add, Config->String[11], "accepting", "");
      OutWin->AddWrap(szLine2Add, Config->Color[11]);
      KChat->NoPrivate(NULL);
      }
  delete(szLine2Add);
  }

void TApp::CMD_Ping(char *szParms) {
  if((strcmp(szParms, "*") == 0) || (strcmp(szParms, "") == 0)) KChat->Ping(NULL);
    else if(KChat->Ping(szParms) == 0) {
      char *szTemp = new char[300]; assert(szTemp != NULL);
      sprintf(szTemp, Config->String[cMaxStrings - 1], szParms);
      OutWin->AddWrap(szTemp, Config->Color[cMaxColors - 1]);
      delete(szTemp);
      }
  }

void TApp::CMD_Quit(int iErrLevel, char *szMessage) {
  iDone = iErrLevel;
  if(strlen(szMessage) > 0) strcpy(Config->ByeMessage, szMessage);
  }

void TApp::CMD_Who(char *szWhoChannel) {
  if(strlen(szWhoChannel) > 0) KChat->Who(szWhoChannel);
    else KChat->Who(Config->Channel);
  }



//*************************************************************************
//*************************************************************************
//*************************************************************************
//*************************************************************************
//*************************************************************************

void TApp::CMD_Away(char *szMessage) {
  char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
  if(strlen(szMessage) > 0) {
    sprintf(szLine2Add, "* %s is away (%s).", Config->Nickname, szMessage);
    OutWin->AddWrap(szLine2Add, Config->Color[5]);
    sprintf(szLine2Add, "is away (%s).", szMessage);
    KChat->Away(szLine2Add);
    KChat->SendPostureMessage(szLine2Add);
    } else {
      sprintf(szLine2Add, "* %s is back.", Config->Nickname, szMessage);
      OutWin->AddWrap(szLine2Add, Config->Color[5]);
      KChat->Away(NULL);
      sprintf(szLine2Add, "is back.", szMessage);
      KChat->SendPostureMessage(szLine2Add);
      }
  delete(szLine2Add);
  }


void TApp::CMD_Dos(char *szParms) {
  spawnlp(P_WAIT, "command", "command", "/c", szParms, "> data.tmp", NULL);
  FILE *ListFile;
  ListFile = fopen("data.tmp", "rt");
  char *szTemp = new char[300]; assert(szTemp != NULL);
  OutWin->AddWrap(" ", Config->DosColor);
  if(ListFile != NULL)
    while(fgets(szTemp, 250, ListFile) != NULL) {
      szTemp[strlen(szTemp) - 1] = 0;
      OutWin->AddWrap(szTemp, Config->DosColor);
      }
  fclose(ListFile);
  remove("data.tmp");
  OutWin->AddWrap(" ", Config->DosColor);
  delete(szTemp);
  }



void TApp::CMD_Users(char *szSearchNick) {
  if(!iKaliLoaded) {
    OutWin->AddWrap(Config->String[cMaxStrings - 6], Config->Color[cMaxColors - 1]);
    return;
    }
  spawnlp(P_WAIT, "command", "command", "/c", "kali /s > data.tmp", NULL);
  FILE *ListFile;
  ListFile = fopen("data.tmp", "rt");
  char *szTemp = new char[300]; assert(szTemp != NULL);
  char *szAdjust = new char[300]; assert(szAdjust != NULL);
  char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
  OutWin->AddWrap(" ", Config->UsersColor);
  OutWin->AddLine("### Nickname        Serial Process        Time  Ping  Loss", Config->UsersColor);
  sprintf(szTemp, "%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c  %c%c%c%c  %c%c%c%c  %c%c%c%c", 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196);
  OutWin->AddLine(szTemp, Config->UsersColor);
  if(ListFile != NULL)
    while(fgets(szTemp, 250, ListFile) != NULL) {
      strcpy(szAdjust, szTemp);
      if(!isdigit(szTemp[2])) sprintf(szTemp, " %s", szAdjust);
      if(isdigit(szTemp[2])) {
        char *szNum = &szTemp[0]; szNum[3] = 0;
        int iNum = (int)strtol(szNum, NULL, 0);
        char *szNick = &szTemp[4]; szTemp[19] = 0;
        char *szSerial = &szTemp[20]; szTemp[26] = 0;
        char *szProcess = &szTemp[49]; szTemp[62] = 0;
        char *szTime = &szTemp[63]; szTemp[68] = 0;
        char *szPing = &szTemp[69]; szTemp[74] = 0;
        char *szLoss = &szTemp[75]; szTemp[79] = 0;
        sprintf(szLine2Add, "%s %s %s %s  %s %s %s", szNum, szNick, szSerial, szProcess, szTime, szPing, szLoss);
        int iWrite = 1;
        if(strlen(szSearchNick) > 0) if(stricmp(szNick, szSearchNick) != 0) iWrite = 0;
        if(iWrite) OutWin->AddWrap(szLine2Add, Config->UsersColor);
        }
      }
  fclose(ListFile);
  remove("data.tmp");
  delete(szTemp);
  delete(szAdjust);
  delete(szLine2Add);
  }

void TApp::CMD_Whois(char *szSearchNick) {
  if(!iKaliLoaded) {
    OutWin->AddWrap(Config->String[cMaxStrings - 6], Config->Color[cMaxColors - 1]);
    return;
    }
  char *szTemp = new char[300]; assert(szTemp != NULL);
  sprintf(szTemp, "kali /w %s > data.tmp", szSearchNick);
  spawnlp(P_WAIT, "command", "command", "/c", szTemp, NULL);
  FILE *ListFile;
  ListFile = fopen("data.tmp", "rt");
  OutWin->AddWrap(" ", Config->WhoisColor);
  if(ListFile != NULL)
    while(fgets(szTemp, 250, ListFile) != NULL) {
      szTemp[strlen(szTemp) - 1] = 0;
      OutWin->AddWrap(szTemp, Config->WhoisColor);
      }
  fclose(ListFile);
  remove("data.tmp");
  OutWin->AddWrap(" ", Config->WhoisColor);
  delete(szTemp);
  }


void TApp::CMD_Email(char *szSearchNick) {
  if(!iKaliLoaded) {
    OutWin->AddWrap(Config->String[cMaxStrings - 6], Config->Color[cMaxColors - 1]);
    return;
    }
  spawnlp(P_WAIT, "command", "command", "/c", "kali /n > data.tmp", NULL);
  FILE *ListFile;
  ListFile = fopen("data.tmp", "rt");
  char *szTemp = new char[300]; assert(szTemp != NULL);
  char *szAdjust = new char[300]; assert(szAdjust != NULL);
  char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
  OutWin->AddWrap(" ", Config->EmailColor);
  OutWin->AddWrap("### Nickname        Email", Config->UsersColor);
  sprintf(szTemp, "%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196, 196);
  OutWin->AddWrap(szTemp, Config->EmailColor);
  if(ListFile != NULL)
    while(fgets(szTemp, 250, ListFile) != NULL) {
      strcpy(szAdjust, szTemp);
      if(!isdigit(szTemp[2])) sprintf(szTemp, " %s", szAdjust);
      if(isdigit(szTemp[2])) {
        char *szNum = &szTemp[0]; szNum[3] = 0;
        int iNum = (int)strtol(szNum, NULL, 0);
        char *szNick = &szTemp[3]; szTemp[18] = 0;

        char *szSerial = &szTemp[19]; szTemp[25] = 0;
        char *szProcess = &szTemp[48]; szTemp[61] = 0;
        char *szTime = &szTemp[62]; szTemp[67] = 0;
        char *szPing = &szTemp[68]; szTemp[73] = 0;
        char *szLoss = &szTemp[74]; szTemp[78] = 0;
        sprintf(szLine2Add, "%s %s %s %s  %s %s  %s", szNum, szNick, szSerial, szProcess, szTime, szPing, szLoss);
        int iWrite = 1;
        if(strlen(szSearchNick) > 0) if(stricmp(szNick, szSearchNick) != 0) iWrite = 0;
        if(iWrite) OutWin->AddWrap(szLine2Add, Config->EmailColor);
        }
      }
  fclose(ListFile);
  remove("data.tmp");
  OutWin->AddWrap(" ", Config->EmailColor);
  CmdLine->Update();
  delete(szTemp);
  delete(szAdjust);
  delete(szLine2Add);
  }

void TApp::CMD_Servers(void) {
  spawnlp(P_WAIT, "command", "command", "/c", "kali /a > data.tmp", NULL);
  FILE *ListFile;
  ListFile = fopen("data.tmp", "rt");
  char *szTemp = new char[300]; assert(szTemp != NULL);
  OutWin->AddWrap(" ", Config->ServersColor);
  if(ListFile != NULL)
    while(fgets(szTemp, 250, ListFile) != NULL) {
      szTemp[strlen(szTemp) - 1] = 0;
      OutWin->AddWrap(szTemp, Config->ServersColor);
      }
  fclose(ListFile);
  remove("data.tmp");
  OutWin->AddWrap(" ", Config->ServersColor);
  CmdLine->Update();
  delete(szTemp);
  }

void TApp::CMD_Motd(void) {
  spawnlp(P_WAIT, "command", "command", "/c", "kali /m > data.tmp", NULL);
  FILE *ListFile;
  ListFile = fopen("data.tmp", "rt");
  char *szTemp = new char[300]; assert(szTemp != NULL);
  OutWin->AddWrap(" ", Config->MotdColor);
  if(ListFile != NULL)
    while(fgets(szTemp, 250, ListFile) != NULL) {
      szTemp[strlen(szTemp) - 1] = 0;
      OutWin->AddWrap(szTemp, Config->MotdColor);
      }
  fclose(ListFile);
  remove("data.tmp");
  OutWin->AddWrap(" ", Config->MotdColor);
  delete(szTemp);
  }

void TApp::CMD_Progress(void) {
#ifdef GAMEMON
  if(iMode == 1) delete(D1Game);
  if(iMode == 2) delete(D2Game);
#endif
  iMode = 100;
  DrawScreen(10, 16);
  }

void TApp::CMD_D1(void) {
#ifdef GAMEMON
  if(iMode == 1) delete(D1Game);
  if(iMode == 2) delete(D2Game);
  iMode = 1;
  iUpdate = Config->UpdateInterval;
  D1Game = new TD1Game; assert(D1Game != NULL);
  D1Game->Startup();
  D1Game->SetSocket(0);
  D1Game->DrawList();
  DrawScreen(0, 15);
#endif
  }

void TApp::CMD_D2(void) {
#ifdef GAMEMON
  if(iMode == 1) delete(D1Game);
  if(iMode == 2) delete(D2Game);
  iMode = 2;
  iUpdate = Config->UpdateInterval;
//  D2Game = new TD2Game;
//  D2Game->Startup();
  DrawScreen(0, 4);
#endif
  }

void TApp::CMD_List(char *szSocket) {
  iUpdate = Config->UpdateInterval;
  int iNewSocket = (int)strtol(szSocket, NULL, 0);
  switch(iMode) {
    case  1: if(abs(iNewSocket) > Config->MaxSockets) OutWin->AddWrap(Config->String[cMaxStrings - 7], Config->Color[cMaxColors - 1]);
               else {
#ifdef GAMEMON
                 D1Game->SetSocket(iNewSocket);
                 D1Game->DrawList();
                 DrawScreen(0, 15);
#endif
                 }
             break;
    case  2: if(abs(iNewSocket) > Config->MaxSockets) OutWin->AddWrap(Config->String[cMaxStrings - 7], Config->Color[cMaxColors - 1]);
               else {
#ifdef GAMEMON
//                 D2Game->SetSocket(iNewSocket);
//                 D2Game->DrawList();
//                 DrawScreen(15);
#endif
                 }
            break;
    default: OutWin->AddWrap(Config->String[cMaxStrings - 8], Config->Color[cMaxColors - 1]); break;
    }
  }

void TApp::CMD_Game(char *szGameNum) {
  }







void TApp::CMD_Shell(char *szMessage) {
  char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
  if(strlen(szMessage) > 0) {
    sprintf(szLine2Add, "* %s has shelled to DOS (%s).", Config->Nickname, szMessage);
    OutWin->AddWrap(szLine2Add, Config->Color[5]);
    sprintf(szLine2Add, "has shelled to DOS (%s).", szMessage);
    KChat->SendPostureMessage(szLine2Add);
    } else {
      sprintf(szLine2Add, "* %s has shelled to DOS.", Config->Nickname, szMessage);
      OutWin->AddWrap(szLine2Add, Config->Color[5]);
      sprintf(szLine2Add, "has shelled to DOS.", szMessage);
      KChat->SendPostureMessage(szLine2Add);
      }
  textmode(C80);
  clrscr();
  spawnlp(P_WAIT, "COMMAND.COM", "COMMAND.COM", NULL);
  sprintf(szLine2Add, "* %s has returned from DOS.", Config->Nickname, szMessage);
  OutWin->AddWrap(szLine2Add, Config->Color[5]);
  sprintf(szLine2Add, "has returned from DOS.", szMessage);
  KChat->SendPostureMessage(szLine2Add);
  textmode(C4350);
  CMD_Chat();
  delete(szLine2Add);
  }


void TApp::CMD_Beep(void) {
//  iBeep = 1 - iBeep;
//  if(iBeep) OutWin->AddWrap("*** Notification beeps enabled.", cInfoColor);
//    else OutWin->AddWrap("*** Notification beeps disabled.", cInfoColor);
  }


void TApp::CMD_Deoffer(char *szParms) {
  if(strlen(szParms) > 0) {
    if(stricmp(szParms, "all") == 0) {
      KChat->DeOffer(-1);
      OutWin->AddWrap("*** Deoffering all files.", Config->DeofferColor);
      } else {
        int iFileNum = (int)strtol(szParms, NULL, 0);
        if(iFileNum >= 0) {
          if(iFileNum < NUMFILEOFFERS) {
            if(KChat->DeOffer(iFileNum) == 0) {
              char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
              sprintf(szLine2Add, "*** Deoffering file %d.", iFileNum);
              OutWin->AddWrap(szLine2Add, Config->DeofferColor);
              delete(szLine2Add);
              } else OutWin->AddWrap(Config->String[cMaxStrings - 9], Config->Color[cMaxColors - 1]);
            } else OutWin->AddWrap(Config->String[cMaxStrings - 9], Config->Color[cMaxColors - 1]);
          } else OutWin->AddWrap(Config->String[cMaxStrings - 9], Config->Color[cMaxColors - 1]);
        }
    } else OutWin->AddWrap(Config->String[cMaxStrings - 2], Config->Color[cMaxColors - 1]);
  }

void TApp::CMD_Offer(char *szParms) {
  int iResult = 0;
  char *szTemp = new char[300]; assert(szTemp != NULL);
  FileXfer Offer;
  FileXfer *Data;
  if(strlen(szParms) > 0) {
    char *lpszFile;
    char *lpszDesc;
    strcpy(szTemp, szParms);
    lpszFile = strtok(szTemp, " ");
    lpszDesc = strchr(lpszFile, 0);
    if(strcmp(lpszFile, szParms) != 0) lpszDesc++;
    strcpy(Offer.File, lpszFile);
    strcpy(Offer.Description, lpszDesc);
    if(KChat->OfferFile(&Offer) < 0) iResult = -1;
    }
  if(iResult < 0) OutWin->AddWrap(Config->String[cMaxStrings - 10], Config->Color[cMaxColors - 1]);
    else {
      int iValid = 0;
      for(int i = 0; i < NUMFILEOFFERS; i++) {
        Data = KChat->GetOfferFileInfo(i);
        if(strlen(Data->File) > 0) iValid = 1;
        }
      if(iValid) {
        CMD_Me("offers the following files for download:");
        for(int i = 0; i < NUMFILEOFFERS; i++) {
        Data = KChat->GetOfferFileInfo(i);
        if(strlen(Data->File) > 0) {
          sprintf(szTemp, "[%d] %s  %s", i, Data->File, Data->Description);
          CMD_Me(szTemp);
          }
        }
      } else OutWin->AddWrap(Config->String[cMaxStrings - 11], Config->Color[cMaxColors - 1]);
    }
  delete(szTemp);
  }

void TApp::CMD_Get(char *szParms) {
  char *szTemp = new char[300]; assert(szTemp != NULL);
  FileXfer XferData;
  strcpy(szTemp, szParms);
  if(strlen(szTemp) > 0) {
    strcpy(XferData.User, strtok(szTemp, " "));
    if(strlen(XferData.User) > 0) {
      char *lpszTemp = strtok(NULL, " ");
      XferData.Index = (int)strtol(lpszTemp, NULL, 0);
      strcpy(XferData.File, strtok(NULL, " "));
      if(strlen(XferData.User) > 0) {
        int iResult = KChat->GetFile(&XferData);
        if(iResult == 0) {
          sprintf(szTemp, "*** Retrieving file #%d from %s", XferData.Index, XferData.User);
          OutWin->AddWrap(szTemp, Config->Color[cMaxColors - 1]);  //FIX THIS!!!!!
          }
        if(iResult == -1) OutWin->AddWrap("!!! unknown user", Config->ErrorColor);
        if(iResult == -2) OutWin->AddWrap("!!! can't open file", Config->ErrorColor);
        } else OutWin->AddWrap("!!! invalid command", Config->ErrorColor);
      } else OutWin->AddWrap("!!! invalid command", Config->ErrorColor);
    } else OutWin->AddWrap("!!! invalid command", Config->ErrorColor);
  delete(szTemp);
  }


