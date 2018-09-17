# PlayersConnectionChat
This is a 'fully functional replacement for kchat' that we wrote back in 1996. It is a DOS chat application that uses the IPX API and replaces the command line chat program in Kali. There isn't really anything interesting here, other to look at code we wrote 22+ years ago.



# PCON 2.01 - Players Connection IRC Client

## New in 2.01: December 21, 1997
* All known bugs fixed!
* Works while kahn is loaded!
* \/Users lists current Kahn users!



## New in 1.8d: December 10, 1997
* Completely fixed Channel and User mode messages
* Added /ctcp support
* Added AutoJoin channel

## New in 1.8c: December 9, 1997
* Fixed the userlist corruption
* Fixed the way topics are displayed
* Partially fixed channel modes

## Version 1.8b: December 10, 1997

Well, it's been a long road for ol' PCON. It all started back
when Aaron Brinton and I were using Kali, back about 2 1/2 
years ago. Frankly, we thought kchat sucked. So we wrote gmon.
Actually, Aaron worked on the great User interface, while I 
attempted to hack the kali chat protocol.
Originally, gmon had Descent game monitoring built in, but
due to the fear of adding huge amounts of lag to the games in
Kali, we disabled it. At any rate, we had a good user interface,
functional chat, and a bunch of bugs. In fact most of the bugs
were mine, due to my attempt to code while I figured out the 
kchat protocol. Then we got 
involved in [Kahn](http://www.descent2.com/kahn/). So I re-wrote gmon using Aaron's original
user interface. It was a lot more stable, but in part due to 
the simple design of the original chat system, messages had
a tendancy to get lost. So, I re-wrote chat as a IRC client.
I think this was a good idea, but still, it has been a lot of
hard work. So, this version of pcon is actually 4th generation
code, and while it is a lot more stable, it probably still has
tons of bugs. In fact, I know of many bugs in this release, but 
it should work, and fill the immediate needs of the users.

===============================================

Features to be added:
* DCC send and receive (no plans for dcc chat at this point, 
 the userinterface doesn't lend itself well to multiple windows.)
* Lots of other stuff I can't remember

To use with Kahn:
* If you use an old pcon.cfg, make sure to add the lines for IRCServer 
 and IRCPort.
* The IP address of the server in pcon.cfg cannot be a name, it must
 be in dotted IP address form.

To use without Kahn:
* Register Kahn anyways!  :)
* Make sure the kahn.cfg has all the right ip address stuff.

Send any bugs to XXXXXXXX Don't expect a reply, 
I am spending too much time working onfixing the bugs to reply to all 
the emails. Any other correspondance to XXXXXXXX

Written and copyrighted By Kevin Bentley, and Aaron Brinton 1995, 1996, 1997.


PC 0.80 - Players Connection

This is currently a fully functional replacement for kchat.

Please let us know if you have any problems or suggestions.

Thanks,
Aaron Brinton (AaronB - email no longer valid)
Kevin Bentley (Kevlar - email no longer valid)
