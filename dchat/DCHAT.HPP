
#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>


#include <time.h>
#include "..\dchat\kchat.h"
#include "...\clib\tcrc.hpp"

#define BREAKPOINT asm int 3;


class Tkchat {
	MsgBuffer *pMsgBuffer[NUMSENDBUFFS+1];
	struct MessageQueue {
		Message Msg;
		struct MessageQueue *Next;
		}*FirstMsgQ,*CurrMsgQ,*TempMsgQ;
	struct KchatUser TmpUser;
	struct KchatUser *Users;
	Message Ack;
	KchatPKT *Incomingkchat;
	Message	TmpMsg;
	Message *SndBuffer;
	ECB *IncomingMsg;
	ECB *SendECBBuffer,*SendPacket;
	IPXHeader *IncomingHeader;
	IPXHeader SendHeader;
	IPXAddress far *TargetAddr;
	FILE *Incoming,*SIncoming;
	FileXfer FileReturnData,FileRcvdData,SFileRcvdData;
	SOfferFile *AvailableFiles;
	SOfferFile *SuperAvailFiles;
	SOfferFile *SuperOffer;
	OfferData *SuperPndXfers;
	OfferData *PendingXfers;

	TCRCMAN *crc;

	int UsersChanged;
	int iLoopSendECB;
	int ChatSocket;
	int SndPos;
	int iAway;
	int iNoPrivate;
	long BytesXferd;
	long Counter;
	char AwayMsg[250];
	char NoPrivateMsg[250];
	char ThisUser[16];
	char ThisChannel[16];
	char ReceivingFile[255];
	void EnterFileSendIdle(void);
	void FileRequest(KchatPKT *Request);
	int SendBufferedPacket(ECB *BuffPKT,char *User);
	void ResendBuffer(MsgBuffer *Buffer,char *Sendee);
	void SendAwayNotice(char *User);
	void ListenForMsg(void); //Allocates the ECBs, and Opens the Sockets
	int PutMessageOnQueue(Message *Msg);
	void StopListening(void);//Closes the Sockets
	void RefreshECB(int Idx);


	int SendGeneralPacket(KchatPKT *DataPacket);
	int SendBroadcastPacket(KchatPKT *DataPacket);
   void SendBasicMessage(char *Msg,int MsgSize,int Type,char *DestUser);
	void SendNoPrivateMsg(char *User);
	void BufferIdle(void);
	void SndAck(IPXAddress *TargetAddr,KchatPKT *Packet);
	void RcvAck(char *szAcker,char *szSequence);
	void ReceivePing(Message *Msg);
	void SndIam(IPXAddress *TargetAddr);
	void Iam(IPXAddress *WhoAddr);
	void AddUser(struct KchatUser *AddUser);
	void WelcomeUser(struct KchatUser *Welcomee);
	void RemoveUser(IPXAddress *DelAddress);
	void RemoveUser(char *DelUser);
	void Pong(IPXAddress *TargetAddr,KchatPKT *Packet);
	void XferFile(KchatPKT *Packet);
	void CancelXfer(KchatPKT *Request);
	void ProcessRequest(KchatPKT *Request);
	void BeginSXfer(KchatPKT *Request);
	void CompleteSXfer(KchatPKT *Request);
	void SaveFileChunk(KchatPKT *FileChunk);
	public:

	Tkchat();
	~Tkchat();
	int UsersChangedFlag(void);
	char * GetUserName(int Idx);
	void SetChannel(char *Channel);
	void AnnounceMe(char *User,char *Channel);  //Join a channel
	void EnterIdle(void);	//checks for incoming data
	Message * GetMessageFromQueue(void);   //returns a pointer to a message struct
	void SendMessage(char *Msg);
	int SendPrivateMessage(char *Msg,char *NickName); //return 0 on error
	void SendPostureMessage(char *Msg);  //post an action
	void Who(char *Channel);
	void GoodBye(char *Remark);
	void Away(char *Msg);	//Away
	void NoPrivate(char *Msg);
	FileXfer * GetOfferFileInfo(int Index);
	int GetFile(FileXfer *GetFileReq);	 //Return 1 if able to initiate request, 0 on error
	int DeOffer(int Index);
	int OfferFile(FileXfer *OfferFile); //Return  0 if unable to offer, 1 if successfull
	void Part(void);
	int Ping(char *User);
	void Whois(char *User);
	void Kali(void);
	int IgnoreUser(char *Name);
	FileXfer * GetRcvdBytes(void);
	OfferData * GetFileSendInfo(int Index);

	int OfferSFile(char *FName,char *User,char *Desc);
	void DeofferSFile(int Index);
	OfferData * GetSXferStats(int Index);
	SOfferFile * GetSOfferData(int Index);
	int GetSFile(char *User,int Index,char *FName);
	FileXfer * GetRcvdInfo(void);

};


#include "..\dchat\kchatusr.hpp"
#include "..\dchat\kchatfil.hpp"
#include "..\dchat\kchatmsg.hpp"
#include "..\dchat\kchatpkt.hpp"


Tkchat::Tkchat()
{
int i,j;

	crc=new TCRCMAN;
	assert(crc);

	SuperOffer=new SOfferFile[NUMSOFFERS+1];
	assert(SuperOffer);

	SuperPndXfers=new OfferData[NUMSXFERS+1];
	assert(SuperPndXfers);

	IncomingMsg=new ECB[NUMMSGECB+1];
	assert(IncomingMsg);

	SndBuffer=new Message[NUMMSGECB+1];
	assert(SndBuffer);

	Incomingkchat=new KchatPKT[NUMMSGECB+1];
	assert(IncomingMsg);

	SendECBBuffer=new ECB[NUMMSGECB+1];
	assert(SendECBBuffer);

	IncomingHeader=new IPXHeader[NUMMSGECB+1];
	assert(IncomingHeader);

	AvailableFiles=new SOfferFile[NUMFILEOFFERS+1];
	assert(AvailableFiles);

	SuperAvailFiles=new SOfferFile[NUMFILEOFFERS+1];
	assert(AvailableFiles);

	PendingXfers=new OfferData[NUMFILESENDS+1];
	assert(PendingXfers);

	Users=new struct KchatUser[MAXUSERS+1];
	assert(Users);
	//Init the linked list
	FirstMsgQ=new struct MessageQueue;
	assert(FirstMsgQ);

	CurrMsgQ=FirstMsgQ;

UsersChanged=0;
iLoopSendECB=0;
ChatSocket=KCHATSOCKET;
SndPos=0;
iAway=0;
iNoPrivate=0;
Counter=1;
BytesXferd=0;
ReceivingFile[0]=NULL;
FileRcvdData.User[0]=NULL;
SendPacket=&SendECBBuffer[iLoopSendECB];
for(i=0;i<NUMFILESENDS;i++)PendingXfers[i].Name[0]=NULL;
for(i=0;i<NUMFILEOFFERS;i++) AvailableFiles[i].fname[0]=NULL;
for(i=0;i<MAXUSERS;i++) {Users[i].Name[0]=NULL; Users[i].Ignore=0;Users[i].Alive=0;}
for(j=0;j<NUMSENDBUFFS;j++) pMsgBuffer[j]=NULL;
ListenForMsg();
CurrMsgQ->Msg.Message[0]=NULL;
}

Tkchat::~Tkchat()
{
//Clear the Linked List.
//StopListening();  //Not needed - close socket cancels all
IPXCloseSocket(KCHATSOCKET);
fcloseall();
//delete SuperOffer;
//delete SuperPndXfers;
//delete IncomingMsg;
//delete SndBuffer;
//delete Incomingkchat;
//delete SendECBBuffer;
//delete IncomingHeader;
//delete AvailableFiles;
//delete SuperAvailFiles;
//delete PendingXfers;
//delete Users;
}

void Tkchat::EnterIdle(void)
{
int i,z,IgnoreMsg=0;


//Check for messages, put them in the appropriate queue, and reset to ECB to
//Listen
for(i=0;i<NUMMSGECB;i++) {

if(IncomingMsg[i].inUseFlag==0)
	if(IncomingMsg[i].completionCode==0) {

		if(Incomingkchat[i].Type==KCHATPKTACK)
			{
			RcvAck(Incomingkchat[i].Nickname,Incomingkchat[i].Message);
			IgnoreMsg=1;
			goto skip;
			}
		else SndAck(&IncomingHeader[i].source,&Incomingkchat[i]);

		//for loop to check for message duplicates.
		for(z=0;z<MAXUSERS;z++)
			{
			if(stricmp(Incomingkchat[i].Nickname,Users[z].Name)==0)
			  if(stricmp(ThisUser,Incomingkchat[i].Nickname)!=0)
				{
				if(Users[z].Ignore) IgnoreMsg=1;
				if(Incomingkchat[i].Sequence==Users[z].LastSequence) 	IgnoreMsg=1;
				Users[z].LastSequence=Incomingkchat[i].Sequence;
				z=MAXUSERS;
				break;
				}
			}

skip:

if(!IgnoreMsg)
{
	if(Incomingkchat[i].Message[0]==NULL) {Incomingkchat[i].Message[0]=' ';Incomingkchat[i].Message[0]=NULL;}

		switch(Incomingkchat[i].Type)
			{
			case KCHATPKTMESSAGE:
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				break;
			case KCHATPKTHELLO:
				memcpy(&TmpUser.Address,&IncomingHeader[i].source,sizeof(IPXAddress));
				strcpy(TmpUser.Name,Incomingkchat[i].Nickname);
				strcpy(TmpUser.Channel,Incomingkchat[i].Channel);
				AddUser(&TmpUser);
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				WelcomeUser(&TmpUser);
				break;
			case KCHATPKTWELCOME:
				memcpy(&TmpUser.Address,&IncomingHeader[i].source,sizeof(IPXAddress));
				strcpy(TmpUser.Name,Incomingkchat[i].Nickname);
				strcpy(TmpUser.Channel,Incomingkchat[i].Channel);
				AddUser(&TmpUser);
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				break;
			case KCHATPKTBYE:
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				RemoveUser(&IncomingHeader[i].source);
				break;
			case KCHATPKTPART:
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				//put some message on the queue
				break;
			case KCHATPKTWHO:
				if((stricmp(ThisChannel,Incomingkchat[i].Channel)==0)||(stricmp("*",Incomingkchat[i].Channel)==0)) {
					SndIam(&IncomingHeader[i].source);
					PutMessageOnQueue((Message *)&Incomingkchat[i]);
					}
				break;
			case KCHATPKTIAM:
				memcpy(&TmpUser.Address,&IncomingHeader[i].source,sizeof(IPXAddress));
				strcpy(TmpUser.Name,Incomingkchat[i].Nickname);
				strcpy(TmpUser.Channel,Incomingkchat[i].Channel);
				AddUser(&TmpUser);
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				//response from who I send
				break;
			case KCHATPKTPOSTURE:
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				break;
			case KCHATPKTPRIVATE:
				if(iNoPrivate) SendNoPrivateMsg(Incomingkchat[i].Nickname);
				if(iAway) SendAwayNotice(Incomingkchat[i].Nickname);
				if(!iNoPrivate)PutMessageOnQueue((Message *)&Incomingkchat[i]);
				break;
			case KCHATPKTPING:
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				Pong(&IncomingHeader[i].source,&Incomingkchat[i]);
				//respond with pong
				break;
			case KCHATPKTPONG:
				//Change the message to equal the response time
				ReceivePing((Message *)&Incomingkchat[i]);
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				//response from ping I sent
				break;
			case KCHATPKTGET:
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				FileRequest(&Incomingkchat[i]);
				//request for file
				break;
			case KCHATPKTNOOFFER:
				fclose(Incoming);
            ReceivingFile[0]=NULL;
				FileRcvdData.User[0]=NULL;
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				//you can't have the stinkin file
				break;
			case KCHATPKTTRANSFER:
				XferFile(&Incomingkchat[i]);
				//actual file data
				break;
			case KCHATPKTCANCEL:
				PutMessageOnQueue((Message *)&Incomingkchat[i]);
				CancelXfer(&Incomingkchat[i]);
				//oops, I dont want the rest of the file.
				break;
			default:
				break;
			}//end of case
}//end of ignore msg
		RefreshECB(i);
		}//end of data is received

	//EnterFileSendIdle();
	}//end of ECB Loop;
BufferIdle();
//Detect disconnected users

}

void Tkchat::SetChannel(char *Channel)
{
strcpy(ThisChannel,Channel);
}
