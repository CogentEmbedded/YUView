/*  This file is part of YUView - The YUV player with advanced analytics toolset
 *   <https://github.com/IENT/YUView>
 *   Copyright (C) 2015  Institut für Nachrichtentechnik, RWTH Aachen University, GERMANY
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

#pragma once

#include <common/Typedef.h>
#include <common/YUViewDomElement.h>

#include "ColorMapper.h"

namespace stats
{

enum class Pattern
{
  Solid,
  Dash,
  Dot,
  DashDot,
  DashDotDot
};

const std::vector<Pattern> AllPatterns = {
    Pattern::Solid, Pattern::Dash, Pattern::Dot, Pattern::DashDot, Pattern::DashDotDot};

struct LineDrawStyle
{
  Color   color{};
  double  width{0.25};
  Pattern pattern{Pattern::Solid};

  bool operator!=(const LineDrawStyle &other) const
  {
    return color != other.color || width != other.width || pattern != other.pattern;
  }
};

/* This class defines a type of statistic to render. Each statistics type entry defines the name and
 * and ID of a statistic. It also defines what type of data should be drawn for this type and how it
 * should be drawn.
 */
class StatisticsType
{
public:
  StatisticsType(int typeID = INT_INVALID, const QString &typeName = "?");
  StatisticsType(int typeID, const QString &typeName, int vectorScaling);
  StatisticsType(int                       typeID,
                 const QString &           typeName,
                 const color::ColorMapper &colorMapper,
                 bool                      hasAndRenderVectorData = false);

  // Save all the values that the user could change. When saving to playlist we can save only the
  // changed values to playlist.
  void setInitialState();

  // Load/Save status of statistics from playlist file
  void savePlaylist(YUViewDomElement &root) const;
  void loadPlaylist(const YUViewDomElement &root);

  // Every statistics type has an ID, a name and possibly a description
  int     typeID{};
  QString typeName{};
  QString description{};

  // Get the value text (from the value map (if there is an entry))
  QString getValueTxt(int val) const;

  void    setMappingValues(std::vector<QString> values);
  QString getMappedValue(int typeID) const;

  // Is this statistics type rendered and what is the alpha value?
  // These are corresponding to the controls in the properties panel
  bool render{};
  int  alphaFactor{50};

  // Value data (a certain value, that is set for a block)
  bool               hasValueData{};          // Does this type have value data?
  bool               renderValueData{};       // Do we render the value data?
  bool               scaleValueToBlockSize{}; // Scale the values according to the size of the block
  color::ColorMapper colorMapper;             // How do we map values to color?

  // Vector data (a vector that is set for a block)
  bool hasVectorData{}; // Does this type have any vector data?
  bool hasAffineTFData{};
  bool renderVectorData{};       // Do we draw the vector data?
  bool renderVectorDataValues{}; // Do we draw the values of the vector next to the vector (by
                                 // default true).
  bool          scaleVectorToZoom{};
  LineDrawStyle vectorStyle; // How do we draw the vectors
  int vectorScale{1}; // Every vector value (x,y) has to be divided by this value before displaying
                      // it (e.g. 1/4 th pixel accuracy)
  bool mapVectorToColor{}; // Color the vectors depending on their direction
  enum class ArrowHead
  {
    arrow,
    circle,
    none
  };
  ArrowHead arrowHead{
      ArrowHead::arrow}; // Do we draw an arrow, a circle or nothing at the end of the arrow?

  // Do we (and if yes how) draw a grid around each block (vector or value)
  bool          renderGrid{true};
  LineDrawStyle gridStyle;
  bool          scaleGridToZoom{};

  // is statistic drawn as a block or as a polygon?
  bool isPolygon{};

private:
  // If set, this map is used to map values to text
  std::map<int, QString> valMap;

  // Backup values for setDefaultState()
  struct initialState
  {
    bool render;
    int  alphaFactor;

    bool               renderValueData;
    bool               scaleValueToBlockSize;
    color::ColorMapper colorMapper;

    bool          renderVectorData;
    bool          scaleVectorToZoom;
    LineDrawStyle vectorStyle;
    int           vectorScale;
    bool          mapVectorToColor;
    ArrowHead     arrowHead;

    bool          renderGrid;
    LineDrawStyle gridStyle;
    bool          scaleGridToZoom;
  };
  initialState init;
};

} // namespace stats
