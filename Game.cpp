//
// Created by julie on 13/02/17.
//

#include "Game.h"
#include "FogParams.h"

enum SHADER {
    WATER,
    SKYBOX,
    PARTICLE,
    POSTPROD,
    MODEL,
    TEXT,
    DEBUG,
    POSTPROD_CONVO,
    POSTPROD_MOSAIC,
    POSTPROD_SWIRL,
    POSTPROD_WARP,
    POSTPROD_FISHEYE,
    POSTPROD_KALEI,
    POSTPROD_KALEI2,
    POSTPROD_PAINT
};

std::unordered_set<int> Game::no_neighbor;
std::unordered_set<int> Game::convolute;

/*------------------------------------CONSTRUCTOR/DESTRUCTOR-----------------------------------------*/
Game::Game()
        : Keys(), ProcessedKeys(), Cam(glm::vec3(0.0f, 0.0f, 0.0f)), lastX(0), lastY(0),
          firstMouse(GL_TRUE), DEBUG(GL_FALSE), gen(std::random_device()()), effect(PostProd::NO_POSTPROD) {}

Game::~Game()
{
    for(Sprite_Renderer *s : this->Renderer)
        delete s;
    delete T_Renderer;
}

/*------------------------------------INIT-----------------------------------------*/
void Game::Init()
{
    //Adapt camera speed for 2D and depending on the screen size (it was originally chosen for a 800x600 screen)
    this->Cam.MovementSpeed = 50.0f*this->Width/800;

    // Load shaders
    vector<Shader> shaders;
    shaders.push_back(ResourceManager::LoadShader("./shaders/water.vs", "./shaders/water.fs", nullptr, "water",
                                                  {"./shaders/LIGHT.fs", "./shaders/FOG.fs"}));
    shaders.push_back(ResourceManager::LoadShader("./shaders/skybox.vs", "./shaders/skybox.fs", nullptr, "skybox"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/particle.vs", "./shaders/particle.fs", "./shaders/particle.gs", "particle"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/postprocessor.vs", "./shaders/postprocessor.fs", nullptr, "postprocessor"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/model.vs", "./shaders/model.fs", "./shaders/model.gs", "model",
                                                 {"./shaders/LIGHT.fs", "./shaders/FOG.fs"}));
    shaders.push_back(ResourceManager::LoadShader("./shaders/text.vs", "./shaders/text.fs", nullptr, "text"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/debug.vs", "./shaders/debug.fs", "./shaders/debug.gs", "debug"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/postprocessor.vs", "./shaders/postprocessor_convolute.fs", nullptr, "postprocessor_convolute"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/postprocessor.vs", "./shaders/postprocessor_mosaic.fs", nullptr, "postprocessor_mosaic"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/postprocessor.vs", "./shaders/postprocessor_swirl.fs", nullptr, "postprocessor_swirl"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/postprocessor.vs", "./shaders/postprocessor_warp.fs", nullptr, "postprocessor_warp"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/postprocessor.vs", "./shaders/postprocessor_fisheye.fs", nullptr, "postprocessor_fisheye"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/postprocessor.vs", "./shaders/postprocessor_kalei.fs", nullptr, "postprocessor_kalei"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/postprocessor.vs", "./shaders/postprocessor_kalei2.fs", nullptr, "postprocessor_kalei2"));
    shaders.push_back(ResourceManager::LoadShader("./shaders/postprocessor.vs", "./shaders/postprocessor_oil_painting.fs", nullptr, "postprocessor_oil_painting"));

    //init postprod commmon groups
    Game::no_neighbor.insert(PostProd::NO_POSTPROD);
    Game::no_neighbor.insert(PostProd::GREYSCALE);
    Game::no_neighbor.insert(PostProd::INVERT);
    Game::no_neighbor.insert(PostProd::BRIGHT);
    Game::no_neighbor.insert(PostProd::DARK);
    Game::no_neighbor.insert(PostProd::THRESHOLD);
    Game::no_neighbor.insert(PostProd::RED_CANAL);
    Game::no_neighbor.insert(PostProd::GREEN_CANAL);
    Game::no_neighbor.insert(PostProd::BLUE_CANAL);
    Game::no_neighbor.insert(PostProd::BIT_REDUCE);
    Game::no_neighbor.insert(PostProd::HALFTONING);

    Game::convolute.insert(PostProd::AVERAGER);
    Game::convolute.insert(PostProd::SHARPENER);
    Game::convolute.insert(PostProd::GAUSSIAN);
    Game::convolute.insert(PostProd::SOBEL);
    Game::convolute.insert(PostProd::LAPLACIAN);
    Game::convolute.insert(PostProd::SKETCH);

    this->setConstantShadersUniforms(shaders);

    // Load Framebuffer for postprocessing
    ResourceManager::LoadFramebuffer(this->Width, this->Height, "postprod");

    // Load textures
    // Water
    GLfloat size(2000);
    Plane water_surface(glm::vec3(-size/2,-size*0.45,0), glm::vec2(size),
                ResourceManager::LoadTexture("./textures/Water_NormalMap.png", GL_FALSE, GL_TRUE, "water_normals"),
                ResourceManager::LoadCubemap(Game::get_skybox("./textures/skybox/hw_deepsea/underwater_", ".png"), "skybox"));
    ResourceManager::LoadCubemap(Game::get_skybox("./textures/skybox/hw_deepsea_outofwater/underwater_", ".png"), "skybox_outside");
    water_surface.Rotation.x=-90;
    planes.push_back(water_surface);

    // Particle
    Texture2D bubble_tex = ResourceManager::LoadTexture("./textures/bubble.png", GL_TRUE, GL_FALSE, "bubble");
    this->add_bubbles(bubble_tex, 20);

    // Models
    this->add_models();

    // Set render-specific controls
    Renderer.push_back(new Sprite_Renderer(shaders[WATER], 1.0f));
    Renderer.push_back(new Sprite_Renderer(shaders[SKYBOX]));
    Renderer.push_back(new Sprite_Renderer(shaders[PARTICLE], 0));
    Renderer.push_back(new Sprite_Renderer(shaders[POSTPROD], 1.0f));
    T_Renderer = new Text_Renderer(this->Width, this->Height);
    T_Renderer->Load("./fonts/Futura_Bold_Font/a_FuturaOrto-Bold_2258.ttf",50);
}

/*------------------------------------UPDATE-----------------------------------------*/
void Game::Update(GLfloat dt, GLfloat currenttime) {
    static double bubble_count_down = 0;
    static bool underwater = true;

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

    //change water's skybox if camera break through water
    if(!underwater) {
        if (this->Cam.Position.y < 100) {
            underwater = !underwater;
            this->State_manager.tex3D = 0;
            this->planes.front().Reflect = ResourceManager::GetCubemap("skybox");
        }
    }
    else if(this->Cam.Position.y >= 100) {
        underwater = !underwater;
        this->State_manager.tex3D = 0;
        this->planes.front().Reflect = ResourceManager::GetCubemap("skybox_outside");
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
        this->effect = static_cast<PostProd::POSTPROD_EFFECT>(this->effect+1);
        if (this->effect == PostProd::NO_POSTPROD_LAST)
            this->effect = PostProd::NO_POSTPROD;
        this->UpdatePostProd();
    }
    if(this->Keys[GLFW_KEY_KP_SUBTRACT] && !this->ProcessedKeys[GLFW_KEY_KP_SUBTRACT]){
        this->ProcessedKeys[GLFW_KEY_KP_SUBTRACT] = GL_TRUE;
        if(this->effect > PostProd::NO_POSTPROD) {
            this->effect = static_cast<PostProd::POSTPROD_EFFECT>(this->effect - 1);
            this->UpdatePostProd();
        }
    }
    //DEBUG
    if(this->Keys[GLFW_KEY_G] && !this->ProcessedKeys[GLFW_KEY_G]){
        this->ProcessedKeys[GLFW_KEY_G] = GL_TRUE;
        this->DEBUG = !this->DEBUG;
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
void Game::RenderBuffer() {
    this->State_manager.ActiveFramebuf(ResourceManager::GetFramebuffer("postprod"));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create camera transformation
    glm::mat4 view3D, projection3D;
    view3D = this->Cam.GetViewMatrix();
    projection3D = glm::perspective(glm::radians(this->Cam.Zoom), static_cast<GLfloat>(this->Width)/static_cast<GLfloat>(this->Height), 0.1f, 1750.0f);
    bool underwater = this->Cam.Position.y < 100;

    //should be drawn at the end but other objects have transparency
    if(underwater)
        this->Renderer[SKYBOX]->DrawSprite(this->State_manager, ResourceManager::GetCubemap("skybox"), projection3D, view3D);
    else
        this->Renderer[SKYBOX]->DrawSprite(this->State_manager, ResourceManager::GetCubemap("skybox_outside"), projection3D, view3D);
    for(GameModel &mod : this->models) {
        if(!mod.cullface)
            glDisable(GL_CULL_FACE);
        mod.Draw(this->State_manager, ResourceManager::GetShader("model"), projection3D, view3D);
        if(this->DEBUG)
            mod.Draw(this->State_manager, ResourceManager::GetShader("debug"), projection3D, view3D);
        if(!mod.cullface)
            glEnable(GL_CULL_FACE);
    }
    //water
    if(!underwater)
        glCullFace(GL_FRONT);
    this->planes.front().Draw(this->State_manager, *this->Renderer[WATER], projection3D, view3D);
    if(!underwater)
        glCullFace(GL_BACK);
    for(Particle &p : this->bubbles)
        p.Draw(this->State_manager, *this->Renderer[PARTICLE], projection3D, view3D);
}

void Game::RenderScreen() {
    this->State_manager.DeactiveFramebuf();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);

    //Framebuffer containing whole scene
    this->Renderer[POSTPROD]->DrawSprite(this->State_manager, ResourceManager::GetFramebuffer("postprod"), this->effect);
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
    shaders[TEXT].SetMatrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f), GL_TRUE);
    for (const int i  : {MODEL, WATER}) {
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
    //Post processor
    shaders[POSTPROD_CONVO].SetVector2fArray("kernel", PostProd::kernel, 9, GL_TRUE);
    shaders[POSTPROD_CONVO].SetFloatArray("average_kernel", PostProd::average_kernel, 9);
    shaders[POSTPROD_CONVO].SetFloatArray("sharpen_kernel", PostProd::sharpen_kernel, 9);
    shaders[POSTPROD_CONVO].SetFloatArray("gauss_kernel", PostProd::gauss_kernel, 9);
    shaders[POSTPROD_CONVO].SetFloatArray("sobel_kernel", PostProd::sobel_kernel, 9);
    shaders[POSTPROD_CONVO].SetFloatArray("laplacian_kernel", PostProd::laplacian_kernel, 9);
    for (const int i  : {POSTPROD_MOSAIC, POSTPROD_WARP, POSTPROD_SWIRL, POSTPROD_KALEI, POSTPROD_KALEI2, POSTPROD_PAINT}) {
        shaders[i].SetFloat("width", this->Width, GL_TRUE);
        shaders[i].SetFloat("height", this->Height);
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
    mod.speed = 10;
    mod.deformation_magnitude = 1.0;
    this->models.push_back(mod);
    mod = GameModel("models3D/phenix/Model_C1018410/fenghuang5.obj", "phenix");
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

void Game::UpdatePostProd(){
    Shader s;
    if (this->no_neighbor.find(this->effect) != this->no_neighbor.end()){
        s = ResourceManager::GetShader("postprocessor");
    } else if (this->convolute.find(this->effect) != this->convolute.end()){
        s = ResourceManager::GetShader("postprocessor_convolute");

    } else {
        switch(this->effect){
            case PostProd::MOSAIC:
                s = ResourceManager::GetShader("postprocessor_mosaic");
                break;
            case PostProd::SWIRL:
                s = ResourceManager::GetShader("postprocessor_swirl");
                break;
            case PostProd::TIMEWARP:
                s = ResourceManager::GetShader("postprocessor_warp");
                break;
            case PostProd::FISHEYE:
                s = ResourceManager::GetShader("postprocessor_fisheye");
                break;
            case PostProd::KALEIDOSCOPE:
                s = ResourceManager::GetShader("postprocessor_kalei");
                break;
            case PostProd::KALEIDOSCOPE2:
                s = ResourceManager::GetShader("postprocessor_kalei2");
                break;
            case PostProd::OIL_PAINTING:
                s = ResourceManager::GetShader("postprocessor_oil_painting");
                break;
            default:
                s = ResourceManager::GetShader("postprocessor");
        }
    }
    this->Renderer[POSTPROD]->SwapShader(s);
}