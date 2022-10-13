#include "stdafx.h"
#include "SpriteFontLoader.h"

SpriteFont* SpriteFontLoader::LoadContent(const ContentLoadInfo& loadInfo)
{
	const auto pReader = new BinaryReader();
	pReader->Open(loadInfo.assetFullPath);

	if (!pReader->Exists())
	{
		Logger::LogError(L"Failed to read the assetFile!\nPath: \'{}\'", loadInfo.assetSubPath);
		return nullptr;
	}

	//TODO_W5(L"Implement SpriteFontLoader >> Parse .fnt file")
	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	char identifierB = pReader->Read<char>();
	char identifierM = pReader->Read<char>();
	char identifierF = pReader->Read<char>();
	char identifierV = pReader->Read<char>();
	//If Identification bytes doesn't match B|M|F,
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	//...
	if (identifierB != 'B' || identifierM != 'M' || identifierF != 'F' || identifierV != 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Not a valid .fnt font");
		return nullptr;
	}

	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...

	//Valid .fnt file >> Start Parsing!
	//use this SpriteFontDesc to store all relevant information (used to initialize a SpriteFont object)
	SpriteFontDesc fontDesc{};

	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the FontSize [fontDesc.fontSize]
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [fontDesc.fontName]
	//...
	char blockID = pReader->Read<char>();
	int blockSize = pReader->Read<int>();
	fontDesc.fontSize = pReader->Read<short>();
	pReader->MoveBufferPosition(12);
	fontDesc.fontName = pReader->ReadNullString();


	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Texture Width & Height [fontDesc.textureWidth/textureHeight]
	//Retrieve PageCount
	//> if pagecount > 1
	//	> Log Error (Only one texture per font is allowed!)
	//Advance to Block2 (Move Reader)
	//...
	blockID = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	pReader->MoveBufferPosition(4);
	fontDesc.textureWidth = pReader->Read<short>();
	fontDesc.textureHeight = pReader->Read<short>();
	short pages = pReader->Read<short>();
	if (pages > 1)
	{
		Logger::LogError(L"Only one texture per font is allowed!");
		return nullptr;
	}
	pReader->MoveBufferPosition(5);

	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the PageName (BinaryReader::ReadNullString)
	//Construct the full path to the page texture file
	//	>> page texture should be stored next to the .fnt file, pageName contains the name of the texture file
	//	>> full texture path = asset parent_path of .fnt file (see loadInfo.assetFullPath > get parent_path) + pageName (filesystem::path::append)
	//	>> Load the texture (ContentManager::Load<TextureData>) & Store [fontDesc.pTexture]
	blockID = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	std::wstring pageName = pReader->ReadNullString();
	std::wstring fullpath = loadInfo.assetFullPath.parent_path().append(pageName);
	fontDesc.pTexture = ContentManager::Load<TextureData>(fullpath);

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve Character Count (see documentation)
	//Create loop for Character Count, and:
	//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
	//> Create instance of FontMetric (struct)
	//	> Set Character (CharacterId) [FontMetric::character]
	//	> Retrieve Xposition (store Local)
	//	> Retrieve Yposition (store Local)
	//	> Retrieve & Set Width [FontMetric::width]
	//	> Retrieve & Set Height [FontMetric::height]
	//	> Retrieve & Set OffsetX [FontMetric::offsetX]
	//	> Retrieve & Set OffsetY [FontMetric::offsetY]
	//	> Retrieve & Set AdvanceX [FontMetric::advanceX]
	//	> Retrieve & Set Page [FontMetric::page]
	//	> Retrieve Channel (BITFIELD!!!) 
	//		> See documentation for BitField meaning [FontMetrix::channel]
	//	> Calculate Texture Coordinates using Xposition, Yposition, fontDesc.TextureWidth & fontDesc.TextureHeight [FontMetric::texCoord]
	//
	//> Insert new FontMetric to the metrics [font.metrics] map
	//	> key = (wchar_t) charId
	//	> value = new FontMetric
	//(loop restarts till all metrics are parsed)
	blockID = pReader->Read<char>();
	blockSize = pReader->Read<int>();
	int characterCount = blockSize / 20;
	for (int i{ 0 }; i < characterCount; ++i)
	{
		wchar_t characterID{};
		FontMetric metric{};
		characterID = static_cast<wchar_t>(pReader->Read<int>());
		metric.character = characterID;

		const short xpos = pReader->Read<short>();
		const short ypos = pReader->Read<short>();

		metric.width = pReader->Read<unsigned short>();
		metric.height = pReader->Read<unsigned short>();
		metric.offsetX = pReader->Read<short>();
		metric.offsetY = pReader->Read<short>();
		metric.advanceX = pReader->Read<short>();
		metric.page = pReader->Read<unsigned char>();

		//bitfield
		metric.channel = pReader->Read<unsigned char>();
		if (metric.channel & 1) metric.channel = 2; //Blue
		else if (metric.channel & 2) metric.channel = 1; //Green
		else if (metric.channel & 4) metric.channel = 0; //Red
		else if (metric.channel & 8) metric.channel = 3; //Alpha
		else if (metric.channel & 15) metric.channel = 4; //All Channels

		metric.texCoord.x = float(xpos) / fontDesc.textureWidth;
		metric.texCoord.y = float(ypos) / fontDesc.textureHeight;

		fontDesc.metrics.emplace(characterID, metric);
	}

	//Done!
	delete pReader;
	return new SpriteFont(fontDesc);
}


void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
