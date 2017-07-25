#ifndef __ERROR_H
#define __ERROR_H

// Status codes for KW1281 communication
typedef enum {
    SUCCESS,
    CONNECT_ERROR,
    GROUP_ERROR,
    RX_ERROR,
    TX_ERROR,
} Error;

#endif
