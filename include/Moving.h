class Paddle {
protected:
    void Limit_Movement() {
        if (y <= 0) {
            y = 0;
        } else if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    int width, height;
    int speed, op;
    int score;

    Paddle(float posx, float posy, int w, int h, int sp, int option) {
        x = posx;
        y = posy;
        width = w;
        height = h;
        speed = sp;
        op = option;
        score = 0;
    }

    Rectangle GetRectangle() const {
        return Rectangle{x, y, (float)width, (float)height};
    }

    int GetScore() {
        return score;
    }

    void Update_player_score() {
        score++;
    }

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, (float)width, (float)height}, 0.5, 0, WHITE);
    }

    void Update() {
        if (op == 1) {
            if (IsKeyDown(KEY_W)) {
                y = y - speed;
            } else if (IsKeyDown(KEY_S)) {
                y = y + speed;
            }
        } else {
            if (IsKeyDown(KEY_UP)) {
                y = y - speed;
            } else if (IsKeyDown(KEY_DOWN)) {
                y = y + speed;
            }
        }
        Limit_Movement();
    }
};


class Ball {
public:
    float x, y;
    int speed_x, speed_y, minspeed, maxspeed;
    int radius;
    Sound paddle_hit, wall_hit, score;
    int bounce;
    bool paused;

    Ball(float posx, float posy, int rad, int minsp, int maxsp, Sound s, Sound p, Sound w) {
        x = posx;
        y = posy;
        radius = rad;
        minspeed = minsp;
        maxspeed = maxsp;
        speed_x = minsp;
        speed_y = minsp;
        score = s;
        paddle_hit = p;
        wall_hit = w;
        bounce = 0;
        paused = true;
        ResetBall();
    }

    void Draw() {
        DrawCircle((int)x, (int)y, (int)radius, YELLOW);
        if (paused) {
            DrawText("Press SPACE to start!", GetScreenWidth() / 2 - 250, GetScreenHeight() / 2 + 100, 40, WHITE);
        }
    }

    void UpdateSpeed() {
        if (bounce == 2) {
            bounce = 0;
            if (speed_x > 0) speed_x++;
            if (speed_y > 0) speed_y++;
            if (speed_x < 0) speed_x--;
            if (speed_y < 0) speed_y--;
            if (speed_x > maxspeed) speed_x = maxspeed;
            if (speed_y > maxspeed) speed_y = maxspeed;
            if (speed_x < -maxspeed) speed_x = -maxspeed;
            if (speed_y < -maxspeed) speed_y = -maxspeed;
        }
    }

    void ReflectBall(Vector2 normal, int paddleSpeed, bool hitPaddle) {
        // Compute dot product
        float dot = speed_x * normal.x + speed_y * normal.y;

        // Apply Reflection Formula: V' = V - 2 * (V · N) * N
        speed_x = speed_x - 2 * dot * normal.x;
        speed_y = speed_y - 2 * dot * normal.y;

        // Apply spin ONLY if hitting a paddle
        if (hitPaddle) {
            speed_y += paddleSpeed / 2;
        }

        // Ensure ball maintains a consistent speed
        float currentSpeed = sqrt(speed_x * speed_x + speed_y * speed_y);
        float desiredSpeed = sqrt(minspeed * minspeed + minspeed * minspeed); // Maintain original speed

        if (currentSpeed != 0) {
            speed_x *= (desiredSpeed / currentSpeed);
            speed_y *= (desiredSpeed / currentSpeed);
        }

        // Prevent near-horizontal motion when hitting paddles
        if (hitPaddle && fabs(speed_y) < 2) {
            speed_y = (speed_y < 0) ? -2 : 2;
        }
    }

    bool RayIntersectsRectangle(Vector2 start, Vector2 end, Rectangle rect) {
        // Cast the movement path as a ray and check if it intersects the paddle
        Vector2 points[4] = {
            {rect.x, rect.y},                         // Top-left
            {rect.x + rect.width, rect.y},            // Top-right
            {rect.x + rect.width, rect.y + rect.height}, // Bottom-right
            {rect.x, rect.y + rect.height}            // Bottom-left
        };

        for (int i = 0; i < 4; i++) {
            Vector2 p1 = points[i];
            Vector2 p2 = points[(i + 1) % 4]; // Next point in the rectangle
            if (CheckLineIntersection(start, end, p1, p2)) {
                return true;
            }
        }
        return false;
    }

    bool CheckLineIntersection(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2) {
        float d = (a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x);
        if (d == 0) return false; // Lines are parallel

        float t = ((a1.x - b1.x) * (b1.y - b2.y) - (a1.y - b1.y) * (b1.x - b2.x)) / d;
        float u = ((a1.x - b1.x) * (a1.y - a2.y) - (a1.y - b1.y) * (a1.x - a2.x)) / d;

        return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
    }


    void Update(Paddle& player1, Paddle& player2) {
        if (paused) {
            if (IsKeyPressed(KEY_SPACE)) {
                paused = false;
            }
            return;
        }

        UpdateSpeed();

        // Compute next frame position
        Vector2 currentPos = { x, y };
        Vector2 nextPos = { x + speed_x, y + speed_y };

        Rectangle rect1 = player1.GetRectangle();
        Rectangle rect2 = player2.GetRectangle();

        // Left Paddle Collision
        if (RayIntersectsRectangle(currentPos, nextPos, rect1)) {
            x = rect1.x + rect1.width + radius; // Move to edge
            ReflectBall(Vector2{1, 0}, player1.speed,true); // Reflect based on left paddle
            bounce++;
            PlaySound(paddle_hit);
        } 
        // Right Paddle Collision
        else if (RayIntersectsRectangle(currentPos, nextPos, rect2)) {
            x = rect2.x - radius; // Move to edge
            ReflectBall(Vector2{-1, 0}, player2.speed,true); // Reflect based on right paddle
            bounce++;
            PlaySound(paddle_hit);
        } 
        // No Paddle Collision → Move Normally
        else {
            x += speed_x;
            y += speed_y;
        }

        // Wall bounce
        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
            PlaySound(wall_hit);
        }

        // Scoring logic
        if (x + radius >= GetScreenWidth()) {
            player1.Update_player_score();
            PlaySound(score);
            ResetBall();
        } 
        else if (x - radius <= 0) {
            player2.Update_player_score();
            PlaySound(score);
            ResetBall();
        }
    }



    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x = minspeed * speed_choices[GetRandomValue(0, 1)];
        speed_y = minspeed * speed_choices[GetRandomValue(0, 1)];

        paused = true;
    }
};
