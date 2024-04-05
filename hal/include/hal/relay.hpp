#ifndef _RELAY_HPP_
#define _RELAY_HPP_

/**
 * Handle opening and closing the relay to open and close lock
 */
class Relay {
  public:
    Relay();

    void open();
    void close();
    bool isOpen();
};

#endif
