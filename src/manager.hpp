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

#ifndef REAPACK_MANAGER_HPP
#define REAPACK_MANAGER_HPP

#include "dialog.hpp"

#include "listview.hpp"

#include <boost/optional.hpp>
#include <map>
#include <set>

class Config;
class ReaPack;
class Remote;
struct NetworkOpts;

class Manager : public Dialog {
public:
  Manager(ReaPack *);

  void refresh();
  bool import();

protected:
  void onInit() override;
  void onCommand(int, int) override;
  bool onKeyDown(int, int) override;

private:
  ListView::Row makeRow(const Remote &) const;

  Remote getRemote(int index) const;
  bool fillContextMenu(Menu &, int index) const;
  void setRemoteEnabled(bool);
  bool isRemoteEnabled(const Remote &) const;
  void uninstall();
  void toggle(boost::optional<bool> &, bool current);
  void refreshIndex();
  void about(int index);
  void copyUrl();
  void launchBrowser();
  void options();
  void setupNetwork();

  void setChange(int);
  bool confirm() const;
  bool apply();
  void reset();

  HWND m_apply;
  ReaPack *m_reapack;
  Config *m_config;
  ListView *m_list;

  size_t m_changes;
  bool m_importing;

  std::map<Remote, bool> m_enableOverrides;
  std::set<Remote> m_uninstall;
  boost::optional<bool> m_autoInstall;
  boost::optional<bool> m_bleedingEdge;
};

class NetworkConfig : public Dialog {
public:
  NetworkConfig(NetworkOpts *);

protected:
  void onInit() override;
  void onCommand(int, int) override;

private:
  void apply();

  NetworkOpts *m_opts;
  HWND m_proxy;
  HWND m_verifyPeer;
};

#endif
