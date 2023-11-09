#include "customecatal.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("CustomEcatAL(Custom EtherCAT Application Layer)\n");

    if (argc > 1) {
        EthercatInit(argv[1]);
        uint8_t ob = 0;
        int8_t dir = 1;
        while (1) {
            EthercatProcessdataLoop();
            for (int i = 0; i < getIbytes(1); i++) {
                printf(" %2.2x ", getInputByte(1, i));
            }
            printf("\r");
            fflush(stdout);
            ob += dir;
            if (ob > 6 || 0 == ob)
                dir *= -1;
            setOutputByte(1, 0, ~(1<<ob));
            setOutputByte(2, 0, (1<<ob));
            usleep(80000);
        }  
    } else {
        printf("no ifname\n");
    }

    return 0;
}