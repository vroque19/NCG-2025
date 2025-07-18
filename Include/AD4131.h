#ifndef AD4131_H_
#define AD4131_H_
// move to header file
#define ADC_STATUS 0x00
#define ADC_ADC_CONTROL  0x01
#define ADC_DATA         0x02
#define ADC_IO_CONTROL   0x03
#define ADC_VBIAS_CONTROL       0x04
#define ADC_ID           0x05
#define ADC_ERROR        0x06
#define ADC_ERROR_EN     0x07
#define ADC_MCLK_COUNT   0x08
// The channel
// for each channel, a predefined ADC setup can be selected
#define ADC_CHANNEL_X(x) (0x09+(x))
// Each ADC setup consists of config, filter, gain and offset
#define ADC_CONFIG_X(x) (0x19+(x))
#define ADC_FILTER_X(x) (0x21+(x))
#define ADC_OFFSET_X(x) (0x29+x)
#define ADC_GAIN_x(x) (0x31+(x))
#define ADC_MISC 0x39

//* ADC Control Register (0x01), Reset: 0x4000 *//
#define BIPOLAR 1 << 14 // bipolar
#define REF_VAL 1 << 13 // internal ref: 1.25V
#define CONT_READ 1 << 11 // enable
#define DATA_STATUS 1 << 10 // append status 
#define CSB_EN 1 << 9 // 4 wire mode
#define REF_EN 1 << 8 // internal ref enable
#define DUTY_CYC_RATIO 1 << 6
#define MODE_X_MASK GENMASK(5, 2) // 0b0: cont. conversion mode by default
#define CLK_SEL GENMASK(1, 0)

/* Satus Register */
#define RDYB 1 << 7
#define MAIN_ERR 1 << 6

#endif // AD4131_H_