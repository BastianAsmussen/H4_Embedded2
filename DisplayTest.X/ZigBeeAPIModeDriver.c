/*
 * File:   ZigBeeAPIModeDriver.c
 * Author: ltpe
 *
 * Created on 22. februar 2025, 20:50
 */


#include <xc.h>
#include <stdbool.h>
#include <string.h>

#include "ProjectDefines.h"
#include "ZigBeeAPIModeDriver.h"
#include "UART.h"

//void escape_byte(uint8_t byte, uint8_t * output, uint16_t * index) {
// if (byte == 0x7E || byte == 0x7D || byte == 0x11 || byte == 0x13) {
//  output[(*index)++] = 0x7D;         // Escape character
//  output[(*index)++] = byte ^ 0x20;  // XOR with 0x20
// } else {
//  output[(*index)++] = byte;
// }
//}
//
//void send_16bit_frame(uint16_t dest_address, void * driver_data, const uint8_t * payload, uint16_t len) {
// uint8_t frame[128];
// uint16_t length = 0;
// uint8_t sum = 0x01;
//
// // Start delimiter
// frame[length++] = 0x7E;
//
// // Length (MSB, LSB)
// uint16_t frame_data_length = XBEE_FRAME_LEN + len;  // 5 bytes of frame data + payload length
// escape_byte((frame_data_length >> 8) & 0xFF, frame, &length);
// escape_byte(frame_data_length & 0xFF, frame, &length);
//
// // Transmit Request (16-bit address) 0x01
// frame[length++] = 0x01;
// frame[length++] = 0x00;
//
// // Destination address (MSB, LSB)
// sum += (dest_address >> 8) & 0xFF;
// escape_byte((dest_address >> 8) & 0xFF, frame, &length);
// sum += dest_address & 0xFF;
// escape_byte(dest_address & 0xFF, frame, &length);
//
// // no options
// frame[length++] = 0x00;
// // add payload
// for (uint8_t i = 0; i < len; i++) {
//  sum += payload[i];
//  escape_byte(payload[i], frame, &length);
// }
//
// // Checksum after len
// escape_byte(0xFF - sum, frame, &length);
//
// // Send the frame over UART
// for (uint16_t i = 0; i < length; i++) {
//  //USART0_write(frame[i]);
//     printf("%d", frame[i]);  // LTPE
//    //Print_uint8_t(frame[i]); 
// }
//}
//
//
//xbee_ctx_t2 user_data = {.xbee_mode = XBEE_MODE_NOT_STARTED};
//
//void xbee_rx(void) {
//
// uint8_t data = UDR0;
//
// if (data == 0x7D) {
//  user_data.escape_next = true;  // Next byte is escaped
//  return;                        // Wait for next byte
// }
//
// if (user_data.escape_next) {
//  data ^= 0x20;  // Unescape the byte
//  user_data.escape_next = false;
// }
//
// switch (user_data.xbee_mode) {
//  case XBEE_MODE_NOT_STARTED:
//   if (data == 0x7E) {
//    // Start delimiter detected
//    user_data.xbee_mode = XBEE_MODE_LENGTH_MSB;
//    user_data.length = 0;
//    user_data.data_index = 0;
//    user_data.checksum = 0;
//   }
//   break;
//
//  case XBEE_MODE_LENGTH_MSB:
//   user_data.length = data << 8;  // Store MSB of length
//   user_data.xbee_mode = XBEE_MODE_LENGTH_LSB;
//   break;
//
//  case XBEE_MODE_LENGTH_LSB:
//   user_data.length |= data;  // Store LSB of length
//   if (user_data.length > CSP_BUFFER_SIZE - XBEE_FRAME_LEN) {
//    user_data.xbee_mode = XBEE_MODE_NOT_STARTED;
//   } else {
//    user_data.xbee_mode = XBEE_MODE_SKIP_FRAMING;
//   }
//   break;
//  case XBEE_MODE_SKIP_FRAMING:
//   user_data.checksum += data;
//   user_data.data_index++;
//
//   if (user_data.data_index >= XBEE_FRAME_LEN) {
//    user_data.xbee_mode = XBEE_MODE_RECEIVING;
//   }
//   break;
//
//  case XBEE_MODE_RECEIVING:
//   if (user_data.data_index < user_data.length) {
//    user_data.checksum += data;
//    if (user_data.buf_len >= CSP_BUFFER_SIZE) {
//     user_data.xbee_mode = XBEE_MODE_SKIP_FRAMING;
//     break;
//    }
//    user_data.buf[user_data.buflen++] = data;
//    user_data.data_index++;
//   } else {
//    // Last byte is the checksum
//    uint8_t calculated_checksum = 0xFF - user_data.checksum;
//    if (calculated_checksum == data) {
//
//     // Here the data is ready
//     
//     // for (uint16_t i = 0; i < frame_len; i++) {
//     // printf("%02X\n", user_data.rx_packet->frame_begin[i]);
//     //}
//    } else {
//     // printf("%u!%u", calculated_checksum, data);
//     //  Checksum invalid
//    }
//    // Reset state for next frame
//    user_data.xbee_mode = XBEE_MODE_NOT_STARTED;
//   }
//   break;
//
//  default:
//   user_data.xbee_mode = XBEE_MODE_NOT_STARTED;
//   break;
// }
//}
 
// Ret 64 Bit adressen herunder til adressen på jeres egen ZigBee Coordinator !!!!
uint8_t XBEEDestinationAddress64Bit[8] = {0x00, 0x13, 0xA2, 0x00, 0x42, 0x3F, 0x063, 0xF2};
uint8_t XBEEDestinationAddress16Bit[2] = {0xFF, 0xFE};

void xbee_send_api_frame(uint8_t *frame, uint16_t frame_len)
{
    for (uint16_t i = 0; i < frame_len; i++) 
    {
        uart_transmit(frame[i]);
    }
}

void add_byte_with_escaping(uint8_t byte, uint8_t *frame, uint16_t *frame_len) 
{
    if ((XBEEStartDeliminterEscapeCharacter == byte) || 
        (XBEEEscapeCharacter == byte) || 
        (XBEEXonEscapeCharacter ==  byte) || 
        (XBEEXoffEscapeCharacter == byte)) 
    {
        frame[(*frame_len)++] = 0x7D;
        frame[(*frame_len)++] = byte ^ 0x20;
    } 
    else 
    {
        frame[(*frame_len)++] = byte;
    }
}

void add_byte_without_escaping(uint8_t byte, uint8_t *frame, uint16_t *frame_len) 
{
    frame[(*frame_len)++] = byte;
}

void xbee_build_and_send_transmit_packet(char *message)
{
    uint8_t frame[100];
    uint8_t DestinationAddressCounter = 0;
    uint16_t frame_len = 0;
    uint16_t checksum16 = 0;
    uint8_t checksum8 = 0;
    uint16_t frame_checksum_counter = 3;
    uint8_t MessageCounter = 0;
    uint16_t message_length;
    char *start_position = message;
    frame[frame_len++] = 0x7E; // Start delimiter
    frame[frame_len++] = 0x00; // Frame length (MSB) - will be filled later
    message_length = strlen(message);
    frame[frame_len++] = XBEE_FRAME_LEN + message_length; // Frame length (LSB) LTPE
    frame[frame_len++] = 0x10; // Frame type: Transmit Request - Explicit Addressing
    frame[frame_len++] = 0x01; // Frame ID
    // Vi beregner Checksum på den pakke vi sender ud, før vi opbygger den endelige pakke.
    // Dette kan synes lidt mærkeligt, men skyldes, at Checksummen skal beregnes på 
    // den pakke vi sender ud, før vi indsætter eventuelle Escape karaketerer i pakken !!!
    // Tyder lidt på, at der er nogle programmører, der har drukket et par øl for meget,
    // da de bestemete sig for dette !!!
    // Checksum beregningen på en Transmit Pakke sker fra position 3 i pakken !!!
    // Start af pakke karakteren 0x7E (byte 0) og længden af pakken (byte 1 og byte 2) skal
    // altså IKKE indgå i Checksum beregningen.
    for (frame_checksum_counter = 3; frame_checksum_counter < 5; frame_checksum_counter++)
    {
        checksum16 += frame[frame_checksum_counter];
    }
    for (DestinationAddressCounter = 0; DestinationAddressCounter < SizeOfArray(XBEEDestinationAddress64Bit); DestinationAddressCounter++)
    {
        checksum16 += XBEEDestinationAddress64Bit[DestinationAddressCounter]; // 64 bit Destination Address
    }
    for (DestinationAddressCounter = 0; DestinationAddressCounter < SizeOfArray(XBEEDestinationAddress16Bit); DestinationAddressCounter++)
    {
        checksum16 += XBEEDestinationAddress16Bit[DestinationAddressCounter]; // 16 bit Destination Address
    }
    MessageCounter = 0;
    do
    {
        checksum16 += (*message);
        MessageCounter++;
    } while (MessageCounter < message_length);
    message = start_position;
    checksum8 = checksum16 & 0xFF;
    checksum8 = 0xFF - checksum8;
    frame_len = FrameStartPointForAddingEscapeCharacters;
    for (DestinationAddressCounter = 0; DestinationAddressCounter < SizeOfArray(XBEEDestinationAddress64Bit); DestinationAddressCounter++)
    {
        add_byte_with_escaping(XBEEDestinationAddress64Bit[DestinationAddressCounter], frame, &frame_len); // 64 bit Destination Address
    }
    for (DestinationAddressCounter = 0; DestinationAddressCounter < SizeOfArray(XBEEDestinationAddress16Bit); DestinationAddressCounter++)
    {
        add_byte_with_escaping(XBEEDestinationAddress16Bit[DestinationAddressCounter], frame, &frame_len); // 16 bit Destination Address
    }
    add_byte_without_escaping(0x00, frame, &frame_len); // Broadcast Radius
    add_byte_without_escaping(0x00, frame, &frame_len); // Options
    strcpy((char *)&frame[frame_len], message);
    frame_len += strlen(message);
    frame[frame_len] = checksum8;
    // Update frame length
    frame[1] = (frame_len - 4) >> 8;
    frame[2] = (frame_len - 4) & 0xFF;
    xbee_send_api_frame(frame, frame_len + 1);
}