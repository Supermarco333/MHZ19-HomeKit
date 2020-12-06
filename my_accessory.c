/*
 * my_accessory.c
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

// Called to identify this accessory. See HAP section 6.7.6 Identify Routine
// Generally this is called when paired successfully or click the "Identify Accessory" button in Home APP.
void my_accessory_identify(homekit_value_t _value) {
    printf("accessory identify\n");
}


homekit_characteristic_t cha_carbon_dioxide_detected = HOMEKIT_CHARACTERISTIC_(CARBON_DIOXIDE_DETECTED, 0);
homekit_characteristic_t cha_carbon_dioxide_level = HOMEKIT_CHARACTERISTIC_(CARBON_DIOXIDE_LEVEL, 400);
homekit_characteristic_t cha_carbon_dioxide_peak_level = HOMEKIT_CHARACTERISTIC_(CARBON_DIOXIDE_PEAK_LEVEL, 400);
homekit_characteristic_t cha_current_temperature = HOMEKIT_CHARACTERISTIC_(CURRENT_TEMPERATURE, 0);


homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_sensor, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Capteur MH-Z19B"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "ESP8266"),
            HOMEKIT_CHARACTERISTIC(MODEL, "MH-Z19B"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(CARBON_DIOXIDE_SENSOR, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Capteur CO2"),
            &cha_carbon_dioxide_detected,
            &cha_carbon_dioxide_level,
            &cha_carbon_dioxide_peak_level,
            NULL
        }),
        HOMEKIT_SERVICE(TEMPERATURE_SENSOR, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Capteur Temperature"),
            &cha_current_temperature,
            NULL
        }),
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = "463-97-342",
    .setupId = "4B6F"
};
