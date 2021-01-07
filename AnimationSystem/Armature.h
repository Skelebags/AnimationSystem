#pragma once

#include <../rapidjson/include/rapidjson/document.h>
#include <vector>
#include <graphics\sprite.h>
#include <graphics\texture.h>
#include <maths/matrix33.h>

#define M_PI   3.14159265358979323846264338327950288

class Armature
{

public:
	struct Animation
	{
		float duration;
		std::string name;
		std::vector<int> displayFrame;
	};

	struct Bone
	{
		std::string name;
		std::string parent;
		gef::Matrix33 transform;
		gef::Matrix33 worldTransform;
	};

	struct Slot
	{
		std::string name;
		std::string parent;
	};

	struct SkinSlot
	{
		std::string name;
		std::string displayName;
		gef::Matrix33 spriteOffsetTransform;
	};

	struct Skin
	{
		std::vector<SkinSlot> skinSlots;
	};

	Armature();
	~Armature();

	static Armature* ReadArmatureFromJSON(const rapidjson::Document & arm_document);

	static Bone* ReadBoneFromJSON(const rapidjson::Value & bone_value, Armature * armature);
	static Slot* ReadSlotFromJSON(const rapidjson::Value & slot_value);
	static Skin* ReadSkinFromJSON(const rapidjson::Value & skin_value);
	static SkinSlot* ReadSkinSlotFromJSON(const rapidjson::Value & skinslot_value);
	static Animation* ReadAnimationFromJSON(const rapidjson::Value & anim_value);

	static gef::Matrix33 GetWorldBoneTransform(Bone* bone, Armature* armature);
	static Bone GetBoneFromName(std::string name, Armature* armature);
	static SkinSlot GetSkinSlotFromName(std::string name, Skin* skin);

	float frameRate;
	std::string name;

	std::vector<Bone> bones;
	std::vector<Slot> slots;
	Skin skin;
	std::vector<Animation> animations;
};
