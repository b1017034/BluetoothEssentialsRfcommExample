#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <wiringPi.h>

#define GPIO_PIN 21

int main(int argc, char **argv)
{
    //struct sockaddr_rc loc_addr = {00:1B:DC:04:AE:D7}, rem_addr = {44:d4:e0:c8:45:11};
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read, status;
    socklen_t opt = sizeof(rem_addr);
    int ch = 1 < argc ? atoi(argv[1]) : 1;
    printf("channel [%d]\n",ch);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    printf("socket [%d]\n",s);

    // bind socket to port 22 of the first available
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) ch;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    if(wiringPiSetupGpio() == -1) return 0;
    pinMode(GPIO_PIN,INPUT);
    int data;

    while(1){

        if( digitalRead(GPIO_PIN) == HIGH){
            char msg[5] = "Hello";

            //測距センサーのレスポンス確認
            //printf( "%d", digitalRead(GPIO_PIN) == HIGH);
            data = digitalRead(GPIO_PIN);

            status = write(client, msg, 5);

            printf("send:%s status: %d pin:%d HIGH:%d \n",msg ,status ,data, HIGH);

            if( status < 0 ) break;
        }

        delay(1000);
    }

    //close connection
    close(client);
    close(s);
    return 0;
}
