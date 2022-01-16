# RC Remote control for Robomow RL series lawn mowers

## What?

Quick and dirty Arduino code to being able to control your Robomow RL series mower* with a PPM remote.

*= Tested with Robomow City 120 however likely to work with RL-1000, RL-2000 RL-550, RL-850, etc.

<iframe width="560" height="315" src="https://www.youtube.com/embed/UquXWUTpAvM" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

# Necessary equipment

 * PPM compatible RC transmitter an receiver (tested with a DJI Phantom MK1 remote an receiver)
 * Arduino Pro micro (the code shall be portable to other Arduinos)
 * Inverter to the Arduino TX line
 
# Connection info - [see schematic in cad folder](https://raw.githubusercontent.com/martonmiklos/robomow_rl_ppm_remote/master/cad/pro_micro_adapter_schematic.pdf)

 * Hook your PPM receiever's output to the Arduino's pin 3
 * Remove the terminal from the mower
 * Connect your Serial1 port TX pin to the RX pin of mower's terminal RJ11 (pin 4)
 * Power your Arduino and the mower
 * Profit!
