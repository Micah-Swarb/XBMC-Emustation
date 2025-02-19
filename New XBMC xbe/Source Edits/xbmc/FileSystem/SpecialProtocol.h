#pragma once
/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <map>
#include "utils/StdString.h"

// static class for path translation from our special:// URLs.

/* paths are as follows:

 special://xbmc/          - the main XBMC folder (i.e. where the app resides).
 special://home/          - a writeable version of the main XBMC folder
                             Linux: ~/.xbmc/
                             OS X:  ~/Library/Application Support/XBMC/
                             Win32: FIXME: Currently the same as special://home/
 special://userhome/      - a writable version of the user home directory
                             Linux, OS X: ~/.xbmc
                             Win32: home directory of user                             
 special://masterprofile/ - the master users userdata folder - usually special://home/userdata
                             Linux: ~/.xbmc/userdata/
                             OS X:  ~/Library/Application Support/XBMC/UserData/
                             Win32: ~/Application Data/XBMC/UserData/
 special://profile/       - the current users userdata folder - usually special://masterprofile/profiles/<current_profile>
                             Linux: ~/.xbmc/userdata/profiles/<current_profile>
                             OS X:  ~/Library/Application Support/XBMC/UserData/profiles/<current_profile>
                             Win32: ~/Application Data/XBMC/UserData/profiles/<current_profile>

 special://temp/          - the temporary directory.
                             Linux: ~/tmp/xbmc<username>
                             OS X:  ~/
                             Win32: ~/Application Data/XBMC/cache
*/
class CURL;
class CSpecialProtocol
{
public:
  static void SetProfilePath(const CStdString &path);
  static void SetXBMCPath(const CStdString &path);
  static void SetHomePath(const CStdString &path);
  static void SetRootPath(const CStdString &path);
  static void SetThemePath(const CStdString &path);
  static void SetCurrentThemePath(const CStdString &path);
  static void SetURLDownloaderPath(const CStdString &path);
  static void SetEmustationScriptsPath(const CStdString &path);
  static void SetUserHomePath(const CStdString &path);
  static void SetMasterProfilePath(const CStdString &path);
  static void SetTempPath(const CStdString &path);

  static bool ComparePath(const CStdString &path1, const CStdString &path2);
  static bool XBMCIsHome();
  static void LogPaths();

  static CStdString TranslatePath(const CStdString &path);
  static CStdString TranslatePath(const CURL &url);
  static CStdString TranslatePathConvertCase(const CStdString& path);
  static CStdString ReplaceOldPath(const CStdString &oldPath, int pathVersion);
  static const int path_version = 1;

private:
  static void SetPath(const CStdString &key, const CStdString &path);
  static CStdString GetPath(const CStdString &key);

  static std::map<CStdString, CStdString> m_pathMap;
};

#define _P(x)     CSpecialProtocol::TranslatePath(x)
#define PTH_IC(x) CSpecialProtocol::TranslatePathConvertCase(x)

#ifdef _WIN32
#define PATH_SEPARATOR_CHAR '\\'
#define PATH_SEPARATOR_STRING "\\"
#else
#define PATH_SEPARATOR_CHAR '/'
#define PATH_SEPARATOR_STRING "/"
#endif
