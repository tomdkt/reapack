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

#include "metadata.hpp"

#include <boost/algorithm/string/predicate.hpp>

auto Metadata::getLinkType(const char *rel) -> LinkType
{
  if(!strcmp(rel, "donation"))
    return DonationLink;

  return WebsiteLink;
}

void Metadata::addLink(const LinkType type, const Link &link)
{
  if(boost::algorithm::starts_with(link.url, "http"))
    m_links.insert({type, link});
}

auto Metadata::links(const LinkType type) const -> LinkList
{
  const auto begin = m_links.lower_bound(type);
  const auto end = m_links.upper_bound(type);

  LinkList list(m_links.count(type));

  for(auto it = begin; it != end; it++)
    list[distance(begin, it)] = &it->second;

  return list;
}
