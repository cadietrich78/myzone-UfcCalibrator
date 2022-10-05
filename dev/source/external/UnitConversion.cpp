/*
 * COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
 *
 * 
 * For the purposes of this copyright and license, "Contributing Authors"
 * is defined as the following set of individuals:
 *
 *    Carlos Augusto Dietrich (cadietrich@gmail.com)
 *
 * This library is supplied "AS IS".  The Contributing Authors disclaim 
 * all warranties, expressed or implied, including, without limitation, 
 * the warranties of merchantability and of fitness for any purpose. 
 * The Contributing Authors assume no liability for direct, indirect, 
 * incidental, special, exemplary, or consequential damages, which may 
 * result from the use of the this library, even if advised of the 
 * possibility of such damage.
 *
 * Permission is hereby granted to use, copy, modify, and distribute this
 * source code, or portions hereof, for any purpose, without fee, subject
 * to the following restrictions:
 *
 * 1. The origin of this source code must not be misrepresented.
 *
 * 2. Altered versions must be plainly marked as such and must not be 
 *    misrepresented as being the original source.
 *
 * 3. This Copyright notice may not be removed or altered from any source 
 *    or altered source distribution.
 *
 * The Contributing Authors specifically permit, without fee, and 
 * encourage the use of this source code as a component in commercial 
 * products. If you use this source code in a product, acknowledgment 
 * is not required but would be appreciated.
 *
 * 
 * "Software is a process, it's never finished, it's always evolving. 
 * That's its nature. We know our software sucks. But it's shipping! 
 * Next time we'll do better, but even then it will be shitty. 
 * The only software that's perfect is one you're dreaming about. 
 * Real software crashes, loses data, is hard to learn and hard to use. 
 * But it's a process. We'll make it less shitty. Just watch!"
 */

#include <Common.h>
#include <Logger.h>

#include <UnitConversion.h>

/**
*/
template <>
double UnitConversion::Picosecond2Microsecond(double psec)
{
    return psec / 1000000.0;
}

/**
*/
template <>
double UnitConversion::Microsecond2Millisecond(double musec)
{
    return musec / 1000.0;
}

/**
*/
template <>
my::int64 UnitConversion::Microsecond2Millisecond(my::int64 musec)
{
    return musec / 1000;
}

/**
*/
template <>
double UnitConversion::Microsecond2Second(double musec)
{
	return musec / 1000000.0;
}

/**
*/
template <>
double UnitConversion::Millisecond2Second(double msec)
{
	return msec / 1000.0;
}

/**
*/
template <>
my::int64 UnitConversion::MillisecondToMicrosecond(my::int64 msec)
{
    return msec * 1000LL;
}

/**
*/
template <>
double UnitConversion::MillisecondToMicrosecond(double msec)
{
    return msec * 1000.0;
}

/**
*/
template <>
double UnitConversion::Millisecond2Minute(double msec)
{
    return msec / 60000.0;
}

/**
*/
template <>
double UnitConversion::Second2Microsecond(double sec)
{
    return sec * 1000000.0;
}

/**
*/
template <>
my::int64 UnitConversion::Second2Microsecond(my::int64 sec)
{
    return sec * 1000000LL;
}

/**
*/
template <>
my::int32 UnitConversion::Second2Microsecond(my::int32 sec)
{
    return sec * 1000000;
}

/**
*/
template <>
double UnitConversion::Second2Millisecond(double sec)
{
    return sec * 1000.0;
}

/**
*/
template <>
my::int64 UnitConversion::Second2Millisecond(my::int64 sec)
{
    return sec * 1000;
}

/**
*/
template <>
double UnitConversion::Second2Minute(double sec)
{
    return sec / 60.0;
}

/**
*/
template <>
double UnitConversion::Centimeters2Feet(double centimeters)
{
    return centimeters * 0.0328083989501312;
}

/**
*/
template <>
double UnitConversion::Centimeters2Inch(double centimeters)
{
    return centimeters * 0.3937007874015748;
}

/**
*/
template <>
double UnitConversion::Feet2Centimeters(double feet)
{
    return feet * 30.48;
}

/**
*/
template <>
double UnitConversion::Feet2Inch(double feet)
{
	return 12.0 * feet;
}

/**
*/
template <>
double UnitConversion::Feet2Meter(double feet)
{
	return 0.3048 * feet;
}

/**
*/
template <>
double UnitConversion::Feet2Millimeters(double feet)
{
    return feet * 304.8;
}

/**
*/
template <>
double UnitConversion::Inch2Centimeter(double inch)
{
    return 2.54 * inch;
}

/**
*/
template <>
float UnitConversion::Inch2Feet(float inch)
{
	return 0.0833333333333333f * inch;
}

/**
*/
template <>
double UnitConversion::Inch2Feet(double inch)
{
	return 0.0833333333333333 * inch;
}

/**
*/
template <>
float UnitConversion::Meter2Feet(float meter)
{
    return 3.280839895013123f * meter;
}

/**
*/
template <>
double UnitConversion::Meter2Feet(double meter)
{
    return 3.280839895013123 * meter;
}

/**
*/
template <>
double UnitConversion::Meter2Yard(double meter)
{
    return 1.093613298337708 * meter;
}

/**
*/
template <>
double UnitConversion::Yard2Feet(double yard)
{
    return 3.0 * yard;
}

/**
*/
template <>
double UnitConversion::FeetPerMillisecondToFeetPerSecond(double feetMsec)
{
    return 1000.0 * feetMsec;
}

/**
*/
template <>
double UnitConversion::FeetPerSecond2MeterPerSecond(double feetPerSecond)
{
    return 0.3048 * feetPerSecond;
}

/**
*/
template <>
double UnitConversion::FeetPerSecondToMilesPerHour(double feetSec)
{
    return 0.681818181818182 * feetSec;
}

/**
*/
template <>
double UnitConversion::KilometerPerHour2FeetPerMilllisecond(double kilometerPerHour)
{
    return 0.0009113444152814232 * kilometerPerHour;
}

/**
*/
template <>
double UnitConversion::KilometerPerHour2FeetPerSecond(double kilometerPerHour)
{
    return 0.9113444152814232 * kilometerPerHour;
}

/**
*/
template <>
double UnitConversion::MeterPerSecond2FeetPerSecond(double meterPerSecond)
{
	return 3.280839895013123 * meterPerSecond;
}

/**
*/
template <>
double UnitConversion::MeterPerSecond2MilesPerHour(double meterPerSecond)
{
	return 2.236936292054402 * meterPerSecond;
}

/**
*/
template <>
double UnitConversion::MilesPerHour2FeetPerMilllisecond(double milesPerHour)
{
    return 0.001466666666666667 * milesPerHour;
}

/**
*/
template <>
double UnitConversion::MilesPerHour2FeetPerSecond(double milesPerHour)
{
    return 1.466666666666667 * milesPerHour;
}

// Conversion base : 1 ft/s2 = 0.3048 m/s2
template <>
double UnitConversion::FeetPerSquareSecondToMetersPerSquareSecond(double feetPerSquareSecond)
{
    return 0.3048 * feetPerSquareSecond;
}

/**
*/
template <>
double UnitConversion::KilogramsToPounds(double kilograms)
{
    return kilograms * 2.204622621848776;
}

/**
*/
template <>
double UnitConversion::PoundsToKilograms(double pounds)
{
    return 0.45359237 * pounds;
}

