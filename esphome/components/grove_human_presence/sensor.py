import esphome.codegen as cg
from esphome.components import i2c, sensor, binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_TEMPERATURE,
    DEVICE_CLASS_TEMPERATURE,
    UNIT_CELSIUS,
    ICON_THERMOMETER,
    DEVICE_CLASS_OCCUPANCY,
    CONF_OCCUPANCY,
    CONF_MOTION,
    DEVICE_CLASS_MOTION,
    ICON_MOTION_SENSOR,
    STATE_CLASS_MEASUREMENT,
)

DEPENDENCIES = ["i2c"]

CONF_I2C_ADDR = 0x64
CONF_POLLING_INTERVAL = "5s"

grove_human_presence_ns = cg.esphome_ns.namespace("grove_human_presence")
GroveHumanPresenceComponent = grove_human_presence_ns.class_(
    "GroveHumanPresenceComponent", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(GroveHumanPresenceComponent),
            # cv.Optional(CONF_OCCUPANCY): binary_sensor.binary_sensor_schema(
            #     icon=ICON_MOTION_SENSOR,
            #     device_class=DEVICE_CLASS_OCCUPANCY,
            #     state_class=STATE_CLASS_MEASUREMENT,
            # ),
            cv.Optional(CONF_MOTION): sensor.sensor_schema(
                icon=ICON_MOTION_SENSOR,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_MOTION,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema(CONF_POLLING_INTERVAL))
    .extend(i2c.i2c_device_schema(CONF_I2C_ADDR))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    for key in [CONF_TEMPERATURE, CONF_MOTION]:
        if sensor_config := config.get(key):
            sensor_ = await sensor.new_sensor(sensor_config)
            cg.add(getattr(var, f"set_{key}_sensor")(sensor_))
