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
#include "remote.hpp"

class Menu;
class ReaPack;

class Manager : public Dialog {
public:
  Manager(ReaPack *);
  ~Manager();

  void refresh();

protected:
  void onInit() override;
  void onCommand(WPARAM, LPARAM) override;
  void onNotify(LPNMHDR, LPARAM) override;
  void onContextMenu(HWND, int x, int y) override;

private:
  static ListView::Row makeRow(const Remote &remote);

  enum Action {
    Enable = 300,
    Disable = 301,
    Uninstall = 302,
  };

  void selectionChanged();
  void markFor(Action action);
  void apply();

  ReaPack *m_reapack;
  ListView *m_list;
  HWND m_uninstall;
};

#endif