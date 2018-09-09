//TOutWin - Output Window (dos)
//Copyright 1996 Aaron Brinton
//
//Usage:
//
//  void main(void) {
//    TOutWin *OutWin;
//    int X1 = 10;
//    int Y1 = 5;
//    int X2 = 70;
//    int Y2 = 15;
//    int Attrib = 31;
//    int Indent = 5;
//    int Size = 50;
//    OutWin = new TOutWin(X1, Y1, X2, Y2, Attrib, Indent, Size);
//    OutWin->AddLine("This is line 1", Attrib);
//    OutWin->AddLine("This is line 2", Attrib);
//    OutWin->AddLine("This is line 3", Attrib);
//    OutWin->AddLine("This is line 4", Attrib);
//    delete(OutWin);
//    }
//
//Notes:
//
//  This is a pretty lame example.  It is meant to be used with a command
//  line input function.
//
//  Redraw() is used to redraw the window in a clear screen situation.
//
//  PageUp() and PageDown() are used to scroll through the data buffer.  The
//  parameter is the number of lines to scroll.
//
//  SetHeight() is used to adjust the Y parameters of the window.  Width cannot
//  be adjusted because of the line length memory allocation.
//
//  AddWrap() is the same as AddLine(), except it will wrap to the next line
//  with indent.
//
//*****************************************************************************

#define cMinBuffer 10
#define cMaxBuffer 500

class TOutWin
{
  void PutChar(int X, int Y, unsigned char Chr, unsigned char Attr);
  void PutString(int X, int Y, char *S, unsigned char Attr);

public:
  TOutWin(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char Attrib, int Indent, int Size);
  ~TOutWin(void);
  void Clear();
  void Redraw();
  void AddLine(char *NewLine, unsigned char Attrib);
  void AddWrap(char *NewLine, unsigned char Attrib);
  void PageUp(int Lines);
  void PageDown(int Lines);
  void SetHeight(unsigned char Y1, unsigned char Y2);

private:
  int SpacePos(char *SearchLine, int Length);
  void AddIndent(char *NewLine, unsigned char Attrib);
  struct TBuffer
  {
    unsigned char ucAttrib;
    char *szString;
  } Buffer[cMaxBuffer];
  unsigned char ucX1, ucY1, ucX2, ucY2, ucAttrib;
  int iIndent, iSize; //startup constants
  int iCurrentLine;
};

TOutWin::TOutWin(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char Attrib, int Indent, int Size)
{
  //store and range check initialization parms
  ucX1 = X1;
  ucY1 = Y1, ucX2 = X2, ucY2 = Y2;
  ucAttrib = Attrib;
  iIndent = Indent;
  iSize = Size + 2; //with a little working room for iSize
  if (iSize < ucY2 - ucY1 + 1)
    iSize = ucY2 - ucY1 + 1;
  if (iSize < cMinBuffer)
    iSize = cMinBuffer;
  if (iSize > cMaxBuffer)
    iSize = cMaxBuffer;
  //allocate and initialize as required
  for (int i = 0; i < iSize; i++)
  {
    Buffer[i].szString = new char[ucX2 - ucX1 + 2];
    assert(Buffer[i].szString != NULL);
  }
  iCurrentLine = iSize;
  Clear();
}

TOutWin::~TOutWin(void)
{
  for (int i = 0; i < iSize; i++)
    delete (Buffer[i].szString);
}

void TOutWin::PutChar(int X, int Y, unsigned char Chr, unsigned char Attr)
{
  char *lpMemory = (char far *)MK_FP(0xB800, 0x0000);
  lpMemory[((Y - 1) * 160) + ((X - 1) * 2)] = Chr;
  lpMemory[((Y - 1) * 160) + ((X - 1) * 2) + 1] = Attr;
}

void TOutWin::PutString(int X, int Y, char *S, unsigned char Attr)
{
  for (int iLoop = 0; iLoop < strlen(S); iLoop++)
    PutChar(X + iLoop, Y, S[iLoop], Attr);
}

void TOutWin::Clear()
{
  for (int i = 0; i < iSize; i++)
  {
    Buffer[i].ucAttrib = 0;
    Buffer[i].szString[0] = 0;
  }
  Redraw();
}

void TOutWin::Redraw()
{
  struct text_info TextInfo;
  gettextinfo(&TextInfo);
  textattr(ucAttrib);
  window(ucX1, ucY1, ucX2, ucY2);
  clrscr();
  window(1, 1, TextInfo.screenwidth, TextInfo.screenheight);
  for (int i = ucY1; i <= ucY2; i++)
    PutString(ucX1, i, Buffer[iCurrentLine - (ucY2 - i) - 1].szString, Buffer[iCurrentLine - (ucY2 - i) - 1].ucAttrib);
}

void TOutWin::AddLine(char *NewLine, unsigned char Attrib)
{
  //move the buffer data to make room for the new line (FIFO style)
  char *pTemp;
  pTemp = Buffer[0].szString;
  for (int i = 1; i < iSize; i++)
  {
    Buffer[i - 1].szString = Buffer[i].szString;
    Buffer[i - 1].ucAttrib = Buffer[i].ucAttrib;
  }
  Buffer[iSize - 1].szString = pTemp;
  strcpy(Buffer[iSize - 1].szString, NewLine);
  Buffer[iSize - 1].ucAttrib = Attrib;
  //move the current lines up and add the new line if we are looking at the end of the data buffer
  if (iCurrentLine == iSize)
  {
    char winX1 = ucX1 - 1;
    char winY1 = ucY1 - 1;
    char winX2 = ucX2 - 1;
    char winY2 = ucY2 - 1;
    char winAttrib = ucAttrib;
    asm {
      mov ah,6
      mov al,1
      mov bh,winAttrib
      mov cl,winX1
      mov ch,winY1
      mov dl,winX2
      mov dh,winY2
      int 0x10
    }
    Buffer[iSize - 1].szString[ucX2 - ucX1 + 1] = 0; //truncate to keep in window
    PutString(ucX1, ucY2, Buffer[iSize - 1].szString, Buffer[iSize - 1].ucAttrib);
  }
}

void TOutWin::AddWrap(char *NewLine, unsigned char Attrib)
{
  int iSpacePos;
  char *lpszWork = NewLine;
  if (strlen(lpszWork) <= ucX2 - ucX1 + 1)
    AddLine(NewLine, Attrib);
  else
  {
    iSpacePos = SpacePos(lpszWork, ucX2 - ucX1 + 1);
    if (lpszWork[iSpacePos] == 32)
    {
      lpszWork[iSpacePos] = 0;
      AddLine(lpszWork, Attrib);
      lpszWork = &lpszWork[iSpacePos + 1];
    }
    else
    {
      AddLine(lpszWork, Attrib);
      lpszWork = &lpszWork[iSpacePos];
    }
    if (lpszWork[0] != 0)
      AddIndent(lpszWork, Attrib);
  }
}

void TOutWin::AddIndent(char *NewLine, unsigned char Attrib)
{
  int iSpacePos;
  char *szTemp1 = new char[strlen(NewLine)];
  assert(szTemp1 != NULL);
  char *szTemp2 = new char[strlen(NewLine)];
  assert(szTemp2 != NULL);
  char *lpszWork = NewLine;
  lpszWork[strlen(lpszWork)] = 0;
  if (strlen(lpszWork) <= ucX2 - ucX1 + 1 - iIndent)
  {
    strcpy(szTemp1, lpszWork);
    for (int i = 0; i < iIndent; i++)
    {
      sprintf(szTemp2, " %s", szTemp1);
      strcpy(szTemp1, szTemp2);
    }
    AddLine(szTemp1, Attrib);
  }
  else
  {
    iSpacePos = SpacePos(lpszWork, ucX2 - ucX1 + 1 - iIndent);
    if (lpszWork[iSpacePos] == 32)
    {
      lpszWork[iSpacePos] = 0;
      strcpy(szTemp1, lpszWork);
      for (int i = 0; i < iIndent; i++)
      {
        sprintf(szTemp2, " %s", szTemp1);
        strcpy(szTemp1, szTemp2);
      }
      AddLine(szTemp1, Attrib);
      lpszWork = &lpszWork[iSpacePos + 1];
    }
    else
    {
      strcpy(szTemp1, lpszWork);
      for (int i = 0; i < iIndent; i++)
      {
        sprintf(szTemp2, " %s", szTemp1);
        strcpy(szTemp1, szTemp2);
      }
      AddLine(szTemp1, Attrib);
      lpszWork = &lpszWork[iSpacePos];
    }
    if (lpszWork[0] != 0)
      AddIndent(lpszWork, Attrib);
  }
  delete (szTemp1);
  delete (szTemp2);
}

int TOutWin::SpacePos(char *SearchLine, int Length)
{
  int iSpacePos = -1;
  for (int i = Length; i >= 0; i--)
    if (SearchLine[i] == 32)
      if (iSpacePos == -1)
        iSpacePos = i;
  if (iSpacePos == -1)
    iSpacePos = Length;
  return (iSpacePos);
}

void TOutWin::PageUp(int Lines)
{
  //move backward in data buffer
  int iChange = iCurrentLine;
  for (int i = 0; i < Lines; i++)
    if (Buffer[iCurrentLine - (ucY2 - ucY1 + 2)].szString[0] != 0)
      if (iCurrentLine > ucY2 - ucY1 + 3)
        iCurrentLine--; //with a little working room
  if (iChange != iCurrentLine)
    Redraw();
}

void TOutWin::PageDown(int Lines)
{
  //move forward in the data buffer
  int iChange = iCurrentLine;
  for (int i = 0; i < Lines; i++)
    if (iCurrentLine < iSize)
      iCurrentLine++;
  if (iChange != iCurrentLine)
    Redraw();
}

void TOutWin::SetHeight(unsigned char Y1, unsigned char Y2)
{
  //adjust the Y window size
  ucY1 = Y1;
  ucY2 = Y2;
  Redraw();
}
