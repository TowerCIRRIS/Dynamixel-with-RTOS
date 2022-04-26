/**
 * @file roboCoreTypes.h
 * @brief types propre à roboCore
 * @version 1.0
 *
 * Version 1.0 2022-04-14-10
 */

#ifndef ROBOCORE_TYPES_H
    #define ROBOCORE_TYPES_H

using namespace std;
#include <stdint.h>
#include <stdbool.h>


typedef struct {
    float x_mm;      // X value in milliometers
    float y_mm;      
    float z_mm;
    
}cartesianPos_rct;





class angleRad_rct
{
    public:
        
        angleRad_rct()
        {

        }

        angleRad_rct(const float radAngle)
        {
            _radAngle = radAngle;
        }

        angleRad_rct & operator=(const angleRad_rct angle)
        {
            _radAngle = angle.rad();
            return *this;  // Return a reference to myself.
        }

        angleRad_rct operator=(const float radAngle)
        {
            _radAngle = radAngle;
            return *this;  // Return a reference to myself.
        }

        double operator+(const double radAngle)
	   {
		   return (double)_radAngle + radAngle;  // Return a reference to internal float
	   }

        angleRad_rct operator+(const angleRad_rct angle)
        {
            angleRad_rct angleCpy;
            angleCpy =  _radAngle + angle.rad();
            return angleCpy;  // Return a reference to myself.
        }

        angleRad_rct operator+(const float radAngle)
		{
			angleRad_rct angleCpy;
			angleCpy =  _radAngle + radAngle;
			return angleCpy;  // Return a reference to myself.
		}

        angleRad_rct operator-(const angleRad_rct angle)
        {
            angleRad_rct angleCpy;
            angleCpy =  _radAngle - angle.rad();
            return angleCpy;
        }

        angleRad_rct operator-(const float radAngle)
		{
			angleRad_rct angleCpy;
			angleCpy =  _radAngle - radAngle;
			return angleCpy;
		}

        angleRad_rct operator-(const double radAngle)
		{
			angleRad_rct angleCpy;
			angleCpy =  (double)_radAngle - radAngle;
			return angleCpy;
		}


        angleRad_rct operator*(const angleRad_rct angle)
        {
            angleRad_rct angleCpy;
            angleCpy =  _radAngle * angle.rad();
            return angleCpy;  // Return a reference to myself.
        }

        angleRad_rct operator*(const float radAngle)
		{
			angleRad_rct angleCpy;
			angleCpy =  _radAngle * radAngle;
			return angleCpy;  // Return a reference to myself.
		}

        angleRad_rct operator*(const double radAngle)
		{
			angleRad_rct angleCpy;
			angleCpy =  (double)_radAngle * radAngle;
			return angleCpy;  // Return a reference to myself.
		}

        angleRad_rct operator/(const angleRad_rct angle)
        {
            angleRad_rct angleCpy;
            angleCpy =  _radAngle / angle.rad();
            return angleCpy;  // Return a reference to myself.
        }

		angleRad_rct operator/(const float radAngle)
		{
			angleRad_rct angleCpy;
			angleCpy =  _radAngle / radAngle;
			return angleCpy;  // Return a reference to myself.
		}

		angleRad_rct operator/(const double radAngle)
		{
			angleRad_rct angleCpy;
			angleCpy =  (double)_radAngle / radAngle;
			return angleCpy;  // Return a reference to myself.
		}



        inline bool operator< (const angleRad_rct rhs){  return _radAngle < rhs.rad();}
        inline bool operator> (const angleRad_rct rhs){  return _radAngle > rhs.rad();}
        inline bool operator<=(const angleRad_rct rhs){ return !(_radAngle > rhs.rad()); }
        inline bool operator>=(const angleRad_rct rhs){ return !(_radAngle < rhs.rad()); } 

        inline bool operator< (const float rhs){  return _radAngle < rhs;}
        inline bool operator> (const float rhs){  return _radAngle > rhs;}
        inline bool operator<=(const float rhs){ return !(_radAngle > rhs); }
        inline bool operator>=(const float rhs){ return !(_radAngle < rhs); }
      

        float rad() const
        {
            return _radAngle;
        }

        void setFromDegrees(const float degVal)
        {
            _radAngle = degVal / 57.29585;
        }

        float degConvert() const 
        {
            return degConvert(_radAngle); //*57.29585;
        }

        float degConvert(const float radAngle) const 
        {
            return radAngle*57.29585;
        }

    private:

    float _radAngle;
};


typedef enum{
    rotary,
    linear

} actuatorType_t;



typedef float temperature_rct;  // In degrees celcius
typedef float current_rct;      // Electrical current in amps
typedef angleRad_rct angularVeocity_rct;      
typedef float rotaryActator_rct;


#endif
