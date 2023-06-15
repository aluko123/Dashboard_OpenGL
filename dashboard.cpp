#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define  TIMER_PERIOD  30 // Period for the timer.
#define  TIMER_ON     1     // 0:disable timer, 1:enable timer

//jh: Because bitmap fonts aren't affected by glScale, let's
//    just #define some fonts so that we can adjust as we change the scale.
#define DIALFONT GLUT_BITMAP_HELVETICA_10
#define SUMMARYFONT GLUT_BITMAP_HELVETICA_10

#define D2R 0.0174532

typedef struct
{
	float x, y;
}point_t;
typedef struct
{
	point_t a, b, c, d;
	int angle;
}rect_t;

//jh: A struct for storing image data (used to draw icons)
struct Image {

    unsigned long sizeX;

    unsigned long sizeY;

    char *data;

};

typedef struct Image Image;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

rect_t rectangle = { 0 };

//jh: An array of integers. Each integer specifies the index number of an OpenGL texture.
//    Right now, the array has enough room for two texture indices, but I'm only actually
//    using the first index.
GLuint texture[10];

//jh: An image (just one so far -- we'll need one for each type of icon)
Image *image1;
Image* image2;
Image* image3;
Image* image4;
Image* image5;
Image* image6;
Image* image7;
Image* image8;
Image* image9;


//jh: Some simple code to load an image from a bitmap file (24 bits per pixel)
//    (8 bits per channel) only! I used GIMP to save as a 24 bit image. If you
//    use GIMP to create any more icons, make sure you look at the advanced settings
//    when saving to .bmp. By default, my version of GIMP tried to make the images 32
//    bits per pixel!
//Arguments: filename: Image file to read from
//           image: image structure to store data into
int ImageLoad(char *filename, Image *image) {

    FILE *file;

    unsigned long size; // size of the image in bytes.

    unsigned long i; // standard counter.

    unsigned short int planes; // number of planes in image (must be 1)

    unsigned short int bpp; // number of bits per pixel (must be 24)

    char temp; // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.

    if ((file = fopen(filename, "rb")) == NULL) {

        printf("File Not Found : %s\n", filename);

        return 0;

    }

    // seek through the bmp header, up to the width/height:

    fseek(file, 18, SEEK_CUR);

    // read the width

    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {

        printf("Error reading width from %s.\n", filename);

        return 0;

    }

    //printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height

    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {

        printf("Error reading height from %s.\n", filename);

        return 0;

    }

    //printf("Height of %s: %lu\n", filename, image->sizeY);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).

    size = image->sizeX * image->sizeY * 3;

    // read the planes

    if ((fread(&planes, 2, 1, file)) != 1) {

        printf("Error reading planes from %s.\n", filename);

        return 0;

    }

    if (planes != 1) {

        printf("Planes from %s is not 1: %u\n", filename, planes);

        return 0;

    }

    // read the bitsperpixel

    if ((i = fread(&bpp, 2, 1, file)) != 1) {

        printf("Error reading bpp from %s.\n", filename);

        return 0;

    }

    if (bpp != 24) {

        printf("Bpp from %s is not 24: %u\n", filename, bpp);

        return 0;

    }

    // seek past the rest of the bitmap header.

    fseek(file, 24, SEEK_CUR);

    // read the data.

    image->data = (char *)malloc(size);

    if (image->data == NULL) {

        printf("Error allocating memory for color-corrected image data");

        return 0;

    }

    if ((i = fread(image->data, size, 1, file)) != 1) {

        printf("Error reading image data from %s.\n", filename);

        return 0;

    }

    for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)

        temp = image->data[i];

        image->data[i] = image->data[i + 2];

        image->data[i + 2] = temp;

    }

    // we're done.

    return 1;

}

//jh: Each image is loaded into an OpenGL texture, and that's why this function is called
//    loadTexture
//    engine indicator
Image * loadTexture() {

    // allocate space for texture

    image1 = (Image *)malloc(sizeof(Image));

    if (image1 == NULL) {

        printf("Error allocating space for image");

        exit(0);

    }

    if (!ImageLoad("cel.bmp", image1)) {

        exit(1);

    }

    return image1;

}

//fola: fuel guage indicator creation
Image* loadTexture2() {

	// allocate space for texture

	image2 = (Image*)malloc(sizeof(Image));

	if (image2 == NULL) {

		printf("Error allocating space for image");

		exit(0);

	}

	if (!ImageLoad("fuel.bmp", image2)) {

		exit(1);

	}

	return image2;

}


//fola: airbag light indicator creation
Image* loadTexture3() {

	// allocate space for texture

	image3 = (Image*)malloc(sizeof(Image));

	if (image3 == NULL) {

		printf("Error allocating space for image");

		exit(0);

	}

	if (!ImageLoad("airbag.bmp", image3)) {

		exit(1);

	}

	return image3;

}


//fola: battery light indicator creation
Image* loadTexture4() {

	// allocate space for texture

	image4 = (Image*)malloc(sizeof(Image));

	if (image4 == NULL) {

		printf("Error allocating space for image");

		exit(0);

	}

	if (!ImageLoad("battery.bmp", image4)) {

		exit(1);

	}

	return image4;

}

//fola: defrost indicator creation
Image* loadTexture5() {

	// allocate space for texture

	image5 = (Image*)malloc(sizeof(Image));

	if (image5 == NULL) {

		printf("Error allocating space for image");

		exit(0);

	}

	if (!ImageLoad("defrost.bmp", image5)) {

		exit(1);

	}

	return image5;

}


//fola: door open indicator creation
Image* loadTexture6() {

	// allocate space for texture

	image6 = (Image*)malloc(sizeof(Image));

	if (image6 == NULL) {

		printf("Error allocating space for image");

		exit(0);

	}

	if (!ImageLoad("door.bmp", image6)) {

		exit(1);

	}

	return image6;

}


//fola: engine temp indicator creation
Image* loadTexture7() {

	// allocate space for texture

	image7 = (Image*)malloc(sizeof(Image));

	if (image7 == NULL) {

		printf("Error allocating space for image");

		exit(0);

	}

	if (!ImageLoad("enginetemp.bmp", image7)) {

		exit(1);

	}

	return image7;

}


//fola: antilock indicator creation
Image* loadTexture8() {

	// allocate space for texture

	image8 = (Image*)malloc(sizeof(Image));

	if (image8 == NULL) {

		printf("Error allocating space for image");

		exit(0);

	}

	if (!ImageLoad("antilock.bmp", image8)) {

		exit(1);

	}

	return image8;

}


//fola: seatblets off indicator creation
Image* loadTexture9() {

	// allocate space for texture

	image9 = (Image*)malloc(sizeof(Image));

	if (image9 == NULL) {

		printf("Error allocating space for image");

		exit(0);

	}

	if (!ImageLoad("seatbelt.bmp", image9)) {

		exit(1);

	}

	return image9;

}

//
// to draw circle, center at (x,y)
//  radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void myCircle_wire(float x, float y, float r) {
	glColor3f(1, 1, 1);
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	for (float angle = 0; angle < 360; angle += 5) {
		glVertex2f(r * cos(angle*D2R) + x, r * sin(angle*D2R) + y);
	}
	glEnd();
}
void myInnerCircle_wire(float x, float y, float r) {
	glColor3f(1, 1, 1);
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	glVertex2f((r + 2) * cos(-70 * D2R) + x, (r + 2) * sin(-70 * D2R) + y);
	glVertex2f((r + 24) * cos(-70 * D2R) + x, (r + 24) * sin(-70 * D2R) + y);
	for (float angle = 0 - 70; angle <= 320 - 70; angle += 5) {
		glVertex2f(r * cos(angle*D2R) + x, r * sin(angle*D2R) + y);
	}
	glVertex2f((r + 2) * cos(250 * D2R) + x, (r + 2) * sin(250 * D2R) + y);
	glVertex2f((r + 24) * cos(250 * D2R) + x, (r + 24) * sin(250 * D2R) + y);
	glEnd();
}
void draw_lines(float x, float y, float r) {
	glColor3f(1, 1, 1);
	glLineWidth(1);
	glBegin(GL_LINES);
	for (float angle = 0 - 70; angle <= 320 - 70; angle += 20) {
		glVertex2f(r * cos(angle*D2R) + x, r * sin(angle*D2R) + y);
		glVertex2f((r - 15) * cos(angle*D2R) + x, (r - 15) * sin(angle*D2R) + y);
	}
	glEnd();
}
void draw_nums(float x, float y, float r) {
	glLineWidth(10);
	glColor3f(1, 1, 1);
	for (float angle = 0 - 70; angle <= 320 - 70; angle += 20) {
		vprint(r * cos(angle*D2R) + x, r * sin(angle*D2R) + y, DIALFONT, "%d", -1 * ((int)angle - 250));
	}
}
void calculate_indicator(float x, float y, float r) {
	rectangle.a.x = r * cos((250 + -rectangle.angle) * D2R) + x;
	rectangle.a.y = r * sin((250 + -rectangle.angle) * D2R) + y;

	rectangle.b.x = 15 * cos((150 + -rectangle.angle) * D2R) + x;
	rectangle.b.y = 15 * sin((150 + -rectangle.angle) * D2R) + y;

	rectangle.c.x = 20 * cos((70 + -rectangle.angle) * D2R) + x;
	rectangle.c.y = 20 * sin((70 + -rectangle.angle) * D2R) + y;

	rectangle.d.x = 15 * cos((-10 + -rectangle.angle) * D2R) + x;
	rectangle.d.y = 15 * sin((-10 + -rectangle.angle) * D2R) + y;
}

void draw_indicator(float x, float y, float r) {
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(rectangle.a.x, rectangle.a.y);
	glVertex2f(rectangle.b.x, rectangle.b.y);
	glVertex2f(rectangle.c.x, rectangle.c.y);
	glVertex2f(rectangle.d.x, rectangle.d.y);
	glEnd();

	glLineWidth(3);
	glColor3f(0.5, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(rectangle.a.x, rectangle.a.y);
	glVertex2f(rectangle.b.x, rectangle.b.y);
	glVertex2f(rectangle.c.x, rectangle.c.y);
	glVertex2f(rectangle.d.x, rectangle.d.y);
	glEnd();

	glColor3f(0, 0, 0);
	circle(x, y, 8);
}
void draw_speedInfo(float x, float y) {
	glColor3f(1, 1, 0);
	glLineWidth(3);
	vprint(x + 10, y, SUMMARYFONT, "%d", rectangle.angle);
	vprint(x, y - 30, SUMMARYFONT, "KM/h");
}
void draw_speedArc(float x, float y, float r) {
	glColor3f(0, 1, 0);
	if (rectangle.angle > 260)
		glColor3f(1, 0, 0);
	glLineWidth(4);
	glBegin(GL_LINE_STRIP);
    //glBegin(GL_POINTS);
	glVertex2f((r + 16) * cos(250 * D2R) + x, (r + 16) * sin(250 * D2R) + y);
	glVertex2f((r + 4) * cos(250 * D2R) + x, (r + 4) * sin(250 * D2R) + y);
	for (float a = 0; a <= rectangle.angle; a += 5) {
		glVertex2f((r + 4) * cos((250 + -a)*D2R) + x, (r + 4) * sin((250 + -a)*D2R) + y);
	}
	glVertex2f((r + 4) * cos((250 + -rectangle.angle) * D2R) + x, (r + 4) * sin((250 + -rectangle.angle) * D2R) + y);
	glVertex2f((r + 16) * cos((250 + -rectangle.angle) * D2R) + x, (r + 16) * sin((250 + -rectangle.angle) * D2R) + y);
	glEnd();
	glBegin(GL_LINE_STRIP);
    //glBegin(GL_POINTS);
	for (float a = 0; a <= rectangle.angle; a++) {
		glVertex2f((r + 16) * cos((250 + -a)*D2R) + x, (r + 16) * sin((250 + -a)*D2R) + y);
	}
	glEnd();
}

void display_outline() {
	myCircle_wire(0, 0, 272);
	myInnerCircle_wire(0, 0, 248);
}
void display_dangerZone() {
	glColor3f(1, 0, 0);
	glLineWidth(4);
	glBegin(GL_LINE_STRIP);
	for (float angle = 320 - 30; angle <= 320 + 30; angle += 5) {
		glVertex2f(240 * cos(angle*D2R) + 0, 240 * sin(angle*D2R) + 0);
	}
	glEnd();
}
void display_speedText() {
	draw_lines(0, 0, 248);
	draw_nums(-10, 0, 200);
}
void display_indicator() {
	calculate_indicator(0, 0, 248);
	draw_indicator(0, 0, 248);
	draw_speedInfo(-20, -160);
}
void display_speedArc() {
	draw_speedArc(0, 0, 250);
}

void display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

//jh: Here's a quick demo for how we can easily move things around / scale them.
//	  OpenGL uses what's called the modelview matrix to do transform items that are
//    being drawn to the screen. Here, we're switching to the modelview matrix mode,
//    loading the identity matrix (i.e., one with no transforms), and then applying
//    a translation of (150, 0, 0) to the x, y, and z coordinates, respectively,
//    and a scaling of 0.5, 0.5, 1.0 to the x, y, and z coordinates, respectively.
//    Note that, because of the way the matrix works, the scaling is actually applied *before*
//    the translation.
//    The effect of this is that we're moving the speedometer 150 units to the right and shrinking it
//    to half size in the x and y dimensions.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(150, 0, 0);
    glScalef(0.5, 0.5, 1);

//jh: Draw the first speedometer.
	display_outline();
	display_speedText();
	display_dangerZone();
	display_indicator();
	display_speedArc();

//jh: Now set up a different translation (this time 150 units to the left) and draw a second speedometer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(-150, 0, 0);
    glScalef(0.5, 0.5, 1);

    display_outline();
    display_speedText();
    display_dangerZone();
    display_indicator();
    display_speedArc();

	//do this for every single image

//jh: Draw a check engine light. In OpenGL, to draw an icon, you actually draw a rectangle and put a texture on it.
//    So that's what we're doing here. First, though, we'll set up another translation and scaling so that we can
//    size / place the icon.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(-250, -175, 0);
    glScalef(0.5, 0.5, 1);

//jh: We need to enable texturing for this part
    glEnable(GL_TEXTURE_2D);
//jh: And tell OpenGL which texture we're using
    glBindTexture(GL_TEXTURE_2D, texture[0]);

//jh: Now we'll draw the textured square.
//    Note that we have to tell it not just where to draw the corners of the rectangle, but also what part of the texture
//    to map to each corner
    glBegin(GL_QUADS);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(-50.0, -50.0, 0.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(-50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(50.0, -50.0, 0.0);

	glEnd();


//fola: Draw a fuel guage indicator. In OpenGL, to draw an icon, you actually draw a rectangle and put a texture on it.
//    So that's what we're doing here. First, though, we'll set up another translation and scaling so that we can
//    size / place the icon.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(-180, -170, 0);
    glScalef(0.5, 0.5, 1);

//jh: We need to enable texturing for this part
    glEnable(GL_TEXTURE_2D);
//jh: And tell OpenGL which texture we're using
    glBindTexture(GL_TEXTURE_2D, texture[1]);

//jh: Now we'll draw the textured square.
//    Note that we have to tell it not just where to draw the corners of the rectangle, but also what part of the texture
//    to map to each corner
    glBegin(GL_QUADS);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(-50.0, -50.0, 0.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(-50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(50.0, -50.0, 0.0);

    glEnd();


	//airbag texture is cross referemced and is functioning for ABS instead
//fola: Draw a airbag guage indicator. In OpenGL, to draw an icon, you actually draw a rectangle and put a texture on it.
//    So that's what we're doing here. First, though, we'll set up another translation and scaling so that we can
//    size / place the icon.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(-125, -170, 0);
    glScalef(0.5, 0.5, 1);

//jh: We need to enable texturing for this part
    glEnable(GL_TEXTURE_2D);
//jh: And tell OpenGL which texture we're using
    glBindTexture(GL_TEXTURE_2D, texture[2]);

//jh: Now we'll draw the textured square.
//    Note that we have to tell it not just where to draw the corners of the rectangle, but also what part of the texture
//    to map to each corner
    glBegin(GL_QUADS);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(-50.0, -50.0, 0.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(-50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(50.0, -50.0, 0.0);

    glEnd();



//fola: Draw a battery guage indicator. In OpenGL, to draw an icon, you actually draw a rectangle and put a texture on it.
//    So that's what we're doing here. First, though, we'll set up another translation and scaling so that we can
//    size / place the icon.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(-70, -170, 0);
    glScalef(0.5, 0.5, 1);

//jh: We need to enable texturing for this part
    glEnable(GL_TEXTURE_2D);
//jh: And tell OpenGL which texture we're using
    glBindTexture(GL_TEXTURE_2D, texture[3]);

//jh: Now we'll draw the textured square.
//    Note that we have to tell it not just where to draw the corners of the rectangle, but also what part of the texture
//    to map to each corner
    glBegin(GL_QUADS);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(-50.0, -50.0, 0.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(-50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(50.0, -50.0, 0.0);

    glEnd();


//fola: Draw a defrost indicator. In OpenGL, to draw an icon, you actually draw a rectangle and put a texture on it.
//    So that's what we're doing here. First, though, we'll set up another translation and scaling so that we can
//    size / place the icon.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(75, -170, 0);
    glScalef(0.5, 0.5, 1);

//jh: We need to enable texturing for this part
    glEnable(GL_TEXTURE_2D);
//jh: And tell OpenGL which texture we're using
    glBindTexture(GL_TEXTURE_2D, texture[4]);

//jh: Now we'll draw the textured square.
//    Note that we have to tell it not just where to draw the corners of the rectangle, but also what part of the texture
//    to map to each corner
    glBegin(GL_QUADS);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(-50.0, -50.0, 0.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(-50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(50.0, -50.0, 0.0);

    glEnd();



//fola: Draw a door indicator. In OpenGL, to draw an icon, you actually draw a rectangle and put a texture on it.
//    So that's what we're doing here. First, though, we'll set up another translation and scaling so that we can
//    size / place the icon.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(130, -173, 0);
    glScalef(0.5, 0.5, 1);

//jh: We need to enable texturing for this part
    glEnable(GL_TEXTURE_2D);
//jh: And tell OpenGL which texture we're using
    glBindTexture(GL_TEXTURE_2D, texture[5]);

//jh: Now we'll draw the textured square.
//    Note that we have to tell it not just where to draw the corners of the rectangle, but also what part of the texture
//    to map to each corner
    glBegin(GL_QUADS);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(-50.0, -50.0, 0.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(-50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(50.0, -50.0, 0.0);

    glEnd();


//fola: Draw an engine temp indicator. In OpenGL, to draw an icon, you actually draw a rectangle and put a texture on it.
//    So that's what we're doing here. First, though, we'll set up another translation and scaling so that we can
//    size / place the icon.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(175, -170, 0);
    glScalef(0.5, 0.5, 1);

//jh: We need to enable texturing for this part
    glEnable(GL_TEXTURE_2D);
//jh: And tell OpenGL which texture we're using
    glBindTexture(GL_TEXTURE_2D, texture[6]);

//jh: Now we'll draw the textured square.
//    Note that we have to tell it not just where to draw the corners of the rectangle, but also what part of the texture
//    to map to each corner
    glBegin(GL_QUADS);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(-50.0, -50.0, 0.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(-50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(50.0, -50.0, 0.0);

    glEnd();


//fola: Draw an antilock indicator. In OpenGL, to draw an icon, you actually draw a rectangle and put a texture on it.
//    So that's what we're doing here. First, though, we'll set up another translation and scaling so that we can
//    size / place the icon.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(225, -170, 0);
    glScalef(0.5, 0.5, 1);

//jh: We need to enable texturing for this part
    glEnable(GL_TEXTURE_2D);
//jh: And tell OpenGL which texture we're using
    glBindTexture(GL_TEXTURE_2D, texture[7]);

//jh: Now we'll draw the textured square.
//    Note that we have to tell it not just where to draw the corners of the rectangle, but also what part of the texture
//    to map to each corner
    glBegin(GL_QUADS);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(-50.0, -50.0, 0.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(-50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(50.0, -50.0, 0.0);

    glEnd();



//fola: Draw a seatbelt indicator. In OpenGL, to draw an icon, you actually draw a rectangle and put a texture on it.
//    So that's what we're doing here. First, though, we'll set up another translation and scaling so that we can
//    size / place the icon.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(203, 0, 0);
    glScalef(0.5, 0.5, 1);

//jh: We need to enable texturing for this part
    glEnable(GL_TEXTURE_2D);
//jh: And tell OpenGL which texture we're using
    glBindTexture(GL_TEXTURE_2D, texture[8]);

//jh: Now we'll draw the textured square.
//    Note that we have to tell it not just where to draw the corners of the rectangle, but also what part of the texture
//    to map to each corner
    glBegin(GL_QUADS);

    glTexCoord2f(1.0, 0.0);

    glVertex3f(-50.0, -50.0, 0.0);

    glTexCoord2f(1.0, 1.0);

    glVertex3f(-50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 1.0);

    glVertex3f(50.0, 50.0, 0.0);

    glTexCoord2f(0.0, 0.0);

    glVertex3f(50.0, -50.0, 0.0);

    glEnd();



//jh: Now turn texturing back off, because everything else we draw won't use textures
    glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;

	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;

	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// to refresh the window it calls display() function	
	glutPostRedisplay();
}

void onMove(int x, int y) {
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	if (up == true && rectangle.angle >= 0 && (rectangle.angle <= 312 || rectangle.angle <= 316)) {
		if (rectangle.angle == 316)
			rectangle.angle = rectangle.angle + 4;
		else
			rectangle.angle = rectangle.angle + 8;
	}
	else if (up == false && rectangle.angle >= 4 && rectangle.angle <= 320)
		rectangle.angle = rectangle.angle - 4;

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//jh: Load up the texture for the icon(s) into the image variable
    loadTexture();
	loadTexture2();
	loadTexture3();
	loadTexture4();
	loadTexture5();
	loadTexture6();
	loadTexture7();
	loadTexture8();
	loadTexture9();

//jh: Tell OpenGL we want to create two textures. It will store the indices of
//    these two textures into our texture array.
    glGenTextures(10, texture);

//jh: Tell OpenGL we want to select the first texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
	

//jh: Set up some parameters for how the texture will be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

//jh: Put the image data into the texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,

        GL_RGB, GL_UNSIGNED_BYTE, image1->data);



	//second texture, fuel indicator

//fola: Tell OpenGL we want to select the second texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);
	

//jh: Set up some parameters for how the texture will be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

//fola: Put fuel indicator image data in texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0,

		GL_RGB, GL_UNSIGNED_BYTE, image2->data);



	//third texture, airbag indicator

//fola: Tell OpenGL we want to select the second texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);
	

//jh: Set up some parameters for how the texture will be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

//fola: Put airbag indicator image data in texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0,

		GL_RGB, GL_UNSIGNED_BYTE, image3->data);


	//fourth texture, battery indicator

//fola: Tell OpenGL we want to select the second texture
    glBindTexture(GL_TEXTURE_2D, texture[3]);
	

//jh: Set up some parameters for how the texture will be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

//fola: Put battery indicator image data in texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image4->sizeX, image4->sizeY, 0,

		GL_RGB, GL_UNSIGNED_BYTE, image4->data);


	//fifth texture, defrost indicator

//fola: Tell OpenGL we want to select the second texture
    glBindTexture(GL_TEXTURE_2D, texture[4]);
	

//jh: Set up some parameters for how the texture will be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

//fola: Put defrost indicator image data in texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image5->sizeX, image5->sizeY, 0,

		GL_RGB, GL_UNSIGNED_BYTE, image5->data);


	//sixth texture, door indicator
//fola: Tell OpenGL we want to select the second texture
    glBindTexture(GL_TEXTURE_2D, texture[5]);
	

//jh: Set up some parameters for how the texture will be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

//fola: Put door indicator image data in texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image6->sizeX, image6->sizeY, 0,

		GL_RGB, GL_UNSIGNED_BYTE, image6->data);


	//seventh texture, enginetemp indicator
//fola: Tell OpenGL we want to select the second texture
    glBindTexture(GL_TEXTURE_2D, texture[6]);
	

//jh: Set up some parameters for how the texture will be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

//fola: Put enginetemp indicator image data in texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image7->sizeX, image7->sizeY, 0,

		GL_RGB, GL_UNSIGNED_BYTE, image7->data);


	//eigth texture, antilock indicator
//fola: Tell OpenGL we want to select the second texture
    glBindTexture(GL_TEXTURE_2D, texture[7]);
	

//jh: Set up some parameters for how the texture will be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

//fola: Put antilock indicator image data in texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image8->sizeX, image8->sizeY, 0,

		GL_RGB, GL_UNSIGNED_BYTE, image8->data);


	//ninth texture, seatbelt indicator
//fola: Tell OpenGL we want to select the second texture
    glBindTexture(GL_TEXTURE_2D, texture[8]);
	

//jh: Set up some parameters for how the texture will be displayed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture

//fola: Put seatbelt indicator image data in texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image9->sizeX, image9->sizeY, 0,

		GL_RGB, GL_UNSIGNED_BYTE, image9->data);


//jh: Select the decal texture mode. This makes it so that the texture is overlayed like a sticker / decal (instead of being blended with the object color)
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glShadeModel(GL_FLAT);

}

int main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("GRADING");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);
	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}