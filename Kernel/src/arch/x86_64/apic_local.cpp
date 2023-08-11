#include <arch/x86_64/apic.h>

ApicLocalInterface::ApicLocalInterface(u8 _apicId, ApicDevice* _apic)
    : apicId(_apicId),
      apic(_apic) {
    basePhys   = apic->lReadBase();
    baseVirtIO = getIoMapping(basePhys);
}

ApicLocalInterface::~ApicLocalInterface() {}

void ApicLocalInterface::write(u32 reg, u32 data) {
    *((volatile u32*)(ApicDevice::baseVirtIO + reg)) = data;
}

void ApicLocalInterface::setup() { write(LOCAL_APIC_SIVR, 0x1ff); }

u32 ApicLocalInterface::read(u32 reg) {
    return *((volatile u32*)(ApicDevice::baseVirtIO + reg));
}

void ApicLocalInterface::sendInterrupt(u32 vector) {
    write(LOCAL_APIC_ICR_HIGH, ((u32)apicId) << 24);
    write(LOCAL_APIC_ICR_LOW, vector);

    while ((read((u32)LOCAL_APIC_ICR_LOW) & LOCAL_APIC_ICR_PENDING) != 0)
        ;
}

void ApicLocalInterface::sendInterrupt(u32 dsh, u32 type, u8 vector) {
    sendInterrupt(dsh | type | ICR_VECTOR(vector));
}

void ApicLocalInterface::eoi() { write(LOCAL_APIC_EOI, 0); }