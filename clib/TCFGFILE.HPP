//TCfgFile - Config File Reader (INI Style)
//Copyright 1996 Aaron Brinton
//
//*****************************************************************************

#define cMaxItems 1000
#define cReadLength 300

class TCfgFile
{
public:
  TCfgFile(char *Filename);
  ~TCfgFile(void);
  int Items;
  char *Parm(int Index);
  char *String(int Index);
  long Number(int Index);

private:
  struct TData
  {
    char *Parm;
    char *Value;
  } Data[cMaxItems];
};

TCfgFile::TCfgFile(char *Filename)
{
  Items = 0;
  char *szDataStr = new char[cReadLength];
  assert(szDataStr != NULL);
  char *szTemp;
  char *szParm = new char[cReadLength];
  assert(szParm != NULL);
  char *szValue = new char[cReadLength];
  assert(szValue != NULL);
  FILE *CFile;
  CFile = fopen(Filename, "rt");
  if (CFile == NULL)
    Items = -1;
  else
    while (fgets(szDataStr, cReadLength, CFile) != NULL)
    {
      szTemp = strchr(szDataStr, ';');
      szTemp[0] = 0;
      if (szDataStr[0] != 0)
      {
        strcpy(szParm, strtok(szDataStr, "="));
        strcpy(szValue, strtok(NULL, "\n"));
        if ((szParm[0] != 0) && (szValue[0] != 0) && (Items < cMaxItems))
        {
          char *lpszParm = &szParm[0];
          while (lpszParm[0] == 32)
            lpszParm++;
          while (lpszParm[strlen(lpszParm) - 1] == 32)
            lpszParm[strlen(lpszParm) - 1] = 0;
          char *lpszValue = &szValue[0];
          while (lpszValue[0] == 32)
            lpszValue++;
          while (lpszValue[strlen(lpszValue) - 1] == 32)
            lpszValue[strlen(lpszValue) - 1] = 0;
          Data[Items].Parm = new char[strlen(lpszParm)];
          assert(Data[Items].Parm != NULL);
          Data[Items].Value = new char[strlen(lpszValue)];
          assert(Data[Items].Value != NULL);
          strcpy(Data[Items].Parm, lpszParm);
          strcpy(Data[Items].Value, lpszValue);
          Items++;
        }
      }
    }
  fclose(CFile);
  delete (szDataStr);
  delete (szParm);
  delete (szValue);
}

TCfgFile::~TCfgFile(void)
{
  if (Items > 0)
    for (int i = 0; i < Items; i++)
    {
      delete (Data[i].Parm);
      delete (Data[i].Value);
    }
}

char *TCfgFile::Parm(int Index)
{
  return (Data[Index].Parm);
}

char *TCfgFile::String(int Index)
{
  return (Data[Index].Value);
}

long TCfgFile::Number(int Index)
{
  long lReturn;
  char *lpszEnd = &Data[Index].Value[strlen(Data[Index].Value) - 1];
  lReturn = strtol(Data[Index].Value, &lpszEnd, 10);
  return (lReturn);
}
