# ADC test code
TODO: Write driver for the AD4131
- Configure the registers (given by John's ACE simulation)
- Initiate read (*note:* simply clocking the SCLK while CS is low is usually enough of a read signal)
- Conversion control
- Read the digital data from the 4131 via MISO line
- Filtering: moving average

The ADC completes its analog-to-digital conversion independently and stores the result internally. When the MCU then performs a read operation over SPI, it's essentially asking the ADC to transmit the most recently completed conversion data that is already available in its output register.
- continuous vs on demand mode?
### Setup
- Connect the MAX 32650 to the AD4131-EVAL board via SPI.

### References
- https://www.anyload.com/load-cell-specifications-guide/
