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

#define MENU_ITEM   10
#define STRING_SIZE 10

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
   int                  row;
} GLFW_Cairo;

static GLFW_Cairo gl;

static void rectangle_fill( cairo_t * cr, double x, double y, double w, double h, unsigned long int color );
static void show_text( cairo_t * cr, double x, double y, const char * utf8, unsigned long int hexColor );
static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );

int main()
{
   memset( &gl, 0, sizeof( gl ) );
   int i, j;
   double x, w, h;
   unsigned long int color[ 10 ] = { 0xfd423d, 0x5fd2b1, 0xffc71c, 0xd5de23, 0x4be2ed, 0x93a7b0, 0xafc7ba, 0x8bc43f, 0x00a69c, 0xd1c97e };

   char menuItem[ MENU_ITEM ][ STRING_SIZE ] =
   {
      "H O M E",
      "upload",
      "cloud",
      "schedule",
      "settings",
      "links",
      "account",
      "download",
      "share",
      "search"
   };

   if( ! glfwInit() )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

   gl.win = glfwCreateWindow( 720, 520, "GLFW .AND. Cairo", NULL, NULL );
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

   cairo_set_font_size( gl.cr, 36 );

   gl.row = 0;
   while( ! glfwWindowShouldClose( gl.win ) )
   {
      glfwGetFramebufferSize( gl.win, &gl.width, &gl.height );
      cairo_xlib_surface_set_size( gl.surface, gl.width, gl.height );

      cairo_push_group( gl.cr );

      cairo_set_source_rgb( gl.cr, 1.0, 1.0, 1.0 );
      cairo_set_operator( gl.cr, CAIRO_OPERATOR_SOURCE );
      cairo_paint( gl.cr );
      //---
      w = 10;
      h = 10;

      for( i = 0; i < gl.width; i++ )
      {
         for( j = 0; j < gl.height; j++ )
         {
            rectangle_fill( gl.cr, i * w, j * h, ( i + 1 ) * w, ( j + 1 ) * h, ( ( i + j + 1 ) % 2 == 0 ) ? 0xEEEEEE : 0xFFFFFF );
         }
      }

      for( i = 0; i < 10; i++ )
      {
         if( gl.row == i )
         {
            rectangle_fill( gl.cr, 320.0, 100.0 * i + 20.0, 160, 80, color[ i ] );
         }
         else
         {
            rectangle_fill( gl.cr, 320.0, 100.0 * i + 20.0, 80, 80, color[ i ] );
         }

         rectangle_fill( gl.cr, 20.0, 100.0 * i + 20.0, 300, 80, 0x4A4D52 );

         cairo_text_extents( gl.cr, menuItem[ i ], &gl.te );
         x = ( 340 - gl.te.width ) / 2;
         show_text( gl.cr, x, 75.0 + 100.0 * i, menuItem[ i ], 0xCDD0D5 );
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

static void rectangle_fill( cairo_t * cr, double x, double y, double w, double h, unsigned long int hexColor )
{
   double r, g, b;

   r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
   g = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
   b = (   hexColor         & 0xFF ) / 255.0;

   cairo_set_source_rgb( cr, r, g, b );
   cairo_rectangle( cr, x, y, w, h );
   cairo_fill( cr );
}

static void show_text( cairo_t * cr, double x, double y, const char * utf8, unsigned long int hexColor )
{
   double r, g, b;

   r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
   g = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
   b = (   hexColor         & 0xFF ) / 255.0;

   cairo_set_source_rgb( cr, r, g, b );
   cairo_move_to( cr, x, y );
   cairo_show_text( cr, utf8 );
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
         if( gl.row < MENU_ITEM - 1 )
         {
            gl.row++;
         }
         break;
      }
   }
}
