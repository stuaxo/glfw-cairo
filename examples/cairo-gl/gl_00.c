/*
 *
 */
#include <math.h>

#include <cairo/cairo.h>
#include <cairo/cairo-gl.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <GLFW/glfw3native.h>

int main( void )
{
   GLFWwindow *win;
   int width = 720, height = 450;
   cairo_device_t *device;
   cairo_surface_t *sf;
   cairo_t *cr;
   int tmp_w = 0, tmp_h = 0;
   double x = 25.0, y = 25.0;
   double w, h;
   double aspect = 1.0;
   double corner_radius = height / 10.0;
   double radius = corner_radius / aspect;
   double degrees = M_PI / 180.0;

   if( ! glfwInit() )
   {
      return -1;
   }

   win = glfwCreateWindow( width, height, "Render Cairo output to a GLFW window, using GL as backend", NULL, NULL );
   if( ! win )
   {
      glfwTerminate();
      return -1;
   }

   glfwMakeContextCurrent( win );
   glfwSwapInterval( 1 );

   device = cairo_glx_device_create( glfwGetX11Display(), glfwGetGLXContext( win ) );
   sf = cairo_gl_surface_create_for_window( device, glfwGetX11Window( win ), width, height );
   cr = cairo_create( sf );

   while( ! glfwWindowShouldClose( win ) )
   {
      glfwGetFramebufferSize( win, &width, &height );
      cairo_gl_surface_set_size( sf, width, height );

      if( tmp_w != width || tmp_h != height )
      {
         w = width - 50.0;
         h = height - 50.0;

         cairo_new_sub_path( cr );
         cairo_arc( cr, x + w - radius, y + radius, radius, -90 * degrees, 0 * degrees );
         cairo_arc( cr, x + w - radius, y + h - radius, radius, 0 * degrees, 90 * degrees );
         cairo_arc( cr, x + radius, y + h - radius, radius, 90 * degrees, 180 * degrees );
         cairo_arc( cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees );
         cairo_close_path( cr );

         cairo_set_source_rgb( cr, 0.5, 0.5, 1 );
         cairo_fill_preserve( cr );
         cairo_set_source_rgba( cr, 0.5, 0, 0, 0.5 );
         cairo_set_line_width( cr, 10.0 );
         cairo_stroke( cr );

         tmp_w = width;
         tmp_h = height;
      }
      cairo_gl_surface_swapbuffers( sf );
      glfwSwapBuffers( win );
      glfwWaitEvents();
   }

   cairo_device_destroy( device );
   cairo_surface_destroy( sf );
   cairo_destroy( cr );

   glfwDestroyWindow( win );
   glfwTerminate();
   return 0;
}
