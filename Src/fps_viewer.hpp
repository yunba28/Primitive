#pragma once
#include "eagle.hpp"

class Font;

namespace Internal {
	class FpsUpdate : public Eagle::Behavior {
	public:
		FpsUpdate();
		~FpsUpdate();

		float GetFps()const noexcept;

	private:
		float count;
		float t;
		float secFps;

		void Update()override;
	};
}

class FpsViewer : public Eagle::Renderer {
public:
	FpsViewer();
	FpsViewer(const Eagle::String& _fontName, unsigned int _fontSize, Eagle::Color _color);
	~FpsViewer();

private:
	Internal::FpsUpdate* fpsUpdate;
	UNIQUE(Font) font;
	Eagle::Color color;

	void Awake()override;
	void Render()override;
};