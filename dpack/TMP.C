//At this point, all the ECB is filled, except for the dest addr.
void SendBufferedPacket(ECB *BuffPKT)
{
int XportTime;
int Break=0;
int i,j,z;

//Find room in the buffer,
for(j=0;j<NUMSENDBUFFS;j++)
{
	if(pMsgBuffer[j]==NULL) goto skip;
}
//post message - no free buffers
return;

skip:
//Allocate the buffer
pMsgBuffer[j]=new MsgBuffer;
memcpy(pMsgBuffer[j]->Ecb,BuffPKT,sizeof(ECB));
memcpy(pMsgBuffer[j]->Ipxheader,BuffPKT->fragmentDescriptor[0].address,BuffPKT->fragmentDescriptor[0].size);
memcpy(pMsgBuffer[j]->message,BuffPKT->fragmentDescriptor[1].address,BuffPKT->fragmentDescriptor[1].size);
pMsgBuffer[j]->Ecb.fragmentDescriptor[0].address=&pMsgBuffer[j]->Ipxheader;
pMsgBuffer[j]->Ecb.fragmentDescriptor[0].size=BuffPKT->fragmentDescriptor[0].size;
pMsgBuffer[j]->Ecb.fragmentDescriptor[1].address=&pMsgBuffer[j]->message;
pMsgBuffer[j]->Ecb.fragmentDescriptor[1].size=BuffPKT->fragmentDescriptor[1].size;

for(i=0;i<MAXUSERS;i++) {
	if(Users[i].Name[0]==NULL) {pMsgBuffer[j]->Received[i]=99; goto skip;}
	TargetAddr=&Users[i].Address;
	IPXGetLocalTarget((BYTE far *)TargetAddr,(BYTE far *)SendPacket->immediateAddress,&XportTime);
	for(z=0;z<6;z++) SendHeader.destination.node[z]=TargetAddr->node[z];
	for(z=0;z<4;z++) SendHeader.destination.network[z]=TargetAddr->network[z];
	pMsgBuffer[j]->Received[i]=0;
	pMsgBuffer[j]->LastSent[i]=(clock()/CLK_TCK);
	//	Users[i].Alive--;
	IPXSendPacket(SendPacket);         //loop for every user
skip:
	}
Counter++;
return ;
}


void Tkchat::RcvAck(char *szAcker,char *szSequence)
{
	Message UserIsToast;
	int Done=0;
	int NotIt=0;
	int z=0,i=0,j;
	long lSequence;


	lSequence=(unsigned long)strtol(szSequence,(char **) &szSequence[strlen(szSequence)],10);
//scan for This sequence #
	for(j=0;j<NUMSENDBUFFS;j++)
		{
			if(pMsgBuffer[j]!=NULL)
				if(lSequence==pMsgBuffer[j]->message.Sequence) goto foundbuff;
		}
//unwanted ack.......
return;
foundbuff:
	//now find the acker
	for(i=0;i<MAXUSERS;i++)
		{
		if(strcmp(szAcker,Users[i].Name)==0) {pMsgBuffer[j].Received[i]=99; return;}
		}
//couldn't find the user who acked us.
return;
}


void ResendBuffer(MsgBuffer *Buffer,char *Sendee)
{
	for(i=0;i<MAXUSERS;i++) {
		if(strcmp(Users[i].Name[0],Sendee==0))
		{
		TargetAddr=&Users[i].Address;
		IPXGetLocalTarget((BYTE far *)TargetAddr,(BYTE far *)SendPacket->immediateAddress,&XportTime);
		for(z=0;z<6;z++) SendHeader.destination.node[z]=TargetAddr->node[z];
		for(z=0;z<4;z++) SendHeader.destination.network[z]=TargetAddr->network[z];
		Buffer->Received[i]++;
		Buffer->LastSent[i]=(clock()/CLK_TCK);
		//	Users[i].Alive--;
		IPXSendPacket(&Buffer->Ecb);         //loop for every user
		return;
		}
}



void BufferIdle(void)
{
	Message UserIsToast;
	int Done=0;
	int NotIt=0;
	int z=0,i=0,j,Now,AllDone=0;

	for(j=0;j<NUMSENDBUFFS;j++)
	{
		AllDone=1;
		//Buffer loop
		if(pMsgBuffer[j]!=NULL)
		{
			//user loop
			for(i=0;i<MAXUSERS;i++)
			{
				Now=(clock()/CLK_TCK);
				if((pMsgBuffer[j].Received[i]<=15)&&((Now-pMsgBuffer[j].LastSent[i])>=2))
				if(pMsgBuffer[j].Received[i]==15)
				{
					pMsgBuffer[j].Received[i]=99;
					strcpy(UserIsToast.Channel,"*");
					strcpy(UserIsToast.Nickname,Users[i].Name);
					sprintf(UserIsToast.Message,"%s has Disconnected!",Users[i].Name);
					UserIsToast.Type=KCHATPKTDISCON;
					PutMessageOnQueue(&UserIsToast);
					//put a message on the queue saying the user dropped.
					RemoveUser(Users[i].Name);
					//remove user;
					}
				else
				{
					ResendBuffer(pMsgBuffer[j],Users[i].Name);
					AllDone=0;
					}
				}
			}
		if(AllDone)
		{
			delete pMsgBuffer[j];
			pMsgBuffer[j]=NULL;
			}
		}

return;
}




















