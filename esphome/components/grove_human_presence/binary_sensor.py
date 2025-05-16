import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_OCCUPANCY,
    DEVICE_CLASS_OCCUPANCY,
    ICON_MOTION_SENSOR,
)

from . import CONF_AK975X_ID, GroveHumanPresenceComponent

DEPENDENCIES = ["grove_human_presence"]

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_AK975X_ID): cv.use_id(GroveHumanPresenceComponent),
    cv.Optional(CONF_OCCUPANCY): binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_OCCUPANCY,
        icon=ICON_MOTION_SENSOR,
    ),
}


async def to_code(config):
    grove_human_presence_component = await cg.get_variable(config[CONF_AK975X_ID])

    if occupancy_config := config.get(CONF_OCCUPANCY):
        sensor = await binary_sensor.new_binary_sensor(occupancy_config)
        cg.add(grove_human_presence_component.set_occupancy_binary_sensor(sensor))
