/** @file geom356.h 
 *  @brief Geometric types and functions.
 *
 *  @author N. Danner
 */

#ifndef GEOM356H
#define GEOM356H

/** The type representing a three-dimensional point.  The point is defined
 *  in reference to some frame.  In particular, the <code>x</code>, 
 *  <code>y</code>, and <code>z</code> fields identify the directions
 *  along the basis that define the frame to name a vector <i>v</i>; 
 *  the point is located at <i>O + v</i>, where <i>O</i> is the origin
 *  of the frame.
 */
typedef struct _point3_t {
    /** Direction along the first basis vector of the frame.
     */
    float   x ;
    /** Direction along the second basis vector of the frame.
     */
    float   y ;
    /** Direction along the third basis vector of the frame.
     */
    float   z ;
} point3_t ;

/** The type representing a vector in three-dimensional space.  The
 *  vector is defined in reference to some frame with basis
 *  arbitrarily named <i>x</i>, <i>y</i>, <i>z</i>.
 */
typedef struct {
    /** The displacement in the direction of <i>x</i>.
     */
    float   x ;
    /** The displacement in the direction of <i>y</i>.
     */
    float   y ;
    /** The displacement in the direction of <i>z</i>.
     */
    float   z ;
} vector3_t ;

/** The type representing a ray in three-dimensional space.  A ray
 *   (actually a line) is defined by a point <i>p</i> and a vector
 *   <i>v</i>.  Such a ray consists of all points of the form
 *   <i>p + tv</i> for all real numbers <i>t</i>; we usually just
 *   work with <i>t >= 0</i>, hence call this a ray instead of a line.
 */
typedef struct {
    /** The base of the vector.
     */
    point3_t   base ;
    /** The direction of the vector.
     */
    vector3_t  dir ;
} ray3_t ;

/** Add a point and a vector.
 *  
 *  @param p any point.
 *  @param u any vector.
 *  @param result a <code>point3_t</code> that will be set to the
 *      value of <code>p + u</code> (point-vector addition).
 */
void pv_add(point3_t* p, vector3_t* u, point3_t* result) ;

/** Subtract one point from another to get a vector.
 *  
 *  @param b one point.
 *  @param a another point.
 *  @param result a vector that will be assigned the value b-a (i.e.,
 *      the vector that points from a to b and whose length is the
 *      distance between a and b).
 */
void pv_subtract(point3_t* b, point3_t* a, vector3_t* result) ;

/** Add two vectors.
 *  
 *  @param u a vector.
 *  @param v a vector.
 *  @param result a vector that will be assigned the value of u+v.
 */
void add(vector3_t* u, vector3_t* v, vector3_t* result) ;

/** Subtract two vectors.
 *  
 *  @param u a vector.
 *  @param v a vector.
 *  @param result a vector that will be assigned the value of u-v.
 */
void subtract(vector3_t* u, vector3_t* v, vector3_t* result) ;

/** Multiply a vector by a given scalar.
 *  
 *  @param v any vector.
 *  @param a any scalar.
 *  @param result the vector in which to store the value of a*v.
 *      It is safe to have result == v.
 */
void multiply(vector3_t* v, float a, vector3_t* result) ;

/** Divide a vector by a given scalar.
 *  
 *  @param v any vector.
 *  @param a any non-zero scalar.
 *  @param result the vector in which to store the value of (1/a)*v.
 *      It is safe to have result == v.
 */
void divide(vector3_t* v, float a, vector3_t* result) ;

/** Get the length of a vector.
 *  
 *  @param v any vector.
 *
 *  @returns the length of v.
 */
float length(vector3_t* v) ;

/** Get the square of the length of a vector.  This method is more efficient
 *  than computing sqrt(length(v)).
 *  
 *  @param v any vector.
 *
 *  @returns the square of the length of v.
 */
float length2(vector3_t* v) ;

/** Compute the dot product of two vectors.
 *
 *  @param v one vector.
 *  @param w another vector.
 *
 *  @returns The dot-product of v and w, computed as (v->x*w->x) +
 *      (v->y*w->y) + (v->z*w->z).
 */
float dot(vector3_t* v, vector3_t* w) ;

/** Compute the cross product of two vectors.
 *  
 *  @param a one vector.
 *  @param b another vector.
 *  @param result a vector that will be filled with the value of a*b.
 *      (a, b, result) will form a right-handed system.
 */
void cross(vector3_t* a, vector3_t* b, vector3_t* result) ;

/** Normalize a vector.
 *  
 *  @param v a vector.  The coordinates of this vector will be changed
 *      so that v still points in the same direction, but v will have
 *      unit length.
 */
void normalize(vector3_t* v) ;

/** Get the square of the distance between two points.
 *  
 *  @param p any point.
 *  @param q any point
 *  @return the square of the distance between <code>p</code> and 
 *      <code>q</code>.
 */
float dist2(point3_t* p, point3_t* q) ;

/** Get the distance between two points.
 *  
 *  @param p any point.
 *  @param q any point
 *  @return the distance between <code>p</code> and 
 *      <code>q</code>.
 */
float dist(point3_t* p, point3_t* q) ;

#endif

