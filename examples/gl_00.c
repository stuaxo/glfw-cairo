/*
 *
 */

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#define UNUSED( x ) ( ( void )( x ) )

static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
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

int main( void )
{
   int width, height;
   cairo_text_extents_t te;
   const char* text = "https://github.com/rjopek/glfw-cairo";
   double x, y;

   int tmp_w = 0, tmp_h = 0;

   if( ! glfwInit() )
   {
      glfwTerminate();
      return 1;
   }

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

   GLFWwindow* window = glfwCreateWindow( 720, 450, "GLFW .AND. Cairo", NULL, NULL );
   if( ! window )
   {
      glfwTerminate();
      return 1;
   }

   glfwSwapInterval( 1 );

   Display* xdpy = glfwGetX11Display();
   Window xwin = glfwGetX11Window( window );

   Visual* vis = DefaultVisual( xdpy, DefaultScreen( xdpy ) );

   glfwSetKeyCallback( window, key_callback );

   while( ! glfwWindowShouldClose( window ) )
   {
      glfwGetFramebufferSize( window, &width, &height );
      //---
      if( tmp_w != width || tmp_h != height )
      {
         cairo_surface_t* surface = cairo_xlib_surface_create( xdpy, xwin, vis, width, height );
         cairo_t* cr = cairo_create( surface );
         cairo_xlib_surface_set_size( surface, width, height );

         cairo_set_source_rgb( cr, 1.0, 1.0, 1.0 );
         cairo_rectangle( cr, 0, 0, width, height );
         cairo_fill( cr );

         cairo_select_font_face( cr, "FreeMono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD );
         cairo_set_font_size( cr, 32 );
         cairo_set_source_rgb( cr, 0.0, 0.0, 0.0 );

         cairo_text_extents( cr, text, &te );

         x = ( width  - te.width ) / 2;
         y = ( height + te.height ) / 2;

         cairo_move_to( cr, x, y );
         cairo_show_text( cr, text );

         cairo_surface_destroy( surface );
         cairo_destroy( cr );

         tmp_w = width;
         tmp_h = height;
      }
      //---
      glfwSwapBuffers( window );
      glfwPollEvents();
   }

   glfwDestroyWindow( window );

   glfwTerminate();

   return 0;
}
