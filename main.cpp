#include <windows.h>   // Required for audio
#include <mmsystem.h>  // Required for PlaySound
#include <GL/glut.h>
#include <cmath>

// ================= VARIABLES =================
float screenRatio;
float timeVar = 0.0f;
float waterOffset = 0.0f;
float waterfallOffset = 0.0f;
float waveAngle = 0.0f;
float fish1Y = -0.5f, fish2Y = -0.7f;
float birdX = -4.0f, wingAngle = 0.0f;

float charX = 0.5f, charY = -0.5f;
float legAngle = 0.0f;
bool isMoving = false;
int blinkTimer = 0;
bool isBlinking = false;

// ================= INIT & RESHAPE =================
void init() {
    glClearColor(0.0, 0.0, 0.0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);

    // 🎵 PLAY BACKGROUND MUSIC 🎵
    // This loops the SpongeBob theme song asynchronously so it doesn't freeze the graphics
  PlaySound(TEXT("Spongebob_Squarepants_Theme_Song.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    screenRatio = (float)w / h;
    gluOrtho2D(-3.0 * screenRatio, 3.0 * screenRatio, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
}

// ================= HELPER FUNCTIONS =================
void drawCircle(float cx, float cy, float r) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float angle = 2 * 3.14159 * i / 100;
        glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
    }
    glEnd();
}

void drawEllipse(float cx, float cy, float rx, float ry) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float angle = 2 * 3.14159 * i / 100;
        glVertex2f(cx + cos(angle) * rx, cy + sin(angle) * ry);
    }
    glEnd();
}

// ================= BACKGROUND & SCENE =================
void drawSunsetSky() {
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.4f, 0.8f); glVertex2f(-6, 2); glVertex2f(6, 2);
    glColor3f(1.0f, 0.6f, 0.7f); glVertex2f(6, 0.5f); glVertex2f(-6, 0.5f);

    glColor3f(1.0f, 0.6f, 0.7f); glVertex2f(-6, 0.5f); glVertex2f(6, 0.5f);
    glColor3f(1.0f, 0.9f, 0.5f); glVertex2f(6, -0.5f); glVertex2f(-6, -0.5f);
    glEnd();

    glColor3f(1.0f, 0.95f, 0.6f); drawCircle(-1.0f, 0.3f, 0.6f);
    glColor4f(1.0f, 1.0f, 0.8f, 0.3f); drawCircle(-1.0f, 0.3f, 0.8f);
}

void drawMountains() {
    glPushMatrix();
    glTranslatef(-1.5f, -0.2f, 0);

    glColor3f(0.4f, 0.5f, 0.6f);
    glBegin(GL_TRIANGLES); glVertex2f(0.0f, -0.2f); glVertex2f(2.0f, -0.2f); glVertex2f(1.0f, 0.8f); glEnd();

    glColor3f(0.5f, 0.6f, 0.7f);
    glBegin(GL_TRIANGLES); glVertex2f(-1.0f, -0.2f); glVertex2f(1.5f, -0.2f); glVertex2f(0.2f, 1.2f); glEnd();

    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_TRIANGLES); glVertex2f(-0.1f, 0.8f); glVertex2f(0.5f, 0.8f); glVertex2f(0.2f, 1.2f); glEnd();
    glPopMatrix();
}

void drawLandscape() {
    glColor3f(0.4f, 0.7f, 0.3f);
    drawEllipse(-2.0f, -0.4f, 2.5f, 0.6f); drawEllipse(1.0f, -0.4f, 3.0f, 0.5f);

    glColor3f(0.5f, 0.8f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(-6, -0.4f); glVertex2f(6, -0.4f);
    glVertex2f(6, -2.0f); glVertex2f(-6, -2.0f);
    glEnd();
}

// ================= UPGRADED WATERFALL =================
void drawWaterfall() {
    glColor3f(0.35f, 0.35f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(-3.6f, 1.6f); glVertex2f(-1.8f, 1.6f);
    glVertex2f(-1.6f, -1.0f); glVertex2f(-3.8f, -1.0f);
    glEnd();

    glColor3f(0.2f, 0.6f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(-3.3f, 1.5f); glVertex2f(-2.1f, 1.5f);
    glVertex2f(-1.9f, -0.8f); glVertex2f(-3.3f, -0.8f);
    glEnd();

    glColor3f(0.4f, 0.4f, 0.4f);
    drawCircle(-2.8f, 0.3f, 0.2f);
    drawCircle(-2.5f, 0.2f, 0.15f);

    glColor4f(0.8f, 0.95f, 1.0f, 0.7f);
    drawCircle(-2.7f, 0.4f, 0.15f);
    drawCircle(-2.5f, 0.3f, 0.1f);

    glColor4f(0.7f, 0.9f, 1.0f, 0.8f);
    glLineWidth(4.0f);
    for (float x = -3.1f; x < -2.1f; x += 0.25f) {
        for (float y = 1.4f; y > 0.5f; y -= 0.3f) {
            glBegin(GL_LINES);
            glVertex2f(x + (y * 0.05f), y + waterfallOffset);
            glVertex2f(x + (y * 0.05f), y - 0.2f + waterfallOffset);
            glEnd();
        }
        for (float y = 0.2f; y > -0.7f; y -= 0.3f) {
            glBegin(GL_LINES);
            glVertex2f(x - 0.1f + (y * 0.05f), y + waterfallOffset);
            glVertex2f(x - 0.1f + (y * 0.05f), y - 0.2f + waterfallOffset);
            glEnd();
        }
    }

    float pulse = sin(timeVar * 5.0f) * 0.05f;

    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    drawCircle(-2.6f, -0.7f, 0.5f + pulse);
    drawCircle(-2.1f, -0.8f, 0.4f + (pulse * 0.5f));

    glColor4f(0.9f, 0.95f, 1.0f, 0.9f);
    drawCircle(-2.7f, -0.8f, 0.25f);
    drawCircle(-2.3f, -0.85f, 0.2f);
    drawCircle(-2.0f, -0.9f, 0.18f);
    drawCircle(-2.9f, -0.7f, 0.15f);

    float dropY = sin(timeVar * 12.0f) * 0.1f;
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(-2.5f, -0.4f + dropY, 0.04f);
    drawCircle(-2.2f, -0.5f - dropY, 0.03f);
    drawCircle(-2.8f, -0.3f + dropY * 0.8f, 0.05f);
}

void drawRiver() {
    glColor3f(0.3f, 0.7f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(-3.5f, -0.8f); glVertex2f(-1.0f, -0.8f);
    glVertex2f(0.5f, -1.2f);  glVertex2f(6.0f, -1.2f);
    glVertex2f(6.0f, -2.0f);  glVertex2f(-3.5f, -2.0f);
    glEnd();

    glColor3f(0.6f, 0.9f, 1.0f);
    glLineWidth(2.0f);
    for (float i = -2.0f; i < 4.0f; i += 0.8f) {
        float waveShift = waterOffset + sin(i);
        glBegin(GL_LINES);
        glVertex2f(i + waveShift, -1.0f);
        glVertex2f(i + 0.4f + waveShift, -1.0f);
        glEnd();
    }
}

// ================= UPGRADED TREE =================
void drawGiantTree() {
    glPushMatrix();
    glTranslatef(2.5f, -0.8f, 0);

    glColor3f(0.4f, 0.25f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.6f); glVertex2f(-0.5f, -0.1f); glVertex2f(-0.3f, 1.0f);
    glVertex2f(0.3f, 1.0f); glVertex2f(0.4f, -0.1f); glVertex2f(1.2f, -0.5f);
    glEnd();

    glColor3f(0.25f, 0.15f, 0.05f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.2f, -0.1f); glVertex2f(-0.2f, 1.0f);
    glVertex2f(0.1f, -0.2f);  glVertex2f(0.1f, 1.2f);
    glVertex2f(-0.5f, -0.3f); glVertex2f(-0.1f, 0.2f);
    glEnd();

    glColor3f(0.4f, 0.25f, 0.1f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.3f, 0.8f); glVertex2f(-0.1f, 1.0f);
    glVertex2f(-1.2f, 1.4f); glVertex2f(-1.3f, 1.2f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.1f, 0.9f); glVertex2f(0.3f, 0.7f);
    glVertex2f(1.3f, 1.3f); glVertex2f(1.1f, 1.5f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(-0.2f, 1.0f); glVertex2f(0.2f, 1.0f);
    glVertex2f(0.1f, 1.8f); glVertex2f(-0.1f, 1.8f);
    glEnd();

    float vineSway = sin(timeVar * 2.0f) * 0.05f;
    glColor3f(0.2f, 0.5f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-1.0f, 1.3f); glVertex2f(-1.0f + vineSway, 1.0f); glVertex2f(-1.05f + vineSway, 0.7f);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(0.8f, 1.4f); glVertex2f(0.85f - vineSway, 1.1f); glVertex2f(0.8f - vineSway, 0.8f);
    glEnd();

    glColor3f(0.1f, 0.4f, 0.2f);
    drawCircle(0.0f, 1.8f, 0.6f); drawCircle(-0.8f, 1.5f, 0.5f); drawCircle(0.8f, 1.5f, 0.5f);

    glColor3f(0.2f, 0.7f, 0.4f); drawCircle(-1.1f, 1.4f, 0.4f);
    glColor3f(0.1f, 0.8f, 0.5f); drawCircle(-0.5f, 1.7f, 0.45f);

    glColor3f(0.8f, 0.3f, 0.6f); drawCircle(0.9f, 1.4f, 0.35f);
    glColor3f(0.3f, 0.6f, 0.8f); drawCircle(0.5f, 1.8f, 0.45f);

    glColor3f(0.5f, 0.9f, 0.2f);
    drawCircle(0.0f, 2.0f, 0.3f); drawCircle(-0.8f, 1.6f, 0.25f); drawCircle(0.7f, 1.7f, 0.2f);

    glColor3f(1.0f, 0.8f, 0.0f); drawEllipse(0.8f, 1.0f, 0.1f, 0.15f);
    glColor3f(0.2f, 0.8f, 0.2f); glBegin(GL_TRIANGLES); glVertex2f(0.75f, 1.15f); glVertex2f(0.85f, 1.15f); glVertex2f(0.8f, 1.3f); glEnd();

    glColor3f(0.9f, 0.1f, 0.2f); drawCircle(-0.6f, 0.8f, 0.1f);
    glColor3f(0.1f, 0.6f, 0.1f); drawCircle(-0.6f, 0.9f, 0.04f);

    glPopMatrix();
}

// ================= DETAILS & WILDLIFE =================
void drawDetails() {
    glColor3f(0.5f, 0.5f, 0.5f);
    drawEllipse(-1.5f, -1.8f, 0.15f, 0.08f);
    drawEllipse(1.2f, -1.4f, 0.2f, 0.1f);

    glColor3f(1.0f, 0.6f, 0.8f);
    drawEllipse(0.5f, -1.7f, 0.1f, 0.06f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES); glVertex2f(0.5f, -1.7f); glVertex2f(0.45f, -1.65f); glEnd();
}

void drawFish(float x, float y, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(r, g, b);
    drawEllipse(0, 0, 0.1f, 0.06f);
    glBegin(GL_TRIANGLES); glVertex2f(0.08f, 0); glVertex2f(0.15f, 0.05f); glVertex2f(0.15f, -0.05f); glEnd();
    glColor3f(1, 1, 1); drawCircle(-0.05f, 0.02f, 0.02f);
    glColor3f(0, 0, 0); drawCircle(-0.05f, 0.02f, 0.01f);
    glPopMatrix();
}

void drawBird(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.2f, 0.2f, 0.2f);
    float flap = sin(wingAngle) * 0.1f;
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.1f, flap); glVertex2f(0, -0.05f); glVertex2f(0.1f, flap);
    glEnd();
    glPopMatrix();
}

// ================= SPONGEBOB =================
void drawSpongeBob() {
    glPushMatrix();

    float idleBob = sin(timeVar * 2.0f) * 0.01f;
    glTranslatef(charX, charY + idleBob, 0);
    glScalef(1.5f, 1.5f, 1.0f);

    glColor3f(1, 1, 0);
    glBegin(GL_LINES); glVertex2f(-0.05f, -0.15f); glVertex2f(-0.05f, -0.3f); glEnd();
    glBegin(GL_LINES); glVertex2f(0.05f, -0.15f); glVertex2f(0.05f, -0.3f); glEnd();

    glColor3f(0, 0, 0);
    drawEllipse(-0.07f, -0.32f, 0.04f, 0.02f);
    drawEllipse(0.07f, -0.32f, 0.04f, 0.02f);

    glColor3f(1, 1, 0);
    glBegin(GL_LINES); glVertex2f(-0.12f, 0.0f); glVertex2f(-0.2f, -0.1f); glEnd();
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES); glVertex2f(-0.12f, 0.02f); glVertex2f(-0.12f, -0.05f); glVertex2f(-0.18f, -0.02f); glEnd();

    glPushMatrix();
    glTranslatef(0.12f, 0.0f, 0);
    glRotatef(waveAngle, 0, 0, 1);

    glColor3f(1, 1, 0);
    glBegin(GL_LINES); glVertex2f(0.0f, 0.0f); glVertex2f(0.15f, 0.1f); glEnd();

    drawCircle(0.15f, 0.1f, 0.03f);
    glBegin(GL_LINES);
    glVertex2f(0.15f, 0.1f); glVertex2f(0.16f, 0.15f);
    glVertex2f(0.15f, 0.1f); glVertex2f(0.19f, 0.13f);
    glVertex2f(0.15f, 0.1f); glVertex2f(0.19f, 0.08f);
    glEnd();

    glPopMatrix();

    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES); glVertex2f(0.12f, 0.02f); glVertex2f(0.12f, -0.05f); glVertex2f(0.18f, -0.02f); glEnd();

    glColor3f(1.0f, 0.95f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, -0.12f); glVertex2f(0.15f, -0.12f);
    glVertex2f(0.15f, 0.18f); glVertex2f(-0.15f, 0.18f);
    glEnd();

    glColor3f(0.85f, 0.8f, 0.0f);
    drawCircle(-0.1f, 0.1f, 0.02f); drawCircle(0.1f, 0.05f, 0.03f); drawCircle(-0.08f, -0.05f, 0.015f);

    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, -0.12f); glVertex2f(0.15f, -0.12f);
    glVertex2f(0.15f, -0.2f); glVertex2f(-0.15f, -0.2f);
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, -0.05f); glVertex2f(0.15f, -0.05f);
    glVertex2f(0.15f, -0.12f); glVertex2f(-0.15f, -0.12f);
    glEnd();

    glColor3f(0.9f, 0, 0);
    glBegin(GL_POLYGON); glVertex2f(0.0f, -0.05f); glVertex2f(-0.02f, -0.08f); glVertex2f(0.0f, -0.16f); glVertex2f(0.02f, -0.08f); glEnd();

    if (!isBlinking) {
        glColor3f(1, 1, 1); drawCircle(-0.05f, 0.08f, 0.045f); drawCircle(0.05f, 0.08f, 0.045f);
        glColor3f(0.3f, 0.6f, 1.0f); drawCircle(-0.05f, 0.08f, 0.02f); drawCircle(0.05f, 0.08f, 0.02f);
        glColor3f(0, 0, 0); drawCircle(-0.05f, 0.08f, 0.01f); drawCircle(0.05f, 0.08f, 0.01f);
    }
    else {
        glColor3f(0, 0, 0); glBegin(GL_LINES); glVertex2f(-0.09f, 0.08f); glVertex2f(-0.01f, 0.08f); glVertex2f(0.01f, 0.08f); glVertex2f(0.09f, 0.08f); glEnd();
    }

    glColor3f(0.9f, 0.4f, 0.4f); drawCircle(-0.1f, 0.02f, 0.015f); drawCircle(0.1f, 0.02f, 0.015f);
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_STRIP);
    for (float i = -0.08f; i <= 0.08f; i += 0.01f) glVertex2f(i, 0.0f - (cos(i * 18) * 0.03f));
    glEnd();

    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(-0.025f, -0.025f); glVertex2f(-0.005f, -0.025f); glVertex2f(-0.005f, -0.05f); glVertex2f(-0.025f, -0.05f);
    glVertex2f(0.005f, -0.025f); glVertex2f(0.025f, -0.025f); glVertex2f(0.025f, -0.05f); glVertex2f(0.005f, -0.05f);
    glEnd();

    glPopMatrix();
}

// ================= MAIN DISPLAY =================
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawSunsetSky();

    drawBird(birdX, 1.2f);
    drawBird(birdX - 0.5f, 1.4f);
    drawBird(birdX - 0.8f, 1.1f);

    drawMountains();
    drawLandscape();

    drawWaterfall();
    drawRiver();

    drawFish(-1.0f, fish1Y, 1.0f, 0.4f, 0.8f);
    drawFish(1.5f, fish2Y, 0.4f, 0.2f, 0.9f);

    drawGiantTree();
    drawDetails();
    drawSpongeBob();

    glutSwapBuffers();
}

// ================= UPDATE LOGIC =================
void update(int) {
    timeVar += 0.01f;

    waterOffset += 0.01f;
    if (waterOffset > 3.14f) waterOffset = 0;

    waterfallOffset -= 0.03f;
    if (waterfallOffset < -0.3f) waterfallOffset = 0;

    waveAngle = 30.0f + sin(timeVar * 15.0f) * 40.0f;

    birdX += 0.008f;
    if (birdX > 5.0f) birdX = -5.0f;
    wingAngle += 0.2f;

    fish1Y = -1.0f + sin(timeVar * 3.0f) * 0.6f;
    fish2Y = -1.2f + cos(timeVar * 2.5f) * 0.5f;

    blinkTimer++;
    if (blinkTimer > 150) {
        isBlinking = true;
        if (blinkTimer > 160) {
            isBlinking = false;
            blinkTimer = 0;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// ================= INPUT =================
void keyboard(unsigned char key, int, int) {
    if (key == 'a') charX -= 0.08f;
    if (key == 'd') charX += 0.08f;
    if (key == 'w') charY += 0.08f;
    if (key == 's') charY -= 0.08f;
}

// ================= MAIN =================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Ultimate Scene - SpongeBob Sunset Oasis");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
}