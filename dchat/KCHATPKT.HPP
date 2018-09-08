void Tkchat::SndAck(IPXAddress *TargetAddr, KchatPKT *Packet)
{
	if (iLoopSendECB >= NUMMSGECB - 2)
		iLoopSendECB++;
	else
		iLoopSendECB = 0;
	SendPacket = &SendECBBuffer[iLoopSendECB];

	int i;
	int XportTime;

	Ack.Sequence = Counter;
	ltoa(Packet->Sequence, Ack.Message, 10);

	Ack.SomethingA = 0;
	Ack.SomethingB = 0;
	Ack.SomethingC = 0;
	Ack.Type = KCHATPKTACK;

	strcpy(Ack.Nickname, ThisUser);
	strcpy(Ack.Channel, ThisChannel);

	IPXGetLocalTarget((BYTE far *)TargetAddr, (BYTE far *)SendPacket->immediateAddress, &XportTime);

	SendPacket->ESRAddress = NULL;
	SendPacket->socketNumber = KCHATSOCKET;
	SendPacket->fragmentCount = 2;
	SendPacket->fragmentDescriptor[0].address = &SendHeader;
	SendPacket->fragmentDescriptor[0].size = sizeof(SendHeader);
	SendPacket->fragmentDescriptor[1].address = &Ack;
	SendPacket->fragmentDescriptor[1].size = strlen(Ack.Message) + 51;
	SendHeader.packetType = 0x04; //IPX packet type
	SendHeader.length = sizeof(IPXHeader);
	for (i = 0; i < 6; i++)
		SendHeader.destination.node[i] = TargetAddr->node[i];
	for (i = 0; i < 4; i++)
		SendHeader.destination.network[i] = TargetAddr->network[i];
	SendHeader.destination.socket[1] = KCHATSOCKETA;
	SendHeader.destination.socket[0] = KCHATSOCKETB;
	IPXSendPacket(SendPacket);
	Counter++;
	return;
}

void Tkchat::RcvAck(char *szAcker, char *szSequence)
{
	int i = 0, j;
	long lSequence;
	FileData *XferData;
	KchatPKT XferPkt;
	XferData = (FileData *)&XferPkt.Message;
	Message XferMsg;

	lSequence = atol(szSequence);
	//scan for This sequence #

	for (j = 0; j < NUMSENDBUFFS; j++)
	{
		if (pMsgBuffer[j] != NULL)
			if (lSequence == pMsgBuffer[j]->message.Sequence)
				for (i = 0; i < MAXUSERS; i++)
				{
					//now find the acker
					if (stricmp(szAcker, Users[i].Name) == 0)
					{
						pMsgBuffer[j]->Received[i] = 99;
						Users[i].Alive = ACKSTODIE;
						j = NUMSENDBUFFS;
					}
				}
	}

	//**********************************************************
	//*scan the file request structs for this user & sequence #*
	//*if found, update the ack value.                         *
	//**********************************************************
	for (i = 0; i < NUMFILESENDS; i++)
	{

		if (PendingXfers[i].Name[0] != 0)
		{

			if (PendingXfers[i].NextAckToRcv == lSequence)
			{
				fseek(PendingXfers[i].Fhandle, PendingXfers[i].Offset, 0);
				XferData->Offset = PendingXfers[i].Offset;
				//if they are the same, read the next 500 bytes
				XferData->PacketSize = fread(&XferData->Data, 1, 500, PendingXfers[i].Fhandle);
				//increment all the ack variables & stuff
				PendingXfers[i].NextAckToRcv = Counter;
				//set the offset to equal size plus prev. offset.
				PendingXfers[i].Offset = PendingXfers[i].Offset + XferData->PacketSize;
				if (iLoopSendECB >= NUMMSGECB - 2)
					iLoopSendECB++;
				else
					iLoopSendECB = 0;
				SendPacket = &SendECBBuffer[iLoopSendECB];
				Message *sMsg = &SndBuffer[SndPos];
				XferPkt.Type = KCHATPKTTRANSFER;
				XferPkt.Sequence = Counter;

				strcpy(XferPkt.Nickname, ThisUser);
				strcpy(XferPkt.Channel, ThisChannel);

				SendPacket->ESRAddress = NULL;
				SendPacket->socketNumber = KCHATSOCKET;
				SendPacket->fragmentCount = 2;
				SendPacket->fragmentDescriptor[0].address = &SendHeader;
				SendPacket->fragmentDescriptor[0].size = sizeof(SendHeader);
				SendPacket->fragmentDescriptor[1].address = &XferPkt;
				//set the length based on fread's return
				SendPacket->fragmentDescriptor[1].size = XferData->PacketSize + 56;
				SendHeader.packetType = 0x04; //IPX packet type
				SendHeader.length = sizeof(IPXHeader);
				SendHeader.destination.socket[1] = KCHATSOCKETA;
				SendHeader.destination.socket[0] = KCHATSOCKETB;
				SendBufferedPacket(SendPacket, PendingXfers[i].Name);
				//if the end of file is reached, set the name[0] to NULL.
				if (XferData->PacketSize < 500)
				{
					XferMsg.Type = KCHATPKTTRANSFER;
					strcpy(XferMsg.Nickname, "File");
					strcpy(XferMsg.Channel, ThisChannel);
					sprintf(XferMsg.Message, "File transfer to %s completed.", PendingXfers[i].Name);
					PutMessageOnQueue(&XferMsg);
					PendingXfers[i].Name[0] = NULL;
				}
				//send a packet that has the next chunck of
				//data that the user needs.
			}
		}
	} //end of for loop for file xfers

	//couldn't find the user who acked us.
	return;
}

int Tkchat::SendBroadcastPacket(KchatPKT *DataPacket)
{
	int i;

	if (iLoopSendECB >= NUMMSGECB - 2)
		iLoopSendECB++;
	else
		iLoopSendECB = 0;
	SendPacket = &SendECBBuffer[iLoopSendECB];

	SendPacket->ESRAddress = NULL;
	SendPacket->socketNumber = KCHATSOCKET;
	SendPacket->fragmentCount = 2;
	SendPacket->fragmentDescriptor[0].address = &SendHeader;
	SendPacket->fragmentDescriptor[0].size = sizeof(SendHeader);
	SendPacket->fragmentDescriptor[1].address = DataPacket;
	SendPacket->fragmentDescriptor[1].size = strlen(DataPacket->Message) + 51;
	SendHeader.packetType = 0x04; //IPX packet type
	SendHeader.length = sizeof(IPXHeader);
	for (i = 0; i < 6; i++)
		SendHeader.destination.node[i] = 0;
	for (i = 0; i < 4; i++)
		SendHeader.destination.network[i] = 0xff;
	for (i = 0; i < 6; i++)
		SendPacket->immediateAddress[i] = 0xff;
	SendHeader.destination.socket[1] = KCHATSOCKETA;
	SendHeader.destination.socket[0] = KCHATSOCKETB;
	IPXSendPacket(SendPacket);
	Counter++;
	return 1;
}

int Tkchat::SendGeneralPacket(KchatPKT *DataPacket)
{
	if (iLoopSendECB >= NUMMSGECB - 2)
		iLoopSendECB++;
	else
		iLoopSendECB = 0;
	SendPacket = &SendECBBuffer[iLoopSendECB];

	SendPacket->ESRAddress = NULL;
	SendPacket->socketNumber = KCHATSOCKET;
	SendPacket->fragmentCount = 2;
	SendPacket->fragmentDescriptor[0].address = &SendHeader;
	SendPacket->fragmentDescriptor[0].size = sizeof(SendHeader);
	SendPacket->fragmentDescriptor[1].address = DataPacket;
	SendPacket->fragmentDescriptor[1].size = strlen(DataPacket->Message) + 51;
	SendHeader.packetType = 0x04; //IPX packet type
	SendHeader.length = sizeof(IPXHeader);
	SendHeader.destination.socket[1] = KCHATSOCKETA;
	SendHeader.destination.socket[0] = KCHATSOCKETB;
	SendBufferedPacket(SendPacket, NULL);
	return 1;
}

int Tkchat::Ping(char *User)
{
	if (iLoopSendECB >= NUMMSGECB - 2)
		iLoopSendECB++;
	else
		iLoopSendECB = 0;
	SendPacket = &SendECBBuffer[iLoopSendECB];

	char szTime[16];
	Message *sMsg = &SndBuffer[SndPos];

	sprintf(szTime, "%d", clock());
	SndBuffer[SndPos].Type = KCHATPKTPING;
	SndBuffer[SndPos].Sequence = Counter;
	strcpy(SndBuffer[SndPos].Message, szTime);
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
	return 1;
}

void Tkchat::ReceivePing(Message *Msg)
{
	char szStartTime[16];
	strcpy(szStartTime, Msg->Message);
	unsigned long lStartTime, lNow;
	lNow = clock();
	lStartTime = (unsigned long)strtol(szStartTime, (char **)&szStartTime[strlen(szStartTime)], 10);
	sprintf(Msg->Message, "%u", (lNow - lStartTime));
}

void Tkchat::Pong(IPXAddress *TargetAddr, KchatPKT *Packet)
{
	if (iLoopSendECB >= NUMMSGECB - 2)
		iLoopSendECB++;
	else
		iLoopSendECB = 0;
	SendPacket = &SendECBBuffer[iLoopSendECB];

	Message *sMsg = &SndBuffer[SndPos];

	SndBuffer[SndPos].Type = KCHATPKTPONG;
	SndBuffer[SndPos].Sequence = Counter;
	strcpy(SndBuffer[SndPos].Message, Packet->Message);
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
	SendBufferedPacket(SendPacket, Packet->Nickname);
	return;
}

void Tkchat::ListenForMsg(void)
{
	IPXOpenSocket(ChatSocket, 0);
	int i;
	for (i = 0; i < NUMMSGECB; i++)
	{
		IncomingMsg[i].ESRAddress = NULL;
		IncomingMsg[i].socketNumber = KCHATSOCKET;
		IncomingMsg[i].immediateAddress[0] = 0xff;
		IncomingMsg[i].immediateAddress[1] = 0xff;
		IncomingMsg[i].immediateAddress[2] = 0xff;
		IncomingMsg[i].immediateAddress[3] = 0xff;
		IncomingMsg[i].immediateAddress[4] = 0xff;
		IncomingMsg[i].immediateAddress[5] = 0xff;
		IncomingMsg[i].fragmentCount = 2;
		IncomingMsg[i].fragmentDescriptor[0].address = &IncomingHeader[i];
		IncomingMsg[i].fragmentDescriptor[0].size = sizeof(IncomingHeader[i]);
		IncomingMsg[i].fragmentDescriptor[1].address = &Incomingkchat[i];
		IncomingMsg[i].fragmentDescriptor[1].size = sizeof(Incomingkchat[i]);
		IPXListenForPacket(&IncomingMsg[i]);
	} //end of for loop
}

void Tkchat::StopListening(void)
{
	int i;
	for (i = 0; i < NUMMSGECB; i++)
	{
		IPXCancelEvent(&IncomingMsg[i]);
	} //end of cancel ECB Loop;
}

void Tkchat::RefreshECB(int Idx)
{
	Incomingkchat[Idx].Message[0] = NULL;
	IncomingMsg[Idx].ESRAddress = NULL;
	IncomingMsg[Idx].socketNumber = KCHATSOCKET;
	IncomingMsg[Idx].immediateAddress[0] = 0xff;
	IncomingMsg[Idx].immediateAddress[1] = 0xff;
	IncomingMsg[Idx].immediateAddress[2] = 0xff;
	IncomingMsg[Idx].immediateAddress[3] = 0xff;
	IncomingMsg[Idx].immediateAddress[4] = 0xff;
	IncomingMsg[Idx].immediateAddress[5] = 0xff;
	IncomingMsg[Idx].fragmentCount = 2;
	IncomingMsg[Idx].fragmentDescriptor[0].address = &IncomingHeader[Idx];
	IncomingMsg[Idx].fragmentDescriptor[0].size = sizeof(IncomingHeader[Idx]);
	IncomingMsg[Idx].fragmentDescriptor[1].address = &Incomingkchat[Idx];
	IncomingMsg[Idx].fragmentDescriptor[1].size = sizeof(Incomingkchat[Idx]);
	IPXListenForPacket(&IncomingMsg[Idx]);
}

//At this point, all the ECB is filled, except for the dest addr.
int Tkchat::SendBufferedPacket(ECB *BuffPKT, char *User)
{
	int XportTime;
	int Break = 0;
	int i, j, z;
	//Find room in the buffer,
	for (j = 0; j < NUMSENDBUFFS; j++)
	{
		if (pMsgBuffer[j] == NULL)
			goto foundbuff;
	}
	//post message - no free buffers***************
	printf("\n\n\n\n\n\nCritical Buffer overflow thingy!\a\n");
	exit(99);
	return 0;

foundbuff:
	//Allocate the buffer

	pMsgBuffer[j] = new MsgBuffer;
	assert(pMsgBuffer[j]);
	memcpy((void *)&pMsgBuffer[j]->Ecb, BuffPKT, sizeof(ECB));
	memcpy((void *)&pMsgBuffer[j]->Ipxheader, BuffPKT->fragmentDescriptor[0].address, BuffPKT->fragmentDescriptor[0].size);
	memcpy((void *)&pMsgBuffer[j]->message, BuffPKT->fragmentDescriptor[1].address, BuffPKT->fragmentDescriptor[1].size);

	pMsgBuffer[j]->Ecb.fragmentDescriptor[0].address = &pMsgBuffer[j]->Ipxheader;
	pMsgBuffer[j]->Ecb.fragmentDescriptor[0].size = BuffPKT->fragmentDescriptor[0].size;
	pMsgBuffer[j]->Ecb.fragmentDescriptor[1].address = &pMsgBuffer[j]->message;
	pMsgBuffer[j]->Ecb.fragmentDescriptor[1].size = BuffPKT->fragmentDescriptor[1].size;

	for (i = 0; i < MAXUSERS; i++)
	{
		if (!User)
		{
			if (Users[i].Name[0] == NULL)
			{
				pMsgBuffer[j]->Received[i] = 99;
				goto skip;
			}
			TargetAddr = &Users[i].Address;
			IPXGetLocalTarget((BYTE far *)TargetAddr, (BYTE far *)pMsgBuffer[j]->Ecb.immediateAddress, &XportTime);
			for (z = 0; z < 6; z++)
				pMsgBuffer[j]->Ipxheader.destination.node[z] = TargetAddr->node[z];
			for (z = 0; z < 4; z++)
				pMsgBuffer[j]->Ipxheader.destination.network[z] = TargetAddr->network[z];
			pMsgBuffer[j]->Received[i] = (unsigned int)(clock() * .001);
			pMsgBuffer[j]->LastSent[i] = (unsigned int)(clock() * .001);
			IPXSendPacket(&pMsgBuffer[j]->Ecb); //loop for every user
		}
		else
		{
			if (stricmp(User, Users[i].Name) != 0)
			{
				pMsgBuffer[j]->Received[i] = 99;
				goto skip;
			}
			else
			{
				TargetAddr = &Users[i].Address;
				IPXGetLocalTarget((BYTE far *)TargetAddr, (BYTE far *)pMsgBuffer[j]->Ecb.immediateAddress, &XportTime);
				for (z = 0; z < 6; z++)
					pMsgBuffer[j]->Ipxheader.destination.node[z] = TargetAddr->node[z];
				for (z = 0; z < 4; z++)
					pMsgBuffer[j]->Ipxheader.destination.network[z] = TargetAddr->network[z];
				pMsgBuffer[j]->Received[i] = (unsigned int)(clock() * .001);
				pMsgBuffer[j]->LastSent[i] = (unsigned int)(clock() * .001);
				IPXSendPacket(&pMsgBuffer[j]->Ecb); //loop for every user
			}
		}
	skip:
	}
	Counter++;
	return 0;
}

void Tkchat::ResendBuffer(MsgBuffer *Buffer, char *Sendee)
{

	int i, z, XportTime;
	if (!Sendee[0])
		return;
	for (i = 0; i < MAXUSERS; i++)
	{
		if (stricmp(Users[i].Name, Sendee) == 0)
		{
			TargetAddr = &Users[i].Address;
			IPXGetLocalTarget((BYTE far *)TargetAddr, (BYTE far *)Buffer->Ecb.immediateAddress, &XportTime);
			for (z = 0; z < 6; z++)
				Buffer->Ipxheader.destination.node[z] = TargetAddr->node[z];
			for (z = 0; z < 4; z++)
				Buffer->Ipxheader.destination.network[z] = TargetAddr->network[z];
			Buffer->Received[i] = (unsigned int)(clock() * .001);
			IPXSendPacket(&Buffer->Ecb); //loop for every user
			return;
		}
	}
}

void Tkchat::BufferIdle(void)
{
	Message UserIsToast;
	int Done = 0;
	int NotIt = 0;
	unsigned int Now;
	int z = 0, i = 0, j, AllDone = 0;

	for (j = 0; j < NUMSENDBUFFS; j++)
	{
		AllDone = 1;
		//Buffer loop
		if (pMsgBuffer[j] != NULL)
		{
			Now = (unsigned int)(clock() * .001);
			//user loop
			for (i = 0; i < MAXUSERS; i++)
			{
				if (Users[i].Name[0] == NULL)
					break;
				if (pMsgBuffer[j]->Received[i] != 99)
				{
					AllDone = 0;
					if ((Now - pMsgBuffer[j]->Received[i]) >= 2) //time to send again
					{
						ResendBuffer(pMsgBuffer[j], Users[i].Name);
						AllDone = 0;
					}											  //end of resend buffer
					if ((Now - pMsgBuffer[j]->LastSent[i]) >= 45) //Disconnect user
					{
						Users[i].Alive--;
						pMsgBuffer[j]->Received[i] = 99;
						if (!Users[i].Alive)
						{
							strcpy(UserIsToast.Channel, "*");
							strcpy(UserIsToast.Nickname, Users[i].Name);
							sprintf(UserIsToast.Message, "%s has Disconnected!", Users[i].Name);
							UserIsToast.Type = KCHATPKTDISCON;
							PutMessageOnQueue(&UserIsToast);
							//put a message on the queue saying the user dropped.
							RemoveUser(Users[i].Name);
							//remove user;
						}
					} //end of user disconnected
				}
			} //end of user loop
			if (AllDone)
			{

				delete pMsgBuffer[j];
				pMsgBuffer[j] = NULL;

			} //end of delete buffer
		}	 //end of not a null buffer
	}		  //end of big loop of all the buffers
	return;
}
