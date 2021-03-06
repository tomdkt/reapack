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

#ifndef REAPACK_METADATA_HPP
#define REAPACK_METADATA_HPP

#include <map>
#include <string>
#include <vector>

struct Link { std::string name; std::string url; };

class Metadata {
public:
  enum LinkType {
    WebsiteLink,
    ScreenshotLink,
    DonationLink,
  };

  typedef std::multimap<LinkType, Link> LinkMap;

  static LinkType getLinkType(const char *rel);

  void setAbout(const std::string &rtf) { m_about = rtf; }
  const std::string &about() const { return m_about; }
  void addLink(const LinkType, const Link &);
  const LinkMap &links() const { return m_links; }

private:
  std::string m_about;
  LinkMap m_links;
};

#endif
