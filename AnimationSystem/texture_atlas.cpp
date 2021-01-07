#include "texture_atlas.h"
#include "load_texture.h"

TextureAtlas::TextureAtlas()
{
}

TextureAtlas * TextureAtlas::ReadTextureAtlasFromJSON(rapidjson::Document & tex_document, gef::Platform& platform_)
{
	TextureAtlas* texture_atlas = new TextureAtlas();
	texture_atlas->name = tex_document["name"].GetString();
	texture_atlas->width = tex_document["width"].GetFloat();
	texture_atlas->height = tex_document["height"].GetFloat();

	gef::Matrix33 scaleMatrix;
	gef::Matrix33 translationMatrix;

	scaleMatrix.Scale(gef::Vector2(0.25f, 0.25f));
	translationMatrix.SetIdentity();
	translationMatrix.SetTranslation(gef::Vector2(platform_.width() * 0.5f, platform_.height() * 0.5f));

	texture_atlas->transform = scaleMatrix * translationMatrix;

	const rapidjson::Value& subtexture_array = tex_document["SubTexture"];
	for (int subtex_num = 0; subtex_num < (int)subtexture_array.Size(); ++subtex_num)
	{
		Subtexture* subtexture = ReadSubTextureFromJSON(subtexture_array[subtex_num]);
		texture_atlas->sub_textures.push_back(*subtexture);
		delete subtexture;
	}

	return texture_atlas;
}

TextureAtlas::Subtexture* TextureAtlas::ReadSubTextureFromJSON(const rapidjson::Value & subtex_value)
{
	Subtexture* subtexture = new Subtexture();

	if(subtex_value.HasMember("name"))
		subtexture->name = subtex_value["name"].GetString();
	if (subtex_value.HasMember("x"))
		subtexture->x = subtex_value["x"].GetFloat();
	else
		subtexture->x = 0;

	if (subtex_value.HasMember("y"))
		subtexture->y = subtex_value["y"].GetFloat();
	else
		subtexture->y = 0;

	if (subtex_value.HasMember("width"))
		subtexture->width = subtex_value["width"].GetFloat();
	else
		subtexture->width = 0;

	if (subtex_value.HasMember("height"))
		subtexture->height = subtex_value["height"].GetFloat();
	else
		subtexture->height = 0;

	if (subtex_value.HasMember("frameX"))
		subtexture->frameX = subtex_value["frameX"].GetFloat();
	else
		subtexture->frameX = 0;

	if (subtex_value.HasMember("frameY"))
		subtexture->frameY = subtex_value["frameY"].GetFloat();
	else
		subtexture->frameY = 0;

	if (subtex_value.HasMember("frameWidth"))
		subtexture->frameWidth = subtex_value["frameWidth"].GetFloat();
	else
		subtexture->frameWidth = 0;

	if (subtex_value.HasMember("frameHeight"))
		subtexture->frameHeight = subtex_value["frameHeight"].GetFloat();
	else
		subtexture->frameHeight = 0;

	subtexture->sub_tex_transform.SetIdentity();

	gef::Matrix33 scaleMatrix;
	gef::Matrix33 translationMatrix;

	scaleMatrix.Scale(gef::Vector2(subtexture->width, subtexture->height));
	translationMatrix.SetIdentity();
	translationMatrix.SetTranslation(gef::Vector2(((subtexture->width* 0.5f) - (subtexture->frameWidth* 0.5f + subtexture->frameX)), ((subtexture->height* 0.5f) - (subtexture->frameHeight* 0.5f + subtexture->frameY))));

	subtexture->sub_tex_transform = scaleMatrix * translationMatrix;

	return subtexture;
}

void TextureAtlas::GetSpriteSizeFromSubtexture(gef::Sprite &sprite, Subtexture & subtexture)
{
	sprite.set_width(subtexture.width);
	sprite.set_height(subtexture.height);
}

void TextureAtlas::GetSpritePositionFromSubtexture(gef::Sprite &sprite, const Subtexture & subtexture, float x, float y)
{
	gef::Vector2 offset = gef::Vector2(subtexture.width*0.5 - (subtexture.frameWidth*0.5 + subtexture.frameX), subtexture.height*0.5 - (subtexture.frameHeight*0.5 + subtexture.frameY));

	sprite.set_position(gef::Vector4(x + offset.x, y + offset.y, 0));
}

void TextureAtlas::GetSpriteTextureFromSubtexture(gef::Sprite &sprite, const Subtexture & subtexture, TextureAtlas* textureAtlas)
{
	float uv_width;
	float uv_height;
	gef::Vector2 uv_pos;

	uv_pos = gef::Vector2(subtexture.x / textureAtlas->width, subtexture.y / textureAtlas->height);
	uv_width = subtexture.width / textureAtlas->width;
	uv_height = subtexture.height / textureAtlas->height;

	sprite.set_uv_position(uv_pos);
	sprite.set_uv_width(uv_width);
	sprite.set_uv_height(uv_height);
}

TextureAtlas::Subtexture TextureAtlas::GetSubtextureFromName(std::string name, TextureAtlas * textureAtlas)
{
	for (int i = 0; i < textureAtlas->sub_textures.size(); i++)
	{
		if (textureAtlas->sub_textures[i].name == name)
		{
			return textureAtlas->sub_textures[i];
		}
	};

	return Subtexture();
}


