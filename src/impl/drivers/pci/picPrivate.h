#ifndef PICPRIVATE_H
#define PICPRIVATE_H

#include <stdint.h>

uint8_t pciReadConfigByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

uint16_t pciReadConfigWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

void checkBus(uint8_t bus);

void checkDevice(uint8_t bus, uint8_t device);

void checkFunction(uint8_t bus, uint8_t device, uint8_t function);

void checkAllBuses(void);

#endif