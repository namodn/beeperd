#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/*
 * beeper daemon - calls 'PHN-NMBR,,,,,0*0*0*0*0' when argv[1] is down
 * ( also see beeperd.dip )
 * 
 * Copyright Robert Helmer/Namodn Enterprises 1999, redistribution allowed 
 * for any purpose as per the GPL. Please read the attached file 'COPYING' 
 * for license info
 */

int errorlog( char *errorstring )
{
   char errorcommand[256];
   sprintf( errorcommand, "echo %s >> /var/log/beeperd.log", errorstring );
   system( errorcommand );
   return 0;
}

int pingloop( const char *IP_address )
{
   char command[256];		/* Command to be run if server is down */
   int status;			/* Host is Up or Down? */
   int ping_result;		/* errno for ICMP bounce goes here */
   int main_loop;		
   int ping_loop;
   int done_been_beeped;	/* beeper has been activated if true */

   sprintf( command, "ping -c 1 %s > /dev/null", IP_address );
   main_loop=1;
   ping_loop=1;
   status=0;

   for( ;; main_loop++ )
   {
      ping_result = system( command );
      if( ping_result == 0 )
      {
         if( !status )
         {
            errorlog( "Host is Up" );
            status = 1;
         }
      }

      if( ping_result != 0 )
      {
            if( status )
            {
               errorlog( "Host Not Responding" );
               status = 0;
            }
         ping_loop++;
         if( ping_loop == 30 ) /* If host is not up, try for 5 minutes */
         {
            errorlog( "Calling the Beeper" );
            ping_result = system( "dip /etc/beeperd/beeperd.dip" );
            done_been_beeped=1;
            break;
         }
      }
      if( done_been_beeped == 1 )
            break;
      if( main_loop == 1 )
      {
         if( done_been_beeped == 1 )
         {
            system( "echo Hi! This is beeperd. I have been unable to reach the outside world for 15 minutes. I have paged Nick and died. Please restart me! Thanks, beeperd. | sendmail admin@namodn.com" );
            break;
         }
         main_loop=0;
         sleep( 600 );    /* check every 10 minutes */  
      }
   }
return ping_result;
}


int main ( int argc, char **argv )
{
   char *IP_address;
   int beeperd;

   IP_address = argv[1];
  
   if (argc != 2)
   {
      printf("beeperd 0.0.1\nCopyright 1999 Robert Helmer/Namodn Enterprises\nbeeperd is free software, covered by the GNU General Public License.\nYou may modify and/or redistribute this software!\nSee the file COPYING and http://robert.namodn.com for more.\n\n" );
      fprintf(stderr, "Usage: %s IP_address\n",
      argv[0]);
 
      exit(0);
   }
   
   if (fork())
   {
      errorlog( "Beeperd Started!" );
      exit(0);
   }


   beeperd = pingloop( IP_address );

return 0;
}

