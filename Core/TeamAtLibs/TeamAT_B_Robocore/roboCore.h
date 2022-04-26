/**
 * @file roboCore.h
 * @brief Base de la librairie robotique pour les robots teamAt
 * @version 1.0
 *
 * Version 1.0 2022-04-14-10
 */

#ifndef ROBOCORE_H
    #define ROBOCORE_H

    using namespace std;
    #include <vector>

    #include "robocore_actuator.h"
	#include "roboCoreTypes.h"


    class roboCore
    {
    private:
        /* data */
    public:
        roboCore(/* args */);
        ~roboCore();

        vector<robocore_actuator*> actuators;
    };
    
    
    


#endif
