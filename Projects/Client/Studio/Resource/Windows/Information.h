#pragma once

#define VERSION_MAJOR_MINOR_STR       "1.0"
#define VERSION_MAJOR_MINOR_PATCH_STR "1.0.0"
#define VERSION_FULL_STR              "1.0.0.0"
#define VERSION_RESOURCE              1, 0, 0, 0
#define VERSION_RESOURCE_STR          VERSION_FULL_STR "\0"

/*
 * These properties are part of VarFileInfo.
 * For more information, please see: https://learn.microsoft.com/en-us/windows/win32/menurc/varfileinfo-block
 */
#define PRODUCT_LANGUAGE             0x0409 // en-US
#define PRODUCT_CHARSET              1200  // Unicode

#define APP_ICON                     "Icon.ico"

#define APP_NAME                     "RNR Studio\0"
#define APP_DESCRIPTION              "RNR's Not Roblox\0"
#define APP_ORGANIZATION             "Legacy Roblox Reverse Engineers\0"
#define APP_COPYRIGHT                "This program is licensed under the GNU General Public License v3.0.\0"