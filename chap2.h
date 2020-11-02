#ifndef _CHAP2_H_
#define _CHAP2_H_
#include <vector>

// POINT DATASTRUCTURE ---> 

struct POINT {
  static std::vector<POINT> points;
  size_t index;
  double x, y;
  POINT (double _x, double _y) : x(_x),y(_y){
    index = points.size();
    points.push_back(*this);
  }

  bool operator < (const POINT & a){
    if (y > a.y)return true;
    if (y < a.y)return false;
    if (x > a.x)return true;
    return false;
  }
};

// EVENT DATASTRUCTURE ---> 

struct EVENT {
  size_t p;
  short type; // LOWER=1, UPPER=2, INTERSECTION=3
  EVENT (size_t _p, short _t, size_t _l, size_t _l2 = 0) : p(_p),type(_t)
  {
    lines[0] = _l;
    lines[1] = _l2;
  }
  bool operator < (const EVENT &a) const {    
    return POINT::points[p] < POINT::points[a.p];
  }
  size_t lines[2];
};

// LINE DATASTRUCTURE ---> 

struct LINE {
  static std::vector<LINE> lines;
  size_t index, p1, p2;  
  LINE (size_t _p1, size_t _p2)
  : p1(_p1),p2(_p2){
    index = lines.size();
    lines.push_back(*this);
  }
  std::vector<size_t> intersections;
  std::vector<size_t> otherline;
};

#endif

