/**
 * Handle opening and closing the relay to open and close lock
*/

#ifndef _RELAY_HPP_
#define _RELAY_HPP_

class Relay{
    public:
        Relay();
        ~Relay();

        void openRelay();
        void closeRelay();
};

#endif