#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <graphics/mesh.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <graphics/sprite.h>
#include "load_texture.h"
#include "load_json.h"

#include "../rapidjson/include/rapidjson/document.h"

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	input_manager_(NULL),
	font_(NULL),
	sprite_texture_(NULL),
	json_file_(NULL)
{
}

void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	InitFont();

	// initialise input manager
	input_manager_ = gef::InputManager::Create(platform_);

	//char* tex_json = LoadJSON("boy-attack_tex.json");
	//char* ske_json = LoadJSON("boy-attack_ske.json");

	char* tex_json = LoadJSON("Dragon_tex.json");
	char* ske_json = LoadJSON("Dragon_ske.json");

	rapidjson::Document tex_document;
	tex_document.Parse(tex_json);

	rapidjson::Document ske_document;
	ske_document.Parse(ske_json);

	texture_atlas_ = TextureAtlas::ReadTextureAtlasFromJSON(tex_document, platform_);
	std::string image_filename = tex_document["imagePath"].GetString();

	armature_ = Armature::ReadArmatureFromJSON(ske_document);

	free(tex_json);
	free(ske_json);

	frame = 0;
	frame_counter = 0;

	sprite_texture_ = CreateTextureFromPNG(image_filename.data(), platform_);

	sprite_.set_texture(sprite_texture_);
}

void SceneApp::CleanUp()
{
	delete sprite_texture_;
	sprite_texture_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

	CleanUpFont();

	delete sprite_renderer_;
	sprite_renderer_ = NULL;
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	/*if (frame >= (int)armature_->animation->displayFrame.size())
	{
		frame = 0;
	}*/

	//int currentFrame = armature_->animation->displayFrame[frame];

	//TextureAtlas::Subtexture currentSubText = TextureAtlas::GetSubtextureFromName(armature_->skin->name[currentFrame], texture_atlas_);

	//TextureAtlas::GetSpritePositionFromSubtexture(sprite_, currentSubText, sprite_pos_.x, sprite_pos_.y);
	//TextureAtlas::GetSpriteSizeFromSubtexture(sprite_, currentSubText);


	input_manager_->Update();

	/*if (frame_counter < (int)armature_->animation->duration)
	{
		frame_counter++;
	}
	else 
	{
		frame_counter = 0;
		frame++;
	}*/

	return true;
}




void SceneApp::Render()
{
	sprite_renderer_->Begin();

	// Render button icon
	//sprite_renderer_->DrawSprite(sprite_);

	for (int i = 0; i < armature_->slots.size(); i++)
	{
		Armature::SkinSlot currentSkinSlot = Armature::GetSkinSlotFromName(armature_->slots[i].name, &armature_->skin);
		Armature::Bone currentBone = Armature::GetBoneFromName(armature_->slots[i].parent, armature_);
		TextureAtlas::Subtexture currentSubTexture = TextureAtlas::GetSubtextureFromName(currentSkinSlot.displayName, texture_atlas_);
		
		TextureAtlas::GetSpriteTextureFromSubtexture(sprite_, currentSubTexture, texture_atlas_);

		gef::Matrix33 finalTransform;
		finalTransform.SetIdentity();
		
		finalTransform = currentSubTexture.sub_tex_transform * currentSkinSlot.spriteOffsetTransform * currentBone.worldTransform * texture_atlas_->transform;

		sprite_renderer_->DrawSprite(sprite_, finalTransform);
	}

	DrawHUD();
	sprite_renderer_->End();
}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

