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

#ifndef REAPACK_TASK_HPP
#define REAPACK_TASK_HPP

#include "path.hpp"
#include "registry.hpp"

#include <boost/signals2.hpp>
#include <set>
#include <vector>

class Download;
class Source;
class Transaction;
class Version;

class Task {
public:
  typedef boost::signals2::signal<void ()> Signal;
  typedef Signal::slot_type Callback;

  Task(Transaction *parent);
  virtual ~Task() {}

  void onCommit(const Callback &callback) { m_onCommit.connect(callback); }
  bool isCancelled() const { return m_isCancelled; }

  void start();
  void commit();
  void rollback();

protected:
  Transaction *transaction() const { return m_transaction; }

  virtual void doStart() = 0;
  virtual bool doCommit() = 0;
  virtual void doRollback() = 0;

private:
  Transaction *m_transaction;
  bool m_isCancelled;

  Signal m_onCommit;
};

class InstallTask : public Task {
public:
  InstallTask(const Version *ver, const std::vector<Registry::File> &oldFiles,
    Transaction *);

  const std::vector<Registry::File> &removedFiles() const { return m_oldFiles; }

protected:
  void doStart() override;
  bool doCommit() override;
  void doRollback() override;

private:
  struct PathGroup { Path target; Path temp; };

  void saveSource(Download *, const Source *);

  const Version *m_version;
  std::vector<Registry::File> m_oldFiles;
  std::vector<PathGroup> m_newFiles;
};

class RemoveTask : public Task {
public:
  RemoveTask(const std::vector<Path> &files, Transaction *);

  const std::set<Path> &removedFiles() const { return m_removedFiles; }

protected:
  void doStart() override {}
  bool doCommit() override;
  void doRollback() override {}

private:
  std::vector<Path> m_files;
  std::set<Path> m_removedFiles;
};

class DummyTask : public Task {
public:
  DummyTask(Transaction *t) : Task(t) {}

protected:
  void doStart() override {}
  bool doCommit() override { return true; }
  void doRollback() override {}
};

#endif
