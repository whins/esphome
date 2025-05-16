# Grove Human Presence Sensor

[Grove - Human Presence Sensor](https://github.com/Seeed-Studio/Grove_Human_Presence_Sensor)

## Using example

```yaml
sensor:
  - platform: grove_human_presence
    name: "Grove Human Presence"
    occupancy:
      name: "Occupancy"
    motion:
      name: "Motion"
    temperature:
      name: "Temperature"

# esp32c3
i2c:
  sda: 8
  scl: 9
```
