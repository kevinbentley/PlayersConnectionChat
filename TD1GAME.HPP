//GMON TD1Game
//Copyright 1996 Aaron Brinton

#define cListItems 4
//#define cScreenNameColor     WHITE
//#define cHeadingColor        LIGHTGREEN
//#define cDataColor           YELLOW

class TD1Game
{
  void RefreshData(void);
  void RefreshList(void);
  void RefreshGame(void);

public:
  TD1Game(void);
  ~TD1Game(void);
  void Startup(void);
  void Run(void);
  void SetSocket(int iSocket);
  void DrawList(void);
  void DrawGame(void);

private:
  Tdpack *DPack;
  int iCurrentView;
  int iCurrentSocket;
  netgame_info GameInfo[8];
};

TD1Game::TD1Game(void)
{
}

TD1Game::~TD1Game(void)
{
  delete (DPack);
}

void TD1Game::Startup(void)
{
  DPack = new Tdpack;
  iCurrentSocket = 0;
  DPack->Start(0);
  for (int i = 0; i < cListItems; i++)
    strcpy(GameInfo[i].game_name, "");
}

void TD1Game::Run(void)
{
  DPack->EnterIdle();
  RefreshData();
}

void TD1Game::SetSocket(int iSocket)
{
  iCurrentSocket = iSocket;
  DPack->Close();
  DPack->Start(iCurrentSocket);
}

void TD1Game::DrawList(void)
{
/*  int wx = wherex();
  int wy = wherey();
  NewScreen();
  iCurrentView = 1;
  PutString(50, 2, "* D1 Game List *", cScreenNameColor);
  PutChar(67, 1, cDblHorzSglBottom, cLineColor);
  PutChar(67, 2, cSglVert, cLineColor);
  PutChar(67, 3, cDblHorzSglTop, cLineColor);
  PutString(68, 2, "Socket:", cScreenNameColor);
  PutString(2, 4, "# Title            Type  Diff  Status    P  CLS MAP  Mission               Lvl", cHeadingColor);
  PutChar(1, 5, cDblVertSglRight, cLineColor);
  HorzSglLine(2, 79, 5);
  PutChar(80, 5, cDblVertSglLeft, cLineColor);
  PutChar(2,  6, 0x31, cHeadingColor);
  PutChar(2,  7, 0x32, cHeadingColor);
  PutChar(2,  8, 0x33, cHeadingColor);
  PutChar(2,  9, 0x34, cHeadingColor);
  PutChar(2, 10, 0x35, cHeadingColor);
  PutChar(2, 11, 0x36, cHeadingColor);
  PutChar(2, 12, 0x37, cHeadingColor);
  PutChar(2, 13, 0x38, cHeadingColor);
  PutChar(1, 14, cDblVertDblRight, cLineColor);
  HorzDblLine(2, 79, 14);
  PutChar(80, 14, cDblVertDblLeft, cLineColor);
  gotoxy(wx, wy);
*/  }

void TD1Game::RefreshList(void)
{
/*  PutString(76, 2, "    ", cDataColor);
  window(4, 6, 79, 9);
  clrscr();
  window(1, 1, 80, 50);

  char sTemp[6];
  if(iCurrentSocket > 0) sprintf(sTemp, "+%d", iCurrentSocket);
  if(iCurrentSocket <= 0) sprintf(sTemp, "%d", iCurrentSocket);
  RightJustify(sTemp, 4);
  PutString(76, 2, sTemp, cDataColor);
  for(int i = 0; i < cListItems; i++) {
    if(GameInfo[i].type > 0) {
      PutString( 4, i + 6, GameInfo[i].game_name, cDataColor);
      switch(GameInfo[i].gamemode) {
        case NETGAME_ANARCHY:       {PutString(21, i + 6, "Arch", cDataColor); break;};
        case NETGAME_TEAM_ANARCHY:  {PutString(21, i + 6, "Team", cDataColor); break;};
        case NETGAME_ROBOT_ANARCHY: {PutString(21, i + 6, "Robo", cDataColor); break;};
        case NETGAME_COOPERATIVE:   {PutString(21, i + 6, "Coop", cDataColor); break;};
        }
      switch(GameInfo[i].difficulty) {
        case 0: {PutString(27, i + 6, "Novc", cDataColor); break;};
        case 1: {PutString(27, i + 6, "Rook", cDataColor); break;};
        case 2: {PutString(27, i + 6, "Htsh", cDataColor); break;};
        case 3: {PutString(27, i + 6, "Ace ", cDataColor); break;};
        case 4: {PutString(27, i + 6, "Insn", cDataColor); break;};
        }
      switch(GameInfo[i].game_status) {
        case NETSTAT_MENU:     {PutString(33, i + 6, "Menu    ", cDataColor); break;};
        case NETSTAT_PLAYING:  {PutString(33, i + 6, "Playing ", cDataColor); break;};
        case NETSTAT_BROWSING: {PutString(33, i + 6, "Browsing", cDataColor); break;};
        case NETSTAT_WAITING:  {PutString(33, i + 6, "Waiting", cDataColor); break;};
        case NETSTAT_STARTING: {PutString(33, i + 6, "Starting", cDataColor); break;};
        case NETSTAT_ENDLEVEL: {PutString(33, i + 6, "EndLevel", cDataColor); break;};
        }
      unsigned char ply = GameInfo[i].numplayers + 0x30;
      PutChar(43, i + 6, ply, cDataColor);

      if(GameInfo[i].game_flags && NETGAME_FLAG_CLOSED != 0) PutString(46, i + 6, "Yes", cDataColor);
        else PutString(46, i + 6, " No", cDataColor);

      //this doesn't seem to be working correctly
      if(GameInfo[i].game_flags && NETGAME_FLAG_SHOW_MAP != 0) PutString(50, i + 6, "Yes", cDataColor);
        else PutString(50, i + 6, " No", cDataColor);

      PutString(55, i + 6, GameInfo[i].mission_title, cDataColor);

      int iTemp = (int)GameInfo[i].levelnum;
      if(iTemp > 0) sprintf(sTemp, "%d", iTemp);
        else sprintf(sTemp, "S%d", abs(iTemp));
      RightJustify(sTemp, 3);
      PutString(77, i + 6, sTemp, cDataColor);
      }
    }
*/  }

void TD1Game::DrawGame(void)
{
  iCurrentView = 2;
}

void TD1Game::RefreshGame(void)
{
}

void TD1Game::RefreshData(void)
{
  int wx = wherex();
  int wy = wherey();
  netgame_info *lpGameInfo[cListItems];
  for (int i = 0; i < cListItems; i++)
  {
    GameInfo[i].type = 0;
    if (DPack->GetCompletionStatus(i))
    {
      lpGameInfo[i] = DPack->GetGameInfo(i);
      memcpy(&GameInfo[i], lpGameInfo[i], sizeof(netgame_info));
    }
    else
      lpGameInfo[i] = NULL;
  }
  if (iCurrentView == 1)
    RefreshList();
  if (iCurrentView == 2)
    RefreshGame();
  gotoxy(wx, wy);
  DPack->Refresh();
}
