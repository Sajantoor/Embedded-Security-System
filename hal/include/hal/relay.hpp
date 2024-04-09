#ifndef _RELAY_HPP_
#define _RELAY_HPP_

/**
 * Handle opening and closing the relay to open and close lock
 */
class Relay {
  public:
    Relay();

    /* Opens the door */
    void open();
    /* Closes the door */
    void close();
    bool isOpen();
};

#endif
