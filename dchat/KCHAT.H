

#define SPACKETSIZE 500
#define NUMSOFFERS 10
#define NUMSXFERS 10

#define NUMFILESENDS 10
#define NUMFILEOFFERS 10

#define NUMSENDBUFFS 100
#define ACKSTODIE 10
#define MAXUSERS 200
#define NUMMSGECB 50
#define KCHATSOCKET 0x8e27
#define KCHATSOCKETA 0x8e
#define KCHATSOCKETB 0x27
//Type Values for Kchat packets
#define KCHATPKTMESSAGE          0   // normal message
#define KCHATPKTHELLO            1   // joining a channel
#define KCHATPKTWELCOME          2   // response to HELLO
#define KCHATPKTBYE              3   // leaving kchat
#define KCHATPKTPART             4   // leaving channel
#define KCHATPKTNICKCHANGE       5   // not used anymore
#define KCHATPKTWHO              6   // request for who's here
#define KCHATPKTIAM              7   // response to WHO
#define KCHATPKTPOSTURE          8   // action (/me) message
#define KCHATPKTPRIVATE          9   // private message
#define KCHATPKTPING             10  // ping (includes time)
#define KCHATPKTPONG             11  // response to ping
#define KCHATPKTACK              12  // response to all packets
#define KCHATPKTOFFER            23  // offering a file (not used anymore)
#define KCHATPKTGET              24  // get a file
#define KCHATPKTNOOFFER          25  // response to get if no file offered
#define KCHATPKTTRANSFER         26  // packet containing xfer data.
#define KCHATPKTCANCEL        	27  // transfer aborted
//Our implementation
#define KCHATPKTSNOOFFER			115 // File isn't offered
#define KCHATPKTSINVALID			116 // Response to an invalid request
#define KCHATPKTSSTART				117 // Our 'Super' use to tell host
												 // we are getting file
#define KCHATPKTSEND					118 // Our 'Super' done getting
#define KCHATPKTSGET					119 // Our 'Super' Get command
#define KCHATPKTSTRANSFER			120 // Our 'Super' transfer pkt
#define KCHATPKTSCANCEL 			121 // Our 'Super' cancel pkt
#define KCHATPKTSINFO				122 // returns the file size in the message

#define KCHATPKTDISCON				99  // User didn't ack
#define KCHATPKTBUFFOVERFLOW		199 // Too many pending sends...



typedef struct KchatPKT {
	int Type;
	char Channel[16];
	char Nickname[16];
	long	Sequence;
	long	SomethingA;
	long	SomethingB;
	long	SomethingC;
	char Message[552];  //Null Terminated
}S_1;

struct KchatUser {
IPXAddress	Address;
char ImmediateAddress[6];
char	Channel[16];
char	Name[16];
struct KchatUser * NextUser;
int LastSequence;
int	Alive;
int Ignore;
}S_2;

typedef struct Message {
	int Type;
	char Channel[16];
	char Nickname[16];
	long	Sequence;
	long	SomethingA;
	long	SomethingB;
	long	SomethingC;
	char Message[552];
}S_3;

typedef struct FileXfer {
char	File[128];
char	Description[300];
char	User[16];
int	Index;
long Progress;
}S_4;

typedef struct OfferData {
char Name[16];
char	Description[300];
int Index;
FILE * Fhandle;
long NextAckToRcv;
long LastAckRcvd;
long Offset; //for Super xfers, the amount of bytes sent
long TotalSize; //this has the total lentgh of the file
}S_5;

typedef struct SOfferFile {
char User[16]; //who can get this file.
char fname[250];
char	Description[300];
FILE *Fhandle;
long Length;
//long file_size;
}S_6;

typedef struct FileData {
int PacketSize;
long Offset;
char Data[500];
}S_7;

typedef struct MsgBuffer {
int Received[MAXUSERS+1],LastSent[MAXUSERS+1];
ECB Ecb;
IPXHeader Ipxheader;
Message message;
}S_8;


typedef struct SuperXferPacket {
unsigned long CRC;
char Sig; //Added support for multiple file requests later.
unsigned long offset;
int packet_size;
char data[SPACKETSIZE];
}S_9;

typedef struct SuperPacketRequest {
char Index;
char Sig;
unsigned long Offset;
int Size;
}S_10;


