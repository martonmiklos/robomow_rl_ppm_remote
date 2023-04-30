# RC Remote control for Robomow RL series lawn mowers

## What?

Quick and dirty Arduino code to being able to control your Robomow RL series mower* with a PPM remote.

*= Tested with Robomow City 120 however likely to work with RL-1000, RL-2000 RL-550, RL-850, etc. which has similar removable terminal with pigtail cable.

[![In action on Youtube](https://img.youtube.com/vi/UquXWUTpAvM/0.jpg)](https://www.youtube.com/watch?v=UquXWUTpAvM)

# Necessary equipment

 * PPM compatible RC transmitter an receiver (tested with a DJI Phantom MK1 remote an receiver)
 * Arduino Pro micro (the code shall be portable to other Arduinos)
 * Inverter to the Arduino TX line
 * [PPM-reader library from Nikkilae](https://github.com/Nikkilae/PPM-reader)
 
# Connection info - [see schematic in cad folder](https://raw.githubusercontent.com/martonmiklos/robomow_rl_ppm_remote/master/cad/pro_micro_adapter_schematic.pdf)

 * Hook your PPM receiever's output to the Arduino's pin 3
 * Remove the terminal from the mower
 * Connect your Serial1 port TX pin to the RX pin of mower's terminal RJ11 (pin 5)
 * Power your Arduino and the mower
 * Profit!
