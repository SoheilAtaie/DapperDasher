#include "raylib.h"
#include "Animations.h"

// window dimensions 
const int windowWidth = { 512 };
const int windowHeight = { 380 };
int windowDimensions[2] = { windowWidth, windowHeight };

int velocity{ 0 };

bool collision{};

// jump velocity pixels/s
int jumpvel = -26;

// the gravity value (pixels/s)/s
const int gravity{1};

bool isOnGraound(AnimData data, int windowHeight)
{
	return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
	// update running time
	data.runningTime += deltaTime;
	if (data.runningTime >= data.runningTime)
	{
		data.runningTime = 0.0;
		//update animation frame
		data.rec.x = data.frame * data.rec.width;
		data.frame++;
		if (data.frame > maxFrame)
		{
			data.frame = 0;
		}
	}
	return data;
}


int main()
{
	// initialize the window 
	InitWindow(windowDimensions[0], windowDimensions[1], "DapperDasher");
	// Nebula variables
	Texture2D Nebula = LoadTexture("textures/12_nebula_spritesheet.png");
	Texture2D background = LoadTexture("textures/far-buildings.png");
	Texture2D midground = LoadTexture("textures/back-buildings.png");
	Texture2D foreground = LoadTexture("textures/foreground.png");
	float bgX{};
	float mgX{};
	float fgX{};

	const int sizeOfNebulae{ 6 };
	AnimData nebulae[sizeOfNebulae]{};

	for (int i = 0; i < sizeOfNebulae; i++)
	{
		nebulae[i].rec.x = 0.0;
		nebulae[i].rec.y = 0.0;
		nebulae[i].rec.width = Nebula.width / 8;
		nebulae[i].rec.height = Nebula.height / 8;
		nebulae[i].pos.y = windowDimensions[1] - Nebula.height / 8;
		nebulae[i].frame = 0;
		nebulae[i].runningTime = 0.0;
		nebulae[i].updateTime = 0.0;
		nebulae[i].pos.x = windowDimensions[0] + i * 500;
	}

	float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };

	Rectangle nebRec{ 0.0, 0.0, Nebula.width / 8, Nebula.height / 8 };
	Vector2 nebPos{ windowWidth, windowHeight - nebRec.height };

	// The second Nebula values
	Rectangle neb2Rec{ 0.0, 0.0, Nebula.width / 8, Nebula.height / 8 };
	Vector2 neb2pos{ windowWidth + 300,  windowHeight - nebRec.height };

	// nebula X velocity (pixels/second)
	int nebVel{ -250 };

    // scarfy variables
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	AnimData scarfyData;
	scarfyData.rec.width = scarfy.width / 6;
	scarfyData.rec.height = scarfy.height;
	scarfyData.rec.x = 0;
	scarfyData.rec.y = 0;
	scarfyData.pos.x = windowWidth / 2 - scarfyData.rec.width / 2;
	scarfyData.pos.y = windowHeight - scarfyData.rec.height;
	scarfyData.frame = 0;
	scarfyData.updateTime = 1.0 / 12.0;
	scarfyData.runningTime = 0.0;

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);

		// create delta time (time since the last frame)
		const float dT{ GetFrameTime() };

		bgX -= 20 * dT;
		if (bgX <= -background.width * 2)
		{
			bgX = 0.0;
		}

		mgX -= 40 * dT;
		if (mgX <= -midground.width * 2)
		{
			mgX = 0.0;
		}

		fgX -= 80 * dT;
		if (fgX <= -foreground.width * 2)
		{
			fgX = 0.0;
		}

		// Draw the background
		Vector2 bg1Pos{ bgX, 0.0 };
		DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
		Vector2 bg2Pos{ bgX + background.width * 2, 0.0 };
		DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

		// Draw the midground
		Vector2 mg1Pos{ mgX, 0.0 };
		DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
		Vector2 mg2Pos{ mgX + midground.width * 2, 0.0 };
		DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

		// Draw the foreground
		Vector2 fg1Pos{ fgX, 0.0 };
		DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
		Vector2 fg2Pos{ fgX + foreground.width * 2, 0.0 };
		DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);


		
		for (AnimData nebula : nebulae)
		{
			float pad{ 50 };
			Rectangle nebRec{
				nebula.pos.x + pad,
				nebula.pos.y + pad,
				nebula.rec.width - 2 * pad,
				nebula.rec.height - 2 * pad,

			};
			Rectangle scarfyRec{
				scarfyData.pos.x,
				scarfyData.pos.y,
				scarfyData.rec.width,
				scarfyData.rec.height,
			};
			if (CheckCollisionRecs(nebRec, scarfyRec))
			{
				collision = true;
			}
		}

		// Update finsih line
		finishLine += nebVel * dT;

		// Draw Scarfy
		if (collision)
		{
			// lose the game
			DrawText("Game Over!", windowDimensions[0]/ 4 , windowDimensions[1] / 2, 50, RED);
		}
		else if (scarfyData.pos.x >= finishLine)
		{
			DrawText("You win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 50, GREEN);
		}
		else
		{
			for (int i = 0; i < sizeOfNebulae; i++)
			{
				DrawTextureRec(Nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
			}
			DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
		}


		// perform ground check
		if (isOnGraound(scarfyData, windowDimensions[1]))
		{
			// the box is on the ground
			velocity = 0;
		}
		else
		{
			// the box is in the air, we can apply teh velocity 
			velocity += (gravity);
		}

		// both checking if the key is pressed and if the character is on the fround 
		// we do not want multiple air jumps 
		if (IsKeyPressed(KEY_SPACE) && (scarfyData.pos.y == windowHeight - scarfyData.rec.height))
		{
			velocity += jumpvel;
		}

		for (int i = 0; i < sizeOfNebulae; i++)
		{
			nebulae[i].pos.x += nebVel * dT;
		}

		

		// Update scarfy vertical position
		scarfyData.pos.y += (velocity);

		// Scarfy animation 
		if (scarfyData.pos.y == windowHeight - scarfyData.rec.height)
		{
			scarfyData.runningTime += dT;
			if (scarfyData.runningTime >= scarfyData.updateTime)
			{
				scarfyData.runningTime = 0.0;

				// now we can update animation frame

				scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;

				scarfyData.frame++;

				if (scarfyData.frame > 5)
				{
					scarfyData.frame = 0;
				}
			}
		}

		for (int i = 0; i < sizeOfNebulae; i++)
		{

			//scarfyData = updateAnimData(scarfyData, dT, 5);
		
			// Nebula animation
			nebulae[i] = updateAnimData(nebulae[i], dT, 7);
		}

		

		EndDrawing();
	}
	UnloadTexture(scarfy);
	UnloadTexture(Nebula);
	UnloadTexture(background);
	UnloadTexture(midground);
	UnloadTexture(foreground);
	CloseWindow();

	return 0;
}
