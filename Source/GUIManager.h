#pragma once

#include "GameObject.h"
#include "SpriteSource.h"
#include <vector>
using std::vector;

#define MAX_UI_DATA 50
//#define UIUpdateFunc(name) void name(void *data[])
//#define UIUpdateFuncPtr(name) void (*name)(void *data[])
typedef void UIUpdateFunc(void *data[]);
//typedef UIUpdateFuncPtr *UIUpdateFunc;

class GUIManager {
public:
	static void Init();
	static void Update();
	static void Shutdown();
	
	static void AddUIElement(UIUpdateFunc UpdateUIFunc, void *data[]);

	//static GameObject **PrintTextOnScreen(const char *formatStr, ...);

private:
	struct UIElement {
		UIElement(UIUpdateFunc *Update, void *data[]);
		UIUpdateFunc *Update;
		void *data[MAX_UI_DATA];
	};

	static vector<UIElement> elements;
	static AEGfxTexture *texture;
	static SpriteSource *font;
	static AEGfxVertexList *mesh;
};