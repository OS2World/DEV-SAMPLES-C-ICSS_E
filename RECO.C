/**********************************************************************/
/*                IBM Continuous Speech Series  1.0                   */
/**********************************************************************/
/* Licensed Materials - Property of IBM                               */
/* IBM Continuous Speech Series.                                      */
/* (C) Copyright IBM Corporation 1992, 1993. All rights reserved.     */
/*                                                                    */
/* US Government Users Restricted Rights -                            */
/* Use, duplication or disclosure restricted by GSA ADP Schedule      */
/* Contract with IBM Corporation.                                     */
/**********************************************************************/
/*                                                                    */
/* reco.c                                                             */
/* ICSS Example API Program                                           */
/*                                                                    */
/**********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "icssapi.h"

#define SIZEBUFFER 256

void quit(char *msg, long rc);
void PrintCopyrightBanner(void);

/*
 * main
 * -----------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
   char    inputbuf[SIZEBUFFER];
   char    buffer[SIZEBUFFER];
   long    rc      = -1;
   long    context = 0;
   long    notdone = 1;
   long    spoke_too_soon;
   float   utt_len;
   float   acq_time;
   float   reco_time;

   if(argc != 2)
   {
      printf("The reco example program requires one argument.\n");
      printf("The name of a context to do recognition against.\n");
      printf("Example: reco /icss/appl/samples/rl/context/rl.m.ctx\n");
      return 0;
   }
   PrintCopyrightBanner();

   rc = ICSSStart(NULL);
   if (rc != ICSS_SUCCESS)  quit("Error: Start Failed",rc);

   rc = ICSSStartConversation(
                              "icssacpa",          /* ADC TYPE       */
                              ICSS_DEFAULT_INT,    /* ADC Number     */
                              ICSS_DEFAULT_STRING, /* rp system name */
                              ICSS_DEFAULT_INT);   /* rp link type   */

   if (rc != ICSS_SUCCESS)  quit("Error: StartConversation Failed",rc);

   rc = ICSSLoadContext(&context, argv[1], ICSS_DEFAULT_FLOAT,
                        ICSS_DEFAULT_FLOAT, ICSS_DEFAULT_FLOAT);
   if (rc != ICSS_SUCCESS)  quit("Error: Define Context failed",rc);

   while(notdone)
   {
      printf("\nListen?[yes,no]>");
      fflush(stdout);
      gets(inputbuf);
      if( (strstr(inputbuf,"NO" ) != NULL) ||
          (strstr(inputbuf,"no" ) != NULL) )   { break; }
      else
      {
         rc = ICSSListen(context, ICSS_DEFAULT_INT, NULL);

         if(rc != ICSS_SUCCESS)  quit("Error: Listen Failed",rc);

         printf("\nBEGIN SPEAKING\n");

         rc = ICSSGetSpokenWords(SIZEBUFFER-1, buffer, &spoke_too_soon,
                                &utt_len, &acq_time, &reco_time);
         if (rc == ICSS_SUCCESS)
         {
            printf("The spoken text was: [%s]\n",buffer);
            printf("Utterance Length %f\n",utt_len);
            printf("Acquistion Time  %f\n",acq_time);
            printf("Recognition Time %f\n",reco_time);
            if (strstr(buffer,"EIGHT"))
              system("RECO e:/test/devita/HAZMAT.CTX");
         }
         else
         {
            quit("Error: GetSpokenWords Failed",rc);
         }
      }
   }

   rc = ICSSEndConversation();
   if (rc != ICSS_SUCCESS)  quit("Error: Conversation End Failed",rc);

   ICSSEnd();

   return 0;
}

/*
 * quit
 * -----------------------------------------------------------------
 */
void quit(char *msg, long rc)
{
   printf("%s rc=%d %s\n",msg,rc,ICSSErrorString[rc]);
   exit(-1);
   return;
}

/*
 * print banner
 * -----------------------------------------------------------------
 */
void     PrintCopyrightBanner(void)
{
  printf("**********************************************************************\n");
  printf("*                IBM Continuous Speech Series  1.0                   *\n");
  printf("**********************************************************************\n");
  printf("* Licensed Materials - Property of IBM                               *\n");
  printf("* IBM Continuous Speech Series.                                      *\n");
  printf("* (C) Copyright IBM Corporation 1992, 1993. All rights reserved.     *\n");
  printf("*                                                                    *\n");
  printf("* US Government Users Restricted Rights -                            *\n");
  printf("* Use, duplication or disclosure restricted by GSA ADP Schedule      *\n");
  printf("* Contract with IBM Corporation.                                     *\n");
  printf("**********************************************************************\n");
  printf("\n\n\n");
  fflush(stdout);
  /*sleep(2);*/
  return;
}
