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
   GLFWwindow *         win;
   int                  width;
   int                  height;
   Display *            xdpy;
   Window               xwin;
   Visual *             vis;
   cairo_surface_t *    surface;
   cairo_t *            cr;
   cairo_text_extents_t te;
   cairo_font_extents_t fe;
} GLFW_Cairo;

static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );

static int row = 0;

int main()
{
   int width = 0, height = 0;
   const char * text = "Vertical Menu";
   double xc;
   int i;


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

      const int scancode = glfwGetKeyScancode( GLFW_KEY_X );

      if( width != gl.width || height != gl.height || scancode == GLFW_KEY_DOWN )
      {
         cairo_set_source_rgb( gl.cr, 1.0, 1.0, 1.0 );
         cairo_set_operator( gl.cr, CAIRO_OPERATOR_SOURCE );
         cairo_paint( gl.cr );
         //---
         cairo_select_font_face( gl.cr, "FreeSans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD );
         cairo_set_font_size( gl.cr, 72 );
         cairo_set_source_rgb( gl.cr, 0.1961, 0.1961, 0.1961 );

         cairo_text_extents( gl.cr, text, &gl.te );
         cairo_font_extents( gl.cr, &gl.fe );

         xc = ( gl.width - gl.te.width ) / 2;

         cairo_move_to( gl.cr, xc, 67 );
         cairo_show_text( gl.cr, text );

         width  = gl.width;
         height = gl.height;
      }
      for( i = 0; i <= 4; i++ )
      {
         if( row == i )
         {
            cairo_set_source_rgb( gl.cr, 1.0, 0.3921, 0.0 );
            cairo_rectangle( gl.cr, 0, 82 + 40 * i, gl.width, 40 );
            cairo_fill( gl.cr );
         }
         else
         {
            cairo_set_source_rgb( gl.cr, 1.0, 1.0, 1.0 );
            cairo_rectangle( gl.cr, 0, 82 + 40 * i, gl.width, 40 );
            cairo_fill( gl.cr );
         }
         cairo_set_source_rgb( gl.cr, 0.9529, 0.9529, 0.9529 );
         cairo_set_line_width( gl.cr, 0.5 );
         cairo_rectangle( gl.cr, 0, 82 + 40 * i, gl.width, 40 );
         cairo_stroke( gl.cr );
      }
      printf( "row : %i\n", row );
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
      case GLFW_KEY_UP:
      {
         if( row > 0 )
         {
            row--;
         }
         break;
      }
      case GLFW_KEY_DOWN:
      {
         if( row < 4 )
         {
            row++;
         }
         break;
      }
   }
}
