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

typedef struct
{
   GLFWwindow *      win;
   int               width;
   int               height;
   Display *         xdpy;
   Window            xwin;
   Visual *          vis;
   cairo_surface_t * surface;
   cairo_t *         cr;
} GLFW_Cairo;

static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods )
{
   UNUSED( scancode );
   UNUSED( mods );

   if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
   {
      glfwSetWindowShouldClose( window, GLFW_TRUE );
   }
}

int main()
{
   int width = 0, height = 0;
   double xc, yc, xc1, yc1;
   double radius = 150;
   size_t n = 45;
   size_t j;

   GLFW_Cairo gl;
   memset( &gl, 0, sizeof( gl ) );

   if( ! glfwInit() )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

   gl.win = glfwCreateWindow( 720, 650, "GLFW .AND. Cairo", NULL, NULL );
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
         cairo_set_source_rgb( gl.cr, 0.0, 1.0, 0.0 );
         cairo_set_line_width( gl.cr, 1.0 );

         cairo_arc( gl.cr, gl.width / 2, gl.height / 2, radius * 2, 0, 2 * M_PI );
         cairo_stroke( gl.cr );

         for( size_t i = 0; i < n; i++ )
         {
            for( j = 0; j < 2; j++ )
            {
               xc1 = sin(        ( 2 * i + j ) * 2 * M_PI / ( n * 2 ) ) * radius * 2 + gl.width / 2;
               yc1 = cos( M_PI + ( 2 * i + j ) * 2 * M_PI / ( n * 2 ) ) * radius * 2 + gl.height / 2;
               cairo_move_to( gl.cr, xc1, yc1 );
               cairo_line_to( gl.cr, gl.width / 2, gl.height / 2 );
               cairo_stroke( gl.cr );
            }
            xc = sin(        i * 2 * M_PI / n ) * radius + gl.width / 2;
            yc = cos( M_PI + i * 2 * M_PI / n ) * radius + gl.height / 2;
            cairo_arc( gl.cr, xc, yc, radius, 0, 2 * M_PI );
            cairo_stroke( gl.cr );
         }
         width  = gl.width;
         height = gl.height;
         printf( "Width  : %i\nWeight : %i \n", width, height );
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
