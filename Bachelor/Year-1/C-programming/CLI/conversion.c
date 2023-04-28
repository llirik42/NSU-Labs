#include "conversion.h"

double celsiusToFahrenheit(const double celsius){
    return 1.8 * celsius + 32;
}
double fahrenheitToCelsius(const double fahrenheit){
    return (5 * fahrenheit - 160) / 9;
}

double celsiusToKelvin(const double celsius){
    return celsius + 273.15;
}
double kelvinToCelsius(const double kelvin){
    return kelvin - 273.15;
}


double metersToFeet(const double meters){
    return meters * 3.28084;
}
double feetToMeters(const double feet){
    return feet * 0.3048;
}

double metersToMiles(const double meters){
    return meters * 0.00062;
}
double milesToMeters(const double miles){
    return miles * 1609.34;
}


double gramsToPounds(const double grams){
    return grams * 0.0022;
}
double poundsToGrams(const double pounds){
    return pounds * 453.59;
}

double gramsToOunces(const double grams){
    return grams * 0.035;
}
double ouncesToGrams(const double ounces){
    return ounces * 28.35;
}
