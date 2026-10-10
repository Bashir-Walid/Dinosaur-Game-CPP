#include <iostream>
#include <vector>
#include <raylib.h>
#include <raymath.h>

int main() {

    const int windowWidth = 1080;

    const int windowHeight = 950;

    float currentFrame = 0;

    float frameCounter = 0;

    float frameSpeed = 8;

    float score = 0.0f;

    float vY = 0.0f;

    float gravity = 0.5f;

    bool isJumping = false;

    bool isDead = false;

    bool isGameOver = false;

    int jumpCount = 0; // عداد القفزات للسماح بالقفزة المزدوجة


    InitWindow(windowWidth, windowHeight, "DRAGON GAME");

    SetTargetFPS(60);


    Texture2D map = LoadTexture("map/123.png");

    Texture2D dragonset = LoadTexture("Characters/spr_monster_reg_strip4.png");

    Texture2D dragondeith = LoadTexture("Characters/spr_enemy_reg_hurt_strip4.png");

    Texture2D obstacleTexture = LoadTexture("Rock/SmallRock2.png");


    Music bgMusic = LoadMusicStream("audio/background.mp3");

    Sound jumpSound = LoadSound("audio/jump.mp3");

    Sound deathSound = LoadSound("audio/Death.mp3");


    PlayMusicStream(bgMusic);  


    float grassTopY = windowHeight - 160.0f;


    float bgScrollX = 0.0f;

    float baseBgScrollSpeed = 2.0f;

    float baseObstacleSpeed = 4.0f;

    float bgScrollSpeed = baseBgScrollSpeed;

    float obstacleSpeed = baseObstacleSpeed;


    Vector2 dragonsetpos{

        windowWidth / 2.0f - (float)dragonset.width / 8.0f,

        grassTopY - (float)dragonset.height * 0.75f

    };


    Texture2D currentTexture;


    const int maxObstacles = 3;

    std::vector<Rectangle> obstacles;

    std::vector<int> obstacleTypes;

    float obstacleSize = 48.0f;


    for (int i = 0; i < maxObstacles; i++) {

        float offsetX = GetRandomValue(0, 400) + i * 400;

        obstacles.push_back({

            (float)windowWidth + offsetX,

            grassTopY - obstacleSize + 8.0f, 

            obstacleSize,

            obstacleSize

        });

        obstacleTypes.push_back(0);

    }


    while (!WindowShouldClose()) {

        UpdateMusicStream(bgMusic); 


        BeginDrawing();

        ClearBackground(WHITE);


        if (!isGameOver) {

            if (!isDead) {

                bgScrollSpeed = baseBgScrollSpeed + score * 0.02f;

                if (bgScrollSpeed > 10.0f) bgScrollSpeed = 10.0f;


                obstacleSpeed = baseObstacleSpeed + score * 0.05f;

                if (obstacleSpeed > 15.0f) obstacleSpeed = 15.0f;


                bgScrollX -= bgScrollSpeed;

                score += 0.1f;


                if (bgScrollX <= -map.width)

                    bgScrollX = 0;


                currentTexture = dragonset;


                frameCounter++;

                if (frameCounter >= frameSpeed) {

                    frameCounter = 0;

                    currentFrame++;

                    if (currentFrame > 3) currentFrame = 0;

                }


                if (IsKeyPressed(KEY_SPACE) && jumpCount < 2) {

                    vY = -16.0f;

                    jumpCount++;

                    isJumping = true;

                    PlaySound(jumpSound);

                }


                vY += gravity;

                dragonsetpos.y += vY;


                float groundY = grassTopY - dragonset.height * 0.75f;

                if (dragonsetpos.y >= groundY) {

                    dragonsetpos.y = groundY;

                    vY = 0.0f;

                    isJumping = false;

                    jumpCount = 0;

                }


                for (int i = 0; i < maxObstacles; i++) {

                    obstacles[i].x -= obstacleSpeed;

                    if (obstacles[i].x + obstacles[i].width < 0) {

                        obstacles[i].x = windowWidth + GetRandomValue(200, 600);

                        obstacleTypes[i] = 0;

                    }


                    Rectangle playerHitbox = {

                        dragonsetpos.x + 10,

                        dragonsetpos.y + 10,

                        (float)currentTexture.width / 4.0f - 20,

                        (float)currentTexture.height - 20

                    };


                    if (CheckCollisionRecs(playerHitbox, obstacles[i])) {

                        if (!isDead) PlaySound(deathSound);

                        isDead = true;

                    }

                }


            } else {

                currentTexture = dragondeith;


                frameCounter++;

                if (frameCounter >= frameSpeed) {

                    frameCounter = 0;

                    currentFrame++;

                    if (currentFrame > 3) currentFrame = 3;

                }


                isGameOver = true;

            }

        }


        DrawTexture(map, bgScrollX, 0, WHITE);

        DrawTexture(map, bgScrollX + map.width, 0, WHITE);


        for (int i = 0; i < maxObstacles; i++) {

            DrawTexture(obstacleTexture, obstacles[i].x, obstacles[i].y, WHITE);

        }


        Rectangle source = {

            currentFrame * (currentTexture.width / 4.0f),

            0.0f,

            (float)currentTexture.width / 4.0f,

            (float)currentTexture.height

        };


        Rectangle dest = {

            dragonsetpos.x,

            dragonsetpos.y,

            (float)currentTexture.width / 4.0f,

            (float)currentTexture.height

        };


        DrawTexturePro(currentTexture, source, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);

        DrawText(TextFormat("Score: %.2f", score), 40, 40, 50, RED);


        if (isGameOver) {

            DrawText("GAME OVER!", windowWidth / 2 - 150, windowHeight / 2 - 50, 50, RED);

            DrawText("Press R to Restart", windowWidth / 2 - 180, windowHeight / 2 + 10, 30, DARKGRAY);


            if (IsKeyPressed(KEY_R)) {

                isDead = false;

                isGameOver = false;

                score = 0.0f;

                currentFrame = 0;

                frameCounter = 0;

                bgScrollX = 0.0f;

                dragonsetpos = {

                    windowWidth / 2.0f - (float)dragonset.width / 8.0f,

                    grassTopY - dragonset.height * 0.75f

                };

                vY = 0.0f;

                isJumping = false;

                jumpCount = 0;

                obstacleSpeed = baseObstacleSpeed;

                bgScrollSpeed = baseBgScrollSpeed;


                for (int i = 0; i < maxObstacles; i++) {

                    float offsetX = GetRandomValue(0, 400) + i * 400;

                    obstacles[i] = {

                        (float)windowWidth + offsetX,

                        grassTopY - obstacleSize + 8.0f,

                        obstacleSize,

                        obstacleSize

                    };

                    obstacleTypes[i] = 0;

                }

            }

        }


        EndDrawing();

    } 


    UnloadMusicStream(bgMusic);

    UnloadSound(jumpSound);

    UnloadSound(deathSound);

    UnloadTexture(map);

    UnloadTexture(dragonset);

    UnloadTexture(dragondeith);

    UnloadTexture(obstacleTexture);


    CloseWindow();

    return 0;

}
