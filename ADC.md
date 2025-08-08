# Load Weigh Scale Feature
This project implements a load weigh scale feature using the Analog Devices AD4131-8 Analog-to-Digital Converter (ADC) interfaced with a Maxim Integrated MAX32650 microcontroller via SPI. The system is designed to read data from load cells, process the ADC output, and convert it into meaningful weight measurements.

## Features
- SPI Communication
- Register Configuration
- Multi-Channel Reading
- ADC Data Conversion
- Scale Calibration
## ADC configuration
The EVAL-AD4131-8 has programmable registers which require proper configuration to ensure appropriate gain, data conversion, and low noise for the load cells. 
We utilized ADI's ACE (Analysis, Control, Evaluation) software to simulate the evaluation and control of the board with various configurations. The configuration that is well-suited for out project is included in `AD4131_mm.csv`.

## Software Setup
### File Structure
___
`4131.c`: Contains the C implementation for interacting with the AD4131-8 ADC via SPI, including register read/write functions and ADC data acquisition.

`4131.h`: Header file for 4131.c, defining ADC register addresses and function prototypes.

`load_cell.c`: Implements the load cell specific logic, such as calibration, averaging of ADC readings, and conversion of ADC codes to grams.

`load_cell.h`: Header file for load_cell.c, defining load cell related function prototypes.

`main.c`: The main application entry point, initializing SPI, configuring the ADC, and handling load cell data processing.

### Hardware Setup
1. Connect MAX32650 to AD4131-EVAL:

- Ensure both boards are powered correctly
    - MAX 3.3V to AD4131 5V Pin

- Connect the SPI lines:
    - MAX32650 `SPI_MISO` to AD4131 `SDO`
    - MAX32650 `SPI_MOSI` to AD4131 `SDI`
    - MAX32650 `SPI_SCK` to AD4131 `SCLK`
    - MAX32650 `GPIO_CS` to AD4131 `CS`

### References
- [ADC data sheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ad4131-8.pdf)
- [MCU data sheet](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX32650FTHR.pdf)
- [Load Cell Guide](https://www.anyload.com/load-cell-specifications-guide/)