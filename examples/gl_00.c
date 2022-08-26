/*
 *
 */

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#define UNUSED( x ) ( ( void )( x ) )

static void key_callback( GLFWwindow *win, int key, int scancode, int action, int mods )
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
   Display *xdpy;
   Window xwin;
   Visual *vis;
   cairo_surface_t *sf;
   cairo_t *cr;
   int width, height;
   cairo_text_extents_t te;
   const char *text = "https://github.com/rjopek/glfw-cairo";
   double x, y;
   int tmp_w = 0, tmp_h = 0;

   if( ! glfwInit() )
   {
      glfwTerminate();
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

   while( ! glfwWindowShouldClose( win ) )
   {
      glfwGetFramebufferSize( win, &width, &height );
      //---
      if( tmp_w != width || tmp_h != height )
      {
         sf = cairo_xlib_surface_create( xdpy, xwin, vis, width, height );
         cr = cairo_create( sf );

         cairo_xlib_surface_set_size( sf, width, height );

         cairo_set_source_rgb( cr, 1.0, 1.0, 1.0 );
         cairo_set_operator( cr, CAIRO_OPERATOR_SOURCE );
         cairo_paint( cr );

         cairo_select_font_face( cr, "FreeMono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD );
         cairo_set_font_size( cr, 32 );
         cairo_set_source_rgb( cr, 0.0, 0.0, 0.0 );

         cairo_text_extents( cr, text, &te );

         x = ( width  - te.width ) / 2;
         y = ( height + te.height ) / 2;

         cairo_move_to( cr, x, y );
         cairo_show_text( cr, text );

         cairo_surface_destroy( sf );
         cairo_destroy( cr );

         tmp_w = width;
         tmp_h = height;
      }
      //---
      glfwSwapBuffers( win );
      glfwPollEvents();
   }

   glfwDestroyWindow( win );

   glfwTerminate();

   return 0;
}
