//PC TD2Game
//Copyright 1996 Aaron Brinton

//#define cScreenNameColor     WHITE
//#define cHeadingColor        LIGHTGREEN
//#define cDataColor           YELLOW

class TD2Game {
  void RefreshData(void);
  void RefreshList(void);
  void RefreshGame(void);
  public:
    TD2Game(void);
    ~TD2Game(void);
    void Startup(void);
    void Run(void);
    void SetSocket(int iSocket);
    void DrawList(void);
    void DrawGame(void);
  private:
    Tdpack *DPack;
    struct time UpdateTime;
    int iTimer;
    int iUpdate;
    int iCurrentView;
    int iCurrentSocket;
  };

TD2Game::TD2Game(void) {
  iTimer = 0;
  iUpdate = 5;
  }

TD2Game::~TD2Game(void) {
//  delete(DPack);
  }

void TD2Game::Startup(void) {
//  DPack = new Tdpack;
//  DPack->Start(0);
  }

void TD2Game::Run(void) {
//  DPack->EnterIdle();
  gettime(&UpdateTime);
  if(iTimer != UpdateTime.ti_sec) iUpdate++;
  iTimer = UpdateTime.ti_sec;
//  if(iUpdate >= cUpdateInterval) RefreshData();
  }

void TD2Game::SetSocket(int iSocket) {
  iCurrentSocket = iSocket;
  }

void TD2Game::DrawList(void) {
/*  int wx = wherex();
  int wy = wherey();
  NewScreen();
  iCurrentView = 1;
  PutString(53, 2, "* Game List *", cScreenNameColor);
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
  //asdf
  gotoxy(wx, wy);
*/  }

void TD2Game::RefreshList(void) {
/*  PutString(76, 2, "    ", cDataColor);
  window(4, 6, 79, 9);
  clrscr();
  window(1, 1, 80, 50);

  char sTemp[6];
  if(iCurrentSocket > 0) sprintf(sTemp, "+%d", iCurrentSocket);
  if(iCurrentSocket <= 0) sprintf(sTemp, "%d", iCurrentSocket);
  RightJustify(sTemp, 4);
  PutString(76, 2, sTemp, cDataColor);
*/  }

void TD2Game::DrawGame(void) {
  iCurrentView = 2;
  }

void TD2Game::RefreshGame(void) {
  }

void TD2Game::RefreshData(void) {
  iUpdate = 0;
  int wx = wherex();
  int wy = wherey();
  if(iCurrentView == 1) RefreshList();
  //asdf
  gotoxy(wx, wy);
  DPack->Refresh();
  }

