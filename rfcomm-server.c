#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <wiringPi.h>

#define GPIO_PIN 21

int main(int argc, char **argv)
{
    //struct sockaddr_rc loc_addr = {00:1B:DC:04:AE:D7}, rem_addr = {44:d4:e0:c8:45:11};
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read, status, val;
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

    // GPIO使えるか確認
    if(wiringPiSetupGpio() == -1) return 0;
    // GPIO_PINをインプットで使用
    pinMode(GPIO_PIN,INPUT);


    /*
      val = 0　ブロッキングモード
      val = 1　ノンブロッキン
      初期設定はブロッキングモードです。
    */
    val = 1;
    ioctl(client, FIONBIO, &val);

    int data;
    while(1){

        if( digitalRead(GPIO_PIN) == HIGH){
            char msg[] = "Hello";

            //測距センサーのレスポンス確認
            data = digitalRead(GPIO_PIN);

            status = write(client, msg, 5);

            printf("send:%s status: %d\n" ,msg ,status);

            // read data from the client
            bytes_read = recv(client, buf, sizeof(buf), 0);

            printf("read [%i]\n", bytes_read );

            if (bytes_read > 0) {
                printf("received [%s]\n", buf);
            }

            if( status < 0 ) break;
        }

        delay(1000);
    }

    //close connection
    close(client);
    close(s);
    return 0;
}
