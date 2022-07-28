#include "enemy_act_artillery.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "bullet_base.hpp"
#include "bullet_commander.hpp"
#include "interval.hpp"
#include "drawer.hpp"
#include "screen_shaker.hpp"

#include "effect_bomb.hpp"

#include "enemy_bullet_laser.hpp"
#include "enemy_bullet_circle.hpp"
#include "rose_collider_view.hpp"

#include "sound_manager.hpp"

#include "game_score.hpp"
#include "result_processor.hpp"

#include "eagle/scene_management.hpp"

namespace Internal {
	class ArtilleryBeam;

	class BeamAttribute final : public Eagle::Tweens::iAttribute<ArtilleryBeam, float> {
	public:
		BeamAttribute() = default;
		~BeamAttribute() = default;

		void SmoothValue(float t)override;
		void ChangeRelative()override;
		void SetReverse()override;
		void SetIncrement()override;
		void SetCurrentValue()override;
	};

	//[summary]
	//EnemyActArtilleryÇ™égÇ§íe
	class ArtilleryBeam : public BulletBase {
	public:
		ArtilleryBeam();
		ArtilleryBeam(const Eagle::Vector3& _velocity);
		~ArtilleryBeam();

		static ArtilleryBeam* Create(Eagle::GameObject* shooter, BulletCommander* commander, const Eagle::Vector3& _velocity, Eagle::Color color, float maxThickness = 50.f);

		CapsuleDrawer* GetCapsuleDrawer()const noexcept;

		Eagle::CapsuleCollider2D* GetCapsuleCollider()const noexcept;

		void SetCallback(std::function<void(void)> _callback);

	private:
		Player* player;
		CapsuleDrawer* capsuleDrawer;
		Eagle::CapsuleCollider2D* capsuleCollider;
		std::function<void(void)> callback;
		Eagle::Tweens::Sequence sequence;
		Eagle::GameObject* shooter;
		ScreenShaker* screenShaker;
		float maxRadius;
		bool predLine;

		void Start()override;

		void Update()override;

		void OnDestroy()override;

		Eagle::TweenerCore<BeamAttribute>& DoRadius(float endValue,float duration);

	};

	ArtilleryBeam::ArtilleryBeam()
		: BulletBase()
		, player(nullptr)
		, capsuleDrawer(nullptr)
		, capsuleCollider(nullptr)
		, callback()
		, sequence()
		, shooter(nullptr)
		, screenShaker(nullptr)
		, maxRadius(50.f)
		, predLine(true)
	{
	}

	ArtilleryBeam::ArtilleryBeam(const Eagle::Vector3& _velocity)
		: BulletBase()
		, player(nullptr)
		, capsuleDrawer(nullptr)
		, capsuleCollider(nullptr)
		, callback()
		, sequence()
		, shooter(nullptr)
		, screenShaker(nullptr)
		, maxRadius(50.f)
		, predLine(true)
	{
		velocity = _velocity;
	}

	ArtilleryBeam::~ArtilleryBeam()
	{
	}

	ArtilleryBeam* ArtilleryBeam::Create(Eagle::GameObject* shooter, BulletCommander* commander, const Eagle::Vector3& _velocity, Eagle::Color color, float maxThickness)
	{
		auto shotObj = Eagle::GameObject::Create();
		auto shotCmt = shotObj->AttachComponent<ArtilleryBeam>(_velocity);
		shotObj->SetLayer(2);
		shotCmt->capsuleDrawer = shotObj->AttachComponent<CapsuleDrawer>();
		shotCmt->capsuleCollider = shotObj->AttachComponent<Eagle::CapsuleCollider2D>();
		shotCmt->screenShaker = shotObj->AttachComponent<ScreenShaker>();
		shotObj->SetTag("EnemyShot");
		shotCmt->bulletCommander = commander;
		shotCmt->bulletCommander->AddBullet(shotCmt);
		shotCmt->player = Eagle::GameObject::FindWithTag("Player")->GetComponent<Player>();

		const Eagle::Vector2 colliderAxis = Eagle::ToVec2(_velocity.Normalize());
		const float colliderHeight = 600.f;
		const float colliderRadius = 12.f;

		shotCmt->capsuleDrawer->SetAxis(colliderAxis);
		shotCmt->capsuleDrawer->SetHeight(colliderHeight);
		shotCmt->capsuleDrawer->SetRadius(colliderRadius);
		shotCmt->capsuleDrawer->SetColor(color);
		shotCmt->capsuleCollider->SetAxis(colliderAxis);
		shotCmt->capsuleCollider->SetHeight(colliderHeight);
		shotCmt->capsuleCollider->SetRadius(colliderRadius);

		shotCmt->maxRadius = maxThickness;
		shotCmt->shooter = shooter;

		return shotCmt;
	}

	CapsuleDrawer* ArtilleryBeam::GetCapsuleDrawer() const noexcept
	{
		return capsuleDrawer;
	}

	Eagle::CapsuleCollider2D* ArtilleryBeam::GetCapsuleCollider() const noexcept
	{
		return capsuleCollider;
	}

	void ArtilleryBeam::SetCallback(std::function<void(void)> _callback)
	{
		callback = _callback;
	}

	void ArtilleryBeam::Start()
	{
		auto onComplate = [&]() {
			Eagle::GameObject::Destroy(this->GetGameObject());
			this->GetGameObject()->SetActive(false);
		};

		capsuleDrawer->SetTransparency(0.5f);

		sequence.Append(DoRadius(0.5f, 2.f).SetOnComplete([&]()
			{
				screenShaker->OnShake(Eagle::ToVec3(10.f, 0.f, 0.f), 1.f);
				capsuleDrawer->SetTransparency(1.f);
				SoundManager::Play("se_laser01.mp3", DX_PLAYTYPE_BACK, 1.f);
				predLine = false;
			}
		));
		sequence.Append(DoRadius(maxRadius, 0.05f));
		sequence.Append(DoRadius(0.1f, 5.f).SetOnComplete(onComplate).SetDelay(1.5f));

		sequence.Play();
	}

	void ArtilleryBeam::Update()
	{
		float radius = capsuleCollider->GetScaledRadius();
		Eagle::Vector2 offset = capsuleCollider->GetExtents().Normalize() * radius;

		capsuleCollider->SetOffset(Eagle::ToVec3(offset));
		capsuleDrawer->SetOffset(offset.x, offset.y);

		if (shooter && shooter->IsPendingDestroy())
		{
			callback = nullptr;
			shooter = nullptr;
			sequence.KillAll();
			auto onComplate = [&]() {
				Eagle::GameObject::Destroy(this->GetGameObject());
				this->GetGameObject()->SetActive(false);
			};
			float radius = capsuleCollider->GetRadius();
			sequence.Append(DoRadius(0.1f, 3.5f * (radius / maxRadius)).SetOnComplete(onComplate));

			SetEnable(false);
		}

		if (predLine)
			return;

		auto collider = GetGameObject()->GetCollider();

		if (collider->Collided(player->GetGameObject()->GetCollider()))
		{
			player->OnDamage(10.f);
		}
	}

	void ArtilleryBeam::OnDestroy()
	{
		if (callback)
			callback();

		BulletBase::OnDestroy();
	}

	Eagle::TweenerCore<BeamAttribute>& ArtilleryBeam::DoRadius(float endValue, float duration)
	{
		auto obj = GetGameObject();
		return *obj->AttachComponent<Eagle::TweenerCore<BeamAttribute>>(this, capsuleCollider->GetRadius(), endValue, duration);
	}

	void BeamAttribute::SmoothValue(float t)
	{
		setter->GetCapsuleCollider()->SetRadius(Eagle::Ease::Linear(t, startValue, endValue));
		setter->GetCapsuleDrawer()->SetRadius(Eagle::Ease::Linear(t, startValue, endValue));
	}

	void BeamAttribute::ChangeRelative()
	{	
		if (!relative)
		{
			endValue = startValue + endValue;
		}
		else
		{
			float changeValue = endValue - startValue;
			endValue = changeValue;
		}

		relative = !relative;
	}

	void BeamAttribute::SetReverse()
	{
		std::swap(startValue, endValue);
	}

	void BeamAttribute::SetIncrement()
	{
		float changeValue = endValue - startValue;
		startValue = endValue;
		endValue = startValue + changeValue;
	}

	void BeamAttribute::SetCurrentValue()
	{
		float r = setter->GetCapsuleCollider()->GetRadius();
		startValue = r;
	}

	class MirrorEnemy : public Eagle::Behavior{
	public:
		MirrorEnemy();
		~MirrorEnemy();

		BoxDrawer* GetBoxDrawer()const noexcept;
		Eagle::BoxCollider2D* GetBoxCollider()const noexcept;

		void SetBulletCommander(BulletCommander* _bulletCommander);
		void SetPlayer(Player* _player);

		static MirrorEnemy* Create();

	private:
		BoxDrawer* boxDrawer;
		Eagle::BoxCollider2D* boxCollider;
		BulletCommander* bulletCommander;
		Player* player;

		void OnCollisionEnter(const Eagle::Collision& collision)override;
	};

	MirrorEnemy::MirrorEnemy()
		: Behavior(ID::NoBehave)
		, boxDrawer(nullptr)
		, boxCollider(nullptr)
		, bulletCommander(nullptr)
		, player(nullptr)
	{
	}

	MirrorEnemy::~MirrorEnemy()
	{
	}

	BoxDrawer* MirrorEnemy::GetBoxDrawer() const noexcept
	{
		return boxDrawer;
	}

	Eagle::BoxCollider2D* MirrorEnemy::GetBoxCollider() const noexcept
	{
		return boxCollider;
	}

	void MirrorEnemy::SetBulletCommander(BulletCommander* _bulletCommander)
	{
		bulletCommander = _bulletCommander;
	}

	void MirrorEnemy::SetPlayer(Player* _player)
	{
		player = _player;
	}

	MirrorEnemy* MirrorEnemy::Create()
	{
		auto obj = Eagle::GameObject::Create();
		auto mrr = obj->AttachComponent<MirrorEnemy>();
		mrr->boxDrawer = obj->AttachComponent<BoxDrawer>();
		mrr->boxCollider = obj->AttachComponent<Eagle::BoxCollider2D>();

		return mrr;
	}

	void MirrorEnemy::OnCollisionEnter(const Eagle::Collision& collision)
	{
		Eagle::Vector3 shotVelocity = Eagle::ToVec3(Eagle::Vector2::FromAngle(Eagle::Random::Range(60.f, 120.f), 500.f));

		EnemyBulletLaser::Create(bulletCommander, shotVelocity, 0xe4007f, 25.f, 2.5f)
			->GetTransform()->SetPosition(collision.hit.transform->GetWorldPosition());
	}

	class ArtilleryBit;

	class InjectionAttribute : public Eagle::Tweens::iAttribute<ArtilleryBit, float> {
	public:
		InjectionAttribute() = default;
		~InjectionAttribute() = default;

		void SmoothValue(float t)override;
		void ChangeRelative()override;
		void SetReverse()override;
		void SetIncrement()override;
		void SetCurrentValue()override;
	};

	enum class BitActType {
		Laser,
		Circle,
		Shotgun
	};

	class ArtilleryBit : public Eagle::Behavior, public Eagle::PauseAndResume {
	public:
		ArtilleryBit();
		~ArtilleryBit();

		void SetInjectionAngle(float angle);
		float GetInjectionAngle();

		CircleDrawer* GetCircleDrawer()const noexcept;

		void Pause()override;
		void Resume()override;

		void SetActIndex(BitActType type);

		Eagle::TweenerCore<InjectionAttribute>& DoAngle(float startValue, float endValue, float duration);

		static ArtilleryBit* Create();

	private:
		UNIQUE(Interval) shotInterval;
		BulletCommander* bulletCommander;
		CircleDrawer* circleDrawer;
		void (ArtilleryBit::* actUpdate[3])();
		int actIndex;
		float injectionAngle;

		void Start()override;
		void Update()override;

		void ActLaser();
		void ActCircle();
		void ActShotgun();
	};

	ArtilleryBit::ArtilleryBit()
		: Behavior(ID::Update)
		, shotInterval(new Interval)
		, bulletCommander(nullptr)
		, circleDrawer(nullptr)
		, actUpdate{}
		, actIndex(0)
		, injectionAngle(0.f)
	{
		shotInterval->SetInterval(0.075f);
	}

	ArtilleryBit::~ArtilleryBit()
	{
	}

	void ArtilleryBit::SetInjectionAngle(float angle)
	{
		injectionAngle = angle;
	}

	float ArtilleryBit::GetInjectionAngle()
	{
		return injectionAngle;
	}

	CircleDrawer* ArtilleryBit::GetCircleDrawer() const noexcept
	{
		return circleDrawer;
	}

	void ArtilleryBit::Pause()
	{
		SetEnable(false);
	}

	void ArtilleryBit::Resume()
	{
		SetEnable(true);
	}

	void ArtilleryBit::SetActIndex(BitActType type)
	{
		actIndex = static_cast<int>(type);

		if (actIndex == 0)
		{
			shotInterval->SetInterval(0.075f);
		}
		else if (actIndex == 1)
		{
			shotInterval->SetInterval(0.1f);
		}
		else
		{
			shotInterval->SetInterval(0.05f);
		}
	}

	Eagle::TweenerCore<InjectionAttribute>& ArtilleryBit::DoAngle(float startValue, float endValue, float duration)
	{
		auto obj = GetGameObject();
		return *obj->AttachComponent<Eagle::TweenerCore<InjectionAttribute>>(this, startValue, endValue, duration);
	}

	ArtilleryBit* ArtilleryBit::Create()
	{
		auto obj = Eagle::GameObject::Create();
		auto cmt = obj->AttachComponent<ArtilleryBit>();
		cmt->circleDrawer = obj->AttachComponent<CircleDrawer>(10.f, 0xff0040);

		return cmt;
	}

	void ArtilleryBit::Start()
	{
		bulletCommander = Eagle::GameObject::FindWithTag("BulletCommander")
			->GetComponent<BulletCommander>();

		actUpdate[0] = &ArtilleryBit::ActLaser;
		actUpdate[1] = &ArtilleryBit::ActCircle;
		actUpdate[2] = &ArtilleryBit::ActShotgun;
	}

	void ArtilleryBit::Update()
	{
		shotInterval->Update();

		if (circleDrawer->GetTransparency() < 0.8f)
			return;

		if (!shotInterval->IsReady())
			return;

		shotInterval->Ready();

		(this->*actUpdate[actIndex])();
	}

	void ArtilleryBit::ActLaser()
	{
		Eagle::Vector3 shotVelocity = Eagle::ToVec3(Eagle::Vector2::FromAngle(injectionAngle, 200.f));
		EnemyBulletLaser::Create(bulletCommander, shotVelocity, circleDrawer->GetColor(), 60.f, 2.5f)
			->GetTransform()->SetPosition(GetTransform()->GetWorldPosition(false) + shotVelocity * 0.1f);
		EnemyBulletLaser::Create(bulletCommander, -shotVelocity, circleDrawer->GetColor(), 60.f, 2.5f)
			->GetTransform()->SetPosition(GetTransform()->GetWorldPosition(false) - shotVelocity * 0.15f);
	}

	void ArtilleryBit::ActCircle()
	{
		float offsetAngle = 0.f;

		for (int i = 0; i < 4; ++i)
		{
			Eagle::Vector3 shotVelocity = Eagle::ToVec3(Eagle::Vector2::FromAngle(injectionAngle + offsetAngle, 100.f));
			EnemyBulletCircle::Create(bulletCommander, shotVelocity, circleDrawer->GetColor(), 7.5f)
				->GetTransform()->SetPosition(GetTransform()->GetWorldPosition(false));
			offsetAngle += 90.f;
		}
	}

	void ArtilleryBit::ActShotgun()
	{
		static bool isActive = true;
		static int index = 0;

		if (!isActive)
		{
			index -= 1;
			if (index == 0)
			{
				isActive = !isActive;
			}
			return;
		}

		float offsetAngle = 0.f;
		Eagle::Vector3 plPos = Eagle::GameObject::FindWithTag("Player")->GetTransform()->GetWorldPosition(false);
		Eagle::Vector2 plPos2 = Eagle::ToVec2(plPos);
		Eagle::Vector2 myPos2 = Eagle::ToVec2(GetTransform()->GetWorldPosition(false));
		Eagle::Vector2 toVec = myPos2.To(plPos2).Normalize();
		offsetAngle = Eagle::Random::Range(-60.f, 60.f);
		float baseAngle = std::atan2f(toVec.y,toVec.x) * Eagle::Math::DEGREE_F;
		Eagle::Vector3 shotVelocity = Eagle::ToVec3(Eagle::Vector2::FromAngle(baseAngle + offsetAngle, 100.f));

		EnemyBulletCircle::Create(bulletCommander, shotVelocity, circleDrawer->GetColor(), 12.f)
			->GetTransform()->SetPosition(GetTransform()->GetWorldPosition(false));

		index += 2;

		if (index == 96)
		{
			isActive = false;
		}
	}

	void InjectionAttribute::SmoothValue(float t)
	{
		setter->SetInjectionAngle(Eagle::Easing(t, startValue, endValue));
	}

	void InjectionAttribute::ChangeRelative()
	{
		if (!relative)
		{
			endValue = startValue + endValue;
		}
		else
		{
			float changeValue = endValue - startValue;
			endValue = changeValue;
		}

		relative = !relative;
	}

	void InjectionAttribute::SetReverse()
	{
		std::swap(startValue, endValue);
	}

	void InjectionAttribute::SetIncrement()
	{
		float changeValue = endValue - startValue;
		startValue = endValue;
		endValue = startValue + changeValue;
	}

	void InjectionAttribute::SetCurrentValue()
	{
		float ang = setter->GetInjectionAngle();
		startValue = ang;
	}

	Eagle::TweenerCore<Eagle::MoveAttribute>* ShotRose(
		BulletCommander* commander, 
		const Eagle::Vector3& origin, 
		int flowerNum, 
		int flowerRadius, 
		float radius, 
		int shotNum,
		Eagle::Color color,
		float duration,
		std::function<void(void)> onComplate)
	{
		float r = 0.f;
		float fx = 0.f;
		float fy = 0.f;
		float t = 0.f;
		int idxLast = 0;
		Eagle::DynamicArray<Eagle::Vector3> point;
		point.Reserve(shotNum);

		float n = static_cast<float>(flowerNum); //â‘Ç—ÇÁÇÃñáêî
		float k = static_cast<float>(flowerRadius); //â‘Ç—ÇÁÇÃçLÇ™ÇË

		float steps = static_cast<float>(shotNum);

		while (t <= 360.f * k)
		{
			Eagle::Math::Clamp(t, 0.f, 360.f * k);
			r = std::sin(n * t / k) * radius;
			fx = r * std::cos(t) + origin.x;
			fy = -r * std::sin(t) + origin.y;
			point.Add(Eagle::Vector3(fx, fy, 0.f));

			t += (360.f / steps) * k;
		}

		Eagle::TweenerCore<Eagle::MoveAttribute>* tween = nullptr;

		float bodyrRdius = (GameConfig::GetGameLevel() == GameLevel::Hard) ? 3.f : 2.f;

		for (auto& pos : point)
		{
			Eagle::Vector3 shotVelocity = origin.To(pos).Normalize() * 100.f;
			auto cir = EnemyBulletCircle::Create(commander, shotVelocity, color, bodyrRdius);
			cir->GetGameObject()->SetLayer(2);
			cir->GetTransform()->SetPosition(origin);
			cir->GetGameObject()->AttachComponent<RoseColliderView>(cir->GetCircleDrawer(), cir->GetGameObject()->AttachComponent<CircleDrawer>(10.f, color));
			tween = &cir->GetTransform()->DoMove(pos, duration).SetEase(Eagle::Ease::Quint::Out);
		}

		return &tween->SetOnComplete(onComplate);
	}
}

EnemyActArtillery::EnemyActArtillery()
	: EnemyActBase()
	, openInterval(new Interval)
	, beamInterval(new Interval)
	, gardMirror()
	, gardBit()
	, circleDrawer(nullptr)
	, myEnemy(nullptr)
	, player(nullptr)
	, callbackTween(nullptr)
	, intervalFlag(0b111)
{
	SetBehaveID(ID::Update);
	beamInterval->SetInterval(GameConfig::IsEnableGameLevel(GameLevel::Hard) ? 9.f : 11.f);
	openInterval->SetInterval(7.5f);
	gardMirror.Fill(nullptr);
	gardBit.Fill(nullptr);
}

EnemyActArtillery::~EnemyActArtillery()
{
}

void CreateBomb(Eagle::Vector3 pos, float radius, Eagle::Color color)
{
	EffectBomb::Create(pos, radius, color);
}

void MoveResultScene()
{
	Eagle::SceneManager::CreateScene("ResultScene");
	Eagle::SceneManager::SetChangeActiveScene("ResultScene");

	Eagle::GameObject::Create("Processor")
		->AttachComponent<ResultProcessor>();
}

void EnemyActArtillery::OnShootDown()
{
	OnMoveEnd();

	if (callbackTween)
		callbackTween->Kill();

	for (auto& mirror : gardMirror)
	{
		Eagle::GameObject::Destroy(mirror->GetGameObject());
	}

	for (auto& bit : gardBit)
	{
		Eagle::GameObject::Destroy(bit->GetGameObject());
		EffectBomb::Create(bit->GetTransform()->GetWorldPosition(false), 20.f, Eagle::Color::Pallet::Orange);
	}

	bulletCommander->RemoveAll();

	EffectBomb::Create(GetTransform()->GetWorldPosition(false), 40.f, Eagle::Color::Pallet::Orange);
	Eagle::Vector3 wPos = GetTransform()->GetWorldPosition(false);
	float offsetX = Eagle::Random::Range01f() * 60.f - 30.f;
	float offsetY = Eagle::Random::Range01f() * 60.f - 30.f;
	Eagle::Invoke(0.75f, CreateBomb, wPos + Eagle::ToVec3(offsetX, offsetY, 0.f), 40.f, Eagle::Color::Pallet::Orange);
	offsetX = Eagle::Random::Range01f() * 60.f - 30.f;
	offsetY = Eagle::Random::Range01f() * 60.f - 30.f;
	Eagle::Invoke(1.5f, CreateBomb, wPos + Eagle::ToVec3(offsetX, offsetY, 0.f), 40.f, Eagle::Color::Pallet::Orange);
	offsetX = Eagle::Random::Range01f() * 60.f - 30.f;
	offsetY = Eagle::Random::Range01f() * 60.f - 30.f;
	Eagle::Invoke(2.25f, CreateBomb, wPos + Eagle::ToVec3(offsetX, offsetY, 0.f), 40.f, Eagle::Color::Pallet::Orange);
	offsetX = Eagle::Random::Range01f() * 60.f - 30.f;
	offsetY = Eagle::Random::Range01f() * 60.f - 30.f;
	Eagle::Invoke(3.0f, CreateBomb, wPos + Eagle::ToVec3(offsetX, offsetY, 0.f), 50.f, Eagle::Color::Pallet::Orange);

	GameScore::AddScore(100000000);
	float bonasRate = [&] {
		int rate = static_cast<int>(player->GetArmor());
		rate -= (rate % 5);
		return static_cast<float>(rate) * 0.01f;
	}();
	float baseBonasScore = 100000000.f;
	GameScore::AddScore(static_cast<int>(bonasRate * baseBonasScore));
	Eagle::Invoke(6.f, MoveResultScene);
}

void EnemyActArtillery::Awake()
{
	for (auto& mirror : gardMirror)
	{
		mirror = Internal::MirrorEnemy::Create();
	}

	GetGameObject()->SetLayer(2);
}

void EnemyActArtillery::Start()
{
	GetTransform()->SetPosition(400, -50.f, 0);
	GetTransform()->DoMoveY(200.f, 2.f).SetEase(Eagle::Ease::Quart::InOut);

	bulletCommander = Eagle::GameObject::FindWithTag("BulletCommander")
		->GetComponent<BulletCommander>();

	Eagle::Vector3 localPos[] = {
		Eagle::ToVec3(20,0,0),
		Eagle::ToVec3(-20,0,0)
	};

	int idx = 0;

	player = Eagle::GameObject::FindWithTag("Player")->GetComponent<Player>();

	for (auto& mirror : gardMirror)
	{
		mirror->GetTransform()->SetPosition(GetTransform()->GetWorldPosition());
		mirror->GetGameObject()->SetParent(GetGameObject());
		mirror->GetGameObject()->GetTransform()->SetPosition(localPos[idx]);
		mirror->GetBoxDrawer()->SetSize(40, 80);
		mirror->GetBoxDrawer()->SetColor(0xff0040);
		mirror->GetBoxDrawer()->SetTransparency(0.5f);
		mirror->GetBoxCollider()->SetSize(80, 40);
		mirror->SetBulletCommander(bulletCommander);
		mirror->SetPlayer(player);
		idx += 1;
	}

	Eagle::Vector3 pos = GetTransform()->GetWorldPosition();

	auto onComplateBit0 = [&]() {
		gardBit[0]->SetEnable(true);
		gardBit[0]->DoAngle(120.f, 480.f, 2.f).SetLoop(-1, Eagle::LoopType::Increment);
		gardBit[0]->GetTransform()->DoMoveY(80.f, 3.f).SetLoop(-1, Eagle::LoopType::Reverse).SetEase(Eagle::Ease::Cubic::InOut);
	};

	auto onComplateBit1 = [&]() {
		gardBit[1]->SetEnable(true);
		gardBit[1]->DoAngle(60.f, -300.f, 2.f).SetLoop(-1, Eagle::LoopType::Increment);
		gardBit[1]->GetTransform()->DoMoveY(80.f, 3.f).SetLoop(-1, Eagle::LoopType::Reverse).SetEase(Eagle::Ease::Cubic::InOut);
	};

	gardBit[0] = Internal::ArtilleryBit::Create();
	gardBit[0]->GetTransform()->SetPosition(550.f, -50.f, 0.f);
	gardBit[0]->SetEnable(false);
	gardBit[0]->GetTransform()->DoMoveY(160.f, 2.f).SetOnComplete(onComplateBit0).SetEase(Eagle::Ease::Quart::InOut);
	gardBit[1] = Internal::ArtilleryBit::Create();
	gardBit[1]->GetTransform()->SetPosition(250.f, -50.f, 0.f);
	gardBit[1]->SetEnable(false);
	gardBit[1]->GetTransform()->DoMoveY(160.f, 2.f).SetOnComplete(onComplateBit1).SetEase(Eagle::Ease::Quart::InOut);

	auto collider = GetGameObject()->GetCollider<Eagle::CircleCollider2D>();

	if (collider)
	{
		collider->SetRadius(38.f);
	}

	circleDrawer = GetGameObject()->GetComponent<CircleDrawer>();

	if (circleDrawer)
	{
		circleDrawer->SetRadius(38.f);
		circleDrawer->SetColor(0xff0040);
	}

	myEnemy = GetGameObject()->GetComponent<Enemy>();
}

void EnemyActArtillery::Update()
{
	if (myEnemy->GetArmor() == 500)
	{
		gardBit[0]->SetActIndex(Internal::BitActType::Shotgun);
		gardBit[1]->SetActIndex(Internal::BitActType::Shotgun);
	}

	if (myEnemy->GetArmor() == 250)
	{
		gardBit[0]->SetActIndex(Internal::BitActType::Circle);
		gardBit[1]->SetActIndex(Internal::BitActType::Circle);
	}

	if (intervalFlag & 0b01)
	{
		openInterval->Update();
	}

	if (intervalFlag & 0b10)
	{
		beamInterval->Update();
	}

	if (openInterval->IsReady())
	{
		gardMirror[0]->GetTransform()->DoScaleY(-0.5f, 1.5f).SetDelay(1.5f);
		gardMirror[1]->GetTransform()->DoScaleY(-0.5f, 1.5f).SetDelay(1.5f);
		gardMirror[0]->GetBoxDrawer()->DoColor(0xf4005f, 1.5f).SetDelay(1.5f);
		gardMirror[1]->GetBoxDrawer()->DoColor(0xf4005f, 1.5f).SetDelay(1.5f);
		gardMirror[0]->GetBoxDrawer()->DoFade(1.f, 1.5f).SetDelay(1.5f);
		gardMirror[1]->GetBoxDrawer()->DoFade(1.f, 1.5f).SetDelay(1.5f);
		gardMirror[0]->GetTransform()->DoLocalRotateZ(90.f, 1.f).SetEase(Eagle::Ease::Quart::In);
		gardMirror[1]->GetTransform()->DoLocalRotateZ(90.f, 1.f).SetEase(Eagle::Ease::Quart::In);
		gardMirror[0]->GetTransform()->DoLocalMoveX(60.f, 3.f).SetEase(Eagle::Ease::Quart::Out);
		gardMirror[1]->GetTransform()->DoLocalMoveX(-60.f, 3.f).SetEase(Eagle::Ease::Quart::Out).SetOnComplete([&]() 
			{
				gardMirror[0]->GetGameObject()->SetTag("Enemy");
				gardMirror[1]->GetGameObject()->SetTag("Enemy");
			}
		);
		gardBit[0]->GetCircleDrawer()->DoColor(0xf4005f, 3.0f);
		gardBit[0]->GetCircleDrawer()->DoFade(0.5f, 1.5f).SetDelay(1.5f);
		gardBit[1]->GetCircleDrawer()->DoColor(0xf4005f, 3.0f);
		gardBit[1]->GetCircleDrawer()->DoFade(0.5f, 1.5f).SetDelay(1.5f);
		circleDrawer->DoColor(0xf4005f, 1.5f).SetDelay(1.5f);

		openInterval->Ready();
		Eagle::BitOff(intervalFlag, 0);
	}

	if (!beamInterval->IsReady())
		return;

	beamInterval->Ready();
	Eagle::BitOff(intervalFlag, 1);

	if (myEnemy->GetArmor() <= 200)
	{
		LastPhase();
		return;
	}

	auto onComplate = [&]() {
		intervalFlag = 0b11;

		gardMirror[0]->GetTransform()->DoScale(Eagle::ToVec3(1, 1, 1), 0.5f).SetDelay(0.5f);
		gardMirror[1]->GetTransform()->DoScale(Eagle::ToVec3(1, 1, 1), 0.5f).SetDelay(0.5f);
		gardMirror[0]->GetBoxDrawer()->DoColor(0xff0040, 0.5f).SetDelay(0.5f);
		gardMirror[1]->GetBoxDrawer()->DoColor(0xff0040, 0.5f).SetDelay(0.5f);
		gardMirror[0]->GetBoxDrawer()->DoFade(0.5f, 0.5f).SetDelay(0.5f);
		gardMirror[1]->GetBoxDrawer()->DoFade(0.5f, 0.5f).SetDelay(0.5f);
		gardMirror[0]->GetTransform()->DoLocalRotateZ(-90.f, 0.5f)
			.SetEase(Eagle::Ease::Quart::In)
			.SetDelay(0.5f);
		gardMirror[1]->GetTransform()->DoLocalRotateZ(-90.f, 0.5f)
			.SetEase(Eagle::Ease::Quart::In)
			.SetDelay(0.5f);
		gardMirror[0]->GetTransform()->DoLocalMoveX(-60.f, 1.f)
			.SetEase(Eagle::Ease::Quart::In)
			.SetDelay(0.5f);
		&gardMirror[1]->GetTransform()->DoLocalMoveX(60.f, 1.f)
			.SetEase(Eagle::Ease::Quart::In)
			.SetDelay(0.5f)
			.SetOnStart([&]() 
				{
					gardMirror[0]->GetGameObject()->SetTag("");
					gardMirror[1]->GetGameObject()->SetTag("");
				}
			);
		gardBit[0]->GetCircleDrawer()->DoColor(0xff0040, 3.f);
		gardBit[0]->GetCircleDrawer()->DoFade(1.f, 1.5f);
		gardBit[1]->GetCircleDrawer()->DoColor(0xff0040, 3.f);
		gardBit[1]->GetCircleDrawer()->DoFade(1.f, 1.5f);
		circleDrawer->DoColor(0xff0040, 0.5f).SetDelay(0.5f);

		callbackTween = nullptr;
	};

	Eagle::Vector3 pos = GetTransform()->GetWorldPosition();

	Internal::ArtilleryBeam* beam = nullptr;
	beam = Internal::ArtilleryBeam::Create(GetGameObject(), bulletCommander, Eagle::ToVec3(0, 1, 0), 0xf4005f, 80.f);
	beam->GetTransform()->SetPosition(pos + Eagle::ToVec3(0, 350, 0));
	beam->SetCallback(onComplate);
}

void EnemyActArtillery::OnCollisionEnter(const Eagle::Collision& collision)
{
	if (circleDrawer->GetColor() != Eagle::Color(0xf4005f))
		return;

	Eagle::Vector3 shotVelocity = Eagle::ToVec3(Eagle::Vector2::FromAngle(Eagle::Random::Range(60.f, 120.f), 500.f));
	EnemyBulletLaser::Create(bulletCommander, shotVelocity, 0xf4005f, 25.f, 2.5f)
		->GetTransform()->SetPosition(collision.hit.transform->GetWorldPosition());
}

void EnemyActArtillery::LastPhase()
{
	auto onComplate = [&]() {
		intervalFlag = 0b11;

		gardMirror[0]->GetTransform()->DoScale(Eagle::ToVec3(1, 1, 1), 0.5f).SetDelay(0.5f);
		gardMirror[1]->GetTransform()->DoScale(Eagle::ToVec3(1, 1, 1), 0.5f).SetDelay(0.5f);
		gardMirror[0]->GetBoxDrawer()->DoColor(0xff0040, 0.5f).SetDelay(0.5f);
		gardMirror[1]->GetBoxDrawer()->DoColor(0xff0040, 0.5f).SetDelay(0.5f);
		gardMirror[0]->GetBoxDrawer()->DoFade(0.5f, 0.5f).SetDelay(0.5f);
		gardMirror[1]->GetBoxDrawer()->DoFade(0.5f, 0.5f).SetDelay(0.5f);
		gardMirror[0]->GetTransform()->DoLocalRotateZ(-90.f, 0.5f)
			.SetEase(Eagle::Ease::Quart::In)
			.SetDelay(0.5f);
		gardMirror[1]->GetTransform()->DoLocalRotateZ(-90.f, 0.5f)
			.SetEase(Eagle::Ease::Quart::In)
			.SetDelay(0.5f);
		gardMirror[0]->GetTransform()->DoLocalMoveX(-60.f, 1.f)
			.SetEase(Eagle::Ease::Quart::In)
			.SetDelay(0.5f);
		gardMirror[1]->GetTransform()->DoLocalMoveX(60.f, 1.f)
			.SetEase(Eagle::Ease::Quart::In)
			.SetDelay(0.5f)
			.SetOnStart([&]()
				{
					gardMirror[0]->GetGameObject()->SetTag("");
					gardMirror[1]->GetGameObject()->SetTag("");
				}
		);
		gardBit[0]->GetCircleDrawer()->DoColor(0xff0040, 3.f);
		gardBit[0]->GetCircleDrawer()->DoFade(1.f, 1.5f);
		gardBit[1]->GetCircleDrawer()->DoColor(0xff0040, 3.f);
		gardBit[1]->GetCircleDrawer()->DoFade(1.f, 1.5f);
		circleDrawer->DoColor(0xff0040, 0.5f).SetDelay(0.5f);

		callbackTween = nullptr;
	};

	static const int flowerNum[] = {7,9,11};

	callbackTween = Internal::ShotRose(
		bulletCommander,
		GetTransform()->GetWorldPosition(false),
		flowerNum[Eagle::Random::Range(0, 2)],
		5,
		190.f,
		360,
		0xf4005f,
		3.5f,
		onComplate
	);
}
