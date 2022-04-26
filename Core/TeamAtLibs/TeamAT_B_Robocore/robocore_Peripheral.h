#ifndef ROBOCORE_PERIPHERAL_H
    #define ROBOCORE_PERIPHERAL_H


class robocore_Peripheral
{
private:
    uint32_t type;
    uint32_t status;

public:

    uint32_t getType(){return type;}
    uint32_t getStatus(){return status;}
};


#endif