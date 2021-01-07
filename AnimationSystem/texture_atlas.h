#ifndef _TEXTURE_ATLAS_H
#define _TEXTURE_ATLAS_H

#include <../rapidjson/include/rapidjson/document.h>
#include <vector>
#include <graphics\sprite.h>
#include <graphics\texture.h>
#include <maths/matrix33.h>

class TextureAtlas 
{

public:
	struct Subtexture
	{
		std::string name;
		float x;
		float y;
		float width;
		float height;
		float frameY;
		float frameX;
		float frameWidth;
		float frameHeight;
		gef::Matrix33 sub_tex_transform;
	};

	TextureAtlas();

	static TextureAtlas* ReadTextureAtlasFromJSON(rapidjson::Document& tex_document, gef::Platform& platform_);
	static Subtexture* ReadSubTextureFromJSON(const rapidjson::Value& subtex_value);
	static void GetSpriteSizeFromSubtexture(gef::Sprite &sprite, Subtexture &subtexture);
	static void GetSpritePositionFromSubtexture(gef::Sprite &sprite, const Subtexture &subtexture, float x, float y);
	static void GetSpriteTextureFromSubtexture(gef::Sprite &sprite, const Subtexture &subtexture, TextureAtlas* textureAtlas);
	static Subtexture GetSubtextureFromName(std::string name, TextureAtlas* textureAtlas);
	std::vector<Subtexture> sub_textures;
	
	gef::Matrix33 transform;

protected:
	std::string name;
	float width;
	float height;
	



	


};

#endif