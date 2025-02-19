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

#include "include.h"
#include "GUIColorManager.h"
#include "utils/URIUtils.h"
#include "FileSystem/SpecialProtocol.h"
#include "SkinInfo.h"
#include "settings/GUIWindowSettingsCategory.h"
#include "FileSystem/File.h"

using namespace XFILE;

CGUIColorManager g_colorManager;

CGUIColorManager::CGUIColorManager(void)
{
}

CGUIColorManager::~CGUIColorManager(void)
{
  Clear();
}

void CGUIColorManager::Clear()
{
  m_colors.clear();
}

// load the color file in
void CGUIColorManager::Load(const CStdString &colorFile)
{
  Clear();

  // load the global color map if it exists
  TiXmlDocument xmlDoc;
  CStdString ThemeName(URIUtils::ReplaceExtension(g_guiSettings.GetString("lookandfeel.skintheme"), ""));
	if (CFile::Exists("special://xbmc/.emustation/themes/" + ThemeName + "/colors.xml"))
		{
			if (xmlDoc.LoadFile(PTH_IC("special://xbmc/.emustation/themes/" + ThemeName + "/colors.xml")))
				LoadXML(xmlDoc);
		}
		else
		{
			if (xmlDoc.LoadFile(PTH_IC("special://xbmc/.emustation/themes/SIMPLE/colors.xml")))
				LoadXML(xmlDoc);
		}


  // first load the default color map if it exists
  CStdString path, basePath;
  URIUtils::AddFileToFolder(g_SkinInfo.GetBaseDir(), "colors", basePath);
  URIUtils::AddFileToFolder(basePath, "defaults.xml", path);

  if (xmlDoc.LoadFile(path))
    LoadXML(xmlDoc);

  // now the color map requested
  if (colorFile.CompareNoCase("SIMPLE") == 0)
    return; // nothing to do

  URIUtils::AddFileToFolder(basePath, colorFile, path);
  CLog::Log(LOGINFO, "Loading colors from %s", path.c_str());

  if (xmlDoc.LoadFile(path))
    LoadXML(xmlDoc);
}

bool CGUIColorManager::LoadXML(TiXmlDocument &xmlDoc)
{
  TiXmlElement* pRootElement = xmlDoc.RootElement();

  CStdString strValue = pRootElement->Value();
  if (strValue != CStdString("colors"))
  {
    CLog::Log(LOGERROR, "color file doesnt start with <colors>");
    return false;
  }

  const TiXmlElement *color = pRootElement->FirstChildElement("color");

  while (color)
  {
    if (color->FirstChild() && color->Attribute("name"))
    {
      color_t value = 0xffffffff;
      sscanf(color->FirstChild()->Value(), "%x", (unsigned int*) &value);
      CStdString name = color->Attribute("name");
      iColor it = m_colors.find(name);
      if (it != m_colors.end())
        (*it).second = value;
      else
        m_colors.insert(make_pair(name, value));
    }
    color = color->NextSiblingElement("color");
  }
  return true;
}

// lookup a color and return it's hex value
color_t CGUIColorManager::GetColor(const CStdString &color) const
{
  // look in our color map
  CStdString trimmed(color);
  trimmed.TrimLeft("= ");
  icColor it = m_colors.find(trimmed);
  if (it != m_colors.end())
    return (*it).second;

  // try converting hex directly
  color_t value = 0;
  sscanf(trimmed.c_str(), "%x", &value);
  return value;
}

