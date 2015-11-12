#include "AckMessage.h"
#include <Message.h>

AckMessage::AckMessage()
        : Message(ACK_MESSAGE_TYPE) {
    payloadSize = (uint8_t) sizeof(_payload);
    flags = SNP_ACK_MESSAGE_FLAGS;
    payload = (uint8_t *) &_payload;
}
