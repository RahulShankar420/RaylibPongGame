
#include <iostream>
#include <raylib.h>
#include <math.h>
#include <colors.h>
#include <Non-Moving.h>
#include <Moving.h>
using namespace std;

int winscore = 10;
int winner = 0;


int main() {
    const int base_width = 1280;
    const int base_height = 720;
    // Initialization
    InitWindow(base_width, base_height, "Pong game functional");
    SetTargetFPS(60);

    InitAudioDevice();

    Sound score = LoadSound("assets/score.wav");
    Sound paddle_hit = LoadSound("assets/paddlehit.wav");
    Sound wall_hit = LoadSound("assets/wallhit.wav");

    bool isGameRunning = true;

    Ball ball(base_width / 2, base_height / 2, 20, 7, 9, score, paddle_hit, wall_hit);
    Paddle player1(10, base_height / 2 - player1.height / 2, 25, 120, 8, 1);
    Paddle player2(base_width - 35, base_height / 2 - player2.height / 2, 25, 120, 8, 2);


    while (isGameRunning && !WindowShouldClose()) {

        // Update game logic
        ball.Update(player1, player2);
        player1.Update();
        player2.Update();

        if (winner != 0 && IsKeyPressed(KEY_R)) {
            player1.score = 0;
            player2.score = 0;
            winner = 0;
            ball.ResetBall();
        }

        if (player1.score >= winscore) {
            winner = 1;
        } else if (player2.score >= winscore) {
            winner = 2;
        }

        
        // Draw the render texture to the actual screen with scaling
        BeginDrawing(); // Clear the screen
        ClearBackground(Dark_Green);

        Draw_background(base_width, base_height); // Draw the background
        ball.Draw();                              // Draw the ball
        player1.Draw();                           // Draw player1 paddle
        player2.Draw();                           // Draw player2 paddle
        Draw_score(base_width, player1.score, player2.score); // Draw scores
        Draw_win_screen(winner);                  // Draw win screen if needed

        EndDrawing();
    }
    CloseWindow();
}
