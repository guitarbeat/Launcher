#include "powerSave.h"
#include "idf/launcher_platform.h"
#include "settings.h"

/* Turn off the display */
void turnOffDisplay() { setBrightness(0, false); }

/* If the device screen is off, turn on, else just refresh sleep timer */
bool wakeUpScreen() {
    previousMillis = launcherMillis();
#if defined(STICKY_MONOCHROME) || defined(E_PAPER_DISPLAY)
    // E-paper displays have no backlight and never dim; never swallow button presses for wake
    return false;
#else
    if (isScreenOff) {
        isScreenOff = false;
        dimmer = false;
        getBrightness();
        return true;
    } else if (dimmer) {
        dimmer = false;
        getBrightness();
        return true;
    }
    return false;
#endif
}

/* Check if it's time to put the device to sleep */
void checkPowerSaveTime() {
#if defined(STICKY_MONOCHROME) || defined(E_PAPER_DISPLAY)
    // E-paper displays cannot dim; do not trigger false dimmer or screen-off state
    return;
#else
    if (dimmerSet != 0) {
        if ((launcherMillis() - previousMillis) >= (dimmerSet * 1000) && dimmer == false &&
            isSleeping == false) {
            dimmer = true;
            setBrightness(5, false);
        } else if (
            (launcherMillis() - previousMillis) >= ((dimmerSet * 1000) + 5000) && isScreenOff == false &&
            isSleeping == false
        ) {
            isScreenOff = true;
            turnOffDisplay();
        }
    }
#endif
}

/* Put device on sleep mode */
void sleepModeOn() {
    isSleeping = true;
#ifndef CONFIG_IDF_TARGET_ESP32P4
    setCpuFrequencyMhz(80);
    turnOffDisplay();
    disableCore0WDT();
#if SOC_CPU_CORES_NUM > 1
    disableCore1WDT();
#endif
    disableLoopWDT();
#endif
}

/* Wake up device */
void sleepModeOff() {
    isSleeping = false;
#ifndef CONFIG_IDF_TARGET_ESP32P4
    setCpuFrequencyMhz(CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ);
    enableCore0WDT();
#if SOC_CPU_CORES_NUM > 1
    enableCore1WDT();
#endif
    enableLoopWDT();
    feedLoopWDT();
#endif
    getBrightness();
}
