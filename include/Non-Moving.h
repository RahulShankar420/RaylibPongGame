void Draw_score(int width, int p_score, int c_score) {
    // Draw player and CPU scores
    DrawText(
        TextFormat("%i", c_score),
        3 * width / 4 - 20, // Position for CPU score
        20,
        80, // Font size
        WHITE
    );
    DrawText(
        TextFormat("%i", p_score),
        width / 4 - 20, // Position for player score
        20,
        80, // Font size
        WHITE
    );
}

void Draw_background(int width, int height) {
    // Draw background with no scaling
    DrawRectangle(
        width / 2, // Position
        0,
        width / 2, // Width
        height,    // Height
        Green
    );

    DrawCircle(
        width / 2,     // Center x
        height / 2,    // Center y
        150,           // Radius
        Light_Green
    );

    // Draw separating line
    DrawLine(
        width / 2, 0,
        width / 2,
        height,
        WHITE
    );
}

void Draw_win_screen(int winner) {
    if (winner != 0) {
        // Draw the background rectangle
        DrawRectangleRounded(
            Rectangle{
                (float)(GetScreenWidth() / 2 - GetScreenWidth() / 4),
                (float)(GetScreenHeight() / 2 - GetScreenHeight() / 4),
                (float)(GetScreenWidth() / 2),
                (float)(GetScreenHeight() / 2)
            },
            0.3, 0, GRAY
        );

        // Draw winner text
        DrawText(
            winner == 1 ? "Player 1 Wins!" : "Player 2 Wins!",
            GetScreenWidth() / 2 - 220, // Centered text
            GetScreenHeight() / 2 - 100,
            60, // Font size
            WHITE
        );

        // Draw restart text
        DrawText(
            "Press R to Restart",
            GetScreenWidth() / 2 - 200,
            GetScreenHeight() / 2 + 60,
            40, // Font size
            WHITE
        );
    }
}
