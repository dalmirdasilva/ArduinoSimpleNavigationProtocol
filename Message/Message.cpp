#include "Message.h"
#include <string.h>

Message::Message(uint8_t type)
        : type(type), flags(0), payload(0), payloadSize(0) {
}

Message::~Message() {
}

uint8_t Message::getType() {
    return this->type;
}

void Message::pack(uint8_t *buf) {
    uint16_t sum = computeChecksum(this->payload);
    buf[SNP_MESSAGE_TYPE_OFFSET] = type;
    buf[SNP_MESSAGE_FLAG_OFFSET] = flags;
    buf[SNP_MESSAGE_PAYLOAD_SIZE_OFFSET] = payloadSize;
    memcpy(&buf[SNP_MESSAGE_PAYLOAD_OFFSET], this->payload, payloadSize);
    memcpy(&buf[SNP_MESSAGE_PAYLOAD_OFFSET + payloadSize], (uint8_t*) sum, SNP_MESSAGE_CHECKSUM_SIZE);
}

uint8_t Message::unpack(uint8_t *buf) {
    uint16_t sum, computedSum = computeChecksum(&buf[SNP_MESSAGE_PAYLOAD_OFFSET]);
    memcpy(this->payload, &buf[SNP_MESSAGE_PAYLOAD_OFFSET], payloadSize);
    memcpy((uint8_t*) &sum, &buf[SNP_MESSAGE_PAYLOAD_OFFSET + payloadSize], SNP_MESSAGE_CHECKSUM_SIZE);
    if (computedSum != sum) {
        return UNPACK_CHECKSUM_MISMATCH;
    }
    return UNPACK_SUCCESS;
}

uint8_t Message::getLength() {
    return SNP_MESSAGE_HEADER_SIZE + this->payloadSize + SNP_MESSAGE_CHECKSUM_SIZE;
}

uint16_t Message::computeChecksum(uint8_t* buf) {
    uint16_t i, sum = 0;
    for (i = 0; i < SNP_MESSAGE_CHECKSUM_SIZE; i++) {
        sum += buf[i];
    }
    return sum;
}
