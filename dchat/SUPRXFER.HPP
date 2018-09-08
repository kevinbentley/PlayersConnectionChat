#include <fcntl.h>
#include <io.h>

void Tkchat::ProcessRequest(KchatPKT *Request)
{
	int i, ValidReq = 0;
	SuperPacketRequest *ReqData;
	SuperXferPacket Response;
	Message SuperSend;

	ReqData = (SuperPacketRequest *)&Request->Message;

	//compare SuperPndXfers[] structures before sending data
	for (i = 0; i < NUMSOFFERS; i++)
	{
		if (stricmp(SuperOffer[i].User, Request->Nickname) == 0)
			if (ReqData->Index == SuperPndXfers[i].Index)
			{
				ValidReq = 1; //flags that this is a valid request
				SuperPndXfers[i].Offset = SuperPndXfers[i].Offset + ReqData->Size;
			}
	}

	if (!ValidReq)
	{
		SendBasicMessage(&ReqData->Sig, 1, KCHATPKTSINVALID, Request->Nickname);
		return;
	}

	//check for invalid index
	if (SuperOffer[ReqData->Index].fname[0] == NULL)
	{
		SendBasicMessage(&ReqData->Sig, 1, KCHATPKTSINVALID, Request->Nickname);
		return;
	}
	if (SuperOffer[ReqData->Index].User[0] != NULL)
	{
		if (stricmp(SuperOffer[ReqData->Index].User, Request->Nickname) == 0)
		{
			//go ahead and send the file
			goto proceed;
		}
		//else send invalid
		SendBasicMessage(&ReqData->Sig, 1, KCHATPKTSINVALID, Request->Nickname);
		return;
	}
proceed:
	//Take a request for a file index,offset, and size,
	//and return a KCHATPKTSINVALID if the offset or size is
	//out of range, or the file is not offered.
	fseek(SuperOffer[ReqData->Index].Fhandle, ReqData->Offset, 0);
	Response.packet_size = fread(&Response.data, 1, SPACKETSIZE, SuperOffer[ReqData->Index].Fhandle);
	Response.offset = ReqData->Offset;
	Response.CRC = crc->GetCRC(0l, &Response.Sig, Response.packet_size + (sizeof(SuperXferPacket) - SPACKETSIZE));
	Response.Sig = ReqData->Sig;
	SendBasicMessage((char *)&Response, Response.packet_size, KCHATPKTSTRANSFER, Request->Nickname);
	/*
Also check the file offered structure for names allowed.
If valid, respond with a KCHATPKTSTRANSFER packet with the
CRC if it is a valid request.
*/
}

int Tkchat::OfferSFile(char *FName, char *User, char *Desc)
{
	//User is the user this is intended for, or NULL string
	//if available to everyone.
	int i, FoundRoom = 0;
	int Handle;

	for (i = 0; i < NUMSOFFERS; i++)
	{
		if (SuperOffer[i].fname[0] == NULL)
			FoundRoom = 1;
		break;
	}
	if (!FoundRoom)
		return -1;
	SuperOffer[i].Fhandle = fopen(FName, "wb");

	if (SuperOffer[i].Fhandle)
	{
		strcpy(SuperOffer[i].fname, FName);
		strcpy(SuperOffer[i].User, User);
		strcpy(SuperOffer[i].Description, Desc);
		open(FName, Handle, O_RDONLY);
		SuperOffer[i].Length = filelength(Handle);
		close(Handle);
		return i;
	}
	else
		return -2;
	/*
Set up offer file struct, and return an index num, or a -1
if none available, or -2 if unable to open
*/
}

void Tkchat::DeofferSFile(int Index)
{
	int i;
	if (Index == -1)
	{
		for (i = 0; i < i < NUMSOFFERS; i++)
		{
			SuperOffer[i].fname[0] = NULL;
			fclose(SuperOffer[i].Fhandle);
		}
		return;
	}
	SuperOffer[Index].fname[0] = NULL;
	fclose(SuperOffer[i].Fhandle);
	return;
}

void Tkchat::BeginSXfer(KchatPKT *Request)
{
	//**Process KCHATPKTSSTART message
	//****Note: the server resopnds with only the file size,
	//		so the requester needs to only have one pending start
	//		at any given time.

	/*
Check User name, and set up structure for this user for
reporting xfer status.
If Invalid, send KCHATPKTSINVALID message
*/
	int i;
	int Index;
	char buf[30];

	Index = strtod(Request->Message, NULL);
	if (SuperOffer[Index].fname[0] != NULL)
	{
		if ((stricmp(SuperOffer[Index].User, Request->Nickname) == 0) || (SuperOffer[i].User[0] == NULL))
		{
			for (i = 0; i < NUMSXFERS; i++)
			{
				if (SuperPndXfers[i].Name[0] == NULL)
				{
					strcpy(SuperPndXfers[i].Name, Request->Nickname);
					strcpy(SuperPndXfers[i].Description, SuperOffer[Index].Description);
					SuperPndXfers[i].Fhandle = SuperOffer[Index].Fhandle;
					SuperPndXfers[i].Offset = 0l;
					SuperPndXfers[i].Index = 0;
					SuperPndXfers[i].TotalSize = SuperOffer[Index].Length;
					strcpy(buf, ltoa(SuperPndXfers[i].TotalSize, buf, 10));
					SendBasicMessage(buf, strlen(buf), KCHATPKTSINFO, Request->Nickname);
					return;
				} //end of found user slot
				else
				{
					SendBasicMessage((char *)&Request->Message, strlen(Request->Message),
									 KCHATPKTSNOOFFER, Request->Nickname);
					return;
				} //end of didn't find user slot
			}	 //end of for loop of user slots
		}		  //end of valid user request
		else
		{
			SendBasicMessage((char *)&Request->Message, strlen(Request->Message),
							 KCHATPKTSNOOFFER, Request->Nickname);
			return;
		} //end of invalid user request
	}	 //end of valid file index
	else
	{
		SendBasicMessage((char *)&Request->Message, strlen(Request->Message),
						 KCHATPKTSNOOFFER, Request->Nickname);
		return;
	} //end of invalid file index

	//no room to start another xfer
	SendBasicMessage((char *)&Request->Message, strlen(Request->Message),
					 KCHATPKTSNOOFFER, Request->Nickname);
	return;
}

void Tkchat::CompleteSXfer(KchatPKT *Request)
{
	//Message has the index # to cancel.
	int i, Index = 0;

	Index = strtod(Request->Message, NULL);
	for (i = 0; i < NUMSXFERS; i++)
	{
		if (stricmp(Request->Nickname, SuperPndXfers[i].Name) == 0)
		{
			if (Index == SuperPndXfers[i].Index)
				SuperPndXfers[i].Name[0] = NULL;
			return;
		}
	}
}

int Tkchat::GetSFile(char *User, int Index, char *FName)
{
	char buf[30];

	SIncoming = fopen(FName, "wb");
	if (!SIncoming)
		return -1;
	strcpy(SFileRcvdData.File, FName);
	strcpy(SFileRcvdData.User, User);
	SFileRcvdData.Progress = 0l;
	SFileRcvdData.Index = Index;
	sprintf(buf, "%d", Index);
	SendBasicMessage(buf, strlen(buf), KCHATPKTSSTART, User);
	/*
Send SSTART command, and set up structure.
Then start sending RequestFileChunk() commands
*/
}

OfferData *Tkchat::GetSXferStats(int Index)
{
	return &SuperOffer[Index];
}

SOfferFile *Tkchat::GetSOfferData(int Index)
{
	return SuperPndXfers[Index];
}

FileXfer *Tkchat::GetRcvdInfo(void)
{
	return &SFileRcvdData;
}
