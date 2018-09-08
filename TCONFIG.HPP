//GMON TConfig
//Copyright 1996 Aaron Brinton

#define cMaxCommands        100
#define cMaxCommandLength    10
#define cMaxMacros           36
#define cMaxMacroLength     200
#define cMaxStrings          30
#define cMaxStringLength     60
#define cMaxColors           30

#include "..\clib\tcfgfile.hpp"

class TConfig {
  int GetColor(char *ColorName);
  int GetBoolean(char *Value);
  public:
    TConfig(void);
    ~TConfig(void);

    int ReadConfig(void);
    void DeleteConfig(void);
    //internal
    int UpdateInterval; int MaxSockets;

    //darkside
    int Debug; char DebugColor;
    int DarkSide; char DarkSideColor;
    int AllMsgs;

    //general
    char Nickname[16];
    char Channel[16];
    char ByeMessage[200];

    int BeepOnArrival;
    int BeepOnPrivate;
    int ShowJoinMsgs;

    char *Macro[cMaxMacros];
    char *Command[cMaxCommands];
    char Color[cMaxColors];
    char *String[cMaxStrings];

  //colors  ********************TRASH!!!!!!!!!!!!
  int JoinColor;
  int PrivateSendColor;
//  int GetColor;
  int NoOfferColor;
  int DeOfferColor;
  int TransferColor;
  int DeofferColor;
  int NoPrivateColor;
  int ErrorColor;
  int UsersColor;
  int EmailColor;
  int ServersColor;
  int MotdColor;
  int WhoisColor;
  int DosColor;
  int UserListColor;
  //sounds
  int JoinFrequency;
  int JoinDuration;
  int PrivateFrequency;
  int PrivateDuration;
  };

TConfig::TConfig(void) {
  for(int i = 0; i < cMaxMacros; i++) {Macro[i] = new char[cMaxMacroLength]; assert(Macro[i] != NULL); Macro[i][0] = 0;}
  for(i = 0; i < cMaxCommands; i++) {Command[i] = new char[cMaxCommandLength]; assert(Command[i] != NULL); Command[i][0] = 0;}
  for(i = 0; i < cMaxColors; i++) Color[i] = 0;
  for(i = 0; i < cMaxStrings; i++) {String[i] = new char[cMaxStringLength]; assert(String[i] != NULL); String[i][0] = 0;}

  //internal settings
  UpdateInterval    = 5;
  MaxSockets        = 99;
#ifdef DARKSIDE
  Debug = 0; DebugColor = DARKGRAY; DarkSide = 0; DarkSideColor = MAGENTA; AllMsgs = 0;
#endif

  //set defaults
  strcpy(Nickname,   "Newbie");
  strcpy(Channel,    "#chat");
  strcpy(ByeMessage, "Leaving");
  BeepOnArrival = 0;
  BeepOnPrivate = 0;
  ShowJoinMsgs  = 0;



  //these are trash
  JoinColor         = CYAN;
  PrivateSendColor  = YELLOW;
//  GetColor          = BROWN;
  NoOfferColor      = BROWN;
  TransferColor     = BROWN;
  ErrorColor        = RED;
  DeofferColor      = CYAN;
  NoPrivateColor    = BROWN;
  UsersColor        = CYAN;
  EmailColor        = CYAN;
  ServersColor      = CYAN;
  MotdColor         = CYAN;
  WhoisColor        = CYAN;
  DosColor          = LIGHTMAGENTA;
  UserListColor     = LIGHTGREEN;
  JoinFrequency     = 400;
  JoinDuration      = 40;
  PrivateFrequency  = 800;
  PrivateDuration   = 60;
  }

TConfig::~TConfig(void) {
  for(int i = 0; i < 36; i++) delete(Macro[i]);
  for(i = 0; i < 100; i++) delete(Command[i]);
  for(i = 0; i < 100; i++) delete(String[i]);
  }

int TConfig::GetColor(char *ColorName) {
  if(stricmp(ColorName, "blue"        ) == 0) return( 1);
  if(stricmp(ColorName, "green"       ) == 0) return( 2);
  if(stricmp(ColorName, "cyan"        ) == 0) return( 3);
  if(stricmp(ColorName, "red"         ) == 0) return( 4);
  if(stricmp(ColorName, "magenta"     ) == 0) return( 5);
  if(stricmp(ColorName, "brown"       ) == 0) return( 6);
  if(stricmp(ColorName, "lightgray"   ) == 0) return( 7);
  if(stricmp(ColorName, "darkgray"    ) == 0) return( 8);
  if(stricmp(ColorName, "lightblue"   ) == 0) return( 9);
  if(stricmp(ColorName, "lightgreen"  ) == 0) return(10);
  if(stricmp(ColorName, "lightcyan"   ) == 0) return(11);
  if(stricmp(ColorName, "lightred"    ) == 0) return(12);
  if(stricmp(ColorName, "lightmagenta") == 0) return(13);
  if(stricmp(ColorName, "yellow"      ) == 0) return(14);
  if(stricmp(ColorName, "white"       ) == 0) return(15);
  return(0);
  }

int TConfig::GetBoolean(char *Value) {
  if(stricmp(Value, "yes" ) == 0) return(1);
  if(stricmp(Value, "true") == 0) return(1);
  if(stricmp(Value, "1") == 0) return(1);
  return(0);
  }

int TConfig::ReadConfig(void) {
  int iResult = 0;
  char *szFilename = new char[80]; assert(szFilename != NULL);
  strcpy(szFilename, _argv[0]);
  char *lpszTemp = strrchr(szFilename, 92);
  lpszTemp[1] = 0;
  strcat(szFilename, "gmon.cfg");
  TCfgFile *CfgFile;
  CfgFile = new TCfgFile(szFilename);
  iResult = CfgFile->Items;
  delete(szFilename);
  if(CfgFile->Items > 0) for(int i = 0; i < CfgFile->Items; i++) {
    //general
    if(stricmp(CfgFile->Parm(i), "Nickname")      == 0) strncpy(Nickname, CfgFile->String(i), 16); Nickname[15] = 0;
    if(stricmp(CfgFile->Parm(i), "Channel")       == 0) strncpy(Channel, CfgFile->String(i), 16); Channel[15] = 0;
    if(stricmp(CfgFile->Parm(i), "ByeMessage")    == 0) strncpy(ByeMessage, CfgFile->String(i), 200); ByeMessage[199] = 0;

    //notification
    if(stricmp(CfgFile->Parm(i), "BeepOnArrival") == 0) BeepOnArrival = GetBoolean(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "BeepOnPrivate") == 0) BeepOnPrivate = GetBoolean(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "ShowJoinMsgs")  == 0) ShowJoinMsgs  = GetBoolean(CfgFile->String(i));

    //macros
    if(stricmp(CfgFile->Parm(i), "Macro1")        == 0) strncpy(Macro[ 0], CfgFile->String(i), cMaxMacroLength); Macro[ 0][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "Macro2")        == 0) strncpy(Macro[ 1], CfgFile->String(i), cMaxMacroLength); Macro[ 1][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "Macro3")        == 0) strncpy(Macro[ 2], CfgFile->String(i), cMaxMacroLength); Macro[ 2][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "Macro4")        == 0) strncpy(Macro[ 3], CfgFile->String(i), cMaxMacroLength); Macro[ 3][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "Macro5")        == 0) strncpy(Macro[ 4], CfgFile->String(i), cMaxMacroLength); Macro[ 4][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "Macro6")        == 0) strncpy(Macro[ 5], CfgFile->String(i), cMaxMacroLength); Macro[ 5][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "Macro7")        == 0) strncpy(Macro[ 6], CfgFile->String(i), cMaxMacroLength); Macro[ 6][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "Macro8")        == 0) strncpy(Macro[ 7], CfgFile->String(i), cMaxMacroLength); Macro[ 7][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "Macro9")        == 0) strncpy(Macro[ 8], CfgFile->String(i), cMaxMacroLength); Macro[ 8][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "Macro0")        == 0) strncpy(Macro[ 9], CfgFile->String(i), cMaxMacroLength); Macro[ 9][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroA")        == 0) strncpy(Macro[10], CfgFile->String(i), cMaxMacroLength); Macro[10][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroB")        == 0) strncpy(Macro[11], CfgFile->String(i), cMaxMacroLength); Macro[11][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroC")        == 0) strncpy(Macro[12], CfgFile->String(i), cMaxMacroLength); Macro[12][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroD")        == 0) strncpy(Macro[13], CfgFile->String(i), cMaxMacroLength); Macro[13][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroE")        == 0) strncpy(Macro[14], CfgFile->String(i), cMaxMacroLength); Macro[14][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroF")        == 0) strncpy(Macro[15], CfgFile->String(i), cMaxMacroLength); Macro[15][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroG")        == 0) strncpy(Macro[16], CfgFile->String(i), cMaxMacroLength); Macro[16][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroH")        == 0) strncpy(Macro[17], CfgFile->String(i), cMaxMacroLength); Macro[17][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroI")        == 0) strncpy(Macro[18], CfgFile->String(i), cMaxMacroLength); Macro[18][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroJ")        == 0) strncpy(Macro[19], CfgFile->String(i), cMaxMacroLength); Macro[19][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroK")        == 0) strncpy(Macro[20], CfgFile->String(i), cMaxMacroLength); Macro[20][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroL")        == 0) strncpy(Macro[21], CfgFile->String(i), cMaxMacroLength); Macro[21][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroM")        == 0) strncpy(Macro[22], CfgFile->String(i), cMaxMacroLength); Macro[22][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroN")        == 0) strncpy(Macro[23], CfgFile->String(i), cMaxMacroLength); Macro[23][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroO")        == 0) strncpy(Macro[24], CfgFile->String(i), cMaxMacroLength); Macro[24][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroP")        == 0) strncpy(Macro[25], CfgFile->String(i), cMaxMacroLength); Macro[25][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroQ")        == 0) strncpy(Macro[26], CfgFile->String(i), cMaxMacroLength); Macro[26][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroR")        == 0) strncpy(Macro[27], CfgFile->String(i), cMaxMacroLength); Macro[27][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroS")        == 0) strncpy(Macro[28], CfgFile->String(i), cMaxMacroLength); Macro[28][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroT")        == 0) strncpy(Macro[29], CfgFile->String(i), cMaxMacroLength); Macro[29][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroU")        == 0) strncpy(Macro[30], CfgFile->String(i), cMaxMacroLength); Macro[30][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroV")        == 0) strncpy(Macro[31], CfgFile->String(i), cMaxMacroLength); Macro[31][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroW")        == 0) strncpy(Macro[32], CfgFile->String(i), cMaxMacroLength); Macro[32][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroX")        == 0) strncpy(Macro[33], CfgFile->String(i), cMaxMacroLength); Macro[33][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroY")        == 0) strncpy(Macro[34], CfgFile->String(i), cMaxMacroLength); Macro[34][cMaxMacroLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "MacroZ")        == 0) strncpy(Macro[35], CfgFile->String(i), cMaxMacroLength); Macro[35][cMaxMacroLength - 1] = 0;

    //commands
    if(stricmp(CfgFile->Parm(i), "CmdQuit")       == 0) strncpy(Command[ 0], CfgFile->String(i), cMaxCommandLength); Command[ 0][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdQuit1")      == 0) strncpy(Command[ 1], CfgFile->String(i), cMaxCommandLength); Command[ 1][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdQuit2")      == 0) strncpy(Command[ 2], CfgFile->String(i), cMaxCommandLength); Command[ 2][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdQuit3")      == 0) strncpy(Command[ 3], CfgFile->String(i), cMaxCommandLength); Command[ 3][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdQuit4")      == 0) strncpy(Command[ 4], CfgFile->String(i), cMaxCommandLength); Command[ 4][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdQuit5")      == 0) strncpy(Command[ 5], CfgFile->String(i), cMaxCommandLength); Command[ 5][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdQuit6")      == 0) strncpy(Command[ 6], CfgFile->String(i), cMaxCommandLength); Command[ 6][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdQuit7")      == 0) strncpy(Command[ 7], CfgFile->String(i), cMaxCommandLength); Command[ 7][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdQuit8")      == 0) strncpy(Command[ 8], CfgFile->String(i), cMaxCommandLength); Command[ 8][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdQuit9")      == 0) strncpy(Command[ 9], CfgFile->String(i), cMaxCommandLength); Command[ 9][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdHelp")       == 0) strncpy(Command[10], CfgFile->String(i), cMaxCommandLength); Command[10][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdChat")       == 0) strncpy(Command[11], CfgFile->String(i), cMaxCommandLength); Command[11][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdUsers")      == 0) strncpy(Command[12], CfgFile->String(i), cMaxCommandLength); Command[12][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdEmail")      == 0) strncpy(Command[13], CfgFile->String(i), cMaxCommandLength); Command[13][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdWhois")      == 0) strncpy(Command[14], CfgFile->String(i), cMaxCommandLength); Command[14][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdServers")    == 0) strncpy(Command[15], CfgFile->String(i), cMaxCommandLength); Command[15][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdMotd")       == 0) strncpy(Command[16], CfgFile->String(i), cMaxCommandLength); Command[16][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdAway")       == 0) strncpy(Command[17], CfgFile->String(i), cMaxCommandLength); Command[17][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdBeep")       == 0) strncpy(Command[18], CfgFile->String(i), cMaxCommandLength); Command[18][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdClear")      == 0) strncpy(Command[19], CfgFile->String(i), cMaxCommandLength); Command[19][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdIgnore")     == 0) strncpy(Command[20], CfgFile->String(i), cMaxCommandLength); Command[20][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdJoin")       == 0) strncpy(Command[21], CfgFile->String(i), cMaxCommandLength); Command[21][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdMe")         == 0) strncpy(Command[22], CfgFile->String(i), cMaxCommandLength); Command[22][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdMsg")        == 0) strncpy(Command[23], CfgFile->String(i), cMaxCommandLength); Command[23][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdPing")       == 0) strncpy(Command[24], CfgFile->String(i), cMaxCommandLength); Command[24][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdWho")        == 0) strncpy(Command[25], CfgFile->String(i), cMaxCommandLength); Command[25][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdOffer")      == 0) strncpy(Command[26], CfgFile->String(i), cMaxCommandLength); Command[26][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdDeoffer")    == 0) strncpy(Command[27], CfgFile->String(i), cMaxCommandLength); Command[27][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdGet")        == 0) strncpy(Command[28], CfgFile->String(i), cMaxCommandLength); Command[28][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdXGet")       == 0) strncpy(Command[29], CfgFile->String(i), cMaxCommandLength); Command[29][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdProgress")   == 0) strncpy(Command[30], CfgFile->String(i), cMaxCommandLength); Command[30][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdCD")         == 0) strncpy(Command[31], CfgFile->String(i), cMaxCommandLength); Command[31][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdDir")        == 0) strncpy(Command[32], CfgFile->String(i), cMaxCommandLength); Command[32][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdType")       == 0) strncpy(Command[33], CfgFile->String(i), cMaxCommandLength); Command[33][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdShell")      == 0) strncpy(Command[34], CfgFile->String(i), cMaxCommandLength); Command[34][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdNoPrivate")  == 0) strncpy(Command[35], CfgFile->String(i), cMaxCommandLength); Command[35][cMaxCommandLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "CmdDos")        == 0) strncpy(Command[36], CfgFile->String(i), cMaxCommandLength); Command[36][cMaxCommandLength - 1] = 0;

    //colors & strings
    if(stricmp(CfgFile->Parm(i), "MessageColor")      == 0) Color[0] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "MessageStr")        == 0) strncpy(String[0], CfgFile->String(i), cMaxStringLength); String[0][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "HelloColor")        == 0) Color[1] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "HelloStr")          == 0) strncpy(String[1], CfgFile->String(i), cMaxStringLength); String[1][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "ByeColor")          == 0) Color[2] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "ByeStr")            == 0) strncpy(String[2], CfgFile->String(i), cMaxStringLength); String[2][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "PartColor")         == 0) Color[3] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "PartStr")           == 0) strncpy(String[3], CfgFile->String(i), cMaxStringLength); String[3][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "IamColor")          == 0) Color[4] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "IamStr")            == 0) strncpy(String[4], CfgFile->String(i), cMaxStringLength); String[4][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "PostureColor")      == 0) Color[5] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "PostureStr")        == 0) strncpy(String[5], CfgFile->String(i), cMaxStringLength); String[5][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "PrivateRecvColor")  == 0) Color[6] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "PrivateRecvStr")    == 0) strncpy(String[6], CfgFile->String(i), cMaxStringLength); String[6][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "PongColor")         == 0) Color[7] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "PongStr")           == 0) strncpy(String[7], CfgFile->String(i), cMaxStringLength); String[7][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "DisconnectColor")   == 0) Color[8] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "DisconnectStr")     == 0) strncpy(String[8], CfgFile->String(i), cMaxStringLength); String[8][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "IgnoreColor")       == 0) Color[9] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "IgnoreStr")         == 0) strncpy(String[9], CfgFile->String(i), cMaxStringLength); String[9][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "PrivateSendColor")  == 0) Color[10] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "PrivateSendStr")    == 0) strncpy(String[10], CfgFile->String(i), cMaxStringLength); String[10][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "NoPrivateColor")    == 0) Color[11] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "NoPrivateStr")      == 0) strncpy(String[11], CfgFile->String(i), cMaxStringLength); String[11][cMaxStringLength - 1] = 0;



    if(stricmp(CfgFile->Parm(i), "NoFilesOfferedStr") == 0) strncpy(String[cMaxStrings - 11], CfgFile->String(i), cMaxStringLength); String[89][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "FileNotFoundStr")   == 0) strncpy(String[cMaxStrings - 10], CfgFile->String(i), cMaxStringLength); String[90][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "InvalidFileStr")    == 0) strncpy(String[cMaxStrings -  9], CfgFile->String(i), cMaxStringLength); String[91][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "NotGameModeStr")    == 0) strncpy(String[cMaxStrings -  8], CfgFile->String(i), cMaxStringLength); String[92][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "InvalidSocketStr")  == 0) strncpy(String[cMaxStrings -  7], CfgFile->String(i), cMaxStringLength); String[93][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "KaliNotLoadedStr")  == 0) strncpy(String[cMaxStrings -  6], CfgFile->String(i), cMaxStringLength); String[94][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "RejectPrivateStr")  == 0) strncpy(String[cMaxStrings -  5], CfgFile->String(i), cMaxStringLength); String[95][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "InvalidChannelStr") == 0) strncpy(String[cMaxStrings -  4], CfgFile->String(i), cMaxStringLength); String[96][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "SameChannelStr")    == 0) strncpy(String[cMaxStrings -  3], CfgFile->String(i), cMaxStringLength); String[97][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "InvalidCommandStr") == 0) strncpy(String[cMaxStrings -  2], CfgFile->String(i), cMaxStringLength); String[98][cMaxStringLength - 1] = 0;
    if(stricmp(CfgFile->Parm(i), "UnknownUserStr")    == 0) strncpy(String[cMaxStrings -  1], CfgFile->String(i), cMaxStringLength); String[99][cMaxStringLength - 1] = 0;

    if(stricmp(CfgFile->Parm(i), "ErrorColor")        == 0) Color[cMaxColors - 1] = GetColor(CfgFile->String(i));


    if(stricmp(CfgFile->Parm(i), "LineColor")         == 0) Color[40] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "TitleColor")        == 0) Color[41] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "ScreenNameColor")   == 0) Color[42] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "ClockColor")        == 0) Color[43] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "HelpTextColor")     == 0) Color[44] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "SendProgressColor") == 0) Color[45] = GetColor(CfgFile->String(i));
    if(stricmp(CfgFile->Parm(i), "RecvProgressColor") == 0) Color[46] = GetColor(CfgFile->String(i));



    }
  delete(CfgFile);
  return(iResult);
  }



