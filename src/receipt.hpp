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

#ifndef REAPACK_RECEIPT_HPP
#define REAPACK_RECEIPT_HPP

#include <set>
#include <string>
#include <vector>

#include "registry.hpp"

class Path;

struct InstallTicket {
  enum Type { Install, Upgrade };

  Type type;
  const Version *version;
  Registry::Entry regEntry;
};

class Receipt {
public:
  struct Error {
    std::string message;
    std::string title;
  };

  Receipt();

  bool isEnabled() const { return m_enabled; }
  void setEnabled(bool newVal) { m_enabled = newVal; }

  bool isRestartNeeded() const { return m_needRestart; }
  void setRestartNeeded(bool newVal) { m_needRestart = newVal; }

  void addTicket(const InstallTicket &ticket);
  const std::vector<InstallTicket> &installs() const { return m_installs; }
  const std::vector<InstallTicket> &updates() const { return m_updates; }

  void addRemoval(const Path &p) { m_removals.insert(p); }
  void addRemovals(const std::set<Path> &);
  const std::set<Path> &removals() const { return m_removals; }

  void addError(const Error &err) { m_errors.push_back(err); }
  const std::vector<Error> &errors() const { return m_errors; }
  bool hasErrors() const { return !m_errors.empty(); }

private:
  bool m_enabled;
  bool m_needRestart;

  std::vector<InstallTicket> m_installs;
  std::vector<InstallTicket> m_updates;
  std::set<Path> m_removals;
  std::vector<Error> m_errors;
};

#endif
