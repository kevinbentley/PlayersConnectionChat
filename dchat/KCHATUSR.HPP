

void Tkchat::SndIam(IPXAddress *TargetAddr)
{
if(iLoopSendECB>=NUMMSGECB-2) iLoopSendECB++;
else iLoopSendECB=0;
SendPacket=&SendECBBuffer[iLoopSendECB];

//int Break;
int i;
int XportTime;
IPXHeader SendHeader;
Message Iam;

Iam.SomethingA=0;
Iam.SomethingB=0;
Iam.SomethingC=0;
Iam.Sequence=Counter;
Iam.Type=KCHATPKTIAM;

strcpy(Iam.Message,ThisChannel);
strcpy(Iam.Channel,ThisChannel);
for(i=0;i<strlen(Iam.Message);i++) if(Iam.Message[i]==0x20){Iam.Message[i]=NULL; break;}
for(i=0;i<strlen(Iam.Channel);i++) if(Iam.Channel[i]==0x20){Iam.Channel[i]=NULL; break;}
strcpy(Iam.Nickname,ThisUser);


IPXGetLocalTarget((BYTE far *)TargetAddr,(BYTE far *)SendPacket->immediateAddress,&XportTime);
SendPacket->ESRAddress=NULL;
SendPacket->socketNumber=KCHATSOCKET;
SendPacket->fragmentCount=2;
SendPacket->fragmentDescriptor[0].address=&SendHeader;
SendPacket->fragmentDescriptor[0].size=sizeof(SendHeader);
SendPacket->fragmentDescriptor[1].address=&Iam;
SendPacket->fragmentDescriptor[1].size=strlen(Iam.Message)+51;
SendHeader.packetType=0x04; //IPX packet type
SendHeader.length=sizeof(IPXHeader);
for(i=0;i<6;i++) SendHeader.destination.node[i]=TargetAddr->node[i];
for(i=0;i<4;i++) SendHeader.destination.network[i]=TargetAddr->network[i];
SendHeader.destination.socket[1]=KCHATSOCKETA;
SendHeader.destination.socket[0]=KCHATSOCKETB;
IPXSendPacket(SendPacket);
Counter++;
//Send Iam message!!!!!
return ;




}

void Tkchat::Who(char *Channel)
{
SndBuffer[SndPos].Type=KCHATPKTWHO;
SndBuffer[SndPos].Sequence=Counter;
strcpy(SndBuffer[SndPos].Message,Channel);
strcpy(SndBuffer[SndPos].Nickname,ThisUser);
strcpy(SndBuffer[SndPos].Channel,ThisChannel);
SendBroadcastPacket((KchatPKT *)&SndBuffer[SndPos]);
if(SndPos>=(NUMMSGECB-2)) SndPos=0;
else SndPos++;
return;
}



void Tkchat::AddUser(struct KchatUser *AddUser)
{
int j;
//check to make sure user isn't already here.
	for(j=0;j<MAXUSERS;j++)
		if(strcmp(Users[j].Name,AddUser->Name)==0) return;

//Add UserInfo to List
	for(j=0;j<MAXUSERS;j++)
		if(Users[j].Name[0]==NULL) {
			UsersChanged=1;
			memcpy(&Users[j].Address,&AddUser->Address,sizeof(IPXAddress));
			strcpy(Users[j].Name,AddUser->Name);
			strcpy(Users[j].Channel,AddUser->Channel);
			Users[j].Alive=ACKSTODIE;
			j=MAXUSERS;
			//put message on the queue announcing user*****************
			}//found a blank spot

}

void Tkchat::RemoveUser(char *DelUser)
{
	//User quit - Remove from list.
	int j;
	for(int i=0;i<MAXUSERS;i++)
	{
		if(stricmp(DelUser,Users[i].Name)==0) {
			for(j=0;j<NUMSENDBUFFS;j++) if(pMsgBuffer[j]!=NULL) pMsgBuffer[j]->Received[i]=99;
			for(j=0;j<NUMFILESENDS;j++) if(stricmp(DelUser,PendingXfers[j].Name)==0) PendingXfers[j].Name[0]=NULL;
			UsersChanged=1;
			Users[i].Name[0]=NULL;
			i=MAXUSERS;
			}
		}

}

void Tkchat::RemoveUser(IPXAddress *DelAddress)
{
	//User quit - Remove from list.

	int NotIt=0;
//	scan list for matching IPX Address, and nuke that user.
	for(int i=0;i<MAXUSERS;i++)
	{
		NotIt=0;
		for(int z=0;z<6;z++)
			if(DelAddress->node[z]!=Users[i].Address.node[z]) NotIt=1;
		if(!NotIt) {
			Users[i].Name[0]=NULL;
         UsersChanged=1;
			i=MAXUSERS;
			}
		}

}
void Tkchat::WelcomeUser(struct KchatUser *Welcomee)
{
if(iLoopSendECB>=NUMMSGECB-2) iLoopSendECB++;
else iLoopSendECB=0;
SendPacket=&SendECBBuffer[iLoopSendECB];

Message Msg;
IPXAddress far *TargetAddr=&Welcomee->Address;
Msg.Type=KCHATPKTPRIVATE;
int i;
int XportTime;
//ECB SendPacket;
//IPXHeader SendHeader;

IPXGetLocalTarget((BYTE far *)TargetAddr,(BYTE far *)SendPacket->immediateAddress,&XportTime);

SendPacket->ESRAddress=NULL;
SendPacket->socketNumber=KCHATSOCKET;
SendPacket->fragmentCount=2;
SendPacket->fragmentDescriptor[0].address=&SendHeader;
SendPacket->fragmentDescriptor[0].size=sizeof(SendHeader);
SendPacket->fragmentDescriptor[1].address=&Msg;
SendPacket->fragmentDescriptor[1].size=strlen(Msg.Message)+51;
SendHeader.packetType=0x04; //IPX packet type
SendHeader.length=sizeof(IPXHeader);
for(i=0;i<6;i++) SendHeader.destination.node[i]=TargetAddr->node[i];
for(i=0;i<4;i++) SendHeader.destination.network[i]=TargetAddr->network[i];
SendHeader.destination.socket[0]=KCHATSOCKETA;
SendHeader.destination.socket[2]=KCHATSOCKETB;
IPXSendPacket(SendPacket);         //loop for every user
Counter++;
//Send actual message!!!!!
return ;

}


int Tkchat::IgnoreUser(char *Name)
{
int z;
	for(z=0;z<MAXUSERS;z++)
		{
		if(stricmp(Name,Users[z].Name)==0)
			{
			if(Users[z].Ignore) Users[z].Ignore=0;
			else Users[z].Ignore=1;
			return Users[z].Ignore;
			}
		}
return -1;
}




void Tkchat::AnnounceMe(char *User,char *Channel)
{
strcpy(ThisUser,User);
strcpy(ThisChannel,Channel);
SndBuffer[SndPos].Type=KCHATPKTHELLO;
SndBuffer[SndPos].Sequence=Counter;
strcpy(SndBuffer[SndPos].Message,"");
strcpy(SndBuffer[SndPos].Nickname,ThisUser);
strcpy(SndBuffer[SndPos].Channel,ThisChannel);
SendBroadcastPacket((KchatPKT *)&SndBuffer[SndPos]);
if(SndPos>=(NUMMSGECB-1)) SndPos=0;
else SndPos++;
}


void Tkchat::Part(void)
{
SndBuffer[SndPos].Type=KCHATPKTPART;
SndBuffer[SndPos].Sequence=Counter;
strcpy(SndBuffer[SndPos].Message,"");
strcpy(SndBuffer[SndPos].Nickname,ThisUser);
strcpy(SndBuffer[SndPos].Channel,ThisChannel);
SendGeneralPacket((KchatPKT *)&SndBuffer[SndPos]);
if(SndPos>=(NUMMSGECB-1)) SndPos=0;
else SndPos++;
}

void Tkchat::GoodBye(char *Remark)
{
SndBuffer[SndPos].Type=KCHATPKTBYE;
SndBuffer[SndPos].Sequence=Counter;
strcpy(SndBuffer[SndPos].Message,Remark);
strcpy(SndBuffer[SndPos].Nickname,ThisUser);
strcpy(SndBuffer[SndPos].Channel,ThisChannel);
SendBroadcastPacket((KchatPKT *)&SndBuffer[SndPos]);
if(SndPos>=(NUMMSGECB-1)) SndPos=0;
else SndPos++;
}

char * Tkchat::GetUserName(int Idx)
{
return Users[Idx].Name;
}

int Tkchat::UsersChangedFlag(void)
{
if(UsersChanged==1) {
	UsersChanged=0;
	return 1;
	}
else return 0;
}
