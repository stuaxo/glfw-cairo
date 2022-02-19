/*
 *
 */

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
   GLFWwindow *         win;
   int                  width;
   int                  height;
   Display *            xdpy;
   Window               xwin;
   Visual *             vis;
   cairo_surface_t *    surface;
   cairo_t *            cr;
} GLFW_Cairo;

static GLFW_Cairo gl;

static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );

int main()
{
   memset( &gl, 0, sizeof( gl ) );
   int width = 0, height = 0;
   unsigned int i, j;
   unsigned int w, h;

   if( ! glfwInit() )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

   gl.win = glfwCreateWindow( 720, 720, "GLFW .AND. Cairo", NULL, NULL );
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
   gl.surface = cairo_xlib_surface_create( gl.xdpy, gl.xwin, gl.vis, gl.width, gl.height );

   gl.cr = cairo_create( gl.surface );
   if( ! gl.cr )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwSetKeyCallback( gl.win, key_callback );

   cairo_select_font_face( gl.cr, "FreeSans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD );
   cairo_set_font_size( gl.cr, 24 );

   while( ! glfwWindowShouldClose( gl.win ) )
   {
      glfwGetFramebufferSize( gl.win, &gl.width, &gl.height );
      cairo_xlib_surface_set_size( gl.surface, gl.width, gl.height );

      cairo_push_group( gl.cr );

      if( width != gl.width || height != gl.height )
      {
         cairo_set_source_rgb( gl.cr, 1.0, 1.0, 1.0 );
         cairo_set_operator( gl.cr, CAIRO_OPERATOR_SOURCE );
         cairo_paint( gl.cr );
         //---
         w = gl.width / 8;
         h = gl.height / 8;

         for( i = 0; i < 8; i ++ )
         {
            for( j = 0; j < 8; j ++ )
            {
               if( ( i + j + 1 ) % 2 == 0 )
               {
                  cairo_set_source_rgb( gl.cr, 1.0, 1.0, 1.0 );
               }
               else
               {
                  cairo_set_source_rgb( gl.cr, 0.0, 0.0, 0.0 );
               }
               cairo_rectangle( gl.cr, i * w, j * h, ( i + 1 ) * w, ( j + 1 ) * h );
               cairo_fill( gl.cr );
            }
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
