
#define CRC32_POLYNOMIAL 0xEDB88320L

class TCRCMAN
{
	unsigned long CRCTable[256];

  public:
	TCRCMAN();
	~TCRCMAN();
	unsigned long GetCRC(unsigned long Seed, char *Buffer, int Size);
};

TCRCMAN::TCRCMAN()
{
	int i, j;
	unsigned long crc;

	for (i = 0; i <= 255; i++)
	{
		crc = i;
		for (j = 8; j > 0; j--)
		{
			if (crc & 1)
				crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
			else
				crc >>= 1;
		}
		CRCTable[i] = crc;
	}
}

TCRCMAN::~TCRCMAN()
{
	return;
}

unsigned long TCRCMAN::GetCRC(unsigned long Seed, char *Buffer, int Size)
{
	unsigned long crc;
	unsigned char *p;
	unsigned long temp1;
	unsigned long temp2;

	p = (unsigned char *)Buffer;
	crc = Seed;

	while (Size-- != 0)
	{
		temp1 = (crc >> 8) & 0x00FFFFFFL;
		temp2 = CRCTable[((int)crc ^ *p++) & 0xff];
		crc = temp1 ^ temp2;
	}
	return crc;
}
