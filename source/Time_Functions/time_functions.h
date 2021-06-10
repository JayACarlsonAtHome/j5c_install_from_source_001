//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"


#ifndef J5C_INSTALL_FROM_SOURCE_001_TIME_FUNCTIONS_H
#define J5C_INSTALL_FROM_SOURCE_001_TIME_FUNCTIONS_H

time_t get_Time()
{
    time_t rawtime;
    time (&rawtime);
    return rawtime;
}

template <typename T>
sstr get_Time_Part(T timePart)
{
    sstr strTimePart = std::to_string(timePart);
    if (timePart < 10) {
        strTimePart = "0" + strTimePart;
    }
    return strTimePart;
}

sstr get_GmtOffset()
{
    time_t theTime;
    struct tm * timeinfo;
    time (&theTime);
    timeinfo = localtime (&theTime);

    long gmt    = timeinfo->tm_gmtoff;
    long gmtHoursOffset = gmt / 3600;

    sstr strGmtOff  = std::to_string(gmtHoursOffset);
    sstr offset;
    if (gmtHoursOffset > -1)
    {
        offset = get_Time_Part<long>(gmtHoursOffset);
        if (gmtHoursOffset == 0)
        {
            strGmtOff = "( " + offset + ")";
        } else
        {
            strGmtOff = "(+" + offset + ")";
        }
    }
    else
    {
        gmtHoursOffset *= -1;
        offset = get_Time_Part<long>(gmtHoursOffset);
        strGmtOff = "(-" + offset + ")";
    }
    return strGmtOff;
}

sstr get_Time_as_String(time_t theTime)
{
    struct tm * timeinfo;
    //if (theTime == 0)
    //{
    //    time (&theTime);
    //}
    timeinfo = localtime (&theTime);

    int hours   = timeinfo->tm_hour;
    int minutes = timeinfo->tm_min;
    int seconds = timeinfo->tm_sec;

    sstr strHours   = get_Time_Part<int>(hours);
    sstr strMinutes = get_Time_Part<int>(minutes);
    sstr strSeconds = get_Time_Part<int>(seconds);

    sstr time = strHours + ":" + strMinutes + ":" + strSeconds + " ";

    return time;
}
sstr make_fileName_dateTime(time_t theTime)
{
    sstr thefileTime = get_Time_as_String(theTime);
    J5C_DSL_Code::j5c_Date d1{};
    sstr theDate = d1.strDate();
    theDate = theDate.replace(4,1,"_");
    theDate = theDate.replace(7,1,"_");
    theDate.append("_at_");
    theDate.append(thefileTime);
    theDate = theDate.replace(16,1,"_");
    theDate = theDate.replace(19,1,"_");
    if (theDate.length() > 22)
    {
        theDate = theDate.substr(0,22);
    }
    return theDate;
}

sstr getDuration(time_t stop, time_t start)
{
    // we are going to give a positive duration
    //   even if the parameters get switched

    long long secondsTotal = 0;
    if (start > stop)
    {
        secondsTotal = start - stop;
    }
    else
    {
        secondsTotal = stop - start;
    }

    // We are only showing 3 digits for the year,
    //    and if that is not enough we have other problems...
    int years = static_cast<int>(secondsTotal / average_seconds_in_year);
    long long remainingSeconds = static_cast<long long>(secondsTotal - (years * average_seconds_in_year));

    // We are only showing 3 digits for the year,
    //    and if that is not enough we have other problems...
    if (years > 999) years = 999;
    sstr strYears = padLeftZeros(3, years);

    // There can only be 3 digits here so we are safe with an int
    int days = static_cast<int>(remainingSeconds / seconds_in_day);
    remainingSeconds = remainingSeconds - (days * seconds_in_day);
    sstr strDays = padLeftZeros(3, days);

    // There can only be 2 digits here so we are safe with an int
    int hours = static_cast<int>(remainingSeconds / seconds_in_hour);
    remainingSeconds = remainingSeconds - (hours * seconds_in_hour);
    sstr strHours = padLeftZeros(2, hours);

    // There can only be 2 digits here so we are safe with an int
    int minutes = static_cast<int>(remainingSeconds / seconds_in_minute);
    remainingSeconds = remainingSeconds - (minutes * seconds_in_minute);
    sstr strMinutes = padLeftZeros(2, minutes);

    // There can only be 2 digits here so we are safe with an int
    int seconds = static_cast<int>(remainingSeconds);
    sstr strSeconds = padLeftZeros(2, seconds);

    sstr result = strYears;
    result.append(":");
    result.append(strDays);
    result.append(":");
    result.append(strHours);
    result.append(":");
    result.append(strMinutes);
    result.append(":");
    result.append(strSeconds);
    result.append(" ");
    return result;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_TIME_FUNCTIONS_H
