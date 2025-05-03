import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID, DEVICE_CLASS_TEMPERATURE, STATE_CLASS_MEASUREMENT, UNIT_CELSIUS
)
from . import ak9753_ns, AK9753Component, CONF_AK9753_ID

DEPENDENCIES = ["i2c"]

AK9753Sensor = ak9753_ns.class_("AK9753Sensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_CELSIUS,
    accuracy_decimals=2,
    device_class=DEVICE_CLASS_TEMPERATURE,
    state_class=STATE_CLASS_MEASUREMENT,
).extend({
    cv.GenerateID(): cv.declare_id(AK9753Sensor),
    cv.GenerateID(CONF_AK9753_ID): cv.use_id(AK9753Component),
})

async def to_code(config):
    paren = await cg.get_variable(config[CONF_AK9753_ID])
    sens = await sensor.new_sensor(config)
    var = cg.new_Pvariable(config[CONF_ID], sens)
    cg.add(paren.register_sensor(var))