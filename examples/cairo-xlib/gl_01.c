/*
 *
 */

#include <math.h>

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#define UNUSED( x ) ( ( void )( x ) )

static void key_callback( GLFWwindow* win, int key, int scancode, int action, int mods )
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
         glfwSetWindowShouldClose( win, GLFW_TRUE );
         break;
      }
   }
}

int main( void )
{
   GLFWwindow *win;
   int width, height;
   Display *xdpy;
   Window xwin;
   Visual *vis;
   cairo_surface_t *sf;
   cairo_t *cr;
   size_t i;
   double x, y;
   double radius = 100;
   size_t n = 45;
   int tmp_w = 0, tmp_h = 0;

   if( ! glfwInit() )
   {
      return 1;
   }

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

   win = glfwCreateWindow( 720, 450, "GLFW .AND. Cairo", NULL, NULL );
   if( ! win )
   {
      glfwTerminate();
      return 1;
   }

   glfwSwapInterval( 1 );

   xdpy = glfwGetX11Display();
   xwin = glfwGetX11Window( win );
   vis = DefaultVisual( xdpy, DefaultScreen( xdpy ) );

   glfwSetKeyCallback( win, key_callback );

   sf = cairo_xlib_surface_create( xdpy, xwin, vis, width, height );
   cr = cairo_create( sf );

   while( ! glfwWindowShouldClose( win ) )
   {
      glfwGetFramebufferSize( win, &width, &height );
      //---
      if( tmp_w != width || tmp_h != height )
      {
         cairo_xlib_surface_set_size( sf, width, height );

         cairo_set_source_rgb( cr, 1.0, 1.0, 1.0 );
         cairo_rectangle( cr, 0, 0, width, height );
         cairo_fill( cr );

         cairo_set_source_rgb( cr, 1.0, 0.0, 0.0 );
         cairo_set_line_width( cr, 1.0 );
         for( i = 0; i < n; i++ )
         {
            x = sin(        i * 2 * M_PI / n ) * radius + width / 2;
            y = cos( M_PI + i * 2 * M_PI / n ) * radius + height / 2;
            cairo_arc( cr, x, y, radius, 0, 2 * M_PI );
            cairo_stroke( cr );
         }

         tmp_w = width;
         tmp_h = height;
      }
      //---
      glfwSwapBuffers( win );
      glfwPollEvents();
   }

   cairo_surface_destroy( sf );
   cairo_destroy( cr );

   glfwDestroyWindow( win );

   glfwTerminate();

   return 0;
}
