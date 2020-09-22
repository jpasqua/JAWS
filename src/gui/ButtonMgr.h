#ifndef ButtonMgr_h
#define ButtonMgr_h

static const int DebounceDelay = 50;

class Button {
public:
  typedef void (*Handler)(Button* button);

  uint8_t   _pin;
  uint8_t   _state;

  Button(uint8_t pin, Handler handler) {
    _pin = pin;
    _handler = handler;
    _state = 1;
    _lastBounce = 0;
    pinMode(_pin, INPUT_PULLUP);
  }

private:
  uint32_t  _lastBounce;
  Handler   _handler;
  friend class ButtonMgr;
};

class ButtonMgr  {
public:
  ButtonMgr(Button *buttons, uint8_t nButtons) {
    _buttons = buttons;
    _nButtons = nButtons;
  }

  void process() {
    unsigned long curMillis = millis();
    for (int i = 0; i < _nButtons; i++) {
      Button* b = &_buttons[i];
      int state = digitalRead(b->_pin);
      if (b->_state != state) {
        if (curMillis - b->_lastBounce > DebounceDelay) {
          b->_state = state;
          b->_handler(b);
        }
        b->_lastBounce = curMillis;
      }
    }
  }

private:
  uint8_t _nButtons;
  Button* _buttons;
};

#endif  // ButtonMgr_h
