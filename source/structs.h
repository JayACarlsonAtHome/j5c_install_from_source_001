//
// Created by jay on 6/2/21.
//

#include "enums.h"

#ifndef J5C_INSTALL_FROM_SOURCE_001_STRUCTS_H
#define J5C_INSTALL_FROM_SOURCE_001_STRUCTS_H

struct an_itemValues
{
    bool bDebug;
    bool bDoTests;
    bool bInstall;
    bool bProtectMode;
    bool bScriptOnly;
    bool bCompileForDebug;
    bool bSkip;

    int     result     = -1;
    int     step       = -1;
    int     debugLevel =  0;

    OS_type thisOSType = OS_type ::RedHat;

    time_t itemStartTime;
    time_t itemStop_Time;

    sstr compression;
    sstr sha256sum_Config;
    sstr sha256sum_FileName;
    sstr sha256sum_Real;

    //Paths
    sstr bldPath;
    sstr etcPath;
    sstr getPath;
    sstr rtnPath;
    sstr srcPath;
    sstr srcPathPNV;
    sstr srcPathInstallOS;
    sstr cpyStgPath;
    sstr stgPath;
    sstr tlsPath;
    sstr tmpPath;
    sstr usrPath;
    sstr wwwPath;

    //fileNames
    sstr fileName_Compressed;
    sstr fileName_Staged;
    sstr fileName_Build;
    sstr fileName_Notes;
    sstr fileName_Results;
    sstr fileName_Protection;

    sstr company;
    sstr programName;
    sstr programNameVersion;
    sstr ProperName;
    sstr version;
    sstr pathVersion;

    sstr cmakeRunPath;
    sstr perl5RunPath;
};


#endif //J5C_INSTALL_FROM_SOURCE_001_STRUCTS_H
