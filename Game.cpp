//
// Created by julie on 13/02/17.
//

#include "Game.h"
#include "FogParams.h"

/*------------------------------------CONSTRUCTOR/DESTRUCTOR-----------------------------------------*/
Game::Game()
        : Keys(), ProcessedKeys(), Cam(glm::vec3(0.0f, 0.0f, 0.0f)), lastX(0), lastY(0),
          firstMouse(GL_TRUE), DEBUG(GL_FALSE), gen(std::random_device()()) {}

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

    // Load shaders
    vector<Shader> shaders;
    shaders.push_back(ResourceManager::LoadShader("./shaders/water.vs", "./shaders/water.fs", nullptr, "water",
                                                  {"shaders/LIGHT.fs", "shaders/FOG.fs"}));
    shaders.push_back(ResourceManager::LoadShader("./shaders/skybox.vs", "./shaders/skybox.fs", nullptr, "skybox"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/model.vs", "./shaders/model.fs", "shaders/model.gs", "model",
                                                 {"shaders/LIGHT.fs", "shaders/FOG.fs"}));
    shaders.push_back(ResourceManager::LoadShader("./shaders/text.vs", "./shaders/text.fs", nullptr, "text"));
    shaders.push_back(ResourceManager::LoadShader("shaders/debug.vs", "shaders/debug.fs", "shaders/debug.gs", "debug"));
    shaders.push_back(ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", "shaders/particle.gs", "particle"));

    this->setConstantShadersUniforms(shaders);

    // Load textures
    // Floor
    GLfloat size(2000);
    Plane water_surface(glm::vec3(-size/2,-size*0.45,0), glm::vec2(size),
                ResourceManager::LoadTexture("textures/Water_NormalMap.png", GL_FALSE, GL_TRUE, "water_normals"),
                ResourceManager::LoadCubemap(Game::get_skybox("./textures/skybox/hw_deepsea/underwater_", ".png"), "skybox"));
    water_surface.Rotation.x=-90;
    planes.push_back(water_surface);

    // Particle
    Texture2D bubble_tex = ResourceManager::LoadTexture("textures/bubble.png", GL_TRUE, GL_FALSE, "bubble");
    this->add_bubbles(bubble_tex, 20);

    // Models
    this->add_models();

    // Set render-specific controls
    Renderer.push_back(new Sprite_Renderer(shaders[WATER-1], 1.0f));
    Renderer.push_back(new Sprite_Renderer(shaders[SKYBOX-1]));
    Renderer.push_back(new Sprite_Renderer(shaders[PARTICLE-1], 0));
    T_Renderer = new Text_Renderer(this->Width, this->Height);
    T_Renderer->Load("./fonts/Futura_Bold_Font/a_FuturaOrto-Bold_2258.ttf",50);
}

/*------------------------------------UPDATE-----------------------------------------*/
void Game::Update(GLfloat dt, GLfloat currenttime) {
    static double bubble_count_down = 0;

    Shader Mshader = ResourceManager::GetShader("model"),
            Wshader = ResourceManager::GetShader("water"),
            Pshader = ResourceManager::GetShader("particle");
    std::uniform_int_distribution<unsigned int> proba_new_bubble(0, 30);
    std::uniform_int_distribution<unsigned int> nb_new_fish_bubble(1, 3);
    Texture2D tex_bubble = ResourceManager::GetTexture("bubble");
    bool new_bubble;

    this->State_manager.Update(dt);

    //Add value specific to the game to shaders
    this->State_manager.Active(Wshader);
    Wshader.SetVector3f("viewPos", this->Cam.Position);
    Wshader.SetFloat("time", currenttime);

    this->State_manager.Active(Mshader);
    Mshader.SetVector3f("viewPos", this->Cam.Position);
    Mshader.SetFloat("time", currenttime);

    this->State_manager.Active(Pshader);
    Pshader.SetVector3f("cam_up", this->Cam.Up);
    Pshader.SetVector3f("cam_right", this->Cam.Right);

    new_bubble = bubble_count_down < currenttime;
    //Update models and Create fishes's bubble trail
    for (GameModel &mod : this->models){
        mod.Update(dt);
        if (!mod.wings && new_bubble){
            this->add_trailing_bubbles(tex_bubble, mod, nb_new_fish_bubble(this->gen));
        }
    }
    if(new_bubble)
        bubble_count_down+=0.3;

    //Update bubbles, destroy if need be and create new ones
    for (auto it = this->bubbles.begin(); it != this->bubbles.end() ;) {
        it->Update(dt, currenttime);
        if (it->Decay <= 0 || it->Position.y > 90)
            it = this->bubbles.erase(it);
        else
            it++;
    }
    if(!proba_new_bubble(this->gen)){
        this->add_bubbles(tex_bubble, 1);
    }
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

    GLdouble xoffset = xpos - this->lastX;
    GLdouble yoffset = this->lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    this->lastX = xpos;
    this->lastY = ypos;

    this->Cam.ProcessMouseMovement(static_cast<GLfloat>(xoffset), static_cast<GLfloat>(yoffset));
}


void Game::ProcessMouseScroll(GLdouble yoffset)
{
    this->Cam.ProcessMouseScroll(static_cast<GLfloat>(yoffset));
}

/*------------------------------------RENDER-----------------------------------------*/
void Game::Render()
{
    // Create camera transformation
    glm::mat4 view3D, projection3D, projection2D;
    view3D = this->Cam.GetViewMatrix();
    projection3D = glm::perspective(glm::radians(this->Cam.Zoom), static_cast<GLfloat>(this->Width)/static_cast<GLfloat>(this->Height), 0.1f, 1750.0f);
    projection2D = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

    //should be drawn at the end but other objects have transparency
    Renderer[1]->DrawSprite(this->State_manager, ResourceManager::GetCubemap("skybox"), projection3D, view3D);
    for(GameModel &mod : this->models) {
        if(!mod.cullface)
            glDisable(GL_CULL_FACE);
        mod.Draw(this->State_manager, ResourceManager::GetShader("model"), projection3D, view3D);
        if(this->DEBUG)
            mod.Draw(this->State_manager, ResourceManager::GetShader("debug"), projection3D, view3D);
        if(!mod.cullface)
            glEnable(GL_CULL_FACE);
    }
    bool top_of_water = this->Cam.Position.y < 100;
    if(top_of_water)
        glCullFace(GL_FRONT);
    for(Plane &plane : this->planes)
        plane.Draw(this->State_manager, *Renderer[0], projection3D, view3D);
    if(top_of_water)
        glCullFace(GL_BACK);
    for(Particle &p : this->bubbles)
        p.Draw(this->State_manager, *Renderer[2], projection3D, view3D);
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


void Game::setConstantShadersUniforms(vector<Shader> &shaders){
    // Configure shaders for value that won't change all throught the program
    shaders[TEXT-1].SetMatrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f), GL_TRUE);
    for (const int i  : {MODEL-1, WATER-1}) {
        shaders[i].SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f, GL_TRUE);
        shaders[i].SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        shaders[i].SetVector3f("dirLight.diffuse", 0.3f, 0.4f, 0.5f);
        shaders[i].SetVector3f("dirLight.specular", 0.4f, 0.5f, 0.7f);
        shaders[i].SetInteger("fogParams.iEquation", FogParameters::iFogEquation);
        shaders[i].SetVector4f("fogParams.vFogColor", FogParameters::vFogColor);
        if(FogParameters::iFogEquation == FOG_EQUATION_LINEAR) {
            shaders[i].SetFloat("fogParams.fStart", FogParameters::fStart);
            shaders[i].SetFloat("fogParams.fEnd", FogParameters::fEnd);
        }
        else
            shaders[i].SetFloat("fogParams.fDensity", FogParameters::fDensity);
    }
}

void Game::add_models() {
    std::uniform_int_distribution<int> dis(10,80);
    std::uniform_int_distribution<int> dis2(0,1);
    std::uniform_int_distribution<int> dis3(10,100);

    GameModel mod = GameModel("models3D/shark/Model_D0202004/wshark.obj", "shark");
    mod.Rotation.y = 270;
    mod.Size = glm::vec3(0.005);
    mod.Position = glm::vec3(50,0,-50);
    mod.centerpoint = glm::vec3(100, 0, 100);
    mod.speed = 20;
    mod.deformation_magnitude = 1.0;
    this->models.push_back(mod);
    mod = GameModel("models3D/phenix_nocullface/Model_C1018410/fenghuang5.obj", "phenix");
    mod.Size = glm::vec3(0.1);
    mod.centerpoint = glm::vec3(50, 250, 0);
    mod.starting_height = 250;
    mod.outline = false;
    mod.cullface = false;
    mod.wings = true;
    mod.deformation_magnitude = 0.1;
    this->models.push_back(mod);
    mod = GameModel("models3D/hummingbird/hummingbird.obj", "hummingbird");
    mod.Size = glm::vec3(0.01);
    mod.Position = glm::vec3(50,150,-20);
    mod.starting_height = 150;
    mod.centerpoint = glm::vec3(50,150,-140);
    mod.speed = -60;
    mod.Rotation.y = -90;
    mod.outline = false;
    mod.wings = true;
    mod.deformation_magnitude = 0.007;
    this->models.push_back(mod);
    mod = GameModel("models3D/ray/something_01.obj", "ray");
    mod.Rotation = glm::vec3(290, 30, -30);
    mod.Size = glm::vec3(0.05);
    mod.Position = glm::vec3(-20,20,-20);
    mod.starting_height = 20;
    mod.centerpoint = glm::vec3(0,20,-20);
    mod.speed = -30;
    mod.wings = true;
    mod.deformation_magnitude = 0.03;
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
            mod.deformation_magnitude = 0.8;
            mod.Size = glm::vec3(0.02);
            mod.Position = glm::vec3(0, 0, dis(this->gen));
            mod.starting_height = dis(this->gen);
            mod.centerpoint = mod.Position;
            mod.speed = dis3(this->gen);
            mod.centerpoint.z += -30;
            if(dis2(this->gen)){
                mod.Rotation.y = -90;
                mod.speed = -mod.speed;
            }
            else
                mod.Rotation.y = 90;
            this->models.push_back(mod);
        }
    }
}

void Game::add_bubbles(Texture2D &tex, unsigned int n){
    std::uniform_real_distribution<float> disZ(0.1, 500);
    std::uniform_int_distribution<int> disX(-(this->Width/2), this->Width/2);

    for (int i = 0; i < n ; ++i) {
        glm::vec3 pos = glm::vec3(this->Cam.Position)+(this->Cam.Front*disZ(this->gen));
        pos.x+=disX(this->gen);
        pos.y -= 200;
        this->bubbles.emplace_back(pos, glm::vec2(1), tex, glm::vec3(0,10,0));
    }
}

void Game::add_trailing_bubbles(Texture2D &tex, GameModel &mod, unsigned int n){
    std::uniform_real_distribution<float> dis(-3, 3);

    for (int i = 0; i < n ; ++i) {
        glm::vec3 tmp = glm::vec3(mod.Last_true_pos * glm::vec4(mod.Position, 1.0));
        glm::vec3 pos = tmp + glm::vec3(dis(this->gen), dis(this->gen), dis(this->gen));
        glm::vec3 vel = (tmp - glm::vec3(mod.New_true_pos * glm::vec4(mod.Position, 1.0))) * glm::vec3(100);
        vel.y += dis(this->gen);
        this->bubbles.emplace_back(pos, glm::vec2(0.5), tex, vel, 0.5);
    }
}