//
// Created by jay on 6/2/21.
//
#include "enums.h"
#include "type_abbreviations.h"
#include "structs.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_FUNCTION_DECLARATIONS_H
#define J5C_INSTALL_FROM_SOURCE_001_FUNCTION_DECLARATIONS_H

void createProtectionWhenRequired(int result, an_itemValues& itemValues, bool show);
void howToRemoveFileProtection(an_itemValues& itemValues);
void print_blank_lines(int count);
void protectProgram_IfRequired(an_itemValues& itemValues, bool show);
void set_bInstall(an_itemValues &itemValues);
void section_already_loaded(sstr& programName, sstr& version);

sstr create_php_configuration(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
sstr delimitPath(sstr& path);
sstr getDigitsInStringAsString(sstr& some_value);
sstr getDuration(time_t stop, time_t start);
sstr get_GmtOffset();
sstr get_InstallOS(OS_type thisOS_Type);
sstr get_ProtectionFileWithPath(an_itemValues& itemValues);
sstr get_sha256sum(an_itemValues& itemValues);
sstr get_Time_as_String(time_t theTime);
sstr get_xxx_Path(const sstr& xxxPath, const sstr& replacement);
sstr getProperNameFromString(sstr& some_value);
sstr getProtectedFileName(sstr& programName);
sstr getValid_pVersion(sstr& some_value);
sstr joinPathParts(sstr& partA, sstr& partB);
sstr joinPathWithFile(sstr& partA, sstr& fileName);
sstr lowerCaseString(sstr& some_value);
sstr make_fileName_dateTime(time_t theTime);
sstr multilineCommand(std::vector<sstr>& commands, bool remove_last_continuation_char = false);
sstr padLeftZeros(int max_width, int number);
sstr removeCharFromStartOfString(sstr& str, char c );
sstr removeCharFromEnd__OfString(sstr &str, char c);
sstr safeFilePath(sstr& path);
sstr stripCharFromString(sstr& inString, const char c);
sstr trimLeftAndRight(sstr& inString, sstr& ws);
sstr unDelimitPath(sstr& path);
sstr upperCaseString(sstr& some_value);


int apache_notes(an_itemValues& itemValues);
int appendNewLogSection(sstr &fileName);
int badSha256sum(an_itemValues& itemValues);
int basicInstall(an_itemValues& itemValues, sstr& configureStr);
int basicInstall_tcl(an_itemValues& itemValues, sstr& configureStr);
int configure(an_itemValues& itemValues,  sstr& configureStr);
int create_apache_conf_File(an_itemValues &itemValues);
int create_file(const sstr& fileName);
int create_mariaDB_cnf_File(an_itemValues &itemValues);
int createTargetFromStage(an_itemValues& itemValues);
int decrementResultIfTesting(an_itemValues& itemValues, int in_result);
int do_configure(an_itemValues& itemValues, sstr& configureStr);
int do_command(sstr& fileName, std::vector<sstr>& vec, bool createScriptOnly = false);
int do_make(an_itemValues& itemValues);
int do_make_clean(an_itemValues& itemValues);
int do_make_install(an_itemValues& itemValues, int results);
int do_make_tests(an_itemValues& itemValues);
int do_post_install(std::map<sstr, sstr>& settings, an_itemValues& itemValues, int results);
int ensure_Directory_exists1(sstr& path);
int ensure_Directory_exists2(sstr& basePath, sstr& path);
int ensure_Directory_main(sstr& fullCommand, int result, bool show = false);
int ensure_file(sstr &fileName);
int ensure_GroupExists(an_itemValues itemValues, sstr groupName);
int ensure_UserExists(an_itemValues itemValues, sstr groupName, sstr userName);
int ensureApacheUserAndGroupExists(an_itemValues& itemValues);
int ensureMariaDB_UserAndGroupExist(an_itemValues& itemValues);
int ensureStageDirectoryExists(an_itemValues &itemValues);
int ensureWrkDirExist(an_itemValues& itemValues);
int install_apache(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_apache_step_01(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_apache_step_02(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_apache_step_03(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_apache_step_04(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_apache_step_05(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_apt_required_dependencies();
int install_apt_required_dependencies(sstr& fileName, sstr& programName, bool createScriptOnly);
int install_cmake(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_libzip(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_mac_required_dependencies(Mac_PM mpm);
int install_mariadb(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_openssl(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_perl5(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_perl6(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_php(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_postfix(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_python(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_ruby(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_tcl(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_tk(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_wxwidgets(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int install_yum_required_dependencies(sstr& fileName, sstr& programName, bool createScriptOnly);
int logFinalSettings(sstr& fileNameBuilds, std::map<sstr, sstr>& settings, sstr& programName );
int make(an_itemValues& itemValues);
int make_clean(an_itemValues& itemValues);
int make_install(an_itemValues& itemValues);
int make_tests(an_itemValues& itemValues);
int mariadb_notes(an_itemValues& itemValues);
//
int process_section(std::map<sstr, sstr>& settings,
                    int (*pfunc)(std::map<sstr, sstr>& settings, an_itemValues& itemValues),
                    an_itemValues& itemValues );
//
int reportResults(an_itemValues& itemValues, int installResult);

int postInstall_Apache(an_itemValues& itemValues);
int postInstall_MariaDB(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int postInstall_PHP(std::map<sstr, sstr>& settings, an_itemValues& itemValues);
int removeDirectory(sstr& fileName, sstr& path, std::vector<sstr>& vec);
int setupInstallDirectories(an_itemValues& itemValues);
int startNewLogSection(std::ofstream& file, sstr utc = "-7");
int test_perl6(an_itemValues& itemValues);
int test_php(an_itemValues& itemValues);
int test_python(an_itemValues& itemValues);
int write_file_entry(std::ofstream& file, const sstr& entry, time_t stop, time_t start, bool includeTime = false);
int file_append_blank_lines(sstr& fileName, int count);
int file_append_line(sstr &fileName, sstr &line, time_t stop, time_t start);
int file_append_results(sstr& fileName, an_itemValues& itemValues, int installResult, time_t stop);
int file_write_vector_to_file(sstr &fileName, std::vector <sstr> &vec_lines, bool includeTime = true);
int removeWorkingDirectories(an_itemValues& itemValues);


std::vector<sstr> readFile(sstr &fileName, unsigned long maxCount);

bool check_Sha256sum(an_itemValues& itemValues);
bool directoryExists(const sstr& path);
bool getBoolFromString(sstr& some_value);
bool isFileSizeGtZero(an_itemValues itemValues, sstr &fileName, bool bShow = false);
bool prior_Results(sstr& fileNameResult, sstr& searchStr);
bool set_settings(std::map<sstr,sstr>& settings, an_itemValues& itemValues );
bool stageSourceCodeIfNeeded(an_itemValues& itemValues);

time_t get_Time();

template <typename T> sstr get_Time_Part(T timePart);

std::map<sstr, sstr> getProgramSettings(sstr& fileSettings);



#endif //J5C_INSTALL_FROM_SOURCE_001_FUNCTION_DECLARATIONS_H
