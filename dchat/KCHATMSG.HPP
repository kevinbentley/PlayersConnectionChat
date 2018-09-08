Message *Tkchat::GetMessageFromQueue(void)
{
	Message *TempMessage;
	if (FirstMsgQ->Msg.Message[0] == NULL)
		return NULL;
	TempMsgQ = FirstMsgQ;
	TempMessage = &FirstMsgQ->Msg;
	TmpMsg.Type = TempMessage->Type;
	strcpy(TmpMsg.Channel, TempMessage->Channel);
	strcpy(TmpMsg.Nickname, TempMessage->Nickname);
	strcpy(TmpMsg.Message, TempMessage->Message);
	FirstMsgQ = FirstMsgQ->Next;
	delete TempMsgQ;
	return &TmpMsg;
}

int Tkchat::PutMessageOnQueue(Message *Msg)
{
	CurrMsgQ->Msg.Sequence = Msg->Sequence;
	CurrMsgQ->Msg.Type = (int)Msg->Type;
	strcpy(CurrMsgQ->Msg.Nickname, Msg->Nickname);
	if (Msg->Channel[0] == NULL)
		strcpy(Msg->Channel, ThisChannel);
	if (Msg->Message[0] == NULL)
		strcpy(CurrMsgQ->Msg.Message, " ");
	else
		strcpy(CurrMsgQ->Msg.Message, Msg->Message);
	strcpy(CurrMsgQ->Msg.Channel, Msg->Channel);
	CurrMsgQ->Next = new struct MessageQueue;
	//assert(CurrMsgQ->Next);
	CurrMsgQ = CurrMsgQ->Next;
	CurrMsgQ->Msg.Message[0] = NULL;
	return NULL;
}

int Tkchat::SendPrivateMessage(char *Msg, char *NickName)
{
	if (iLoopSendECB >= NUMMSGECB - 2)
		iLoopSendECB++;
	else
		iLoopSendECB = 0;
	SendPacket = &SendECBBuffer[iLoopSendECB];

	IPXAddress far *TargetAddr = NULL;
	int NotIt = 1, z;

	Message *sMsg = &SndBuffer[SndPos];

	for (z = 0; z < MAXUSERS; z++)
	{
		if (stricmp(NickName, Users[z].Name) == 0)
			NotIt = 0;
	}
	if (NotIt)
		return 1;

	SndBuffer[SndPos].Type = KCHATPKTPRIVATE;
	SndBuffer[SndPos].Sequence = Counter;
	strcpy(SndBuffer[SndPos].Message, Msg);
	strcpy(SndBuffer[SndPos].Nickname, ThisUser);
	strcpy(SndBuffer[SndPos].Channel, ThisChannel);

	SendPacket->ESRAddress = NULL;
	SendPacket->socketNumber = KCHATSOCKET;
	SendPacket->fragmentCount = 2;
	SendPacket->fragmentDescriptor[0].address = &SendHeader;
	SendPacket->fragmentDescriptor[0].size = sizeof(SendHeader);
	SendPacket->fragmentDescriptor[1].address = sMsg;
	SendPacket->fragmentDescriptor[1].size = strlen(sMsg->Message) + 51;
	SendHeader.packetType = 0x04; //IPX packet type
	SendHeader.length = sizeof(IPXHeader);
	SendHeader.destination.socket[1] = KCHATSOCKETA;
	SendHeader.destination.socket[0] = KCHATSOCKETB;
	SendBufferedPacket(SendPacket, NickName);
	//Send actual message!!!!!
	return 0;
}

void Tkchat::SendMessage(char *Msg)
{
	SndBuffer[SndPos].Type = KCHATPKTMESSAGE;
	SndBuffer[SndPos].Sequence = Counter;
	strcpy(SndBuffer[SndPos].Message, Msg);
	strcpy(SndBuffer[SndPos].Nickname, ThisUser);
	strcpy(SndBuffer[SndPos].Channel, ThisChannel);
	SendGeneralPacket((KchatPKT *)&SndBuffer[SndPos]);
	if (SndPos >= (NUMMSGECB - 2))
		SndPos = 0;
	else
		SndPos++;
	return;
}

void Tkchat::SendPostureMessage(char *Msg)
{
	SndBuffer[SndPos].Type = KCHATPKTPOSTURE;
	SndBuffer[SndPos].Sequence = Counter;
	strcpy(SndBuffer[SndPos].Message, Msg);
	strcpy(SndBuffer[SndPos].Nickname, ThisUser);
	strcpy(SndBuffer[SndPos].Channel, ThisChannel);
	SendGeneralPacket((KchatPKT *)&SndBuffer[SndPos]);
	if (SndPos >= (NUMMSGECB - 2))
		SndPos = 0;
	else
		SndPos++;
	return;
}

void Tkchat::Away(char *Msg)
{
	strcpy(AwayMsg, Msg);
	//posts an away message - NULL to come back.
	if (Msg != NULL)
	{
		iAway = 1;
		strcpy(AwayMsg, Msg);
	}
	else
	{
		iAway = 0;
		return;
	}
	//posture a message
}

void Tkchat::SendAwayNotice(char *User)
{
	if (iLoopSendECB >= NUMMSGECB - 2)
		iLoopSendECB++;
	else
		iLoopSendECB = 0;
	SendPacket = &SendECBBuffer[iLoopSendECB];

	Message *sMsg = &SndBuffer[SndPos];

	SndBuffer[SndPos].Type = KCHATPKTPOSTURE;
	SndBuffer[SndPos].Sequence = Counter;
	strcpy(SndBuffer[SndPos].Message, AwayMsg);
	strcpy(SndBuffer[SndPos].Nickname, ThisUser);
	strcpy(SndBuffer[SndPos].Channel, ThisChannel);

	SendPacket->ESRAddress = NULL;
	SendPacket->socketNumber = KCHATSOCKET;
	SendPacket->fragmentCount = 2;
	SendPacket->fragmentDescriptor[0].address = &SendHeader;
	SendPacket->fragmentDescriptor[0].size = sizeof(SendHeader);
	SendPacket->fragmentDescriptor[1].address = sMsg;
	SendPacket->fragmentDescriptor[1].size = strlen(sMsg->Message) + 51;
	SendHeader.packetType = 0x04; //IPX packet type
	SendHeader.length = sizeof(IPXHeader);
	SendHeader.destination.socket[1] = KCHATSOCKETA;
	SendHeader.destination.socket[0] = KCHATSOCKETB;

	SendBufferedPacket(SendPacket, User);
	return;
}

void Tkchat::SendNoPrivateMsg(char *User)
{
	if (iLoopSendECB >= NUMMSGECB - 2)
		iLoopSendECB++;
	else
		iLoopSendECB = 0;
	SendPacket = &SendECBBuffer[iLoopSendECB];

	Message *sMsg = &SndBuffer[SndPos];

	SndBuffer[SndPos].Type = KCHATPKTPOSTURE;
	SndBuffer[SndPos].Sequence = Counter;
	strcpy(SndBuffer[SndPos].Message, NoPrivateMsg);
	strcpy(SndBuffer[SndPos].Nickname, ThisUser);
	strcpy(SndBuffer[SndPos].Channel, ThisChannel);

	SendPacket->ESRAddress = NULL;
	SendPacket->socketNumber = KCHATSOCKET;
	SendPacket->fragmentCount = 2;
	SendPacket->fragmentDescriptor[0].address = &SendHeader;
	SendPacket->fragmentDescriptor[0].size = sizeof(SendHeader);
	SendPacket->fragmentDescriptor[1].address = sMsg;
	SendPacket->fragmentDescriptor[1].size = strlen(sMsg->Message) + 51;
	SendHeader.packetType = 0x04; //IPX packet type
	SendHeader.length = sizeof(IPXHeader);
	SendHeader.destination.socket[1] = KCHATSOCKETA;
	SendHeader.destination.socket[0] = KCHATSOCKETB;

	SendBufferedPacket(SendPacket, User);
	return;
}

void Tkchat::NoPrivate(char *Msg)
{
	strcpy(AwayMsg, Msg);
	//posts an away message - NULL to come back.
	if (Msg != NULL)
	{
		iNoPrivate = 1;
		strcpy(NoPrivateMsg, Msg);
	}
	else
	{
		iNoPrivate = 0;
		strcpy(AwayMsg, "");
		return;
	}
}

void Tkchat::SendBasicMessage(char *Msg, int MsgSize, int Type, char *DestUser)
{

	if (iLoopSendECB >= NUMMSGECB - 2)
		iLoopSendECB++;
	else
		iLoopSendECB = 0;
	SendPacket = &SendECBBuffer[iLoopSendECB];

	Message *sMsg = &SndBuffer[SndPos];

	SndBuffer[SndPos].Sequence = Counter;

	SndBuffer[SndPos].Type = Type;
	memcpy(SndBuffer[SndPos].Message, Msg, MsgSize);
	strcpy(SndBuffer[SndPos].Nickname, ThisUser);
	strcpy(SndBuffer[SndPos].Channel, ThisChannel);

	SendPacket->ESRAddress = NULL;
	SendPacket->socketNumber = KCHATSOCKET;
	SendPacket->fragmentCount = 2;
	SendPacket->fragmentDescriptor[0].address = &SendHeader;
	SendPacket->fragmentDescriptor[0].size = sizeof(SendHeader);
	SendPacket->fragmentDescriptor[1].address = sMsg;
	SendPacket->fragmentDescriptor[1].size = MsgSize + 51;
	SendHeader.packetType = 0x04; //IPX packet type
	SendHeader.length = sizeof(IPXHeader);
	SendHeader.destination.socket[1] = KCHATSOCKETA;
	SendHeader.destination.socket[0] = KCHATSOCKETB;
	SendBufferedPacket(SendPacket, DestUser);
	return;
}
