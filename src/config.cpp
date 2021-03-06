/* ReaPack: Package manager for REAPER
 * Copyright (C) 2015-2016  Christian Fillion
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.hpp"

#include "path.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <swell/swell.h>
#endif

using namespace std;

static const auto_char *GENERAL_GRP = AUTO_STR("general");
static const auto_char *VERSION_KEY = AUTO_STR("version");

static const auto_char *INSTALL_GRP = AUTO_STR("install");
static const auto_char *AUTOINSTALL_KEY = AUTO_STR("autoinstall");
static const auto_char *PRERELEASES_KEY = AUTO_STR("prereleases");

static const auto_char *BROWSER_GRP = AUTO_STR("browser");
static const auto_char *SHOWDESCS_KEY = AUTO_STR("showdescs");
static const auto_char *STATE_KEY = AUTO_STR("state");

static const auto_char *NETWORK_GRP = AUTO_STR("network");
static const auto_char *PROXY_KEY = AUTO_STR("proxy");
static const auto_char *VERIFYPEER_KEY = AUTO_STR("verifyPeer");

static const auto_char *SIZE_KEY = AUTO_STR("size");

static const auto_char *REMOTES_GRP = AUTO_STR("remotes");
static const auto_char *REMOTE_KEY  = AUTO_STR("remote");

static auto_string ArrayKey(const auto_string &key, const unsigned int i)
{
  return key + to_autostring(i);
}

static const int BUFFER_SIZE = 2083;

Config::Config()
  : m_isFirstRun(false), m_version(0), m_remotesIniSize(0)
{
  resetOptions();
}

void Config::resetOptions()
{
  m_install = {false, false};
  m_browser = {true, ""};
  m_network = {"", true};
}

void Config::restoreSelfRemote()
{
  const string name = "ReaPack";
  const string url = "https://github.com/cfillion/reapack/raw/master/index.xml";

  Remote remote = m_remotes.get(name);
  remote.setName(name);
  remote.setUrl(url);
  remote.protect();

  m_remotes.add(remote);
}

void Config::restoreDefaultRemotes()
{
  const vector<pair<string,string> > repos = {
    {"ReaTeam Scripts",
      "https://github.com/ReaTeam/ReaScripts/raw/master/index.xml"},
    {"ReaTeam JSFX",
      "https://github.com/ReaTeam/JSFX/raw/master/index.xml"},
    {"MPL Scripts",
      "https://github.com/MichaelPilyavskiy/ReaScripts/raw/master/index.xml"},
    {"X-Raym Scripts",
      "https://github.com/X-Raym/REAPER-ReaScripts/raw/master/index.xml"},
  };

  for(const auto &pair : repos) {
    Remote remote = m_remotes.get(pair.first);

    if(!remote)
      remote.setEnabled(false); // disable by default

    remote.setName(pair.first);
    remote.setUrl(pair.second);
    m_remotes.add(remote);
  }
}

void Config::migrate()
{
  const unsigned int version = getUInt(GENERAL_GRP, VERSION_KEY);

  switch(version) {
  case 0:
    m_isFirstRun = true;
    restoreDefaultRemotes();
    break;
  default:
    // configuration is up-to-date, don't write anything now
    // keep the version intact in case it comes from a newer version of ReaPack
    m_version = version;
    return;
  };

  m_version = 1;
  write();
}

void Config::read(const Path &path)
{
  m_path = make_autostring(path.join());

  m_install.autoInstall = getUInt(INSTALL_GRP,
    AUTOINSTALL_KEY, m_install.autoInstall) > 0;
  m_install.bleedingEdge = getUInt(INSTALL_GRP,
    PRERELEASES_KEY, m_install.bleedingEdge) > 0;

  m_browser.showDescs = getUInt(BROWSER_GRP,
    SHOWDESCS_KEY, m_browser.showDescs) > 0;
  m_browser.state = getString(BROWSER_GRP, STATE_KEY, m_browser.state);

  m_network.proxy = getString(NETWORK_GRP, PROXY_KEY, m_network.proxy);
  m_network.verifyPeer = getUInt(NETWORK_GRP,
    VERIFYPEER_KEY, m_network.verifyPeer) > 0;

  readRemotes();
  restoreSelfRemote();
  migrate();
}

void Config::write()
{
  setUInt(GENERAL_GRP, VERSION_KEY, m_version);

  setUInt(INSTALL_GRP, AUTOINSTALL_KEY, m_install.autoInstall);
  setUInt(INSTALL_GRP, PRERELEASES_KEY, m_install.bleedingEdge);

  setUInt(BROWSER_GRP, SHOWDESCS_KEY, m_browser.showDescs);
  setString(BROWSER_GRP, STATE_KEY, m_browser.state);

  setString(NETWORK_GRP, PROXY_KEY, m_network.proxy);
  setUInt(NETWORK_GRP, VERIFYPEER_KEY, m_network.verifyPeer);

  writeRemotes();
}

void Config::readRemotes()
{
  m_remotesIniSize = getUInt(REMOTES_GRP, SIZE_KEY);

  for(unsigned int i = 0; i < m_remotesIniSize; i++) {
    const string data = getString(REMOTES_GRP, ArrayKey(REMOTE_KEY, i));

    m_remotes.add(Remote::fromString(data));
  }
}

void Config::writeRemotes()
{
  unsigned int i = 0;
  m_remotesIniSize = max((unsigned int)m_remotes.size(), m_remotesIniSize);

  for(auto it = m_remotes.begin(); it != m_remotes.end(); it++, i++) {
    setString(REMOTES_GRP, ArrayKey(REMOTE_KEY, i), it->toString());
  }

  cleanupArray(REMOTES_GRP, REMOTE_KEY, i, m_remotesIniSize);

  setUInt(REMOTES_GRP, SIZE_KEY, m_remotesIniSize = i);
}

string Config::getString(const auto_char *group,
  const auto_string &key, const string &fallback) const
{
  auto_char buffer[BUFFER_SIZE];
  GetPrivateProfileString(group, key.c_str(), make_autostring(fallback).c_str(),
    buffer, sizeof(buffer), m_path.c_str());

  return from_autostring(buffer);
}

void Config::setString(const auto_char *group,
  const auto_string &key, const string &val) const
{
  WritePrivateProfileString(group, key.c_str(),
    make_autostring(val).c_str(), m_path.c_str());
}

unsigned int Config::getUInt(const auto_char *group,
  const auto_string &key, const unsigned int fallback) const
{
  return GetPrivateProfileInt(group, key.c_str(), fallback, m_path.c_str());
}

void Config::setUInt(const auto_char *group, const auto_string &key,
  const unsigned int val) const
{
  setString(group, key, to_string(val));
}

void Config::deleteKey(const auto_char *group, const auto_string &key) const
{
  WritePrivateProfileString(group, key.c_str(), 0, m_path.c_str());
}

void Config::cleanupArray(const auto_char *group, const auto_string &key,
  const unsigned int begin, const unsigned int end) const
{
  for(unsigned int i = begin; i < end; i++)
    deleteKey(group, ArrayKey(key, i));
}
