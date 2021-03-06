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

#ifndef REAPACK_PACKAGE_HPP
#define REAPACK_PACKAGE_HPP

#include "metadata.hpp"
#include "path.hpp"
#include "version.hpp"

class Category;

class Package;
typedef std::vector<const Package *> PackageList;

class Package {
public:
  enum Type {
    UnknownType,
    ScriptType,
    ExtensionType,
    EffectType,
    DataType,
    ThemeType,
  };

  static Type getType(const char *);
  static std::string displayType(Type);

  Package(const Type, const std::string &name, const Category * = nullptr);
  ~Package();

  const Category *category() const { return m_category; }
  Type type() const { return m_type; }
  std::string displayType() const { return displayType(m_type); }
  const std::string &name() const { return m_name; }
  std::string fullName() const;
  void setDescription(const std::string &d) { m_desc = d; }
  std::string description() const { return m_desc; }

  Metadata *metadata() { return &m_metadata; }
  const Metadata *metadata() const { return &m_metadata; }

  bool addVersion(const Version *ver);
  const VersionSet &versions() const { return m_versions; }
  const Version *version(size_t index) const;
  const Version *lastVersion(bool pres = true, const Version &from = {}) const;
  const Version *findVersion(const Version &) const;

private:
  const Category *m_category;

  Type m_type;
  std::string m_name;
  std::string m_desc;
  Metadata m_metadata;
  VersionSet m_versions;
};

#endif
