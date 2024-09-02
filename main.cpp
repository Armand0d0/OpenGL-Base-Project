#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>
#include <fstream>
#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define X glm::vec3(1.f,.0f,.0f)
#define Y glm::vec3(0.f,1.f,.0f)
#define Z glm::vec3(0.f,.0f,1.0f)
#define X1 glm::vec4(1.f,.0f,.0f,1.0f)
#define Y1 glm::vec4(0.f,1.f,.0f,1.0f)
#define Z1 glm::vec4(0.f,.0f,1.0f,1.0f)

#define TARGET_UPS 60.
#define SECOND_PER_UPDATE 1./TARGET_UPS

using namespace std;

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

string readFile(const char* filename){

    // 1. retrieve the shader source code from filePath
    std::string shaderCode;
    std::string fragmentCode;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        shaderFile.open(filename);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderCode   = shaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return shaderCode;
}

glm::vec3 rotate3(glm::vec3 v, float angle, glm::vec3 axis){

    glm::vec4 v4 = glm::vec4(v.x,v.y,v.z,1.0f);
    v4 = glm::rotate(glm::mat4(1.0f),angle,axis)*v4;

    return glm::vec3(v4.x,v4.y,v4.z);
}
glm::vec3 normalize(glm::vec3 v){
    float n = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    if(n < 0.000001){
        return glm::vec3(0.,0.,0.);
    } 
    return v/n;
}

void initIMGUI(GLFWwindow* window){
    // Setup Dear ImGui context
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


// Setup Platform/Renderer backends
ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
ImGui_ImplOpenGL3_Init();
}

struct windowParams{
    int width;
    int height;
    float ratio;
}typedef windowParams;
windowParams* initWindowsParams(){
    windowParams* wp = new windowParams;
    wp->width = 1920;
    wp->height = 1080;
    wp->ratio = (float)wp->width/(float)wp->height;
    return wp;
}

struct mouseParams{
    glm::vec2 mouseSensivity;
}typedef mouseParams;
mouseParams* initMouseParams(){
    mouseParams* mp = new mouseParams;
    mp->mouseSensivity = glm::vec2(1.f,1.f);
    return mp;
}

struct inputData{
    glm::vec2 mousePos;
    float forward;
    float sideways;
    float upwards;
    float running;
     //hud parameters keys
    bool key_Z;
    bool key_V;
    bool key_E;
    bool key_B;
    bool key_N;
    bool key_ENTER;
    //hud parameters
    int wireMode;
    int showEdges;
    int showBackSideEdges;
    int showVertexIndicies;
    int showNormals;
    int debugMode;
} typedef inputData;
inputData* initInputData(){
    inputData* in;
    in = new inputData;
    in->mousePos = glm::vec2(0.f);
    in->forward = 0.f;
    in->sideways = 0.f;
    in->upwards = 0.f;
    in->running = 0.f;
     //hud parameters keys
    in->key_Z = false;
    in->key_V = false;
    in->key_E = false;
    in->key_B = false;
    in->key_N = false;
    in->key_ENTER = false;
    //hud parameters
    in->wireMode = 0;
    in->showEdges = 0;
    in->showBackSideEdges = 1;
    in->showVertexIndicies = 0;
    in->showNormals = 0;
    in->debugMode = 0;
    return in;
}
struct camera{
    glm::vec3 position;
    glm::vec2 angleRotation;
    glm::vec3 relativeXAxis;
    glm::vec3 relativeZAxis;
    float speed;
    float runningSpeedFactor;

}typedef camera;
camera* initCamera(windowParams* wp){
    camera* cam = new camera;
    cam->position = glm::vec3(0.f,0.0f,-2.0f);
    cam->angleRotation = glm::vec2(0.,0.);
    cam->relativeZAxis = Z;
    cam->speed = 0.02f;
    cam->runningSpeedFactor = 5.0;
    return cam;
}

struct renderData{
    unsigned int VAO;
    unsigned int shaderProgram;
    unsigned int texture;
}typedef renderData;
renderData* initRenderData(unsigned int VAO,unsigned int shaderProgram,unsigned int texture){
    renderData* rd = new renderData;
    rd->VAO = VAO;
    rd->shaderProgram = shaderProgram;
    rd->texture = texture;
    return rd;
}
unsigned int compileShader(const char * fileName,unsigned int shaderType){
    string sourceString = readFile(fileName);
    const char *shaderSource = sourceString.c_str();
    unsigned int shader;
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    //check for compilation errors
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::COMPILATION_FAILED\n"<< fileName << " : " << infoLog << endl;
    }
    return shader;
}
unsigned int buildShaderProgram(){
    unsigned int vertexShader = compileShader("./vertexShader.glsl",GL_VERTEX_SHADER);
    unsigned int geometryShader = compileShader("./geometryShader.glsl",GL_GEOMETRY_SHADER);
    unsigned int fragmentShader = compileShader("./fragmentShader.glsl",GL_FRAGMENT_SHADER);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //check for errors
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout<<"ERROR LINKING SHADER PROGRAM : "<< infoLog<<endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader); 
    return shaderProgram;
}
unsigned int loadMesh(float* vertices,unsigned int vertexCount,unsigned int* indices,unsigned int indexCount){
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int buffer;
    glGenBuffers(1,&buffer);
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    glBufferData(GL_ARRAY_BUFFER,vertexCount,vertices,GL_STATIC_DRAW);

    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount, indices, GL_STATIC_DRAW); 


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); 
    return VAO;
}
unsigned int loadTexture(const char* fileName){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int texWidth, texHeight, nrChannels;
    unsigned char *data = stbi_load(fileName, &texWidth, &texHeight, &nrChannels, 0); 

    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}
void onePressToggle(GLFWwindow* window, int key, bool* was_pressed, int* toggle){
    if(glfwGetKey(window,key) == GLFW_PRESS){
        *was_pressed = true;
    }else if(*was_pressed){
        *toggle = 1-*toggle;
        *was_pressed = false;

    }
}
void processInputs(GLFWwindow* window, inputData * in,mouseParams* mp){

    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);   
        }
        
        double mx,my;
        glfwGetCursorPos(window,&mx,&my);
        in->mousePos = glm::vec2((float)mx*mp->mouseSensivity.x,(float)my*mp->mouseSensivity.x);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            in->forward = 1;   
        }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            in->forward = -1;
        }else{
            in->forward = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            in->sideways = 1;   
        }else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            in->sideways = -1;
        }else{
            in->sideways = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            in->upwards = 1;   
        }else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
            in->upwards = -1;
        }else{
            in->upwards = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
            in->running = 1;   
        }else{
            in->running = 0;
        } 
        onePressToggle(window, GLFW_KEY_Z,&(in->key_Z),&(in->wireMode));
        if(in->wireMode == 0){
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        onePressToggle(window, GLFW_KEY_V,&(in->key_V),&(in->showVertexIndicies));
        onePressToggle(window, GLFW_KEY_E,&(in->key_E),&(in->showEdges));
        onePressToggle(window, GLFW_KEY_B,&(in->key_B),&(in->showBackSideEdges));
        onePressToggle(window, GLFW_KEY_N,&(in->key_N),&(in->showNormals));
        onePressToggle(window, GLFW_KEY_ENTER,&(in->key_ENTER),&(in->debugMode));
        if(in->debugMode == 0){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }else{
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
}

void update(inputData* in, windowParams* wp, camera* cam){

        //Camera mouvement
        float camSpeed = cam->speed*(1.f+in->running*(cam->runningSpeedFactor-1.f));
        cam->position += normalize(cam->relativeZAxis*in->forward + cam->relativeXAxis*in->sideways - Y*in->upwards)*camSpeed;
}


void render(GLFWwindow* window, windowParams* wp, renderData* rd, camera* cam, inputData* in){
        
        //Camera orientation
        if(in->debugMode == 0){
            cam->angleRotation= glm::vec2(-(-in->mousePos.x + 0.5f*wp->width)/wp->width,-(-in->mousePos.y + 0.5f*wp->height)/wp->height);
            cam->relativeXAxis = rotate3(X,-cam->angleRotation.x,Y);
            cam->relativeZAxis = rotate3(cam->relativeXAxis,glm::radians(-90.0),Y);
            cam->relativeZAxis = rotate3(cam->relativeZAxis,-cam->angleRotation.y,cam->relativeXAxis);
        }
        //camera setup
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::rotate(viewMatrix,cam->angleRotation.x,Y);
        viewMatrix = glm::rotate(viewMatrix,cam->angleRotation.y,cam->relativeXAxis);
        viewMatrix = glm::translate(viewMatrix,cam->position);
        glm::mat4 projMatrix = glm::perspective(glm::radians(60.0f),wp->ratio,0.0001f,100.0f);
        //update uniform variables
        glUniformMatrix4fv(glGetUniformLocation(rd->shaderProgram,"viewMatrix"),1,GL_FALSE,glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(glGetUniformLocation(rd->shaderProgram,"projMatrix"),1,GL_FALSE,glm::value_ptr(projMatrix));
        glUniform1f(glGetUniformLocation(rd->shaderProgram,"ratio"),wp->ratio);
        glUniform1i(glGetUniformLocation(rd->shaderProgram,"showEdges"),in->showEdges);
        glUniform1i(glGetUniformLocation(rd->shaderProgram,"showBackSideEdges"),in->showBackSideEdges);
        glUniform1i(glGetUniformLocation(rd->shaderProgram,"showNormals"),in->showNormals);
        glUniform1i(glGetUniformLocation(rd->shaderProgram,"showVertexIndicies"),in->showVertexIndicies);
        glUniform1f(glGetUniformLocation(rd->shaderProgram, "time"), glfwGetTime());
        //Draw
        glClearColor(135./255.,209./255.,235/255.,1.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rd->texture);
        glUseProgram(rd->shaderProgram);
        glBindVertexArray(rd->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES,39/*sizeof(gameItem->indices)*/,GL_UNSIGNED_INT,0);
        //glDrawElements(GL_LINE_STRIP,sizeof(indices),GL_UNSIGNED_INT,0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
}
int main(){
    GLFWwindow* window;
    int width = 1920,
    height = 1080;

    if (!glfwInit()){
            cout<<"The glfw window intialisation failed !"<<endl;
            return -1;
    }
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    //----------------------------------------------------------------------------------------- MESH DATA
    float vertices[] = {
    //positions             texCoords
     0.5f,  0.5f, 0.5f,     1.f,0.f,         // top right 
     0.5f, -0.5f, 0.5f,     1.f,1.f,        // bottom right
    -0.5f, -0.5f, 0.5f,     0.f,1.f,         // bottom left
    -0.5f,  0.5f, 0.5f,     0.f,0.f,        // top left 
    //Back
     0.5f,  0.5f, -0.5f,     1.,0.,        // top right
     0.5f, -0.5f, -0.5f,     1.,1.,       // bottom right
    -0.5f, -0.5f, -0.5f,     0.,1.,       // bottom left
    -0.5f,  0.5f, -0.5f,      0.,0.,       // top left */
    };
    unsigned int indices[] = {  // cube faces
        0, 1, 3,    //front
        1, 2, 3,
        1, 2, 6,    //botom 
        1, 6, 5,
        5, 6, 7,    //back
        5, 7, 4,
        0, 5, 4,    //right  
        1, 0, 5,
        3, 2, 6,    //left
        6, 3, 7,
        0, 3, 7,    //top
        0, 4, 7,//*/

    };  
    //-----------------------------------------------------------------------------------------

    unsigned int shaderProgram = buildShaderProgram();
    unsigned int VAO = loadMesh(vertices,sizeof(vertices),indices,sizeof(indices));
    unsigned int numbersTexture = loadTexture("screen.png");
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnable(GL_DEPTH_TEST);  

    inputData* in = initInputData();
    mouseParams* mp = initMouseParams();
    windowParams* wp = initWindowsParams();
    camera* cam = initCamera(wp);
    renderData* rd = initRenderData(VAO,shaderProgram,numbersTexture);



    double previous = glfwGetTime();
    double lag = 0.;
    while (!glfwWindowShouldClose(window))  //------------------------------------------------------------------------------------------LOOP
    {
        double current = glfwGetTime();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        processInputs(window,in,mp);

        

        int counter = 0;
        double t1 = glfwGetTime();
        while (lag >= SECOND_PER_UPDATE){   // NEED average update time < SECOND_PER_UPDATE 
            counter++;     
           update(in,wp,cam);
           lag-=SECOND_PER_UPDATE;
        }
        double t2 = glfwGetTime();
        cout << "FPS : " << 1./elapsed << "   updates per frame : "<< counter << "  average update time : "<< (counter == 0 ? 0 : (t2-t1)/(float)counter )
        << "   SECOND_PER_UPDATE : " << SECOND_PER_UPDATE <<endl;
        render(window,wp,rd,cam,in);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete in;
    delete mp;
    delete wp;
    delete cam;
    delete rd;
    glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &buffer);//TODO : cleanup VAO, VBO EBO 
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

