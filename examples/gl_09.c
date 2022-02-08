/*
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#define UNUSED( x ) ( ( void )( x ) )
#define MAX( a, b ) ( ( a ) < ( b ) ? ( b ) : ( a ) )
#define RANDOMINT( min, max ) ( ( rand() % ( int ) ( ( ( max ) + 1u ) - ( min ) ) ) + ( min ) )

typedef struct
{
   GLFWwindow *         win;
   int                  width;
   int                  height;
   Display *            xdpy;
   Window               xwin;
   Visual *             vis;
   cairo_surface_t *    surface;
   cairo_t *            cr;
} GLFW_Cairo;

static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );

int main()
{
   int width = 0, height = 0;
   double x, y;
   double r = 1.0, g = 1.0, b = 1.0;
   int i, n;

   GLFW_Cairo gl;
   memset( &gl, 0, sizeof( gl ) );

   if( ! glfwInit() )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

   gl.win = glfwCreateWindow( 720, 512, "GLFW .AND. Cairo", NULL, NULL );
   if( ! gl.win )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwSwapInterval( 1 );

   gl.xdpy = glfwGetX11Display();
   gl.xwin = glfwGetX11Window( gl.win );

   gl.vis  = DefaultVisual( gl.xdpy, DefaultScreen( gl.xdpy ) );
   if( ! gl.vis )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwGetFramebufferSize( gl.win, &gl.width, &gl.height );
   gl.surface  = cairo_xlib_surface_create( gl.xdpy, gl.xwin, gl.vis, gl.width, gl.height );

   gl.cr = cairo_create( gl.surface );
   if( ! gl.cr )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwSetKeyCallback( gl.win, key_callback );

   while( ! glfwWindowShouldClose( gl.win ) )
   {
      glfwGetFramebufferSize( gl.win, &gl.width, &gl.height );
      cairo_xlib_surface_set_size( gl.surface, gl.width, gl.height );

      cairo_push_group( gl.cr );

      if( width != gl.width || height != gl.height )
      {
         cairo_set_source_rgb( gl.cr, 0.0, 0.0, 0.0 );
         cairo_set_operator( gl.cr, CAIRO_OPERATOR_SOURCE );
         cairo_paint( gl.cr );
         //---
         x = 0;
         y = 0;

         n = MAX( gl.width, gl.height );
         n = n * n / 4;

         for( i = 0; i < n; i++ )
         {
            switch( RANDOMINT( 0, 4 ) )
            {
            case 0:
               x = 0.333 * x;
               y = 0.333 * y;
               r = 1.0; g = 0.0; b = 0.0;
               break;
            case 1:
               x = 0.333 * x + 0.666;
               y = 0.333 * y;
               r = 0.0; g = 1.0; b = 0.0;
               break;
            case 2:
               x = 0.333 * x + 0.333;
               y = 0.333 * y + 0.333;
               r = 1.0; g = 1.0; b = 0.0;
               break;
            case 3:
               x = 0.333 * x;
               y = 0.333 * y + 0.666;
               r = 0.0; g = 0.0; b = 1.0;
               break;
            case 4:
               x = 0.333 * x + 0.666;
               y = 0.333 * y + 0.666;
               r = 0.0; g = 1.0; b = 1.0;
               break;
            }
            cairo_set_source_rgb( gl.cr, r, g, b );
            cairo_rectangle( gl.cr, x * gl.width, y * gl.height, 1, 1 );
            cairo_fill( gl.cr );
         }
         width  = gl.width;
         height = gl.height;
      }
      //---
      cairo_pop_group_to_source( gl.cr );
      cairo_paint( gl.cr );
      cairo_surface_flush( gl.surface );

      glfwSwapBuffers( gl.win );
      glfwPollEvents();
   }
   cairo_destroy( gl.cr );
   cairo_surface_finish( gl.surface );
   cairo_surface_destroy( gl.surface );
   glfwDestroyWindow( gl.win );

   glfwTerminate();
   exit( EXIT_SUCCESS );
}

static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods )
{
   UNUSED( scancode );
   UNUSED( mods );

   if( action == GLFW_PRESS )
   {
      return;
   }

   switch( key )
   {
      case GLFW_KEY_ESCAPE:
      {
         glfwSetWindowShouldClose( window, GLFW_TRUE );
         break;
      }
   }
}
