#include "stdafx.h"
#include "NewUnitButton.h"

using std::to_string;
#include <map>
using std::map;

#include "GameObjectManager.h"
#include "TextBox.h"
#include "ArmyTraits.h"

#include "ChangeStatButton.h"
#include "ChangeWeaponAbilityButton.h"
#include "CustomPointCounter.h"
#include "CostDisplay.h"

const vector<NewUnitButton::ObjData> NewUnitButton::objData = {
	{"Box",			"Box",					{0, 0}},
	{"Name",		"Name",					{-190, 45}},
	{"Sprite",		"Sprite",				{-190, -20}},
	{"Strength",	"Stat",					{-60, 37}},
	{"StrengthUp",	"StatChange",			{0, 46}},
	{"StrengthDown","StatChange",			{0, 28}},
	{"Agility",		"Stat",					{-60, 0}},
	{"AgilityUp",	"StatChange",			{0, 9}},
	{"AgilityDown",	"StatChange",			{0, -9}},
	{"Defense",		"Stat",					{-60, -37}},
	{"DefenseUp",	"StatChange",			{0, -28}},
	{"DefenseDown",	"StatChange",			{0, -46}},
	{"AbilityT",	"Stat",					{90, 45}},
	{"AbilityV",	"Stat",					{90, 25}},
	{"AbilityNext",	"WeaponAbilityChange",	{160, 25}},
	{"AbilityPrev",	"WeaponAbilityChange",	{20, 25}},
	{"WeaponT",		"Stat",					{90, -25}},
	{"WeaponV",		"Stat",					{90, -45}},
	{"WeaponNext",	"WeaponAbilityChange",	{160, -45}},
	{"WeaponPrev",	"WeaponAbilityChange",	{20, -45}},
	{"PointCounter","PointCounter",			{215, 45}},
	{"CostDisplay",	"CostDisplay",			{215, -25}},
};

NewUnitButton::NewUnitButton() : Button("NewUnitButton") {
}

void NewUnitButton::Load(rapidjson::Value & obj) {
	Button::Load(obj);
	rapidjson::Value *tmp;

	if (obj.HasMember("Area") && (tmp = &obj["Area"])->IsArray() && tmp->Size() == 4) {
		area.top = (*tmp)[0].GetFloat();
		area.bottom = (*tmp)[1].GetFloat();
		area.left = (*tmp)[2].GetFloat();
		area.right = (*tmp)[3].GetFloat();
	}

	if (obj.HasMember("Spacing") && (tmp = &obj["Spacing"])->IsArray() && tmp->Size() == 2) {
		spacing.x = (*tmp)[0].GetFloat();
		spacing.y = (*tmp)[1].GetFloat();
	}
}

Component * NewUnitButton::Clone() const {
	return new NewUnitButton(*this);
}

void NewUnitButton::ClickEffect(float dt) {
	GameObjectManager *gom = GetParent()->GetGameObjectManager();
	ArmyTraits *armyTraits = gom->GetObjectByName("MainObject")->GetComponent<ArmyTraits>();

	BehaviorUnit::Traits *newUnit = armyTraits->NewUnitData(string("Unit")+to_string(unitCount+1));

	int cols = (int)(area.Size().x / fabs(spacing.x));
	int rows = (int)(area.Size().y / fabs(spacing.y));
	Vector2D pos = area.Center() - (spacing * Vector2D((float)(cols-1), (float)(rows-1)) / 2.f);
	pos += spacing * Vector2D((float)(unitCount % cols), (float)(unitCount / cols));

	map<string, GameObject*> objs;
	for (ObjData obj : objData) {
		GameObject *go = gom->AddNew(obj.archetype+"Archetype");
		go->SetName(obj.name+to_string(unitCount+1));
		go->GetComponent<Transform>()->SetTranslation(obj.pos + pos);
		objs[obj.name] = go;
	}

	objs["Name"]->GetComponent<TextBox>()->SetText(newUnit->name);

	CustomPointCounter *pointCounter = objs["PointCounter"]->GetComponent<CustomPointCounter>();
	pointCounter->SetPoints(25);

	CostDisplay *costDisplay = objs["CostDisplay"]->GetComponent<CostDisplay>();
	costDisplay->SetUnit(newUnit);

	Text *strengthText = objs["Strength"]->GetComponent<Text>();
	Text *agilityText = objs["Agility"]->GetComponent<Text>();
	Text *defenseText = objs["Defense"]->GetComponent<Text>();
	strengthText->SetText(string("Strength:")+to_string(newUnit->strength));
	agilityText->SetText(string("Agility:")+to_string(newUnit->agility));
	defenseText->SetText(string("Defense:")+to_string(newUnit->defense));

	objs["StrengthUp"]->GetComponent<ChangeStatButton>()->SetStat(&newUnit->strength, 1, strengthText, "Strength:", pointCounter);
	objs["StrengthDown"]->GetComponent<ChangeStatButton>()->SetStat(&newUnit->strength, -1, strengthText, "Strength:", pointCounter);
	objs["AgilityUp"]->GetComponent<ChangeStatButton>()->SetStat(&newUnit->agility, 1, agilityText, "Agility:", pointCounter);
	objs["AgilityDown"]->GetComponent<ChangeStatButton>()->SetStat(&newUnit->agility, -1, agilityText, "Agility:", pointCounter);
	objs["DefenseUp"]->GetComponent<ChangeStatButton>()->SetStat(&newUnit->defense, 1, defenseText, "Defense:", pointCounter);
	objs["DefenseDown"]->GetComponent<ChangeStatButton>()->SetStat(&newUnit->defense, -1, defenseText, "Defense:", pointCounter);

	objs["StrengthUp"]->GetComponent<Sprite>()->SetFrame(0);
	objs["StrengthDown"]->GetComponent<Sprite>()->SetFrame(1);
	objs["AgilityUp"]->GetComponent<Sprite>()->SetFrame(0);
	objs["AgilityDown"]->GetComponent<Sprite>()->SetFrame(1);
	objs["DefenseUp"]->GetComponent<Sprite>()->SetFrame(0);
	objs["DefenseDown"]->GetComponent<Sprite>()->SetFrame(1);

	objs["AbilityT"]->GetComponent<Text>()->SetText("Ability:");
	objs["WeaponT"]->GetComponent<Text>()->SetText("Weapon:");
	Text *abilityText = objs["AbilityV"]->GetComponent<Text>();
	Text *weaponText = objs["WeaponV"]->GetComponent<Text>();
	abilityText->SetText(BehaviorUnit::AbilityNames[newUnit->ability]);
	weaponText->SetText(BehaviorUnit::Weapons[newUnit->weapon].name);

	vector<string> abilities(BehaviorUnit::AbilityNames, BehaviorUnit::AbilityNames+cNumAbilities);
	vector<int> abilityCosts;
	abilityCosts.reserve(cNumAbilities);
	abilityCosts.push_back(0);
	for (int i = 1; i < cNumAbilities; i++)
		abilityCosts.push_back(5);
	vector<string> weapons;
	weapons.reserve(cNumWeapons);
	for (BehaviorUnit::Weapon &weapon : BehaviorUnit::Weapons)
		weapons.push_back(weapon.name);
	objs["AbilityNext"]->GetComponent<ChangeWeaponAbilityButton>()->SetStat(&newUnit->ability, 1, abilityText, abilities, abilityCosts, pointCounter);
	objs["AbilityPrev"]->GetComponent<ChangeWeaponAbilityButton>()->SetStat(&newUnit->ability, -1, abilityText, abilities, abilityCosts, pointCounter);
	objs["WeaponNext"]->GetComponent<ChangeWeaponAbilityButton>()->SetStat(&newUnit->weapon, 1, weaponText, weapons, {}, pointCounter);
	objs["WeaponPrev"]->GetComponent<ChangeWeaponAbilityButton>()->SetStat(&newUnit->weapon, -1, weaponText, weapons, {}, pointCounter);

	objs["AbilityNext"]->GetComponent<Sprite>()->SetFrame(3);
	objs["AbilityPrev"]->GetComponent<Sprite>()->SetFrame(2);
	objs["WeaponNext"]->GetComponent<Sprite>()->SetFrame(3);
	objs["WeaponPrev"]->GetComponent<Sprite>()->SetFrame(2);

	unitCount++;
}
