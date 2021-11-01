/*  This file is part of YUView - The YUV player with advanced analytics toolset
 *   <https://github.com/IENT/YUView>
 *   Copyright (C) 2015  Institut f�r Nachrichtentechnik, RWTH Aachen University, GERMANY
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   In addition, as a special exception, the copyright holders give
 *   permission to link the code of portions of this program with the
 *   OpenSSL library under certain conditions as described in each
 *   individual source file, and distribute linked combinations including
 *   the two.
 *
 *   You must obey the GNU General Public License in all respects for all
 *   of the code used other than OpenSSL. If you modify file(s) with this
 *   exception, you may extend this exception to your version of the
 *   file(s), but you are not obligated to do so. If you do not wish to do
 *   so, delete this exception statement from your version. If you delete
 *   this exception statement from all source files in the program, then
 *   also delete it here.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "superres_params.h"

#include "Typedef.h"
#include "sequence_header_obu.h"

namespace parser::av1
{

using namespace reader;

unsigned superres_params::parse(SubByteReaderLogging &               reader,
                                std::shared_ptr<sequence_header_obu> seqHeader,
                                unsigned                             UpscaledWidth)
{
  SubByteReaderLoggingSubLevel subLevel(reader, "superres_params()");

  if (seqHeader->enable_superres)
    this->use_superres = reader.readFlag("use_superres");
  else
    this->use_superres = false;

  if (this->use_superres)
  {
    this->coded_denom   = reader.readBits("coded_denom", SUPERRES_DENOM_BITS);
    this->SuperresDenom = coded_denom + SUPERRES_DENOM_MIN;
  }
  else
    this->SuperresDenom = SUPERRES_NUM;

  auto FrameWidth = (UpscaledWidth * SUPERRES_NUM + (SuperresDenom / 2)) / SuperresDenom;
  return FrameWidth;
}

} // namespace parser::av1