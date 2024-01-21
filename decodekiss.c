/*
Copyright 2024- G7TAJ - DecodeAX25 KISS packets
This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
If not, see http://www.gnu.org/licenses
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOL int
#define TRUE 1
#define FALSE 0

#define IN_RANGE(n, lower, upper) \
    (((n) >= (lower)) && ((n) <= (upper)))

char OutStr[1000];
BOOL infoFrame = FALSE;

char * Callsign(char * BPQbuff, int S) {
    int I = 0;

    while (I < 6 && ((BPQbuff[I + S] >> 1) != 0x20)) {
        sprintf(OutStr, "%s%c", OutStr, (char)(BPQbuff[I + S] >> 1) );
        I++;
    }
    char St[2];
    sprintf(St, "%d", ((BPQbuff[S + 6] >> 1) & 0x0F)); // Strip SSID
    if (strcmp(St, "0") != 0) {
        sprintf(OutStr, "%s-%s", OutStr, St);
    }
}

void Node_table(char * BPQbuff) {
    int I, S;
    strcat(OutStr, "\n    ");
    S = 23;
    //S = S + 6;
    do {
        sprintf(OutStr, "%s               Node:    ", OutStr);
        Callsign(BPQbuff, S);
        S = S + 7;
        strcat(OutStr, ":");
        I = 0;
        while (I < 6 && BPQbuff[I + S] != 32) {
            sprintf(OutStr, "%s%c", OutStr, BPQbuff[I + S]);
            I = I + 1;
        }
        S = S + 6;
        strcat(OutStr, " -> ");
        Callsign(BPQbuff, S);
        S = S + 7;
        sprintf(OutStr, "%s  %d", OutStr, BPQbuff[S]);
        S = S + 1;
        strcat(OutStr, "\n    ");
    } while (S < strlen(BPQbuff));
}

BOOL Decode_Netrom(char * BPQbuff, int s) {

  strcat(OutStr, "\t(NRom: ");
  Callsign(BPQbuff, s+3);
  strcat(OutStr, " ");
  Callsign(BPQbuff, s+10);

  switch (BPQbuff[s+22] & 0x0F) {
  case 1 :
	   strcat(OutStr, " C)");
	   break;
  case 2 :
           strcat(OutStr, " C AK)");
	   break;
  case 3 :
	   strcat(OutStr," D)");
	   break;
  case 4 :
           strcat(OutStr, " D AK)");
	   break;
  case 5 :
           strcat(OutStr," I)");
           infoFrame = TRUE;
	   break;
  case 6 : strcat(OutStr, " I AK)");
	   break;
  default:
       sprintf(OutStr, "Type %s", (BPQbuff[s+23] & 0x0F) );
       break;
 }

 return infoFrame;
}

BOOL DecodePacket(char * BPQbuff, int len) {
    BOOL info = FALSE;
    int i;
    char St[20];
    OutStr[0]='\0';

    info = FALSE;
    Callsign(BPQbuff,7);
    strcat(OutStr, " to ");
    Callsign(BPQbuff,0);

    int j = 13;
    while ((BPQbuff[j] & 0x01) != 1) {
        strcat(OutStr, " via ");
        sprintf("%s%s", OutStr, Callsign(BPQbuff, j));
        if ((BPQbuff[j+7] & 0x80) == 0x80) {
            strcat(OutStr, "*");
        }
        j = j + 7;
    }

    switch (BPQbuff[j+1] & 0x01) {
        case 0: // An information frame
            switch (BPQbuff[j+2]) { // Case on the PID
                case 0xCF:
                    sprintf(OutStr, "%s ctl I+ pid CF", OutStr);
                    if (Decode_Netrom(BPQbuff, j)) {
			j+=20;
		    }
                    break;
                case 0xF0: // Normal Packet
                    sprintf(OutStr, "%s ctl I+ pid F0", OutStr);
		    infoFrame=TRUE;
                    break;
                default: // Any other PID
                    sprintf(OutStr, "%s PID %d", OutStr, BPQbuff[j+2]);
                    break;
            }
            break;
        case 1: // Must be a U or S frame
            if ((BPQbuff[j+1] & 0x02) == 0) { // Is this a supervisory frame
                St[0] = '\0';
                switch (BPQbuff[j+1] & 0x0C) {
                    case 0x00: sprintf(St, "ctl RR"); break;
                    case 0x04: sprintf(St, "ctl RNR"); break;
                    case 0x08: sprintf(St, "ctl REJ"); break;
                }
                sprintf(OutStr, "%s %s", OutStr, St);
                sprintf(St, "%d", BPQbuff[j+1] >> 5); // Strip out N(R)
                sprintf(OutStr, "%s %s", OutStr, St);
            } else {
                switch (BPQbuff[j+1] & 0xEC) { // U Frame
                    case 0x00:
                        sprintf(OutStr, "%s ctl UI^ pid F0", OutStr);
                        if (((BPQbuff[0] >> 1) == 'N') &&
                            ((BPQbuff[1] >> 1) == 'O') &&
                            (BPQbuff[16] == 0xFF)) {
                                Node_table(BPQbuff);
                            } else {
		                infoFrame=TRUE;
			    }
                        break;
                    case 0x0C: sprintf(OutStr, "%s ctl DM-", OutStr); break;
                    case 0x2C: sprintf(OutStr, "%s ctl SABM+", OutStr); break;
                    case 0x40: sprintf(OutStr, "%s ctl DISC+", OutStr); break;
                    case 0x60: sprintf(OutStr, "%s ctl UA-", OutStr); break;
                    case 0x84: sprintf(OutStr, "%s ctl FRMR", OutStr); break;
                }
            }
            break;
    }

    if (infoFrame) {

        strcat(OutStr, "\r\n  ");
	j+=3;
	while ( j <= len-1 ) {

	  if ( IN_RANGE(BPQbuff[j], 0x00, 0x0C) || IN_RANGE(BPQbuff[j], 0x0E, 0x1F) ) {
		sprintf(OutStr, "%s<%02x>", OutStr, BPQbuff[j]);
	  } else {
		if ( BPQbuff[j] == 0x0D )
		 strcat(OutStr, "\r\n  ");
		else
		 sprintf(OutStr, "%s%c", OutStr, BPQbuff[j]);
          }
	  j++;
	}
    }

}


int main() {
    size_t bufferSize = 300; // Set a suitable buffer size
    char* buffer = (char*) malloc(bufferSize);
    size_t dataIndex = 0;
    int c;

    char input[1024]; // Adjust the size as per your requirements
    int bytesRead = fread(input, sizeof(char), sizeof(input), stdin);

    if ( bytesRead > 17 ) {
       DecodePacket( input , bytesRead);
       printf("\r\n%s", OutStr);
    } else {

//	printf("ACK packet : %d\r\n", input[0]);
    }

    return 0;
}
