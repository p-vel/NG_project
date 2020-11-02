#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <list>
#include <string>
#include "chap2.h"

std::vector<POINT> POINT::points; // global vars
std::vector<LINE> LINE::lines; 

FILE *f;

size_t N_INTERS = 0;

double X_LINE (size_t l, double Y){

  LINE &L   = LINE::lines[l];
  POINT &P1 = POINT::points[L.p1];
  POINT &P2 = POINT::points[L.p2];

  double m = (P2.x-P1.x)/(P2.y-P1.y);
  return P2.x + m * (Y - P2.y);  
}

void CheckIntersection (double Y, size_t l1, size_t l2, std::set<EVENT> &Queue){

  LINE &L1   = LINE::lines[l1];
  POINT &p1 = POINT::points[L1.p1];
  POINT &p2 = POINT::points[L1.p2];

  if (l1 == l2) printf("ARGHGGGGH checking %lu %lu\n",l1,l2);

  for (size_t i=0;i<L1.intersections.size();i++){
    size_t other = L1.otherline[i];
    if (other == l2)return;    
  }
  
  LINE &L2   = LINE::lines[l2];
  POINT &q1 = POINT::points[L2.p1];
  POINT &q2 = POINT::points[L2.p2];

  // l1 --> x11 + u (x12-x11) = x
  // l2 --> x21 + v (x22-x21) = x 

  
  double A[2][2];
  A[0][0] = p2.x - p1.x;
  A[0][1] = q1.x - q2.x;
  A[1][0] = p2.y - p1.y;
  A[1][1] = q1.y - q2.y;
  double b[2] = {q1.x - p1.x, q1.y - p1.y};

  double det = A[0][0] * A[1][1] -  A[0][1] * A[1][0];

  if (fabs(det) < 1.e-10)printf("should use robust predicates (%22.15E)\n",det);
  
  double u = (b[0] * A[1][1] -  A[0][1] * b[1])/det;
  double v = (A[0][0] * b[1] -  b[0] * A[1][0])/det;

  
  if (u > 0 && v > 0 && u < 1 && v < 1){
    double x = p1.x + u *(p2.x-p1.x);
    double y = p1.y + u *(p2.y-p1.y);
    if (y <= Y){
      POINT intersection (x,y);
      L1.intersections.push_back(intersection.index);
      L2.intersections.push_back(intersection.index);
      L1.otherline.push_back(l2);
      L2.otherline.push_back(l1);
      EVENT e (intersection.index, 3, L1.index,L2.index);
      Queue.insert(e);
      N_INTERS++;
      //      printf("INTERS (%g,%g,0) = {%lu} between %lu %lu\n",x,y,intersection.index,l1,l2);
      fprintf(f,"SP(%g,%g,0){%lu};\n",x,y,intersection.index);
    }
  }
  
}

void AddToStatus (double Y, size_t l, std::list<size_t> &Status,
		  std::set<EVENT> &Queue){

  double myX = X_LINE (l, Y);

  if (Status.empty())Status.push_back(l);
  else{
    //    printf("----------_ADDING %lu ------------\n",l);
    for (std::list<size_t>::iterator it = Status.begin();
	 it != Status.end(); ++it){
      double X = X_LINE(*it,Y);
      if (X > myX) {	
	// check intersections before and after
	std::list<size_t>::iterator itNew = Status.insert (it,l);
	if (itNew != Status.begin()) {	  
	  itNew --;
	  CheckIntersection (Y, l,*itNew, Queue);
	  itNew ++;
	}
	if (++itNew != Status.end()) {
	  CheckIntersection (Y, l,*itNew, Queue);
	}
	return;
      }
    }
    Status.push_back (l);
    std::list<size_t>::iterator itNew = -- Status.end();
    CheckIntersection (Y, l,*(--itNew), Queue);    
  }
}

void printStatus (FILE *ff, std::list<size_t> &Status, double Y){
  fprintf(ff,"SL(%g,%g,0,%g,%g,0){-1,-1};\n",-.2,Y,1.2,Y);

  size_t counter=0;
  for (std::list<size_t>::iterator it = Status.begin();
       it != Status.end(); ++it){
    LINE &L = LINE::lines[*it];
    POINT &p1 = POINT::points[L.p1];
    POINT &p2 = POINT::points[L.p2];
    fprintf(ff,"SL(%g,%g,0,%g,%g,0){%lu,%lu};\n",
	    p1.x,p1.y,p2.x,p2.y,counter, counter);
    counter++;
  }
}


void SwapInStatus (double Y, size_t l1, size_t l2,
		   std::list<size_t> &Status,
		   std::set<EVENT> &Queue){

  for (std::list<size_t>::iterator it = Status.begin();
       it != Status.end(); ++it){
    if (*it == l1){
      std::list<size_t>::iterator it_l1 = it;      
      std::list<size_t>::iterator it_l2 = ++it;      
      if (*it_l2 != l2)printf("error\n");
      *it_l1 = l2;
      *it_l2 = l1;

      if (++it_l2 != Status.end())
	CheckIntersection (Y, l1,*it_l2, Queue);
      if (it_l1 != Status.begin())
	CheckIntersection (Y, *(--it_l1),l2, Queue);
      return;
    }
    if (*it == l2){
      std::list<size_t>::iterator it_l2 = it;      
      std::list<size_t>::iterator it_l1 = ++it;      


      if (*it_l1 != l1)printf("error\n");
      *it_l1 = l2;
      *it_l2 = l1;

      if (++it_l1 != Status.end())
	CheckIntersection (Y, l2,*it_l1, Queue);
      if (it_l2 != Status.begin())
	CheckIntersection (Y, *(--it_l2),l1, Queue);
      return;
    }
  }
}



int main (int argc, char *argv[]){  

  std::set<EVENT> Queue;


  f = fopen ("lines.pos","w");
  fprintf(f,"View \"Lines\"{\n");

  const int N = atoi(argv[1]);
  
  for (int i=0;i<N;i++){        // Create all the points and segments
    double dx1 = drand48 ()/20; // drand48: random uniform float between [0,1]
    double dx2 = drand48 ()/20;
    double dy1 = drand48 ()/20;
    double dy2 = drand48 ()/20;

    double x = drand48 ();
    double y = drand48 ();

    POINT p1 (x+dx1,y+dy1); // create points
    POINT p2 (x+dx2,y+dy2);
    LINE L (p1.index,p2.index); // create line
    fprintf(f,"SL(%g,%g,0,%g,%g,0){%lu,%lu};\n",
    	    x+dx1,y+dy1,x+dx2,y+dy2,L.index,L.index);

    if (p2 < p1) {
      Queue.insert (EVENT(p1.index, 1,L.index)); // p1 is the upper point
      Queue.insert (EVENT(p2.index, 2,L.index)); // p2 is the lower point
    }
    else{
      Queue.insert (EVENT(p1.index, 2,L.index)); // converse
      Queue.insert (EVENT(p2.index, 1,L.index));
    }
  }
  

  std::list<size_t> Status;

  int iter = 0;
  
  while (!Queue.empty()){


    std::set<EVENT>::iterator it = Queue.begin();
    const EVENT &e = *it; // 1st event in the queue at each iteration
    Queue.erase(it);

    // position of the sweep line
    double Y = POINT::points[e.p].y/*-1.e-12*/;  // the y-coordinate of the event (pts/intersection)

    //    printf("%lu %lu %lu %d (%lu %lu)\n",Status.size(),Queue.size(),
    //	   POINT::points.size(),e.type,e.lines[0],e.lines[1]);
    //    for (std::list<size_t>::iterator its = Status.begin();
    //    	 its != Status.end(); ++its){
    //      printf("%lu ",*its);
    //    }
    //    printf("\n");
    
    if (e.type == 1) { // lower point
      for (std::list<size_t>::iterator its = Status.begin();
	   its != Status.end(); ++its){
	if (*its == e.lines[0]){
	  std::list<size_t>::iterator it_after = its; ++ it_after;
	  if (it_after != Status.end()){
	    std::list<size_t>::iterator it_before = its;
	    if (it_before != Status.begin()){
	      CheckIntersection (Y, *(it_after),*(--it_before), Queue);
	    }
	  }
	  Status.erase(its);
	  break;
	}
      }
    }
    else if (e.type == 2) { // upper point
      AddToStatus (Y, e.lines[0], Status,Queue);
    }
    else if (e.type == 3) { // intersection 
      SwapInStatus (Y, e.lines[0], e.lines[1], Status,Queue);
    }
    /*
    double prev = 0;
    for (std::list<size_t>::iterator it = Status.begin();
	 it != Status.end(); ++it){
      double X = X_LINE(*it,Y);
      printf("%15.14g ",X);
      if (prev > X){
	std::string fn = "status"+std::to_string(iter++)+".pos";
	FILE *ff = fopen (fn.c_str(),"w");
	fprintf(ff,"View \"Status\"{\n");
	printStatus (ff, Status, Y);
	fprintf(ff,"};\n");
	fclose(ff);
	fprintf(f,"};\n");
	fclose(f);
	exit(1);
      }
	
      prev = X;
    }
    printf("\n");
    */

    /*
    std::string fn = "status"+std::to_string(iter++)+".pos";
    FILE *ff = fopen (fn.c_str(),"w");
    fprintf(ff,"View \"Status\"{\n");
    printStatus (ff, Status, Y);
    fprintf(ff,"};\n");
    fclose(ff);
    */

  }
  
  fprintf(f,"};\n");
  fclose(f);

  printf("%8lu intersections\n",N_INTERS);

}
