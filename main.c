/**
 * @file main.c
 *
 * @brief the main application
 */

#include <stdio.h>
#include <string.h>
#include <hidapi/hidapi.h>

/**
 * @brief tries to open the first compatible Apex keyboard
 * @return a pointer to the respective hid_device or NULL if not keyboard was found
 */
hid_device* find_keyboard()
{
    hid_device* dev = NULL;
    if (hid_init() == 0)
    {
        unsigned short ids[4] = {
            0x1206, //Apex 350
            0x1208, //Apex 300
            0x1200, //Apex
            0x1202  //Apex [RAW]
        };

        for (int i=0; i<4 && dev == NULL; ++i)
            dev = hid_open(0x1038, ids[i], 0);
    }

    return dev;
}

/**
 * @brief application's entry point
 * @param argc number of command line arguments
 * @param argv command line argument array; the first value is always the program's name
 * @return 0 if everything succeeded, -1 otherwise for invalid arguments, 1 if the device was found but the command could not be sent
 */
int main(int argc, char** argv)
{
    int ret;
    if (argc == 2)
    {
        ret = 0;
        hid_device* dev = find_keyboard();

        if (dev != NULL)
        {
            if (strcmp(argv[1], "enable")==0)
            {
                unsigned char buffer[3] = { 2, 0, 2 };
                if (!hid_send_feature_report(dev, buffer, 3))
                    ret = 1;
            }
            else if (strcmp(argv[1], "disable")==0)
            {
                unsigned char buffer[3] = { 2, 0, 1 };
                ret = hid_send_feature_report(dev, buffer, 3);
                if (!hid_send_feature_report(dev, buffer, 3))
                    ret = 1;
            }
            else
            {
                ret = -1;
            }
            hid_close(dev);
        }

        hid_exit();
    }
    else
    {
        ret = -1;
    }

    if (ret == -1)
        printf("Invalid arguments supplied; please use either 'enable' or 'disable'\n");

    return ret;
}