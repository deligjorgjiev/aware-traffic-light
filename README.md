# aware-traffic-light
A simple traffic light that operates based on inputs from pedestrians and vehicles, implemented on Raspberry Pi Pico and programmed via Arduino IDE

This is a simple C++ code written in Arduino IDE for the Raspberry Pi Pico MCU that controls traffic lights installed on pedestrian crossings. The inputs are a pedestrian crossing push button, and 4 induction loop-based traffic counters, while outputs being the traffic lights themselves. It regulates the minimum open time based on factors such as information on current congestion from the induction loops and time-of-day. For demo purposes, time-of-day is entered upon initialisation.
