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

#define MENU_ITEM   7
#define STRING_SIZE 23

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
   unsigned int         row;
} GLFW_Cairo;

static GLFW_Cairo gl;

static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );

int main()
{
   memset( &gl, 0, sizeof( gl ) );
   unsigned int i;
   double x;
   char str[ 2 ];

   char menuItem[ MENU_ITEM ][ STRING_SIZE ] =
   {
      "Wheel and tire cleaner",
      "Pre-soak",
      "High-pressure soap",
      "Foam brush",
      "High pressure rinse",
      "High pressure wax",
      "Spot free final rinse"
   };

   if( ! glfwInit() )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

   gl.win = glfwCreateWindow( 720, 450, "GLFW .AND. Cairo", NULL, NULL );
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

   gl.row = 0;
   while( ! glfwWindowShouldClose( gl.win ) )
   {
      glfwGetFramebufferSize( gl.win, &gl.width, &gl.height );
      cairo_xlib_surface_set_size( gl.surface, gl.width, gl.height );

      cairo_push_group( gl.cr );

      cairo_set_source_rgb( gl.cr, 0.0, 0.0, 0.0 );
      cairo_set_operator( gl.cr, CAIRO_OPERATOR_SOURCE );
      cairo_paint( gl.cr );
      //---

      for( i = 0; i < MENU_ITEM; i++ )
      {
         if( gl.row == i )
         {
            cairo_set_source_rgb( gl.cr, 0.0, 1.0, 0.0 );
            cairo_move_to( gl.cr, 300.0, ( 20.0 + 40.0 ) * i + 40.0 );
            cairo_line_to( gl.cr, 350.0, ( 20.0 + 40.0 ) * i + 40.0 );
            cairo_set_line_width( gl.cr, 40.0 );
            cairo_set_line_cap( gl.cr, CAIRO_LINE_CAP_ROUND );
            cairo_stroke( gl.cr );
         }
         else
         {
            cairo_set_source_rgb( gl.cr, 0.0, 0.549019608, 0.862745098 );
            cairo_move_to( gl.cr, 300.0, ( 20.0 + 40.0 ) * i + 40.0 );
            cairo_line_to( gl.cr, 350.0, ( 20.0 + 40.0 ) * i + 40.0 );
            cairo_set_line_width( gl.cr, 40.0 );
            cairo_set_line_cap( gl.cr, CAIRO_LINE_CAP_ROUND );
            cairo_stroke( gl.cr );
         }
         
         cairo_set_source_rgb( gl.cr, 0.235294118, 0.235294118, 0.235294118 );
         cairo_rectangle( gl.cr, 0, ( 20 + 40 ) * i + 20, 300, 40 );
         cairo_fill( gl.cr );

         if( gl.row == i )
         {
            cairo_text_extents( gl.cr, menuItem[ i ], &gl.te );
            x = ( 300 - gl.te.width ) / 2;
            cairo_set_source_rgb( gl.cr, 0.0, 1.0, 0.0 );
            cairo_move_to( gl.cr, x, 50 + 60 * i );
            cairo_show_text( gl.cr, menuItem[ i ] );
         }
         else
         {
            cairo_text_extents( gl.cr, menuItem[ i ], &gl.te );
            x = ( 300 - gl.te.width ) / 2;
            cairo_set_source_rgb( gl.cr, 0.0, 0.549019608, 0.862745098 );
            cairo_move_to( gl.cr, x, 50.0 + 60.0 * i );
            cairo_show_text( gl.cr, menuItem[ i ] );
         }
         cairo_set_source_rgb( gl.cr, 0.0, 0.0, 0.0 );
         cairo_move_to( gl.cr, 325.0, 50.0 + 60.0 * i );
         sprintf( str, "%d", i );
         cairo_show_text( gl.cr, str );
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
      case GLFW_KEY_UP:
      {
         if( gl.row > 0 )
         {
            gl.row--;
         }
         break;
      }
      case GLFW_KEY_DOWN:
      {
         if( gl.row < MENU_ITEM - 1)
         {
            gl.row++;
         }
         break;
      }
   }
}
