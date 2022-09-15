/*
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include <cairo/cairo.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <GL/glu.h>

#define UNUSED( x ) ( ( void )( x ) )

static void error_callback( int error, const char *description )
{
   UNUSED( error );
   fprintf( stderr, "Error: %s\n", description );
}

static void key_callback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
   UNUSED( scancode );
   UNUSED( mods );

   if( action != GLFW_PRESS )
   {
      return;
   }

   switch( key )
   {
      case GLFW_KEY_ESCAPE:
         glfwSetWindowShouldClose( window, GLFW_TRUE );
         break;
   }

}

static cairo_t *create_cairo_context( unsigned char **data, cairo_surface_t **surface, int width, int height )
{
   int stride;
   cairo_t *cr;

   stride = cairo_format_stride_for_width( CAIRO_FORMAT_ARGB32, width );
   *data = malloc( stride * height );
   *surface = cairo_image_surface_create_for_data( *data, CAIRO_FORMAT_ARGB32, width, height, stride );

   cr = cairo_create( *surface );

   return cr;
}

static void refresh( unsigned int *texture, int width, int height  )
{
   glViewport( 0, 0, width, height );
   glClear( GL_COLOR_BUFFER_BIT );
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( 0.0f, width, height, 0.0f, -1.0f, 1.0f );

   glDeleteTextures( 1, texture );

   glGenTextures( 1, texture );
   glBindTexture( GL_TEXTURE_2D, *texture );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL );

   glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
}

int main( void )
{
   GLFWwindow *win;
   int width = 720, height = 450;
   cairo_surface_t *sf;
   unsigned char *data;
   cairo_t *cr;
   int tmp_w = 0, tmp_h = 0;
   unsigned int texture;
   cairo_text_extents_t te;
   const char *text = "cairo_image_surface_create_for_data";
   double x, y;

   glfwSetErrorCallback( error_callback );

   if( ! glfwInit() )
   {
      exit( EXIT_FAILURE );
   }

   win = glfwCreateWindow( width, height, "GLFW .AND. Cairo", NULL, NULL );
   if( ! win )
   {
      glfwTerminate();
      exit( EXIT_FAILURE );
   }

   glfwSetKeyCallback( win, key_callback );

   glfwMakeContextCurrent( win );
   glfwSwapInterval( 1 );

   glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
   glDisable( GL_DEPTH_TEST );
   glEnable( GL_BLEND );
   glEnable( GL_TEXTURE_2D );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

   cr = create_cairo_context( &data, &sf, width, height );

   while( ! glfwWindowShouldClose( win ) )
   {
      glfwGetFramebufferSize( win, &width, &height );

      if( tmp_w != width || tmp_h != height )
      {
         refresh( &texture, width, height  );
         free( data );
         cairo_destroy( cr );
         cr = create_cairo_context( &data, &sf, width, height );

         tmp_w = width;
         tmp_h = height;
      }
      //---
      cairo_save( cr );
      cairo_set_source_rgba( cr, 1.0, 1.0, 1.0, 1.0 );
      cairo_set_operator( cr, CAIRO_OPERATOR_OVER );
      cairo_scale( cr, width, height );
      cairo_paint( cr );
      cairo_restore( cr );

      cairo_push_group( cr );
      cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 0.0 );
      cairo_paint( cr );

      cairo_set_source_rgba( cr, 0.0, 0.0, 0.0, 1.0 );
      cairo_select_font_face( cr, "FreeMono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD );
      cairo_set_font_size( cr, 26 );
      cairo_text_extents( cr, text, &te );
      x = ( width  - te.width ) / 2;
      y = ( height + te.height ) / 2;
      cairo_move_to( cr, x, y );
      cairo_show_text( cr, text );

      cairo_surface_flush( sf );

      cairo_pop_group_to_source( cr );
      cairo_paint( cr );
      //---
      glMatrixMode( GL_MODELVIEW );
      glLoadIdentity();
      glPushMatrix();

      glGenTextures( 1, &texture);
      glBindTexture( GL_TEXTURE_2D, texture );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data );

      glBegin( GL_QUADS );
         glTexCoord2f( 0.0f, 0.0f ); glVertex2f( 0.0f, 0.0f );
         glTexCoord2f( 1.0f, 0.0f ); glVertex2f( width, 0.0f );
         glTexCoord2f( 1.0f, 1.0f ); glVertex2f( width, height );
         glTexCoord2f( 0.0f, 1.0f ); glVertex2f( 0.0f, height );
      glEnd();

      glPopMatrix();

      glfwSwapBuffers( win );
      glfwWaitEvents();
   }

   free( data );
   cairo_destroy( cr );

   glfwDestroyWindow( win );
   glfwTerminate();
   exit( EXIT_SUCCESS );
}
