import esphome.codegen as cg
from esphome.components import i2c, sensor, binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_TEMPERATURE,
    DEVICE_CLASS_TEMPERATURE,
    UNIT_CELSIUS,
    ICON_THERMOMETER,
    CONF_MOTION,
    DEVICE_CLASS_MOTION,
    ICON_MOTION_SENSOR,
    STATE_CLASS_MEASUREMENT,
)

DEPENDENCIES = ["grove_human_presence"]

from . import CONF_AK975X_ID, GroveHumanPresenceComponent

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_AK975X_ID): cv.use_id(GroveHumanPresenceComponent),
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


async def to_code(config):
    grove_human_presence_component = await cg.get_variable(config[CONF_AK975X_ID])

    if motion_config := config.get(CONF_MOTION):
        snsr = await sensor.new_sensor(motion_config)
        cg.add(grove_human_presence_component.set_motion_sensor_(snsr))

    if temperature_config := config.get(CONF_TEMPERATURE):
        snsr = await sensor.new_sensor(temperature_config)
        cg.add(grove_human_presence_component.set_temperature_sensor_(snsr))
