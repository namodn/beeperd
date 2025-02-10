#include <stdio.h>
#include <errno.h>
#include <unistd.h>

/*
 * beeper daemon - calls 'PHN-NMBR,,,,,0*0*0*0*0' when argv[1] is down
 * ( also see beeperd.dip )
 * 
 * Copyright Robert Helmer/Namodn Enterprises 1999, redistribution allowed 
 * for any purpose as per the GPL. Please read the attached file 'COPYING' 
 * for license info
 */

int main ( int argc, char **argv )
{
   char *IP_address;
   char command[256];
   int ping_result;
   int main_loop;
   int ping_loop;
   int modem_open;
   int done_been_beeped; 

   IP_address = argv[1];
   sprintf( command, "ping -c 1 %s > /dev/null", IP_address );
   main_loop=1;
   ping_loop=1;   
  
   if (argc != 2)
   {
      printf("beeperd 0.0.1\nCopyright 1999 Robert Helmer/Namodn Enterprises\nbeeperd is free software, covered by the GNU General Public License.\nYou may modify and/or redistribute this software!\nSee the file COPYING and http://robert.namodn.com for more.\n\n" );
      fprintf(stderr, "Usage: %s IP_address\n",
      argv[0]);
 
      exit(0);
   }

   if (fork())
   {
      printf("Forked child OK Exit now ...\n");
      exit(0);
   }  

   for( ;; main_loop++ )
   {
      ping_result = system( command );
      printf( "%i\n", ping_result );
      if( ping_result != 0 )
         {
            for( ;; ping_loop++ )
	       {	
	          if( ping_loop >= 10 )
		  {
		      printf( "Calling the beeper..\n" );
		      ping_result = system( "dip ./beeperd.dip" );
		      done_been_beeped=1;
		      break;
		   }
		   else
		      break;
               }
	       if( done_been_beeped == 1 )
	          break;
         } 
      if( main_loop == 1 )
      {
         if( done_been_beeped == 1 )
            break;
         main_loop=0;
         sleep( 300 );     /* 5 minutes */
      }
   }
return 0;
}
