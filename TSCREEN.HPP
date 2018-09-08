//TScreen - GMON Screen Drawing
//Copyright 1996 Aaron Brinton

#define cSglHorz 0xC4
#define cSglVert 0xB3
#define cSglTopLeft 0xDA
#define cSglTopRight 0xBF
#define cSglBotLeft 0xC0
#define cSglBotRight 0xD9
#define cDblHorz 0xCD
#define cDblVert 0xBA
#define cDblTopLeft 0xC9
#define cDblTopRight 0xBB
#define cDblBotLeft 0xC8
#define cDblBotRight 0xBC
#define cDblVertDblLeft 0xB9
#define cDblVertDblRight 0xCC
#define cDblHorzDblTop 0xCA
#define cDblHorzDblBottom 0xCB
#define cDblVertSglLeft 0xB6
#define cDblVertSglRight 0xC7
#define cDblHorzSglTop 0xCF
#define cDblHorzSglBottom 0xD1
#define cSglVertSglLeft 0xB4
#define cSglVertSglRight 0xC3
#define cSglHorzSglTop 0xC1
#define cSglHorzSglBottom 0xC2

#include <dos.h>

class TScreen
{
  void PutChar(int X, int Y, unsigned char Chr, unsigned char Attr);
  void PutString(int X, int Y, char *S, unsigned char Attr);
  void HorzSglLine(int X1, int X2, int Y);
  void VertSglLine(int X, int Y1, int Y2);
  void HorzDblLine(int X1, int X2, int Y);
  void VertDblLine(int X, int Y1, int Y2);
  void HelpDraw(int iDivider, int iLastLine);
  void HelpHelp(void);
  void HelpKali(void);
  void HelpEdit(void);
  void HelpChat(void);
  void HelpFile(void);
  void HelpGame(void);
  void HelpDos(void);
  void Progress(void);

public:
  TScreen(int LineColor, int TitleColor, int ScreenNameColor, int ClockColor, int HelpTextColor, int SendProgressColor, int RecvProgressColor);
  ~TScreen(void);
  void UpdateClock(void);
  void DrawScreen(int Type, int FirstChatLine);

private:
  int iLineColor;
  int iTitleColor;
  int iScreenNameColor;
  int iClockColor;
  int iHelpTextColor;
  int iSendProgressColor;
  int iRecvProgressColor;
};

TScreen::TScreen(int LineColor, int TitleColor, int ScreenNameColor, int ClockColor, int HelpTextColor, int SendProgressColor, int RecvProgressColor)
{
  iLineColor = LineColor;
  iTitleColor = TitleColor;
  iScreenNameColor = ScreenNameColor;
  iClockColor = ClockColor;
  iHelpTextColor = HelpTextColor;
  iSendProgressColor = SendProgressColor;
  iRecvProgressColor = RecvProgressColor;
}

TScreen::~TScreen(void)
{
}

void TScreen::PutChar(int X, int Y, unsigned char Chr, unsigned char Attr)
{
  void *lpMemory = MK_FP(0xB800, 0x0000);
  char *lpszMemory = (char far *)lpMemory;
  lpszMemory[((Y - 1) * 160) + ((X - 1) * 2)] = Chr;
  lpszMemory[((Y - 1) * 160) + ((X - 1) * 2) + 1] = Attr;
}

void TScreen::PutString(int X, int Y, char *S, unsigned char Attr)
{
  for (int iLoop = 0; iLoop < strlen(S); iLoop++)
    PutChar(X + iLoop, Y, S[iLoop], Attr);
}

void TScreen::HorzSglLine(int X1, int X2, int Y)
{
  int iLoop;
  for (iLoop = X1; iLoop <= X2; iLoop++)
    PutChar(iLoop, Y, cSglHorz, iLineColor);
}

void TScreen::VertSglLine(int X, int Y1, int Y2)
{
  int iLoop;
  for (iLoop = Y1; iLoop <= Y2; iLoop++)
    PutChar(X, iLoop, cSglVert, iLineColor);
}

void TScreen::HorzDblLine(int X1, int X2, int Y)
{
  int iLoop;
  for (iLoop = X1; iLoop <= X2; iLoop++)
    PutChar(iLoop, Y, cDblHorz, iLineColor);
}

void TScreen::VertDblLine(int X, int Y1, int Y2)
{
  int iLoop;
  for (iLoop = Y1; iLoop <= Y2; iLoop++)
    PutChar(X, iLoop, cDblVert, iLineColor);
}

void TScreen::UpdateClock(void)
{
  struct time UpdateTime;
  gettime(&UpdateTime);
  char szTemp[10];
  PutString(71, 49, "        ", iClockColor);
  sprintf(szTemp, "%02d:%02d:%02d", UpdateTime.ti_hour, UpdateTime.ti_min, UpdateTime.ti_sec);
  PutString(71, 49, szTemp, iClockColor);
}

void TScreen::DrawScreen(int Type, int FirstChatLine)
{
  clrscr();
  PutChar(1, 1, cDblTopLeft, iLineColor);
  HorzDblLine(2, 79, 1);
  PutChar(80, 1, cDblTopRight, iLineColor);
  HorzDblLine(2, 79, 3);
  VertDblLine(1, 2, 49);
  VertDblLine(80, 2, 49);
  HorzDblLine(2, 79, 48);
  PutChar(1, 3, cDblVertDblRight, iLineColor);
  PutChar(80, 3, cDblVertDblLeft, iLineColor);
  PutChar(1, 48, cDblVertDblRight, iLineColor);
  PutChar(80, 48, cDblVertDblLeft, iLineColor);
  PutChar(1, 50, cDblBotLeft, iLineColor);
  HorzDblLine(2, 79, 50);
  PutChar(80, 50, cDblBotRight, iLineColor);
  PutString(2, 2, cTitle, iTitleColor);
  if (random(100) < 50)
    PutString(2, 49, cCopyright1, iTitleColor);
  else
    PutString(2, 49, cCopyright2, iTitleColor);

  PutString(59, 49, "GMON v", iTitleColor);
  PutString(65, 49, cVersion, iTitleColor);
  PutChar(69, 48, cDblHorzSglBottom, iLineColor);
  PutChar(69, 49, cSglVert, iLineColor);
  PutChar(69, 50, cDblHorzSglTop, iLineColor);
  PutChar(1, 46, cDblVertSglRight, iLineColor);
  HorzSglLine(2, 79, 46);
  PutChar(80, 46, cDblVertSglLeft, iLineColor);
  switch (Type)
  {
  case 1:
    HelpHelp();
    break;
  case 2:
    HelpEdit();
    break;
  case 3:
    HelpKali();
    break;
  case 4:
    HelpChat();
    break;
  case 5:
    HelpFile();
    break;
  case 6:
    HelpGame();
    break;
  case 7:
    HelpDos();
    break;
  case 10:
    Progress();
    break;
  }
  PutChar(60, FirstChatLine - 1, cDblHorzSglBottom, iLineColor);
  VertSglLine(60, FirstChatLine, 45);
  PutChar(60, 46, cSglHorzSglTop, iLineColor);
}

void TScreen::HelpDraw(int iDivider, int iLastLine)
{
  PutChar(1, iLastLine, cDblVertDblRight, iLineColor);
  HorzDblLine(2, 79, iLastLine);
  PutChar(80, iLastLine, cDblVertDblLeft, iLineColor);
  PutChar(iDivider, 3, cDblHorzSglBottom, iLineColor);
  for (int i = 4; i < iLastLine; i++)
    PutChar(iDivider, i, cSglVert, iLineColor);
  PutChar(iDivider, iLastLine, cDblHorzSglTop, iLineColor);
}

void TScreen::HelpHelp(void)
{
  PutString(66, 2, "Help Commands", iScreenNameColor);
  PutString(2, 4, "/help edit  Command Line Editing", iHelpTextColor);
  PutString(2, 5, "/help kali  Kali Commands", iHelpTextColor);
  PutString(2, 6, "/help chat  Chat Commands", iHelpTextColor);
  PutString(2, 7, "/help file  File Commands", iHelpTextColor);
  PutString(2, 8, "/help game  Game Commands", iHelpTextColor);
  PutString(2, 9, "/help dos   DOS Commands", iHelpTextColor);
  PutString(2, 10, "/chat       Return to full screen chat", iHelpTextColor);
  PutString(2, 11, "/quit       Quit", iHelpTextColor);
  PutString(2, 12, "/quitX      Quit with errorlevel X (1-9)", iHelpTextColor);
  HelpDraw(13, 13);
}

void TScreen::HelpEdit(void)
{
  PutString(59, 2, "Command Line Editing", iScreenNameColor);
  PutString(2, 4, "Up/Down Arrows        Review previous commands", iHelpTextColor);
  PutString(2, 5, "PageUp/PageDown       Scrollback", iHelpTextColor);
  PutString(2, 6, "Ctrl-PageUp/PageDown  Scroll user list", iHelpTextColor);
  PutString(2, 7, "Alt-1 to Alt-0        Macros", iHelpTextColor);
  PutString(2, 8, "Tab                   Reply to private message (/msg)", iHelpTextColor);
  HelpDraw(23, 9);
}

void TScreen::HelpKali(void)
{
  PutString(66, 2, "Kali Commands", iScreenNameColor);
  PutString(2, 4, "/users [<nickname>]  User list with status", iHelpTextColor);
  PutString(2, 5, "/email [<nickname>]  User list with email address (needs cleanup)", iHelpTextColor);
  PutString(2, 6, "/whois <nickname>    Retrieve user information", iHelpTextColor);
  PutString(2, 7, "/servers             Kali server list (needs cleanup)", iHelpTextColor);
  PutString(2, 8, "/motd                Show server message of the day (needs cleanup)", iHelpTextColor);
  HelpDraw(22, 9);
}

void TScreen::HelpChat(void)
{
  PutString(66, 2, "Chat Commands", iScreenNameColor);
  PutString(2, 4, "/away [<message>]             Leave an away message", iHelpTextColor);
  PutString(2, 5, "/beep                         Toggle beeping (if beeps are enabled)", iHelpTextColor);
  PutString(2, 6, "/clear                        Clear chat window", iHelpTextColor);
  PutString(2, 7, "/ignore <nickname>            Ignore user", iHelpTextColor);
  PutString(2, 8, "/join <channel> [<password>]  Join channel", iHelpTextColor);
  PutString(2, 9, "/me <message>                 Posture message", iHelpTextColor);
  PutString(2, 10, "/msg <#|nickname> <message>   Send private message", iHelpTextColor);
  PutString(2, 11, "/ping [<*|nickname>]          Ping users", iHelpTextColor);
  PutString(2, 12, "/who [<*|channel>]            List users", iHelpTextColor);
  HelpDraw(31, 13);
}

void TScreen::HelpFile(void)
{
  PutString(66, 2, "File Commands", iScreenNameColor);
  PutString(2, 4, "/offer [<filename>]                    Offer a file for download (up to 10)", iHelpTextColor);
  PutString(2, 5, "/deoffer <#|all]                       Deoffer files", iHelpTextColor);
  PutString(2, 6, "/get <nickname> <#> <local filename>   Download a file (currently overwrites)", iHelpTextColor);
  PutString(2, 7, "/xget <nickname> <#> <local filename>  Download a file with overwrite", iHelpTextColor);
  PutString(2, 8, "/progress                              Show progress of file transfers", iHelpTextColor);
  HelpDraw(40, 9);
}

void TScreen::HelpGame(void)
{
  PutString(66, 2, "Game Commands", iScreenNameColor);
  //  HelpDraw(31, 13);
}

void TScreen::HelpDos(void)
{
  PutString(67, 2, "DOS Commands", iScreenNameColor);
  PutString(2, 4, "/cd [<parms>]    Change directory", iHelpTextColor);
  PutString(2, 5, "/dir [<parms>]   Directory", iHelpTextColor);
  PutString(2, 6, "/type [<parms>]  Type file", iHelpTextColor);
  PutString(2, 7, "/shell           Shell to DOS", iHelpTextColor);
  HelpDraw(18, 8);
}

void TScreen::Progress(void)
{
  PutString(57, 2, "File Transfer Progress", iScreenNameColor);
  PutChar(1, 15, cDblVertDblRight, iLineColor);
  HorzDblLine(2, 79, 15);
  PutChar(80, 15, cDblVertDblLeft, iLineColor);
  PutChar(2, 4, 82, iRecvProgressColor);
  PutChar(2, 5, 48, iSendProgressColor);
  PutChar(2, 6, 49, iSendProgressColor);
  PutChar(2, 7, 50, iSendProgressColor);
  PutChar(2, 8, 51, iSendProgressColor);
  PutChar(2, 9, 52, iSendProgressColor);
  PutChar(2, 10, 53, iSendProgressColor);
  PutChar(2, 11, 54, iSendProgressColor);
  PutChar(2, 12, 55, iSendProgressColor);
  PutChar(2, 13, 56, iSendProgressColor);
  PutChar(2, 14, 57, iSendProgressColor);
  PutChar(3, 3, cDblHorzSglBottom, iLineColor);
  for (int i = 4; i < 15; i++)
    PutChar(3, i, cSglVert, iLineColor);
  PutChar(3, 15, cDblHorzSglTop, iLineColor);
  PutChar(20, 3, cDblHorzSglBottom, iLineColor);
  for (i = 4; i < 15; i++)
    PutChar(20, i, cSglVert, iLineColor);
  PutChar(20, 15, cDblHorzSglTop, iLineColor);
  PutChar(29, 3, cDblHorzSglBottom, iLineColor);
  for (i = 4; i < 15; i++)
    PutChar(29, i, cSglVert, iLineColor);
  PutChar(29, 15, cDblHorzSglTop, iLineColor);
}
