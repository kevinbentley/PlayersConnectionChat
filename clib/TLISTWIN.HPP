//TListWin - Sorted List Window (dos)
//Copyright 1996 Aaron Brinton
//
//*****************************************************************************

#define cMinBuffer 10
#define cMaxBuffer 500

class TListWin
{
  void PutChar(int X, int Y, unsigned char Chr, unsigned char Attr);
  void PutString(int X, int Y, char *S, unsigned char Attr);

public:
  TListWin(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char Attrib, int Size);
  ~TListWin(void);
  void Clear();
  void Redraw();
  void Sort();
  void AddLine(char *NewLine, unsigned char Attrib);
  void PageUp(int Lines);
  void PageDown(int Lines);
  void SetHeight(unsigned char Y1, unsigned char Y2);

private:
  struct TBuffer
  {
    unsigned char ucAttrib;
    char *szString;
  } Buffer[cMaxBuffer];
  unsigned char ucX1, ucY1, ucX2, ucY2, ucAttrib;
  int iSize; //startup constants
  int iCurrentLine;
};

TListWin::TListWin(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char Attrib, int Size)
{
  //store and range check initialization parms
  ucX1 = X1;
  ucY1 = Y1, ucX2 = X2, ucY2 = Y2;
  ucAttrib = Attrib;
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
  iCurrentLine = 0;
  Clear();
}

TListWin::~TListWin(void)
{
  for (int i = 0; i < iSize; i++)
    delete (Buffer[i].szString);
}

void TListWin::PutChar(int X, int Y, unsigned char Chr, unsigned char Attr)
{
  char *lpMemory = (char far *)MK_FP(0xB800, 0x0000);
  lpMemory[((Y - 1) * 160) + ((X - 1) * 2)] = Chr;
  lpMemory[((Y - 1) * 160) + ((X - 1) * 2) + 1] = Attr;
}

void TListWin::PutString(int X, int Y, char *S, unsigned char Attr)
{
  for (int iLoop = 0; iLoop < strlen(S); iLoop++)
    PutChar(X + iLoop, Y, S[iLoop], Attr);
}

void TListWin::Clear()
{
  for (int i = 0; i < iSize; i++)
  {
    Buffer[i].ucAttrib = 0;
    Buffer[i].szString[0] = 0;
  }
  Redraw();
}

void TListWin::Redraw()
{
  struct text_info TextInfo;
  gettextinfo(&TextInfo);
  textattr(ucAttrib);
  window(ucX1, ucY1, ucX2, ucY2);
  clrscr();
  window(1, 1, TextInfo.screenwidth, TextInfo.screenheight);
  for (int i = ucY1; i <= ucY2; i++)
    PutString(ucX1, i, Buffer[iCurrentLine + i - ucY1].szString, Buffer[iCurrentLine + i - ucY1].ucAttrib);
}

void TListWin::Sort()
{
  int i = 0;
  struct TBuffer TempBuffer;
  while (Buffer[i].szString[0] != 0)
    i++;
  for (int j = 0; j < i; j++)
    for (int k = 0; k < i - 1; k++)
      if (stricmp(Buffer[k].szString, Buffer[k + 1].szString) > 0)
      {
        TempBuffer.szString = Buffer[k].szString;
        TempBuffer.ucAttrib = Buffer[k].ucAttrib;
        Buffer[k].szString = Buffer[k + 1].szString;
        Buffer[k].ucAttrib = Buffer[k + 1].ucAttrib;
        Buffer[k + 1].szString = TempBuffer.szString;
        Buffer[k + 1].ucAttrib = TempBuffer.ucAttrib;
      }
}

void TListWin::AddLine(char *NewLine, unsigned char Attrib)
{
  int i = 0;
  while (Buffer[i].szString[0] != 0)
    i++;
  if (i < iSize)
  {
    strcpy(Buffer[i].szString, NewLine);
    Buffer[i].ucAttrib = Attrib;
  }
}

void TListWin::PageUp(int Lines)
{
  //move backward in data buffer
  int iChange = iCurrentLine;
  for (int i = 0; i < Lines; i++)
    if (iCurrentLine > 0)
      iCurrentLine--; //with a little working room
  if (iChange != iCurrentLine)
    Redraw();
}

void TListWin::PageDown(int Lines)
{
  //move forward in the data buffer
  int iChange = iCurrentLine;
  for (int i = 0; i < Lines; i++)
    if (Buffer[iCurrentLine + ucY2 - ucY1 + 1].szString[0] != 0)
      iCurrentLine++;
  if (iChange != iCurrentLine)
    Redraw();
}

void TListWin::SetHeight(unsigned char Y1, unsigned char Y2)
{
  //adjust the Y window size
  ucY1 = Y1;
  ucY2 = Y2;
  Redraw();
}
