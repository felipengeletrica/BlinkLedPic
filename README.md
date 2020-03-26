# uCscheduling use MPLABX and XC8 for compile
This program uses MICROCHIP PIC18F4550 microcontroller and has a structure of timers generics for process which can be used for counting time, processes, communications timeout. It is easily ported to other microcontrollers just provide an interruption of one milli second.

# This Program:
  Send every 10 seconds (Task) "Keep Alive" through the serial port and flash one LED every 1 second using a flexible timer structure. It can create numerous timers so just declaring a structure so your programs can process several "task"
  
  ## Tests
  ### Blink Led in realitme:
  ![alt text](https://raw.githubusercontent.com/felipengeletrica/uCscheduling/master/pictures/Board.jpg)

  ### Hardware and tests
  ![alt text](https://raw.githubusercontent.com/felipengeletrica/uCscheduling/master/pictures/BoardAndOsciloscope.jpg)
  
  
  ### Preemptive timers schedules
  ![alt text](https://raw.githubusercontent.com/felipengeletrica/uCscheduling/master/pictures/Osciloscope.jpg)
