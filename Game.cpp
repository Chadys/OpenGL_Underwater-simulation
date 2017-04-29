//
// Created by julie on 13/02/17.
//

#include "Game.h"


/*------------------------------------CONSTRUCTOR/DESTRUCTOR-----------------------------------------*/
Game::Game()
        : Keys(), ProcessedKeys(), Cam(glm::vec3(0.0f, 0.0f, 0.0f)), lastX(0), lastY(0), firstMouse(GL_TRUE), DEBUG(GL_FALSE) {}

Game::~Game()
{
    for(Sprite_Renderer *s : this->Renderer)
        delete s;
    delete T_Renderer;
}

/*------------------------------------INIT-----------------------------------------*/
void Game::Init()
{
    //Adapt camera speed for 2D and depending on the screen size (it was originalled choosen for a 800x600 screen)
    this->Cam.MovementSpeed = 100.0f*this->Width/800;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-50,50);
    std::uniform_int_distribution<int> dis2(0,360);

    // Load shaders
    Shader shader = ResourceManager::LoadShader("./shaders/jeu.vs", "./shaders/jeu.fs", "./shaders/jeu.gs", "jeu");
    ResourceManager::LoadShader("./shaders/skybox.vs", "./shaders/skybox.fs", nullptr, "skybox");
    Shader mshader = ResourceManager::LoadShader("./shaders/model.vs", "./shaders/model.fs", nullptr, "model");
    Shader tshader = ResourceManager::LoadShader("./shaders/text.vs", "./shaders/text.fs", nullptr, "text");
    ResourceManager::LoadShader("shaders/debug.vs", "shaders/debug.fs", "shaders/debug.gs", "debug");

    // Configure shaders
    shader.SetInteger("sprite", 0, true);
    tshader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f), true);
    mshader.SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f, true);
    mshader.SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f, true);
    mshader.SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f, true);
    mshader.SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f, true);


    // Load textures
    // Floor
    GLfloat size(1000);
    Object3D floor(glm::vec3(-size/2,-size*0.506,0), glm::vec2(size), ResourceManager::LoadTexture("./textures/sol.png", GL_FALSE, GL_TRUE, "floor"));
    floor.Rotation.x=-90;
    objects.push_back(floor);

    // Cubemap (Skybox)
    ResourceManager::LoadCubemap(Game::get_skybox("./textures/skybox/hw_deepsea/underwater_", ".png"), "skybox");

    // Models
    GameModel mod = GameModel("models3D/shark/Model_D0202004/wshark.obj", "shark");
    mod.Rotation.y = 180;
    mod.Size = glm::vec3(0.005);
    mod.Position = glm::vec3(50,0,-50);
    this->models.push_back(mod);
    mod = GameModel("models3D/phenix_nocullface/Model_C1018410/fenghuang5.obj", "phenix");
    mod.Size = glm::vec3(0.1);
    mod.Position.y = 250.0f;
    mod.outline = false;
    mod.cullface = false;
    this->models.push_back(mod);
    mod = GameModel("models3D/hummingbird/hummingbird.obj", "hummingbird");
    mod.Size = glm::vec3(0.01);
    mod.Position = glm::vec3(50,150,-100);
    mod.outline = false;
    this->models.push_back(mod);
    mod = GameModel("models3D/ray/something_01.obj", "ray");
    mod.Rotation = glm::vec3(270, 50, 0);
    mod.Size = glm::vec3(0.05);
    mod.Position = glm::vec3(-20,20,-20);
    this->models.push_back(mod);
    for (char i = '0', max = '9'; i <= '1'; ++i, max='5') {
        for (int j = '1'; j <= max; ++j) {
            string filename("models3D/Tropical Fish Pack/TropicalFish"), name("fish");
            filename+=i;
            filename+=j;
            filename+=".obj";
            name+=i;
            name+=j;
            mod = GameModel(filename, name);
            mod.Size = glm::vec3(0.02);
            mod.Position = glm::vec3(dis(gen), dis(gen)+50, dis(gen));
            mod.Rotation.y = dis2(gen);
            this->models.push_back(mod);
        }
    }

    // Texts
    this->texts.push_back(Text("GAME OVER", glm::vec2(this->Width/2, this->Height/3), this->Width/1920*2, glm::vec4(1)));
    this->texts.push_back(Text("Press ENTER to try again", glm::vec2(this->Width/2, this->Height*0.66f), this->Width/1920*1.5, glm::vec4(1)));

    // Set render-specific controls
    Renderer.push_back(new Sprite_Renderer(shader, 1.0f));
    Renderer.push_back(new Sprite_Renderer(shader, 15.0f));
    Renderer.push_back(new Sprite_Renderer(ResourceManager::GetShader("skybox")));
    T_Renderer = new Text_Renderer(this->Width, this->Height);
    T_Renderer->Load("./fonts/Futura_Bold_Font/a_FuturaOrto-Bold_2258.ttf",50);
}

/*------------------------------------UPDATE-----------------------------------------*/
void Game::Update(GLfloat dt)
{
    Shader shader = ResourceManager::GetShader("jeu"), mshader = ResourceManager::GetShader("model");

    this->State_manager.Update(dt);

    shader.SetVector3f("viewPos", this->Cam.Position);
    mshader.SetVector3f("viewPos", this->Cam.Position);
}

/*------------------------------------PROCESSORS-----------------------------------------*/
void Game::ProcessInput(GLfloat dt)
{
    if(this->Keys[GLFW_KEY_ESCAPE]){
    }

    // Cam
    if(this->Keys[GLFW_KEY_S])
        this->Cam.ProcessKeyboard(BACKWARD, dt);
    if(this->Keys[GLFW_KEY_A])
        this->Cam.ProcessKeyboard(LEFT, dt);
    if(this->Keys[GLFW_KEY_D])
        this->Cam.ProcessKeyboard(RIGHT, dt);
    if(this->Keys[GLFW_KEY_W])
        this->Cam.ProcessKeyboard(FORWARD, dt);
    if(this->Keys[GLFW_KEY_KP_ADD] && !this->ProcessedKeys[GLFW_KEY_KP_ADD]){
        this->ProcessedKeys[GLFW_KEY_KP_ADD] = GL_TRUE;
    }
    if(this->Keys[GLFW_KEY_KP_SUBTRACT] && !this->ProcessedKeys[GLFW_KEY_KP_SUBTRACT]){
        this->ProcessedKeys[GLFW_KEY_KP_SUBTRACT] = GL_TRUE;
    }
    //DEBUG
    if(this->Keys[GLFW_KEY_G] && !this->ProcessedKeys[GLFW_KEY_G]){
        this->ProcessedKeys[GLFW_KEY_G] = GL_TRUE;
        this->DEBUG = !this->DEBUG;
    }
    // Turbo
    if(this->Keys[GLFW_KEY_SPACE] && !this->ProcessedKeys[GLFW_KEY_SPACE]){
    }
}

void Game::ProcessMouseMovement(GLdouble xpos, GLdouble ypos)
{
    if(this->firstMouse)
    {
        this->lastX = xpos;
        this->lastY = ypos;
        this->firstMouse = false;
    }

    GLfloat xoffset = xpos - this->lastX;
    GLfloat yoffset = this->lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    this->lastX = xpos;
    this->lastY = ypos;

    this->Cam.ProcessMouseMovement(xoffset, yoffset);
}


void Game::ProcessMouseScroll(GLdouble yoffset)
{
    this->Cam.ProcessMouseScroll(yoffset);
}

/*------------------------------------RENDER-----------------------------------------*/
void Game::Render()
{
    // Create camera transformation
    glm::mat4 view3D, projection3D, projection2D;
    view3D = this->Cam.GetViewMatrix();
    projection3D = glm::perspective(glm::radians(this->Cam.Zoom), static_cast<GLfloat>(this->Width)/static_cast<GLfloat>(this->Height), 0.1f, 1000.0f);
    projection2D = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

    for (Square &s : squares)
        s.Draw(this->State_manager,*Renderer[0], projection2D, glm::mat4(), GL_TRUE);
//    objects[0].Draw(this->State_manager, *Renderer[1], glm::vec2(-this->Cam.Position.x, this->Cam.Position.z)/100.0f,projection3D, glm::mat4(glm::mat3(view3D)));
    for(int i = 1; i < this->objects.size(); i++)
        objects[i].Draw(this->State_manager, *Renderer[0], glm::vec2(0),projection3D, view3D);
    for(int i = 0; i < this->models.size(); i++) {
        if(!models[i].cullface)
            glDisable(GL_CULL_FACE);
        models[i].Draw(this->State_manager, ResourceManager::GetShader("model"), projection3D, view3D);
        if(this->DEBUG)
            models[i].Draw(this->State_manager, ResourceManager::GetShader("debug"), projection3D, view3D);
        if(!models[i].cullface)
            glEnable(GL_CULL_FACE);
    }
    Renderer[2]->DrawSprite(this->State_manager, ResourceManager::GetCubemap("skybox"), projection3D, view3D);
}

/*------------------------------------MISCELLANOUS-----------------------------------------*/


vector<string> Game::get_skybox(string path, string ext){
    vector<string> faces;
    faces.push_back(path+"rt"+ext);
    faces.push_back(path+"lf"+ext);
    faces.push_back(path+"up"+ext);
    faces.push_back(path+"dn"+ext);
    faces.push_back(path+"bk"+ext);
    faces.push_back(path+"ft"+ext);
    return faces;
}
