#include<raylib.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

typedef struct player{
	Rectangle pos;
	float speedy;
	float speedx;
	char name[20];
	float health;
} player;

typedef struct shot {
	Rectangle pos;
	float speedy;
	float speedx;
	int shotTime;
	bool finished;
} shot;

typedef struct enemy {
	Rectangle pos;
	float health;
	float damage;
	bool alive;
	float speedy;
	float speedx;
} enemy;

shot shots[100];
enemy enemys[100];
Texture2D gob1;
Texture2D gob2;
Texture2D gob3;
Texture2D gob4;
Texture2D gobTexture;

int killCount = 1;
unsigned long long int frameCount = 0;

void movePlayer(player *Player, float delta){
	if (IsKeyDown(KEY_W)) Player->speedy -= 1;
	if (IsKeyDown(KEY_S)) Player->speedy += 1;
	if (IsKeyDown(KEY_A)) Player->speedx -= 1;
	if (IsKeyDown(KEY_D)) Player->speedx += 1;

	Player->pos.x += Player->speedx * delta;
	Player->pos.y += Player->speedy * delta;

	if (Player->speedy < 0) Player->speedy += 0.5;
	else if (Player->speedy > 0) Player->speedy -= 0.5;
	if (Player->speedx < 0) Player->speedx += 0.5;
	else if (Player->speedx > 0) Player->speedx -= 0.5;

	if (Player->speedy > 800) Player->speedy = 800;
	else if (Player->speedy < -800) Player->speedy = -800;
	if (Player->speedx > 800) Player->speedx = 800;
	else if (Player->speedx < -800) Player->speedx = -800;

	if (Player->pos.x < 0) Player->pos.x = 0;
	else if (Player->pos.x + Player->pos.width > 1920) Player->pos.x = 1920 - Player->pos.width;
	if (Player->pos.y < 0) Player->pos.y = 0;
	else if (Player->pos.y + Player->pos.height > 1080) Player->pos.y = 1080 - Player->pos.height;
}

void shoot(player *Player){
	if (IsKeyDown(KEY_UP)) {
		shot new = {{Player->pos.x+40, Player->pos.y+50, 10, 10}, Player->speedy - 100, Player->speedx, 0, false};
		for (int i = 0; i < 100; i++)
			if (shots[i].finished == true){
				shots[i] = new;
				break;
			}
	}
	else if (IsKeyDown(KEY_DOWN)) {
		shot new = {{Player->pos.x+40, Player->pos.y+50, 10, 10}, Player->speedy + 100, Player->speedx, 0, false};
		for (int i = 0; i < 100; i++)
			if (shots[i].finished == true){
				shots[i] = new;
				break;
			}
	}
	else if (IsKeyDown(KEY_LEFT)) {
		shot new = {{Player->pos.x+40, Player->pos.y+50, 10, 10}, Player->speedy, Player->speedx - 100, 0, false};
		for (int i = 0; i < 100; i++)
			if (shots[i].finished == true){
				shots[i] = new;
				break;
			}
	}
	else if (IsKeyDown(KEY_RIGHT)) {
		shot new = {{Player->pos.x+40, Player->pos.y+50, 10, 10}, Player->speedy, Player->speedx + 100, 0, false};
		for (int i = 0; i < 100; i++)
			if (shots[i].finished == true){
				shots[i] = new;
				break;
			}
	}
}

void moveShot(shot *Shot, float delta){
	Shot->shotTime++;

	Shot->pos.x += Shot->speedx * delta / 10;
	Shot->pos.y += Shot->speedy * delta / 10;
	
	if (Shot->speedy > 1000) Shot->speedy = 1000;
	else if (Shot->speedy < -1000) Shot->speedy = -1000;
	if (Shot->speedx > 1000) Shot->speedx = 1000;
	else if (Shot->speedx < -1000) Shot->speedx = -1000;
	if (Shot->shotTime > 20000){
		Shot->finished = true;
	}
}

void shotDoDamage(shot *Shot, enemy *Enemy){
	if (CheckCollisionRecs(Shot->pos, Enemy->pos)){
		Enemy->health -= 10;
		Shot->finished = true;
	}
	if (Enemy->health <= 0) {
		Enemy->alive = false;
		killCount++;
	}
}

void newEnemy(){
	for (int i = 0; i < 100; i++){
		srand(time(NULL));
		if (enemys[i].alive == false){
			enemy new;
			new.pos.x = rand() % 1920;
			new.pos.y = rand() % 1080;
			new.pos.width = 80;
			new.pos.height = 80;
			new.health = 100;
			new.damage = 10;
			new.alive = true;
			new.speedx = 0;
			new.speedy = 0;
			enemys[i] = new;
			break;
		}
	}
}

void moveEnemy(enemy *Enemy, player *Player, float delta){
	if (Enemy->pos.x < Player->pos.x) {
		Enemy->speedx += 0.4;
		gobTexture = gob4;
	}
	else if (Enemy->pos.x > Player->pos.x) {
		Enemy->speedx -= 0.4;
		gobTexture = gob3;
	}
	if (Enemy->pos.y < Player->pos.y) {
		Enemy->speedy += 0.4;
		gobTexture = gob1;
	}
	else if (Enemy->pos.y > Player->pos.y) {
		Enemy->speedy -= 0.4;
		gobTexture = gob2;
	}

	Enemy->pos.x += Enemy->speedx * delta;
	Enemy->pos.y += Enemy->speedy * delta;


	if (Enemy->speedy < 0) Enemy->speedy += 0.3;
	else if (Enemy->speedy > 0) Enemy->speedy -= 0.3;
	if (Enemy->speedx < 0) Enemy->speedx += 0.3;
	else if (Enemy->speedx > 0) Enemy->speedx -= 0.3;

	if (Enemy->speedy > 800) Enemy->speedy = 800;
	else if (Enemy->speedy < -800) Enemy->speedy = -800;
	if (Enemy->speedx > 800) Enemy->speedx = 800;
	else if (Enemy->speedx < -800) Enemy->speedx = -800;

	if (Enemy->pos.x < 0) Enemy->pos.x = 0;
	else if (Enemy->pos.x > 1920) Enemy->pos.x = 1920;
	if (Enemy->pos.y < 0) Enemy->pos.y = 0;
	else if (Enemy->pos.y > 1080) Enemy->pos.y = 1080;
}

int main(){
	for (int i = 0; i < 100; i++){
		shots[i].finished = true;
	}
	for (int i = 0; i < 100; i++){
		enemys[i].alive = false;
	}

	InitWindow(1920, 1080, "the game");

	Texture2D casp1 = LoadTexture("caspar.png");
	Texture2D casp2 = LoadTexture("caspar2.png");
	Texture2D casp3 = LoadTexture("caspar3.png");
	Texture2D casp4 = LoadTexture("caspar4.png");

	gob1 = LoadTexture("gob1.png");
	gob2 = LoadTexture("gob2.png");
	gob3 = LoadTexture("gob3.png");
	gob4 = LoadTexture("gob4.png");

	Texture2D texture = casp1;
	gobTexture = gob1;

	Texture2D bg = LoadTexture("bg.png");

	Texture2D furball = LoadTexture("furball.png");

	player Player = {{(float)1920/2, (float)1080/2, 100, 100}, 0.0, 0.0, "caspar", 100};
	SetTargetFPS(60);

	while (!WindowShouldClose()){
		if (IsKeyDown(KEY_UP)) texture = casp2;
		if (IsKeyDown(KEY_DOWN)) texture = casp1;
		if (IsKeyDown(KEY_LEFT)) texture = casp3;
		if (IsKeyDown(KEY_RIGHT)) texture = casp4;

		frameCount++;

		float delta = GetFrameTime() * 100;
		movePlayer(&Player, delta);
		if ((frameCount % 30) == 0){
			shoot(&Player);
		}
		if ((frameCount % 200) == 0){
			newEnemy();
		}

		for (int i = 0; i < 100; i++){
			if (shots[i].finished == false)
				moveShot(&shots[i], delta);

			if (enemys[i].alive == true)
				moveEnemy(&enemys[i], &Player, delta);

			if ((frameCount % 30) == 0){
				if (enemys[i].alive == true){
					if (CheckCollisionRecs(Player.pos, enemys[i].pos)){
						Player.health -= enemys[i].damage;
					}
				}
			}

		}
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < 100; j++){
				if (enemys[j].alive == true)
					shotDoDamage(&shots[i], &enemys[j]);
			}
		}


		if (Player.health <= 0) break;


		BeginDrawing();
		ClearBackground(WHITE);
		DrawTexture(bg, 0, 0, WHITE);

		if (frameCount % 120)
			if ((killCount % 5) == 0){
				for (int i = 0; i < 5; i++){
					for (int j = 0; j < 100; j++){
						DrawRectangle(rand()%1920, rand()%1080, 40, 40, BLUE);
					}
				}		

			}
		
		DrawTexture(texture, Player.pos.x, Player.pos.y, WHITE);
		for (int i = 0; i < 100; i++){
			if (shots[i].finished != true)
				DrawTexture(furball, shots[i].pos.x, shots[i].pos.y, WHITE);
		}
		for (int i = 0; i < 100; i++){
			if (enemys[i].alive == true){
				DrawTexture(gobTexture, enemys[i].pos.x, enemys[i].pos.y, WHITE);
			}
		}
		char *health = malloc(10);
		char *kills = malloc(100);

		snprintf(health, 10, "%f", Player.health);
		snprintf(kills, 100, "%d", killCount);
		DrawText(health, 0, 0, 20, RED);
		DrawText(kills, 0, 30, 20, RED);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
