FileXfer * Tkchat::GetRcvdBytes(void)
{
if(FileRcvdData.User[0]==NULL) return NULL;
FileRcvdData.Progress=BytesXferd;
return &FileRcvdData;
}

void Tkchat::XferFile(KchatPKT *Packet)
{
int Testme=0;
char Tempbuff[35];
Message XferMsg;
//put messages on the queue
FileData *IncomingXfer;
IncomingXfer=(FileData *)Packet->Message;
if(ReceivingFile[0]==0) return; //we didn't expcect this packet!
if(IncomingXfer->PacketSize==0) {
	//file size /500
	fclose(Incoming);
	ReceivingFile[0]=NULL;
	FileRcvdData.User[0]=NULL;
	BytesXferd=0;
	XferMsg.Type=KCHATPKTTRANSFER;
	strcpy(XferMsg.Nickname,"File");
	strcpy(XferMsg.Channel,ThisChannel);
	ltoa(IncomingXfer->Offset+IncomingXfer->PacketSize,Tempbuff,10);
	sprintf(XferMsg.Message,"File transfer from %s completed. %s bytes received.",Packet->Nickname,Tempbuff);
	PutMessageOnQueue(&XferMsg);
	return;
	}
if(IncomingXfer->PacketSize<500) {
	//end of file.
	fseek(Incoming,IncomingXfer->Offset,SEEK_SET);
	fwrite(IncomingXfer->Data,IncomingXfer->PacketSize,1,Incoming);
	fclose(Incoming);
	BytesXferd=0;
	ReceivingFile[0]=NULL;
	FileRcvdData.User[0]=NULL;
	XferMsg.Type=KCHATPKTTRANSFER;
	strcpy(XferMsg.Nickname,"File");
	strcpy(XferMsg.Channel,ThisChannel);
	ltoa(IncomingXfer->Offset+IncomingXfer->PacketSize,Tempbuff,10);
	sprintf(XferMsg.Message,"File transfer from %s completed. %s bytes received.",Packet->Nickname,Tempbuff);
	PutMessageOnQueue(&XferMsg);
	return;
	}
fseek(Incoming,IncomingXfer->Offset,SEEK_SET);
fwrite(IncomingXfer->Data,IncomingXfer->PacketSize,1,Incoming);
BytesXferd=IncomingXfer->Offset+IncomingXfer->PacketSize;
return;
}


int Tkchat::GetFile(FileXfer *GetFileReq)
{
//set up the ecb, and ipx header & kchatpkt
//send request packet to specific user, including the file number
//the file name, and the user to request from

//set the variable ReceivingFile,GetFileReq->File

char szIndex[9];

IPXAddress far *TargetAddr=NULL;
int NotIt=1,z;


if(ReceivingFile[0]!=NULL) return -3;
//check the user name
for(z=0;z<MAXUSERS;z++) {
	if(stricmp(GetFileReq->User,Users[z].Name)==0) NotIt=0;
	}
if(NotIt) return -1; //can't find user

//Open the file for creating
Incoming=fopen(GetFileReq->File,"wb");
if(!Incoming) return -2; //can't create file

if(iLoopSendECB>=NUMMSGECB-2) iLoopSendECB++;
else iLoopSendECB=0;
SendPacket=&SendECBBuffer[iLoopSendECB];
Message *sMsg=&SndBuffer[SndPos];

strcpy(FileRcvdData.File,GetFileReq->File);
strcpy(FileRcvdData.User,GetFileReq->User);
FileRcvdData.Index=GetFileReq->Index;

strcpy(ReceivingFile,GetFileReq->File);
sprintf(szIndex,"%d",GetFileReq->Index);

SndBuffer[SndPos].Type=KCHATPKTGET;
SndBuffer[SndPos].Sequence=Counter;

strcpy(SndBuffer[SndPos].Message,szIndex);
strcpy(SndBuffer[SndPos].Nickname,ThisUser);
strcpy(SndBuffer[SndPos].Channel,ThisChannel);

SendPacket->ESRAddress=NULL;
SendPacket->socketNumber=KCHATSOCKET;
SendPacket->fragmentCount=2;
SendPacket->fragmentDescriptor[0].address=&SendHeader;
SendPacket->fragmentDescriptor[0].size=sizeof(SendHeader);
SendPacket->fragmentDescriptor[1].address=sMsg;
SendPacket->fragmentDescriptor[1].size=strlen(sMsg->Message)+51;
SendHeader.packetType=0x04; //IPX packet type
SendHeader.length=sizeof(IPXHeader);
SendHeader.destination.socket[1]=KCHATSOCKETA;
SendHeader.destination.socket[0]=KCHATSOCKETB;
SendBufferedPacket(SendPacket,GetFileReq->User);
//Send actual message!!!!!
return 0;
}

int Tkchat::OfferFile(FileXfer *OfferFile)
{
int i;
//do a for loop looking for free index number
for(i=0;i<NUMFILEOFFERS;i++){
	if(AvailableFiles[i].fname[0]==NULL){
		AvailableFiles[i].Fhandle=fopen(OfferFile->File,"rb");
		if(AvailableFiles[i].Fhandle) {
			//open the file for reading
			strcpy(AvailableFiles[i].Description,OfferFile->Description);
			strcpy(AvailableFiles[i].fname,OfferFile->File);
			return i;
			//return index
			}//end of able to open file
		else return -2;
		}//end of found empty spot
		//if unable to open, return -1
	}//end of this slot available
//if none available, return -1
return -1;
}

int Tkchat::DeOffer(int Index)
{
int i;
if(Index==-1) for(i=0;i<NUMFILEOFFERS;i++) {
	AvailableFiles[i].fname[0]=NULL;
	fclose(AvailableFiles[i].Fhandle);
	}
else {
	if(AvailableFiles[Index].fname[0]==NULL)return -1;
	AvailableFiles[Index].fname[0]=NULL;
	fclose(AvailableFiles[Index].Fhandle);
	}
return 0;
}

FileXfer * Tkchat::GetOfferFileInfo(int Index)
{
strcpy(FileReturnData.Description,AvailableFiles[Index].Description);
strcpy(FileReturnData.File,AvailableFiles[Index].fname);
return  &FileReturnData;
}

OfferData * Tkchat::GetFileSendInfo(int Index)
{
return &PendingXfers[Index];
}

void Tkchat::CancelXfer(KchatPKT *Request)
{
int i;
for(i=0;i<NUMFILESENDS;i++)
	if(strcpy(Request->Nickname,PendingXfers[i].Name)==0)
		PendingXfers[i].Name[0]=NULL;
}

void Tkchat::FileRequest(KchatPKT *Request)
{
int Index,FoundRoom=0,i=0,z;
Index=strtod(Request->Message,(char **) &Request->Message[strlen(Request->Message)]);

FileData *XferData;
KchatPKT XferPkt;
XferData=(FileData *)&XferPkt.Message;
Message XferMsg;
if(Index>=NUMFILESENDS) goto nooffer;
//do a for loop, looking for an open file request
if(AvailableFiles[Index].fname[0]!=NULL){
	for(i=0;i<NUMFILESENDS;i++) {
		if(PendingXfers[i].Name[0]==NULL) {
			//Set up the request struct
			strcpy(PendingXfers[i].Description,AvailableFiles[Index].Description);
			PendingXfers[i].Fhandle=AvailableFiles[Index].Fhandle;
			strcpy(PendingXfers[i].Name,Request->Nickname);
			PendingXfers[i].Index=Index;
			//set both ack values to 0
			PendingXfers[i].NextAckToRcv=0;
			PendingXfers[i].LastAckRcvd=0;
			//set offset to 0
			PendingXfers[i].Offset=0;
			FoundRoom=1;

			fseek(PendingXfers[i].Fhandle,PendingXfers[i].Offset,0);
			XferData->Offset=PendingXfers[i].Offset;
			for(z=0;z!=500;z++) {
			//if they are the same, read the next 500 bytes
				fread(&XferData->Data[z],1,1,PendingXfers[i].Fhandle);
				if(feof(PendingXfers[i].Fhandle)) break;
				}
			XferData->PacketSize=z;
			//increment all the ack variables & stuff
			PendingXfers[i].NextAckToRcv=Counter;
			//set the offset to equal size plus prev. offset.
			PendingXfers[i].Offset=PendingXfers[i].Offset+z;

			if(iLoopSendECB>=NUMMSGECB-2) iLoopSendECB++;
			else iLoopSendECB=0;
			SendPacket=&SendECBBuffer[iLoopSendECB];
			Message *sMsg=&SndBuffer[SndPos];
			XferPkt.Type=KCHATPKTTRANSFER;
			XferPkt.Sequence=Counter;

			strcpy(XferPkt.Nickname,ThisUser);
			strcpy(XferPkt.Channel,ThisChannel);

			SendPacket->ESRAddress=NULL;
			SendPacket->socketNumber=KCHATSOCKET;
			SendPacket->fragmentCount=2;
			SendPacket->fragmentDescriptor[0].address=&SendHeader;
			SendPacket->fragmentDescriptor[0].size=sizeof(SendHeader);
			SendPacket->fragmentDescriptor[1].address=&XferPkt;
			//set the length based on fread's return
			SendPacket->fragmentDescriptor[1].size=z+56;
			SendHeader.packetType=0x04; //IPX packet type
			SendHeader.length=sizeof(IPXHeader);
			SendHeader.destination.socket[1]=KCHATSOCKETA;
			SendHeader.destination.socket[0]=KCHATSOCKETB;
			SendBufferedPacket(SendPacket,PendingXfers[i].Name);
			//if the end of file is reached, set the name[0] to NULL.
			if(z<500) {
				XferMsg.Type=KCHATPKTTRANSFER;
				strcpy(XferMsg.Nickname,"File");
				strcpy(XferMsg.Channel,ThisChannel);
				sprintf(XferMsg.Message,"File transfer to %s completed.",PendingXfers[i].Name);
				PutMessageOnQueue(&XferMsg);
				PendingXfers[i].Name[0]=NULL;
				}
			//send a packet that has the next chunck of
			//data that the user needs.



			break;
			}//end of found spot
		}//end of available offer structs.
if(!FoundRoom) goto nooffer; //no send spots were found
	}//end of valid request
//if index is not valid, send noofer packet
else {
nooffer:
	//send nooffer packet
	if(iLoopSendECB>=NUMMSGECB-2) iLoopSendECB++;
	else iLoopSendECB=0;
	SendPacket=&SendECBBuffer[iLoopSendECB];
	Message *sMsg=&SndBuffer[SndPos];
	SndBuffer[SndPos].Type=KCHATPKTNOOFFER;
	SndBuffer[SndPos].Sequence=Counter;
	strcpy(SndBuffer[SndPos].Message,Request->Message);
	strcpy(SndBuffer[SndPos].Nickname,ThisUser);
	strcpy(SndBuffer[SndPos].Channel,ThisChannel);
	SendPacket->ESRAddress=NULL;
	SendPacket->socketNumber=KCHATSOCKET;
	SendPacket->fragmentCount=2;
	SendPacket->fragmentDescriptor[0].address=&SendHeader;
	SendPacket->fragmentDescriptor[0].size=sizeof(SendHeader);
	SendPacket->fragmentDescriptor[1].address=sMsg;
	SendPacket->fragmentDescriptor[1].size=strlen(sMsg->Message)+51;
	SendHeader.packetType=0x04; //IPX packet type
	SendHeader.length=sizeof(IPXHeader);
	SendHeader.destination.socket[1]=KCHATSOCKETA;
	SendHeader.destination.socket[0]=KCHATSOCKETB;
	SendBufferedPacket(SendPacket,Request->Nickname);
	//Send actual message!!!!! ^^^
	return ;
	}//end of invalid request!
}

void Tkchat::EnterFileSendIdle(void)
{
FileData *XferData;
KchatPKT XferPkt;
XferData=(FileData *)&XferPkt.Message;
Message XferMsg;
int i,z;
//scan through all file send structs (and loop)
for(i=0;i<NUMFILESENDS;i++){
	if(PendingXfers[i].Name[0]!=NULL) {
		//if name[0]!=null, compare both ack's
		if(PendingXfers[i].NextAckToRcv==PendingXfers[i].LastAckRcvd) {
			fseek(PendingXfers[i].Fhandle,PendingXfers[i].Offset,0);
			XferData->Offset=PendingXfers[i].Offset;
			for(z=0;z!=500;z++) {
			//if they are the same, read the next 500 bytes
				fread(&XferData->Data[z],1,1,PendingXfers[i].Fhandle);
				if(feof(PendingXfers[i].Fhandle)) break;
				}
			XferData->PacketSize=z;
			//increment all the ack variables & stuff
			PendingXfers[i].NextAckToRcv=Counter;
			//set the offset to equal size plus prev. offset.
			PendingXfers[i].Offset=PendingXfers[i].Offset+z;

			if(iLoopSendECB>=NUMMSGECB-2) iLoopSendECB++;
			else iLoopSendECB=0;
			SendPacket=&SendECBBuffer[iLoopSendECB];
			Message *sMsg=&SndBuffer[SndPos];
			XferPkt.Type=KCHATPKTTRANSFER;
			XferPkt.Sequence=Counter;

			strcpy(XferPkt.Nickname,ThisUser);
			strcpy(XferPkt.Channel,ThisChannel);

			SendPacket->ESRAddress=NULL;
			SendPacket->socketNumber=KCHATSOCKET;
			SendPacket->fragmentCount=2;
			SendPacket->fragmentDescriptor[0].address=&SendHeader;
			SendPacket->fragmentDescriptor[0].size=sizeof(SendHeader);
			SendPacket->fragmentDescriptor[1].address=&XferPkt;
			//set the length based on fread's return
			SendPacket->fragmentDescriptor[1].size=z+56;
			SendHeader.packetType=0x04; //IPX packet type
			SendHeader.length=sizeof(IPXHeader);
			SendHeader.destination.socket[1]=KCHATSOCKETA;
			SendHeader.destination.socket[0]=KCHATSOCKETB;
			SendBufferedPacket(SendPacket,PendingXfers[i].Name);
			//if the end of file is reached, set the name[0] to NULL.
			if(z<500) {
				XferMsg.Type=KCHATPKTTRANSFER;
				strcpy(XferMsg.Nickname,"File");
				strcpy(XferMsg.Channel,ThisChannel);
				sprintf(XferMsg.Message,"File transfer to %s completed.",PendingXfers[i].Name);
				PutMessageOnQueue(&XferMsg);
				PendingXfers[i].Name[0]=NULL;
				}
			//send a packet that has the next chunck of
			//data that the user needs.
			//send the packet.
			//here I need to set up the ecb
			//the ipx header
			//the message (extra large)
			//and the chunk of data from the file.
			}//end of time to send another packet
		}//end of valid user transfer
	}//end of file transfer structs
//end of loop
}
//******************************************************
//Super file Xfer's!												//
//******************************************************

#ifdef SXFER


#include "i:\dev\dpack\suprxfer.hpp"




void RequestFileChunk(int GetNum);
/*
do I really need this???

SuperPacketRequest is the struct type to fill out and send.

GetNum is the index of a pending request (i.e. multiple files)
ask for index, offset, and size of a file
*/

void Tkchat::SaveFileChunk(KchatPKT *FileChunk)
{
unsigned long ulCRC;
SuperXferPacket *RxData;
//SuperPacketRequest ReReq;

RxData=(SuperXferPacket *)&FileChunk->Message;
ulCRC=crc->GetCRC(0l,&RxData->Sig,RxData->packet_size+(sizeof(SuperXferPacket)-SPACKETSIZE));
if(ulCRC!=RxData->CRC) {
	ReReq.Size=SPACKETSIZE;
	ReReq.Offset=SFileRcvdData.Progress;
	ReReq.Index=SFileRcvdData.Index;
	SendBasicMessage(ReReq,SPACKETSIZE,KCHATPKTSGET,SFileRcvdData.User);
	return;
	//re-request!!!
	}//Invalid CRC received!!!!!
fwrite(RxData->Data,1,RxData->packet_size,SIncoming);
SFileRcvdData.Progress=SFileRcvdData.Progress+RxData->packet_size;

/*
Compare calculated CRC against CRC stored in the packet, if it is valid,
write to disk.
If not, re-request packet.
*/
}

void SFileIdle(void);
/*
If a Sfile transfer is in progress, check for timeout
and rerequest if needed.
Also, drop off pending requests that have timed out.
Keep an array to track what part of the file is missing?
Or, make sure we have the last X packets before requesting more...
*/



/*
-adding the time it took to receive the file would be good

typedef struct FileXfer {
char	File[128];
char	Description[300];
char	User[16];
int	Index;
long Progress;
};

typedef struct OfferData {
char Name[16];
char	Description[300];
int Index;
FILE * Fhandle;
long NextAckToRcv;
long LastAckRcvd;
long Offset;
};

typedef struct SOfferFile {
char fname[250];
char User[16];
char	Description[300];
FILE *Fhandle;
};

typedef struct FileData {
int PacketSize;
long Offset;
char Data[500];
};


typedef struct SuperXferPacket {
unsigned long CRC;
char Sig; //Added support for multiple file requests later.
unsigned long offset;
int packet_size;
char data[SPACKETSIZE];
};

typedef struct SuperPacketRequest {
char Index;
char Sig;
unsigned long Offset;
int Size;
};


*/

#endif
