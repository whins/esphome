import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv

from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

CONF_POLLING_INTERVAL = "5s"
CONF_I2C_ADDR = 0x64

CONF_AK975X_ID = "ak975x_id"
CONF_OCCUPANCY_SENSITIVITY = "occupancy_sensitivity"
CONF_MOTION_SENSITIVITY = "motion_sensitivity"

grove_human_presence_ns = cg.esphome_ns.namespace("grove_human_presence")
GroveHumanPresenceComponent = grove_human_presence_ns.class_(
    "GroveHumanPresenceComponent", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(GroveHumanPresenceComponent),
            cv.Optional(CONF_OCCUPANCY_SENSITIVITY, default=1.0): cv.All(
                cv.float_range(min=1.0, max=6.0),
            ),
            cv.Optional(CONF_MOTION_SENSITIVITY, default=2.0): cv.All(
                cv.float_range(min=1.0, max=12.0),
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
