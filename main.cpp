#include <bits/stdc++.h>
#include <raylib.h>
#include <raymath.h>

using namespace std;

Color green ={173,204,96,255};
Color Darkgreen = {43,51,24,255};

int cellSize=30;
int cellCount=25;

double lastupdate = 0;

bool eventTriggered(double interval){
    double currTime =GetTime();

    if(currTime-lastupdate>interval){
        lastupdate=currTime;
        return true;
    }

    return false;
};

bool ElementinDeque(Vector2 element,deque<Vector2> d){
    for(size_t i=0;i<d.size();i++){
        if(Vector2Equals(d[i],element)){
            return true;
        }
    }

    return false;
}

Vector2 generateRandomCell(){
    float x =GetRandomValue(0,cellCount-1);
    float y =GetRandomValue(0,cellCount-1);

     return    Vector2{x,y};
}

class Snake{
    public:
    deque<Vector2> body ={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
    Vector2 direction ={1,0};

    void Draw(){
        for(size_t i=0;i<body.size();i++){
             int x = body[i].x;
             int y = body[i].y;
             Rectangle segment = Rectangle{x*cellSize,y*cellSize,cellSize,cellSize};
             DrawRectangleRounded(segment,0.8,15,Darkgreen);
        }
    }

    void Update(){
        body.pop_back();
        body.push_front(Vector2Add(body[0],direction));
    };
};

class Food{

    public:
    Vector2 position;
    Texture2D texture;
    

    Food(deque<Vector2> snakeBody){
        Image img =LoadImage("Graphics/food.png");
        texture =LoadTextureFromImage(img);
        UnloadImage(img);
        position=generateRandomPos(snakeBody);
    }

    ~Food(){
        UnloadTexture(texture);
    }

    void Draw(){
        DrawTexture(texture,position.x*cellSize,position.y*cellSize,Darkgreen);
    }

    Vector2 generateRandomPos(deque<Vector2> snakeBody){
        
        Vector2 position = generateRandomCell();
        while(ElementinDeque(position,snakeBody)){
            position=generateRandomCell();
        }

        return position;
    }
};

class Game{
    public:
    Snake snake = Snake();
    Food food =Food(snake.body);
    int Score=0;

    void Draw(){
        snake.Draw();
        food.Draw();
    };

    void Update(){
        snake.Update();
        // cout<<"Updating"<<endl;
        CheckCollisionWithFood();
    };

    void CheckCollisionWithFood() {
        cout << "Snake Head Position: (" << snake.body[0].x << ", " << snake.body[0].y << ")" << endl;
    cout << "Food Position: (" << food.position.x << ", " << food.position.y << ")" << endl;
       if (Vector2Equals(snake.body[0], food.position)) {
        cout << "Eating Food" << endl;
        snake.body.push_back(snake.body.back());  // Add a new segment at the snake's tail
        food.position = food.generateRandomPos(snake.body);
         cout << "New Food Position: (" << food.position.x << ", " << food.position.y << ")" << endl;
         Score++;
    }
}   

};
int main () {

   
    cout << "Hello World" << endl;

    InitWindow(cellSize*cellCount,cellCount*cellSize,"Snake_Game");
    SetTargetFPS(60);

    Game game =Game();

    while(WindowShouldClose()==false){
        BeginDrawing();
        ClearBackground(green);
        game.Draw();
        

        if(eventTriggered(0.3)){
            game.Update();
        }

        if(IsKeyPressed(KEY_UP)&&game.snake.direction.y!=1){
            game.snake.direction={0,-1};
        }

        if(IsKeyPressed(KEY_DOWN)&&game.snake.direction.y!=-1){
            game.snake.direction={0,1};
        }
        if(IsKeyPressed(KEY_LEFT)&&game.snake.direction.x!=1){
            game.snake.direction={-1,0};
        }

        if(IsKeyPressed(KEY_RIGHT)&&game.snake.direction.x!=-1){
            game.snake.direction={1,0};
        }
        EndDrawing();

    }
    CloseWindow();
    return 0;
}