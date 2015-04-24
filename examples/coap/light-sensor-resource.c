#include "coap-common.h"
#include "light-sensor-resource.h"

#if REST_RES_LIGHT_SOLAR
#if REST_RES_LIGHT_SOLAR_PERIODIC
#define REST_RES_LIGHT_SOLAR_RESOURCE REST_PERIODIC_RESOURCE
#else
#define REST_RES_LIGHT_SOLAR_RESOURCE REST_RESOURCE
#endif
#else
#define REST_RES_LIGHT_SOLAR_RESOURCE(...)
#endif

#if REST_RES_LIGHT_PHOTO
#if REST_RES_LIGHT_PHOTO_PERIODIC
#define REST_RES_LIGHT_PHOTO_RESOURCE REST_PERIODIC_RESOURCE
#else
#define REST_RES_LIGHT_PHOTO_RESOURCE REST_RESOURCE
#endif
#else
#define REST_RES_LIGHT_PHOTO_RESOURCE(...)
#endif

#if REST_RES_LIGHT_SOLAR_RAW
#define LIGHT_SOLAR_VALUE REST_FORMAT_ONE_INT("solar", light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR))
#else
#define LIGHT_SOLAR_VALUE REST_FORMAT_ONE_INT("solar", (light_sensor.value(LIGHT_SENSOR_TOTAL_SOLAR) * 46) / 10)
#endif

#if REST_RES_LIGHT_PHOTO_RAW
#define LIGHT_PHOTOSYNTHETIC_VALUE REST_FORMAT_ONE_INT("photo", light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC))
#else
#define LIGHT_PHOTOSYNTHETIC_VALUE REST_FORMAT_ONE_INT("photo", (light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC) * 10) / 7)
#endif

REST_RES_LIGHT_PHOTO_RESOURCE(light_photo,
    REST_RES_LIGHT_PHOTO_PERIOD,
    LIGHT_PHOTOSYNTHETIC_SENSOR_RES,
    IF_SENSOR,
    LIGHT_SENSOR_RT,
    LIGHT_PHOTOSYNTHETIC_VALUE)

REST_RES_LIGHT_SOLAR_RESOURCE(light_solar,
    REST_RES_LIGHT_SOLAR_PERIOD,
    LIGHT_SOLAR_SENSOR_RES,
    IF_SENSOR,
    LIGHT_SENSOR_RT,
    LIGHT_SOLAR_VALUE)
