#include "pasterx.h"
#include "driver.h"
#include "Print.hpp"
#include "d3d9_x.h"
#include "xor.hpp"
#include <dwmapi.h>
#include <vector>
#include "Keybind.h"
#include "offsets.h"
#include "xstring"
#include "Definitions.h"
#define color1 (WORD)(0x0001 | 0x0000)
#define color2 (WORD)(0x0002 | 0x0000)
#define color3 (WORD)(0x0003 | 0x0000)
#define color4 (WORD)(0x0004 | 0x0000)
#define color5 (WORD)(0x0005 | 0x0000)
#define color6 (WORD)(0x0006 | 0x0000)
#define color7 (WORD)(0x0007 | 0x0000)
#define color8 (WORD)(0x0008 | 0x0000)
#define color9 (WORD)(0x0008 | 0x0000)
#define COLOR(h, c) SetConsoleTextAttribute(h, c);

#include "pasterx.h"
#include "driver.h"
#include "Print.hpp"
#include "d3d9_x.h"
#include "xor.hpp"
#include <dwmapi.h>
#include <vector>
#include "Keybind.h"
#include "offsets.h"
#include "xstring"
#include "Definitions.h"
#define color1 (WORD)(0x0001 | 0x0000)
#define color2 (WORD)(0x0002 | 0x0000)
#define color3 (WORD)(0x0003 | 0x0000)
#define color4 (WORD)(0x0004 | 0x0000)
#define color5 (WORD)(0x0005 | 0x0000)
#define color6 (WORD)(0x0006 | 0x0000)
#define color7 (WORD)(0x0007 | 0x0000)
#define color8 (WORD)(0x0008 | 0x0000)
#define color9 (WORD)(0x0008 | 0x0000)
#define COLOR(h, c) SetConsoleTextAttribute(h, c);

//static int aimkeypos = 3;
//static int aimbone = 1;

int faken_rot = 0;

float BOG_TO_GRD(float BOG) {
	return (180 / M_PI) * BOG;
}

float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}


DWORD64 bonearray;
DWORD_PTR Uworld;
DWORD_PTR LocalPawn;
DWORD_PTR PlayerState;
DWORD_PTR Localplayer;
DWORD_PTR Rootcomp;
DWORD_PTR PlayerController;
DWORD_PTR Persistentlevel;
uintptr_t PlayerCameraManager;
uintptr_t virtualaddy;
Vector3 localactorpos;
Vector3 relativelocation;
Vector3 Relativelocation;

IDirect3DTexture9* logo = nullptr;
IDirect3DTexture9* option1 = nullptr;
IDirect3DTexture9* option2 = nullptr;
IDirect3DTexture9* option3 = nullptr;

//Helpers
ImFont* m_pFont;
ImFont* details;
ImFont* arrow;
ImFont* mainfont;
ImFont* titlefont;
ImFont* icons;
ImVec2 position;
ImDrawList* drawlist;
ImVec2 skeletonpos;
ImDrawList* skeletondraw;
int tabs = 0;

uint64_t TargetPawn;
int localplayerID;


RECT GameRect = { NULL };
D3DPRESENT_PARAMETERS d3dpp;
DWORD ScreenCenterX;
DWORD ScreenCenterY;
Vector3 LocalRelativeLocation; struct FBoxSphereBounds
{
	struct Vector3                                     Origin;                                                   // 0x0000(0x0018) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	struct Vector3                                     BoxExtent;                                                // 0x0018(0x0018) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	double                                             SphereRadius;                                             // 0x0030(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
};
static void xCreateWindow();
static void xInitD3d();
static void xMainLoop();
static void xShutdown();
static LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static HWND Window = NULL;
IDirect3D9Ex* p_Object = NULL;
static LPDIRECT3DDEVICE9 D3dDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9 TriBuf = NULL;

typedef struct {
	float X, Y, Z;
} FVector;

typedef struct {
	float X, Y;
} FVector2D;

typedef struct {
	float Pitch;
	float Yaw;
	float Roll;
} FRotator;

typedef struct {
	FVector Location;
	float FOV;
	float OrthoWidth;
	float OrthoNearClipPlane;
	float OrthoFarClipPlane;
	float AspectRatio;
} FMinimalViewInfo;

typedef struct {
	float M[4][4];
} FMatrix;

typedef struct {
	FVector ViewOrigin;
	char _padding_0[4];
	FMatrix ViewRotationMatrix;
	FMatrix ProjectionMatrix;
} FSceneViewProjectionData;

std::string encryptString(const std::string& input) { // + 1 ascii
	std::string encrypted;
	for (char c : input) {
		encrypted.push_back(c + 1);
	}
	return encrypted;
}

std::string decryptString(const std::string& encrypted) { // - 1 ascii
	std::string decrypted;
	for (char c : encrypted) {
		decrypted.push_back(c - 1);
	}
	return decrypted;
}


class UClass {
public:
	BYTE _padding_0[0x40];
	UClass* SuperClass;
};

class UObject {
public:
	PVOID VTableObject;
	DWORD ObjectFlags;
	DWORD InternalIndex;
	UClass* Class;
	BYTE _padding_0[0x8];
	UObject* Outer;

	inline BOOLEAN IsA(PVOID parentClass) {
		for (auto super = this->Class; super; super = super->SuperClass) {
			if (super == parentClass) {
				return TRUE;
			}
		}

		return FALSE;
	}
};

void DrawLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
}


class FUObjectItem {
public:
	UObject* Object;
	DWORD Flags;
	DWORD ClusterIndex;
	DWORD SerialNumber;
	DWORD SerialNumber2;
};

class TUObjectArray {
public:
	FUObjectItem* Objects[9];
};

class GObjects {
public:
	TUObjectArray* ObjectArray;
	BYTE _padding_0[0xC];
	DWORD ObjectCount;
};


template<class T>
struct TArray {
	friend struct FString;

public:
	inline TArray() {
		Data = nullptr;
		Count = Max = 0;
	};

	inline INT Num() const {
		return Count;
	};

	inline T& operator[](INT i) {
		return Data[i];
	};

	inline BOOLEAN IsValidIndex(INT i) {
		return i < Num();
	}

private:
	T* Data;
	INT Count;
	INT Max;
};

struct FString : private TArray<WCHAR> {
	FString() {
		Data = nullptr;
		Max = Count = 0;
	}

	FString(LPCWSTR other) {
		Max = Count = static_cast<INT>(wcslen(other));

		if (Count) {
			Data = const_cast<PWCHAR>(other);
		}
	};

	inline BOOLEAN IsValid() {
		return Data != nullptr;
	}

	inline PWCHAR c_str() {
		return Data;
	}
};

VOID(*FreeInternal)(PVOID) = nullptr;

VOID Free(PVOID buffer) {
	FreeInternal(buffer);
}

#pragma once

namespace detail
{
	extern "C" void* _spoofer_stub();

	template <typename Ret, typename... Args>
	static inline auto shellcode_stub_helper(
		const void* shell,
		Args... args
	) -> Ret
	{
		auto fn = (Ret(*)(Args...))(shell);
		return fn(args...);
	}

	template <std::size_t Argc, typename>
	struct argument_remapper
	{
		template<
			typename Ret,
			typename First,
			typename Second,
			typename Third,
			typename Fourth,
			typename... Pack
		>
		static auto do_call(const void* shell, void* shell_param, First first, Second second,
			Third third, Fourth fourth, Pack... pack) -> Ret
		{
			return shellcode_stub_helper< Ret, First, Second, Third, Fourth, void*, void*, Pack... >(shell, first, second, third, fourth, shell_param, nullptr, pack...);
		}
	};

	template <std::size_t Argc>
	struct argument_remapper<Argc, std::enable_if_t<Argc <= 4>>
	{
		template<
			typename Ret,
			typename First = void*,
			typename Second = void*,
			typename Third = void*,
			typename Fourth = void*
		>
		static auto do_call(
			const void* shell,
			void* shell_param,
			First first = First{},
			Second second = Second{},
			Third third = Third{},
			Fourth fourth = Fourth{}
		) -> Ret
		{
			return shellcode_stub_helper<
				Ret,
				First,
				Second,
				Third,
				Fourth,
				void*,
				void*
			>(
				shell,
				first,
				second,
				third,
				fourth,
				shell_param,
				nullptr
			);
		}
	};
}

template <typename Ret, typename... Args>
static inline auto SpoofCall(Ret(*fn)(Args...), Args... args) -> Ret
{
	static const void* jmprbx = nullptr;
	if (!jmprbx) {
		const auto ntdll = reinterpret_cast<const unsigned char*>(::GetModuleHandleW(NULL));
		const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(ntdll);
		const auto nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(ntdll + dos->e_lfanew);
		const auto sections = IMAGE_FIRST_SECTION(nt);
		const auto num_sections = nt->FileHeader.NumberOfSections;

		constexpr char section_name[5]{ '.', 't', 'e', 'x', 't' };
		const auto     section = std::find_if(sections, sections + num_sections, [&](const auto& s) {
			return std::equal(s.Name, s.Name + 5, section_name);
			});

		constexpr unsigned char instr_bytes[2]{ 0xFF, 0x26 };
		const auto              va = ntdll + section->VirtualAddress;
		jmprbx = std::search(va, va + section->Misc.VirtualSize, instr_bytes, instr_bytes + 2);
	}

	struct shell_params
	{
		const void* trampoline;
		void* function;
		void* rdx;
	};

	shell_params p
	{
		jmprbx,
		reinterpret_cast<void*>(fn)
	};

	using mapper = detail::argument_remapper<sizeof...(Args), void>;
	return mapper::template do_call<Ret, Args...>((const void*)&detail::_spoofer_stub, &p, args...);
}

namespace SpoofRuntime {
	inline float acosf_(float x)
	{
		return SpoofCall(acosf, x);
	}

	inline float atan2f_(float x, float y)
	{
		return SpoofCall(atan2f, x, y);
	}
}



#define BONE_HEAD_ID (109)
#define BONE_NECK_ID (66)
#define BONE_CHEST_ID (37)
#define BONE_PELVIS_ID (2)




FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	int isCached = driver.read<int>(mesh + OFFSETS::BoneCache);
	bonearray = driver.read<DWORD_PTR>(mesh + OFFSETS::BoneArray);
	if (bonearray == NULL)
	{
		bonearray = driver.read<DWORD_PTR>(mesh + OFFSETS::BoneArray + 0x10);
	}
	return driver.read<FTransform>(bonearray + (index * 0x60));
}


Vector3 GetBoneWithRotation(uintptr_t mesh, int bone_id)
{
	uintptr_t bone_array = driver.read<uintptr_t>(mesh + OFFSETS::BoneArray);
	if (bone_array == NULL) bone_array = driver.read<uintptr_t>(mesh + OFFSETS::BoneArray + 0x10);
	FTransform bone = driver.read<FTransform>(bone_array + (bone_id * 0x60));
	FTransform component_to_world = driver.read<FTransform>(mesh + OFFSETS::ComponetToWorld);
	D3DMATRIX matrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
	return Vector3(matrix._41, matrix._42, matrix._43);
}

D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}


double __fastcall Atan2(double a1, double a2)
{
	double result; // xmm0_8

	result = 0.0;
	if (a2 != 0.0 || a1 != 0.0)
		return atan2(a1, a2);
	return result;
}
double __fastcall FMod(double a1, double a2)
{
	if (fabs(a2) > 0.00000001)
		return fmod(a1, a2);
	else
		return 0.0;
}

double ClampAxis(double Angle)
{
	// returns Angle in the range (-360,360)
	Angle = FMod(Angle, (double)360.0);

	if (Angle < (double)0.0)
	{
		// shift to [0,360) range
		Angle += (double)360.0;
	}

	return Angle;
}

double NormalizeAxis(double Angle)
{
	// returns Angle in the range [0,360)
	Angle = ClampAxis(Angle);

	if (Angle > (double)180.0)
	{
		// shift to (-180,180]
		Angle -= (double)360.0;
	}

	return Angle;
}




FRotator Rotator(FQuat* F)
{
	const double SingularityTest = F->z * F->x - F->w * F->y;
	const double YawY = 2.f * (F->w * F->z + F->x * F->y);
	const double YawX = (1.f - 2.f * ((F->y * F->y) + (F->z * F->z)));

	const double SINGULARITY_THRESHOLD = 0.4999995f;
	const double RAD_TO_DEG = 57.295776;
	float Pitch, Yaw, Roll;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		Pitch = -90.f;
		Yaw = (Atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = NormalizeAxis(-Yaw - (2.f * Atan2(F->x, F->w) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		Pitch = 90.f;
		Yaw = (Atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = NormalizeAxis(Yaw - (2.f * Atan2(F->x, F->w) * RAD_TO_DEG));
	}
	else
	{
		Pitch = (asin(2.f * SingularityTest) * RAD_TO_DEG);
		Yaw = (Atan2(YawY, YawX) * RAD_TO_DEG);
		Roll = (Atan2(-2.f * (F->w * F->x + F->y * F->z), (1.f - 2.f * ((F->x * F->x) + (F->y * F->y)))) * RAD_TO_DEG);
	}

	FRotator RotatorFromQuat = FRotator{ Pitch, Yaw, Roll };
	return RotatorFromQuat;
}
struct CameraInfo
{
	Vector3 location;
	Vector3 rotation;
	float fov;
};

float fov;

CameraInfo GetCameraInfo()
{
	CameraInfo camera;
	auto location_pointer = driver.read<uintptr_t>(Uworld + 0x110); //110
	auto rotation_pointer = driver.read<uintptr_t>(Uworld + 0x120); //120

	struct RotationInfo
	{
		double pitch;
		char pad_0008[24];
		double yaw;
		char pad_0028[424];
		double roll;
	} rotationInfo;

	rotationInfo.pitch = driver.read<double>(rotation_pointer);
	rotationInfo.yaw = driver.read<double>(rotation_pointer + 0x20);
	rotationInfo.roll = driver.read<double>(rotation_pointer + 0x1d0);

	camera.location = driver.read<Vector3>(location_pointer);
	camera.rotation.x = asin(rotationInfo.roll) * (180.0 / M_PI);
	camera.rotation.y = ((atan2(rotationInfo.pitch * -1, rotationInfo.yaw) * (180.0 / M_PI)) * -1) * -1;
	camera.fov = driver.read<float>((uintptr_t)PlayerController + 0x394) * 90.f;

	return camera;
}



Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	CameraInfo viewInfo = GetCameraInfo();
	D3DMATRIX tempMatrix = Matrix(viewInfo.rotation);
	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
	Vector3 vDelta = WorldLocation - viewInfo.location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));
	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return Vector3((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(viewInfo.fov * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(viewInfo.fov * (float)M_PI / 360.f))) / vTransformed.z, 0);
}




void DrawStrokeText(int x, int y, const char* str)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);

	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(255, 255, 255, 255)), utf_8_2.c_str());
}


float DrawLobbyText(ImFont* pFont, const std::string& text, float size, ImU32 color, bool center)
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((textSize.x / 2.0f) + 1, (textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((textSize.x / 2.0f) - 1, (textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((textSize.x / 2.0f) + 1, (textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((textSize.x / 2.0f) - 1, (textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(textSize.x / 2.0f, textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(+1, (textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(-1, (textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(+1, (textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(-1, (textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());


		}

		y = +textSize.y * (i + 1);
		i++;
	}
	return y;
}


float DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}
	return y;
}


void DrawText1(int x, int y, const char* str, RGBA* color)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}
std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

std::wstring MBytesToWString(const char* lpcszString)
{
	//SPOOF_FUNC;
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}
void DrawString(float fontSize, int x, int y, RGBA* color, bool bCenter, bool stroke, const char* format, ...)
{
	//SPOOF_FUNC;
	va_list args;
	va_start(args, format);

	char buffer[1024];
	vsnprintf_s(buffer, sizeof(buffer), format, args);

	va_end(args);

	std::string text = WStringToUTF8(MBytesToWString(buffer).c_str());

	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - static_cast<int>(textSize.x / 2);
		y = y - static_cast<int>(textSize.y / 2);
	}

	ImU32 textColor = ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0f, color->G / 255.0f, color->B / 255.0f, color->A / 255.0f));

	if (stroke)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i != 0 || j != 0)
				{
					ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + i, y + j), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
				}
			}
		}
	}

	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), textColor, text.c_str());
}

void LobbyLine(int x1, int y1, int x2, int y2, RGBA* color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(959, 1100), ImVec2(960, 540), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 000.0, color->B / 000.0, color->A / 255.0)), 1.0f);
}

void LobbyBox(float X, float Y, float W, float H, RGBA* color)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(815 + 1, 967 + 1), ImVec2(((830 + 275) - 1), ((851 - 625) - 1)), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
	//ImGui::GetOverlayDrawList()->AddRect(ImVec2(150, 60), ImVec2(150 + 63, 60 + 125), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

void DrawCircle(int x, int y, int radius, RGBA* color, int segments)
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}
void DrawBox(float X, float Y, float W, float H, RGBA* color)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}
void DrawFilledRect(int x, int y, int w, int h, ImU32 color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}
void DrawNormalBox(int x, int y, int w, int h, int borderPx, ImU32 color)
{

	DrawFilledRect(x + borderPx, y, w, borderPx, color); //top 
	DrawFilledRect(x + w - w + borderPx, y, w, borderPx, color); //top 
	DrawFilledRect(x, y, borderPx, h, color); //left 
	DrawFilledRect(x, y + h - h + borderPx * 2, borderPx, h, color); //left 
	DrawFilledRect(x + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRect(x + w - w + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRect(x + w + borderPx, y, borderPx, h, color);//right 
	DrawFilledRect(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color);//right 
}



// draw_cornered_box


/*void draw_cornered_box(int x, int y, int w, int h, const ImColor color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + (h / 3)), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y), ImVec2(x + (w / 3), y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - (w / 3), y), ImVec2(x + w, y), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + (h / 3)), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y + h - (h / 3)), ImVec2(x, y + h), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + (w / 3), y + h), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w - (w / 3), y + h), ImVec2(x + w, y + h), color, thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x + w, y + h - (h / 3)), ImVec2(x + w, y + h), color, thickness);
}*/




// normal

void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness) {
	float lineW = (W / 3);
	float lineH = (H / 3);

	//corners
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}



void DrawBox(int X, int Y, int W, int H, const ImU32& color, int thickness) {
	float lineW = (W / 1);
	float lineH = (H / 1);
	ImDrawList* Drawlist = ImGui::GetOverlayDrawList();

	//corners
	Drawlist->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	Drawlist->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	Drawlist->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	Drawlist->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	Drawlist->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	Drawlist->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	Drawlist->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	Drawlist->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}

void Normalbox(int X, int Y, int W, int H, const ImU32& color, int thickness, int borderPx) {
	float lineW = (W / 1);
	float lineH = (H / 1);
	DrawFilledRect(X + borderPx, Y, W, borderPx, ImColor(10, 10, 10, 180));


	//corners
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}





void DrawOutlinedNormalBox(int X, int Y, int W, int H, const ImU32& color, int thickness, int borderPx) {
	//DrawFilledRect(X + borderPx, Y, W, borderPx, ImColor(10, 10, 10, 180)); // Draw the top border

	Normalbox(X, Y, W, H, IM_COL32(0, 0, 0, 255), thickness, 0);

	// Draw the inner filled box
	Normalbox(X + thickness, Y + thickness, W - 2 * thickness, H - 2 * thickness, color, thickness, 0);

	// Draw the bottom border
	//DrawFilledRect(X + borderPx, Y + H - borderPx, W, borderPx, ImColor(10, 10, 10, 180));
}



typedef struct _FNlEntity
{
	uint64_t Actor;
	int ID;
	uint64_t mesh;
}FNlEntity;

std::vector<FNlEntity> entityList;


struct HandleDisposer
{
	using pointer = HANDLE;
	void operator()(HANDLE handle) const
	{
		if (handle != NULL || handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
		}
	}
};
using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;
void clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

static std::uint32_t _GetProcessId(std::string process_name) {
	PROCESSENTRY32 processentry;
	const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

	if (snapshot_handle.get() == INVALID_HANDLE_VALUE)
		return 0;

	processentry.dwSize = sizeof(MODULEENTRY32);

	while (Process32Next(snapshot_handle.get(), &processentry) == TRUE) {
		if (process_name.compare(processentry.szExeFile) == 0)
			return processentry.th32ProcessID;
	}
	return 0;
}
// random standard header




DWORD Menuthread(LPVOID in)
{
	while (1)
	{
		if (MouseController::GetAsyncKeyState(VK_INSERT) & 100) {
			// ShowMenu = !ShowMenu;
		}
		Sleep(1);
	}
}
using namespace std;

void SetConsoleSize(int width, int height)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rect = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
	COORD size = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
	SetConsoleWindowInfo(consoleHandle, TRUE, &rect);
	SetConsoleScreenBufferSize(consoleHandle, size);
}

std::string random_string(std::string::size_type length)
{
	static auto& chrs = "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#%^&*()";

	thread_local static std::mt19937 rg{ std::random_device{}() };
	thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

	std::string s;

	s.reserve(length);

	while (length--)
		s += chrs[pick(rg)];

	return s + ".exe";
}
std::string path()
{
	char shitter[_MAX_PATH]; // defining the path
	GetModuleFileNameA(NULL, shitter, _MAX_PATH); // getting the path
	return std::string(shitter); //returning the path
}
void rndmTitle()
{
	constexpr int length = 25;
	const auto characters = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	TCHAR title[length + 1]{};

	for (int j = 0; j != length; j++)
	{
		title[j] += characters[rand() % 55 + 1];
	}
}
struct slowly_printing_string {
	std::string data;
	long int delay;
};
std::ostream& operator<<(std::ostream& out, const slowly_printing_string& s) {
	for (const auto& c : s.data) {
		out << c << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(s.delay));
	}
	return out;
}


void checkurmomez2()
{
	system(E("taskkill /f /im HTTPDebuggerUI.exe >nul 2>&1"));
	system(E("taskkill /f /im HTTPDebuggerSvc.exe >nul 2>&1"));
	system(E("taskkill /f /im Ida64.exe >nul 2>&1"));
	system(E("taskkill /f /im OllyDbg.exe >nul 2>&1"));
	system(E("taskkill /f /im Dbg64.exe >nul 2>&1"));
	system(E("taskkill /f /im Dbg32.exe >nul 2>&1"));
	system(E("sc stop HTTPDebuggerPro >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /f /im HTTPDebuggerUI.exe >nul 2>&1"));
	system(E("taskkill /f /im HTTPDebuggerSvc.exe >nul 2>&1"));
	system(E("sc stop HTTPDebuggerPro >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq x64dbg*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq x32dbg*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq ollydbg*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq fiddler*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq fiddler*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq charles*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq ida*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1"));
	system(E("sc stop HTTPDebuggerPro >nul 2>&1"));
	system(E("sc stop HTTPDebuggerProSdk >nul 2>&1"));
	system(E("sc stop KProcessHacker3 >nul 2>&1"));
	system(E("sc stop KProcessHacker2 >nul 2>&1"));
	system(E("sc stop KProcessHacker1 >nul 2>&1"));
	system(E("sc stop wireshark >nul 2>&1"));
	system(E("taskkill /f /im HTTPDebuggerUI.exe >nul 2>&1"));
	system(E("taskkill /f /im HTTPDebuggerSvc.exe >nul 2>&1"));
	system(E("sc stop HTTPDebuggerPro >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq x64dbg*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq x32dbg*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq ollydbg*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq fiddler*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /FI \"IMAGENAME eq die*\" /IM * /F /T >nul 2>&1"));
	system(E("taskkill /f /im HTTPDebuggerSvc.exe >nul 2>&1"));
	system(E("taskkill /f /im HTTPDebugger.exe >nul 2>&1"));
	system(E("taskkill /f /im FolderChangesView.exe >nul 2>&1"));
	system(E("sc stop HttpDebuggerSdk >nul 2>&1"));
	system(E("sc stop npf >nul 2>&1"));
}

DWORD loopDbg2(LPVOID in) {

	while (1) {
		if (GetAsyncKeyState(NULL) & 1) {

		}
		else
		{
			checkurmomez2();
		}



	}
}

int main(int argc, const char* argv[])

{
	HANDLE hpStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	MouseController::Init();

	CreateThread(NULL, NULL, Menuthread, NULL, NULL, NULL);
	SetConsoleTitleA(random_string(30).c_str());
	//std::rename(path().c_str(), random_string(15).c_str());
	COLOR(hpStdout, color3);

	Log3(E(""));
	std::cout << slowly_printing_string{ " Connecting...\n\n\n",10 };
	CreateThread(NULL, NULL, loopDbg2, NULL, NULL, NULL);
	//system(E("curl https://cdn.discordapp.com/attachments/1109503024260460615/1112053353975906505/Driver_That_uses_SpacePort_Hook.sys --output C:\\Windows\\System32\\mappeddrv.sys >nul 2>&1"));
	//system(E("curl https://cdn.discordapp.com/attachments/1113602589980905602/1130585133779517530/kdmapper.exe --output C:\\Windows\\System32\\mapperrrrr.exe >nul 2>&1")); //loads the driver
	//system(E("start https://discord.gg/urp2cname"));
	Log3(E(""));
	std::cout << slowly_printing_string{ " Press [ ANY KEY ] To Load Drivers\n",1 };
	Beep(500, 500);
	system(E("pause >nul 2>&1"));
	//system(E("C:\\Windows\\System32\\mapperrrrr.exe C:\\Windows\\System32\\mappeddrv.sys"));
	//std::remove(E("C:\\Windows\\System32\\mappeddrv.sys"));
	//std::remove(E("C:\\Windows\\System32\\mapperrrrr.exe"));

	Beep(500, 500);
	system(E("cls"));
	Log3(E(""));
	std::cout << slowly_printing_string{ " Loaded Drivers!!!\n\n\n",10 };
	Log3(E(""));
	std::cout << slowly_printing_string{ " Press [ ANY KEY ] To Load The Cheat\n",10 };
	system(E("pause >nul 2>&1"));
	Beep(325, 300);
	while (hwnd == NULL)
	{
		XorS(wind, "Fortnite  ");
		hwnd = FindWindowA(0, wind.decrypt());
		Sleep(100);
	}


	if (!librarys::init)
	{
		printf("\n [ - ] The libraries arent initiaized");
		Sleep(5000);
		exit(0);
	}
	else
	{
		printf("\n [ + ] The libraries initiaized");
	}


	while (!driver.init()) {

		std::cout << "Driver is not loaded\n";
	}

	driver.process_id = driver.get_process_id("FortniteClient-Win64-Shipping.exe");

	driver.base_address = driver.get_base_address();

	base_address = driver.base_address;

	std::cout << " [ base_address ] -> " << base_address << std::endl;
	//printf(" Fortnites Id Base Is: %p", (void*)base_address);
	Log3(E(" Loaded\n"));
	Beep(500, 500);
	xCreateWindow();
	xInitD3d();

	xMainLoop();
	xShutdown();

	return 0;
}



void SetWindowToTarget()
{
	while (true)
	{
		if (hwnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(hwnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(Window, GameRect.left, GameRect.top, Width, Height, true);
		}
		else
		{
			exit(0);
		}
	}
}




const MARGINS Margin = { -1 };

void xCreateWindow()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

	WNDCLASS windowClass = { 0 };
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hInstance = NULL;
	windowClass.lpfnWndProc = WinProc;
	windowClass.lpszClassName = "Notepad";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&windowClass))
		std::cout << "\n\n Notepad";

	Window = CreateWindow("Notepad",
		NULL,
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		NULL,
		NULL);

	ShowWindow(Window, SW_SHOW);

	DwmExtendFrameIntoClientArea(Window, &Margin);
	SetWindowLong(Window, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	UpdateWindow(Window);
}




void xInitD3d()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.hDeviceWindow = Window;
	d3dpp.Windowed = TRUE;

	p_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice);

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX9_Init(D3dDevice);

	ImGui::StyleColorsClassic();
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowMinSize = ImVec2(600, 300);
	style->WindowBorderSize = 0;

	style->ChildRounding = 4.25f;
	style->FrameRounding = 4.25f;
	style->ScrollbarRounding = 4.25f;
	style->GrabRounding = 4.25f;
	style->PopupRounding = 4.25f;
	style->WindowRounding = 4.25f;

	style->WindowBorderSize = 3.0f;
	style->WindowRounding = 4.25f;
	style->ItemSpacing = ImVec2(4, 4);
	style->ItemInnerSpacing = ImVec2(4, 4);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 4.25f;
	style->GrabMinSize = 4.25f;
	style->GrabRounding = 4.25f;
	style->ChildRounding = 4.25f;
	style->FrameRounding = 4.25f;


	style->Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 30, 255);
	style->Colors[ImGuiCol_Border] = ImColor(30, 30, 30, 255);
	style->Colors[ImGuiCol_FrameBg] = ImColor(50, 50, 50, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(144, 0, 255, 255); // Purple
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(90, 90, 90, 255);
	style->Colors[ImGuiCol_CheckMark] = ImColor(144, 0, 255, 255); // Purple
	style->Colors[ImGuiCol_SliderGrab] = ImColor(144, 0, 255, 255); // Purple
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(144, 0, 255, 255); // Purple
	style->Colors[ImGuiCol_Button] = ImColor(3, 24, 252, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(90, 90, 90, 255);
	style->Colors[ImGuiCol_Separator] = ImColor(89, 3, 247, 255);
	style->Colors[ImGuiCol_TitleBg] = ImColor(3, 24, 252);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(3, 24, 252);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(3, 24, 252);

	style->WindowTitleAlign.x = 0.50f;

	XorS(font, "C:\\Windows\\Fonts\\Bahnschrift.ttf");
	m_pFont = io.Fonts->AddFontFromFileTTF(font.decrypt(), 14.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

	p_Object->Release();
}

struct { //color configs
	float Black[3];
	float RGBRed[3] = { 1.0f, 0.0f, 0.0f };
	float RGBYelllow[3] = { 1.0f, 1.0f, 0.0f };
	float RGBGreen[3] = { 0.0f, 1.0f, 0.0f };
	float RGBBlue[3] = { 0.0f, 0.0f, 1.0f };
	float CMYKRed[3] = { 0.92f, 0.10f, 0.14f };
	float CMYKYellow[3] = { 1.0f, 0.94f, 0.0f };
	float CMYKGreen[3] = { 0.0f, 0.65f, 3.17f };
	float CMYKBlue[3] = { 0.18f, 0.19f, 0.57f };
	float PastelRed[3] = { 255.00f, 0.00f, 119.00f };
	float PastelRedOrange[3] = { 0.97f, 0.67f, 0.50f };
	float PastelYellowOrange[3] = { 0.99f, 0.77f, 0.53f };
	float PastelYellow[3] = { 1.0f, 0.96f, 0.6f };
	float PastelPeaGreen[3] = { 0.76f, 0.87f, 0.60f };
	float PastelYellowGreen[3] = { 0.63f, 0.82f, 0.61f };
	float PastelGreen[3] = { 0.50f, 0.79f, 0.61f };
	float PastelGreenCyan[3] = { 0.47f, 0.8f, 0.78f };
	float PastelCyan[3] = { 0.42f, 0.81f, 0.96f };
	float PastelCyanBlue[3] = { 0.49f, 0.65f, 0.85f };
	float PastelBlue[3] = { 0.51f, 0.57f, 0.79f };
	float PastelBlueViolet[3] = { 0.52f, 0.50f, 0.74f };
	float PastelViolet[3] = { 0.63f, 0.52f, 0.74f };
	float PastelVioletMagenta[3] = { 0.74f, 0.54f, 0.74f };
	float PastelMagenta[3] = { 0.95f, 0.60f, 0.75f };
	float PastelMagentaRed[3] = { 0.96f, 0.59f, 0.61f };
	float LightRed[3] = { 0.94f, 0.42f, 0.30f };
	float LightRedOrange[3] = { 0.96f, 0.55f, 0.33f };
	float LightYellowOrange[3] = { 0.98f, 0.68f, 0.36f };
	float LightYellow[3] = { 1.0f, 0.96f, 0.40f };
	float LightPeaGreen[3] = { 0.67f, 0.82f, 0.45f };
	float LightYellowGreen[3] = { 0.48f, 0.77f, 0.46f };
	float LightGreen[3] = { 0.23f, 0.72f, 0.47f };
	float LightGreenCyan[3] = { 0.10f, 0.73f, 0.70f };
	float LightCyan[3] = { 0.0f, 0.74f, 0.95f };
	float LightCyanBlue[3] = { 0.26f, 0.54f, 0.79f };
	float LightBlue[3] = { 0.33f, 0.45f, 0.72f };
	float LightBlueViolet[3] = { 0.37f, 0.36f, 0.65f };
	float LightViolet[3] = { 0.52f, 0.37f, 0.65f };
	float LightVioletMagenta[3] = { 0.65f, 0.39f, 0.65f };
	float LightMagenta[3] = { 0.94f, 0.43f, 0.66f };
	float LightMagentaRed[3] = { 0.94f, 0.42f, 0.49f };
	float Red[3] = { 0.92f, 0.10f, 0.14f };
	float RedOrange[3] = { 0.94f, 0.39f, 0.13f };
	float YellowOrange[3] = { 0.96f, 0.58f, 0.11f };
	float Yellow[3] = { 1.0f, 0.94f, 0.0f };
	float PeaGreen[3] = { 0.55f, 0.77f, 0.24f };
	float YellowGreen[3] = { 0.22f, 0.70f, 0.29f };
	float Green[3] = { 0.0f, 0.65f, 0.31f };
	float GreenCyan[3] = { 0.0f, 0.66f, 0.61f };
	float Cyan[3] = { 0.0f, 0.68f, 0.93f };
	float CyanBlue[3] = { 0.0f, 0.44f, 0.34f };
	float Blue[3] = { 0.0f, 0.32f, 0.65f };
	float BlueViolet[3] = { 0.19f, 0.19f, 0.57f };
	float Violet[3] = { 0.18f, 0.19f, 0.57f };
	float VioletMagenta[3] = { 0.57f, 0.15f, 5.63f };
	float Magenta[3] = { 0.92f, 0.0f, 0.54f };
	float MagentaRed[3] = { 0.92f, 0.07f, 0.35f };
	float DarkRed[3] = { 0.61f, 0.04f, 0.05f };
	float DarkROrange[3] = { 0.62f, 0.25f, 0.05f };
	float DarkYellowOrange[3] = { 0.53f, 0.38f, 0.03f };
	float DarkYellow[3] = { 0.67f, 0.62f, 0.0f };
	float DarkPeaGreen[3] = { 0.34f, 0.52f, 0.15f };
	float DarkYellowGreen[3] = { 0.09f, 0.48f, 0.18f };
	float DarkGreen[3] = { 0.0f, 0.44f, 0.21f };
	float DarkGreenCyan[3] = { 0.0f, 0.45f, 0.41f };
	float DarkCyan[3] = { 0.0f, 0.46f, 0.63f };
	float DarkCyanBlue[3] = { 0.0f, 0.29f, 0.50f };
	float DarkBlue[3] = { 0.0f, 0.20f, 0.44f };
	float DarkBlueViolet[3] = { 0.10f, 0.07f, 0.39f };
	float DarkViolet[3] = { 0.26f, 0.05f, 0.38f };
	float DarkVioletMagenta[3] = { 0.38f, 0.01f, 0.37f };
	float DarkMagenta[3] = { 0.61f, 0.0f, 0.36f };
	float DarkMagentaRed[3] = { 0.61f, 0.0f, 0.22f };
	float DarkerRed[3] = { 0.47f, 0.0f, 0.0f };
	float DarkerROrange[3] = { 0.48f, 0.18f, 0.0f };
	float DarkerYellowOrange[3] = { 0.49f, 0.28f, 0.0f };
	float DarkerYellow[3] = { 0.50f, 0.48f, 0.0f };
	float DarkerPeaGreen[3] = { 0.25f, 0.4f, 0.09f };
	float DarkerYellowGreen[3] = { 0.0f, 0.36f, 0.12f };
	float DarkerGreen[3] = { 0.0f, 0.34f, 0.14f };
	float DarkerGreenCyan[3] = { 0.0f, 0.34f, 0.32f };
	float DarkerCyan[3] = { 0.0f, 0.35f, 0.49f };
	float DarkerCyanBlue[3] = { 0.0f, 0.21f, 0.38f };
	float DarkerBlue[3] = { 0.0f, 0.12f, 0.34f };
	float DarkerBlueViolet[3] = { 0.05f, 0.0f, 0.29f };
	float DarkerViolet[3] = { 0.19f, 0.0f, 0.29f };
	float DarkerVioletMagenta[3] = { 0.29f, 0.0f, 0.28f };
	float DarkerMagenta[3] = { 0.48f, 0.0f, 0.27f };
	float DarkerMagentaRed[3] = { 0.47f, 0.27f, 0.14f };
	float PaleCoolBrown[3] = { 0.78f, 0.69f, 0.61f };
	float LightCoolBrown[3] = { 0.6f, 0.52f, 0.45f };
	float MiumCoolBrown[3] = { 0.45f, 0.38f, 0.34f };
	float DarkCoolBrown[3] = { 0.32f, 0.27f, 0.25f };
	float DarkerCoolBrown[3] = { 0.21f, 0.18f, 0.17f };
	float PaleWarmBrown[3] = { 0.77f, 0.61f, 0.43f };
	float LightWarmBrown[3] = { 0.65f, 0.48f, 0.32f };
	float MiumWarmBrown[3] = { 0.54f, 0.38f, 0.22f };
	float DarkWarmBrown[3] = { 0.45f, 0.29f, 0.14f };
	float DarkerWarmBrown[3] = { 0.37f, 0.22f, 0.07f };
	float purple[3] = { 144.00f, 0.00f, 255.00f };
	float white[3] = { 255.00f, 255.00f, 255.00f };
	float Sexycyan[3] = { 0.00f, 229.00f, 250.00f };
	float DarkestRed[3] = { 84.00f, 6.00f, 00.00f };
} color;

bool IsVisible(uintptr_t mesh)
{
	float LastSumbitTime = driver.read<float>(mesh + OFFSETS::LastSubmitTime);
	float LastRenderTimeOnScreen = driver.read<float>(mesh + OFFSETS::LastRenderTimeOnScreen);

	bool Visible = LastRenderTimeOnScreen + 0.06f >= LastSumbitTime;

	return Visible;
}


ImU32 RGBtoU32(int r, int g, int b)
{
	float fr = (float)r / 255.0;
	float fg = (float)g / 255.0;
	float fb = (float)b / 255.0;
	return ImGui::GetColorU32(ImVec4(fr, fg, fb, 1));
}
auto RGB = 0;

double NRX;
double NRY;

void aimbot(float x, float y)
{
	float ScreenCenterX = (Width / 2);
	float ScreenCenterY = (Height / 2);
	int AimSpeed = smooth;
	float TargetX = 0;
	float TargetY = 0;

	if (x != 0)
	{
		if (x > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - x);
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (x < ScreenCenterX)
		{
			TargetX = x - ScreenCenterX;
			TargetX /= AimSpeed;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (y != 0)
	{
		if (y > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - y);
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (y < ScreenCenterY)
		{
			TargetY = y - ScreenCenterY;
			TargetY /= AimSpeed;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);
	return;
}
void AimAt(DWORD_PTR entity)
{
	uint64_t currentactormesh = driver.read<uint64_t>(entity + 0x318);
	auto rootHead = GetBoneWithRotation(currentactormesh, hitbox);
	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead);
	//updated by jeremylovecats
	if (rootHeadOut.y != 0 || rootHeadOut.y != 0)
	{
		aimbot(rootHeadOut.x, rootHeadOut.y);
	}
}


static auto DrawCircleFilled(int x, int y, int radius, RGBA* color) -> void
{
	ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}
namespace cumera
{
	Vector3 Location;
};


// skeleton
__forceinline auto skeleton(uintptr_t mesh) -> void
{
	const int color = ImColor(1.0f, 1.0f, 1.0f);


	Vector3 bonePositions[] = {
		GetBoneWithRotation(mesh, 109),  // HeadBone
		GetBoneWithRotation(mesh, 2),   // Hip
		GetBoneWithRotation(mesh, 66),  // Neck
		GetBoneWithRotation(mesh, 9),   // UpperArmLeft
		GetBoneWithRotation(mesh, 38),  // UpperArmRight
		GetBoneWithRotation(mesh, 10),  // LeftHand
		GetBoneWithRotation(mesh, 39),  // RightHand
		GetBoneWithRotation(mesh, 11),  // LeftHand1
		GetBoneWithRotation(mesh, 40),  // RightHand1
		GetBoneWithRotation(mesh, 78),  // RightThigh
		GetBoneWithRotation(mesh, 71),  // LeftThigh
		GetBoneWithRotation(mesh, 79),  // RightCalf
		GetBoneWithRotation(mesh, 72),  // LeftCalf
		GetBoneWithRotation(mesh, 75),  // LeftFoot         
		GetBoneWithRotation(mesh, 82),   // RightFoot     


	};

	Vector3 bonePositionsOut[16];
	for (int i = 0; i < 16; ++i) {
		bonePositionsOut[i] = ProjectWorldToScreen(bonePositions[i]);
	}

	if (IsVisible(mesh)) {

		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[1].x, bonePositionsOut[1].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[3].x, bonePositionsOut[3].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[4].x, bonePositionsOut[4].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[5].x, bonePositionsOut[5].y), ImVec2(bonePositionsOut[3].x, bonePositionsOut[3].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[6].x, bonePositionsOut[6].y), ImVec2(bonePositionsOut[4].x, bonePositionsOut[4].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[5].x, bonePositionsOut[5].y), ImVec2(bonePositionsOut[7].x, bonePositionsOut[7].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[6].x, bonePositionsOut[6].y), ImVec2(bonePositionsOut[8].x, bonePositionsOut[8].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[10].x, bonePositionsOut[10].y), ImVec2(bonePositionsOut[1].x, bonePositionsOut[1].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[9].x, bonePositionsOut[9].y), ImVec2(bonePositionsOut[1].x, bonePositionsOut[1].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[12].x, bonePositionsOut[12].y), ImVec2(bonePositionsOut[10].x, bonePositionsOut[10].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[11].x, bonePositionsOut[11].y), ImVec2(bonePositionsOut[9].x, bonePositionsOut[9].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[13].x, bonePositionsOut[13].y), ImVec2(bonePositionsOut[12].x, bonePositionsOut[12].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[14].x, bonePositionsOut[14].y), ImVec2(bonePositionsOut[11].x, bonePositionsOut[11].y), RGBtoU32(PlayerColor::SkeletonVisible[0] * 255, PlayerColor::SkeletonVisible[1] * 255, PlayerColor::SkeletonVisible[2] * 255), Skeleton_Thickness);
	}


	if (!IsVisible(mesh)) {

		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[1].x, bonePositionsOut[1].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[3].x, bonePositionsOut[3].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[4].x, bonePositionsOut[4].y), ImVec2(bonePositionsOut[2].x, bonePositionsOut[2].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[5].x, bonePositionsOut[5].y), ImVec2(bonePositionsOut[3].x, bonePositionsOut[3].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[6].x, bonePositionsOut[6].y), ImVec2(bonePositionsOut[4].x, bonePositionsOut[4].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[5].x, bonePositionsOut[5].y), ImVec2(bonePositionsOut[7].x, bonePositionsOut[7].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[6].x, bonePositionsOut[6].y), ImVec2(bonePositionsOut[8].x, bonePositionsOut[8].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[10].x, bonePositionsOut[10].y), ImVec2(bonePositionsOut[1].x, bonePositionsOut[1].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[9].x, bonePositionsOut[9].y), ImVec2(bonePositionsOut[1].x, bonePositionsOut[1].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[12].x, bonePositionsOut[12].y), ImVec2(bonePositionsOut[10].x, bonePositionsOut[10].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[11].x, bonePositionsOut[11].y), ImVec2(bonePositionsOut[9].x, bonePositionsOut[9].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[13].x, bonePositionsOut[13].y), ImVec2(bonePositionsOut[12].x, bonePositionsOut[12].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(bonePositionsOut[14].x, bonePositionsOut[14].y), ImVec2(bonePositionsOut[11].x, bonePositionsOut[11].y), RGBtoU32(PlayerColor::SkeletonNotVisible[0] * 255, PlayerColor::SkeletonNotVisible[1] * 255, PlayerColor::SkeletonNotVisible[2] * 255), Skeleton_Thickness);
	}

}


// USERNAME 

__forceinline std::string username(__int64 player)
{

	__int64 Name = driver.read<__int64>(player + 0xAF0);
	if (!Name) return std::string("BOT / AI");
	int Length = driver.read<int>(Name + 16i64);
	__int64 v6 = Length;
	if (!v6) return std::string("BOT / AI");
	uintptr_t Data = driver.read<__int64>(Name + 8);
	if (!Data) return std::string("BOT / AI");
	wchar_t* NameBuffer = new wchar_t[Length];
	driver.readm((uintptr_t*)Data, (uint8_t*)(NameBuffer), (Length * 2));

	char v21; // al
	int v22; // r8d
	int i; // ecx
	int v25; // eax
	_WORD* v23;

	v21 = v6 - 1;
	if (!(_DWORD)v6)
		v21 = 0;
	v22 = 0;
	v23 = (_WORD*)NameBuffer;
	for (i = (v21) & 3; ; *v23++ += i & 7)
	{
		v25 = v6 - 1;
		if (!(_DWORD)v6)
			v25 = 0;
		if (v22 >= v25)
			break;
		i += 3;
		++v22;
	}

	std::wstring Temp{ NameBuffer };
	delete[] NameBuffer;
	return std::string(Temp.begin(), Temp.end());


}








double GetCrossDistance(double x1, double y1, double x2, double y2) {
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}


void OutlinedText(int x, int y, ImColor Color, const char* text)
{
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 1, y), ImColor(0, 0, 0), text);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImColor(0, 0, 0), text);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImColor(0, 0, 0), text);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImColor(0, 0, 0), text);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), Color, text);
}
inline char* wchar_to_char(const wchar_t* pwchar)
{
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}

void DrawESP()
{
	static const auto size = ImGui::GetIO().DisplaySize;
	static const auto center = ImVec2(size.x / 2, size.y / 2);



	// Fov Circles



	if (fovcircle)
	{
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), float(AimFOV), ImColor(0, 96, 192, 255), 100.0f, 1.5f);
	}






	if (showFps)
	{
		char dist[256];
		sprintf_s(dist, "posted for free niggers | FPS %.f\n", ImGui::GetIO().Framerate);
		ImGui::GetOverlayDrawList()->AddText(ImVec2(15, 15), ImGui::GetColorU32({ color.DarkRed[0], color.DarkRed[1], color.DarkRed[2], 4.0f }), dist);
	}






	if (Filled_Fov) {
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(ScreenCenterX, ScreenCenterY), float(AimFOV), ImColor(255, 255, 0, 255), 100.0f, 1.5f);
		ImGui::GetOverlayDrawList()->AddCircleFilled(center, AimFOV, ImColor(0, 0, 0, 110), 100);
	}




	// hitboxs

	if (aimbone == 0)
	{
		hitbox = 109; // head
	}
	else if (aimbone == 1)
	{
		hitbox = 66; // neck
	}
	else if (aimbone == 2)
	{
		hitbox = 37; // chest
	}
	else if (aimbone == 3)
	{
		hitbox = 2; // pelvis
	}
	else if (aimbone == 4)






		// aimkeys

		if (aimkeypos == 0)
		{
			aimkey = 0x01;//left mouse button
		}
		else if (aimkeypos == 1)
		{
			aimkey = 0x02;//right mouse button
		}
		else if (aimkeypos == 2)
		{
			aimkey = 0x04;//middle mouse button
		}
		else if (aimkeypos == 3)
		{
			aimkey = 0x05;//x1 mouse button
		}
		else if (aimkeypos == 4)
		{
			aimkey = 0x06;//x2 mouse button
		}
		else if (aimkeypos == 5)
		{
			aimkey = 0x03;//control break processing
		}
		else if (aimkeypos == 6)
		{
			aimkey = 0x08;//backspace
		}
		else if (aimkeypos == 7)
		{
			aimkey = 0x09;//tab
		}
		else if (aimkeypos == 8)
		{
			aimkey = 0x0c;//clear
		}
		else if (aimkeypos == 9)
		{
			aimkey == 0x0D;//enter
		}
		else if (aimkeypos == 10)
		{
			aimkey = 0x10;//shift
		}
		else if (aimkeypos == 11)
		{
			aimkey = 0x11;//ctrl
		}
		else if (aimkeypos == 12)
		{
			aimkey == 0x12;//alt
		}
		else if (aimkeypos == 13)
		{
			aimkey == 0x14;//caps lock
		}
		else if (aimkeypos == 14)
		{
			aimkey == 0x1B;//esc
		}
		else if (aimkeypos == 15)
		{
			aimkey == 0x20;//space
		}
		else if (aimkeypos == 16)
		{
			aimkey == 0x30;//0
		}
		else if (aimkeypos == 17)
		{
			aimkey == 0x31;//1
		}
		else if (aimkeypos == 18)
		{
			aimkey == 0x32;//2
		}
		else if (aimkeypos == 19)
		{
			aimkey == 0x33;//3
		}
		else if (aimkeypos == 20)
		{
			aimkey == 0x34;//4
		}
		else if (aimkeypos == 21)
		{
			aimkey == 0x35;//5
		}
		else if (aimkeypos == 22)
		{
			aimkey == 0x36;//6
		}
		else if (aimkeypos == 23)
		{
			aimkey == 0x37;//7
		}
		else if (aimkeypos == 24)
		{
			aimkey == 0x38;//8
		}
		else if (aimkeypos == 25)
		{
			aimkey == 0x39;//9
		}
		else if (aimkeypos == 26)
		{
			aimkey == 0x41;//a
		}
		else if (aimkeypos == 27)
		{
			aimkey == 0x42;//b
		}
		else if (aimkeypos == 28)
		{
			aimkey == 0x43;//c
		}
		else if (aimkeypos == 29)
		{
			aimkey == 0x44;//d
		}
		else if (aimkeypos == 30)
		{
			aimkey == 0x45;//e
		}
		else if (aimkeypos == 31)
		{
			aimkey == 0x46;//f
		}
		else if (aimkeypos == 32)
		{
			aimkey == 0x47;//g
		}
		else if (aimkeypos == 33)
		{
			aimkey == 0x48;//h
		}
		else if (aimkeypos == 34)
		{
			aimkey == 0x49;//i
		}
		else if (aimkeypos == 35)
		{
			aimkey == 0x4A;//j
		}
		else if (aimkeypos == 36)
		{
			aimkey == 0x4B;//k
		}
		else if (aimkeypos == 37)
		{
			aimkey == 0x4C;//L
		}
		else if (aimkeypos == 38)
		{
			aimkey == 0x4D;//m
		}
		else if (aimkeypos == 39)
		{
			aimkey == 0x4E;//n
		}
		else if (aimkeypos == 40)
		{
			aimkey == 0x4F;//o
		}
		else if (aimkeypos == 41)
		{
			aimkey == 0x50;//p
		}
		else if (aimkeypos == 42)
		{
			aimkey == 0x51;//q
		}
		else if (aimkeypos == 43)
		{
			aimkey == 0x52;//r
		}
		else if (aimkeypos == 44)
		{
			aimkey == 0x53;//s
		}
		else if (aimkeypos == 45)
		{
			aimkey == 0x54;//t
		}
		else if (aimkeypos == 46)
		{
			aimkey == 0x55;//u
		}
		else if (aimkeypos == 47)
		{
			aimkey == 0x56;//v
		}
		else if (aimkeypos == 48)
		{
			aimkey == 0x57;//w
		}
		else if (aimkeypos == 49)
		{
			aimkey == 0x58;//x
		}
		else if (aimkeypos == 50)
		{
			aimkey == 0x59;//y
		}
		else if (aimkeypos == 51)
		{
			aimkey == 0x5A;//z
		}
		else if (aimkeypos == 52)
		{
			aimkey == 0x60;//numpad 0
		}
		else if (aimkeypos == 53)
		{
			aimkey == 0x61;//numpad 1
		}
		else if (aimkeypos == 54)
		{
			aimkey == 0x62;//numpad 2
		}
		else if (aimkeypos == 55)
		{
			aimkey == 0x63;//numpad 3
		}
		else if (aimkeypos == 56)
		{
			aimkey == 0x64;//numpad 4
		}
		else if (aimkeypos == 57)
		{
			aimkey == 0x65;//numpad 5
		}
		else if (aimkeypos == 58)
		{
			aimkey == 0x66;//numpad 6
		}
		else if (aimkeypos == 59)
		{
			aimkey == 0x67;//numpad 7
		}
		else if (aimkeypos == 60)
		{
			aimkey == 0x68;//numpad 8
		}
		else if (aimkeypos == 61)
		{
			aimkey == 0x69;//numpad 9
		}
		else if (aimkeypos == 62)
		{
			aimkey == 0x6A;//multiply
		}

	auto entityListCopy = entityList;
	float closestDistance = FLT_MAX;

	DWORD_PTR closestPawn = NULL;
	Uworld = driver.read<DWORD_PTR>(base_address + OFFSETS::UWORLD);
	DWORD_PTR Gameinstance = driver.read<DWORD_PTR>(Uworld + OFFSETS::Gameinstance);
	DWORD_PTR LocalPlayers = driver.read<DWORD_PTR>(Gameinstance + OFFSETS::LocalPlayers);
	Localplayer = driver.read<DWORD_PTR>(LocalPlayers);
	PlayerController = driver.read<DWORD_PTR>(Localplayer + OFFSETS::PlayerController);
	LocalPawn = driver.read<DWORD_PTR>(PlayerController + OFFSETS::LocalPawn);
	uintptr_t pcmc = driver.read<uint64_t>(PlayerController + 0x338);
	PlayerState = driver.read<DWORD_PTR>(LocalPawn + OFFSETS::PlayerState);
	DWORD_PTR PlayerCameraManager = driver.read<DWORD_PTR>(PlayerController + OFFSETS::Cameramanager);
	PlayerCameraManager = driver.read<DWORD_PTR>(LocalPawn + PlayerCameraManager);
	Rootcomp = driver.read<DWORD_PTR>(LocalPawn + OFFSETS::RootComponet);
	Persistentlevel = driver.read<DWORD_PTR>(Uworld + OFFSETS::PersistentLevel);
	uintptr_t Crrneytwep = driver.read<uintptr_t>(LocalPawn + 0x948);
	DWORD ActorCount = driver.read<DWORD>(Persistentlevel + OFFSETS::ActorCount);
	DWORD_PTR AOFFSETS = driver.read<DWORD_PTR>(Persistentlevel + OFFSETS::AActor);

	DWORD_PTR GameState = driver.read<DWORD_PTR>(Uworld + OFFSETS::GameState);//gamestate
	DWORD_PTR PlayerArray = driver.read<DWORD_PTR>(GameState + OFFSETS::PlayerArray);//playerarray
	uint64_t CurrentVehicle = driver.read<uint64_t>(LocalPawn + 0x2590); //FortPlayerPawn::CurrentVehicle

	bool InLobby;
	InLobby = false;
	if (LocalPawn) InLobby = true;
	int Num = driver.read<int>(GameState + (OFFSETS::PlayerArray + sizeof(uintptr_t))); //reads the total number of player states in this array

	for (uint32_t i = 0; i < Num; i++)
	{



		auto player = driver.read<uintptr_t>(PlayerArray + i * 0x8);
		auto CurrentActor = driver.read<uintptr_t>(player + OFFSETS::Private);//PawnPrivate

		if (!CurrentActor) {
			continue;
		}



		int NewPlayerLocationX;
		int NewPlayerLocationY;

		//uint64_t CurrentActor = read<uint64_t>(AOFFSETS + i * OFFSETS::CurrentActor);
	   // if (read<float>(CurrentActor + OFFSETS::Revivefromdbnotime) != 10) continue;
		uint64_t CurrentActorMesh = driver.read<uint64_t>(CurrentActor + OFFSETS::Mesh);
		int MyTeamId = driver.read<int>(PlayerState + OFFSETS::TeamId);
		DWORD64 otherPlayerState = driver.read<uint64_t>(CurrentActor + 0x290);
		int ActorTeamId = driver.read<int>(otherPlayerState + OFFSETS::TeamId);
		auto isDBNO = (driver.read<char>(CurrentActor + 0x7c2) >> 4) & 1;

		auto entityListCopy = entityList;
		if (MyTeamId == ActorTeamId) continue;
		if (LocalPawn)
		{
			VisDist;

		}
		else
		{
			VisDist = 2400;

		}
		if (slefESP)
		{
			continue;
		}
		else {
			if (CurrentActor == LocalPawn) continue;
		}
		if (CurrentActor == LocalPawn) continue;
		Vector3 Headpos = GetBoneWithRotation(CurrentActorMesh, 67);
		Vector3 head2 = GetBoneWithRotation(CurrentActorMesh, 68);
		Vector3 footpos = GetBoneWithRotation(CurrentActorMesh, 0);
		localactorpos = driver.read<Vector3>(Rootcomp + OFFSETS::LocalActorPos);
		auto normal_head = ProjectWorldToScreen(Headpos);
		float distance = localactorpos.Distance(Headpos) / ChangerFOV;

		Vector3 bone15 = GetBoneWithRotation(CurrentActorMesh, 50);
		Vector3 top = ProjectWorldToScreen(bone15);

		Vector3 bone0 = GetBoneWithRotation(CurrentActorMesh, 0);
		Vector3 bottom = ProjectWorldToScreen(bone0);
		Vector3 Headbox = ProjectWorldToScreen(Vector3(Headpos.x, Headpos.y, Headpos.z + 15));
		Vector3 HeadElvar = ProjectWorldToScreen(Vector3(Headpos.x, Headpos.y, Headpos.z));
		Vector3 FeetElvar = ProjectWorldToScreen(Vector3(footpos.x, footpos.y, footpos.z));
		//Vector3 Toebox = ProjectWorldToScreen(Vector3(Toepos.x, Toepos.y, Toepos.z + 15));
		Vector3 w2shead = ProjectWorldToScreen(Headpos);

		float BoxHeight = (float)(Headbox.y - bottom.y);
		float BoxWidth = BoxHeight * 0.50f;

		float LeftX = (float)Headbox.x - (BoxWidth / 1);
		float LeftY = (float)bottom.y;

		int center_x = GetSystemMetrics(0) / 2 - 3;
		int center_y = GetSystemMetrics(1) / 2 - 3;





		int dynamedfov = AimFOV - ((fovcircle - 80) * 2.5);
		float CornerHeight = abs(Headbox.y - bottom.y);
		float CornerWidth = CornerHeight * BoxWidthValue;
		if (distance < VisDist)
		{

			if (Esp)
			{

				if (BoxEsp) {

					float time = ImGui::GetTime();


					float frequency = 1.0f;
					float intensity = 128.0f;


					int red = static_cast<int>(128.0f + 128.0f * cos(frequency * time));
					int green = static_cast<int>(128.0f + 128.0f * cos(frequency * time + 2.0f));
					int blue = static_cast<int>(128.0f + 128.0f * cos(frequency * time + 4.0f));

					Normalbox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, RGBtoU32(VisColor[0] * 255, VisColor[1] * 255, VisColor[2] * 255), EspThickness, 0); // * 255), 3.5, 0);
				}





				// Esp_LineBottom bottom screen

				if (Esp_Line_Bottom)
				{

					if (IsVisible(CurrentActorMesh)) {
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height), ImVec2(bottom.x, bottom.y), ImColor(255, 255, 255), 1);
					}
					if (!IsVisible(CurrentActorMesh)) {
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height), ImVec2(bottom.x, bottom.y), ImColor(255, 0, 0), 1);
					}

				}



				// CrosshairLines

				if (Crosshair_Lines)
				{

					if (IsVisible(CurrentActorMesh)) {
						DrawLine(Width / 2, Height / 2, HeadElvar.x, HeadElvar.y, &Col.green, 2.5);
					}
					if (!IsVisible(CurrentActorMesh)) {
						DrawLine(Width / 2, Height / 2, HeadElvar.x, HeadElvar.y, &Col.red, 2.5);
					}

				}



				// Crosshair

				if (Crosshair)
				{
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height / 2), ImVec2(Width / 2 - 10, Height / 2), ImColor(255, 0, 0), 1.0f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height / 2), ImVec2(Width / 2 + 10, Height / 2), ImColor(255, 0, 0), 1.0f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height / 2), ImVec2(Width / 2, Height / 2 - 10), ImColor(255, 0, 0), 1.0f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(Width / 2, Height / 2), ImVec2(Width / 2, Height / 2 + 10), ImColor(255, 0, 0), 1.0f);
				}


				if (SkeletonEsp) // Skeleton Esp
				{
					skeleton(CurrentActorMesh);
				}



				// sell = gay

				if (Box_3D) // 3D BOX
				{
					if (HeadElvar.x != 0 || HeadElvar.y != 0 || HeadElvar.z != 0)
					{
						ImU32 ESPSkeleton;
						if (IsVisible(CurrentActorMesh))
						{
							ESPSkeleton = ImColor(0, 255, 0);
						}
						else if (!IsVisible(CurrentActorMesh))
						{
							ESPSkeleton = ImColor(255, 0, 0);
						}

						Vector3 bottom1 = ProjectWorldToScreen(Vector3(footpos.x + 40, footpos.y - 40, footpos.z));
						Vector3 bottom2 = ProjectWorldToScreen(Vector3(footpos.x - 40, footpos.y - 40, footpos.z));
						Vector3 bottom3 = ProjectWorldToScreen(Vector3(footpos.x - 40, footpos.y + 40, footpos.z));
						Vector3 bottom4 = ProjectWorldToScreen(Vector3(footpos.x + 40, footpos.y + 40, footpos.z));

						Vector3 top1 = ProjectWorldToScreen(Vector3(Headpos.x + 40, Headpos.y - 40, Headpos.z + 15));
						Vector3 top2 = ProjectWorldToScreen(Vector3(Headpos.x - 40, Headpos.y - 40, Headpos.z + 15));
						Vector3 top3 = ProjectWorldToScreen(Vector3(Headpos.x - 40, Headpos.y + 40, Headpos.z + 15));
						Vector3 top4 = ProjectWorldToScreen(Vector3(Headpos.x + 40, Headpos.y + 40, Headpos.z + 15));

						ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(top1.x, top1.y), ESPSkeleton, 2.0f);
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(top2.x, top2.y), ESPSkeleton, 2.0f);
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(top3.x, top3.y), ESPSkeleton, 2.0f);
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(top4.x, top4.y), ESPSkeleton, 2.0f);

						ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(bottom2.x, bottom2.y), ESPSkeleton, 2.0f);
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(bottom3.x, bottom3.y), ESPSkeleton, 2.0f);
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(bottom4.x, bottom4.y), ESPSkeleton, 2.0f);
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(bottom1.x, bottom1.y), ESPSkeleton, 2.0f);

						ImGui::GetOverlayDrawList()->AddLine(ImVec2(top1.x, top1.y), ImVec2(top2.x, top2.y), ESPSkeleton, 2.0f);
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(top2.x, top2.y), ImVec2(top3.x, top3.y), ESPSkeleton, 2.0f);
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(top3.x, top3.y), ImVec2(top4.x, top4.y), ESPSkeleton, 2.0f);
						ImGui::GetOverlayDrawList()->AddLine(ImVec2(top4.x, top4.y), ImVec2(top1.x, top1.y), ESPSkeleton, 2.0f);
					}
				}





				if (CorneredBoxEsp) // cornered esp/size
				{
					if (IsVisible(CurrentActorMesh)) {
						DrawCorneredBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, IM_COL32(255, 255, 255, 255), 2);
					}
					if (!IsVisible(CurrentActorMesh)) {
						DrawCorneredBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, IM_COL32(255, 0, 0, 255), 2);
					}

				}




				if (FilledBoxEsp)
				{

					DrawFilledRect(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, IM_COL32(100, 100, 100, 100));
				}


				if (StreamProof) {
					SetWindowDisplayAffinity(Window, WDA_EXCLUDEFROMCAPTURE);
				}
				else {
					SetWindowDisplayAffinity(Window, !WDA_EXCLUDEFROMCAPTURE);
				}








				if (nameEsp) // Name Esp
				{
					auto name = username((uintptr_t)player);
					DrawText1(bottom.x - 12, bottom.y + 13, name.c_str(), &Col.purple);
				}

				


				if (DistanceEsp)
				{
					char name[64];
					sprintf_s(name, "%2.fm", distance);
					DrawString(16, Headbox.x, Headbox.y - 15, &Col.yellow, true, true, name);
				}





				if (VisibleCheck) {
					float LastSumbitTime = driver.read<float>(OFFSETS::Mesh + OFFSETS::LastSubmitTime);
					float LastRenderTimeOnScreen = driver.read<float>(OFFSETS::Mesh + OFFSETS::LastRenderTimeOnScreen);

					bool Visible = LastRenderTimeOnScreen + 0.06f >= LastSumbitTime;
				}



			}
		}


		auto dx = w2shead.x - (Width / 2);
		auto dy = w2shead.y - (Height / 2);
		auto dist = sqrtf(dx * dx + dy * dy);


		if (IsVisible(CurrentActorMesh) || !VisibleCheck) {

			if (dist < AimFOV && dist < closestDistance) {
				closestDistance = dist;
				closestPawn = CurrentActor;
			}
		}

	}


	





	// EXPLOITS




	if (spinbot) {

		auto Mesh = driver.read<uint64_t>(LocalPawn + 0x318);
		static auto Cached = driver.read<Vector3>(Mesh + 0x138);
		driver.write<Vector3>(Mesh + 0x138, Vector3(1, rand() % 361, 1));

	}



}
IDirect3DTexture9* aimTexture;
IDirect3DTexture9* visualsTexture;
IDirect3DTexture9* exploitsTexture;
IDirect3DTexture9* customizeTexture;
IDirect3DTexture9* skullTexture;


// menu

ImDrawList* draw;

void render()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT))
	{
		ShowMenu = !ShowMenu;
		Sleep(100);
	}

	static int maintabs;
	static ImVec2 pos(180, 100);
	ImGui::PushFont(DefaultFont);


	ImGui::SetNextWindowSize({ 300, 330 }); // size meniu
	XorS(box_esp, "Box");
	XorS(snapline, "Snapline");
	XorS(es5, "Max Visuals Distance");
	XorS(aim1, "Aimbot");
	XorS(aim2, "Aimbot Key");
	XorS(aim3, "Aimbot Bone");
	XorS(aim6, "Aimbot FOV");
	XorS(smoothh, "Smooth");

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.5f, 12.5f)); // Add padding to the window content

	if (ShowMenu)
	{
		ImGui::Begin("latter post free niggers", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		ImGui::SetCursorPos({ 22,31.f });
		if (ImGui::Button("Aimbot", { 125,30 }))
		{
			maintabs = 0;
		}
		ImGui::SetCursorPos({ 164,31.f });
		if (ImGui::Button("Visuals", { 125,30 }))
		{
			maintabs = 1;
		}
		ImGui::SetCursorPos({ 308,31.f });
		if (ImGui::Button("Misc", { 125,30 }))
		{
			maintabs = 2;
		}
		ImGui::SetCursorPos({ 456,31.f });
		if (ImGui::Button("Exploits", { 125,30 }))
		{
			maintabs = 3;
		}

		if (maintabs == 0)
		{
			ImGui::Text(" Aimbot");
			ImGui::Text("");


			ImGui::Spacing();
			ImGui::Checkbox("Enable Aimbot", &Aimbot);

			ImGui::SameLine();
			ImGui::Checkbox("Visible Check", &VisibleCheck); // work

			ImGui::SameLine();
			ImGui::Checkbox("Circle FOV", &fovcircle); // work

			ImGui::SameLine();
			ImGui::Checkbox("Filled Fov", &Filled_Fov); // work

			ImGui::SameLine(500);
			if (ImGui::Button("Unload"))
				exit(0);




			ImGui::Spacing();
			//HotkeyButton(hotkeys::aimkey, ChangeKey, keystatus);
			ImGui::Spacing();

			ImGui::Spacing();
			ImGui::Text("Aim Settings:"); // work
			ImGui::Spacing();

			ImGui::Spacing();
			ImGui::SliderFloat("Aimbot Smoothness", &smooth, 1.0f, 30.0f); // work
			ImGui::Spacing();

			ImGui::Spacing();
			ImGui::SliderFloat("Aim FOV", &AimFOV, 50.f, 800.f); // work
			ImGui::Spacing();


		}

		if (maintabs == 1)
		{
			// boxs


			ImGui::Text("");


			ImGui::Spacing();
			ImGui::Checkbox("Enable ESP", &Esp); // work

			ImGui::SameLine(200.200);
			ImGui::Checkbox("Distance", &DistanceEsp); // work


			ImGui::Spacing();
			ImGui::Checkbox("Box", &BoxEsp); // work


			ImGui::SameLine(200.200);
			ImGui::Checkbox("Crosshair", &Crosshair); // work


			ImGui::Spacing();
			ImGui::Checkbox("3D Box", &Box_3D); // work


			ImGui::SameLine(200.200);
			ImGui::Checkbox("Esp Line Top / removed", &Esp_Line_Top); // work


			ImGui::Spacing();
			ImGui::Checkbox("Cornered Boxes", &CorneredBoxEsp); // work


			ImGui::SameLine(200.200);
			ImGui::Checkbox("Esp Line Bottom", &Esp_Line_Bottom); // work 


			ImGui::Spacing();
			ImGui::Checkbox("Filled Boxes", &FilledBoxEsp); // work


			ImGui::SameLine(200.200);
			ImGui::Checkbox("Target Line", &Crosshair_Lines); // work


			ImGui::Spacing();
			ImGui::Checkbox("nameEsp", &nameEsp); // work


			ImGui::SameLine(200.200);
			ImGui::Checkbox("Skeleton ", &SkeletonEsp); // work

			ImGui::SliderFloat(("Skeleton Thickness"), &Skeleton_Thickness, 0.f, 5);
			ImGui::ColorEdit3(TEXT("Skeleton Visible "), PlayerColor::SkeletonVisible);
			ImGui::ColorEdit3(TEXT("Skeleton Not Visible "), PlayerColor::SkeletonNotVisible);

		}

		if (maintabs == 2)
		{
			ImGui::Spacing();

			ImGui::Text("");
			ImGui::Spacing();
			//	ImGui::Text("Custom Options");
			ImGui::Spacing();
			//ImGui::Checkbox("Stream-proof", &StreamProof);
			ImGui::Spacing();
			ImGui::Checkbox(E("Show FPS"), &showFps);
			ImGui::Spacing();

			ImGui::Checkbox(("Stream Proof"), &StreamProof);



			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::SliderFloat(E("ESP Thickness"), &EspThickness, 1, 8);
			ImGui::Text("");
			ImGui::Spacing();
			// if (ImGui::Button("Unload"))
				// exit(0);
		}

		if (maintabs == 3)
		{
			ImGui::Spacing();
			ImGui::Text("Exploits Dont sell");
			ImGui::Text("");
			ImGui::Spacing();

			ImGui::Spacing();
			ImGui::Checkbox("Spinbot", &spinbot);
			ImGui::Spacing();


		}


		ImGui::End();
	}

	DrawESP();

	ImGui::EndFrame();
	D3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	D3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (D3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		D3dDevice->EndScene();
	}
	HRESULT result = D3dDevice->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && D3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		D3dDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

MSG Message = { NULL };
int Loop = 0;
void xMainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, Window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();

		if (hwnd_active == hwnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(Window, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(hwnd, &rc);
		ClientToScreen(hwnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = NULL;
		io.ImeWindowHandle = hwnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			d3dpp.BackBufferWidth = Width;
			d3dpp.BackBufferHeight = Height;
			SetWindowPos(Window, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			D3dDevice->Reset(&d3dpp);
		}
		render();
		if (Loop == 0) {
			XorS(base, "Process base address: %p.\n");
			//printf(base.decrypt(), (void*)base_address);
		}
		Loop = Loop + 1;
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow(Window);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		xShutdown();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (D3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			d3dpp.BackBufferWidth = LOWORD(lParam);
			d3dpp.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = D3dDevice->Reset(&d3dpp);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void xShutdown()
{
	TriBuf->Release();
	D3dDevice->Release();
	p_Object->Release();

	DestroyWindow(Window);
	UnregisterClass("Bluestacks", NULL);
}
