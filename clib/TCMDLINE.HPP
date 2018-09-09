//TCmdLine - Command Line Interface (dos)
//Copyright 1996 Aaron Brinton
//
//Usage:
//
//  void main(void) {
//    TCmdLine *CmdLine;
//    int X1 = 10;
//    int X2 = 70;
//    int Y = 24;
//    int Attrib = 15;
//    int InputLength = 200;
//    int HistorySize = 50;
//    CmdLine = new TCmdLine(X1, X2, Y, Attrib, InputLength, HistorySize);
//    int iDone = 0;
//    int iResult = 0;
//    while(!iDone) {  //*** this is an idle loop
//      iResult = CmdLine->Run();
//      if(iResult == 1) HandleCommand(CmdLine->szReturn);
//      if(iResult == 2) iDone = HandleHotKey(CmdLine->iHotKey);
//      }
//    delete(CmdLine);
//    }
//
//Notes:
//
//  Run() is designed to be called in an idle loop.  It will return 0 unless
//  CR is pressed (returning 1, with data in szReturn) or a hotkey is pressed
//  (returning 2, with data in iHotKey).
//
//  iHotKey will be the ascii value for the hotkey pressed, negative values for
//  normal characters (control characters, excluding CR and BS) and positive
//  values for extended characters (exluding those used by this class).
//
//  In the above example, HandleHotKey() would return a value if an exit hotkey
//  was pressed.
//
//  Clear() is used to clear the current command line.
//
//  Update() is used to refresh the command line in a screen clear situation.
//
//  SetText() is used to pre-initialize the command line to some text string.
//
//  Supported editing is as follows:
//
//    UpArrow       Previous history item
//    DownArrow     Next history item
//    LeftArrow     Move left
//    RightArrow    Move right
//    Delete        Delete character at cursor
//    Backspace     Delete character left of cursor
//    Home          Move to beginning of line
//    End           Move to end of line
//
//*****************************************************************************

#define cMinInputLength 10
#define cMaxInputLength 1000
#define cMinHistory 10
#define cMaxHistory 1000

#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

class TCmdLine
{
  void PutChar(int X, int Y, unsigned char Chr, unsigned char Attr);
  void PutString(int X, int Y, char *S, unsigned char Attr);
  void BackSpace(void);
  void CarriageReturn(void);
  void NormalChar(char Chr);
  void Home(void);
  void UpArrow(void);
  void LeftArrow(void);
  void RightArrow(void);
  void End(void);
  void DownArrow(void);
  void Delete(void);
  int HandleKey(unsigned char Chr);
  void AddHistoryLine(char *szAddLine);

public:
  TCmdLine(unsigned char X1, unsigned char X2, unsigned char Y, unsigned char Attrib, int InputLength, int HistorySize);
  ~TCmdLine(void);
  int Run(void);
  void Clear(void);
  void Update(void);
  void SetText(char *szText);
  char *szReturn;
  int iHotKey;

private:
  unsigned char ucX1, ucX2, ucY, ucAttrib;
  int iInputLength, iHistorySize; //startup constants
  char *lpszHistory[cMaxHistory]; //history pointer array
  int iLastX, iLastY;             // store cursor position when leaving Run();
  int iCurrentHistoryLine;
  int iCmdLinePos;
  char *szCommandLine;
};

TCmdLine::TCmdLine(unsigned char X1, unsigned char X2, unsigned char Y, unsigned char Attrib, int InputLength, int HistorySize)
{
  //store and range check initialization parms
  ucX1 = X1;
  ucX2 = X2;
  ucY = Y;
  ucAttrib = Attrib;
  iInputLength = InputLength;
  iHistorySize = HistorySize;
  if (iInputLength < cMinInputLength)
    iInputLength = cMinInputLength;
  if (iInputLength > cMaxInputLength)
    iInputLength = cMaxInputLength;
  if (iHistorySize < cMinHistory)
    iHistorySize = cMinHistory;
  if (iHistorySize > cMaxHistory)
    iHistorySize = cMaxHistory;
  //allocate and initialize as required
  szReturn = new char[cMaxInputLength];
  assert(szReturn != NULL);
  szCommandLine = new char[cMaxInputLength];
  assert(szCommandLine != NULL);
  for (int i = 0; i < iHistorySize; i++)
  {
    lpszHistory[i] = new char[iInputLength + 2];
    assert(lpszHistory[i] != NULL);
    lpszHistory[i][0] = 0;
  }
  Clear();
}

TCmdLine::~TCmdLine(void)
{
  for (int i = 0; i < iHistorySize; i++)
    delete (lpszHistory[i]);
  delete (szCommandLine);
  delete (szReturn);
}

void TCmdLine::PutChar(int X, int Y, unsigned char Chr, unsigned char Attr)
{
  char *lpMemory = (char far *)MK_FP(0xB800, 0x0000);
  lpMemory[((Y - 1) * 160) + ((X - 1) * 2)] = Chr;
  lpMemory[((Y - 1) * 160) + ((X - 1) * 2) + 1] = Attr;
}

void TCmdLine::PutString(int X, int Y, char *S, unsigned char Attr)
{
  for (int iLoop = 0; iLoop < strlen(S); iLoop++)
    PutChar(X + iLoop, Y, S[iLoop], Attr);
}

void TCmdLine::Clear(void)
{
  //clear command line
  struct text_info TextInfo;
  gettextinfo(&TextInfo);
  iCmdLinePos = 0;
  strcpy(szCommandLine, "");
  textattr(ucAttrib);
  window(ucX1, ucY, ucX2, ucY);
  clrscr();
  window(1, 1, TextInfo.screenwidth, TextInfo.screenheight);
  gotoxy(ucX1, ucY);
  iLastX = ucX1;
  iLastY = ucY;
}

void TCmdLine::Update(void)
{
  //update command line
  char *szTemp = new char[cMaxInputLength];
  assert(szTemp != NULL);
  strcpy(szTemp, &szCommandLine[iCmdLinePos]);
  if (strlen(szTemp) > ucX2 - ucX1)
    szTemp[ucX2 - ucX1] = 0;
  else
    for (int i = strlen(szTemp); i <= ucX2 - ucX1; i++)
      strcat(szTemp, " ");
  PutString(ucX1, ucY, szTemp, ucAttrib);
  delete (szTemp);
}

void TCmdLine::BackSpace(void)
{
  char *szTemp1 = new char[cMaxInputLength];
  assert(szTemp1 != NULL);
  char *szTemp2 = new char[cMaxInputLength];
  assert(szTemp2 != NULL);
  if (strlen(szCommandLine) > 0)
    if ((iCmdLinePos > 0) || (wherex() > ucX1))
    {
      strcpy(szTemp1, szCommandLine);
      szTemp1[iCmdLinePos + wherex() - ucX1 - 1] = 0;
      strcpy(szTemp2, &szCommandLine[iCmdLinePos + wherex() - ucX1]);
      if (wherex() + iCmdLinePos - ucX1 > strlen(szCommandLine))
        strcpy(szTemp2, "");
      sprintf(szCommandLine, "%s%s", szTemp1, szTemp2);
      int wx = wherex();
      if (iCmdLinePos > 0)
        if (wx <= ucX1 + (ucX2 - ucX1) / 2)
        {
          wx++;
          iCmdLinePos--;
        }
      Update();
      gotoxy(wx - 1, ucY);
    }
  delete (szTemp1);
  delete (szTemp2);
}

void TCmdLine::CarriageReturn(void)
{
  if (strlen(szCommandLine) > 0)
    if (strcmp(szCommandLine, lpszHistory[iHistorySize - 1]) != 0)
      AddHistoryLine(szCommandLine);
}

void TCmdLine::NormalChar(char Chr)
{
  char *szTemp1 = new char[cMaxInputLength];
  assert(szTemp1 != NULL);
  char *szTemp2 = new char[cMaxInputLength];
  assert(szTemp2 != NULL);
  if (strlen(szCommandLine) < iInputLength)
  {
    iCurrentHistoryLine = iHistorySize;
    int wx = wherex();
    strcpy(szTemp1, szCommandLine);
    szTemp1[iCmdLinePos + wherex() - ucX1] = 0;
    strcpy(szTemp2, &szCommandLine[iCmdLinePos + wherex() - ucX1]);
    sprintf(szCommandLine, "%s%c%s", szTemp1, Chr, szTemp2);
    if (wx == ucX2)
      iCmdLinePos++;
    Update();
    if (wx <= ucX2 - 1)
      gotoxy(wx + 1, ucY);
    else
      gotoxy(ucX2, ucY);
  }
  delete (szTemp1);
  delete (szTemp2);
}

void TCmdLine::Home(void)
{
  iCmdLinePos = 0;
  Update();
  gotoxy(ucX1, ucY);
}

void TCmdLine::UpArrow(void)
{
  if (iCurrentHistoryLine > 0)
    if (strlen(lpszHistory[iCurrentHistoryLine - 1]) > 0)
    {
      if (iCurrentHistoryLine == iHistorySize)
        if (strcmp(szCommandLine, lpszHistory[iHistorySize - 1]) != 0)
          if (strlen(szCommandLine) > 0)
          {
            AddHistoryLine(szCommandLine);
            iCurrentHistoryLine--;
          }
      iCmdLinePos = 0;
      iCurrentHistoryLine--;
      strcpy(szCommandLine, lpszHistory[iCurrentHistoryLine]);
      End();
    }
}

void TCmdLine::LeftArrow(void)
{
  if (wherex() != ucX1)
    gotoxy(wherex() - 1, ucY);
  else if (iCmdLinePos != 0)
  {
    iCmdLinePos--;
    Update();
    gotoxy(ucX1, ucY);
  }
}

void TCmdLine::RightArrow(void)
{
  if ((iCmdLinePos + wherex() - ucX1 + 1) <= strlen(szCommandLine))
  {
    if (wherex() != ucX2)
      gotoxy(wherex() + 1, ucY);
    else
    {
      iCmdLinePos++;
      Update();
      gotoxy(ucX2, ucY);
    }
  }
}

void TCmdLine::End(void)
{
  if (strlen(szCommandLine) > ucX2 - ucX1)
    iCmdLinePos = strlen(szCommandLine) - (ucX2 - ucX1);
  Update();
  gotoxy(strlen(szCommandLine) - iCmdLinePos + ucX1, ucY);
}

void TCmdLine::DownArrow(void)
{
  if (iCurrentHistoryLine < iHistorySize - 1)
  {
    iCmdLinePos = 0;
    iCurrentHistoryLine++;
    strcpy(szCommandLine, lpszHistory[iCurrentHistoryLine]);
    End();
  }
}

void TCmdLine::Delete(void)
{
  char *szTemp1 = new char[cMaxInputLength];
  assert(szTemp1 != NULL);
  char *szTemp2 = new char[cMaxInputLength];
  assert(szTemp2 != NULL);
  szCommandLine[strlen(szCommandLine)] = 0;
  if (strlen(szCommandLine) > 0)
  {
    strcpy(szTemp1, szCommandLine);
    szTemp1[iCmdLinePos + wherex() - ucX1] = 0;
    strcpy(szTemp2, &szCommandLine[iCmdLinePos + wherex() - ucX1 + 1]);
    sprintf(szCommandLine, "%s%s", szTemp1, szTemp2);
    int wx = wherex();
    Update();
    gotoxy(wx, ucY);
  }
  delete (szTemp1);
  delete (szTemp2);
}

int TCmdLine::HandleKey(unsigned char Chr)
{
  //handle incoming keystrokes
  int iResult = 0;
  if (Chr != 0)
  {
    //normal chars
    switch (Chr)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      iHotKey = 0 - Chr;
      break;
    case 8:
      BackSpace();
      break;
    case 9:
    case 10:
    case 11:
    case 12:
      iHotKey = 0 - Chr;
      break;
    case 13:
      CarriageReturn();
      iResult = 1;
      break;
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
      iHotKey = 0 - Chr;
      break;
    default:
      NormalChar(Chr);
    }
  }
  else
  {
    //extended chars
    Chr = getch();
    switch (Chr)
    {
    case 71:
      Home();
      break;
    case 72:
      UpArrow();
      break;
    case 75:
      LeftArrow();
      break;
    case 77:
      RightArrow();
      break;
    case 79:
      End();
      break;
    case 80:
      DownArrow();
      break;
    case 83:
      Delete();
      break;
    default:
      iHotKey = Chr;
    }
  }
  if (iHotKey)
    iResult = 2;
  if (iResult == 1)
  {
    iCurrentHistoryLine = iHistorySize;
    strcpy(szReturn, szCommandLine);
    Clear();
  }
  return (iResult);
}

void TCmdLine::AddHistoryLine(char *szAddLine)
{
  for (int i = 1; i < iHistorySize; i++)
    strcpy(lpszHistory[i - 1], lpszHistory[i]);
  strcpy(lpszHistory[iHistorySize - 1], szAddLine);
}

void TCmdLine::SetText(char *szText)
{
  Clear();
  for (int i = 0; i < strlen(szText); i++)
    HandleKey(szText[i]);
  iLastX = wherex();
  iLastY = wherey(); //save cursor position
}

int TCmdLine::Run(void)
{
  //idle loop
  gotoxy(iLastX, iLastY); //restore cursor position
  int iResult = 0;
  iHotKey = 0;
  if (kbhit() != 0)
    iResult = HandleKey(getch()); //0=normal, 1=command/message, 2=hotkey
  iLastX = wherex();
  iLastY = wherey(); //save cursor position
  return (iResult);
}
