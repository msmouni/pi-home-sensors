# Pi-Home Sensors

Pi-Home Sensors is the sensor service running on the Raspberry Pi for [Pi-Home](https://github.com/msmouni/pi-home-os) .

It handles sensors connected directly to the embedded Linux system, collects their measurements, and stores the data in [SQLite](https://sqlite.org/) for use by other Pi-Home components such as the [Pi-Home Dashboard](https://github.com/msmouni/pi-home-dashboard).

It uses the common sensor functionality provided by [libsensors](https://github.com/msmouni/libsensors).
