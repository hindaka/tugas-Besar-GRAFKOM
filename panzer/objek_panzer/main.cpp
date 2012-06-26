#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include <math.h>

GLuint texture[1];

static int putarx=0 ,putary=0, spin=0;

struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image; //struktur data untuk image texture

//mengambil gambar BMP
int ImageLoad(char *filename, Image *image) {
	FILE *file;
	unsigned long size; // ukuran image dalam bytes
	unsigned long i; // standard counter.
	unsigned short int plane; // number of planes in image

	unsigned short int bpp; // jumlah bits per pixel
	char temp; // temporary color storage for var warna sementara untuk memastikan filenya ada


	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}
	// mencari file header bmp
	fseek(file, 18, SEEK_CUR);
	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
	//printf("Width of %s: %lu\n", filename, image->sizeX);
	// membaca nilai height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	//printf("Height of %s: %lu\n", filename, image->sizeY);
	//menghitung ukuran image(asumsi 24 bits or 3 bytes per pixel).

	size = image->sizeX * image->sizeY * 3;
	// read the planes
	if ((fread(&plane, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (plane != 1) {
		printf("Planes from %s is not 1: %u\n", filename, plane);
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
	image->data = (char *) malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i < size; i += 3) { // membalikan semuan nilai warna (gbr - > rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	// we're done.
	return 1;
}

//mengambil tekstur panzer
Image * loadTexture() {
	Image *image1;
	// alokasi memmory untuk tekstur
	image1 = (Image *) malloc(sizeof(Image));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	//pic.bmp is a 64x64 picture
	if (!ImageLoad("kulit panser.bmp", image1)) {
		exit(1);
	}
	return image1;
}

void myinit(void) {
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Image *image1 = loadTexture();

	if (image1 == NULL) {
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate texture/ membuat texture
	glGenTextures(1, texture);

	//binding texture untuk membuat texture 2D
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	//menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //
	//menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //

	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB,
			GL_UNSIGNED_BYTE, image1->data);

	glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_FLAT);
}

void panzer()
{
    glPushMatrix();
    glRotatef((GLfloat)putarx,1.0,0.0,0.0);
    glRotatef((GLfloat)putary,0.0,1.0,0.0);
//============== BEGIN BODY PANZER=====================//
    //bagian bawah
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.0f,0.0f);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(1.0,0.0,0.0);
        glVertex3f(1.0,0.0,-2.5);
        glVertex3f(0.0,0.0,-2.5);
    glEnd();

    //awal  depan bagian setengah ke atas
    //kiri
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.6);
        glTexCoord2f(1.0,0.0);glVertex3f(-0.3,0.5,0.0);
        glTexCoord2f(1.0,1.0);glVertex3f(-0.3,0.5,-1.0);
        glTexCoord2f(0.0,1.0);glVertex3f(-0.15,0.8,-1.0);
        glTexCoord2f(0.0,0.0);glVertex3f(-0.15,0.8,-0.105);
        glVertex3f(-0.2,0.6,0.01);
    glEnd();

    //kanan
    glBegin(GL_POLYGON);
        glTexCoord2f(1.0,0.0);glVertex3f(1.3,0.5,0.0);
        glTexCoord2f(1.0,1.0);glVertex3f(1.3,0.5,-1.0);
        glTexCoord2f(0.0,1.0);glVertex3f(1.15,0.8,-1.0);
        glTexCoord2f(0.0,0.0);glVertex3f(1.15,0.8,-0.105);
        glVertex3f(1.2,0.6,0.01);
    glEnd();

    //atap
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
        glTexCoord2f(1.0,0.0);glVertex3f(1.15,0.8,-0.105);
        glTexCoord2f(1.0,1.0);glVertex3f(1.15,0.8,-1.0);
        glTexCoord2f(0.0,1.0);glVertex3f(-0.15,0.8,-1.0);
        glTexCoord2f(0.0,0.0);glVertex3f(-0.15,0.8,-0.105);
        glVertex3f(0.5,0.8,0.0);
    glEnd();

    //link tengah
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.6);
        glTexCoord3f(1.3,0.5,-1.0);glVertex3f(1.3,0.5,-1.0);
        glTexCoord3f(1.2,0.9,-1.1);glVertex3f(1.2,0.9,-1.1);
        glTexCoord3f(1.15,0.8,-1.0);glVertex3f(1.15,0.8,-1.0);
    glEnd();

    glBegin(GL_POLYGON);
        glTexCoord3f(-0.3,0.5,-1.0);glVertex3f(-0.3,0.5,-1.0);
        glTexCoord3f(-0.2,0.9,-1.1);glVertex3f(-0.2,0.9,-1.1);
        glTexCoord3f(-0.15,0.8,-1.0);glVertex3f(-0.15,0.8,-1.0);
    glEnd();

    glBegin(GL_POLYGON);
        glTexCoord3f(1.15,0.8,-1.0);glVertex3f(1.15,0.8,-1.0);
        glTexCoord3f(1.2,0.9,-1.1);glVertex3f(1.2,0.9,-1.1);
        glTexCoord3f(-0.2,0.9,-1.1);glVertex3f(-0.2,0.9,-1.1);
        glTexCoord3f(-0.15,0.8,-1.0);glVertex3f(-0.15,0.8,-1.0);
    glEnd();

    //atas setengah kebelakang
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.6);
        glTexCoord3f(1.2,0.9,-2.6);glVertex3f(1.2,0.9,-2.6);
        glTexCoord3f(1.3,0.5,-2.5);glVertex3f(1.3,0.5,-2.5);
        glTexCoord3f(1.0,0.5,-2.5);glVertex3f(1.0,0.5,-2.5);
        glTexCoord3f(1.0,0.0,-2.5);glVertex3f(1.0,0.0,-2.5);
        glTexCoord3f(0.0,0.0,-2.5);glVertex3f(0.0,0.0,-2.5);
        glTexCoord3f(-0.1,0.5,-2.5);glVertex3f(-0.1,0.5,-2.5);
        glTexCoord3f(-0.3,0.5,-2.5);glVertex3f(-0.3,0.5,-2.5);
        glTexCoord3f(-0.2,0.9,-2.6);glVertex3f(-0.2,0.9,-2.6);
    glEnd();

    //frame pintu belakang
    glBegin(GL_POLYGON);
    glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(0.9,0.85,-2.61);
        glVertex3f(0.1,0.85,-2.61);
        glVertex3f(0.1,0.05,-2.61);
        glVertex3f(0.9,0.05,-2.61);
    glEnd();

    //pintu belakang
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.1);
        glVertex3f(0.7,0.82,-2.615);
        glVertex3f(0.15,0.82,-2.615);
        glVertex3f(0.15,0.07,-2.615);
        glVertex3f(0.7,0.07,-2.615);
    glEnd();
    //badan naik
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);glVertex3f(1.2,0.9,-1.1);
        glTexCoord2f(1.0,0.0);glVertex3f(-0.2,0.9,-1.1);
        glTexCoord2f(1.0,1.0);glVertex3f(-0.2,0.9,-2.6);
        glTexCoord2f(0.0,1.0);glVertex3f(1.2,0.9,-2.6);
    glEnd();

    //samping kiri
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);glVertex3f(1.3,0.5,-2.5);
        glTexCoord2f(1.0,0.0);glVertex3f(1.2,0.9,-2.6);
        glTexCoord2f(1.0,1.0);glVertex3f(1.2,0.9,-1.1);
        glTexCoord2f(0.0,1.0);glVertex3f(1.3,0.5,-1.0);
    glEnd();

    //samping kanan
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);glVertex3f(-0.3,0.5,-2.5);
        glTexCoord2f(1.0,0.0);glVertex3f(-0.2,0.9,-2.6);
        glTexCoord2f(1.0,1.0);glVertex3f(-0.2,0.9,-1.1);
        glTexCoord2f(0.0,1.0);glVertex3f(-0.3,0.5,-1.0);
    glEnd();

    //awal bemper depan atas
    //kanan atas I
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.6);
        glTexCoord3f(-0.3,0.5,0.0);glVertex3f(-0.3,0.5,0.0);
        glTexCoord3f(-0.2,0.6,0.01);glVertex3f(-0.2,0.6,0.01);
        glTexCoord3f(-0.105,0.55,0.3);glVertex3f(-0.105,0.55,0.3);
        glTexCoord3f(-0.15,0.5,0.3);glVertex3f(-0.15,0.5,0.3);
    glEnd();

    //kanan atas II
    glBegin(GL_POLYGON);
        glTexCoord3f(-0.15,0.5,0.3);glVertex3f(-0.15,0.5,0.3);
        glTexCoord3f(0.1,0.5,0.5);glVertex3f(0.1,0.5,0.5);
        glTexCoord3f(0.15,0.52,0.48);glVertex3f(0.15,0.52,0.48);
        glTexCoord3f(-0.105,0.55,0.3);glVertex3f(-0.105,0.55,0.3);
    glEnd();

    //kiri atas I
    glBegin(GL_POLYGON);
        glTexCoord3f(1.3,0.5,0.0);glVertex3f(1.3,0.5,0.0);
        glTexCoord3f(1.15,0.5,0.3);glVertex3f(1.15,0.5,0.3);
        glTexCoord3f(1.1,0.55,0.3);glVertex3f(1.1,0.55,0.3);
        glTexCoord3f(1.2,0.6,0.01);glVertex3f(1.2,0.6,0.01);
    glEnd();

    //kiri atas II
    glBegin(GL_POLYGON);
        glTexCoord3f(1.15,0.5,0.3);glVertex3f(1.15,0.5,0.3);
        glTexCoord3f(0.9,0.5,0.5);glVertex3f(0.9,0.5,0.5);
        glTexCoord3f(0.85,0.52,0.48);glVertex3f(0.85,0.52,0.48);
        glTexCoord3f(1.1,0.55,0.3);glVertex3f(1.1,0.55,0.3);
    glEnd();

    //moncong
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
        glTexCoord3f(0.9,0.5,0.5);glVertex3f(0.9,0.5,0.5);
        glTexCoord3f(0.85,0.52,0.48);glVertex3f(0.85,0.52,0.48);
        glTexCoord3f(0.15,0.52,0.48);glVertex3f(0.15,0.52,0.48);
        glTexCoord3f(0.1,0.5,0.5);glVertex3f(0.1,0.5,0.5);
        glTexCoord3f(0.2,0.45,0.5);glVertex3f(0.2,0.45,0.5);
        glTexCoord3f(0.8,0.45,0.5);glVertex3f(0.8,0.45,0.5);
    glEnd();

    //atap moncong
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
        glTexCoord3f(0.85,0.52,0.48);glVertex3f(0.85,0.52,0.48);
        glTexCoord3f(0.15,0.52,0.48);glVertex3f(0.15,0.52,0.48);
        glTexCoord3f(-0.1,0.55,0.3);glVertex3f(-0.1,0.55,0.3);
        glTexCoord3f(-0.2,0.6,0.0);glVertex3f(-0.2,0.6,0.0);
        glTexCoord3f(0.5,0.6,0.1);glVertex3f(0.5,0.6,0.1);
        glTexCoord3f(1.2,0.6,0.0);glVertex3f(1.2,0.6,0.0);
        glTexCoord3f(1.1,0.55,0.3);glVertex3f(1.1,0.55,0.3);
    glEnd();

    //kaca depan full
    // frame kiri
    glBegin(GL_POLYGON);
        glTexCoord3f(1.2,0.6,0.0);glVertex3f(1.2,0.6,0.0);
        glTexCoord3f(0.5,0.6,0.1);glVertex3f(0.5,0.6,0.1);
        glTexCoord3f(0.5,0.8,0.0);glVertex3f(0.5,0.8,0.0);
        glTexCoord3f(1.15,0.8,-0.1);glVertex3f(1.15,0.8,-0.1);
    glEnd();

    //kaca depan I
    glBegin(GL_POLYGON);
    glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(-0.15,0.65,-0.01);
        glVertex3f(0.3,0.63,0.06);
        glVertex3f(0.3,0.75,0.0);
        glVertex3f(-0.1,0.75,-0.06);
    glEnd();

    // frame kanan
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.6);
        glTexCoord3f(-0.2,0.6,0.0);glVertex3f(-0.2,0.6,0.0);
        glTexCoord3f(0.5,0.6,0.1);glVertex3f(0.5,0.6,0.1);
        glTexCoord3f(0.5,0.8,0.0);glVertex3f(0.5,0.8,0.0);
        glTexCoord3f(-0.15,0.8,-0.1);glVertex3f(-0.15,0.8,-0.1);
    glEnd();

    //kaca depan II
    glBegin(GL_POLYGON);
    glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(1.15,0.65,-0.01);
        glVertex3f(0.7,0.63,0.06);
        glVertex3f(0.7,0.75,0.0);
        glVertex3f(1.1,0.75,-0.06);
    glEnd();

    //awal bemper depan bawah
    //bagian depan
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.6);
        glTexCoord3f(1.1,0.0,0.0);glVertex3f(1.1,0.0,0.0);
        glTexCoord3f(1.05,0.0,0.1);glVertex3f(1.05,0.0,0.1);
        glTexCoord3f(-0.05,0.0,0.1);glVertex3f(-0.05,0.0,0.1);
        glTexCoord3f(-0.1,0.0,0.0);glVertex3f(-0.1,0.0,0.0);
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord3f(1.05,0.0,0.1);glVertex3f(1.05,0.0,0.1);
        glTexCoord3f(0.8,0.45,0.5);glVertex3f(0.8,0.45,0.5);
        glTexCoord3f(0.2,0.45,0.5);glVertex3f(0.2,0.45,0.5);
        glTexCoord3f(-0.05,0.0,0.1);glVertex3f(-0.05,0.0,0.1);
    glEnd();

    //samping kiri
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.6);
        glTexCoord3f(1.0,0.0,0.0);glVertex3f(1.0,0.0,0.0);
        glTexCoord3f(1.1,0.0,0.0);glVertex3f(1.1,0.0,0.0);
        glTexCoord3f(1.3,0.5,0.0);glVertex3f(1.3,0.5,0.0);
        glTexCoord3f(1.0,0.5,0.0);glVertex3f(1.0,0.5,0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glTexCoord3f(1.1,0.0,0.0);glVertex3f(1.1,0.0,0.0);
        glTexCoord3f(1.05,0.0,0.1);glVertex3f(1.05,0.0,0.1);
        glTexCoord3f(1.15,0.5,0.3);glVertex3f(1.15,0.5,0.3);
        glTexCoord3f(1.3,0.5,0.0);glVertex3f(1.3,0.5,0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glTexCoord3f(1.05,0.0,0.1);glVertex3f(1.05,0.0,0.1);
        glTexCoord3f(0.8,0.45,0.5);glVertex3f(0.8,0.45,0.5);
        glTexCoord3f(0.9,0.5,0.5);glVertex3f(0.9,0.5,0.5);
        glTexCoord3f(1.15,0.5,0.3);glVertex3f(1.15,0.5,0.3);
    glEnd();

    //samping kanan
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_POLYGON);
    glColor3f(0.4,0.5,0.6);
        glTexCoord3f(0.0,0.0,0.0);glVertex3f(0.0,0.0,0.0);
        glTexCoord3f(-0.1,0.0,0.0);glVertex3f(-0.1,0.0,0.0);
        glTexCoord3f(-0.3,0.5,0.0);glVertex3f(-0.3,0.5,0.0);
        glTexCoord3f(0.0,0.5,0.0);glVertex3f(0.0,0.5,0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glTexCoord3f(-0.1,0.0,0.0);glVertex3f(-0.1,0.0,0.0);
        glTexCoord3f(-0.05,0.0,0.1);glVertex3f(-0.05,0.0,0.1);
        glTexCoord3f(-0.15,0.5,0.3);glVertex3f(-0.15,0.5,0.3);
        glTexCoord3f(-0.3,0.5,0.0);glVertex3f(-0.3,0.5,0.0);
    glEnd();

    glBegin(GL_POLYGON);
        glTexCoord3f(-0.05,0.0,0.1);glVertex3f(-0.05,0.0,0.1);
        glTexCoord3f(0.2,0.45,0.5);glVertex3f(0.2,0.45,0.5);
        glTexCoord3f(0.1,0.5,0.5);glVertex3f(0.1,0.5,0.5);
        glTexCoord3f(-0.15,0.5,0.3);glVertex3f(-0.15,0.5,0.3);
    glEnd();


    //bagian sayap
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);glVertex3f(-0.3,0.5,0.0);
        glTexCoord2f(1.0,0.0);glVertex3f(1.3,0.5,0.0);
        glTexCoord2f(1.0,1.0);glVertex3f(1.3,0.5,-2.5);
        glTexCoord2f(0.0,1.0);glVertex3f(-0.3,0.5,-2.5);
    glEnd();

    //akhir bemper depan


    //bagian belakang
    /*glBegin(GL_POLYGON);
    glColor3f(0.2,0.2,0.2);
        glVertex3f(0.0,0.0,-2.5);
        glVertex3f(1.0,0.0,-2.5);
        glVertex3f(1.0,0.5,-2.5);
        glVertex3f(0.0,0.5,-2.5);
    glEnd();*/

    //bagian body bawah kanan
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);glVertex3f(0.0,0.0,0.0);
        glTexCoord2f(1.0,0.0);glVertex3f(0.0,0.5,0.0);
        glTexCoord2f(1.0,1.0);glVertex3f(0.0,0.5,-2.5);
        glTexCoord2f(0.0,1.0);glVertex3f(0.0,0.0,-2.5);
    glEnd();

    //bagian body bawah kiri
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0);glVertex3f(1.0,0.0,0.0);
        glTexCoord2f(1.0,0.0);glVertex3f(1.0,0.5,0.0);
        glTexCoord2f(1.0,1.0);glVertex3f(1.0,0.5,-2.5);
        glTexCoord2f(0.0,1.0);glVertex3f(1.0,0.0,-2.5);
    glEnd();
//============== END BODY PANZER=====================//
//============== BEGIN RODA PANZER=====================//
    //RODA Kiri FULL
        //ban kiri_1
        glPushMatrix();
        glTranslatef(1.0,0.0,-0.4);
        glScalef(0.3,0.5,0.5);
        glRotatef(90,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

        //ban kiri_2
        glPushMatrix();
        glTranslatef(1.0,0.0,-1.28);
        glScalef(0.3,0.5,0.5);
        glRotatef(90,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

        //ban kiri_3
        glPushMatrix();
        glTranslatef(1.0,0.0,-2.1);
        glScalef(0.3,0.5,0.5);
        glRotatef(90,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

    //ban dalam kiri_1
        glPushMatrix();
        glTranslatef(1.0,0.0,-0.4);
        glScalef(0.3,0.5,0.5);
        glRotatef(270,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

        //ban dalam kiri_2
        glPushMatrix();
        glTranslatef(1.0,0.0,-1.28);
        glScalef(0.3,0.5,0.5);
        glRotatef(270,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

        //ban dalam kiri_3
        glPushMatrix();
        glTranslatef(1.0,0.0,-2.1);
        glScalef(0.3,0.5,0.5);
        glRotatef(270,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

    //END BAN KIRI FULL
    //BEGIN BAN KANAN FULL
        //ban kiri_1
        glPushMatrix();
        glTranslatef(0.0,0.0,-0.4);
        glScalef(0.3,0.5,0.5);
        glRotatef(270,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

        //ban kiri_2
        glPushMatrix();
        glTranslatef(0.0,0.0,-1.28);
        glScalef(0.3,0.5,0.5);
        glRotatef(270,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

        //ban kiri_3
        glPushMatrix();
        glTranslatef(0.0,0.0,-2.1);
        glScalef(0.3,0.5,0.5);
        glRotatef(270,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

    //ban dalam kiri_1
        glPushMatrix();
        glTranslatef(0.0,0.0,-0.4);
        glScalef(0.3,0.5,0.5);
        glRotatef(90,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

        //ban dalam kiri_2
        glPushMatrix();
        glTranslatef(0.0,0.0,-1.28);
        glScalef(0.3,0.5,0.5);
        glRotatef(90,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

        //ban dalam kanan_3
        glPushMatrix();
        glTranslatef(0.0,0.0,-2.1);
        glScalef(0.3,0.5,0.5);
        glRotatef(90,0.0,1.0,0.0);
        glColor3f(0.0f,0.0f,0.0f);
            glutSolidTorus(0.5,0.3,20,20);

           //pelk
            glPushMatrix();
                glTranslatef(0.0,0.0,0.5);
                glScalef(0.5,0.5,0.5);
                glColor3f(0.1f,0.1f,0.1f);
                glutSolidCone(1.0,0.1,20,15);
            glPopMatrix();

        glPopMatrix();

    //END BAN KIRI FULL
//============== END RODA PANZER=====================//
    glPopMatrix();
    glutSwapBuffers();
}

void mykey(int key, int x, int y){
switch(key){
    case GLUT_KEY_HOME      : putarx = (putarx + 5) % 360;
                              glutPostRedisplay();
                              break;
    case GLUT_KEY_END       : putarx = (putarx - 5) % 360;
                              glutPostRedisplay();
                              break;
    case GLUT_KEY_PAGE_UP   : putary = (putary + 5) % 360;
                              glutPostRedisplay();
                              break;
    case GLUT_KEY_PAGE_DOWN : putary = (putary - 5) % 360;
                              glutPostRedisplay();
                              break;
    default             : break;
    }
}

void spinDisplay(void){
    spin=spin+1.0;
    if (spin > 360.0);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y){
switch(button){
    case GLUT_LEFT_BUTTON : if (state == GLUT_DOWN)
                                glutIdleFunc(spinDisplay);
                                break;
    case GLUT_RIGHT_BUTTON : if(state == GLUT_DOWN)
                                glutIdleFunc(NULL);
                                break;
    default                 : break;
    }
}

void setMaterial()
{
    //set properties of surfaces material
    GLfloat mat_ambient[] = {0.7f,0.7f,0.7f,1.0f};
    GLfloat mat_diffuse[] = {0.6f,0.6f,0.6f,1.0f};
    GLfloat mat_specular[] = {1.0f,1.0f,1.0f,1.0f};
    GLfloat mat_shininess[] = {30.0f};
    glColorMaterial(GL_FRONT_AND_BACK,GL_EMISSION);
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}


void setLighting()
{
    //set light sources
    GLfloat lightIntensity[] = {0.5f,0.5f,0.5f,1.0f};//mensetting pencahayaan
    GLfloat light_position[] = {3.0f,2.0f,6.0f,0.0f};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightIntensity);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
}


void setViewport()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double winHt = 1.0;//half height of the window
    glOrtho(-winHt*64/48,winHt*64/48,-winHt,winHt,0.1,100.0);
}


void setCamera()
{
    //set the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(50.0,28.0,70.0,1.0,1.0,1.0,0.0,1.0,0.0);
}


void displayObject()
{
    setMaterial();
    setLighting();
    setViewport();
    setCamera();
    myinit();
    //startDrawing
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    panzer();//memanggil fungsi menggambar objek sate
    glFlush();//mengirim semua objek untuk dirender
}


int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(250,200);
    glutCreateWindow("bangun 3D");
    glutDisplayFunc(displayObject);//fungsi dari display object yang menggabungkan objek sate lighting material dan kamera
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glutSpecialFunc(mykey);
    //glutMouseFunc(mouse);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glViewport(0,0,640,480);
    glutMainLoop();
}
