//////////////////////////////////////////////////////
// Lift 3D dengan Kontrol Intensitas Cahaya Sederhana
//////////////////////////////////////////////////////

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath> // Untuk fungsi matematika seperti fmaxf

// Globals Lift
static float doorOffset = 0.0f;
static float maxDoorOpen = 0.8f;

// Globals Kontrol Cahaya yang Ditinggalkan
static float lightIntensity = 1.0f;
static float ambientIntensity = 0.2f;

// --- FUNGSI TAMBAHAN UNTUK INTERAKSI ---
void printLightControls() {
    std::cout << "\n=== KONTROL CAHAYA ===" << std::endl;
    std::cout << "I/K - Naikkan/Turunkan Intensitas Cahaya (Light Intensity)" << std::endl;
    std::cout << "A/S - Naikkan/Turunkan Cahaya Sekitar (Ambient Intensity)" << std::endl;
    std::cout << "R   - Reset pengaturan cahaya" << std::endl;
    std::cout << "H   - Tampilkan Bantuan ini" << std::endl;
    std::cout << "======================\n" << std::endl;
}

// --- FUNGSI UTAMA OPENGL ---

void setupLighting() {
    glEnable(GL_DEPTH_TEST);
    // Kita tambahkan juga properti Specular default yang rendah
    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat mat_shininess[] = { 10.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

// Fungsi drawCube versi lama (tetap digunakan untuk mengatur Diffuse)
void drawCube(float x, float y, float z, float r, float g, float b) {
    GLfloat mat_diffuse[] = { r, g, b, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    glPushMatrix();
    glScalef(x, y, z);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera dari kanan atas agak miring
    gluLookAt(4.0, 2.5, 7.0,
        0.0, 0.0, -1.0,
        0.0, 1.0, 0.0);

    // ======================================
    // 1. PENGATURAN CAHAYA DENGAN VARIABEL GLOBAL
    // ======================================
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Cahaya Diffuse dan Specular (Putih, dikalikan dengan Light Intensity)
    GLfloat light_diffuse[] = { lightIntensity, lightIntensity, lightIntensity, 1.0f };
    GLfloat light_specular[] = { lightIntensity, lightIntensity, lightIntensity, 1.0f };

    // Cahaya Ambient (Cahaya merata)
    GLfloat light_ambient[] = { ambientIntensity, ambientIntensity, ambientIntensity, 1.0f };

    // Posisi Cahaya (Di atas dan di depan lift)
    GLfloat lightPos[] = { 4.0f, 5.0f, 8.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Attenuation diatur ke konstan (tidak ada pelemahan jarak)
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);

    // ======================================
    // 2. DRAWING OBJEK (DENGAN glMaterialfv)
    // ======================================

    // ====== RUANGAN LUAR ======
    GLfloat outerWall[] = { 0.2f, 0.4f, 0.5f, 1.0f };
    GLfloat outerFloor[] = { 0.3f, 0.35f, 0.4f, 1.0f };
    GLfloat outerCeiling[] = { 0.7f, 0.8f, 0.8f, 1.0f };

    // Lantai luar
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, outerFloor);
    glPushMatrix();
    glTranslatef(0.0f, -1.6f, 1.0f);
    glScalef(6.0f, 0.1f, 6.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Dinding kiri luar
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, outerWall);
    glPushMatrix();
    glTranslatef(-3.0f, 0.0f, 1.0f);
    glScalef(0.1f, 3.2f, 6.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Dinding kanan luar
    glPushMatrix();
    glTranslatef(3.0f, 0.0f, 1.0f);
    glScalef(0.1f, 3.2f, 6.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Dinding belakang luar (belakang lift)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -3.0f);
    glScalef(6.0f, 3.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Atap ruangan
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, outerCeiling);
    glPushMatrix();
    glTranslatef(0.0f, 1.6f, 1.0f);
    glScalef(6.0f, 0.1f, 6.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ====== BAGIAN DALAM LIFT ======
    GLfloat innerCream[] = { 1.0f, 0.85f, 0.65f, 1.0f };
    GLfloat darkerCream[] = { 0.9f, 0.7f, 0.55f, 1.0f };

    // Lantai lift
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, darkerCream);
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, -1.0f);
    glScalef(2.0f, 0.05f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Dinding kiri lift
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, innerCream);
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, -1.0f);
    glScalef(0.05f, 3.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Dinding kanan lift
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, innerCream);
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, -1.0f);
    glScalef(0.05f, 3.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Dinding belakang lift
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, innerCream);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -2.0f);
    glScalef(2.0f, 3.0f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Plafon lift
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, innerCream);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -1.0f);
    glScalef(2.0f, 0.05f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ====== BINGKAI DEPAN LIFT ======
    GLfloat outerGray[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, outerGray);

    // Atas bingkai
    glPushMatrix();
    glTranslatef(0.0f, 1.55f, 0.0f);
    glScalef(2.0f, 0.1f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Bawah bingkai
    glPushMatrix();
    glTranslatef(0.0f, -1.55f, 0.0f);
    glScalef(2.0f, 0.1f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Kiri bingkai
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, 0.0f);
    glScalef(0.1f, 3.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Kanan bingkai
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glScalef(0.1f, 3.0f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ====== PINTU LIFT ======
    GLfloat doorGray[] = { 0.1f, 0.2f, 0.4f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, doorGray);

    // Pintu kiri
    glPushMatrix();
    glTranslatef(-0.5f - doorOffset, 0.0f, 0.1f);
    glScalef(0.9f, 2.7f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pintu kanan
    glPushMatrix();
    glTranslatef(0.5f + doorOffset, 0.0f, 0.1f);
    glScalef(0.9f, 2.7f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glutSwapBuffers();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
}

void specialKeyInput(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT && doorOffset < maxDoorOpen)
        doorOffset += 0.05f;
    if (key == GLUT_KEY_LEFT && doorOffset > 0.0f)
        doorOffset -= 0.05f;
    glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y) {
    switch (key)
    {
    case 27:
        exit(0);
        break;
        // Light intensity controls
    case 'i':
    case 'I':
        lightIntensity = fmaxf(lightIntensity + 0.1f, 0.0f);
        std::cout << "Light Intensity: " << lightIntensity << std::endl;
        break;

    case 'k':
    case 'K':
        lightIntensity = fmaxf(lightIntensity - 0.1f, 0.0f);
        std::cout << "Light Intensity: " << lightIntensity << std::endl;
        break;

        // Ambient light controls
    case 'a':
    case 'A':
        ambientIntensity = fmaxf(ambientIntensity + 0.05f, 0.0f);
        std::cout << "Ambient Intensity: " << ambientIntensity << std::endl;
        break;

    case 's':
    case 'S':
        ambientIntensity = fmaxf(ambientIntensity - 0.05f, 0.0f);
        std::cout << "Ambient Intensity: " << ambientIntensity << std::endl;
        break;

        // Reset to defaults
    case 'r':
    case 'R':
        lightIntensity = 1.0f;
        ambientIntensity = 0.2f;
        std::cout << "Light settings reset to defaults" << std::endl;
        break;

    case 'h':
    case 'H':
        printLightControls();
        break;

    default:
        break;
    }
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    printLightControls();
    std::cout << "\nInteraksi Pintu Lift:\n";
    std::cout << "Panah kanan = buka pintu\n";
    std::cout << "Panah kiri  = tutup pintu\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 700);
    glutCreateWindow("Lift 3D di Dalam Ruangan - Kontrol Cahaya Sederhana");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);
    glewInit();
    setupLighting();
    glutMainLoop();
}