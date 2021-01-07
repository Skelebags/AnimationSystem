#include "Armature.h"

Armature::Armature()
{
}

Armature * Armature::ReadArmatureFromJSON(const rapidjson::Document & arm_document)
{
	Armature* armature = new Armature();

	armature->frameRate = arm_document["armature"][0]["frameRate"].GetFloat();
	armature->name = arm_document["armature"][0]["name"].GetString();

	const rapidjson::Value& bone_val = arm_document["armature"][0]["bone"];
	for (int bone_num = 0; bone_num < (int)bone_val.Size(); bone_num++)
	{
		Bone* bone = ReadBoneFromJSON(bone_val[bone_num], armature);
		armature->bones.push_back(*bone);
		delete bone;
	}

	const rapidjson::Value& slot_val = arm_document["armature"][0]["slot"];
	for (int slot_num = 0; slot_num < (int)slot_val.Size(); slot_num++)
	{
		Slot* slot = ReadSlotFromJSON(slot_val[slot_num]);
		armature->slots.push_back(*slot);
		delete slot;
	}

	const rapidjson::Value& skin_val = arm_document["armature"][0]["skin"][0];
	armature->skin = *ReadSkinFromJSON(skin_val);

	const rapidjson::Value& anim_val = arm_document["armature"][0]["animation"];
	for (int anim_num = 0; anim_num < (int)anim_val.Size(); ++anim_num)
	{
		Animation* animation = ReadAnimationFromJSON(anim_val[anim_num]);
		armature->animations.push_back(*animation);
		delete animation;
	}

	return armature;
}

Armature::Bone * Armature::ReadBoneFromJSON(const rapidjson::Value & bone_value, Armature* armature)
{
	Bone* bone = new Bone();

	if (bone_value.HasMember("name"))
		bone->name = bone_value["name"].GetString();
	else
		bone->name = "";

	if (bone_value.HasMember("parent"))
		bone->parent = bone_value["parent"].GetString();
	else
		bone->parent = "";

	if (bone_value.HasMember("transform"))
	{
		gef::Matrix33 translationMatrix;
		gef::Matrix33 rotationMatrix;
		translationMatrix.SetIdentity();
		translationMatrix.SetTranslation(gef::Vector2(bone_value["transform"]["x"].GetFloat(), bone_value["transform"]["y"].GetFloat()));

		float rotRadians = (bone_value["transform"]["skY"].GetFloat() * M_PI) / 180;

		rotationMatrix.Rotate(rotRadians);

		bone->transform = rotationMatrix * translationMatrix;

		bone->worldTransform = GetWorldBoneTransform(bone, armature);
	} 
	else
	{
		bone->transform.SetIdentity();
		bone->worldTransform.SetIdentity();
	}

	return bone;
}

Armature::Slot * Armature::ReadSlotFromJSON(const rapidjson::Value & slot_value)
{
	Slot* slot = new Slot();

	if (slot_value.HasMember("name"))
		slot->name = slot_value["name"].GetString();
	else
		slot->name = "";

	if (slot_value.HasMember("parent"))
		slot->parent = slot_value["parent"].GetString();
	else
		slot->parent = "";

	return slot;
}

Armature::Skin * Armature::ReadSkinFromJSON(const rapidjson::Value & skin_value)
{
	Skin* skin = new Skin();

	
	if (skin_value.HasMember("slot"))
	{
		for (int slot_num = 0; slot_num < skin_value["slot"].Size(); slot_num++)
		{
			SkinSlot* skinslot = new SkinSlot();
			skinslot = ReadSkinSlotFromJSON(skin_value["slot"][slot_num]);
			skin->skinSlots.push_back(*skinslot);
			delete skinslot;
		}
	}
	
	return skin;
}

Armature::SkinSlot * Armature::ReadSkinSlotFromJSON(const rapidjson::Value & skinslot_value)
{
	SkinSlot* skinslot = new SkinSlot();

	if (skinslot_value.HasMember("name"))
		skinslot->name = skinslot_value["name"].GetString();
	else
		skinslot->name = "";

	if (skinslot_value.HasMember("display"))
	{
		if (skinslot_value["display"][0].HasMember("name"))
		{
			skinslot->displayName = skinslot_value["display"][0]["name"].GetString();
		}
		else
		{
			skinslot->displayName = "";
		}
		gef::Matrix33 translationMatrix;
		gef::Matrix33 rotationMatrix;
		translationMatrix.SetIdentity();
		rotationMatrix.SetIdentity();

		if (skinslot_value["display"][0].HasMember("transform"))
		{
			
			if (skinslot_value["display"][0]["transform"].HasMember("x") && skinslot_value["display"][0]["transform"].HasMember("y"))
			{
				float spriteX = skinslot_value["display"][0]["transform"]["x"].GetFloat();
				float spriteY = skinslot_value["display"][0]["transform"]["y"].GetFloat();

				translationMatrix.SetTranslation(gef::Vector2(spriteX, spriteY));
			}
			if (skinslot_value["display"][0]["transform"].HasMember("skY"))
			{
				float rotRadians = (skinslot_value["display"][0]["transform"]["skY"].GetFloat() * M_PI) / 180;
				rotationMatrix.Rotate(rotRadians);
			}
			
		}

		skinslot->spriteOffsetTransform = rotationMatrix * translationMatrix;
	}
	else
	{
		skinslot->displayName = "";
		skinslot->spriteOffsetTransform.SetIdentity();
	}


	return skinslot;
}

Armature::Animation * Armature::ReadAnimationFromJSON(const rapidjson::Value & anim_value)
{
	Animation* animation = new Animation();

	animation->duration = anim_value["duration"].GetFloat();
	animation->name = anim_value["name"].GetString();

	if (anim_value.HasMember("slot"))
	{
		for (int slot_num = 0; slot_num < (int)anim_value["slot"].Size(); slot_num++)
		{
			for (int frame_num = 0; frame_num < (int)anim_value["slot"][slot_num]["displayFrame"].Size(); frame_num++)
			{
				if (anim_value["slot"][slot_num]["displayFrame"][frame_num].HasMember("value"))
				{
					animation->displayFrame.push_back(anim_value["slot"][slot_num]["displayFrame"][frame_num]["value"].GetInt());
				}
			}
		}
	}

	//if(anim_value.HasMember(""))

	return animation;
}

gef::Matrix33 Armature::GetWorldBoneTransform(Bone* bone, Armature* armature)
{
	gef::Matrix33 worldBoneTransform;
	worldBoneTransform.SetIdentity();

	worldBoneTransform = bone->transform;

	if (bone->parent != "")
	{
		worldBoneTransform = worldBoneTransform * GetBoneFromName(bone->parent, armature).worldTransform;
	}

	return worldBoneTransform;
}

Armature::Bone Armature::GetBoneFromName(std::string name, Armature* armature)
{
	Bone bone;

	for (int bone_num = 0; bone_num < armature->bones.size(); bone_num++)
	{
		if (armature->bones[bone_num].name == name)
		{
			bone = armature->bones[bone_num];
		}
	}

	return bone;
}

Armature::SkinSlot Armature::GetSkinSlotFromName(std::string name, Skin * skin)
{
	SkinSlot skinSlot;

	for (int slot_num = 0; slot_num < skin->skinSlots.size(); slot_num++)
	{
		if (skin->skinSlots[slot_num].name == name)
		{
			skinSlot = skin->skinSlots[slot_num];
		}
	}

	return skinSlot;
}
