#include <nwipxspx.h>

#ifdef KEVINHOME
#include "dstructs.hpp"
#include "dchat.hpp"
#else
#include "i:\dev\dpack\dstructs.hpp"
#include "i:\dev\dpack\dchat.hpp"
#endif

#define ALTTIMEOUT 10000 //10 seconds
#define NUMLISTENECB 10
#define NUMDGAMES 8

class Tdpack
{

	clock_t clkSentReq, clkLastReceived;
	int ThisSocket;
	int AltSocket;
	int iGameCount;
	int GameECB[NUMLISTENECB];		 //inuse code
	int CompletionStatus[NUMDGAMES]; //TRUE if game data is in this struct.
	ECB far Request;
	ECB far GameStsECB[NUMLISTENECB];
	netgame_info far NetGame[NUMLISTENECB];
	IPXHeader RequestHeader;
	IPXHeader NetGameHeader[NUMLISTENECB];
	sequence_packet GetData;
	ubyte far MyAddr[10];

	netgame_info far *Gameinfo[NUMDGAMES]; //Game data is stored here.
	void RequestGameStatus();
	int FlipEndian(int Source);
	//	void pascal far _EventServiceRoutine(ECB* ecb)
  public:
	Tdpack() { ThisSocket = 0; };
	~Tdpack(){};
	void EnterIdle(void); //Checks for filled ECB
	void Refresh(void);   //Zeros out the completion data & requsts status
	void Close(void);
	void Start(int Socket);				//Note: Socket is BIG-ENDIAN!
	netgame_info *GetGameInfo(int Idx); //Don't call this until completion status==1
	int GetCompletionStatus(int Idx);   //TRUE if game data is in this struct.
};

void Tdpack::Start(int Socket)
{
	int i;
	iGameCount = 0;

	for (i = 0; i < NUMLISTENECB; i++)
		GameECB[i] = 0;
	for (i = 0; i < NUMDGAMES; i++)
		CompletionStatus[i] = 0;

	//Close if this is is called more than once (+/- sockets...)
	if (ThisSocket)
		IPXCloseSocket(ThisSocket);
	if (AltSocket)
		IPXCloseSocket(AltSocket);

	ThisSocket = FlipEndian(Socket + 0x5100);
	AltSocket = FlipEndian(Socket + 0x6100);

#ifdef DNGM
	IPXInitialize();
#endif
	IPXOpenSocket((unsigned char *)&AltSocket, 0);
	IPXOpenSocket((unsigned char *)&ThisSocket, 0);

	IPXGetInternetworkAddress((BYTE far *)MyAddr);
	RequestGameStatus();
}

void Tdpack::Close()
{
	IPXCloseSocket(AltSocket);
	IPXCloseSocket(ThisSocket);
}

void Tdpack::Refresh(void)
{
	iGameCount = 0;
	for (int i = 0; i < NUMLISTENECB; i++)
		GameECB[i] = 0;
	RequestGameStatus();
}

void Tdpack::RequestGameStatus(void)
{
	//clkSentReq=clock();
	//Send request on alt socket
	//set up listen ecb's on alt socket
	//everytime packet is received - Set clkLastReceived to clock();

	//if no packets for ALTTIMEOUT, request server again

	//Inside Idle loop, check clock...if ALTTIMEOUT has passed
	//Call function to become server.
	//AND subsequently call server function in idle loop

	//Used for setting up the sockets....
	union UEndian {
		int TargetInt;
		struct SEndian
		{
			BYTE FirstByte;
			BYTE SecondByte;
		} SByte;
	} USocket;

	int i;
	sequence_packet GetInfo;

	for (i = 0; i < 4; i++)
		GetInfo.player.server[i] = MyAddr[i];
	for (i = 0; i < 6; i++)
		GetInfo.player.node[i] = MyAddr[i + 4];
	for (i = 0; i < NUMLISTENECB; i++)
	{
		GameStsECB[i].ESRAddress = NULL;
		GameStsECB[i].socketNumber = ThisSocket;
		GameStsECB[i].immediateAddress[0] = 0xff;
		GameStsECB[i].immediateAddress[1] = 0xff;
		GameStsECB[i].immediateAddress[2] = 0xff;
		GameStsECB[i].immediateAddress[3] = 0xff;
		GameStsECB[i].immediateAddress[4] = 0xff;
		GameStsECB[i].immediateAddress[5] = 0xff;
		GameStsECB[i].fragmentCount = 2;
		GameStsECB[i].fragmentDescriptor[0].address = &NetGameHeader[i];
		GameStsECB[i].fragmentDescriptor[0].size = sizeof(NetGameHeader[i]);
		GameStsECB[i].fragmentDescriptor[1].address = &NetGame[i];
		GameStsECB[i].fragmentDescriptor[1].size = sizeof(NetGame[i]);
	}

	GetInfo.player.socket = ThisSocket;
	GetInfo.player.callsign[0] = '$';
	GetInfo.type = 36;
	Request.ESRAddress = NULL;
	Request.socketNumber = ThisSocket; //Descent Default Socket
	Request.completionCode = 0;
	Request.fragmentCount = 2;
	Request.fragmentDescriptor[0].address = &RequestHeader;
	Request.fragmentDescriptor[0].size = sizeof(IPXHeader);
	Request.fragmentDescriptor[1].address = &GetInfo;
	Request.fragmentDescriptor[1].size = sizeof(sequence_packet);
	RequestHeader.packetType = 0x04; //IPX packet type
	RequestHeader.length = sizeof(IPXHeader);
	for (i = 0; i < 4; i++)
		RequestHeader.destination.network[i] = 0x00;
	for (i = 0; i < 6; i++)
		RequestHeader.destination.node[i] = 0xff; //Broadcast
	for (i = 0; i < 6; i++)
		Request.immediateAddress[i] = 0xff;
	USocket.TargetInt = ThisSocket;
	RequestHeader.destination.socket[0] = USocket.SByte.FirstByte;
	RequestHeader.destination.socket[1] = USocket.SByte.SecondByte;
	for (i = 0; i < NUMLISTENECB; i++)
		IPXListenForPacket(&GameStsECB[i]);
	IPXSendPacket(&Request);
}

void Tdpack::EnterIdle(void)
{
	int iloop1;
	if (iGameCount >= 4)
		return;
	for (iloop1 = 0; iloop1 < NUMLISTENECB; iloop1++)
	{
		if (!GameECB[iloop1])
			if (GameStsECB[iloop1].inUseFlag == 0)
				if (GameStsECB[iloop1].completionCode == 0)
					if (NetGame[iloop1].packet_type == '%')
					{
						Gameinfo[iGameCount] = &NetGame[iloop1];
						CompletionStatus[iGameCount] = 1;
						iGameCount++;
						GameECB[iloop1] = 1;
					} //end of last if statement (This ECB has game response data!)
					else
						IPXListenForPacket(&GameStsECB[iloop1]); //Reopen this ECB for data
																 //because the packet received
																 //was either a request, or not valid.
	}															 //end of for loop
}

netgame_info *Tdpack::GetGameInfo(int Idx)
{
	return Gameinfo[Idx];
}

int Tdpack::GetCompletionStatus(int Idx)
{
	return CompletionStatus[Idx];
}

int Tdpack::FlipEndian(int Source)
{
	union UEndian {
		int TargetInt;
		struct SEndian
		{
			BYTE FirstByte;
			BYTE SecondByte;
		} SEndian;
	} USource, UTarget;
	USource.TargetInt = Source;
	UTarget.SEndian.SecondByte = USource.SEndian.FirstByte;
	UTarget.SEndian.FirstByte = USource.SEndian.SecondByte;
	return UTarget.TargetInt;
}

/*
void pascal far Tdpack::_EventServiceRoutine(ECB* ecb)
{
EnterIdle();
}
*/
