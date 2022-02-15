/*
 * Simple file viewer using the GLFW .AND. Cairo
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
   size_t               ad;
   char **              fileLine;
   size_t               fileSize;
   size_t               row;
   size_t               col;
   int                  addRow;
   int                  addCol;
   size_t               nLine;
} GLFW_Cairo;

static GLFW_Cairo gl;

static void init_GLFW_Cairo( void );
static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods );

int main( int argc, char * argv[] )
{
   memset( &gl, 0, sizeof( gl ) );
   FILE * fp;
   ssize_t lineLen = 0;
   char * line = NULL;
   size_t linecap = 0;
   size_t i = 0;
   double x, y;

   if( argc != 2 )
   {
      argv[ 1 ] = "gl_10.c";
   }

   if( ! ( fp = fopen( argv[ 1 ], "r" ) ) )
   {
      perror( "File opening failed" );
      exit( EXIT_FAILURE );
   }

   init_GLFW_Cairo();

   while( ( lineLen = getline( &line, &linecap, fp ) ) != -1 )
   {
      if( i >= gl.fileSize )
      {
         gl.fileSize++;
         gl.fileLine = realloc( gl.fileLine, gl.fileSize * sizeof( char ** ) );
      }
      line[ strcspn( line, "\n" ) ] = 0;

      *( gl.fileLine + i ) = malloc( ( lineLen + 1 ) * sizeof( char ) );

      strcpy( * ( gl.fileLine + i ), line );

      i++;
   }
   fclose( fp );
   free( line );

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
   //---
   cairo_select_font_face( gl.cr, "FreeMono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL );
   cairo_set_font_size( gl.cr, 18 );
   cairo_text_extents( gl.cr, "tmp", &gl.te );
   cairo_font_extents( gl.cr, &gl.fe );
   gl.ad = ( gl.fe.ascent + gl.fe.descent );
   //---
   while( ! glfwWindowShouldClose( gl.win ) )
   {
      glfwGetFramebufferSize( gl.win, &gl.width, &gl.height );
      cairo_xlib_surface_set_size( gl.surface, gl.width, gl.height );

      cairo_push_group( gl.cr );
      cairo_set_source_rgb( gl.cr, 0.117647059, 0.117647059, 0.117647059 );
      cairo_set_operator( gl.cr, CAIRO_OPERATOR_SOURCE );
      cairo_paint( gl.cr );
      //---
      for( i = 0; i <= ( gl.height / gl.ad ); i++ )
      {
         gl.nLine = i + gl.addRow;

         if( gl.nLine <= gl.fileSize )
         {
            if( gl.row == i )
            {
               cairo_set_source_rgb( gl.cr, 0.156862745, 0.156862745, 0.156862745 );
               cairo_rectangle( gl.cr, 0, gl.ad * i, gl.width, gl.ad );
               cairo_stroke( gl. cr );
            }

            x = 0;
            y = gl.ad * ( i + 1 ) - gl.fe.descent;

            cairo_set_source_rgb( gl.cr, 0.960784314, 0.960784314, 0.960784314 );
            cairo_move_to( gl.cr, x, y );
            cairo_show_text( gl.cr, *( gl.fileLine + gl.nLine ) );
         }
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

   for( i = 0; i < gl.fileSize; i++ )
   {
      free( *( gl.fileLine + i ) );
   }

   free( gl.fileLine );

   glfwTerminate();
   exit( EXIT_SUCCESS );
}

static void init_GLFW_Cairo( void )
{
   gl.fileLine = malloc( 1 * sizeof( char ** ) );
   gl.fileSize = 0;
   gl.row      = 0;
   gl.col      = 0;
   gl.addRow   = 0;
   gl.addCol   = 0;
   gl.nLine    = 0;
}

static void key_callback( GLFWwindow * window, int key, int scancode, int action, int mods )
{
   int width, height;
   UNUSED( scancode );
   UNUSED( mods );

   glfwGetFramebufferSize( window, &width, &height );

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
         else
         {
            if( gl.addRow > 0 )
            {
               gl.addRow--;
            }
         }
         break;
      }
      case GLFW_KEY_DOWN:
      {
         if( gl.row < ( height / gl.ad ) && gl.row < gl.fileSize )
         {
            gl.row++;
         }
         else
         {
            if( gl.addRow + gl.row < gl.fileSize )
            {
               gl.addRow++;
            }
         }
         break;
      }
   }
}
