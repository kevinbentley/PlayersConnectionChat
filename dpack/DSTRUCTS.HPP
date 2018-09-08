#define CONNECT_DISCONNECTED 0  //(Player was in game but left)
#define CONNECT_PLAYING 1		//(Player is in game and playing)
#define CONNECT_WAITING 2		//(Player is waiting to start the next level)
#define CONNECT_DIED_IN_MINE 3  //(Player died in mine)
#define CONNECT_FOUND_SECRET 4  //(Player exited mine via secret exit)
#define CONNECT_ESCAPE_TUNNEL 5 //(Player is flying thru escape tunnel)
#define CONNECT_END_MENU 6		//(Player is looking at the kill list or bonus)

// Values for game_status field:
#define NETSTAT_MENU 0
#define NETSTAT_PLAYING 1
#define NETSTAT_BROWSING 2
#define NETSTAT_WAITING 3
#define NETSTAT_STARTING 4
#define NETSTAT_ENDLEVEL 5

// Values for game_flags field
#define NETGAME_FLAG_CLOSED 1
#define NETGAME_FLAG_SHOW_ID 2 // Not currently in use
#define NETGAME_FLAG_SHOW_MAP 4

// Values for gamemode field
#define NETGAME_ANARCHY 0
#define NETGAME_TEAM_ANARCHY 1
#define NETGAME_ROBOT_ANARCHY 2
#define NETGAME_COOPERATIVE 3

typedef long fix;	 //16 bits int, 16 bits frac
typedef short fixang; //angles

typedef unsigned char ubyte;
typedef char byte;
typedef unsigned int uint;
typedef unsigned short ushort;

typedef struct netplayer_info
{
	char callsign[9];
	ubyte server[4];
	ubyte node[6];
	ushort socket;
	char connect;
} netplayer_info;

typedef struct netplayer_req
{
	char callsign[10];
	ubyte server[4];
	ubyte node[6];
	ushort socket;
} netplayer_req;

typedef struct sequence_packet
{
	long type; //36decimal
	netplayer_req player;
} sequence_packet;

typedef struct netgame_info
{
	long type;		  // Always 37 decimal for this packet type
	char packet_type; //$ if request, % if answer
	char game_name[16];
	char team_name[2][9];
	ubyte gamemode;   // (see NETGAME_XXX defines)
	ubyte difficulty; // (0-4)
	ubyte game_status;
	ubyte numplayers;	 // how many active players
	ubyte max_numplayers; //(4 or 8, depending on mode)
	ubyte game_flags;	 // (see NETGAME_FLAG_XXX defines)
	netplayer_info players[8];
	long locations[8];		// starting locations (not always valid)
	short kills[8][8];		// array of who-killed-who
	long levelnum;			// -1 = secret #1
	ubyte protocol_version; // 2 for Registered
	ubyte team_vector;		// bitvector, 0 = blue, 1 = red.
	ushort segments_checksum;
	short team_kills[2];
	short killed[8];
	short kills1[8];
	fix level_time;
	fix control_invul_time; // while level time < this, no damage to controlcen
	long monitor_vector;	// which monitors are exploded, 32-bit bitvector
	long player_score[8];   // valid only during coop games
	ubyte player_flags[8];  // can get info about keys players have
	char mission_name[9];   // filename of mission
	char mission_title[22]; // title of mission
} netgame_info;
