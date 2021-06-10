//
// Created by jay on 6/2/21.
//

#ifndef J5C_INSTALL_FROM_SOURCE_001_CONST_EXPRESSIONS_H
#define J5C_INSTALL_FROM_SOURCE_001_CONST_EXPRESSIONS_H

//constant expressions
constexpr auto commandPosition = 19;
constexpr auto daysIn400Years = 146097L;
constexpr auto seconds_in_minute = 60;
constexpr auto seconds_in_hour   = 60 * seconds_in_minute;
constexpr auto seconds_in_day    = 24 * seconds_in_hour;
// the actual value is not constant, but we can average over 400 years to make it closer
constexpr auto average_seconds_in_year = (daysIn400Years * seconds_in_day)/400.0;


#endif //J5C_INSTALL_FROM_SOURCE_001_CONST_EXPRESSIONS_H
