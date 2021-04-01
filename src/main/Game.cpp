#include "Game.h"

Game::Game() {

    window = NULL;
    
    isRunning = true;

    context = 0;

}

int Game::OnExecute() {

    SHMY_LOGD("Initializing\n");
    OnInit();

    RNG::setSeed(5);    

    SHMY_LOGD("Initializing shaders\n");
    initShaders();

    // TEXTURES & MATERIALS
    SHMY_LOGD("Initializing materials\n");
    initMaterials();
    
    skybox.Init();


    // STAGE
    
    SHMY_LOGD("Initializing stage\n");
    initStage();

    // AUDIO

    //Audio::LoadMusic("assets/audio/Warp Tapes 89-93 Part 1.wav");
    //Audio::PlayMusic();
    //Audio::LoadSample("audio/bell.wav");


    // PLAYER

    PlayerArgs playerArgs;
    TransformArgs playerTransform = {
        .position = Complex(0.f, -200.f),
        .velocity = Complex::zero,
    }; 
    player = Player(playerArgs, playerTransform, playerAnimation);

    // LASER, MOVE TO SPAWNER
    /*
    LaserArgs laser1;
    TransformArgs laser1Transform{
        .position = {-300.f,-200.f},
        .velocity = {8.f,0.f},
        .rotation = Complex::FromAngle(90.f),
        .rotationDelta = Complex::FromAngle(-2.f)
    };
    Laser myLaser(laser1, laser1Transform);
    */


    // LUA
    SHMY_LOGD("Initializing lua\n");
    initLua();


    //UI

    SHMY_LOGD("Initializing UI\n");
    gameUI.Init();

    SHMY_LOGD("Initializing ImGui fonts\n");
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* font1 = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 20);
    ImFont* font2 = io.Fonts->AddFontFromFileTTF("assets/fonts/EBGaramond-BoldItalic.ttf", 15);

    // Text to be printed
    std::string text = "High-Score: Graze:Bomb:0123456789,";
    std::string textJap = "最 高得点グレーズボムプレイヤー ";
    //std::string textJap0 = "ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをんゔゕゖ゙゚゛゜ゝゞゟぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをんゔゕゖ゙゚゛゜ゝゞゟ゠ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヴヵヶヷヸヹヺ・ーヽヾヿ｟｠｡｢｣､･ｦｧｨｩｪｫｬｭｮｯｰｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞ、。〃〄々〆〇〈〉《》「」『』【】〒〓〔〕〖〗〘〙〚〛〜〝〞〟〠〡〢〣〤〥〦〧〨〩〪〭〮〯〫〬〰〱〲〳〴〵〶〷〸〹〺〻〼〽〾〿ㇰㇱㇲㇳㇴㇵㇶㇷㇸㇹㇺㇻㇼㇽㇾㇿ";
    //std::string textJap1 = "日一国会人年大十二本中長出三同時政事自行社見月分議後前民生連五発間対上部東者党地合市業内相方四定今回新場金員九入選立開手米力学問高代明実円関決子動京全目表戦経通外最言氏現理調体化田当八六約主題下首意法不来作性的要用制治度務強気小七成期公持野協取都和統以機平総加山思家話世受区領多県続進正安設保改数記院女初北午指権心界支第産結百派点教報済書府活原先共得解名交資予川向際査勝面委告軍文反元重近千考判認画海参売利組知案道信策集在件団別物側任引使求所次水半品昨論計死官増係感特情投示変打男基私各始島直両朝革価式確村提運終挙果西勢減台広容必応演電歳住争談能無再位置企真流格有疑口過局少放税検藤町常校料沢裁状工建語球営空職証土与急止送援供可役構木割聞身費付施切由説転食比難防補車優夫研収断井何南石足違消境神番規術護展態導鮮備宅害配副算視条幹独警宮究育席輸訪楽起万着乗店述残想線率病農州武声質念待試族象銀域助労例衛然早張映限親額監環験追審商葉義伝働形景落欧担好退準賞訴辺造英被株頭技低毎医復仕去姿味負閣韓渡失移差衆個門写評課末守若脳極種美岡影命含福蔵量望松非撃佐核観察整段横融型白深字答夜製票況音申様財港識注呼渉達";

    SHMY_LOGD("Initializing ft-gl texture atlases\n");
    // Texture atlas to store individual glyphs
    ftgl::texture_atlas_t* atlas = texture_atlas_new(1024, 1024, 1);

    // Build a new texture font from its description and size
    ftgl::texture_font_t* font = texture_font_new_from_file(atlas, 36, "assets/fonts/Roboto-Bold.ttf");
    ftgl::texture_font_t* fontOutline = texture_font_new_from_file(atlas, 36, "assets/fonts/Roboto-Bold.ttf");
    ftgl::texture_font_t* fontJap0 = texture_font_new_from_file(atlas, 26, "assets/fonts/NotoSerifJP-Black.otf");
    ftgl::texture_font_t* fontJapOutline0 = texture_font_new_from_file(atlas, 26, "assets/fonts/NotoSerifJP-Black.otf");
    fontOutline->rendermode = RENDER_OUTLINE_EDGE;
    fontOutline->outline_thickness = 2.0;
    fontJapOutline0->rendermode = RENDER_OUTLINE_EDGE;
    fontJapOutline0->outline_thickness = 1.0;

    ftgl::texture_font_load_glyphs(font, text.c_str());
    ftgl::texture_font_load_glyphs(fontOutline, text.c_str());
    ftgl::texture_font_load_glyphs(fontJap0, textJap.c_str());
    ftgl::texture_font_load_glyphs(fontJapOutline0, textJap.c_str());

    SHMY_LOGD("Writing atlas to assets/sprites/ATLAS.png\n");
    stbi_write_png("assets/sprites/ATLAS.png", 1024, 1024, 1, atlas->data, 1024);


    std::string englishText[4] = {"Hi-Score :", "Score :", "Graze :", "Bomb :" };
    std::string japaneseText[4] = {"最高得点", "得点", "グレーズ", "ボム" };
    std::string numbers = "12,345,678";

    int sizeEvery = 0;
    for (int i = 0; i < 4; i++) {
        sizeEvery += japaneseText[i].length()/3 + englishText[i].length();
    }
    sizeEvery += 2 * numbers.length();
    sizeEvery *= 2;

    SHMY_LOGD("Initializing fonts\n");
    myFont.Init(sizeEvery, F_STATIC);
    myFont.LoadTexture(1024, 1024, atlas);

    glm::vec2 pens[4] = { { 230.f, 418.f }, { 230.f, 356.f }, { 230.f, 294.f }, { 230.f, 232.f } };
    glm::vec2 pens0[4] = { pens[0], pens[1], pens[2], pens[3] };
    glm::vec2 japOffset = { 8.f, 18.f };
    glm::vec2 japPens[4] = { pens[0] - japOffset, pens[1] - japOffset, pens[2] - japOffset, pens[3] - japOffset };
    glm::vec2 japPens0[4] = { japPens[0], japPens[1], japPens[2], japPens[3] };

    glm::vec2 scoreOffset = { 190.f, 0.f };
    glm::vec2 scoresPen[2] = { pens[0] + scoreOffset, pens[1] + scoreOffset };
    glm::vec2 scoresPen0[2] = { scoresPen[0], scoresPen[1] };


    glm::vec4 black = { 0.f,   0.f,   0.f,   1.f };
    glm::vec4 white = { 1.f,   1.f,   1.f,   1.f };
    glm::vec4 colors[4] = {
        {1.f,    1.f,    1.f,    1.f},
        {0.8f,   0.8f,   0.8f,   1.f},
        {0.8f,   0.8f,   0.8f,   1.f},
        {0.6f,   0.6f,   0.6f,   1.f}
    };

    SHMY_LOGV("Adding gradients and text to fonts\n");
    myFont.AddTextGradient(fontOutline, englishText[0], colors, &pens0[0]);
    myFont.AddTextGradient(fontOutline, englishText[1], colors, &pens0[1]);
    myFont.AddTextGradient(fontOutline, englishText[2], colors, &pens0[2]);
    myFont.AddTextGradient(fontOutline, englishText[3], colors, &pens0[3]);

    myFont.AddText(font, englishText[0], &black, &pens[0]);
    myFont.AddText(font, englishText[1], &black, &pens[1]);
    myFont.AddText(font, englishText[2], &black, &pens[2]);
    myFont.AddText(font, englishText[3], &black, &pens[3]);

    myFont.AddTextGradient(fontOutline, numbers, colors, &scoresPen[0]);
    myFont.AddTextGradient(fontOutline, numbers, colors, &scoresPen[1]);
    myFont.AddText(font, numbers, &black, &scoresPen0[0]);
    myFont.AddText(font, numbers, &black, &scoresPen0[1]);

    myFont.AddText(fontJapOutline0, japaneseText[0], &black, &japPens[0], 3);
    myFont.AddText(fontJapOutline0, japaneseText[1], &black, &japPens[1], 3);
    myFont.AddText(fontJapOutline0, japaneseText[2], &black, &japPens[2], 3);
    myFont.AddText(fontJapOutline0, japaneseText[3], &black, &japPens[3], 3);

    myFont.AddTextGradient(fontJap0, japaneseText[0], colors, &japPens0[0], 3);
    myFont.AddTextGradient(fontJap0, japaneseText[1], colors, &japPens0[1], 3);
    myFont.AddTextGradient(fontJap0, japaneseText[2], colors, &japPens0[2], 3);
    myFont.AddTextGradient(fontJap0, japaneseText[3], colors, &japPens0[3], 3);

    SHMY_LOGV("Prefilling font buffers\n");
    myFont.FillBuffers();

    SHMY_LOGV("Deleting ft-gl atlases\n");
    ftgl::texture_font_delete(font);
    ftgl::texture_font_delete(fontJap0);



    glDisable(GL_MULTISAMPLE);
    glDisable(GL_SCISSOR_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SHMY_LOGD("Initializing framebuffers\n");
    frameBuffer.Init(DEFAULT_PLAY_W, DEFAULT_PLAY_H, FB_COLOR, 2, false);
    gaussianFramebuffer[0].Init(DEFAULT_PLAY_W / DOWNSCALE_FACTOR, DEFAULT_PLAY_H / DOWNSCALE_FACTOR, FB_COLOR, 1);
    gaussianFramebuffer[1].Init(DEFAULT_PLAY_W / DOWNSCALE_FACTOR, DEFAULT_PLAY_H / DOWNSCALE_FACTOR, FB_COLOR, 1);
    intermediateFramebuffer.Init(DEFAULT_PLAY_W, DEFAULT_PLAY_H);
    
    cubeMap.BindCubeMap(0);

    texture.Bind(0);
    normalTest.Bind(6);
    uiTexture.Bind(7);

    myFont.Bind(1);


    for (unsigned int i = 0; i < stage.textures.size(); i++)
        stage.textures[i].Bind(stage.textures[i].slot);

    gaussianFramebuffer[0].BindTexture(5);
    gaussianFramebuffer[1].BindTexture(5);

    frameBuffer.BindTexture(4, 1);
    frameBuffer.BindTexture(5, 2);

    intermediateFramebuffer.BindTexture(4);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    //GAME LOOP //


    //int lastTime = SDL_GetTicks();
    //int nbFrames = 0;

    while (isRunning) {
        
        /*
        nbFrames++;
        if (SDL_GetTicks() - lastTime >= 1000.0) { // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame, Average: %i\n", 1000.0 / float(nbFrames), nbFrames);
            nbFrames = 0;
            lastTime = SDL_GetTicks();
        } */


        //ON EVENT
        SHMY_LOGV("Handling events\n");
        OnEvent();

        //ON LOOP
        SHMY_LOGV("Handling logic\n");
        OnLoop();

        //ON RENDER
        SHMY_LOGV("Rendering\n");
        OnRender();        
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        
        RenderImGuiDebug();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        
        SDL_GL_SwapWindow(window);

    }

    OnCleanup();

    return 0;

}

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    Game game;


    return game.OnExecute();

}