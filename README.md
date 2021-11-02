# Robomow RL series remote

## What?

Quick and dirty Arduino code to being able to control your Robomow RL series mower* with a PPM remote.

* Tested with Robomow City 120 however likely to work with RL-1000, RL-2000 RL-550, RL-850, etc.

# Necessary equipment

 * PPM compatible RC transmitter an receiver (tested with a DJI Phantom MK1 remote an receiver)
 * Arduino Pro micro (however the code shall be portable)
 
# Connection info

 * Hook your PPM output to the Arduino's pin 3
 * Remove the terminal from the mower
 * Connect your Serial1 port TX pin to the RX pin of mower's terminal RJ11 (pin 4)
 * Power your Arduino and the mower
 * Profit!
