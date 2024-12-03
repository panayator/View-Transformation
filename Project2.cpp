#include <GL/glut.h>
#include <GL/glui.h>
#include <cmath>
#include <iostream>

// Define cube vertices
GLfloat cubeVertices[8][3] = {
    {-0.5, -0.5, 0.5},
    {0.5, -0.5, 0.5},
    {0.5, 0.5, 0.5},
    {-0.5, 0.5, 0.5},
    {-0.5, -0.5, -0.5},
    {0.5, -0.5, -0.5},
    {0.5, 0.5, -0.5},
    {-0.5, 0.5, -0.5}
};

// Define cube faces
GLint cubeFaces[6][4] = {
    {0, 1, 2, 3},  // Front face
    {1, 5, 6, 2},  // Right face
    {4, 5, 6, 7},  // Back face
    {0, 4, 7, 3},  // Left face
    {3, 2, 6, 7},  // Top face
    {0, 1, 5, 4}   // Bottom face
};

// Define pyramid vertices
GLfloat pyramidVertices[5][3] = {
    {0.0, 0.5, 0.0},
    {-0.5, -0.5, -0.5},
    {0.5, -0.5, -0.5},
    {0.5, -0.5, 0.5},
    {-0.5, -0.5, 0.5}
};

// Define pyramid faces
GLint pyramidFaces[5][3] = {
    {0, 1, 2},  // Base face 1
    {0, 2, 3},  // Base face 2
    {0, 3, 4},  // Base face 3
    {0, 4, 1},  // Base face 4
    {1, 3, 2}   // Side face
};

// Perspective Projection parameters
GLfloat theta = 45.0;  // Rotation angle for cube
GLfloat aspect = 1.0; // Aspect ratio
GLfloat n = 1.0;     // Near clipping distance
GLfloat f = 15.0;    // Far clipping distance
GLfloat gridScale = 1.0;    // The scale of the grid, it won't change by scaling

int width = 1200;   // Window width
int height = 920;   // Window height

// Variables for rotation
GLfloat rotationSpeed = 0.2;    
GLfloat rotationAngleX = 0.0;
GLfloat rotationAngleY = 0.0;
GLfloat rotationAngleZ = 0.0;
GLfloat angleX = 0.0;
GLfloat angleY = 0.0;
GLfloat angleZ = 0.0;
GLfloat angleX_degrees;
GLfloat angleY_degrees;
GLfloat angleZ_degrees;

// Variables for scaling
GLfloat sx = 1.0;
GLfloat sy = 1.0;
GLfloat sz = 1.0;
GLfloat controlScale = 0;
GLUI_Translation *scaling;
GLfloat scaleFactor = 0.02; // Basically the amount the object is scaled up or down

// Variables for translation
GLfloat tx = 0.0;
GLfloat ty = 0.0;
GLfloat tz = 0.0;
GLUI_Translation *translationXZ;
GLUI_Translation *translationY;
GLfloat translationSpeed = 0.1; // Basically the amount of translation applied each time


GLfloat identityMatrix[16] = {  // Identity Matrix to apply rotations on
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};

GLfloat defaultIdentity[16] = { // Identity Matrix to use when resetting transformations
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};

GLfloat scaleMatrix[16] = {
    sx, 0.0, 0.0, 0.0,
    0.0, sy, 0.0, 0.0,
    0.0, 0.0, sz, 0.0,
    0.0, 0.0, 0.0, 1.0
};

GLfloat scaleMatrixGrid[16] = {
    gridScale, 0.0, 0.0, 0.0,
    0.0, gridScale, 0.0, 0.0,
    0.0, 0.0, gridScale, 0.0,
    0.0, 0.0, 0.0, 1.0
};

GLfloat translationMatrix[16] = {   // OpenGL reads each line of this as a column of the table
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    tx, ty, tz, 1.0
    
};

GLfloat pyramidOnTop[16] = {    // Translation Matrix to move the pyramid on top of the cube
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 1.0, 0.0, 1.0
};

const float minScale = 0.01;    // Minimum scale


int mouseX = 0; // Initial x-axis mouse position
int mouseY = 0; // Initial y-axis mouse position

// State variables for mouse buttons
bool leftMouseButton = false;
bool rightMouseButton = false;
bool middleClick = false;

// State variables for W, A, S, D, Arrow Up and Arrow Down keys
bool isWKeyPressed = false;
bool isSKeyPressed = false;
bool isAKeyPressed = false;
bool isDKeyPressed = false;
bool isArrowUpPressed = false;
bool isArrowDownPressed = false;


// Grid state
bool gridVisible = true;

// GLUI variables
GLUI *glui;
int MAIN_WINDOW; 
int GRID = 1, AXES = 0, isWireframe = 0, instructionsON = 1;   // Boolean Variables in binary form to be used for buttons 
GLUI *instructions_glui;

// Strings for static text to appear in the GLUI
GLUI_StaticText *static_theta;
char theta_val[50];
GLUI_StaticText *static_aspect;
char aspect_val[50];
GLUI_StaticText *static_n;
char n_val[50];
GLUI_StaticText *static_f;
char f_val[50];

GLUI_StaticText *static_width;
char width_val[50];
GLUI_StaticText *static_height;
char height_val[50];

GLUI_StaticText *static_rotationX;
char rotationX_val[50];
GLUI_StaticText *static_rotationY;
char rotationY_val[50];
GLUI_StaticText *static_rotationZ;
char rotationZ_val[50];
GLUI_StaticText *static_rotationSpeed;
char rotationSpeed_val[50];

GLUI_StaticText *static_tx;
char tx_val[50];
GLUI_StaticText *static_ty;
char ty_val[50];
GLUI_StaticText *static_tz;
char tz_val[50];
GLUI_StaticText *static_translationSpeed;
char translationSpeed_val[50];

GLUI_StaticText *static_sx;
char sx_val[50];
GLUI_StaticText *static_sy;
char sy_val[50];
GLUI_StaticText *static_sz;
char sz_val[50];
GLUI_StaticText *static_scaleFactor;
char scaleFactor_val[50];

// Function to draw a cube
void drawCube() {
    for (int i = 0; i < 6; ++i) {
        glBegin(GL_QUADS);
        for (int j = 0; j < 4; ++j) {
            glVertex3fv(cubeVertices[cubeFaces[i][j]]);
        }
        glEnd();
    }
}

// Function to draw a pyramid
void drawPyramid() {
    for (int i = 0; i < 5; ++i) {
        glBegin(GL_TRIANGLES);
        for (int j = 0; j < 3; ++j) {
            glVertex3fv(pyramidVertices[pyramidFaces[i][j]]);
        }
        glEnd();
    }
}

// Function to draw the grid floor
void drawGrid() {
    glColor3f(0.5, 0.5, 0.5); // Set grid color to gray

    glPushMatrix();
    glTranslatef(0.0, -0.5, 0.0); // Translate the grid vertically

    glMultMatrixf(scaleMatrixGrid); // Apply grid scaling

    // Draw horizontal lines
    for (float i = -5.0; i <= 5.0; ++i) {
        glBegin(GL_LINES);
        glVertex3f(-5.0, 0.0, i);
        glVertex3f(5.0, 0.0, i);
        glEnd();
    }

    // Draw vertical lines
    for (float i = -5.0; i <= 5.0; ++i) {
        glBegin(GL_LINES);
        glVertex3f(i, 0.0, -5.0);
        glVertex3f(i, 0.0, 5.0);
        glEnd();
    }

    glPopMatrix();
}


// Function to draw the axes with text labels
void drawAxes() {
     glLineWidth(3.0); // Set the line width
    // X-axis (Purple)
    glColor3f(0.5, 0.0, 0.5); 
    glBegin(GL_LINES);
    glVertex3f(-1.5, 0.0, 0.0);
    glVertex3f(1.5, 0.0, 0.0);
    glEnd();
    glRasterPos3f(1.6, 0.0, 0.0); // Position the text label
    _glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "X"); // Draw the text label
    
    // Y-axis (Turquoise)
    glColor3f(0.0, 0.5, 0.5); 
    glBegin(GL_LINES);
    glVertex3f(0.0, -1.5, 0.0);
    glVertex3f(0.0, 1.5, 0.0);
    glEnd();
    glRasterPos3f(0.0, 1.6, 0.0); // Position the text label
    _glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Y"); // Draw the text label
    
    // Z-axis (Blue)
    glColor3f(0.0, 0.0, 1.0); 
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0, -1.5);
    glVertex3f(0.0, 0.0, 1.5);
    glEnd();
    glRasterPos3f(0.0, 0.0, 1.6); // Position the text label
    _glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "Z"); // Draw the text label
}



// Function to update the GLUI's static strings in real time when a change is made (callback function)
void updateVarsText(int id) {
    sprintf(theta_val, "Theta: %.1f", theta);
    static_theta->set_text(theta_val);

    sprintf(aspect_val, "Aspect Ratio: %.1f", aspect);
    static_aspect->set_text(aspect_val);

    sprintf(n_val, "n: %.1f", n);
    static_n->set_text(n_val);

    sprintf(f_val, "f: %.1f", f);
    static_f->set_text(f_val);

    sprintf(rotationX_val, "Rotation X: %.2f", (angleX/rotationSpeed));
    static_rotationX->set_text(rotationX_val);

    sprintf(rotationY_val, "Rotation Y: %.2f", (angleY/rotationSpeed));
    static_rotationY->set_text(rotationY_val);

    sprintf(rotationZ_val, "Rotation Z: %.2f", (angleZ/rotationSpeed));
    static_rotationZ->set_text(rotationZ_val);

    sprintf(rotationSpeed_val, "Rotation Speed: %.2f", rotationSpeed);
    static_rotationSpeed->set_text(rotationSpeed_val);

    
    sprintf(tx_val, "X-axis Translation: %.2f", tx);
    static_tx->set_text(tx_val);

    sprintf(ty_val, "Y-axis Translation: %.2f", ty);
    static_ty->set_text(ty_val);

    sprintf(tz_val, "Y-axis Translation: %.2f", tz);
    static_tz->set_text(tz_val);

    sprintf(translationSpeed_val, "Translation Speed: %.2f", translationSpeed);
    static_translationSpeed->set_text(translationSpeed_val);


    sprintf(sx_val, "X-axis Scale: %.2f", sx);
    static_sx->set_text(sx_val);

    sprintf(sy_val, "Y-axis Scale: %.2f", sy);
    static_sy->set_text(sy_val);

    sprintf(sz_val, "Y-axis Scale: %.2f", sz);
    static_sz->set_text(sz_val);

    sprintf(scaleFactor_val, "Scale Factor: %.2f", scaleFactor);
    static_scaleFactor->set_text(scaleFactor_val);
}

void translateXZ(int id){   // Callback function for the TranslationXZ Control
    
    // Update the translation values
    tx = translationXZ->get_x();
    tz = -translationXZ->get_y();

}

void translateY(int id){ // Callback function for the TranslationY Control

    // Update the translation values
    ty = translationY->get_y();

}

void scale(int id){ // Callback function for the Scale Control (it's actually a TranslationY Control)

    controlScale = scaling->get_y();
    if(controlScale >= minScale)
    sx = sy = sz = controlScale;

}

// Function to handle scene drawing
void drawScene() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen, allowing for a fresh frame to be rendered
    glLoadIdentity();   // Loads identity matrix

    // Updates the perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(theta, aspect, n, f);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Basically from which side of the scene we are looking
    glMultMatrixf(identityMatrix);  // Multiplies the Modelview matrix with the new Identity Matrix (on which Rotation was applied) (Currently: Modelview * identityMatrix)
    // NOTE: Doing this before drawning any of our objects ensures that everything will be rotated, as the transformations happen on everything below the transformation function
    
    // Draw the grid floor
    if (GRID) {
        drawGrid();
    }

    // To make sure we are editing the correct elements of the Translation Matrix
    translationMatrix[12] = tx; // OpenGL reads matrices column by column
    translationMatrix[13] = ty;
    translationMatrix[14] = tz;
    glMultMatrixf(translationMatrix);   // Apply translation by multiplying the Modelview matrix with the translationMatrix
    // NOTE: At this point it's: Modelview * identityMatrix * translationMatrix, they stack
    // NOTE: Doing this below drawGrid() and above drawAxes() ensures that the grid won't be translated

    // Draw the axes
    if (!AXES) {
        drawAxes();
    }
    
    // To make sure we are editing the correct elements of the Scale Matrix
    scaleMatrix[0] = sx;
    scaleMatrix[5] = sy;
    scaleMatrix[10] = sz;
    glMultMatrixf(scaleMatrix); // Apply scaling by multiplying the Modelview matrix with the scaleMatrix
    // NOTE: At this point it's: Modelview * identityMatrix * translationMatrix * scaleMatrix, they stack
    // NOTE: Doing this below drawGrid() and drawAxes() ensures that only the cube and the pyramid will be scaled

    
    if (isWireframe) {  // Makes the cube and the pyramid Wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(!isWireframe){   //makes the cube and the pyramid solid
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

    
    if (isWKeyPressed) {    // Move away from the camera when W is pressed
        tz -= translationSpeed;
    }
    if (isSKeyPressed) {    // Move towards the camera when S is pressed
        tz += translationSpeed;
    }
    if (isAKeyPressed) {    // Move left when A is pressed
        tx -= translationSpeed;
    }
    if (isDKeyPressed) {    // Move right when D is pressed
        tx += translationSpeed;
    }        
    if (isArrowUpPressed){  // Move up when Arrow Up is pressed
        ty += translationSpeed;
    }       
    if (isArrowDownPressed){    // Move down when Arrow Down is pressed
        ty -= translationSpeed;
    }
    

    
    
    glColor3f(0.0, 1.0, 0.0); // Set cube color to red
    drawCube();
    glMultMatrixf(pyramidOnTop); // Translate pyramid to the top of the cube
    glColor3f(1.0, 0.0, 0.0); // Set pyramid color to green
    drawPyramid();
    glFlush();
    glutSwapBuffers();
    updateVarsText(0);  // update all static texts (0 is called since we have no id to pass)
}

void resetRotation(int) {   // Resets the current identityMatrix with the default identity matrix
    memcpy(identityMatrix, defaultIdentity, sizeof(defaultIdentity));   
}

void resetScaling(int){ // Resets scaling to 1
    
    sx = sy = sz = 1.0; 
}

void resetTranslation(int){ //resets translation to 0
    tx = ty = tz = 0.0;
}

// void wireframeCallback(int value) {  // for debugging, to check is the wireframe option is working
//     std::cout << "Address of isWireframe: " << &isWireframe << std::endl;
//     std::cout << "Value of value: " << isWireframe << std::endl;
// }



// Function for when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q' || key == 27)
        exit(0);    // Exit if Q or ESC is pressed
    
    if (key == 'w' || key == 'W') { // Saves the pressed state of the W key
        isWKeyPressed = true;
    }
    if (key == 's' || key == 'S') { // Saves the pressed state of the S key
        isSKeyPressed = true;
    }
    if (key == 'a' || key == 'A') { // Saves the pressed state of the A key
        isAKeyPressed = true;
    }
    if (key == 'd' || key == 'D') { // Saves the pressed state of the D key
        isDKeyPressed = true;
    }
}

// Function for when a key is released
void handleKeyUp(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') { // Saves the released state of the W key
        isWKeyPressed = false;
    }
    if (key == 's' || key == 'S') { // Saves the released state of the S key
        isSKeyPressed = false;
    }
    if (key == 'a' || key == 'A') { // Saves the released state of the A key
        isAKeyPressed = false;
    }
    if (key == 'd' || key == 'D') { // Saves the released state of the D key
        isDKeyPressed = false;
    }
}

// Function for when a special key is presed (for Arrow Keys)
void handleSpecialKeypress(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {   // Saves the pressed state of the Arrow Up key
        isArrowUpPressed = true;
    }
    if (key == GLUT_KEY_DOWN) { // Saves the pressed state of the Arrow Down key
        isArrowDownPressed = true;
    }
}

// Function for when a special key is released (for Arrow Keys)
void handleSpecialKeyUp(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {   // Saves the released state of the Arrow Up key
        isArrowUpPressed = false;
    }
    if (key == GLUT_KEY_DOWN) { // Saves the released state of the Arrow Down key
        isArrowDownPressed = false;
    }
}


// Function to handle mouse button presses and scroll wheel events
void handleMouse(int button, int state, int x, int y) {
    leftMouseButton = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);   // Listens for Left Clicks
    rightMouseButton = (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN);     // Listens for Right Clicks
    middleClick = (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN); // Listens for Middle Clicks
    if (leftMouseButton) {  // Saves the mouse's position on Left Click
            mouseX = x;
            mouseY = y;
    }
    if (rightMouseButton) {  // Saves the mouse's x-axis position on Right Click
            mouseX = x;
    }
    if (middleClick) {  // Reset all transformations when middle click is pressed
        resetRotation(-1);
        resetScaling(-1);
        resetTranslation(-1);
    }
    if (button == 3){   // Scroll Wheel Up
        sx += scaleFactor;
        sy += scaleFactor;
        sz += scaleFactor;
        glLoadIdentity();   // Loads Identity Matrix
        glMultMatrixf(scaleMatrix); // Multiplies Identity Matrix with scaleMatrix to scale the objects
    }

    if (button == 4 && sx > minScale && sy > minScale && sz > minScale){    // Scroll Wheel Down, ensures scaling won't go zero or negative
        sx -= scaleFactor;
        sy -= scaleFactor;
        sz -= scaleFactor;
        glLoadIdentity();   // Loads Identity Matrix
        glMultMatrixf(scaleMatrix); // Multiplies Identity Matrix with scaleMatrix to scale the objects
    }
      
}


// Function that handles mouse movement
void handleMouseMotion(int x, int y) {

    if (leftMouseButton) {  // If Left Clicking, move mouse vertically to rotate around the X-axis and horizontally to rotate around the Y-axis
        int deltaX = x - mouseX;    // The difference between the original x-axis mouse position and the new one
        int deltaY = y - mouseY;    // The difference between the original y-axis mouse position and the new one
        mouseX = x; // Sets the new x-axis position as the new old position 
        mouseY = y; // Sets the new y-axis position as the new old position 
        rotationAngleX = deltaY * rotationSpeed * 0.0174532925;    // Apply rotationSpeed to slow down the movement and convert to radians
        rotationAngleY = deltaX * rotationSpeed * 0.0174532925;    // Apply rotationSpeed to slow down the movement and convert to radians
        
        GLfloat rotationX[16] = {   // X-axis Rotation Matrix
            1.0, 0.0, 0.0, 0.0,
            0.0, (GLfloat)cos(rotationAngleX), (GLfloat)sin(rotationAngleX), 0.0,   // (GLfloat) to avoid a "narrowing conversion" warning
            0.0, (GLfloat)-sin(rotationAngleX), (GLfloat)cos(rotationAngleX), 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        GLfloat rotationY[16] = {   // Y-axis Rotation Matrix
            (GLfloat)cos(rotationAngleY), 0.0, (GLfloat)-sin(rotationAngleY), 0.0, 
            0.0, 1.0, 0.0, 0.0,
            (GLfloat)sin(rotationAngleY), 0.0, (GLfloat)cos(rotationAngleY), 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        glLoadIdentity();   // Load Identity Matrix
        glMultMatrixf(identityMatrix);  // Multiply by our Identity Matrix (so that we can save the changes)
        glMultMatrixf(rotationX);   // Multiply the X-axis Rotation Matrix
        glMultMatrixf(rotationY);   // Multiply the Y-axis Rotation Matrix
        glGetFloatv(GL_MODELVIEW_MATRIX, identityMatrix);   // Basically lets OpenGL know that we want this multiplication to apply the new identityMatrix on our Modelview Matrix
    }
    if (rightMouseButton){  // If Right Clicking, move mouse horizontally to rotate around the Z-axis
        int deltaZ = x - mouseX;    // The difference between the original x-axis mouse position and the new one
        mouseX = x; // Sets the new X-axis position as the new old position
        rotationAngleZ = deltaZ * rotationSpeed * 0.0174532925;    // Apply rotationSpeed to slow down the movement and convert to radians
        
        GLfloat rotationZ[16] = {   // Z-axis Rotation Matrix
            (GLfloat)cos(rotationAngleZ), (GLfloat)sin(rotationAngleZ), 0.0, 0.0,
            (GLfloat)-sin(rotationAngleZ), (GLfloat)cos(rotationAngleZ), 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 
            0.0, 0.0, 0.0, 1.0
        };

        glLoadIdentity();   // Load Identity Matrix
        glMultMatrixf(identityMatrix);  // Multiply by our Identity Matrix (so that we can save the changes)
        glMultMatrixf(rotationZ);   // Multiply the Z-axis Rotation Matrix
        glGetFloatv(GL_MODELVIEW_MATRIX, identityMatrix);   // Basically lets OpenGL know that we want this multiplication to apply the new identityMatrix on our Modelview Matrix
    }

}

// Function to handle GLUI (and overall program) idle event
void idle() {
    glutSetWindow(MAIN_WINDOW); // Specifies the currently active window in case something goes wrong
    glutPostRedisplay();    // Redisplay the scene
    
}

// Function to handle window reshape/resize
void handleResize(int width, int height) {


    GLUI_Master.auto_set_viewport();    // Automatically sets the viewport whenever the window size changes, aka the position and dimensions of the drawable area of the current window
    aspect = (GLfloat)width / (GLfloat)height;  // Update aspect ratio according to new window dimensions
    glutReshapeWindow(width, height);   // Programmatically reshapes the window, might be unnecessary to our current program, but kept just to be safe

    sprintf(width_val, "Window Width: %d", width);  // Update the static text with the new width
    static_width->set_text(width_val);

    sprintf(height_val, "Window Height: %d", height);   // Update the static text with the new height
    static_height->set_text(height_val);

    // Update the perspective projection, same functions that were used in drawScene()
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(theta, aspect, n, f);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    updateVarsText(0);  //Updates all static strings, 0 because we are not passing any id
}

// Function that handles the instructions window's visibility
void instructionsWindowVisibility(int id){

    if(instructionsON){
        instructions_glui -> show();
    }
    else{
        instructions_glui -> hide();
    }
}

//  Function to set up the interactive GUI
void setup_glui(){

    GLUI_Master.set_glutReshapeFunc(handleResize);  // Registers resize function
    // Register Keyboard, Mouse and Special functions (Since we have a GLUI we must do this in here instead of in main())
    GLUI_Master.set_glutKeyboardFunc(handleKeypress);
    GLUI_Master.set_glutMouseFunc(handleMouse);
    GLUI_Master.set_glutSpecialFunc(handleSpecialKeypress);
    
    GLUI *glui = GLUI_Master.create_glui_subwindow(MAIN_WINDOW, GLUI_SUBWINDOW_RIGHT | GLUI_PANEL_EMBOSSED);    // Create GLUI subwindow
    glui -> set_main_gfx_window(MAIN_WINDOW);   // Set the main window as the window for the GLUI to attach

    // Creates a panel called "Menu" with checkboxes to toggle Grid and Axes visibility, and a toggle for the Wireframe
    GLUI_Panel *panel_menu = glui -> add_panel("Menu");
    GLUI_Checkbox *showGrid = glui -> add_checkbox_to_panel(panel_menu, "Show Grid", &GRID);
    GLUI_Checkbox *hideAxes = glui -> add_checkbox_to_panel(panel_menu, "Hide Axes", &AXES);
    GLUI_Checkbox *wireframeCheck = glui->add_checkbox_to_panel(panel_menu, "Wireframe Toggle", &isWireframe);
    GLUI_Checkbox *instructionsCheck = glui->add_checkbox_to_panel(panel_menu, "Instructions", &instructionsON, -1, instructionsWindowVisibility);
    

    GLUI_Panel *panel_cube = glui -> add_panel("Transformations");  // Creates a panel called "Transformations"
    GLUI_Panel *panel_rotation = glui -> add_panel_to_panel(panel_cube, "Rotation");    // Creates a panel called "Rotation", nested in the "Transformations" panel
    GLUI_Rotation *rotation = glui -> add_rotation_to_panel(panel_rotation, "Rotate", identityMatrix);  // Adds a Rotation control sphere
    glui -> add_column_to_panel(panel_rotation, false); // Adds invisible column to the "Rotation" panel

    GLUI_Panel *panel_rotations = glui -> add_panel_to_panel(panel_rotation, "Rotation Values (Beta)"); // Creates panel "Rotation Values (Beta)" inside "Rotation"
    // Displays current rotation angle around each axis(doesn't display correct numbers, hence 'beta')
    static_rotationX = glui -> add_statictext_to_panel(panel_rotations, rotationX_val);
    static_rotationY = glui -> add_statictext_to_panel(panel_rotations, rotationY_val);
    static_rotationZ = glui -> add_statictext_to_panel(panel_rotations, rotationZ_val);
    static_rotationSpeed = glui -> add_statictext_to_panel(panel_rotations, rotationSpeed_val); // Displays current rotation Speed
    GLUI_Button *resetRotationButton = glui -> add_button_to_panel(panel_rotations, "Reset Rotation", -1, resetRotation);   // Reset Rotation button
    

    GLUI_Panel *panel_translation = glui -> add_panel_to_panel(panel_cube, "Translation");  // Creates "Translation" panel, nested in the "Transformations" panel
    translationXZ = glui -> add_translation_to_panel(panel_translation, "Translate XZ", GLUI_TRANSLATION_XY, NULL, -1, translateXZ);    // Adds a translation control for 2 axes, we use it for XZ translation
    translationXZ->set_speed(translationSpeed);
    translationY = glui -> add_translation_to_panel(panel_translation, "Translate Y", GLUI_TRANSLATION_Y, NULL, -1, translateY);    // Adds a translation control for the Y axis
    translationY->set_speed(translationSpeed);
    glui -> add_column_to_panel(panel_translation, false);  // Adds invisible column in the "Translation" panel

    GLUI_Panel *panel_translations = glui -> add_panel_to_panel(panel_translation, "Translation Values");   // Create "Translation Values" panel, nested inside "Translation" panel
    // Displays current translation applied on each axis (aka the object's coordinates)
    static_tx = glui -> add_statictext_to_panel(panel_translations, tx_val);
    static_ty = glui -> add_statictext_to_panel(panel_translations, ty_val);
    static_tz = glui -> add_statictext_to_panel(panel_translations, tz_val);
    static_translationSpeed = glui -> add_statictext_to_panel(panel_translations, translationSpeed_val);    // Displays current translation Speed (by how much the object moves)
    GLUI_Button *resetTranslationButton = glui -> add_button_to_panel(panel_translation, "Reset Translation", -1, resetTranslation);    // Reset Translation button

    GLUI_Panel *panel_scale = glui -> add_panel_to_panel(panel_cube, "Scaling");    // Create "Scaling" panel, nested inside "Transformations" panel
    scaling = glui -> add_translation_to_panel(panel_scale, "Scale", GLUI_TRANSLATION_Y, NULL,  -1, scale); // Add translation control for the Y axis, we use this for scaling up and down
    scaling->set_speed(scaleFactor);
    glui -> add_column_to_panel(panel_scale, false);    // Add invisible column in the "Scaling" panel
    
    GLUI_Panel *panel_scaling = glui -> add_panel_to_panel(panel_scale, "Scaling Values");  // Create "Scaling Values" panel, nested inside "Scaling" panel
    // Displays current scaling applied on each axis (aka the object's size)
    static_sx = glui -> add_statictext_to_panel(panel_scaling, sx_val);
    static_sy = glui -> add_statictext_to_panel(panel_scaling, sy_val);
    static_sz = glui -> add_statictext_to_panel(panel_scaling, sz_val);
    static_scaleFactor = glui -> add_statictext_to_panel(panel_scaling, scaleFactor_val);   // Displays current scale factor
    GLUI_Button *resetScalingButton = glui -> add_button_to_panel(panel_scaling, "Reset Scaling", -1, resetScaling);    // Reset Scaling button


    GLUI_Panel *panel_values = glui -> add_panel("Edit Values");    // Create "Edit Values" panel
    // Adds editable text boxes for theta, aspect, n, f, rotationSpeed, translationSpeed and scaleFactor. They all have the updateVarsText callback function in order to update the changes in the static texts
    GLUI_EditText *edit_theta = glui -> add_edittext_to_panel(panel_values, "Field Of View (theta):", GLUI_EDITTEXT_FLOAT, &theta, -1, updateVarsText);
    GLUI_EditText *edit_aspect = glui->add_edittext_to_panel(panel_values, "Aspect Ratio:", GLUI_EDITTEXT_FLOAT, &aspect, -1, updateVarsText);
    GLUI_EditText *edit_n = glui->add_edittext_to_panel(panel_values, "Near Clipping Plane (n):", GLUI_EDITTEXT_FLOAT, &n, -1, updateVarsText);
    GLUI_EditText *edit_f = glui->add_edittext_to_panel(panel_values, "Far Clipping Plane (f):", GLUI_EDITTEXT_FLOAT, &f, -1, updateVarsText);
    GLUI_EditText *edit_rotSpeed = glui->add_edittext_to_panel(panel_values, "Rotation Speed:", GLUI_EDITTEXT_FLOAT, &rotationSpeed, -1, updateVarsText);
    GLUI_EditText *edit_transSpeed = glui->add_edittext_to_panel(panel_values, "Translation Speed:", GLUI_EDITTEXT_FLOAT, &translationSpeed, -1, updateVarsText);
    GLUI_EditText *edit_scaleFactor = glui->add_edittext_to_panel(panel_values, "Scale Factor:", GLUI_EDITTEXT_FLOAT, &scaleFactor, -1, updateVarsText);

    GLUI_Panel *panel_live = glui -> add_panel("Current Projection Values");    // Create "Current Projection Values" panel
    // Displays current theta, aspect, n, f, width and height values
    static_theta = glui -> add_statictext_to_panel(panel_live, theta_val);
    static_aspect = glui -> add_statictext_to_panel(panel_live, aspect_val);
    static_n = glui -> add_statictext_to_panel(panel_live, n_val);
    static_f = glui -> add_statictext_to_panel(panel_live, f_val);
    static_width = glui -> add_statictext_to_panel(panel_live, width_val);
    static_height = glui -> add_statictext_to_panel(panel_live, height_val);
    glui -> add_separator();
    GLUI_Button *quitButton = glui -> add_button( "Quit", -1, exit);    // Quit Button

    instructions_glui = GLUI_Master.create_glui("Instructions", 0, 0, 0);   // Create a new window to display usage instructions with flag 0, and at position (0,0) of the screen (top left)
    instructions_glui->set_main_gfx_window(MAIN_WINDOW);    // Set the main window as the main graphics window that the GLUI should operate on
    
    
    // Adds multiple static texts containing instructions for the program (GLUI doesn't understand `\n`, so we couldn't have just one static text)
    instructions_glui->add_statictext(""); // Empty static text for spacing
    GLUI_StaticText *howToUse11 = instructions_glui -> add_statictext("CLOSING THIS WINDOW WITH THE X BUTTON WILL TERMINATE THE WHOLE PROGRAM!");
    instructions_glui->add_statictext(""); // Empty static text for spacing
    GLUI_StaticText *howToUse1 = instructions_glui -> add_statictext("Use W, A, S, D keys to move the object horizontally (x-axis and z-axis)");
    GLUI_StaticText *howToUse2 = instructions_glui -> add_statictext("Use Arrow Up and Arrow Down to move object up and down");
    GLUI_StaticText *howToUse3 = instructions_glui -> add_statictext("Drag with Left Click horizontally to rotate around the y-axis and vertically to rotate around the x-axis (this simulates a z-axis rotation as well)");
    GLUI_StaticText *howToUse4 = instructions_glui -> add_statictext("Drag with Right Click horizontally to rotate around the z-axis only");
    GLUI_StaticText *howToUse5 = instructions_glui -> add_statictext("Use Scroll Wheel Up to increase the object's size");
    GLUI_StaticText *howToUse6 = instructions_glui -> add_statictext("Use Scroll Wheel Down to decrease the object's size");
    GLUI_StaticText *howToUse7 = instructions_glui -> add_statictext("Use Middle Click to reset all transformations");
    GLUI_StaticText *howToUse8 = instructions_glui -> add_statictext("Use Q or ESC to close the program");
    GLUI_StaticText *howToUse9 = instructions_glui -> add_statictext("You can also use the menu on the right. Enjoy!");
    instructions_glui->add_statictext(""); // Empty static text for spacing
    GLUI_StaticText *howToUse10 = instructions_glui -> add_statictext("Made by Panagiotis Karoutsos and Evgenia Baka");
    
    // Sync all the live variables in the GLUI interface
    glui -> sync_live();
    GLUI_Master.set_glutIdleFunc(idle); // We register the idle function in the GLUI
    
    
}


    



int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(400, 100);
    MAIN_WINDOW = glutCreateWindow("View Transformation");
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    // Initially sets the static text strings
    sprintf(theta_val, "Theta: %.1f", theta);
    sprintf(aspect_val, "Aspect Ratio: %.1f", aspect);
    sprintf(n_val, "n: %.1f", n);
    sprintf(f_val, "f: %.1f", f);
    sprintf(width_val, "Window Width: %d", width);
    sprintf(height_val, "Window Height: %d", height);
    sprintf(rotationX_val, "Rotation X: %.2f", (angleX/rotationSpeed));
    sprintf(rotationY_val, "Rotation Y: %.2f", (angleY/rotationSpeed));
    sprintf(rotationZ_val, "Rotation Z: %.2f", (angleZ/rotationSpeed));
    sprintf(rotationSpeed_val, "Rotation Speed: %.2f", rotationSpeed);
    sprintf(tx_val, "X-axis Translation: %.2f", tx);
    sprintf(ty_val, "Y-axis Translation: %.2f", ty);
    sprintf(tz_val, "Y-axis Translation: %.2f", tz);
    sprintf(translationSpeed_val, "Translation Speed: %.2f.2f", translationSpeed);

    glutDisplayFunc(drawScene); // Register our Display Function as drawScene
    setup_glui();   // Draw GLUI
    
    // Remaining function registrations
    glutKeyboardUpFunc(handleKeyUp);
    glutSpecialUpFunc(handleSpecialKeyUp);
    glutMotionFunc(handleMouseMotion);

    
    // Enter the GLUT event loop
    glutMainLoop();
    return 0;
}