#include "fps_viewer.hpp"
#include "font.hpp"

namespace Internal {
	FpsUpdate::FpsUpdate()
		: Behavior(ID::Update)
		, count(0.f)
		, t(0.f)
		, secFps(0.f)
	{
	}

	FpsUpdate::~FpsUpdate()
	{
	}

	float FpsUpdate::GetFps() const noexcept
	{
		return secFps;
	}

	void FpsUpdate::Update()
	{
		count += 1.f;
		t += Eagle::Time::DeltaTimeF();
		if (t >= 1.f)
		{
			float ext = 1.f / t;
			secFps = count * ext;
			count = 0.f;
			t = 0.f;
		}
	}
}

FpsViewer::FpsViewer()
	: Renderer()
	, fpsUpdate(nullptr)
	, font(nullptr)
	, color(0xffffff)
{
}

FpsViewer::FpsViewer(const Eagle::String& _fontName, unsigned int _fontSize, Eagle::Color _color)
	: Renderer()
	, fpsUpdate(nullptr)
	, font(Font::Create(_fontName, _fontSize))
	, color(_color)
{
}

FpsViewer::~FpsViewer()
{
}

void FpsViewer::Awake()
{
	fpsUpdate = GetGameObject()->AttachComponent<Internal::FpsUpdate>();
}

void FpsViewer::Render()
{
	Eagle::String fps = Eagle::Format("%06.2f fps", fpsUpdate->GetFps());
	Eagle::Vector3 pos = GetTransform()->GetWorldPosition();
	(font)
		? DrawString(pos, fps, font->GetHandle(), color,DrawPosition::Right)
		: DrawString(pos, fps, color, DrawPosition::Right);
}
