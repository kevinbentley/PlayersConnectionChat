//GMON TApp
//Copyright 1996 Aaron Brinton

//#define NODEBUG
#define DARKSIDE
#define HELP
//#define GAMEMON

#include "dpack\dpackapi.hpp"
#include "dchat\dchat.hpp"
#include "..\clib\tcmdline.hpp"
#include "..\clib\toutwin.hpp"
#include "..\clib\tlistwin.hpp"
#include "tconfig.hpp"
#include "tscreen.hpp"

#ifdef GAMEMON
  #include "td1game.hpp"
  #include "td2game.hpp"
#endif

#include <dos.h>
#include <ctype.h>

class TApp {
  int ReadConfig(void);
  void DeleteConfig(void);
  int CheckForKali(char *szNick);
  void SendText(char *szText);
  void RefreshUsers(void);
  void DrawScreen(int Type, int FirstChatLine);
  int ParseRecipients(char *szString, char **szMessage, char *szList[]);
  void ProgressUpdate(void);
  void PutChar(int X, int Y, unsigned char Chr, unsigned char Attr);
  void PutString(int X, int Y, char * S, unsigned char Attr);

  //command functions
  void HandleHotKey(int iHotKey);
  void HandleCommand(char *szCommand);
  void CMD_Help(char *szTopic);
  void CMD_Quit(int iErrLevel, char *szMessage);
  void CMD_Chat(void);
  void CMD_Users(char *szSearchNick);
  void CMD_Email(char *szSearchNick);
  void CMD_Whois(char *szSearchNick);
  void CMD_Servers(void);
  void CMD_Motd(void);
  void CMD_Progress(void);
  void CMD_D1(void);
  void CMD_D2(void);
  void CMD_Dos(char *szParms);
  void CMD_List(char *szSocket);
  void CMD_Game(char *szGameNum);
  void CMD_Debug(void);
  void CMD_DarkSide(void);
  void CMD_AllMsgs(void);
  void CMD_Away(char *szMessage);
  void CMD_NoPrivate(char *szMessage);
  void CMD_Join(char *szNewChannel);
  void CMD_Msg(char *szParms);
  void CMD_Beep(void);
  void CMD_Shell(char *szMessage);
  void CMD_Me(char *szMessage);
  void CMD_Who(char *szWhoChannel);
  void CMD_Offer(char *szParms);
  void CMD_Deoffer(char *szParms);
  void CMD_Get(char *szParms);
  void CMD_Ping(char *szParms);
  void CMD_Ignore(char *szBadNick);
  void CMD_Clear(void);

  //message functions
  void HandleMessage(Message *ChatMsg);
  void MSG_Message(Message *ChatMsg);
  void MSG_Hello(Message *ChatMsg);
  void MSG_Bye(Message *ChatMsg);
  void MSG_Part(Message *ChatMsg);
  void MSG_Who(Message *ChatMsg);
  void MSG_Iam(Message *ChatMsg);
  void MSG_Posture(Message *ChatMsg);
  void MSG_Private(Message *ChatMsg);
  void MSG_Ping(Message *ChatMsg);
  void MSG_Pong(Message *ChatMsg);
  void MSG_Get(Message *ChatMsg);
  void MSG_NoOffer(Message *ChatMsg);
  void MSG_Transfer(Message *ChatMsg);
  void MSG_Discon(Message *ChatMsg);
  void MSG_Unknown(Message *ChatMsg);

  public:
    TApp(void);
    ~TApp(void);
    int Startup(void);
    int Run(void);
    void Shutdown(void);

  private:
    int iKaliLoaded;
    int iDone;
    int iMode;
    int iTimer;
    int iUpdate;

    TConfig *Config;
    TScreen *Screen;
    TCmdLine *CmdLine;
    TOutWin *OutWin;
    TListWin *ListWin;
    Tkchat *KChat;
    char *szLastMsgNick;
#ifdef GAMEMON
    TD1Game *D1Game;
    TD2Game *D2Game;
#endif
  };

#include "tapp_cmd.hpp" //command handling
#include "tapp_msg.hpp" //message handling

TApp::TApp(void) {
  }

TApp::~TApp(void) {
  }

int TApp::Startup(void) {
  Config = new TConfig; assert(Config != NULL);
  if(Config->ReadConfig() < 0) return(-1);
  if(IPXInitialize() != 0) return(-2);
  iKaliLoaded = CheckForKali(Config->Nickname);
  iDone = 0;
  iMode = 0;
  iTimer = 0;
  iUpdate = 0;
  CmdLine = new TCmdLine(2, 79, 47, 15, 200, 50); assert(CmdLine != NULL);

  Screen = new TScreen(Config->Color[40], Config->Color[41], Config->Color[42], Config->Color[43], Config->Color[44], Config->Color[45], Config->Color[46]); assert(Screen != NULL);
  //set up chat
  OutWin = new TOutWin(2, 4, 59, 45, 15, 5, 500); assert(OutWin != NULL);
  ListWin = new TListWin(61, 4, 79, 45, 15, 500); assert(ListWin != NULL);
  DrawScreen(0, 4);
  KChat = new Tkchat; assert(KChat != NULL);
  KChat->AnnounceMe(Config->Nickname, Config->Channel);
  delay(100);
  KChat->Who("*");
  szLastMsgNick = new char[16]; assert(szLastMsgNick != NULL);
  return(0);
  }

int TApp::Run(void) {
  //idle chat
  Message *ChatMsg;
  KChat->EnterIdle();
  for(int i = 0; i < 20; i++) {
    ChatMsg = KChat->GetMessageFromQueue();
    if(ChatMsg == NULL) i = 20;
      else HandleMessage(ChatMsg);
    }

  //idle cmdline
  int iCmdResult = CmdLine->Run();
  if(iCmdResult == 1) HandleCommand(CmdLine->szReturn);
  if(iCmdResult == 2) HandleHotKey(CmdLine->iHotKey);

  //handle time sensitive stuff
  struct time UpdateTime;
  gettime(&UpdateTime);
  if(iTimer != UpdateTime.ti_sec) { //update each second
    Screen->UpdateClock();
      RefreshUsers();
      if(iMode == 100) ProgressUpdate();
    iUpdate++;
    iTimer = UpdateTime.ti_sec;
    }
  if(iUpdate >= Config->UpdateInterval) { //update each specified interval
    iUpdate = 0;
#ifdef GAMEMON
    if(iMode == 1) D1Game->Run();
//  if(iMode == 2) D2Game->Run();
#endif
    }
  return(iDone);
  }

void TApp::Shutdown(void) {
  KChat->GoodBye(Config->ByeMessage);
#ifdef GAMEMON
  if(iMode == 1) delete(D1Game);
  if(iMode == 2) delete(D2Game);
#endif
  delete(szLastMsgNick);
  delete(CmdLine);
  delete(Screen);
  delete(KChat);
  delete(ListWin);
  delete(OutWin);
  delete(Config);
  }

int TApp::CheckForKali(char *szNick) {
  //from Jay's code, with cosmetic modifications because I'm picky
  union	REGS Regs;
  struct SREGS SRegs;
  Regs.x.bx = 0x8201;
  int86x(0x7a, &Regs, &Regs, &SRegs);
  if(Regs.x.bx != 0x0182) return(0);
  memcpy(szNick, MK_FP(SRegs.es, Regs.x.di), 16);
  return(1);
  }

void TApp::SendText(char *szText) {
  char *szLine2Add = new char[300]; assert(szLine2Add != NULL);
  if(Config->AllMsgs) sprintf(szLine2Add, "[%s] <%s> %s", Config->Channel, Config->Nickname, szText);
    else sprintf(szLine2Add, Config->String[0], Config->Nickname, szText);
  OutWin->AddWrap(szLine2Add, Config->Color[0]);
  KChat->SendMessage(szText);
  delete(szLine2Add);
  }

void TApp::RefreshUsers() {
  if(!KChat->UsersChangedFlag()) return;
  char *lpszUserNick;
  char *szNick = new char[20]; assert(szNick != NULL);
  char *szTemp = new char[20]; assert(szTemp != NULL);
  ListWin->Clear();
  for(int i = 0; i < MAXUSERS; i++) {
    lpszUserNick = KChat->GetUserName(i);
    if(lpszUserNick[0] != 0) {
      strcpy(szNick, lpszUserNick);
      for(int j = strlen(szNick); j < 16; j++) szNick[j] = 32;
      szNick[16] = 0;
      sprintf(szTemp, "%s%d", szNick, i);
      if(i < 100) sprintf(szTemp, "%s %d", szNick, i);
      if(i < 10) sprintf(szTemp, "%s  %d", szNick, i);
      ListWin->AddLine(szTemp, Config->UserListColor);
      }
    }
  ListWin->Sort();
  ListWin->Redraw();
  delete(szNick);
  delete(szTemp);
  }

void TApp::DrawScreen(int Type, int FirstChatLine) {
  Screen->DrawScreen(Type, FirstChatLine);
  OutWin->SetHeight(FirstChatLine, 45);
  ListWin->SetHeight(FirstChatLine, 45);
  CmdLine->Update();
  Screen->UpdateClock();
  }

int TApp::ParseRecipients(char *szString, char **szMessage, char *szList[]) {
  //pass the message string (szString), a message pointer, and a list array pointer
  //returns number of recipients or 0 if invalid/none
  if(szString[0] == 0) return(0);
  char *szTempList[20];
  szString[strlen(szString) + 1] = 0;
  *szMessage = strtok(szString, " ");
  if(*szMessage == NULL) return(0);
  *szMessage = strtok(NULL, "\n");
  if(*szMessage == NULL) *szMessage = &szString[strlen(szString)];
  *szMessage++; //assign message pointer
  szTempList[0] = strtok(szString, ",");
  if(szList[0] == NULL) return(0);
  int Finished = 0;
  int Count = 1;
  while(!Finished) {
    szTempList[Count] = strtok(NULL, ",");
    szTempList[16] = 0;
    if((szTempList[Count] == NULL) || (Count > 19)) Finished = 1;
      else Count++;
    }
  for(int i = 0; i < Count; i++) {
    if(szTempList[i][0] == 35) { //check for #xx
      int iValidUser = 1;
      int iUser = (int)strtol(&szTempList[i][1], NULL, 0);
      if(iUser < 0) iValidUser = 0;
      if(iUser >= MAXUSERS) iValidUser = 0;
      if(iValidUser) if(strcmp(KChat->GetUserName(iUser), "") == 0) iValidUser = 0;
      if(iValidUser) sprintf(szList[i], "%d%s", 1, KChat->GetUserName(iUser));
        else sprintf(szList[i], "%d%s", 0, szTempList[i]);
      } else { //try to match as nickname
        int iUser = -1;
        for(int j = 0; j < MAXUSERS; j++)
          if(strncmpi(szTempList[i], KChat->GetUserName(j), strlen(szTempList[i])) == 0) {
            iUser = j; j = MAXUSERS;}
        if(iUser != -1) sprintf(szList[i], "%d%s", 1, KChat->GetUserName(iUser));
        else sprintf(szList[i], "%d%s", 0, szTempList[i]);
        }
    }
  return(Count);
  }

void TApp::ProgressUpdate(void) {
  window(4, 4, 19, 13);
  clrscr();
  window(21, 4, 28, 13);
  clrscr();
  window(30, 4, 78, 13);
  clrscr();
  window(1, 1, 80, 50);
  char *szTemp = new char[100]; assert(szTemp != NULL);
  OfferData *Offer;
  FileXfer *FileX;
  FileX = KChat->GetRcvdBytes();
  if(FileX != NULL) {
    PutString(4, 4, FileX->User, Config->Color[6]);
    ltoa(FileX->Progress, szTemp, 10);
    PutString(21, 4, szTemp, Config->Color[6]);
    PutString(30, 4, FileX->File, Config->Color[6]);
    }
  for(int i = 0; i < NUMFILESENDS; i++) {
    Offer = KChat->GetFileSendInfo(i);
    if(strcmp(Offer->Name, "") != 0) {
      FileX = KChat->GetOfferFileInfo(Offer->Index);
      PutString(4, i + 5, Offer->Name, Config->Color[5]);
      ltoa(Offer->Offset, szTemp, 10);
      PutString(21, i + 5, szTemp, Config->Color[5]);
      PutString(30, i + 5, FileX->File, Config->Color[5]);
      }
    }
  delete(szTemp);
  }

void TApp::PutChar(int X, int Y, unsigned char Chr, unsigned char Attr) {
  char *lpMemory = (char far *)MK_FP(0xB800, 0x0000);
  lpMemory[((Y-1) * 160) + ((X-1) * 2)] = Chr;
  lpMemory[((Y-1) * 160) + ((X-1) * 2) + 1] = Attr;
  }

void TApp::PutString(int X, int Y, char * S, unsigned char Attr) {
  for(int iLoop = 0; iLoop < strlen(S); iLoop++)
    PutChar(X + iLoop, Y, S[iLoop], Attr);
  }

