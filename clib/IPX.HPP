#include <dos.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long LONG;

union REGS regs;
struct SREGS sregs;

typedef struct IPXAddress
{
      BYTE network[4]; // high-low
      BYTE node[6];    // high-low
      BYTE socket[2];  // high-low
} IPXAddress;

typedef struct IPXHeader
{
      WORD checkSum; // high-low
      WORD length;   // high-low
      BYTE transportControl;
      BYTE packetType;
      IPXAddress destination;
      IPXAddress source;
} IPXHeader;

typedef struct ECBFragment
{
      void far *address;
      WORD size; // low-high
} ECBFragment;

typedef struct ECB
{
      void far *linkAddress;
      void(far *ESRAddress)();
      BYTE inUseFlag;
      BYTE completionCode;
      WORD socketNumber;        // high-low
      BYTE IPXWorkspace[4];     // N/A
      BYTE driverWorkspace[12]; // N/A
      BYTE immediateAddress[6]; // high-low
      WORD fragmentCount;       // low-high
      ECBFragment fragmentDescriptor[5];
} ECB;

BYTE IPXInitialize(void)
{
      BYTE rvalue;
      asm 
      {
            mov ax,0x7a00;
            int 0x2f;
            mov rvalue,al;
      }
      return rvalue;
}

int IPXOpenSocket(short socketNumber, int longevity)
{
      BYTE rvalue1;
      short rvalue2;

      asm 
      {
            mov bx,0;
            mov al,0;
            mov dx,socketNumber;
            int 0x7a;
            mov rvalue1,al;
            mov rvalue2,dx;
      }
      if (rvalue1) 
            return -1;
      else 
            return rvalue2;
}

void IPXCloseSocket(short socketNumber)
{
      asm 
      {
            mov bx,1;
            mov dx,socketNumber;
            int 0x7a;
      }
}

int IPXListenForPacket(ECB *ecb)
{
      BYTE rvalue1;
      short ECBSEG, ECBOFS;
      ECBSEG = FP_SEG(ecb);
      ECBOFS = FP_OFF(ecb);

      asm 
      {
        mov si,ECBOFS;
            mov es,ECBSEG;
            mov bx,4;
            int 0x7a;
            mov rvalue1,al;
      }
      if (rvalue1) 
            return 1;
      else 
            return 0;
}

int IPXGetInternetworkAddress(BYTE far *Address)
{
      short ADDRSEG, ADDROFS;
      BYTE rvalue;
      ADDRSEG = FP_SEG(Address);
      ADDROFS = FP_OFF(Address);

      asm 
      {
            mov si,ADDROFS;
            mov es,ADDRSEG;
            mov bx,9;
            int 0x7a;
            mov al,rvalue;
      }
      if (rvalue) 
            return 1;
      else 
            return 0;
}

void IPXSendPacket(ECB *ecb)
{
      short ECBSEG, ECBOFS;
      ECBSEG = FP_SEG(ecb);
      ECBOFS = FP_OFF(ecb);

      asm 
      {
            mov si,ECBOFS;
            mov es,ECBSEG;
            mov bx,3;
            int 0x7a;
      }
}

void IPXGetLocalTarget(BYTE far *TargetAddr, BYTE far *immediateAddress, int *XportTime)
{
      short TGTADDROFS, TGTADDRSEG, IMMEDOFS;
      TGTADDRSEG = FP_SEG(TargetAddr);
      TGTADDROFS = FP_OFF(TargetAddr);
      IMMEDOFS = FP_OFF(immediateAddress);

      asm 
      {
            mov si,TGTADDROFS;
            mov es,TGTADDRSEG;
            mov di,IMMEDOFS;
            mov bx,2;
            int 0x7a;
            mov *XportTime,cx;
      }
      return;
}

void IPXCancelEvent(ECB *ecb)
{
      short ECBSEG, ECBOFS;
      ECBSEG = FP_SEG(ecb);
      ECBOFS = FP_OFF(ecb);

      asm 
      {
            mov si,ECBOFS;
            mov es,ECBSEG;
            mov bx,6;
            int 0x7a;
      }
      return;
}
