#include "Recovery.hpp"
#include "Recovery/HeistModifier.hpp"
#include "Recovery/StatEditor.hpp"
#include "Recovery/Transactions.hpp"
#include "Recovery/DailyActivities.hpp"
#include "game/frontend/items/Items.hpp"
#include "game/gta/ScriptGlobal.hpp"

#include "game/gta/Player.hpp"

namespace YimMenu::Submenus
{
	static bool g_isCheckingPlayer = false;
	static bool g_isCheckingServer = true;
	static int money = 0;

	static std::string FormatMoney(long long value)
	{
		std::stringstream ss;
		ss.imbue(std::locale("en_US.UTF-8"));
		ss << value;
		return ss.str();
	}

	Recovery::Recovery() :
#define ICON_FA_SACK_DOLLAR "\xef\x93\x80"
	    Submenu::Submenu("Recovery", ICON_FA_SACK_DOLLAR)
	{
		auto main = std::make_shared<Category>("Main");
		auto businesses = std::make_shared<Category>("Businesses");

		auto verification = std::make_shared<Group>("Verification");

		auto generalGroup = std::make_shared<Group>("General");

		generalGroup->AddItem(std::make_shared<BoolCommandItem>("playallmissionssolo"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("unlockgtaplus"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("overriderpmultiplier"_J));
		generalGroup->AddItem(std::make_shared<ConditionalItem>("overriderpmultiplier"_J, std::make_shared<FloatCommandItem>("rpmultiplierinput"_J)));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("freechangeappearance"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("nochangeappearancecooldown"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("allowgenderchange"_J));
		generalGroup->AddItem(std::make_shared<BoolCommandItem>("casinomanipulaterigslotmachines"_J));

		verification->AddItem(std::make_shared<ImGuiItem>([] {
			auto local_player = Player(static_cast<uint8_t>(0));
			ImGui::BeginDisabled(true);
			if (local_player.IsValid())
			{
				const char* username = local_player.GetName();
				if (money == 0)
					money = local_player.GetMoney();
				int rate = local_player.GetMoney() - money;
				std::string rateStr = FormatMoney(rate);
				if (rate == 0)
					rateStr = "";
				else if (rate > 0)
					rateStr = std::format(" +{}", rateStr);

				ImGui::Checkbox(std::format("Status: {}", username).c_str(), &g_isCheckingPlayer);
				ImGui::SameLine();
				ImGui::Checkbox(std::format("RID: {}", local_player.GetRID()).c_str(), &g_isCheckingServer);
				ImGui::SameLine();
				ImGui::Checkbox(std::format("Id Session: {}", local_player.GetId()).c_str(), &g_isCheckingServer);
				ImGui::Checkbox(std::format("Money: {} {}", FormatMoney(local_player.GetMoney()), rateStr).c_str(), &g_isCheckingServer);

				if (std::string(username) == "xBzTTx")
				{
					g_isCheckingPlayer = true;
				}
			}
			ImGui::EndDisabled();
			if (local_player.IsValid() && local_player.GetMoney() != money)
			{
				ImGui::SameLine();
				if (ImGui::Button("Reset"))
				{
					money = local_player.GetMoney();
				};
			}
		}));
		verification->AddItem(std::make_shared<ImGuiItem>([] {
			if (g_isCheckingPlayer)
				return;
			static char password_buf[64] = {};
			constexpr const char* SECRET = "thuan0132";
			ImGui::SetNextItemWidth(300.0f);
			ImGui::InputText("Password", password_buf, sizeof(password_buf), ImGuiInputTextFlags_Password);
			ImGui::SameLine();
			if (ImGui::Button("Unlock"))
			{
				if (std::string(password_buf) == SECRET)
				{
					g_isCheckingPlayer = true;
				}
			}
		}));

		businesses->AddItem(std::move(verification));

		auto claim = std::make_shared<Group>("Claim Safe", 1);
		claim->AddItem(std::make_shared<CommandItem>("claimsafearcade"_J));
		claim->AddItem(std::make_shared<CommandItem>("claimsafeagency"_J));
		claim->AddItem(std::make_shared<CommandItem>("claimsafebailoffice"_J));
		claim->AddItem(std::make_shared<CommandItem>("claimsafegarmentfactory"_J));
		claim->AddItem(std::make_shared<CommandItem>("claimsafesalvageyard"_J));
		claim->AddItem(std::make_shared<CommandItem>("claimsafehowc"_J));
		claim->AddItem(std::make_shared<CommandItem>("claimsafenightclub"_J));

		auto in_line = std::make_shared<Group>("By BnTT", 1);
		in_line->AddItem(std::make_shared<CommandItem>("resupply"_J));
		in_line->AddItem(std::make_shared<CommandItem>("ncpay"_J));
		in_line->AddItem(std::make_shared<CommandItem>("bunker"_J));
		in_line->AddItem(std::make_shared<CommandItem>("ride"_J));
		in_line->AddItem(std::make_shared<BoolCommandItem>("cooldown"_J));
		in_line->AddItem(std::make_shared<BoolCommandItem>("spin"_J));

		auto ceo_gp = std::make_shared<Group>("Warehouse", 1);
		ceo_gp->AddItem(std::make_shared<ListCommandItem>("whgoods"_J));
		ceo_gp->AddItem(std::make_shared<CommandItem>("ceo"_J));
		ceo_gp->AddItem(std::make_shared<CommandItem>("sellceo"_J));

		auto air_gp = std::make_shared<Group>("Hangar Warehouse", 1);
		air_gp->AddItem(std::make_shared<ListCommandItem>("hggoods"_J));
		air_gp->AddItem(std::make_shared<CommandItem>("air"_J));
		air_gp->AddItem(std::make_shared<CommandItem>("sellair"_J));

		auto clw_gp = std::make_shared<Group>("Casino Lucky Wheel", 1);
		clw_gp->AddItem(std::make_shared<ListCommandItem>("clws"_J));
		clw_gp->AddItem(std::make_shared<CommandItem>("clw"_J));

		auto ctms_gp = std::make_shared<Group>("Contact Mission", 1);
		ctms_gp->AddItem(std::make_shared<CommandItem>("vincent"_J));
		ctms_gp->AddItem(std::make_shared<CommandItem>("dre"_J));
		ctms_gp->AddItem(std::make_shared<CommandItem>("avi"_J));
		ctms_gp->AddItem(std::make_shared<CommandItem>("oscar"_J));
		ctms_gp->AddItem(std::make_shared<ListCommandItem>("hits"_J));
		ctms_gp->AddItem(std::make_shared<CommandItem>("phonehit"_J));

		businesses->AddItem(std::make_shared<ImGuiItem>([claim, in_line, ceo_gp, air_gp, clw_gp, ctms_gp] {
			ImGui::BeginDisabled(!g_isCheckingPlayer);
			claim->Draw();
			in_line->Draw();
			ctms_gp->Draw();
			ceo_gp->Draw();
			air_gp->Draw();
			clw_gp->Draw();
			ImGui::EndDisabled();
		}));

		main->AddItem(generalGroup);

		AddCategory(std::move(businesses));
		AddCategory(std::move(main));
		//AddCategory(std::move(casino));
		AddCategory(BuildStatEditorMenu());
		//AddCategory(BuildTransactionsMenu());
		AddCategory(BuildHeistModifierMenu());
		AddCategory(BuildDailyActivitiesMenu());
	}
}
