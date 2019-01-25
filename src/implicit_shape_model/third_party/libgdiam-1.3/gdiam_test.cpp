/*
 * Original copyright holder is Sariel Har-Peled (ssaarriieell@cs.uiuc.edu)
 * As copyright holder, he permitted me the use and redistribution of this code under the new BSD license.
 *
 * The original code is available at:
 *
 *              https://sarielhp.org/research/papers/00/diameter/diam_prog.html
 *
 *
 *
 * This file is modified by Viktor Seib, 2018 and the following license text was added:
 * (the original license does not apply anymore, but is kept farther down in the file for completeness)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */




// NOTE: the following original license does not apply anymore, but is kept here for completeness


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
 * gdiam_test.cpp -
 *     Test code for computing diameter and tight-fitting
 * bounding box.
 *
 * Copyright 2000 Sariel Har-Peled (ssaarriieell@cs.uiuc.edu)
 *
 * * the GNU General Public License as published by the Free
 *   Software Foundation; either version 2, or (at your option)
 *   any later version.
 *
 * or
 *
 * * the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2.1, or (at your option)
 *   any later version.
 *
 * Code is based on the paper:
 *   A Practical Approach for Computing the Diameter of a Point-Set.
 *   Sariel Har-Peled (http://www.uiuc.edu/~sariel)
 *---------------------------------------------------------------
 * History:
 * 3/28/01 - Extended test program, so that it can load a
 *           text-file with points. Format is
 *                    [npoints]
 *                    [x_1 y_1 z_1]
 8                     ...
 *                    [x_n y_n z_n]
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include  <stdlib.h>
#include  <stdio.h>
#include  <assert.h>
#include  <memory.h>
#include  <math.h>

#include  "gdiam.hpp"

/*--- Start of Code ---*/


void   test_itself( gdiam_real  * points, int  num )
{
    GPointPair   pair;

    printf( "Computing the diameter for %d points selected "
            "uniformly from the unit cube\n", num );
    pair = gdiam_approx_diam_pair( (gdiam_real *)points, num, 0.0 );
    printf( "Diameter distance: %g\n", pair.distance );
    printf( "Points realizing the diameter\n"
            "\t(%g, %g, %g) - (%g, %g, %g)\n",
            pair.p[ 0 ], pair.p[ 1 ], pair.p[ 2 ],
            pair.q[ 0 ], pair.q[ 1 ], pair.q[ 2 ] );


    gdiam_point  * pnt_arr;
    gdiam_bbox   bb;

    pnt_arr = gdiam_convert( (gdiam_real *)points, num );

    printf( "Computing a tight-fitting bounding box of the point-set\n" );
    bb = gdiam_approx_mvbb_grid_sample( pnt_arr, num, 5, 400 );

    printf( "Resulting bounding box:\n" );
    bb.dump();

    printf( "Axis parallel bounding box\n" );
    GBBox   bbx;
    bbx.init();
    for  ( int  ind = 0; ind < num; ind++ )
        bbx.bound( points + (ind * 3) );
    bbx.dump();
        
}


void   standard_test()
{
    gdiam_real  * points;
    int  num;

    num = 1000000;

    points = (gdiam_point)malloc( sizeof( gdiam_point_t ) * num );
    assert( points != NULL );

    // Pick randomly points from the unit cube */
    for  ( int  ind = 0; ind < num; ind++ ) {
        points[ ind * 3 + 0 ] = drand48();
        points[ ind * 3 + 1 ] = drand48();
        points[ ind * 3 + 2 ] = drand48();
    }

    test_itself( points, num );
}


void  read_points( FILE   * fl, gdiam_real  * points, int  points_num )
{
    int  args;
    double  x, y, z;

    for  ( int  ind = 0; ind < points_num; ind++ ) {
        args = fscanf( fl, "%lg %lg %lg\n", &x, &y, &z );
        assert( args == 3 );

        points[ ind * 3 + 0 ] = x;
        points[ ind * 3 + 1 ] = y;
        points[ ind * 3 + 2 ] = z;
    }
}


void  test_file( const char  * file_name )
{
    gdiam_real  * points;
    FILE   * fl;
    int  args, points_num;

    fl = fopen( file_name, "rt" );
    if  ( fl == NULL ) {
        printf( "Unable to open file: [%s]\n", file_name );
        exit( -1 );
    }
    args = fscanf( fl, "%d\n", &points_num );
    assert( ( args > 0 )  &&  ( points_num > 0 ) );

    points = (gdiam_point)malloc( sizeof( gdiam_point_t ) * points_num );
    assert( points != NULL );

    read_points( fl, points, points_num );
    fclose( fl );

    test_itself( points, points_num );
}


int  main( int  argc, char  ** argv )
{
    if  ( argc == 1 ) {
        standard_test();
        return  0;
    }

    for  ( int  ind = 1; ind < argc; ind++ )
        test_file( argv[ ind ] );

    return  0;
}

/* gdiam_test.C - End of File ------------------------------------------*/
