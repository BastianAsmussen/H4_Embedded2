#ifndef ZigBeeAPIModeDriver_H_
#define ZigBeeAPIModeDriver_H_

#define XBEE_FRAME_LEN 5
#define CSP_BUFFER_SIZE 32

#define XBEEStartDeliminterEscapeCharacter   0x7E
#define XBEEEscapeCharacter                  0x7D
#define XBEEXonEscapeCharacter               0x11  // (Software Flow Control)
#define XBEEXoffEscapeCharacter              0x13  // (Software Flow Control)

#define CheckpointStartPosition              3
#define FrameStartPointForAddingEscapeCharacters 5


typedef enum
{
    XBEE_MODE_NOT_STARTED,
    XBEE_MODE_LENGTH_LSB,
    XBEE_MODE_LENGTH_MSB,
    XBEE_MODE_SKIP_FRAMING,
    XBEE_MODE_RECEIVING
}xbee_receive_state_t;

typedef struct {
 xbee_receive_state_t xbee_mode;
 uint16_t data_index;
 uint16_t length;
 uint8_t checksum;
 bool escape_next;
 uint8_t buf[CSP_BUFFER_SIZE];
 uint16_t buf_len;
} xbee_ctx_t2;

extern void xbee_build_and_send_transmit_packet(char *message);

#endif