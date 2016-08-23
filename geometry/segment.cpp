/****************************************************************************
* Italian National Research Council                                         *
* Institute for Applied Mathematics and Information Technologies, Genoa     *
* IMATI-GE / CNR                                                            *
*                                                                           *
* Author: Marco Livesu (marco.livesu@gmail.com)                             *
*                                                                           *
* Copyright(C) 2016                                                         *
* All rights reserved.                                                      *
*                                                                           *
* This file is part of CinoLib                                              *
*                                                                           *
* CinoLib is free software; you can redistribute it and/or modify           *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 3 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
****************************************************************************/
#include <cinolib/geometry/segment.h>

namespace cinolib
{

CINO_INLINE
std::ostream & operator<<(std::ostream & in, const Segment & s)
{
    in << s.first << "\t" << s.second << "\n";
    return in;
}


CINO_INLINE
Segment::Segment(const vec3d & P0, const vec3d & P1)
{
    first  = P0;
    second = P1;
}


CINO_INLINE
double Segment::operator[](const vec3d & p) const
{
    return dist_to_point(p);
}


CINO_INLINE
double Segment::dist_to_point(const vec3d & p) const
{
    vec3d v = second - first;
    vec3d w = p  - first;

    float cos_wv = w.dot(v);
    float cos_vv = v.dot(v);

    if (cos_wv <= 0.0)    return first.dist(p);
    if (cos_vv <= cos_wv) return second.dist(p);

    float b  = cos_wv / cos_vv;
    vec3d Pb = first + v*b;
    return (p-Pb).length();
}

CINO_INLINE
bool Segment::is_in_between(const vec3d &p) const
{
    vec3d v = second - first;
    vec3d w = p  - first;

    float cos_wv = w.dot(v);
    float cos_vv = v.dot(v);

    if (cos_wv <= 0.0)    return false;
    if (cos_vv <= cos_wv) return false;
    return true;
}

CINO_INLINE
std::vector<Plane> Segment::to_planes() const
{
    vec3d d = dir();

    vec3d n0(-d.y(),  d.x(),     0);
    vec3d n1(-d.z(),      0, d.x());
    vec3d n2(     0, -d.z(), d.y());

    std::vector<Plane> planes;
    if (n0.length() > 0) planes.push_back(Plane(first, n0));
    if (n1.length() > 0) planes.push_back(Plane(first, n1));
    if (n2.length() > 0) if (planes.size() < 2) planes.push_back(Plane(first, n2));
    assert(planes.size() == 2);

    return planes;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
vec3d Segment::dir() const
{
    vec3d d = first-second;
    d.normalize();
    return d;
}

}
