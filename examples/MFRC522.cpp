#include "lib/Serial.h"
#include "lib/SPI.h"
#include "lib/GPIO.h"
#include "lib/MFRC522.h"
#include "lib/Utils.h"

volatile bool flag;

extern "C" {
void EXTI9_5_IRQ_handler(void) {
    GPIO::clearPendingInterrupt(GPIO::Pin::A9);
    flag = true;
}
}

int main() {
    flag = false;
    MFRC522_Uid uid;

    Serial serial(SerialN::S3, 115200);
    serial.print("Uart initialized\n");
    SPI spi(SPIn::SPI2, GPIO::Pin::B12);
    MFRC522 mfrc522(spi);

    //Enable interrupt pin
    Utils::enablePeripheral(Utils::Peripheral::PortA);
    GPIO::setInPin(GPIO::Pin::A9, GPIO::InMode::PullUp);
    Utils::enablePeripheral(Utils::Peripheral::AFIO);
    GPIO::setupInterrupt(GPIO::Pin::A9, GPIO::IntTrigger::Falling);
    mfrc522.PCD_EnableIrq();

    serial.print("Version: ");
    serial.print(mfrc522.getVersion(), 16);
    serial.print("\n");

    while (true) {
        if (flag) {
            if (mfrc522.PICC_ReadCardSerial(uid)) {
                for (int i = 0; i < uid.size; i++) {
                    serial.print(uid.uidByte[i], 16);
                    if (i != uid.size - 1)
                        serial.print(":");
                }

                serial.print("\n");
            }

            mfrc522.PCD_ClearInterrupt();
            mfrc522.PICC_HaltA();
            flag = false;
        }

        mfrc522.PCD_InterruptReactivateReception();

        for (int i = 0; i < 1000; i++)
                __asm__("nop");
    }
}