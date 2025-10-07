#include "../../../intf/stdio.h"
#include "picPrivate.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

// list of offsets
#define VENDORID 0x0
#define DEVICEID 0x2
#define COMMAND 0x4
#define STATUS 0x6
#define REVISIONID 0x8
#define PROGIF 0x9
#define SUBCLASS 0xA
#define CLASS 0xB
#define CACHELINE 0xC
#define LATENCYTIMER 0xD
#define HEADERTYPE 0xE
#define BIST 0xF

// offset specific to bridges
#define SECONDARY_BUS 0x19

// class codes
#define UNCLASSIFIED 0x0
#define MASS_STORAGE_CONTROLLER 0x1
#define NETWORK_CONTROLLER 0x2
#define BRIDGE 0x6

// * subclass codes
// mass storage controller
#define SCSI_BUS_CONTROLLER 0x0
#define IDE_CONTROLLER 0x1
#define FLOPPY_DISK_CONTROLLER 0x2
// network controller
#define ETHERNET_CONTROLLER 0x0
// bridge
#define HOST_BRIDGE 0x0
#define PCI_PCI_BRIDGE 0x4
#define CARDBUS_BRIDGE 0x6

uint8_t pciReadConfigByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t retval;

    uint32_t address = (uint32_t)((lbus << 16) | (lslot << 11)
     | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000)
    );

    // write to get le data
    outl(CONFIG_ADDRESS, address);

    // ((offset & 2) * 8) = 0 will choose the first word of 32-bit register
    retval = (uint8_t)((inl(CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return retval;
}

uint16_t pciReadConfigWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t retval;

    uint32_t address = (uint32_t)((lbus << 16) | (lslot << 11)
     | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000)
    );

    // write to get le data
    outl(CONFIG_ADDRESS, address);

    // ((offset & 2) * 8) = 0 will choose the first word of 32-bit register
    retval = (uint16_t)((inl(CONFIG_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
    return retval;
}

void checkAllBuses(void) {
    uint8_t function;
    uint8_t bus;

    uint16_t header_type = pciReadConfigByte(0, 0, 0, HEADERTYPE);
    if ((header_type & 0x80) == 0) {
        // is not multifunction
        checkBus(0);
    } else {
        // is multifunction
        for (function = 0; function < 8; function++) {
            if (pciReadConfigWord(0, 0, function, VENDORID) == 0xFFFF) break;
            bus = function;
            checkBus(bus);
        }
    }
}

void checkBus(uint8_t bus) {
    uint8_t device;

    for (device = 0; device < 32; device++) {
        checkDevice(bus, device);
    }
}

void checkDevice(uint8_t bus, uint8_t device) {
    uint8_t function = 0;
    uint8_t baseClass;
    uint8_t subClass;

    uint16_t vendorID = pciReadConfigWord(bus, device, function, VENDORID);

    printf("Device %d: %d/%d/%d, %d/%d, %d/%d\n", device, bus, device, function, 0, vendorID, baseClass, subClass);

    if (vendorID == 0xFFFF) return; // doesn't exist
    checkFunction(bus, device, function);
    uint16_t header_type = pciReadConfigByte(bus, device, function, HEADERTYPE);
    if ((header_type & 0x80) != 0) {
        // this means it is a multifunction device... more work for me
        for (function = 1; function < 8; function++) {
            if (pciReadConfigWord(bus, device, function, VENDORID) != 0xFFFF) {
                printf("Device %d: %d/%d/%d, %d/%d, %d/%d\n", device, bus, device, function, 0, vendorID, baseClass, subClass);
                checkFunction(bus, device, function);
            }
        }
    }
}

void checkFunction(uint8_t bus, uint8_t device, uint8_t function) {
    uint8_t baseClass;
    uint8_t subClass;
    uint8_t secondaryBus;

    baseClass = pciReadConfigByte(bus, device, function, CLASS);
    subClass = pciReadConfigByte(bus, device, function, SUBCLASS);
    if ((baseClass == BRIDGE) && (subClass == PCI_PCI_BRIDGE)) {
        secondaryBus = pciReadConfigByte(bus, device, function, SECONDARY_BUS);
        checkBus(secondaryBus);
    }
}
