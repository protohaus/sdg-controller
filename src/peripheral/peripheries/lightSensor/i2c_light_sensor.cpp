#include "i2c_light_sensor.h"

namespace bernd_box {
namespace peripheral {
namespace peripheries {
namespace light_sensor {

I2CLightSensor::I2CLightSensor(const JsonObjectConst& parameter)
    : I2CAbstractPeriphery(parameter) {}

}  // namespace light_sensor
}  // namespace peripheries
}  // namespace peripheral
}  // namespace bernd_box