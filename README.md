Numerical Geometry Project: Convex Hulls
========================================

This folder contains our project on convex hulls.
More precisely, four algorithms have been implemented:
 * Graham Scan
 * Jarvis March
 * Chan's Algorithm
 * Akl-Toussaint Heuristic.
 
We mainly focused on Chan's algorithm hence we provided animations for Graham Scan and Chan's algorithm but not for Jarvis march nor Akl-Toussaint heuristic (which is not very funny to animate).

To launch **the animations**, please enter 

`myexec set algo`

where `set` describes the set on which the animation is run and can take the values
 * `0` random set of points
 * `1` confused Jackie Chan
 * `2` happy Jackie Chan
 * `3` Christmas Tree 
 
and `algo` is the algorithm illustrated by the animation:
 * `0` Graham Scan
 * `1` Chan's algorithm
 
 For instance, `myexec 1 0` will display the Graham Scan on a confused Jackie.
 
 To show the **convex hull without animation**, please enter 
 
 `myexec set algo 0`
 
 where `set` and `algo` can be used as above, with a third possibility for `algo`: if set to `2`, the Jarvis March will be used to compute the convex hull.
 
To **benchmark** our three algorithms with the Akl-Toussaint heuristic, please enter

`myexec nPoints`

with `nPoints` being an integer value egals to the size of a random set that will be convexhullised by our algorithms. The result of the benchmark will be displayed on the console. Taking `nPoints <= 1e7` works perfectly fine on our computers.

If you simply wants to play with our **sandbox**, please enter

`myexec`

You can then add points by left clicking and remove points with right click. Please use a mouse and not a trackpad :-)


A more complete documentation for each algorithm is available in [doc/fullReadMe.md](doc/fullReadMe.md).

![](doc/chan.jpg)


<!--
It should contain:
 * this file (**README.md**)
 * the description of the structure of the program in **CMakeLists.txt**
 * a **src** directory containing the the source code of your program
 * a **doc** directory containing more documentation
 * a **deps** directory containing the BOV library

See [doc/COMPILING.md](doc/COMPILING.md) for a step by step tutorial
on how to build the program.

See [doc/tutorial.md](doc/tutorial.md) for a step by step tutorial on
how to use the BOV library.

See [deps/BOV/include/BOV.h](deps/BOV/include/BOV.h)
for help on the BOV library functions.

See [deps/BOV/examples/](deps/BOV/examples/) for more
examples using the BOV library

-->
