#include "stdafx.h"

#include "GUIManager.h"
#include "Mesh.h"
#include <AEEngine.h>
#include <cstdarg>


vector<GUIManager::UIElement> GUIManager::elements;
AEGfxTexture *GUIManager::texture;
SpriteSource *GUIManager::font;
AEGfxVertexList *GUIManager::mesh;

void GUIManager::Init() {
	texture = AEGfxTextureLoad("Assets/FontSheet.png");
	font = new SpriteSource(16, 6, texture);
	mesh = MeshCreateQuad(0.5f, 0.5f, 1.0f / 16, 1.0f / 5, "LetterMesh");
}

void GUIManager::Update() {
	for (UIElement uie : elements) {
		uie.Update(uie.data);
	}
}

void GUIManager::Shutdown() {
	elements.clear();
	delete font;
}

void GUIManager::AddUIElement(UIUpdateFunc UpdateUIFunc, void * data[]) {
	elements.push_back(UIElement(UpdateUIFunc, data));
}

/*GameObject **GUIManager::PrintTextOnScreen(const char * formatStr, ...) {
	va_list args;
	va_start(args, formatStr);
	
	char string[100];
	sprintf(string, formatStr, args);
	//GameObject *letters[100];
	int i;
	for (i = 0; string[i]; i++) {
		int frameIndex = 0;
		switch (string[i]) {
		case '!':
			break;
		}

		float u, v;
		font->GetUV(frameIndex, &u, &v);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(font->GetTexture(), u, v);
	}

	va_end(args);
}*/

GUIManager::UIElement::UIElement(UIUpdateFunc *Update, void * data[]) :
		Update(Update)
{
	int i;
	for (i = 0; data[i]; i++)
		this->data[i] = data[i];
	this->data[i] = nullptr;
}
