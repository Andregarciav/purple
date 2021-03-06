//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

char *geraPayload(int N){
    char byte = 0;
    time_t semente;
    char *pkt = (char *) malloc ((N)*sizeof(char));
    if (!pkt)
        return NULL;
    int i = 0;
    srand((unsigned) time(&semente));
    for(i=0; i < N ; i++){
        byte = rand() % (126-33+1)+33; //range da função rand
        pkt[i] = byte;
    }
    return pkt;
}

int main(int argc, char **argv) {
    int pru_data, pru_clock; // file descriptors
    uint8_t sinebuf[490];
    // char  txbuffer[100];
    char *txbuffer;
    char *conteudo;
   // txbuffer[0] = 'g';
    int buffer_size = 0;
    ssize_t readpru, writepipe, prime_char, pru_clock_command;
    struct timeval tempoInicio, tempoFim;
    double t0, t1, total;
    
    // Create a file
    //  Now open the PRU1 clock control char device and start the clock.
    if(argc == 2){    
	    // printf("%d\n", strlen(argv[1]));
	    // buffer_size = strlen(argv[1]);
    	// for(int i = 0 ; i < strlen(argv[1]); i++){
		//     txbuffer[i+1] = argv[1][i];
		//     //printf("%c\n", txbuffer[i+1]);
        // }
        buffer_size = atoi(argv[1]);
        printf ("buffer_size: %d\n", buffer_size);
	txbuffer = (char*)malloc ((buffer_size + 1)*sizeof(char));
        conteudo = geraPayload(buffer_size);
	txbuffer[0] = 'g';
        for(int i = 0 ; i < buffer_size; i++){
		    txbuffer[i+1] = conteudo[i];
		    printf("%c\n", txbuffer[i+1]);
        }

        pru_clock = open("/dev/rpmsg_pru31", O_RDWR);
        gettimeofday(&tempoInicio, NULL);
        pru_clock_command = write(pru_clock, txbuffer, buffer_size+2 );
        gettimeofday(&tempoFim, NULL);
        if (pru_clock_command < 0)
            printf("The pru clock start command failed.");
        printf("Message sent successfully...\n");
        t0 = (double)tempoInicio.tv_sec+(double)tempoInicio.tv_usec/1000000;
        t1 = (double)tempoFim.tv_sec+(double)tempoFim.tv_usec/1000000;
        total = t0 - t1;
        printf("Duration time: %f", total);
    }   
}
