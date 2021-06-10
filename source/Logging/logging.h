//
// Created by jay on 6/2/21.
//

#include "../const_expressions.h"
#include "../type_abbreviations.h"
#include "../structs.h"
#include "../function_declarations.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_LOGGING_H
#define J5C_INSTALL_FROM_SOURCE_001_LOGGING_H

int reportResults(an_itemValues& itemValues, int installResult)
{
    int result = 0;
    sstr positionCommand = std::string(commandPosition, ' ');
    time_t  now = get_Time();
    sstr strNow = get_Time_as_String(now);

    std::cout << " " << strNow << "Summary: # " << "Install " << itemValues.ProperName
              << "-" << itemValues.version;
    if (itemValues.step < 0)
    {
        std::cout << " : Result = " << installResult << "." << std::endl;
    }
    else
    {
        std::cout << " step " << std::setw(2) << std::setfill('0') << itemValues.step
                  << " : Result = " << installResult << std::endl;
    }
    time_t stop = get_Time();

    result += file_append_results(itemValues.fileName_Build,   itemValues, installResult, stop);
    result += file_append_results(itemValues.fileName_Results, itemValues, installResult, stop);

    print_blank_lines(2);
    return result;
};

int logFinalSettings(sstr& fileNameBuilds, std::map<sstr, sstr>& settings, sstr& programName )
{
    sstr tempProgramName = "";
    int max_set_width = 32;
    if (programName == "perl")
    {
        tempProgramName = programName + "5->";
    }
    else
    {
        tempProgramName = programName + "->";
    }
    sstr pad_string;
    sstr str_buffer;
    std::vector<sstr> generated_settings;
    std::cout << std::endl << std::endl;
    generated_settings.emplace_back("#                 Listing Settings : Values ");
    generated_settings.emplace_back("#============================================================================");
    for (auto element : settings)
    {

        if (element.first.substr(0, tempProgramName.length()) == tempProgramName) {
            auto pad_length = max_set_width - element.first.length();
            pad_string = sstr(pad_length, ' ');
            str_buffer = pad_string + element.first + " : " + element.second;
            generated_settings.push_back(str_buffer);
        }
    }
    file_write_vector_to_file(fileNameBuilds, generated_settings);
    return 0;
}


#endif //J5C_INSTALL_FROM_SOURCE_001_LOGGING_H
