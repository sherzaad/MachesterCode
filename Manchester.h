/*
Manchester Coding Proof of Concept.
This Library was based on SoftwareSerial(formerly NewSoftSerial) Library.
8N2 Serial <--> Manchester Code
Half duplex implementation
*/

#ifndef Manchester_h
#define Manchester_h

/******************************************************************************
  Definitions
******************************************************************************/

#ifndef _SS_MAX_RX_BUFF
#define _SS_MAX_RX_BUFF 64 // RX buffer size
#endif

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

class Manchester
{
  private:
    // per object data
    uint8_t _receivePin;
    uint8_t _receiveBitMask;
    volatile uint8_t *_receivePortRegister;
    uint8_t _transmitBitMask;
    volatile uint8_t *_transmitPortRegister;
    volatile uint8_t *_pcint_maskreg;
    uint8_t _pcint_maskvalue;

    // Expressed as 4-cycle delays (must never be 0!)
    uint16_t _rx_delay_centering;
    uint16_t _rx_delay_intrabit;
    uint16_t _rx_delay_stopbit;
    uint16_t _tx_delay;

    uint16_t _buffer_overflow: 1;
    uint16_t _inverse_logic: 1;

    // static data
    static uint16_t _receive_buffer[_SS_MAX_RX_BUFF];
    static volatile uint8_t _receive_buffer_tail;
    static volatile uint8_t _receive_buffer_head;
    static Manchester *active_object;

    // private methods
    inline void recv() __attribute__((__always_inline__));
    uint8_t rx_pin_read();
    void setTX(uint8_t transmitPin);
    void setRX(uint8_t receivePin);
    inline void setRxIntMsk(bool enable) __attribute__((__always_inline__));

    // Return num - sub, or 1 if the result would be < 1
    static uint16_t subtract_cap(uint16_t num, uint16_t sub);

    // private static method for timing
    static inline void tunedDelay(uint16_t delay);

  public:
    // public methods
    Manchester(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);
    ~Manchester();
    void begin(long speed);
    bool listen();
    void end();
    bool isListening() {
      return this == active_object;
    }
    bool stopListening();
    bool overflow() {
      bool ret = _buffer_overflow;
      if (ret) _buffer_overflow = false;
      return ret;
    }
    int16_t peek();

    int8_t write(uint8_t b, uint8_t start_bit= 1, uint8_t stop_bit= 2, uint8_t parity_bit = 0); //parity_bit -  0: no parity, 1: even, 2:odd
    int16_t read();
    int16_t available();

    operator bool() {
      return true;
    }

    // public only for easy access by interrupt handlers
    static inline void handle_interrupt() __attribute__((__always_inline__));
};

#endif
