#pragma once
// DYNAMIC FOV
bool dynamicEsp = false;
float decreaseStep = 0.1f;
bool isButtonPressed = false;
int steps = 300;

// AIM
bool Aimbot = false;
bool trigerbot = false;
bool fovcircle = false;
bool Filled_Fov = false;
bool humanAim = false;


// ESP
bool lobby = false;
bool Esp = false;
bool slefESP = false;
bool BoxEsp = false;
bool D3_Box = false;
bool CorneredBoxEsp = false;
bool FilledBoxEsp = false;
bool DistanceEsp = false;
bool Name_Esp = false;
bool SkeletonEsp = false;
bool bigDickEsp = false;
bool tinyDickEsp = false;
bool ClosestSnaplines = false;
bool VisibleCheck = false;
float NotVisColor[3] = { 0.92f, 0.10f, 0.14f }; // Red
float VisColor[3] = { 144.00f, 0.00f, 255.00f };   // Purple
float EspThickness = 1.f;
bool headCircleEsp = false;
bool reloadEsp = false;
bool weaponEsp = false;
bool outlinedBoxEsp = false;
bool nameEsp = false;
bool Box_3D = false; // need test

bool Esp_Line_Bottom = false; // need test
bool Esp_Line_Top = false;
bool Crosshair_Lines = false;


// MISC
bool ShowMenu = true;
bool particles = true;
bool StreamProof = false;
bool Crosshair = false;
bool showFps = true;

// EXPLOITS
bool BigPlayers = false; // new test
bool noRecoil = false;
bool bigPlayer = false;
bool airstuck = false;
bool spinbot = false;

// ADDITIONAL VARIABLES
float BoxWidthValue = 0.550;
float ChangerFOV = 80;
int aimkeypos = 3;
int aimbone = 0;

float smooth = 5.0f;
int VisDist = 2400.f;
float AimFOV = 150;
int aimkey;
int hitbox;
int hitboxpos = 0;
float CrossHair[3];
float CrossThickness = 1.5f;




float Skeleton_Thickness = 1.5f;




namespace PlayerColor {
	

	float SkeletonVisible[4] = { 1.f, 1.f, 1.f, 1.0f };
	float SkeletonNotVisible[4] = { 1.f, 1.f, 1.f, 1.0f };



}