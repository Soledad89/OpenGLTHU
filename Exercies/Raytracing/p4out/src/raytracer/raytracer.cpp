/**
 * @file raytacer.cpp
 * @brief Raytracer class
 *
 * Implement these functions for project 2.
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#include "raytracer.hpp"
#include "scene/scene.hpp"

#include <SDL/SDL_timer.h>
#include <iostream>


namespace _462 {

Raytracer::Raytracer()
    : scene( 0 ), width( 0 ), height( 0 ) { }

Raytracer::~Raytracer() { }

/**
 * Initializes the raytracer for the given scene. Overrides any previous
 * initializations. May be invoked before a previous raytrace completes.
 * @param scene The scene to raytrace.
 * @param width The width of the image being raytraced.
 * @param height The height of the image being raytraced.
 * @return true on success, false on error. The raytrace will abort if
 *  false is returned.
 */
bool Raytracer::initialize( Scene* scene, size_t width, size_t height )
{
    this->scene = scene;
    this->width = width;
    this->height = height;

    current_row = 0;

    // TODO any initialization or precompuation before the trace

    return true;
}


/**
 * Performs a raytrace on the given pixel on the current scene.
 * The pixel is relative to the bottom-left corner of the image.
 * @param scene The scene to trace.
 * @param x The x-coordinate of the pixel to trace.
 * @param y The y-coordinate of the pixel to trace.
 * @param width The width of the screen in pixels.
 * @param height The height of the screen in pixels.
 * @return The color of that pixel in the final image.
 */
static Color3 trace_pixel( const Scene* scene, size_t x, size_t y, size_t width, size_t height )
{
    assert( 0 <= x && x < width );
    assert( 0 <= y && y < height );
    // TODO return the color of the given pixel
    // you don't have to use this stub function if you prefer to
    // write your own version of Raytracer::raytrace.
    return Color3( 0.3, 0.2, 0.9 ); // just a test color
}

/**
 * Raytraces some portion of the scene. Should raytrace for about
 * max_time duration and then return, even if the raytrace is not copmlete.
 * The results should be placed in the given buffer.
 * @param buffer The buffer into which to place the color data. It is
 *  32-bit RGBA (4 bytes per pixel), in row-major order.
 * @param max_time, If non-null, the maximum suggested time this
 *  function raytrace before returning, in seconds. If null, the raytrace
 *  should run to completion.
 * @return true if the raytrace is complete, false if there is more
 *  work to be done.
 */
bool Raytracer::raytrace( unsigned char *buffer, real_t* max_time )
{
    // TODO Add any modifications to this algorithm, if needed.

    static const size_t PRINT_INTERVAL = 64;

    // the time in milliseconds that we should stop
    unsigned int end_time = 0;
    bool is_done;

    if ( max_time ) {
        // convert duration to milliseconds
        unsigned int duration = (unsigned int) ( *max_time * 1000 );
        end_time = SDL_GetTicks() + duration;
    }

    // until time is up, run the raytrace. we render an entire row at once
    // for simplicity and efficiency.
    for ( ; !max_time || end_time > SDL_GetTicks(); ++current_row ) {

        if ( current_row % PRINT_INTERVAL == 0 ) {
            printf( "Raytracing (row %u)...\n", current_row );
        }

        // we're done if we finish the last row
        is_done = current_row == height;
        // break if we finish
        if ( is_done )
            break;

        for ( size_t x = 0; x < width; ++x ) {
            // trace a pixel
            Color3 color = trace_pixel( scene, x, current_row, width, height );
            // write the result to the buffer, always use 1.0 as the alpha
            color.to_array( &buffer[4 * ( current_row * width + x )] );
        }
    }

    if ( is_done ) {
        printf( "Done raytracing!\n" );
    }

    return is_done;
}

} /* _462 */

