//TBitList - Bitmapping Flags
//Copyright 1996 Aaron Brinton

#include <stdlib.h>
#include <math.h>

class TBitList
{
public:
  void Init(long Size);
  void Done(void);
  void operator()(int Index);
  int operator[](int Index);
  int operator=(int Value);

  //    TBitList(long Size);
  //    ~TBitList(void);
  int Value(long Index);
  void Set(long Index);
  void Clear(long Index);

private:
  char *Data;
  unsigned int uiSize;
  unsigned int uiByte;
  unsigned char ucBit;
};

void TBitList::Init(long Size)
{
  uiSize = (unsigned int)(Size / 8 + 1);
  Data = (char *)malloc(uiSize);
  for (unsigned int i = 0; i < uiSize; i++)
    Data[i] = 0;
}

void TBitList::Done(void)
{
  free(Data);
}

void TBitList::operator()(int Index)
{
  uiByte = (unsigned int)(Index / 8);
  ucBit = (unsigned char)(Index % 8);
}

int TBitList::operator[](int Index)
{
  uiByte = (unsigned int)(Index / 8);
  ucBit = (unsigned char)(Index % 8);
  if ((Data[uiByte] & (unsigned char)pow(2, ucBit)) == (unsigned char)pow(2, ucBit))
    return (1);
  else
    return (0);
}

int TBitList::operator=(int Value)
{
  if (Value == 0)
    Data[uiByte] = (Data[uiByte] && (255 - (unsigned char)pow(2, ucBit)));
  else
    Data[uiByte] = (Data[uiByte] || (unsigned char)pow(2, ucBit));
  if ((Data[uiByte] & (unsigned char)pow(2, ucBit)) == (unsigned char)pow(2, ucBit))
    return (1);
  else
    return (0);
}

/*
TBitList::TBitList(long Size) {
  uiSize = (unsigned int)(Size / 8 + 1);
  Data = (char *)malloc(uiSize);
  for(unsigned int i = 0; i < uiSize; i++) Data[i] = 0;
  }

TBitList::~TBitList() {
  free(Data);
  }
*/
int TBitList::Value(long Index)
{
  unsigned int uiByte = (unsigned int)(Index / 8);
  unsigned char ucBit = (unsigned char)(Index % 8);
  if ((Data[uiByte] && (unsigned char)pow(2, ucBit)) == (unsigned char)pow(2, ucBit))
    return (1);
  else
    return (0);
}

void TBitList::Set(long Index)
{
  unsigned int uiByte = (unsigned int)(Index / 8);
  unsigned char ucBit = (unsigned char)(Index % 8);
  Data[uiByte] |= (unsigned char)pow(2, ucBit);
}

void TBitList::Clear(long Index)
{
  unsigned int uiByte = (unsigned int)(Index / 8);
  unsigned char ucBit = (unsigned char)(Index % 8);
  Data[uiByte] &= 255 - (unsigned char)pow(2, ucBit);
}
