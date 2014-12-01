/**
Basic 3D Sound Visualizer
-- one can load in an audiofile in the console or use the built-in input
- citations: edited portaudio/libsndfile examples used aswell as edited features of the maze project by Professor Danner
Zack Cohen
- to use audio you need to load the file as an argument in the console  
*/

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//required frameworks
#ifdef __MACOSX__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <CoreAudio/CoreAudio.h>
#include <Carbon/Carbon.h>
#include <AudioUnit/AudioUnit.h>
#include <CoreServices/CoreServices.h>
#include <AudioToolbox/AudioToolbox.h>

//if linux - not used in this case 

#elif defined __LINUX__ || defined __CYGWIN__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

//other important libraries
#include "debug.h"
#include "geom356.h"

//sound libraries
#include "portaudio.h"
#include "sndfile.h"



// Defaults for the window.
#define DEFAULT_WIN_WIDTH 800
#define DEFAULT_WIN_HEIGHT 600

//size of array for audio stored data
#define STACK_SIZE 1000
//used array for audio
float stack[STACK_SIZE];

//simple method used to add data to top of array
void push(float v);
int top = -1;
float temp[50];
void push(float value)
{   
   top++;
   stack[top] = value;
}

// important declerations
int iterations;
int win_width, win_height ;
int window;
float beep;
int actual_position;
int frame_Count;
int length_track;// GLUT callbacks.             
void handle_reshape(int w, int h) ;  
  // Reshape callback.
   
#define SAMPLE_RATE         (44100)
#define PA_SAMPLE_TYPE      paFloat32
#define FRAMES_PER_BUFFER   (64)



typedef float SAMPLE;

// Application functions.
void init() ;
float move = 0.0;
float shift;
void add_arr(float test);
//int menu;


//file data and randomizer
char* file;
float change;

//view data
float view_plane_near = 4.0f ;
float view_plane_far = 300.0f ;

// Application data.
enum {NONE, WINDOW, CONSOLE} position_dest = NONE ;

// Materials and lights.  Note that colors are RGBA colors.  OpenGL lights
// have diffuse, specular, and ambient components; we'll default to setting
// the first two equal to each other and the OpenGL default for ambient
// (black).  And the position is in homogeneous coordinates, so a w-value
// of 0 means "infinitely far away."


typedef struct OurData OurData;
struct OurData
{
  SNDFILE *sndFile;
  SF_INFO sfInfo;
  int position;
};


typedef struct _material_t {
    GLfloat ambient[4] ;
    GLfloat diffuse[4] ;
    GLfloat specular[4] ;
    GLfloat phong_exp ;
} material_t ;

typedef struct _light_t {
    GLfloat position[4] ;
    GLfloat color[4] ;
} light_t ;

GLfloat BLACK[4] = {0.0, 0.0, 0.0, 1.0} ;

light_t far_light = {
    {1000.0, 100.0, 00.0, 1.0},
    {0.75, 0.75, 0.75, 1.0}
} ;


void set_camera() {
    debug("set_camera()") ;
    glMatrixMode(GL_MODELVIEW) ;
    glLoadIdentity() ;
    glTranslatef(-60,-10.0,-30.0);
    glRotatef(5, 0, 3, 0) ;

}

void set_lights() {
    debug("set_lights()") ;
    
    light_t* light = &far_light ;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light->color) ;
    glLightfv(GL_LIGHT0, GL_AMBIENT, BLACK) ;
    glLightfv(GL_LIGHT0, GL_SPECULAR, light->color) ;
}

void draw_shape(float moved, float val) {
 
    // Specify the material for the cube.
    GLfloat glcolordefuse[] = {val, 0.8-val, 0.01, 0.0f};
    GLfloat glcolorspecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glcolordefuse) ;
    glMaterialfv(GL_FRONT, GL_SPECULAR, glcolorspecular) ;
    glMaterialf(GL_FRONT, GL_SHININESS, 1000.0f) ;
    
    //create a series of quads that will be used to animate the sound
    glBegin(GL_QUADS) ;
    glNormal3f(0.8, 0.3, 0.0) ;
    glVertex3f(1.0+moved+shift, -1.0+val, 1.0) ;
     glVertex3f(1.0+moved+shift, -1.0+val, -1.0) ;
     glVertex3f(1.0+moved+shift, 1.0+val, -1.0) ;
     glVertex3f(1.0+moved+shift, 1.0+val, 1.0) ;
    GLfloat glcolordefuse2[] = {val, val, 0.01, 0.0f};
    GLfloat glcolorspecular2[] = {val, val, 1.0, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glcolordefuse2) ;
    glMaterialfv(GL_FRONT, GL_SPECULAR, glcolorspecular2) ;
    glMaterialf(GL_FRONT, GL_SHININESS, 1000.0f) ;

    
    glVertex3f(1.0+moved+shift, -2.0+val, 3.0) ;
    glVertex3f(1.0+moved+shift, -2.0+val, 1.0) ;
    glVertex3f(1.0+moved+shift, 0.0+val, 1.0) ;
    glVertex3f(1.0+moved+shift, 0.0+val, 3.0) ;


    GLfloat glcolordefuse3[] = {val, 1-val, 1.0, 0.0f};
    GLfloat glcolorspecular3[] = {val, 1-val, 1.0, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glcolordefuse3) ;
    glMaterialfv(GL_FRONT, GL_SPECULAR, glcolorspecular3) ;
    glMaterialf(GL_FRONT, GL_SHININESS, 1000.0f) ;
    
    glVertex3f(0.0+moved+shift, -5.0+val, 10.0) ;
    glVertex3f(0.0+moved+shift, -5.0+val, -10.0) ;
    glVertex3f(1.0+moved+shift, -5.0+val, -10.0) ;
    glVertex3f(1.0+moved+shift, -5.0+val, 10.0) ;

      

    glEnd() ;
}

void draw_shape2(float moved, float val) {

    // Specify the material for the cube.
    GLfloat glcolordefuse[] = {0.5f, 0+change, val, 0.0f};
     GLfloat glcolorspecular[] = {0.5f, 0.5f, 1.0f, 0.5f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glcolordefuse) ;
    glMaterialfv(GL_FRONT, GL_SPECULAR, glcolorspecular) ;
    glMaterialf(GL_FRONT, GL_SHININESS, 1000.0f) ;
    
    //more quads
    glBegin(GL_QUADS) ;
    glNormal3f(0.8, 0.3, 0.0) ;
    glVertex3f(.0*val+moved+shift, -5.0, -80.0) ;
    glVertex3f(.0*val+moved+shift, 40.0, 70.0) ;
    glVertex3f(5.0*val+moved+shift, -5.0, -80.0) ;
    glVertex3f(5.0*val+moved+shift, 40.0, 70.0) ;

    glEnd() ;
}

void draw_track(moved) {
    debug("track()") ;

    glLightfv(GL_LIGHT0, GL_POSITION, far_light.position) ;


   

    for (int i=0;i<length_track/300;i++){

     GLfloat glcolordefuse[] = {1.0f, 0, shift*0.01, 0.0f};
     GLfloat glcolorspecular[] = {1.0f, 0, shift*0.01, 1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glcolordefuse) ;
        glMaterialfv(GL_FRONT, GL_SPECULAR, glcolorspecular) ;
        glMaterialf(GL_FRONT, GL_SHININESS, 1000.0f) ;
            glBegin(GL_TRIANGLES) ;

        glNormal3f(0.8, 0.3, 0.0) ;
        glVertex3f(300*i, 10.0, -10.0) ;
        glVertex3f(300*i, 00.0, -10.0) ;
        glVertex3f(300*i+300, 0.0, -10.0) ;

        glEnd();

        GLfloat glcolordefuse2[] = {0.3f, 0, 1.0f, 0.0f};
        GLfloat glcolorspecular2[] = {1.0f, 1.0, shift*0.01, 1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glcolordefuse2) ;
        glMaterialfv(GL_FRONT, GL_SPECULAR, glcolorspecular2) ;
        glMaterialf(GL_FRONT, GL_SHININESS, 1000.0f) ;
        
        glBegin(GL_QUADS) ;

        glNormal3f(0.8, 0.3, 0.0) ;
        glVertex3f(100+300*i, 0.0, -5.0) ;
        glVertex3f(100+300*i, 6.0, -5.0) ;
        glVertex3f(100+300*i+20,0.0, -5.0) ;
        glVertex3f(100+300*i+20, 6.0, -5.0) ;

        glEnd();
    }   

}

static int submenu_id;
static int value = 0; 
void menu(int num){
  if(num == 0){
    glutDestroyWindow(window);
    exit(0);
  }else{
    value = num;
  }
  glutPostRedisplay();
} 
void createMenu(void){     submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Microphone", 1);
    glutAddMenuEntry("Audiofile", 2);
    glutAddMenuEntry("Quit", 0);     glutAttachMenu(GLUT_RIGHT_BUTTON);

} 
void display(void){
  glClear(GL_COLOR_BUFFER_BIT);   
         glMatrixMode(GL_MODELVIEW) ;
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(value == 1){

    mic();
  }else if(value == 2){

     
    song();

}
  //glFlush();
} 


void handle_display() {

    
    debug("handle_display()") ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
    glLightfv(GL_LIGHT0, GL_POSITION, far_light.position) ;
    glMatrixMode(GL_MODELVIEW) ;
    change = (float)(rand())/(float)(RAND_MAX); 
    iterations = iterations+1;
    printf("iterations: %d\n", iterations);
    glTranslatef(-1, 0.00, 0.0000f) ;
      move = 0.0;
      shift = shift+1;
    draw_track(move);
    float move1 = 0.0;

    //shift the array
    for (int i=0;i<STACK_SIZE;i++)
        {
        move = move+0.07; 
        draw_shape(move, stack[i]) ;
        move1 = move1+(0.3*beep);
        draw_shape2(move1, stack[i]) ;
        }

   glFlush() ;
}


void set_projection_viewport() {
    debug("set_projection_viewport()") ;
    
    // Set perspective projection transform.
    glMatrixMode(GL_PROJECTION) ;
    glLoadIdentity() ;
    gluPerspective(60.0, (GLdouble)win_width/win_height, view_plane_near,
                   view_plane_far) ;
    
    // Set the viewport transform.
    glViewport(0, 0, win_width, win_height) ;
}
void handle_reshape(int width, int height) {
    debug("handle_resize(%d, %d)\n", width, height) ;
    
    win_width = width ;
    win_height = height ;
    
    set_projection_viewport() ;
    
    glutPostRedisplay() ;
}

/** Handle reshape events.  All we do here is record the new width and
 *  height.
 *
 *  @param w the width of the resized window.
 *  @param h the height of the resized window.
 */

static int gNumNoInputs = 0;
/* This routine will be called by the PortAudio engine when audio is needed.
 ** It may be called at interrupt level on some machines so don't do anything
 ** that could mess up the system like calling malloc() or free().
 */
static int micCallback( const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData )
{
    SAMPLE *out = (SAMPLE*)outputBuffer;
    const SAMPLE *in = (const SAMPLE*)inputBuffer;
    unsigned int i;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) userData;
    float use;
    
    if( inputBuffer == NULL )
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = 0;  /* left - silent */
            *out++ = 0;  /* right - silent */
        }
        gNumNoInputs += 1;
    }
    else
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            use = (*in++);  /* left - clean */
            use = *in++;          /* right - clean */
        }
    }

    beep = use*10;
    add_arr(beep);

    return paContinue;
}

//function shift array
void add_arr(float use){
        for (int i=0;i<STACK_SIZE;i++){
            if (i>0){
                *(stack-1+i)=*(stack+i);
                if (i==STACK_SIZE-1){
                    *(stack+i)=use;
                }
       
            }
        
       
        } 
}
int songCallback(const void *input,
             void *output,
             unsigned long frameCount,
             const PaStreamCallbackTimeInfo* paTimeInfo,
             PaStreamCallbackFlags statusFlags,
             void *userData)
{

  OurData *data = (OurData *)userData; /* we passed a data structure
into the callback so we have something to work with */
  int *cursor; /* current pointer into the output  */
  int *out = (int *)output;
  int thisSize = frameCount;
  int thisRead;
  frame_Count = (int) frameCount;
 
  //printf("frameCount: %d\n", paTimeInfo);
  cursor = out; /* set the output cursor to the beginning */
  while (thisSize > 0)
  {
   
    /* seek to our current file position */
    sf_seek(data->sndFile, data->position, SEEK_SET);

    /* are we going to read past the end of the file?*/
    if (thisSize > (data->sfInfo.frames - data->position))
    {
      /*if we are, only read to the end of the file*/
      thisRead = data->sfInfo.frames - data->position;
      /* and then loop to the beginning of the file */
     
    }
    else
    {
      /* otherwise, we'll just fill up the rest of the output buffer */
      thisRead = thisSize;
      /* and increment the file position */
      data->position += thisRead;

    }



    /* since our output format and channel interleaving is the same as
sf_readf_int's requirements */
    /* we'll just read straight into the output buffer */
    sf_readf_int(data->sndFile, cursor, thisRead);
    /* increment the output cursor*/
    cursor += thisRead;
    /* decrement the number of samples left to process */
    thisSize -= thisRead;
    //printf("%d \n", data->position);
    int out3 = (*out);
    float out4 = (float)out3*.000000005;
    beep = out4;
    add_arr(beep);
    //printf("%dframes:\n", data->position);
    actual_position = data->position;
    length_track = data->sfInfo.frames;
  }
 
  return paContinue;
}

void init() {
    debug("init") ;
    

    move = 0.0;

 float pixels[] = {
    0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
};

    // Set the lights.
    set_lights() ;
    
    // Set the viewpoint.
    set_camera() ;

        
}


void handle_key(int key, int x, int y) {
  //  printf("handle_key\n");
    switch (key) {
        case GLUT_KEY_RIGHT:
            //eye_r += EYE_DIST_INCR ;
              glTranslatef(-1.0, 0.00, 0.0000f) ;
            //set_camera() ;

            break ;
        case GLUT_KEY_LEFT:
            glTranslatef(1.0, 0.00, 0.0000f) ;
            //set_camera() ;
            break ;
        case '-':
            glTranslatef(0.0,0.0,-1.0);
        break;
        case '+':
            glTranslatef(0.0,0.0,1.0);
        break;
        case 'r':
        glTranslatef(1.4142,0.0,1.4142);
            glRotatef(1.0,0.0,0.1,0.0);
        break;
         case GLUT_KEY_DOWN:
            //eye_r += EYE_DIST_INCR ;
              glRotatef(1.0,1.0,0.0,0.0);
            //set_camera() ;

            break ;
             case GLUT_KEY_UP:
            //eye_r += EYE_DIST_INCR ;
          glRotatef(1.0,-1.0,0.0,0.0);
            //set_camera() ;

            break ;
        default:
            break ;
    }
   //glutPostRedisplay() ;
}

/*******************************************************************/
int main(int argc, char **argv);
int mic();
int song();
int main(int argc, char **argv)
{
    shift = 0;
  glutInitWindowSize(DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT) ;
    glutInitWindowPosition(0, 0) ;
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH ) ;
    glutInit(&argc, argv) ;
    if (!glutGet(GLUT_DISPLAY_MODE_POSSIBLE)) {
        fprintf(stderr, "Cannot get requested display mode; exiting.\n") ;
        exit(EXIT_FAILURE) ;
    }
    
    for (int i=0; i<argc; ++i) {
        if (strcmp(argv[i], "--wposition") == 0) position_dest = WINDOW ;
        else if (strcmp(argv[i], "--cposition") == 0) position_dest = CONSOLE ;
    }
     file=argv[1];
    // Create the main window.
    debug("Creating window") ;
    window = glutCreateWindow("Visualizer") ;
    
    debug("Setting callbacks") ;
    glutReshapeFunc(handle_reshape) ;
    //glutKeyboardFunc(handle_key) ;
    glutSpecialFunc(handle_key) ;
    createMenu();
   glutDisplayFunc((display)) ;
    
    // GL initialization.
    glEnable(GL_DEPTH_TEST) ;
    glEnable(GL_LIGHTING) ;
    glEnable(GL_LIGHT0) ;
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1) ;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
    //gluPerspective(50.0, 1.0, 3.0, 7.0);
   
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW) ;

    glLoadIdentity() ;

    glutAttachMenu(GLUT_LEFT_BUTTON);

    // Application initialization.
    init() ;

    //opengl-begin
    glutMainLoop();


}

//the function to use for the input
int mic()
{

     debug("input") ;
            PaStreamParameters inputParameters, outputParameters;
        PaStream *stream;
        PaError err;
            err = Pa_Initialize();
            if( err != paNoError ) goto error;
            
            inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
            if (inputParameters.device == paNoDevice) {
                fprintf(stderr,"Error: No default input device.\n");
                goto error;
            }
            inputParameters.channelCount = 2;       /* stereo input */
            inputParameters.sampleFormat = PA_SAMPLE_TYPE;
            inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
            inputParameters.hostApiSpecificStreamInfo = NULL;
            
            outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
            if (outputParameters.device == paNoDevice) {
                fprintf(stderr,"Error: No default output device.\n");
                goto error;
            }
            outputParameters.channelCount = 2;       /* stereo output */
            outputParameters.sampleFormat = PA_SAMPLE_TYPE;
            outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
            outputParameters.hostApiSpecificStreamInfo = NULL;
            
            err = Pa_OpenStream(
                                &stream,
                                &inputParameters,
                                &outputParameters,
                                SAMPLE_RATE,
                                FRAMES_PER_BUFFER,
                                0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
                                micCallback,
                                NULL );
            if( err != paNoError ) goto error;
            
            err = Pa_StartStream( stream );
            if( err != paNoError ) goto error;
    glutIdleFunc(handle_display);
    glutSpecialFunc(handle_key) ;
    //start opengl loop
    glutMainLoop();
    getchar();
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;
    return 
    printf("Finished. gNumNoInputs = %d\n", gNumNoInputs );

    Pa_Terminate();

    return 0;
    return EXIT_SUCCESS ;
    
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );

    }

//function for loading a song
int song(){
    glutDestroyMenu(menu);
    glutDisplayFunc(display);
               glutIdleFunc(handle_display);
  
            debug("song") ;
                 OurData *data = (OurData *)malloc(sizeof(OurData));
          PaStream *stream;
          PaError error;
          PaStreamParameters outputParameters;

          /* initialize our data structure */
          data->position = 0;
          data->sfInfo.format = 0;
          /* try to open the file */
          data->sndFile = sf_open(file, SFM_READ, &data->sfInfo);

          if (!data->sndFile)
          {
            printf("error opening file\n");
            return 1;
          }
          Pa_Initialize();

          /* set the output parameters */
          outputParameters.device = Pa_GetDefaultOutputDevice(); /* use the
        default device */
          outputParameters.channelCount = data->sfInfo.channels; /* use the
        same number of channels as our sound file */
          outputParameters.sampleFormat = paInt32; /* 32bit int format */
          outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;; /* 200 ms ought to satisfy
        even the worst sound card */
          outputParameters.hostApiSpecificStreamInfo = 0; /* no api specific data */

          /* try to open the output */
          error = Pa_OpenStream(&stream,  /* stream is a 'token' that we need
        to save for future portaudio calls */
                                0,  /* no input */
                                &outputParameters,
                                data->sfInfo.samplerate,  /* use the same
        sample rate as the sound file */
                                paFramesPerBufferUnspecified,  /* let
        portaudio choose the buffersize */
                                paNoFlag,  /* no special modes (clip off, dither off) */
                                songCallback,  /* callback function defined above */
                                data ); /* pass in our data structure so the
        callback knows what's up */
 
   
          /* if we can't open it, then bail out */
          if (error)
          {
            printf("error opening output, error code = %i\n", error);
            Pa_Terminate();
            return 1;
          }

          /* when we start the stream, the callback starts getting called */
          Pa_StartStream(stream);
         
        //start opengl loop
         glutMainLoop() ;
  
          Pa_Sleep(10000); /* pause for 2 seconds (2000ms) so we can hear a bit
        of the output */

          Pa_StopStream(stream); // stop the stream
          Pa_Terminate(); // and shut down portaudio
          return 0;
          return EXIT_SUCCESS ;
     }
