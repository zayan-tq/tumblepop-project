#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1136; 
int screen_y = 896;


void display_level(RenderWindow& window, char**lvl, Texture& bgTex,Sprite& bgSprite,Texture& blockTexture,Sprite& blockSprite, const int height, const int width, const int cell_size)
{
	window.draw(bgSprite);

	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{

			if (lvl[i][j] == '#')
			{
				blockSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(blockSprite);
			}
		}
	}

}

//----------------------------------------------------- Menu ----------------------------------------------------------------------

void drawCharacterSelect(RenderWindow& window, int &character_Select, int &currentboxX, int currentboxY) {

	Texture BackgroundTexture;
	BackgroundTexture.loadFromFile("Data/Selectmenu/MenuBg1.png");
	Sprite Background;
	Background.setTexture(BackgroundTexture);
	Background.setScale(0.5,0.7);
	// ---------------- green player -----------------------
	Texture GreenPlayerTexture;
	GreenPlayerTexture.loadFromFile("Data/Selectmenu/GreenPlayer.png");
	Sprite Greenplayer;
	Greenplayer.setTexture(GreenPlayerTexture);

	Greenplayer.setScale(4,4);
	Greenplayer.setPosition(300, 320);

	// ------------------- yellow player ----------------------

	bool right_arrow = false;
	bool left_arrow = false;

	bool currentRightArrow = Keyboard::isKeyPressed(Keyboard::Right);
	bool currentLeftArrow = Keyboard::isKeyPressed(Keyboard::Left);


	Texture YellowPlayerTexture;
	YellowPlayerTexture.loadFromFile("Data/Selectmenu/YellowPlayer.png");
	Sprite YellowPlayer;
	YellowPlayer.setTexture(YellowPlayerTexture);
	YellowPlayer.setScale(4, 4);
	YellowPlayer.setPosition(800, 320);


	// ----------------------- Texts ----------------------------------

	Texture HeadTextTexture;
	Texture GreenInfoTextTexture;
	Texture YellowInfoTextTexture;

	HeadTextTexture.loadFromFile("Data/Selectmenu/SelectionTextHead.png");
	GreenInfoTextTexture.loadFromFile("Data/Selectmenu/GreenPopperInfo.png");
	YellowInfoTextTexture.loadFromFile("Data/Selectmenu/YellowPopperInfo.png");

	Sprite HeadText, GreenInfoText, YellowInfoText;

	HeadText.setTexture(HeadTextTexture); 
	GreenInfoText.setTexture(GreenInfoTextTexture);
	YellowInfoText.setTexture(YellowInfoTextTexture);

	HeadText.setScale(1,1);
	HeadText.setPosition(200, -20);

	GreenInfoText.setScale(0.76,0.75);
	YellowInfoText.setScale(0.75,0.75);
	GreenInfoText.setPosition(50, 560);
	YellowInfoText.setPosition(580, 560);



	// -------------- Box ---------------------------

	Texture BoxTexture;
	BoxTexture.loadFromFile("Data/Selectmenu/Box.png");
	Sprite Box;
	Box.setTexture(BoxTexture);
	float Box_x = 300;
	float Box_y = 400;

	// int currentboxX, currentboxY;
	Box.setOrigin(Box_x / 2.0f, Box_y / 2.0f);

	if(currentRightArrow && !right_arrow){
		currentboxX = 890;
		currentboxY = 430;
		character_Select = 2;
		YellowPlayer.setScale(5,5);
	}
	else if (currentLeftArrow && !left_arrow){
		currentboxX = 400;
		currentboxY = 430;
		character_Select = 1;
		Greenplayer.setScale(5,5);
	}

	Box.setPosition(currentboxX, currentboxY);

	right_arrow = currentRightArrow;
	left_arrow = currentLeftArrow;
	
	
	// Box.setPosition(400, 430);


	window.draw(Background);
	window.draw(HeadText);
	window.draw(GreenInfoText);
	window.draw(YellowInfoText);
	window.draw(Box);
	window.draw(Greenplayer);
	window.draw(YellowPlayer);

}

// -------------------------------------------------- Checking for whether in bound or not ----------------------------------------

bool isInBounds(int gridX, int gridY, int level_width, int level_height) {
    return (gridX >= 0 && gridX < level_width && gridY >= 0 && gridY < level_height);
}


// ------------------------------------------------------ landing check -----------------------------------------------------------------


bool checkLandingCollision(char** lvl,
    float player_x, float player_y,
    int Pwidth, int Pheight, int cell_size,
    float velocityY, bool dropDown)
{
    if (velocityY <= 0 || dropDown) return false;  // jumping up  ignore

    float next_y = player_y + velocityY;
    int gridY = (next_y + Pheight) / cell_size;
    int left  = player_x / cell_size;
    int mid   = (player_x + Pwidth / 2) / cell_size;
    int right = (player_x + Pwidth) / cell_size;

    bool leftHit = false, midHit = false, rightHit = false;

    if (isInBounds(left, gridY, 18, 14)) {
        leftHit = (lvl[gridY][left] == '#');
    }
    
    if (isInBounds(mid, gridY, 18, 14)) {
        midHit = (lvl[gridY][mid] == '#');
    }
    
    if (isInBounds(right, gridY, 18, 14)) {
        rightHit = (lvl[gridY][right] == '#');
    }
	
	return (leftHit || midHit || rightHit);
}

// ------------------------------- Head Collision Checking ----------------------------------------------------------------------------

bool checkHeadCollision(char** lvl,float player_x, float player_y, int Pwidth, int Pheight, int cell_size, float velocityY)
{
    if (velocityY >= 0) return false; // 

    int gridY = (player_y) / cell_size;
    int left  = (player_x) / cell_size;
    int mid   = (player_x + Pwidth / 2) / cell_size;
    int right = (player_x + Pwidth) / cell_size;

	bool lefthit = false, righthit = false, midhit = false;

	if(isInBounds(left, gridY, 18, 14)){
		lefthit = (lvl[gridY][left] == '#');
	}
	
	if(isInBounds(right, gridY, 18, 24)){
		righthit = (lvl[gridY][right] == '#');
	}
	if(isInBounds(mid, gridY, 18, 14)){
		midhit = (lvl[gridY][mid]);
	}


    return (lefthit || midhit || righthit);
}

// ---------------------------------------------- Checks for getting the tile --------------------------------------------------------

char getTileChar(char** lvl, float worldX, float worldY, int cell_size)
{
    int gx = int(worldX / cell_size);
    int gy = int(worldY / cell_size);

	if(!isInBounds(gx, gy, 18,14)){
		return '.';
	}

    return lvl[gy][gx];
}

// --------------------------------------------- Left Collide ------------------------------------------------------------------------------

bool leftCollide(char** lvl,float player_x, float player_y, int PHeight, int Pwidth, int cell_size, int speed, bool isJumping, float velocityY){

	if(isJumping) return false;	

		float next_x = player_x - (speed * 1.5);
		if(next_x < 0) 
        	return true;
			
		char center = getTileChar(lvl, next_x , player_y + PHeight / 2.0, cell_size);
	
		return (center == '#');
		
}

// ---------------------------------------------- Right Collide ----------------------------------------------------------------------------------

bool rightCollide(char** lvl,float player_x, float player_y, int PHeight, int Pwidth, int cell_size, int speed, bool isJumping, float velocityY){

			
	if(isJumping) return false;

		float next_x = player_x + (speed * 1.5);
		
		if(next_x > screen_x) return true;
		
		// char right_top = getTileChar(lvl, next_x, player_y, cell_size);
		char center = getTileChar(lvl, (next_x + Pwidth), player_y + PHeight / 2.0, cell_size);
		// char right_bottom = getTileChar(lvl, next_x, player_y + PHeight - 1, cell_size);
		
		return ( center == '#' );
		
}

// ---------------------------------------------------Is there a platform below the tile -------------------------------------------------------

bool isPlatformBelow( char **lvl, float player_x, float player_y, int Pwidth, int Pheight, int cell_size, int level_height){


	int currentgridy = (player_y + Pheight) / cell_size; 
	int gridY_below = currentgridy + 1;

	if (gridY_below >= level_height || gridY_below < 0){
		return false;
	}

	

	int left = player_x / cell_size;
	int mid = (player_x + Pwidth / 2) / cell_size;
	int right = (player_x + Pwidth) / cell_size;

	    bool leftHit = false, midHit = false, rightHit = false;

    if (isInBounds(left, gridY_below, 18, 14)) {
        leftHit = (lvl[gridY_below][left] == '#');
    }
    
    if (isInBounds(mid, gridY_below, 18, 14)) {
        midHit = (lvl[gridY_below][mid] == '#');
    }
    
    if (isInBounds(right, gridY_below, 18, 14)) {
        rightHit = (lvl[gridY_below][right] == '#');
    }
	

	return (leftHit || midHit || rightHit);
}

// ----------------------------------------- Is there a platform above --------------------------------------------------------

bool isPlatformAbove(char **lvl, float player_x, float player_y, int Pwidth, int Pheight, int cell_size, int level_height){


	int current_grid = player_y / cell_size;
	int gridY_above = current_grid - 1;

	if (gridY_above < 0 ){
		return false;
	}

	int left = player_x / cell_size;
	int mid = (player_x + Pwidth / 2.0) / cell_size;
	int right = (player_x + Pwidth) / cell_size;

	bool leftHit = false, rightHit = false, midHit = false;

	if (isInBounds(left, gridY_above, 18, 14)) {
        leftHit = (lvl[gridY_above][left] == '#');
    }
    
    if (isInBounds(mid, gridY_above, 18, 14)) {
        midHit = (lvl[gridY_above][mid] == '#');
    }
    
    if (isInBounds(right, gridY_above, 18, 14)) {
        rightHit = (lvl[gridY_above][right] == '#');
    }


	return (leftHit || midHit  || rightHit );
}

// ---------------------------------------------------- Player falling off ledge -----------------------------------------------------------

void playerFalling(bool &wasOnGround, bool &canMoveinAir, bool &isJumping, bool &onGround, int airborneFrames, bool &isFalling){
    

    if (!onGround && !isJumping && airborneFrames >= 2) {
        canMoveinAir = false;
		isFalling = true;
    }
    
    
    if (onGround || isJumping) {
        canMoveinAir = true;
    }
    
    wasOnGround = onGround;
}

// ------------------------------------------------------------- Player Gravity ---------------------------------------------------------------------------------------

void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth, bool &isJumping, bool &dropDown, float &speed)
{

	float previous_bottom = player_y + Pheight;

	offset_y = player_y;

	offset_y += velocityY;

	float next_bottom = offset_y + Pheight;

	bool landing = checkLandingCollision(lvl, player_x, player_y, Pwidth, Pheight, cell_size, velocityY, dropDown);
	bool headHit = checkHeadCollision(lvl, player_x, player_y, Pwidth, Pheight, cell_size, velocityY);

	char bottom_left_down = getTileChar(lvl, player_x, offset_y, cell_size);
	char bottom_right_down =  getTileChar(lvl, player_x + Pwidth/2.0, offset_y, cell_size);
	char bottom_mid_down = getTileChar(lvl, player_x + Pwidth, offset_y + Pheight, cell_size);

	char top_left_up = getTileChar(lvl, player_x, offset_y, cell_size);
	char top_mid_up = getTileChar(lvl, player_x + Pwidth/2.0, offset_y, cell_size);
	char top_right_up = getTileChar(lvl, player_x + Pwidth, offset_y , cell_size);

	char right_mid = getTileChar(lvl, player_x + Pwidth, (offset_y + Pheight) / 2.0, cell_size);


	
	// if (bottom_left_down == '#' || bottom_mid_down == '#' || bottom_right_down == '#')
	// {
	// 	int blockY = ((int)((player_y + Pheight) / cell_size)) * cell_size;
    // 	player_y = blockY - Pheight;
    // 	velocityY = 0;

	// 	onGround = true;
	// 	isJumping = false;
		
	// }
	// else
	// {
	// 	player_y = offset_y;
	// 	onGround = false;
		
	// }

if( velocityY < 0){
	player_y = offset_y;

}

else{
	
	if (landing && !dropDown)
	{
		// snap to block top
		int gridY = (offset_y + Pheight ) / cell_size;
		if(gridY >= 14) gridY = 13;
		float blockTop = (gridY * cell_size) ;

		if (previous_bottom <= blockTop  && next_bottom > blockTop ) {

			if(!dropDown){

				player_y = blockTop - Pheight;
				velocityY = 0;
				onGround = true;
				isJumping = false;
			}

			else{
				player_y = offset_y;
				onGround = false;

			}
		}
		else{
			
			player_y = offset_y;
			onGround = false;
		}
	
	}
	else
	{
		player_y = offset_y;
		onGround = false;
	}

}


	
	if (!onGround)
	{
		
		velocityY += gravity;
		if (velocityY >= terminal_Velocity) velocityY = terminal_Velocity;
	}

	else
	{
		velocityY = 0;
	}
}

// --------------------------------------------------------------- Player Jumping -----------------------------------------------------------------------

void player_jump(float& velocityY, bool& onGround, const float jumpStrength, bool& isJumping)
{
	if (onGround && !isJumping)
	{
		velocityY = jumpStrength;
		onGround = false;
		isJumping = true;
	}
}

// -------------------------------------------------------------- Player moving Right ---------------------------------------------------------------------

void playerMove_right(float &speed,float &player_x, bool &facing_right){

	facing_right = true;

	player_x += speed ;
}

// --------------------------------------------------------------Player Moving left -----------------------------------------------------------------------

void playerMove_left(float &speed,float &player_x){


	player_x -= speed ;
	

}

// ----------------------------------------- Vaccum Fire logic ----------------------------------------------

void vaccumPull(bool &vaccum, int &vaccumFrameCount, int &vaccumFrameDelay, int &vaccumPhase, float &offset_vaccum_x, int &vaccumCurrentFrame, Sprite &RainbowSprite, IntRect vaccum_anim[]){
					vaccumFrameCount++;

				if (vaccumFrameCount >= vaccumFrameDelay){
					vaccumFrameCount = 0;
					
					if(vaccumCurrentFrame == 3){
						vaccumPhase = 1;
						
						offset_vaccum_x = true;
					}
					else if (vaccumCurrentFrame == 4){
						vaccumPhase = 2;
					}
					else if (vaccumCurrentFrame == 5){
						vaccumPhase = 3;
					}
					
					else if (vaccumCurrentFrame == 6){

						vaccumPhase = 4;
					}

					vaccumCurrentFrame++;
					
					if(vaccumCurrentFrame > 6){
					
						vaccumCurrentFrame = 4;
						
					}
					RainbowSprite.setTextureRect(vaccum_anim[vaccumCurrentFrame]);

				}
				vaccum = true;
}

void rainbowShootHook(Sprite &RainbowSprite, float pipeX, float pipeY, float pipeRotation, bool facing_right, int vaccumPhase, float &rainbowHitboxX, float &rainbowHitboxY, float &rainbowHitboxWidth, float &rainbowHitboxHeight, bool crouch){




    float phaseOffset = 0.0f;
    if (vaccumPhase == 1) {
		phaseOffset = 20.0f; 
	}
    else if (vaccumPhase == 2) {
		phaseOffset = 40.0f;
	}
    else if (vaccumPhase == 3) {
		phaseOffset = 60.0f;
	}
    
    float baseDistance = 50.0f +phaseOffset;
    float rainbowX = pipeX;
    float rainbowY = pipeY;

// for facing right => right side, facing left = > left side 

	if (pipeRotation == 0) { 
        if (facing_right) {
            rainbowX = pipeX + baseDistance + 13.0f; 
			rainbowY = pipeY - 50.0f;

			if(crouch){
				rainbowY = rainbowY + 10.9f;			
			
			}

        	} 
		
		else {
            rainbowX = pipeX - baseDistance - 10.f; 
			rainbowY = pipeY - 50.0f;
        }
    }
    else if (pipeRotation == 180) { 
        if (facing_right) {
            rainbowX = pipeX - baseDistance - 13.0f;
			rainbowY = pipeY + 50.0f; 

			
			if (crouch){
				rainbowY = rainbowY + 10.0f;
			}
	

        } else {
            rainbowX = pipeX + baseDistance + 10.0f;
			rainbowY = pipeY + 50.0f; 
        }
    }

	// this is for facing right upward
    else if (pipeRotation == -90) { 

		if(facing_right){
			rainbowX = pipeX - 52.0f;
			rainbowY = pipeY - baseDistance - 15.0f;
			
		}
		else{
			rainbowX = pipeX - 55.0f; 
			rainbowY = pipeY + baseDistance + 15.0f; 

		}
    }

	// for facing left upwards and right downward
    else if (pipeRotation == 90) { 
		if(facing_right){
			rainbowX = pipeX + 50.0f;
			rainbowY = pipeY + baseDistance + 13.0f;
		}
		else{
			rainbowX = pipeX + 52.0f; 
			rainbowY = pipeY - baseDistance - 15.0f; 
		}

	
		
	
    }
    
    RainbowSprite.setPosition(rainbowX, rainbowY);
    RainbowSprite.setRotation(pipeRotation);
    
    if (facing_right) {
        RainbowSprite.setScale(-3, 3); 
    } else {
        RainbowSprite.setScale(3, 3); 
    }

	float baseWidth = 34.0f * 3.0f;  
	float baseHeight = 33.0f * 3.0f; 
	
	// For horizontal directions (0 or 180 degrees)
	if (pipeRotation == 0 || pipeRotation == 180) {
		rainbowHitboxWidth = baseWidth;
		rainbowHitboxHeight = baseHeight;
		rainbowHitboxX = rainbowX;
		rainbowHitboxY = rainbowY;
	}
	// For vertical directions (-90 or 90 degrees)
	else {
		rainbowHitboxWidth = baseHeight;  // Swap width and height for vertical
		rainbowHitboxHeight = baseWidth;
		rainbowHitboxX = rainbowX - (baseHeight - baseWidth) / 2.0f;
		rainbowHitboxY = rainbowY;
	}


}

void vacuumRangeCollision(float vacuumHeight, float vacuumWidth, float vacuumX, float vacuumY, int cell_size){

	int top = (vacuumX / cell_size);
	int mid = ((vacuumY + vacuumHeight / 2.0) / cell_size );
	int bottom = vacuumX + vacuumHeight;


	

}


int main()
{
// ----------------------------------------- Game State ------------------------------------------------

	const int MENU_STATE = 0;
	const int CHARACTER_STATE = 1;
	const int PLAY_STATE = 2;
	const int PAUSE_STATE = 3;
	const int GAME_STATE = 4;
	const int OVER_STATE = 5;

	int currentState = MENU_STATE;

// ---------------------------------------- Level Number ----------------------------------------------

	int level = 1; // default level is one

// --------------------------------------- Rendering --------------------------------------------------

	RenderWindow window(VideoMode(screen_x, screen_y), "Tumble-POP", Style::Resize);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	//level specifics
	const int cell_size = 64; // screen divided into 64 x 64 cell blocks 
	const int height = 14;
	const int width = 18;
	char** lvl;

	const int FallW = 30;
	const int FallH = 42;

	//level and background textures and sprites
	Texture bgTex;
	Sprite bgSprite;
	Texture blockTexture;
	Sprite blockSprite;

	bgTex.loadFromFile("Data/lvl1Bg.png");
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0,0);

	blockTexture.loadFromFile("Data/block1.png");
	blockSprite.setTexture(blockTexture);
	blockSprite.setTexture(blockTexture);
	blockSprite.setTexture(blockTexture);
	blockSprite.setPosition(80,90);
	
	//Music initialisation
	Music lvlMusic;
	bool music = false;


	//player data
	float player_x = 200;
	float player_y = 700;

	float speed = 5;

	const float jumpStrength = -20; // Initial jump velocity
	const float gravity = 1;  // Gravity acceleration

	bool isJumping = false;  // Track if jumping

	bool up_collide = false;
	bool left_collide = false;
	bool right_collide = false;
	bool dropDown = false;

	bool crouch = false;
// --------------------- Player Selection  --------------------------------------

	int Character_Select = 1;


// -------------------- Green Tumblepopper ----------------------------------

	Texture PlayerTexture;
	Texture PlayerJumpTexture;
	Texture PlayerCrouchTexture;
	Texture PlayerFallingTexture;
	Texture BackPackTexture;
	Texture PlayerWalk;


	Sprite PlayerJump;
	Sprite PlayerSprite;
	Sprite backpack;

// -------------------- Rainbow Hitbox ---------------------------------------



// -------------------- Yellow Tumblepopper -----------------------------------

	Texture YellowPlayerTexture;
	YellowPlayerTexture.loadFromFile("Data/Yellow-tumblepop/YellowStanding.png");
	
	Texture YellowPlayerJumpTexture;
	YellowPlayerJumpTexture.loadFromFile("Data/Yellow-tumblepop/YellowJumpTexture.png");

	Texture YellowCrouchTexture;
	YellowCrouchTexture.loadFromFile("Data/Yellow-tumblepop/YellowCrouchTexture.png");

	Texture YellowPlayerFallingTexture;
	YellowPlayerFallingTexture.loadFromFile("Data/Yellow-tumblepop/YellowFallingTexture.png");

	Sprite YellowSprite;

	Sprite YellowRun;

	YellowSprite.setTexture(YellowPlayerTexture);
	YellowSprite.setPosition(player_x, player_y);
	YellowSprite.setScale(3, 3);
	
	Texture YellowRunning;
	YellowRunning.loadFromFile("Data/Yellow-tumblepop/Yellow_RunAnimation.png");
	YellowRun.setTexture(YellowRunning);
	YellowRun.setPosition(player_x, player_y);
	YellowRun.setScale(3, 3);

// --------------------Vaccum PNG ----------------------------------

	float vacuumRainbowHeight = 25.0f;
	float vacuumRainbowWidth = 32.0f;

	

// ------------------- Vacuum PNG End ---------------------------
	bool onGround = false;

	float offset_x = 0;
	float offset_y = 0;
	float velocityY = 0;

	float terminal_Velocity = 20;

	int PlayerHeight = 102;
	int PlayerWidth = 96;

	char top_left = '\0';
	char top_right = '\0';
	char top_mid = '\0';

	char left_mid = '\0';
	char right_mid = '\0';

	char bottom_left = '\0';
	char bottom_right = '\0';
	char bottom_mid = '\0';

	char bottom_left_down = '\0';
	char bottom_right_down = '\0';
	char bottom_mid_down = '\0';

	char top_right_up = '\0';
	char top_mid_up = '\0';
	char top_left_up = '\0';

	bool facing_right = false;

	bool wasOnGround = false;      // Track previous frame's ground state    
	bool canMoveInAir = true;
	bool isFalling = false;

	bool vaccum = false;

	int airborneframe = 0;

// -------------------------------- Select menu ---------------------------------------------

	int currentboxX = 400;
	int currentboxY = 430;


// ------------------------------- hitbox---------------------------------------------

	// Rainbow hitbox variables
	float rainbowHitboxX = 0;
	float rainbowHitboxY = 0;
	float rainbowHitboxWidth = 0;
	float rainbowHitboxHeight = 0;



//--------------------------------------------------- Textures and sprites ---------------------------------------------------------------------

	PlayerTexture.loadFromFile("Data/green_player.png");
	BackPackTexture.loadFromFile("Data/tank_01.png");

	backpack.setTexture(BackPackTexture);

	int backpackX_offset = 10;
	int backpackY_offset = 8;
	

	PlayerSprite.setTexture(PlayerTexture);
	PlayerSprite.setScale(3,3);
	PlayerSprite.setPosition(player_x, player_y);

	PlayerJumpTexture.loadFromFile("Data/jumping_01.png");
	PlayerCrouchTexture.loadFromFile("Data/crouch_01.png");
	PlayerFallingTexture.loadFromFile("Data/falling_01.png");
	PlayerWalk.loadFromFile("Data/running_spritesheet2.png");

	IntRect walk[4] = {
		
		IntRect(0,7, 32, 40),
		IntRect(45, 7, 32, 40),
		IntRect(95, 7, 32, 40),
		IntRect(141, 7, 32, 40)
	};

// ------------------------- Vaccum -------------------------------------------------

	IntRect vaccum_anim[7] = {
		IntRect{166, 0, 24, 33},
		IntRect(133, 0, 22, 33),
		IntRect(104, 0, 21, 33),
		IntRect(87, 0, 17, 33),
		IntRect(70, 0, 17, 33),
		IntRect(40, 0, 29, 33),
		IntRect(0, 0, 34, 33)
	};
	
	Texture Rainbow;
	Rainbow.loadFromFile("Data/Rainbow_Sprite_Animatino.png");
	Sprite RainbowSprite;
	RainbowSprite.setTexture(Rainbow);
	// RainbowSprite.setOrigin(RainbowSprite.getLocalBounds().width / 2.0f, RainbowSprite.getLocalBounds().height / 2.0f);	
// -----------------------------------Player Running --------------------------------------------------


	Sprite PlayerRun;
	PlayerRun.setTexture(PlayerWalk);
	PlayerRun.setPosition(player_x, player_y);
	PlayerRun.setScale(3, 3);

// ----------------------------------- Vaccum sprite stuff ---------------------------------------
Texture Vacuumpipe;
Vacuumpipe.loadFromFile("Data/vacuum.png");

Sprite VacuumSprite;
VacuumSprite.setTexture(Vacuumpipe);
VacuumSprite.setOrigin(19 / 2.0f, 13/ 2.0f);

	int vacuum_state = 0;
	float vacuum_pipe_x = 0.0f;
	float vacuum_pipe_y = 0.0f;
	float pipe_rotation = 0.0f;

// --------------- Animation logic ---------------------------

	int currentFrame = 0;
	int framecount = 0;
	int frame_delay = 4;
	int PLAYER_STATE = 0;



	int vaccumCurrentFrame = 0;
	int vaccumFrameCount = 0;
	int vaccumFrameDelay = 4;
	
	int vaccumPhase = 0;
	float Vaccumoffset_x = 50.f;
	bool offset_vaccum_x = false;

	
	int currentPlayerHeight = PlayerHeight;
	int currentPlayerWidth = PlayerWidth;
	// rectangle box for debugging

	RectangleShape playerHitbox(Vector2f(PlayerWidth, PlayerHeight));
	playerHitbox.setFillColor(Color::Transparent); // make it invisible
	playerHitbox.setOutlineColor(Color::Red);      // outline color
	playerHitbox.setOutlineThickness(2);  

	playerHitbox.setPosition(player_x, player_y);


	// Rainbow hitbox rectangle for visualization
	RectangleShape rainbowHitboxRect;
	rainbowHitboxRect.setFillColor(Color::Transparent);
	rainbowHitboxRect.setOutlineColor(Color::Cyan);
	rainbowHitboxRect.setOutlineThickness(2);

	

	bool isGoingUp   = (velocityY < 0);
	bool isGoingDown = (velocityY > 0);

	Font myfont;
	myfont.loadFromFile("Data/InriaSans-Regular.ttf");
	


	//creating level array
	lvl = new char* [height];
	for (int i = 0; i < height; i += 1)
	{
		lvl[i] = new char[width];
	}



ifstream file("level1.txt");

for (int i = 0; i < height; i++) {
    string line;
    getline(file, line);
    for (int j = 0; j < width; j++) {
        lvl[i][j] = line[j];
    }
}
file.close();

bool enterKeyPressed = false;
bool PKeyPressed = false;
bool MKeyPressed = false;


	Event ev;
	//main loop
	while (window.isOpen())
	{
		if (isFalling) {
  		  	currentPlayerHeight = 120;  
    		currentPlayerWidth = 90;   
		} 
		else if (crouch) {
    		currentPlayerHeight = 70;  
    		currentPlayerWidth = 96;
		}
		else {
   		 	currentPlayerHeight = 102; 
    		currentPlayerWidth = 96;
		}

// Update hitbox
		playerHitbox.setSize(Vector2f(currentPlayerWidth, currentPlayerHeight));
		playerHitbox.setPosition(player_x, player_y);
		
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed) 
			{
				window.close();
			}

			

		}

	
		bool currentEnterKey = Keyboard::isKeyPressed(Keyboard::Enter);
		bool currentPKey = Keyboard::isKeyPressed(Keyboard::P);
		bool currentMKey = Keyboard::isKeyPressed(Keyboard::M);


		//presing escape to close
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		


	if (currentState == MENU_STATE){
    	if (currentEnterKey && !enterKeyPressed) {
        	currentState = CHARACTER_STATE;
    	}


	}

	else if (currentState == CHARACTER_STATE){

		drawCharacterSelect(window, Character_Select, currentboxX, currentboxY);
		if (currentEnterKey && !enterKeyPressed) {
        	currentState = PLAY_STATE;
    	}

	}
		
	else if (currentState == PAUSE_STATE){
		if (currentPKey && !PKeyPressed) {
        	currentState = PLAY_STATE;
    	}
    	else if (currentMKey && !MKeyPressed) {
        	currentState = MENU_STATE;
    	}

	}
	
// ------------------------------------- playing state ------------------------------------------------------

	else if (currentState == PLAY_STATE){

		if(currentPKey && !PKeyPressed){
			currentState = PAUSE_STATE;
		}

		cout << "Current State: ";

if (!music){
	
	if (level == 1){
	
		lvlMusic.openFromFile("Data/lvl1music.ogg");
		lvlMusic.setVolume(10);
		lvlMusic.play();
		lvlMusic.setLoop(true);
		music = true;
	}
}


	// Handling speed logic:

		if (Character_Select == 1){
			speed = 7.5;
		}
		else{
			speed = 5;
		}
	// --------------------- Making sure player doesn't jump out of bounds ----------------------------------
		
		bool left_bound = (player_x < 70);
		bool right_bound = (player_x > 984);
		
		cout << player_x << endl;
		
	// ---------------------------------- Keys and Their handling -------------------------------------
	
		PLAYER_STATE = 1; // Default Idle State
		
		window.clear();
	
		if(Keyboard::isKeyPressed(Keyboard::Up)){
	
			
			if(!isPlatformAbove(lvl, player_x, player_y, PlayerWidth, PlayerHeight, cell_size, height)){
	
				player_jump(velocityY, onGround, jumpStrength, isJumping);
			}
	
			
					
		}
	
		if(Keyboard::isKeyPressed(Keyboard::Right)){
			
			PlayerRun.setTextureRect(walk[currentFrame]);
	
			bool right_check = rightCollide(lvl, player_x, player_y, PlayerHeight, PlayerWidth, cell_size, speed, isJumping, velocityY);
			
			// check to prevent going off screen on the right side
	
			int screen_gridx = int(((player_x + PlayerWidth) / cell_size) - 1);
			bool  screen_left = (screen_gridx >= width);
			
			if((!crouch || isJumping) && !right_check && canMoveInAir && !right_bound){
				PLAYER_STATE = 2;
				framecount++;
				
				if (framecount >= frame_delay){
	
					framecount = 0;
					currentFrame++;
					if(currentFrame >= 4){
						currentFrame = 0;
					}
				}

			
				if(Character_Select == 1){
					
					PlayerRun.setTextureRect(walk[currentFrame]);
					playerMove_right(speed, player_x,facing_right);
				
				}
				else if (Character_Select == 2){

					YellowRun.setTextureRect(walk[currentFrame]);
					playerMove_right(speed, player_x, facing_right);
				}
				
			}
			else{
				PLAYER_STATE = 1;
			}
	
			if(!facing_right){
				vacuum_state = 0;
			}
			
			facing_right = true;
		}
		
		if(Keyboard::isKeyPressed(Keyboard::Left)){
			
			
			bool left_check = leftCollide(lvl, player_x, player_y, PlayerHeight, PlayerWidth, cell_size, speed, isJumping, velocityY);
	
			if((!crouch || isJumping) && !left_check && canMoveInAir && !left_bound){
	
				PLAYER_STATE = 2;
				framecount++;
	
				if (framecount >= frame_delay){
	
					framecount = 0;
					currentFrame++;
					if(currentFrame >= 4){
						currentFrame = 0;
					}
				}

				if (Character_Select == 1){
					PlayerRun.setTextureRect(walk[currentFrame]);
				}
				else if (Character_Select == 2){
					YellowRun.setTextureRect(walk[currentFrame]);
				}
					playerMove_left(speed, player_x);
	
			}
	
			if(facing_right){
				vacuum_state = 0;
			}
	
			facing_right = false;			
		}
	
	// --------------------------------------------- Vaccum Key ---------------------------------
	
		if(Keyboard::isKeyPressed(Keyboard::Space)){
				
			if((!isJumping || !isFalling)){
				
				vaccumPull(vaccum, vaccumFrameCount, vaccumFrameDelay, vaccumPhase, Vaccumoffset_x, vaccumCurrentFrame, RainbowSprite, vaccum_anim);
			}
	
		}
		else{
			vaccum = false;
			vaccumCurrentFrame = 0;
			vaccumPhase = 0;
			vaccumFrameDelay = 4;
			RainbowSprite.setTextureRect(IntRect(0, 0, 0, 0));
		}
		
	
	
		if(PLAYER_STATE == 1){
			currentFrame = 0;
			framecount = 0;
		}
	
	
		if (Keyboard::isKeyPressed(Keyboard::C)){
	
	
			int playerGrid_y = (player_y + PlayerHeight) / cell_size;
	
			if(playerGrid_y < height - 1)
	
				if(!(isPlatformBelow(lvl, player_x,player_y,PlayerWidth,PlayerHeight, cell_size, height))){
				dropDown = true;
				}
				else{
					dropDown = false;
				}
			else{
				dropDown = false;
			}
			
		}
	
		else {
			
			dropDown = false;
		}
	
		if (Keyboard::isKeyPressed(Keyboard::Down)){
			crouch = true;
		}
		else{
			crouch = false;
		}
	
	// --------------------------------------------------- Vacuum rotate logic ------------------------------------------
	
		if (Keyboard::isKeyPressed(Keyboard::W)){
			vacuum_state = 1;
		}
	
		if (Keyboard::isKeyPressed(Keyboard::A)){
			vacuum_state = 2;
		}
	
		if (Keyboard::isKeyPressed(Keyboard::S)){
			vacuum_state = 3;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::D)){
			vacuum_state = 4;
		}
		
	
	// --------------------------------------------------- Gravity Logic ---------------------------------------------------------
		
		display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, height, width, cell_size);
		player_gravity(lvl,offset_y,velocityY,onGround,gravity,terminal_Velocity, player_x, player_y, cell_size, PlayerHeight, PlayerWidth, isJumping, dropDown, speed);
		
		if (!onGround && !isJumping) {
			airborneframe++;
		} else {
			airborneframe = 0;
			isFalling = false;
		}
		
		playerFalling(wasOnGround, canMoveInAir, isJumping, onGround, airborneframe, isFalling);
	
	
	// ---------------------------------- Drawing and Sprite Selection Logic --------------------------------
	
	
		Sprite* spriteToRender = nullptr;
		bool useRunAnimation = false;
	
		
		switch(Character_Select){

			case 1:
				// Priority order: Crouch > Jumping > Falling > Running > Idle
				if (crouch) {
					PlayerSprite.setTexture(PlayerCrouchTexture);
					spriteToRender = &PlayerSprite;
				}
				else if (isJumping) {
					PlayerSprite.setTexture(PlayerJumpTexture);
					spriteToRender = &PlayerSprite;
				}
				else if (isFalling) {
					PlayerSprite.setTexture(PlayerFallingTexture);
					spriteToRender = &PlayerSprite;
				}
				else if (PLAYER_STATE == 2) {  

					spriteToRender = &PlayerRun;
					useRunAnimation = true;
				}
				else {  // Idle (PLAYER_STATE == 1)
					PlayerSprite.setTexture(PlayerTexture);
					spriteToRender = &PlayerSprite;
				}
				break;

			// --- Yelow tumblepopper
			case 2:

				if(crouch){
					PlayerSprite.setTexture(YellowCrouchTexture);
					spriteToRender = &PlayerSprite;
				}
				else if (isJumping){
					PlayerSprite.setTexture(YellowPlayerJumpTexture);
					spriteToRender = &PlayerSprite;

				}
				else if (isFalling){
					PlayerSprite.setTexture(YellowPlayerFallingTexture);
					spriteToRender = &PlayerSprite;
				}
				else if (PLAYER_STATE == 2){
					spriteToRender = &YellowRun;
					useRunAnimation = true;
				}
				else{
					PlayerSprite.setTexture(YellowPlayerTexture);
					spriteToRender = &PlayerSprite;
				}
		
		}

	// ----------------------- Handling Facing right ------------------------------------------------
	
		if (facing_right) {
	
			if (useRunAnimation) {
				// Running animation - flip horizontally
				if (Character_Select == 1){
					
					PlayerRun.setScale(-3, 3);
					PlayerRun.setPosition(player_x + PlayerWidth, player_y - 20.0f);
				
				}
				else if (Character_Select == 2){

					YellowRun.setScale(-3, 3);
					YellowRun.setPosition(player_x + PlayerWidth, player_y - 20.0f);
				}
			} else {
				
				PlayerSprite.setScale(-3, 3);
				PlayerSprite.setPosition(player_x + PlayerWidth, player_y);
			}
			
			// Backpack position for facing right
			if (isJumping) {
				backpack.setPosition(player_x + 15.0f, player_y + PlayerWidth/2.0);
			}
			
			else if(crouch){
				backpack.setPosition(player_x + 7.0f, (player_y + PlayerWidth/2.0) + 15);
			}
			else {
				backpack.setPosition(player_x + 3.0f, player_y + PlayerWidth/2.0);
			}
	


			VacuumSprite.setScale(-3,-3);
	
			float pipe_x = player_x + PlayerWidth / 2.0f + 20.f;
			float pipe_y = player_y + PlayerHeight / 2.0f + 10.f;
	
// ------------------------------- Applying rotation of vacuum based on it's phase

			if (vacuum_state == 0 || vacuum_state == 4){
				VacuumSprite.setRotation(0);
				VacuumSprite.setPosition(pipe_x, pipe_y);
	
				vacuum_pipe_x = pipe_x;
				vacuum_pipe_y = pipe_y;
				pipe_rotation = 0;
	
			}
			else if (vacuum_state == 1){
				VacuumSprite.setRotation(-90);

				if(crouch){  // <- for crouch it will be slightly up and down
					VacuumSprite.setPosition(pipe_x - 15.f, pipe_y - 20.f);
					vacuum_pipe_y = pipe_y - 20.f;
					
				}
				else{
					VacuumSprite.setPosition(pipe_x - 15.f, pipe_y - 30.f);
					vacuum_pipe_y = pipe_y - 30.f;

				}
	
				vacuum_pipe_x = pipe_x - 15.f;
				pipe_rotation = -90;
				
			}
			else if( vacuum_state == 3){
				VacuumSprite.setRotation(90);
				VacuumSprite.setPosition(pipe_x - 15.f, pipe_y + 30.f);
	
				vacuum_pipe_x = pipe_x - 15.f;
				vacuum_pipe_y = pipe_y + 30.f;
				pipe_rotation = 90;
	
			}
			else if (vacuum_state == 2){
				VacuumSprite.setRotation(180);

				if(crouch){
					VacuumSprite.setPosition(pipe_x - 62.0f , pipe_y + 10.0f);
					vacuum_pipe_y = pipe_y + 10.0f;


				}
				else{
					VacuumSprite.setPosition(pipe_x - 62.0f , pipe_y - 1.0f);
					vacuum_pipe_y = pipe_y - 1.0f;
					
				}
				
				vacuum_pipe_x = pipe_x - 62.0f;
				pipe_rotation = 180;
			}
	
	
	// ----------------------- Vaccum Placement logic (to be updated)------------
	
			float vaccumXpos = player_x + PlayerWidth + 45;
			float vaccumYpos = player_y + 10.0f;
			
			if (vaccumPhase == 1){
	
				RainbowSprite.setPosition(vaccumXpos + 20 , vaccumYpos);
			}
	
			else if (vaccumPhase == 2){
				RainbowSprite.setPosition(vaccumXpos + 40 , vaccumYpos);
	
			}
	
			else if (vaccumPhase == 3){
				RainbowSprite.setPosition(vaccumXpos + 60 , vaccumYpos);
	
			}
			else if (vaccumPhase == 4){
				RainbowSprite.setPosition(vaccumXpos, vaccumYpos);
			}
			else{
				RainbowSprite.setPosition(vaccumXpos , vaccumYpos);
			}
	
			RainbowSprite.setScale(-3,3);
	
		}
	
	// ---------------------------------------- Facing Left ----------------------------------------------
	
		else {  
			if (useRunAnimation) {
				// Running animation - normal orientation
				if (Character_Select == 1){
					PlayerRun.setScale(3, 3);
					PlayerRun.setPosition(player_x, player_y - 20.0f);
				}
				else if (Character_Select == 2){
					YellowRun.setScale(3, 3);
					YellowRun.setPosition(player_x, player_y - 20.0f);
				}

			} else {
				// Static sprites - normal orientation
				if (isFalling) {
					PlayerSprite.setScale(3, 3);  // Use normal scale for falling
				} else {
					PlayerSprite.setScale(3, 3);
				}
				PlayerSprite.setPosition(player_x, player_y);
			}
			
			// Backpack position for facing left
			if (isJumping) {
				backpack.setPosition(player_x + PlayerWidth - 28.0f, player_y + PlayerWidth/2.0);
			} 
			
			else if(crouch){
				backpack.setPosition((player_x + PlayerWidth) - 16.0f, (player_y + PlayerWidth/2.0) + 15);
			}
			
			else {
				backpack.setPosition(player_x + PlayerWidth - 15.0f, player_y + PlayerWidth/2.0f);
			}
	
	// ---------------------------- Vacuum Pump position handling -----------------------------------------
	
			VacuumSprite.setScale(3,3);
	
			float pipe_x = player_x + PlayerWidth / 2.0f - 24.f;
			float pipe_y = player_y + PlayerHeight / 2.0f + 10.f;
	
			if ( vacuum_state == 0){
				VacuumSprite.setRotation(0);
				VacuumSprite.setPosition(pipe_x, pipe_y);
	
				vacuum_pipe_x = pipe_x;
				vacuum_pipe_y = pipe_y;
				pipe_rotation = 0;
	
			}

			else if (vacuum_state == 2){
				VacuumSprite.setRotation(0);
				
				if(crouch){
					VacuumSprite.setPosition(pipe_x, pipe_y + 10.f); // THis is to adjust for the crouch position
					vacuum_pipe_y = pipe_y + 10.f;

				}
				else{
					VacuumSprite.setPosition(pipe_x, pipe_y);
					vacuum_pipe_y = pipe_y;

				}
	
				vacuum_pipe_x = pipe_x;
				pipe_rotation = 0;
			}

			else if (vacuum_state == 1){
				VacuumSprite.setRotation(90);
				
				if (crouch){
					VacuumSprite.setPosition(pipe_x + 20.f, pipe_y - 20.f);
					vacuum_pipe_y = pipe_y - 20.f;
					
				}
				else{
					VacuumSprite.setPosition(pipe_x + 20.f, pipe_y - 30.f);
					vacuum_pipe_y = pipe_y - 30.f;
				
				}
				
				vacuum_pipe_x = pipe_x + 20.f;
				pipe_rotation = 90;
				

			}
			else if( vacuum_state == 3){
				VacuumSprite.setRotation(-90);
				if(crouch){
					VacuumSprite.setPosition(pipe_x + 25.f, pipe_y + 40.f);
					vacuum_pipe_y = pipe_y + 40.f;
				}
				else{
					VacuumSprite.setPosition(pipe_x + 25.f, pipe_y + 30.f);
					vacuum_pipe_y = pipe_y + 30.f;

				}
			
				vacuum_pipe_x = pipe_x + 25.f;
				pipe_rotation = -90;
			}
			else if (vacuum_state == 4){
				VacuumSprite.setRotation(180);
				VacuumSprite.setPosition(pipe_x + PlayerWidth / 2.0f + 10.0f , pipe_y - 3.0f);
			
				vacuum_pipe_x = pipe_x + PlayerWidth / 2.0f + 10.f;
				vacuum_pipe_y = pipe_y - 3.0f;
				pipe_rotation = 180;
			}
	
			float vaccumXpos = player_x - 50.0f;
			float vaccumYpos = player_y + 10.0f;
	
			if (vaccumPhase == 1){
				RainbowSprite.setPosition(vaccumXpos - 10, vaccumYpos);
			}
			else if (vaccumPhase == 2){
				RainbowSprite.setPosition(vaccumXpos - 30, vaccumYpos);
			}
			else if (vaccumPhase == 3){
				RainbowSprite.setPosition(vaccumXpos - 45, vaccumYpos);
			}
			else{
				RainbowSprite.setPosition(vaccumXpos, vaccumYpos);
			}
	
		
		RainbowSprite.setScale(3,3);
	
		}
		playerHitbox.setPosition(player_x, player_y);
		
	//--------------------------Drawing pipe --------------------------
	
	bool pipe_draw = true;
	
	if(facing_right && (vacuum_state == 0 || vacuum_state == 4)){
		pipe_draw = false;
	}
	else if(!facing_right && (vacuum_state == 0 || vacuum_state == 2) ){
		pipe_draw = false;
	}
	
	
	// ------------------------ Shooting logic ------------------------
		if(vaccum){
	
			rainbowShootHook(RainbowSprite, vacuum_pipe_x, vacuum_pipe_y, pipe_rotation, facing_right, vaccumPhase, rainbowHitboxX, rainbowHitboxY, rainbowHitboxWidth, rainbowHitboxHeight, crouch);
		    
			RectangleShape rainbowHitboxRect(Vector2f(rainbowHitboxWidth, rainbowHitboxHeight));
			rainbowHitboxRect.setFillColor(Color::Transparent);
			rainbowHitboxRect.setOutlineColor(Color::Cyan);
			rainbowHitboxRect.setOutlineThickness(2);
			rainbowHitboxRect.setPosition(rainbowHitboxX, rainbowHitboxY);
			rainbowHitboxRect.setPosition(rainbowHitboxX, rainbowHitboxY);
			
			window.draw(RainbowSprite);
			window.draw(rainbowHitboxRect);
	
		}
		
			
			backpack.setScale(1.5, 1.5);
			window.draw(backpack);
			window.draw(*spriteToRender);  
			window.draw(playerHitbox);
			if(pipe_draw){
				window.draw(VacuumSprite);
			}
	}


	enterKeyPressed = currentEnterKey;
	PKeyPressed = currentPKey;
	MKeyPressed = currentMKey;
		

		window.display();
	}

	//stopping music and deleting level array
	lvlMusic.stop();
	for (int i = 0; i < height; i++)
	{
		delete[] lvl[i];
	}
	delete[] lvl;

	return 0;
}

