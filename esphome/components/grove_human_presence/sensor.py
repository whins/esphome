import esphome.codegen as cg
from esphome.components import i2c, sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
)

DEPENDENCIES = ["i2c"]

CONF_I2C_ADDR = 0x64
CONF_POLLING_INTERVAL = "5s"

grove_human_presence_ns = cg.esphome_ns.namespace("grove_human_presence")
GroveHumanPresenceSensor = grove_human_presence_ns.class_(
    "GroveHumanPresenceSensor", sensor.Sensor, cg.PollingComponent, i2c.I2CDevice
)


# CONFIG_SCHEMA = cv.Schema(
#     {
#         cv.Optional(
#             CONF_BINARY_SENSORS,
#             default=[
#                 {
#                     CONF_NAME: "Demo Basement Floor Wet",
#                     CONF_DEVICE_CLASS: DEVICE_CLASS_MOISTURE,
#                 },
#                 {
#                     CONF_NAME: "Demo Movement Backyard",
#                     CONF_DEVICE_CLASS: DEVICE_CLASS_MOTION,
#                 },
#             ],
#         ): [
#             binary_sensor.binary_sensor_schema(DemoBinarySensor).extend(
#                 cv.polling_component_schema("60s")
#             )
#         ],
#         cv.Optional(
#             CONF_SENSORS,
#             default=[
#                 {
#                     CONF_NAME: "Demo Plain Sensor",
#                 },
#                 {
#                     CONF_NAME: "Demo Temperature Sensor",
#                     CONF_UNIT_OF_MEASUREMENT: UNIT_CELSIUS,
#                     CONF_ICON: ICON_THERMOMETER,
#                     CONF_ACCURACY_DECIMALS: 1,
#                     CONF_DEVICE_CLASS: DEVICE_CLASS_TEMPERATURE,
#                     CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
#                 },
#                 {
#                     CONF_NAME: "Demo Temperature Sensor",
#                     CONF_UNIT_OF_MEASUREMENT: UNIT_CELSIUS,
#                     CONF_ICON: ICON_THERMOMETER,
#                     CONF_ACCURACY_DECIMALS: 1,
#                     CONF_DEVICE_CLASS: DEVICE_CLASS_TEMPERATURE,
#                     CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
#                 },
#                 {
#                     CONF_NAME: "Demo Force Update Sensor",
#                     CONF_UNIT_OF_MEASUREMENT: UNIT_PERCENT,
#                     CONF_ACCURACY_DECIMALS: 0,
#                     CONF_DEVICE_CLASS: DEVICE_CLASS_HUMIDITY,
#                     CONF_STATE_CLASS: STATE_CLASS_MEASUREMENT,
#                     CONF_FORCE_UPDATE: True,
#                 },
#                 {
#                     CONF_NAME: "Demo Energy Sensor",
#                     CONF_UNIT_OF_MEASUREMENT: UNIT_WATT_HOURS,
#                     CONF_ACCURACY_DECIMALS: 0,
#                     CONF_DEVICE_CLASS: DEVICE_CLASS_ENERGY,
#                     CONF_STATE_CLASS: STATE_CLASS_TOTAL_INCREASING,
#                 },
#             ],
#         ): [
#             sensor.sensor_schema(DemoSensor, accuracy_decimals=0).extend(
#                 cv.polling_component_schema("60s")
#             )
#         ],
#         cv.Optional(
#             CONF_SWITCHES,
#             default=[
#                 {
#                     CONF_NAME: "Demo Switch 1",
#                 },
#                 {
#                     CONF_NAME: "Demo Switch 2",
#                     CONF_INVERTED: True,
#                     CONF_ICON: ICON_BLUETOOTH,
#                 },
#             ],
#         ): [switch.switch_schema(DemoSwitch).extend(cv.COMPONENT_SCHEMA)],
#         cv.Optional(
#             CONF_TEXT_SENSORS,
#             default=[
#                 {
#                     CONF_NAME: "Demo Text Sensor 1",
#                 },
#                 {
#                     CONF_NAME: "Demo Text Sensor 2",
#                     CONF_ICON: ICON_BLUR,
#                 },
#             ],
#         ): [
#             text_sensor.text_sensor_schema(DemoTextSensor).extend(
#                 cv.polling_component_schema("60s")
#             )
#         ],
#     }
# )


CONFIG_SCHEMA = (
    sensor.sensor_schema(GroveHumanPresenceSensor)
    .extend(
        {
            cv.GenerateID(): cv.declare_id(GroveHumanPresenceSensor),
        }
    )
    .extend(cv.polling_component_schema(CONF_POLLING_INTERVAL))
    .extend(i2c.i2c_device_schema(CONF_I2C_ADDR))
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
    yield i2c.register_i2c_device(var, config)
