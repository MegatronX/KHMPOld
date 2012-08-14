#include "PlayerParty.h"

namespace Game
{
	namespace Character
	{
		std::string PlayerParty::pcompindex = "Player Component";

		PlayerParty::PlayerParty() : PartyInterface<BaseCharacter>("Player Party"), PlayerCharacterComponents("Player Character Components")
		{

		}
		void PlayerParty::AddCharacter(const std::string& name, boost::shared_ptr<BaseCharacter> character)
		{
			PartyInterface<BaseCharacter>::AddMember(character, name);
			auto ch = boost::shared_ptr<PlayerCharacterComponent>(new PlayerCharacterComponent(character.get()));
			character->RegisterComponent(pcompindex, ch);
			PlayerCharacterComponents.AddMember(ch, character->GetName(), true);
		}

		void PlayerParty::RemoveCharacter(const std::string& name)
		{
			PlayerCharacterComponents.RemoveMemberByName(name);
			auto finder =  PartyInterface<BaseCharacter>::GetPartyMembers().find(name);
			if (finder != PartyInterface<BaseCharacter>::GetPartyMembers().end())
			{
				auto ch = finder->second;
				ch->RemoveComponent(pcompindex);
				PartyInterface<BaseCharacter>::RemoveMemberByName(ch->GetName());
			}
		}

		PartyInterface<PlayerCharacterComponent> PlayerParty::GetPlayerCharacterParty()
		{
			return PlayerCharacterComponents;
		}
	}
}