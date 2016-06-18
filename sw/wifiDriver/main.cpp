#include <QCoreApplication>
#include <iwlib.h>
#include <QDateTime>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

//#define WIFI_INTERFACE "wlp0s29u1u2"
#define WIFI_INTERFACE "wlan0"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    bool running = true;

    wireless_scan_head head;
    wireless_scan *result;
    iwrange range;
    int     sock;
    char    buffer[128];


    while(running == true)
    {
        /* Open socket to kernel */
        sock = iw_sockets_open();
        result = NULL;

        memset((char*)&head,0,sizeof(result));
        memset((char*)&range,0,sizeof(range));

        /* Get some metadata to use for scanning */
        if (iw_get_range_info(sock, WIFI_INTERFACE, &range) < 0)
        {
            printf("Error during iw_get_range_info. Aborting.\n");
            exit(2);
        }

        /* Perform the scan */
        if (iw_scan(sock, WIFI_INTERFACE, range.we_version_compiled, &head) < 0)
        {
            printf("Error during iw_scan. Aborting.\n");
            exit(2);
        }
        else
        {
            /* Traverse the results */
            result = head.result;
            while (NULL != result)
            {
                int8_t level = result->stats.qual.level;
                iw_saether_ntop(&result->ap_addr, buffer);  //get mac address in string form
                fprintf(stdout, "dbm %s %hd \n", buffer , level);
                fflush(stdout);

                result = result->next;
            }
        }

        iw_sockets_close(sock);
    }

    return a.exec();
}
