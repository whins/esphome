import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv

from esphome.const import CONF_ID, CONF_DISABLED

DEPENDENCIES = ["i2c"]
# CODEOWNERS = [""]
MULTI_CONF = True

CONF_POLLING_INTERVAL = "5s"
CONF_I2C_ADDR = 0x64

grove_human_presence_ns = cg.esphome_ns.namespace("grove_human_presence")
GroveHumanPresenceComponent = grove_human_presence_ns.class_(
    "GroveHumanPresenceComponent", cg.PollingComponent, i2c.I2CDevice
)

CONF_AK975X_ID = "ak975x_id"

CONF_SENSITIVITY_PRESENCE = "sensitivity_presence"
CONF_SENSITIVITY_MOVEMENT = "sensitivity_movement"
CONF_DETECT_INTERVAL = "detect_interval"
CONF_CONTINOUS_READING = "continous_reading"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(GroveHumanPresenceComponent),
        cv.Optional(CONF_SENSITIVITY_PRESENCE, default=1.0): cv.All(
            cv.float_range(min=1.0, max=5.0),
        ),
        cv.Optional(CONF_SENSITIVITY_MOVEMENT, default=2.0): cv.All(
            cv.float_range(min=1.0, max=15.0),
        ),
        cv.Optional(CONF_DETECT_INTERVAL, default=30): cv.All(
            cv.float_range(min=10, max=500),
        ),
        cv.Optional(CONF_CONTINOUS_READING, default=False): cv.All(
            cv.boolean,
        ),
        cv.Optional(CONF_DISABLED, default=False): cv.All(
            cv.boolean,
        ),
    }
)


CONFIG_SCHEMA = cv.All(
    CONFIG_SCHEMA.extend(cv.polling_component_schema(CONF_POLLING_INTERVAL)).extend(
        i2c.i2c_device_schema(CONF_I2C_ADDR)
    )
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
