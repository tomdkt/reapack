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

#ifndef REAPACK_OSTREAM_HPP
#define REAPACK_OSTREAM_HPP

#include <sstream>

class Version;

class OutputStream {
public:
  OutputStream();

  std::ostringstream::pos_type tellp() { return m_stream.tellp(); }
  std::string str() const { return m_stream.str(); }

  void indented(const std::string &);

  template<typename T>
  OutputStream &operator<<(T t) { m_stream << t; return *this; }
  OutputStream &operator<<(const Version &);

private:
  std::ostringstream m_stream;
};

#endif
