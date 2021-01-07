#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include <graphics/sprite.h>
#include <input/input_manager.h>
#include <vector>
#include "load_json.h"


#include "texture_atlas.h"
#include "Armature.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
}

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
    
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::InputManager* input_manager_;

	gef::Texture* sprite_texture_;
	gef::Sprite sprite_;

	TextureAtlas* texture_atlas_;
	Armature* armature_;

	int frame;
	int frame_counter;
	float fps_;

	char* json_file_;
	
	gef::Vector4 sprite_pos_;
};

#endif // _SCENE_APP_H
